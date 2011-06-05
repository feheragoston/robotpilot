/*
 * node_Magnet.cpp
 *
 *  Created on: Feb 17, 2011
 *      Author: triszalo
 */





#include "include/node_Magnet.h"

using namespace std;


node_Magnet::node_Magnet(void) : node(MAGNET_ID, "node_Magnet", MAGNET_KEEP_ALIVE_MS, MAGNET_SEND_PERIOD_TO_PC_MS, MAGNET_SEND_PERIOD_TO_NODE_MS){


	//----- valtozo init ELEJE -----
	//----- valtozo init VEGE -----


}


node_Magnet::~node_Magnet(){

}


void node_Magnet::evalMsg(UDPmsg* msg){

	u8 num;


	//ha o a cimzett, csak akkor dolgozza fel
	if(msg->node_id == id){

		switch(msg->function){

			case MSG_PING_REPLY:
				sem_post(&pingSemaphore);
				break;

			case MSG_INIT_PARAM_REPLY:
				initparamOK	= GET_BOOL(&(msg->data[0]), 0);
				sem_post(&initparamSemaphore);
				break;

			case MSG_MAGNET_SET_POLARITY_REPLY:
				num = GET_U8(&(msg->data[0]));
				SetPolarity[num].done = GET_BOOL(&(msg->data[1]), 0);
				SetPolarity[num].inProgress = false;
				SetPolarity[num].finished = true;
				cout << name << "\t___recv SETPOLARITY [" << (u32)num << "]___:\t" << (SetPolarity[num].done?"1":"0") << endl;
				break;

			default:
				break;


		}

	}

}


void node_Magnet::MAGNET_SET_POLARITY(u8 num, u8 polarity){

	UDPmsg msg;

	msg.node_id		= id;
	msg.function	= CMD_MAGNET_SET_POLARITY;
	msg.length		= 1;
	SET_U8(&(msg.data[0]), num);
	SET_U8(&(msg.data[1]), polarity);

	UDPdriver::send(&msg);

	SetPolarity[num].inProgress = true;
	SetPolarity[num].finished = false;

	cout << name << "\t___send SETPOLARITY [" << (u32)num << "]___:\t" << (s32)polarity << endl;

}


void node_Magnet::INIT_PARAM(void){

	UDPmsg msg;

	msg.node_id		= id;
	msg.function	= CMD_INIT_PARAM;
	msg.length		= 0;

	UDPdriver::send(&msg);

}
