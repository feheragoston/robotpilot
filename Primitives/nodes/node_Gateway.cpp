/*
 * node_Gateway.cpp
 *
 *  Created on: Feb 16, 2011
 *      Author: triszalo
 */





#include "include/node_Gateway.h"

using namespace std;


node_Gateway::node_Gateway(void) : node(GATEWAY_ID, "node_Gateway"){


	//----- valtozo init ELEJE -----
	//----- valtozo init VEGE -----


}


node_Gateway::~node_Gateway(){

}


void node_Gateway::evalMsg(UDPmsg* msg){

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

			default:
				break;


		}

	}

}


void node_Gateway::GATEWAY_REMOVE_NODE(u8 node_id){

	UDPmsg msg;

	msg.node_id		= id;
	msg.function	= CMD_GATEWAY_REMOVE_NODE;
	msg.length		= 1;
	SET_U8(&(msg.data[0]), node_id);

	UDPdriver::send(&msg);

}


void node_Gateway::GATEWAY_ADD_NODE_CANA(u8 node_id){

	UDPmsg msg;

	msg.node_id		= id;
	msg.function	= CMD_GATEWAY_ADD_NODE_CANA;
	msg.length		= 1;
	SET_U8(&(msg.data[0]), node_id);

	UDPdriver::send(&msg);

}


void node_Gateway::GATEWAY_ADD_NODE_CANB(u8 node_id){

	UDPmsg msg;

	msg.node_id		= id;
	msg.function	= CMD_GATEWAY_ADD_NODE_CANB;
	msg.length		= 1;
	SET_U8(&(msg.data[0]), node_id);

	UDPdriver::send(&msg);

}


void node_Gateway::GATEWAY_ADD_BOOTLOADER_ID(u8 node_id){

	UDPmsg msg;

	msg.node_id		= id;
	msg.function	= CMD_GATEWAY_ADD_BOOTLOADER_ID;
	msg.length		= 1;
	SET_U8(&(msg.data[0]), node_id);

	UDPdriver::send(&msg);

}


void node_Gateway::INIT_PARAM(void){

	UDPmsg msg;

	msg.node_id		= id;
	msg.function	= CMD_INIT_PARAM;
	msg.length		= 0;

	UDPdriver::send(&msg);

}
