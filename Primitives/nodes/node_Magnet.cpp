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
		setmagnet_inProgress[i] = false;

	for(unsigned int i=0 ; i<MAGNET_COUNT ; i++)
		setmagnet_finished[i] = false;
	//----- valtozo init VEGE -----

}


node_Magnet::~node_Magnet(){

}


void node_Magnet::evalMsg(UDPmsg* msg){

	//ha o a cimzett, csak akkor dolgozza fel
	if(msg->node_id == id){

		switch(msg->function){

			case MSG_PING_REPLY:
				sem_post(&pingSemaphore);
				break;

			case MSG_SET_MAGNET0_REPLY:
				setmagnet_inProgress[0] = false;
				setmagnet_finished[0] = true;
				break;

			case MSG_SET_MAGNET1_REPLY:
				setmagnet_inProgress[1] = false;
				setmagnet_finished[1] = true;
				break;

			default:
				break;


		}

	}

}


void node_Magnet::SET_MAGNET(unsigned int num, int polarity){

	UDPmsg msg;

	msg.node_id		= id;

	switch(num){

		case 0:
			msg.function	= CMD_SET_MAGNET0;
			break;

		case 1:
			msg.function	= CMD_SET_MAGNET1;
			break;

		default:	//nem lehet, rossz parameter
			return;

	}

	msg.length		= 1;

	switch(polarity){

		case 1:
			msg.data[0] = polarityPull;
			break;

		case 0:
			msg.data[0] = polarityOff;
			break;

		case -1:
			msg.data[0] = polarityPush;
			break;

		default:	//nem lehet, rossz parameter
			return;

	}

	UDPdriver::send(&msg);

	setmagnet_inProgress[num] = true;
	setmagnet_finished[num] = false;

}
