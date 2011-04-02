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
//------------------------------ include VEGE ------------------------------





//------------------------------ define ELEJE ------------------------------
#define DefaultTargetHost			"192.168.131.111"
//#define DefaultTargetHost			"127.0.0.1"
#define RemotePort					10001
#define LocalPort					10002
//------------------------------ define VEGE ------------------------------





//------------------------------ UDPconn ELEJE ------------------------------
class UDPconn{

public:

	static void init(const char* TargetHostIp);
	
	static bool recieve(void* buffer, unsigned int maxCount, unsigned int* count, struct timeval* timeOut);
	static void send(void* buffer, unsigned int count);


private:

	static void init_socket(void);
	
	static char					TargetHost[16];

	static int					mSockFd;				//socket
	static fd_set				mGatewayInputset;		//input file descriptor set

	static struct sockaddr_in	addr;
	static struct hostent*		hp;
	
	static pthread_mutex_t		sendMutex;

};
//------------------------------ UDPconn VEGE ------------------------------





#endif /*UDPCONN_H_*/
