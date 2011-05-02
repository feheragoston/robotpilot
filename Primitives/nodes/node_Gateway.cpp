/*
 * node_Gateway.cpp
 *
 *  Created on: Feb 16, 2011
 *      Author: triszalo
 */





#include "include/node_Gateway.h"

using namespace std;


node_Gateway::node_Gateway(void){

	id = GATEWAY_ID;
	strcpy(name, "node_Gateway");

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

			default:
				break;


		}

	}

}


void node_Gateway::GATEWAY_REMOVE_NODE(unsigned int node_id){

	UDPmsg msg;

	msg.node_id		= id;
	msg.function	= CMD_GATEWAY_REMOVE_NODE;
	msg.length		= 1;
	msg.data[0]		= node_id;

	UDPdriver::send(&msg);

}


void node_Gateway::GATEWAY_ADD_NODE_CANA(unsigned int node_id){

	UDPmsg msg;

	msg.node_id		= id;
	msg.function	= CMD_GATEWAY_ADD_NODE_CANA;
	msg.length		= 1;
	msg.data[0]		= node_id;

	UDPdriver::send(&msg);

}


void node_Gateway::GATEWAY_ADD_NODE_CANB(unsigned int node_id){

	UDPmsg msg;

	msg.node_id		= id;
	msg.function	= CMD_GATEWAY_ADD_NODE_CANB;
	msg.length		= 1;
	msg.data[0]		= node_id;

	UDPdriver::send(&msg);

}


void node_Gateway::GATEWAY_ADD_BOOTLOADER_ID(unsigned int node_id){

	UDPmsg msg;

	msg.node_id		= id;
	msg.function	= CMD_GATEWAY_ADD_BOOTLOADER_ID;
	msg.length		= 1;
	msg.data[0]		= node_id;

	UDPdriver::send(&msg);

}
