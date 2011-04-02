/*
 * node_Console.cpp
 *
 *  Created on: Feb 17, 2011
 *      Author: triszalo
 */





#include "include/node_Console.h"

using namespace std;


node_Console::node_Console(void){

	id = CONSOLE_ID;
	strcpy(name, "node_Console");


	//----- valtozo init ELEJE -----
	pos = 0;

	stop_inProgress			= false;
	setpos_inProgress		= false;
	calibrate_inProgress	= false;

	stop_finished			= false;
	setpos_finished			= false;
	calibrate_finished		= false;
	//----- valtozo init VEGE -----

}


node_Console::~node_Console(){

}


void node_Console::evalMsg(UDPmsg* msg){

	//ha o a cimzett, csak akkor dolgozza fel
	if(msg->node_id == id){

		switch(msg->function){

			case MSG_PING_REPLY:
				sem_post(&pingSemaphore);
				break;

			case MSG_POS:
				pos	= *(float*)(&(msg->data[0]));
				break;

			case MSG_STOP_REPLY:
				stop_inProgress = false;
				stop_finished = true;
				break;

			case MSG_SET_POS_REPLY:
				setpos_inProgress = false;
				setpos_finished = true;
				break;

			case MSG_CALIBRATE_REPLY:
				calibrate_inProgress = false;
				calibrate_finished = true;
				break;

			default:
				break;


		}

	}

}


void node_Console::STOP(void){

	UDPmsg msg;

	msg.node_id		= id;
	msg.function	= CMD_STOP;
	msg.length		= 0;

	UDPdriver::send(&msg);

	stop_inProgress = true;
	stop_finished = false;

}


void node_Console::SET_POS(double pos, double speed, double acc){

	float*	tmp;

	UDPmsg msg;

	msg.node_id		= id;
	msg.function	= CMD_SET_POS;
	msg.length		= 12;
	tmp = (float*)(&(msg.data[0]));		*tmp = (float)pos;
	tmp = (float*)(&(msg.data[4]));		*tmp = (float)speed;
	tmp = (float*)(&(msg.data[8]));		*tmp = (float)acc;

	UDPdriver::send(&msg);

	setpos_inProgress = true;
	setpos_finished = false;

}


void node_Console::CALIBRATE(void){

	UDPmsg msg;

	msg.node_id		= id;
	msg.function	= CMD_CALIBRATE;
	msg.length		= 0;

	UDPdriver::send(&msg);

	calibrate_inProgress = true;
	calibrate_finished = false;

}
