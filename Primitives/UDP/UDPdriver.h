/*
 * UDPdriver.h
 *
 *  Created on: Feb 14, 2011
 *      Author: triszalo
 */


#ifndef UDPDRIVER_H_
#define UDPDRIVER_H_





//------------------------------ include ELEJE ------------------------------
#include "UDPconn.h"
#include "conv.h"
//------------------------------ include VEGE ------------------------------





//------------------------------ valtoztathato ELEJE ------------------------------
#define MAX_MSG_LENGTH				128
#define RECIEVE_BUFFER_SIZE			1024
//------------------------------ valtoztathato VEGE ------------------------------





//------------------------------ UDPmsg ELEJE ------------------------------
typedef struct{
	u8	node_id;
	u8	function;
	u8	data[MAX_MSG_LENGTH];
	u8	length;
}UDPmsg;
//------------------------------ UDPmsg VEGE ------------------------------





//------------------------------ UDPdriver ELEJE ------------------------------
class UDPdriver{

public:

	static void init(void);

	static bool recieve(UDPmsg* msg, __time_t sec, __suseconds_t usec);
	static void send(UDPmsg* msg);


private:

	static void getMsg(UDPmsg* msg);

	static u8		recieveBuffer[RECIEVE_BUFFER_SIZE];
	static u32		recieveBufferNextIndex;
	static u32		recieveBufferCount;

};
//------------------------------ UDPdriver VEGE ------------------------------





#endif /*UDPDRIVER_H_*/
