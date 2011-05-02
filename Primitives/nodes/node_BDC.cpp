/*
 * node_BDC.cpp
 *
 *  Created on: Feb 17, 2011
 *      Author: triszalo
 */





#include "include/node_BDC.h"

using namespace std;


node_BDC::node_BDC(void){

	id = BDC_ID;
	strcpy(name, "node_BDC");


	//----- valtozo init ELEJE -----
	stop_inProgress		= false;
	move_inProgress		= false;

	stop_finished		= false;
	move_finished		= false;
	//----- valtozo init VEGE -----

}


node_BDC::~node_BDC(){

}


void node_BDC::evalMsg(UDPmsg* msg){

	//ha o a cimzett, csak akkor dolgozza fel
	if(msg->node_id == id){

		switch(msg->function){

			case MSG_PING_REPLY:
				sem_post(&pingSemaphore);
				break;

			case MSG_BDC_STOP_REPLY:
			case MSG_BDC_HARD_STOP_REPLY:
				stop_inProgress = false;
				stop_finished = true;
				break;

			case MSG_BDC_GO_REPLY:
			case MSG_BDC_GOTO_REPLY:
			case MSG_BDC_TURN_REPLY:
			case MSG_BDC_SET_SPEED_REPLY:
				move_inProgress = false;
				move_finished = true;
				break;

			default:
				break;


		}

	}

}


void node_BDC::BDC_STOP(double acc){

	float*	tmp;

	UDPmsg msg;

	msg.node_id		= id;
	msg.function	= CMD_BDC_STOP;
	msg.length		= 4;
	tmp = (float*)(&(msg.data[0]));		*tmp = (float)acc;

	UDPdriver::send(&msg);

	stop_inProgress = true;
	stop_finished = false;

}


void node_BDC::BDC_HARD_STOP(void){

	UDPmsg msg;

	msg.node_id		= id;
	msg.function	= CMD_BDC_HARD_STOP;
	msg.length		= 0;

	UDPdriver::send(&msg);

	stop_inProgress = true;
	stop_finished = false;

}


void node_BDC::BDC_GO(double distance, double max_speed, double max_acc){

	float*	tmp;

	UDPmsg msg;

	msg.node_id		= id;
	msg.function	= CMD_BDC_GO;
	msg.length		= 12;
	tmp = (float*)(&(msg.data[0]));		*tmp = (float)distance;
	tmp = (float*)(&(msg.data[4]));		*tmp = (float)max_speed;
	tmp = (float*)(&(msg.data[8]));		*tmp = (float)max_acc;

	UDPdriver::send(&msg);

	move_inProgress = true;
	move_finished = false;

}


void node_BDC::BDC_GOTO(double x, double y, double max_speed, double max_acc){

	float*	tmp;

	UDPmsg msg;

	msg.node_id		= id;
	msg.function	= CMD_BDC_GOTO;
	msg.length		= 16;
	tmp = (float*)(&(msg.data[0]));		*tmp = (float)x;
	tmp = (float*)(&(msg.data[4]));		*tmp = (float)y;
	tmp = (float*)(&(msg.data[8]));		*tmp = (float)max_speed;
	tmp = (float*)(&(msg.data[12]));	*tmp = (float)max_acc;

	UDPdriver::send(&msg);

	move_inProgress = true;
	move_finished = false;

}


void node_BDC::BDC_TURN(double angle, double max_speed, double max_acc){

	float*	tmp;

	UDPmsg msg;

	msg.node_id		= id;
	msg.function	= CMD_BDC_TURN;
	msg.length		= 12;
	tmp = (float*)(&(msg.data[0]));		*tmp = (float)angle;
	tmp = (float*)(&(msg.data[4]));		*tmp = (float)max_speed;
	tmp = (float*)(&(msg.data[8]));		*tmp = (float)max_acc;

	UDPdriver::send(&msg);

	move_inProgress = true;
	move_finished = false;

}


void node_BDC::BDC_SET_SPEED(double v, double w){

	float*	tmp;

	UDPmsg msg;

	msg.node_id		= id;
	msg.function	= CMD_BDC_SET_SPEED;
	msg.length		= 8;
	tmp = (float*)(&(msg.data[0]));		*tmp = (float)v;
	tmp = (float*)(&(msg.data[4]));		*tmp = (float)w;

	UDPdriver::send(&msg);

	move_inProgress = true;
	move_finished = false;

}
