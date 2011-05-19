/*
 * UDPconn.cpp
 *
 *  Created on: Feb 13, 2011
 *      Author: triszalo
 */





#include "UDPconn.h"


using namespace std;


//----- valtozo ELEJE -----
char					UDPconn::TargetHost[16];

int						UDPconn::mSockFd;				//socket
fd_set					UDPconn::mGatewayInputset;		//input file descriptor set

struct sockaddr_in		UDPconn::addr;
struct hostent*			UDPconn::hp;

pthread_mutex_t			UDPconn::sendMutex;
//----- valtozo VEGE -----


bool UDPconn::init(const char* TargetHostIp){

	if(strcmp(TargetHostIp, "") == 0)	strcpy(TargetHost, DefaultTargetHost);
	else								strcpy(TargetHost, TargetHostIp);

	pthread_mutex_init(&sendMutex, NULL);

	return init_socket();

}


bool UDPconn::init_socket(void){

	//socket letrehozasa
	if( ( mSockFd = socket(AF_INET, SOCK_DGRAM, 0) ) == -1 ){
			cerr << "ERROR socket" << endl;
			return false;
	}


	//socket structure init
	bzero(&addr, sizeof(addr));
	addr.sin_family			= AF_INET;
	addr.sin_addr.s_addr	= INADDR_ANY;
	addr.sin_port			= htons(LocalPort);


	//socket bind
	if( bind(mSockFd, (struct sockaddr *) &addr, sizeof(addr)) == -1){
		cerr << "ERROR bind" << endl;
		return false;
	}


	//figyelendo filedescriptorok
	FD_ZERO(&mGatewayInputset);
	FD_SET(mSockFd, &mGatewayInputset);


	//minden OK
	return true;

}


bool UDPconn::recieve(void* buffer, u32 maxCount, u32* count, struct timeval* timeOut){

	//megnezzuk, hogy jott-e uzenet
	switch(select(1+mSockFd, &mGatewayInputset, NULL, NULL, timeOut)){

		//hiba
		case -1:
			cerr << "ERROR select" << endl;
			return false;

		//nem jott
		case 0:
			FD_ZERO(&mGatewayInputset);
			FD_SET(mSockFd, &mGatewayInputset);
			return false;

		//jott
		default:

			if(FD_ISSET(mSockFd, &mGatewayInputset)){

				*count = recv(mSockFd, buffer, maxCount, 0);

				FD_ZERO(&mGatewayInputset);
				FD_SET(mSockFd, &mGatewayInputset);

				return true;

			}

			return false;

	}

}


void UDPconn::send(void* buffer, u32 count){

	if ((hp = gethostbyname(TargetHost)) == NULL){
		cerr << "ERROR gethostbyname" << endl;
		return;
	}


	bzero(&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	memcpy(&addr.sin_addr, hp->h_addr, hp->h_length);
	addr.sin_port = htons(RemotePort);

	
	//----- critical ELEJE -----
	//lock
	pthread_mutex_lock(&sendMutex);

	//ha hiba a kuldesben
	if(sendto(mSockFd, buffer, count, 0, (struct sockaddr *)&addr, sizeof(addr)) == -1)
		cerr << "ERROR send" << endl;
		
	//unlock
	pthread_mutex_unlock(&sendMutex);
	//----- critical VEGE -----

}
