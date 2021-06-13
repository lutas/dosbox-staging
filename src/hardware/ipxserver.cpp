/*
 *  Copyright (C) 2002-2021  The DOSBox Team
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#include "dosbox.h"

#if C_IPX

#include <cstdlib>
#include <cstring>
#include <atomic>
#include "ipxserver.h"
#include "timer.h"
#include "ipx.h"

constexpr int UDP_UNICAST = -1; // SDLNet magic number

IPaddress ipxServerIp;  // IPAddress for server's listening port
UDPsocket ipxServerSocket;  // Listening server socket

packetBuffer connBuffer[SOCKETTABLESIZE];

Bit8u inBuffer[IPXBUFFERSIZE];
IPaddress ipconn[SOCKETTABLESIZE];  // Active TCP/IP connection
UDPsocket tcpconn[SOCKETTABLESIZE]; // Active TCP/IP connections
SDLNet_SocketSet serverSocketSet;
SDL_Thread *serverThread = nullptr;
SDL_mutex *serverMutex = nullptr;
std::atomic_bool terminateServer;

Bit8u packetCRC(Bit8u *buffer, Bit16u bufSize) {
	Bit8u tmpCRC = 0;
	Bit16u i;
	for(i=0;i<bufSize;i++) {
		tmpCRC ^= *buffer;
		buffer++;
	}
	return tmpCRC;
}

/*
static void closeSocket(Bit16u sockidx) {
	Bit32u host;

	host = ipconn[sockidx].host;
	LOG_MSG("IPXSERVER: %d.%d.%d.%d disconnected", CONVIP(host));

	SDLNet_TCP_DelSocket(serverSocketSet,tcpconn[sockidx]);
	SDLNet_TCP_Close(tcpconn[sockidx]);
	connBuffer[sockidx].connected = false;
	connBuffer[sockidx].waitsize = false;
}
*/

static void sendIPXPacket(Bit8u *buffer, Bit16s bufSize) {
	Bit16u srcport, destport;
	Bit32u srchost, desthost;
	UDPpacket outPacket;
	outPacket.channel = -1;
	outPacket.data = buffer;
	outPacket.len = bufSize;
	outPacket.maxlen = bufSize;
	IPXHeader *tmpHeader;
	tmpHeader = (IPXHeader *)buffer;

	srchost = tmpHeader->src.addr.byIP.host;
	desthost = tmpHeader->dest.addr.byIP.host;

	srcport = tmpHeader->src.addr.byIP.port;
	destport = tmpHeader->dest.addr.byIP.port;

	if(desthost == 0xffffffff) {
		// Broadcast
		for (uint16_t i = 0; i < SOCKETTABLESIZE; ++i) {
			if(connBuffer[i].connected && ((ipconn[i].host != srchost)||(ipconn[i].port!=srcport))) {
				outPacket.address = ipconn[i];
				const int result = SDLNet_UDP_Send(ipxServerSocket,
				                                   UDP_UNICAST,
				                                   &outPacket);
				if (result == 0) {
					LOG_MSG("IPXSERVER: %s", SDLNet_GetError());
					continue;
				}
				//LOG_MSG("IPXSERVER: Packet of %d bytes sent from %d.%d.%d.%d to %d.%d.%d.%d (BROADCAST) (%x CRC)", bufSize, CONVIP(srchost), CONVIP(ipconn[i].host), packetCRC(&buffer[30], bufSize-30));
			}
		}
	} else {
		// Specific address
		for (uint16_t i = 0; i < SOCKETTABLESIZE; ++i) {
			if((connBuffer[i].connected) && (ipconn[i].host == desthost) && (ipconn[i].port == destport)) {
				outPacket.address = ipconn[i];
				const int result = SDLNet_UDP_Send(ipxServerSocket,
				                                   UDP_UNICAST,
				                                   &outPacket);
				if (result == 0) {
					LOG_MSG("IPXSERVER: %s", SDLNet_GetError());
					continue;
				}
				//LOG_MSG("IPXSERVER: Packet sent from %d.%d.%d.%d to %d.%d.%d.%d", CONVIP(srchost), CONVIP(desthost));
			}
		}
	}
}

bool IPX_isConnectedToServer(Bits tableNum, IPaddress ** ptrAddr) {
	if(tableNum >= SOCKETTABLESIZE) return false;
	SDL_LockMutex(serverMutex);
	*ptrAddr = &ipconn[tableNum];
	bool connected = connBuffer[tableNum].connected;
	SDL_UnlockMutex(serverMutex);
	return connected;
}

