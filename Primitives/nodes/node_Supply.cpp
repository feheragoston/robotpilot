/*
 * node_Supply.cpp
 *
 *  Created on: Feb 17, 2011
 *      Author: triszalo
 */





#include "include/node_Supply.h"

using namespace std;


node_Supply::node_Supply(void){

	id = SUPPLY_ID;
	strcpy(name, "node_Supply");


	//----- valtozo init ELEJE -----
	StopButton = false;

	motor_inProgress	= false;

	motor_finished		= false;
	//----- valtozo init VEGE -----

}


node_Supply::~node_Supply(){

}


void node_Supply::evalMsg(UDPmsg* msg){

	//ha o a cimzett, csak akkor dolgozza fel
	if(msg->node_id == id){

		switch(msg->function){

			case MSG_PING_REPLY:
				sem_post(&pingSemaphore);
				break;

			case MSG_STOP_BUTTON:
				StopButton = ((msg->data[0] & 0x01)	? true : false);
				break;

			case MSG_MOTOR_REPLY:
				motor_inProgress = false;
				motor_finished = true;
				break;

			default:
				break;


		}

	}

}


void node_Supply::MOTOR(bool powered){

	UDPmsg msg;

	msg.node_id		= id;

	if(powered)
		msg.function	= CMD_MOTOR_ON;
	else
		msg.function	= CMD_MOTOR_OFF;

	msg.length		= 0;

	UDPdriver::send(&msg);

	motor_inProgress = true;
	motor_finished = false;

}
