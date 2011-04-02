/*
 * node_Sonar.cpp
 *
 *  Created on: Feb 17, 2011
 *      Author: triszalo
 */





#include "include/node_Sonar.h"

using namespace std;


node_Sonar::node_Sonar(void){

	id = SONAR_ID;
	strcpy(name, "node_Sonar");


	//----- valtozo init ELEJE -----
	pos_x = 0;
	pos_y = 0;
	//----- valtozo init VEGE -----

}


node_Sonar::~node_Sonar(){

}


void node_Sonar::evalMsg(UDPmsg* msg){

	//ha o a cimzett, csak akkor dolgozza fel
	if(msg->node_id == id){

		switch(msg->function){

			case MSG_PING_REPLY:
				sem_post(&pingSemaphore);
				break;

			case MSG_POS:
				pos_x	= *(float*)(&(msg->data[0]));
				pos_y	= *(float*)(&(msg->data[4]));
				break;

			default:
				break;


		}

	}

}
