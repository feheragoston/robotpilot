/*
 * node_Input.cpp
 *
 *  Created on: Feb 17, 2011
 *      Author: triszalo
 */





#include "include/node_Input.h"

using namespace std;


node_Input::node_Input(void){

	id = INPUT_ID;
	strcpy(name, "node_Input");


	//----- valtozo init ELEJE -----
	for(unsigned int i=0 ; i<DISTANCE_COUNT ; i++)
		distance_mm[i] = 0;

	for(unsigned int i=0 ; i<BUTTON_COUNT ; i++)
		button[i] = false;
	//----- valtozo init VEGE -----

}


node_Input::~node_Input(){

}


void node_Input::evalMsg(UDPmsg* msg){

	//ha o a cimzett, csak akkor dolgozza fel
	if(msg->node_id == id){

		switch(msg->function){

			case MSG_PING_REPLY:
				sem_post(&pingSemaphore);
				break;

			case MSG_DISTANCE:
				for(unsigned int i=0 ; i<DISTANCE_COUNT ; i++)
					distance_mm[i] = distanceMultiplier * (double)(*(unsigned short*)(&(msg->data[2*i])));
				break;

			case MSG_BUTTON:
				for(unsigned int i=0 ; i<BUTTON_COUNT ; i++)
					button[i] = ((msg->data[0] & (0x01 << i)) ? true : false);
				break;

			default:
				break;


		}

	}

}
