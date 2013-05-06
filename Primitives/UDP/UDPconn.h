/*
 * UDPconn.h
 *
 *  Created on: Feb 13, 2011
 *      Author: triszalo
 */


#ifndef UDPCONN_H_
#define UDPCONN_H_





//------------------------------ include ELEJE ------------------------------
#include <iostream>

#include <netdb.h>
#include <string.h>

#include <pthread.h>

#include "../define/type.h"
//------------------------------ include VEGE ------------------------------





//------------------------------ define ELEJE ------------------------------
#define DefaultLocalIP				"192.168.131.121"	//nem használjuk, csak hogy tudjuk, hogy hova küld a remote
#define DefaultTargetHost			"192.168.131.111"
//#define DefaultTargetHost			"127.0.0.1"
#define RemotePort					10001
#define LocalPort					10002
//------------------------------ define VEGE ------------------------------





//------------------------------ UDPconn ELEJE ------------------------------
class UDPconn{

public:

	static bool init(const char* TargetHostIp);
	
	static bool recieve(void* buffer, u32 maxCount, u32* count, struct timeval* timeOut);
	static void send(void* buffer, u32 count);


private:

	static bool init_socket(void);
	
	static char					TargetHost[16];

	static int					mSockFd;				//socket
	static fd_set				mGatewayInputset;		//input file descriptor set

	static struct sockaddr_in	addr;
	static struct hostent*		hp;
	
	static pthread_mutex_t		sendMutex;

};
//------------------------------ UDPconn VEGE ------------------------------





#endif /*UDPCONN_H_*/
