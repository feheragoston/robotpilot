/*
 * node_Magnet.cpp
 *
 *  Created on: Feb 17, 2011
 *      Author: triszalo
 */





#include "include/node_Magnet.h"

using namespace std;


node_Magnet::node_Magnet(void){

	id = MAGNET_ID;
	strcpy(name, "node_Magnet");


	//----- valtozo init ELEJE -----
	for(unsigned int i=0 ; i<MAGNET_COUNT ; i++)
		set_polarity_inProgress[i] = false;

	for(unsigned int i=0 ; i<MAGNET_COUNT ; i++)
		set_polarity_finished[i] = false;
	//----- valtozo init VEGE -----

}


node_Magnet::~node_Magnet(){

}


void node_Magnet::evalMsg(UDPmsg* msg){

	unsigned char num;


	//ha o a cimzett, csak akkor dolgozza fel
	if(msg->node_id == id){

		switch(msg->function){

			case MSG_PING_REPLY:
				sem_post(&pingSemaphore);
				break;

			case MSG_MAGNET_SET_POLARITY_REPLY:
				num = (unsigned char)(msg->data[0]);
				set_polarity_inProgress[num] = false;
				set_polarity_finished[num] = true;
				break;

			default:
				break;


		}

	}

}


void node_Magnet::MAGNET_SET_POLARITY(unsigned int num, char polarity){

	UDPmsg msg;

	msg.node_id		= id;
	msg.function	= CMD_MAGNET_SET_POLARITY;

	msg.length		= 1;
	msg.data[0] = num;
	msg.data[1] = polarity;

	UDPdriver::send(&msg);

	set_polarity_inProgress[num] = true;
	set_polarity_finished[num] = false;

}
