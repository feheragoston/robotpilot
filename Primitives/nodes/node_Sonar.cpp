/*
 * node_Sonar.cpp
 *
 *  Created on: Feb 17, 2011
 *      Author: triszalo
 */





#include "include/node_Sonar.h"

using namespace std;


node_Sonar::node_Sonar(void) : node(SONAR_ID, "node_Sonar", SONAR_KEEP_ALIVE_MS, SONAR_SEND_PERIOD_TO_PC_MS, SONAR_SEND_PERIOD_TO_NODE_MS){


	//----- valtozo init ELEJE -----
	SonarPosX = 0;
	SonarPosY = 0;
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

			case MSG_INIT_PARAM_REPLY:
				initparamOK	= GET_BOOL(&(msg->data[0]), 0);
				sem_post(&initparamSemaphore);
				break;

			case MSG_PERIODIC_TO_PC:
				SonarPosX	= GET_FLOAT(&(msg->data[0]));
				SonarPosY	= GET_FLOAT(&(msg->data[4]));
				break;

			default:
				break;


		}

	}

}


void node_Sonar::INIT_PARAM(void){

	UDPmsg msg;

	msg.node_id		= id;
	msg.function	= CMD_INIT_PARAM;

	msg.length		= 0;

	UDPdriver::send(&msg);

}


void node_Sonar::GET_POS(double* x, double* y){

	*x = SonarPosX;
	*y = SonarPosY;

}