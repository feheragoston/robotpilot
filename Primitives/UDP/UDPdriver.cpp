/*
 * UDPdriver.cpp
 *
 *  Created on: Feb 14, 2011
 *      Author: triszalo
 */





#include "UDPdriver.h"


//----- valtozo ELEJE -----
char					UDPdriver::recieveBuffer[RECIEVE_BUFFER_SIZE];
unsigned int			UDPdriver::recieveBufferNextIndex;
unsigned int			UDPdriver::recieveBufferCount;
//----- valtozo VEGE -----


void UDPdriver::init(void){

	recieveBufferCount = 0;

}


void UDPdriver::getMsg(UDPmsg* msg){

	msg->node_id	= recieveBuffer[recieveBufferNextIndex + 0];
	msg->function	= recieveBuffer[recieveBufferNextIndex + 1];
	msg->length		= recieveBuffer[recieveBufferNextIndex + 2];

	memcpy(msg->data, &(recieveBuffer[recieveBufferNextIndex + 3]), msg->length);

	recieveBufferCount -= msg->length + 3;
	recieveBufferNextIndex += msg->length + 3;

}


bool UDPdriver::recieve(UDPmsg* msg, __time_t sec, __suseconds_t usec){

	struct timeval		timeOut;
	struct timeval*		timeOut_param;


	//ennyit fogunk aludni, amig nem jon semmi
	timeOut.tv_sec	= sec;
	timeOut.tv_usec	= usec;


	//ha 0-k, akkor blokkolosan varunk
	if((sec == 0) && (usec == 0))
		timeOut_param = NULL;
	else
		timeOut_param = &timeOut;


	//ha van a bufferben uzenet
	if(recieveBufferCount > 0){

		getMsg(msg);

		return true;

	}

	//ha nincs a bufferben, de jott uzenet
	else if(UDPconn::recieve(recieveBuffer, RECIEVE_BUFFER_SIZE, &recieveBufferCount, timeOut_param)){

		recieveBufferNextIndex = 0;

		getMsg(msg);

		return true;

	}

	//ha nincs a bufferben, es nem is jott uzenet
	else
		return false;


}


void UDPdriver::send(UDPmsg* msg){

	char	sendBuffer[MAX_MSG_LENGTH];


	sendBuffer[0] = msg->node_id;
	sendBuffer[1] = msg->function;
	sendBuffer[2] = msg->length;

	memcpy(&(sendBuffer[3]), msg->data, msg->length);

	UDPconn::send(sendBuffer, msg->length + 3);

}