static void ackClient(IPaddress clientAddr) {
	IPXHeader regHeader;
	UDPpacket regPacket;

	SDLNet_Write16(0xffff, regHeader.checkSum);
	SDLNet_Write16(sizeof(regHeader), regHeader.length);

	SDLNet_Write32(0, regHeader.dest.network);
	PackIP(clientAddr, &regHeader.dest.addr.byIP);
	SDLNet_Write16(0x2, regHeader.dest.socket);

	SDLNet_Write32(1, regHeader.src.network);
	PackIP(ipxServerIp, &regHeader.src.addr.byIP);
	SDLNet_Write16(0x2, regHeader.src.socket);
	regHeader.transControl = 0;

	regPacket.data = (Uint8 *)&regHeader;
	regPacket.len = sizeof(regHeader);
	regPacket.maxlen = sizeof(regHeader);
	regPacket.address = clientAddr;
	// Send registration string to client.  If client doesn't get this, client will not be registered
	const int result = SDLNet_UDP_Send(ipxServerSocket, UDP_UNICAST, &regPacket);
	if (result == 0)
		LOG_MSG("IPXSERVER: Connection response not sent: %s",
		        SDLNet_GetError());
}

static int IPX_ServerLoop(MAYBE_UNUSED void *ptr) {
	UDPpacket inPacket;
	IPaddress tmpAddr;

	//char regString[] = "IPX Register\0";

	Bit32u host;

	inPacket.channel = -1;
	inPacket.data = &inBuffer[0];
	inPacket.maxlen = IPXBUFFERSIZE;

	while (1) {
		if (terminateServer) return 0;		
		int result = SDLNet_UDP_Recv(ipxServerSocket, &inPacket);
		assert(result != -1);
		if (result == 0) {
			Delay(1);
			continue;
		}
		bool isRegPacket = false;
		{
			// Check to see if incoming packet is a registration packet
			// For this, I just spoofed the echo protocol packet designation 0x02
			IPXHeader *tmpHeader;
			tmpHeader = (IPXHeader *)&inBuffer[0];

			// Check to see if echo packet
			SDL_LockMutex(serverMutex);
			if(SDLNet_Read16(tmpHeader->dest.socket) == 0x2) {
				// Null destination node means its a server registration packet
				if(tmpHeader->dest.addr.byIP.host == 0x0) {
					UnpackIP(tmpHeader->src.addr.byIP, &tmpAddr);
					for (uint16_t i = 0; i < SOCKETTABLESIZE; ++i) {
						if(!connBuffer[i].connected) {
							// Use prefered host IP rather than the reported source IP
							// It may be better to use the reported source
							ipconn[i] = inPacket.address;

							connBuffer[i].connected = true;
							host = ipconn[i].host;
							LOG_MSG("IPXSERVER: Connect from %d.%d.%d.%d", CONVIP(host));
							ackClient(inPacket.address);
							isRegPacket = true;
							break;
						} else {
							if((ipconn[i].host == tmpAddr.host) && (ipconn[i].port == tmpAddr.port)) {

								LOG_MSG("IPXSERVER: Reconnect from %d.%d.%d.%d", CONVIP(tmpAddr.host));
								// Update anonymous port number if changed
								ipconn[i].port = inPacket.address.port;
								ackClient(inPacket.address);
								isRegPacket = true;
								break;
							}
						}
					}
				}
			}

			// IPX packet is complete.  Now interpret IPX header and send to respective IP address
			if (!isRegPacket) sendIPXPacket((Bit8u *)inPacket.data, inPacket.len);
			SDL_UnlockMutex(serverMutex);
		}
	}
}

void IPX_StopServer() {
	int ts;
	terminateServer = true;
	SDL_WaitThread(serverThread, &ts);
	SDLNet_UDP_Close(ipxServerSocket);
}

bool IPX_StartServer(uint16_t portnum)
{
	if (!SDLNet_ResolveHost(&ipxServerIp, nullptr, portnum)) {
		//serverSocketSet = SDLNet_AllocSocketSet(SOCKETTABLESIZE);
		ipxServerSocket = SDLNet_UDP_Open(portnum);
		if(!ipxServerSocket) return false;

		for (uint16_t i = 0; i < SOCKETTABLESIZE; ++i)
			connBuffer[i].connected = false;	
		serverMutex = SDL_CreateMutex();
		assert(serverMutex);
		terminateServer = false;
		serverThread = SDL_CreateThread(IPX_ServerLoop, "IPX_SERVER", nullptr);
		assert(serverThread);
		return true;
	}
	return false;
}

#endif
