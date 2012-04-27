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
	for(u8 i=0 ; i<SONAR_TRANSMITTER_COUNT ; i++){
		SonarPosX[i] = -1000;
		SonarPosY[i] = -1000;
	}
	gettimeofday(&prevMsgTime, NULL);
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

				//ASCII-ban kuldi
				double* val;

				val = &(SonarPosX[0]);
				(*val) = 0;
				for(u8 i=0 ; i<4 ; i++){
					(*val) *= 10;
					(*val) += GET_U8(&(msg->data[i])) - '0';
				}

				val = &(SonarPosY[0]);
				(*val) = 0;
				for(u8 i=0 ; i<4 ; i++){
					(*val) *= 10;
					(*val) += GET_U8(&(msg->data[i+4])) - '0';
				}

				val = &(SonarPosX[1]);
				(*val) = 0;
				for(u8 i=0 ; i<4 ; i++){
					(*val) *= 10;
					(*val) += GET_U8(&(msg->data[i])) - '0';
				}

				val = &(SonarPosY[1]);
				(*val) = 0;
				for(u8 i=0 ; i<4 ; i++){
					(*val) *= 10;
					(*val) += GET_U8(&(msg->data[i+4])) - '0';
				}

				gettimeofday(&prevMsgTime, NULL);

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


long int node_Sonar::GET_POS(u8 num, double* x, double* y){

	struct timeval timeElapsed;

	*x = SonarPosX[num];
	*y = SonarPosY[num];

	TimeMeasure(&prevMsgTime, &timeElapsed);

	return ((double)timeElapsed.tv_sec*1000 + (double)timeElapsed.tv_usec/1000);

}
