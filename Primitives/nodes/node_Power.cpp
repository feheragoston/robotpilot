/*
 * node_Power.cpp
 *
 *  Created on: Feb 17, 2011
 *      Author: triszalo
 */





#include "include/node_Power.h"

using namespace std;


node_Power::node_Power(void){

	id = POWER_ID;
	strcpy(name, "node_Power");


	//----- valtozo init ELEJE -----
	StopButton = false;

	motor_on_off_inProgress		= false;

	motor_on_off_finished		= false;
	//----- valtozo init VEGE -----

}


node_Power::~node_Power(){

}


void node_Power::evalMsg(UDPmsg* msg){

	//ha o a cimzett, csak akkor dolgozza fel
	if(msg->node_id == id){

		switch(msg->function){

			case MSG_PING_REPLY:
				sem_post(&pingSemaphore);
				break;

			case MSG_POWER_STOP_BUTTON:
				StopButton = ((msg->data[0] & 0x01)	? true : false);
				break;

			case MSG_POWER_MOTOR_ON_REPLY:
			case MSG_POWER_MOTOR_OFF_REPLY:
				motor_on_off_inProgress = false;
				motor_on_off_finished = true;
				break;

			default:
				break;


		}

	}

}


void node_Power::POWER_MOTOR_ON(void){

	UDPmsg msg;

	msg.node_id		= id;
	msg.function	= CMD_POWER_MOTOR_ON;
	msg.length		= 0;

	UDPdriver::send(&msg);

	motor_on_off_inProgress = true;
	motor_on_off_finished = false;

}


void node_Power::POWER_MOTOR_OFF(void){

	UDPmsg msg;

	msg.node_id		= id;
	msg.function	= CMD_POWER_MOTOR_OFF;
	msg.length		= 0;

	UDPdriver::send(&msg);

	motor_on_off_inProgress = true;
	motor_on_off_finished = false;

}
