/*
 * node_Drive.cpp
 *
 *  Created on: Feb 17, 2011
 *      Author: triszalo
 */





#include "include/node_Drive.h"

using namespace std;


node_Drive::node_Drive(void){

	id = BDC_ID;
	strcpy(name, "node_Drive");


	//----- valtozo init ELEJE -----
	stop_inProgress			= false;
	go_inProgress			= false;
	goto_inProgress			= false;
	turn_inProgress			= false;
	setspeed_inProgress		= false;

	stop_finished			= false;
	go_finished				= false;
	goto_finished			= false;
	turn_finished			= false;
	setspeed_finished		= false;
	//----- valtozo init VEGE -----

}


node_Drive::~node_Drive(){

}


void node_Drive::evalMsg(UDPmsg* msg){

	//ha o a cimzett, csak akkor dolgozza fel
	if(msg->node_id == id){

		switch(msg->function){

			case MSG_PING_REPLY:
				sem_post(&pingSemaphore);
				break;

			case MSG_STOP_REPLY:
				stop_inProgress = false;
				stop_finished = true;
				break;

			case MSG_GO_REPLY:
				go_inProgress = false;
				go_finished = true;
				break;

			case MSG_GOTO_REPLY:
				goto_inProgress = false;
				goto_finished = true;
				break;

			case MSG_TURN_REPLY:
				turn_inProgress = false;
				turn_finished = true;
				break;

			case MSG_SET_SPEED_REPLY:
				setspeed_inProgress = false;
				setspeed_finished = true;
				break;

			default:
				break;


		}

	}

}


void node_Drive::STOP(double acc){

	float*	tmp;

	UDPmsg msg;

	msg.node_id		= id;

	//HARD_STOP
	if(acc == 0){
		msg.node_id		= id;
		msg.function	= CMD_HARD_STOP;
		msg.length		= 0;
	}

	//STOP
	else{
		msg.function	= CMD_STOP;
		msg.length		= 4;
		tmp = (float*)(&(msg.data[0]));		*tmp = (float)acc;
	}

	UDPdriver::send(&msg);

	stop_inProgress = true;
	stop_finished = false;

}


void node_Drive::GO(double distance, double max_speed, double max_acc){

	float*	tmp;

	UDPmsg msg;

	msg.node_id		= id;
	msg.function	= CMD_GO;
	msg.length		= 12;
	tmp = (float*)(&(msg.data[0]));		*tmp = (float)distance;
	tmp = (float*)(&(msg.data[4]));		*tmp = (float)max_speed;
	tmp = (float*)(&(msg.data[8]));		*tmp = (float)max_acc;

	UDPdriver::send(&msg);

	go_inProgress = true;
	go_finished = false;

}


void node_Drive::GOTO(double x, double y, double max_speed, double max_acc){

	float*	tmp;

	UDPmsg msg;

	msg.node_id		= id;
	msg.function	= CMD_GOTO;
	msg.length		= 16;
	tmp = (float*)(&(msg.data[0]));		*tmp = (float)x;
	tmp = (float*)(&(msg.data[4]));		*tmp = (float)y;
	tmp = (float*)(&(msg.data[8]));		*tmp = (float)max_speed;
	tmp = (float*)(&(msg.data[12]));	*tmp = (float)max_acc;

	UDPdriver::send(&msg);

	goto_inProgress = true;
	goto_finished = false;

}


void node_Drive::TURN(double angle, double max_speed, double max_acc){

	float*	tmp;

	UDPmsg msg;

	msg.node_id		= id;
	msg.function	= CMD_TURN;
	msg.length		= 12;
	tmp = (float*)(&(msg.data[0]));		*tmp = (float)angle;
	tmp = (float*)(&(msg.data[4]));		*tmp = (float)max_speed;
	tmp = (float*)(&(msg.data[8]));		*tmp = (float)max_acc;

	UDPdriver::send(&msg);

	turn_inProgress = true;
	turn_finished = false;

}


void node_Drive::SET_SPEED(double v, double w){

	float*	tmp;

	UDPmsg msg;

	msg.node_id		= id;
	msg.function	= CMD_SET_SPEED;
	msg.length		= 8;
	tmp = (float*)(&(msg.data[0]));		*tmp = (float)v;
	tmp = (float*)(&(msg.data[4]));		*tmp = (float)w;

	UDPdriver::send(&msg);

	setspeed_inProgress = true;
	setspeed_finished = false;

}
