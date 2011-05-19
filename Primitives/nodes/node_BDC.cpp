/*
 * node_BDC.cpp
 *
 *  Created on: Feb 17, 2011
 *      Author: triszalo
 */





#include "include/node_BDC.h"

using namespace std;


node_BDC::node_BDC(void) : node(BDC_ID, "node_BDC"){


	//----- valtozo init ELEJE -----
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

			case MSG_INIT_PARAM_REPLY:
				initparamOK	= GET_BOOL(&(msg->data[0]), 0);
				sem_post(&initparamSemaphore);
				break;

			case MSG_BDC_STOP_REPLY:
			case MSG_BDC_HARD_STOP_REPLY:
				stop.error = GET_BOOL(&(msg->data[0]), 0);
				stop.inProgress = false;
				stop.finished = true;
				break;

			case MSG_BDC_GO_REPLY:
			case MSG_BDC_GOTO_REPLY:
			case MSG_BDC_TURN_REPLY:
			case MSG_BDC_SET_SPEED_REPLY:
				move.error = GET_BOOL(&(msg->data[0]), 0);
				move.inProgress = false;
				move.finished = true;
				break;

			default:
				break;


		}

	}

}


void node_BDC::BDC_STOP(double acc){

	UDPmsg msg;

	msg.node_id		= id;
	msg.function	= CMD_BDC_STOP;
	msg.length		= 4;
	SET_FLOAT(&(msg.data[0]), acc);

	UDPdriver::send(&msg);

	stop.inProgress = true;
	stop.finished = false;
	stop.error = false;

}


void node_BDC::BDC_HARD_STOP(void){

	UDPmsg msg;

	msg.node_id		= id;
	msg.function	= CMD_BDC_HARD_STOP;
	msg.length		= 0;

	UDPdriver::send(&msg);

	stop.inProgress = true;
	stop.finished = false;
	stop.error = false;

}


void node_BDC::BDC_GO(double distance, double max_speed, double max_acc){

	UDPmsg msg;

	msg.node_id		= id;
	msg.function	= CMD_BDC_GO;
	msg.length		= 12;
	SET_FLOAT(&(msg.data[0]), distance);
	SET_FLOAT(&(msg.data[4]), max_speed);
	SET_FLOAT(&(msg.data[8]), max_acc);

	UDPdriver::send(&msg);

	move.inProgress = true;
	move.finished = false;
	move.error = false;

}


void node_BDC::BDC_GOTO(double x, double y, double max_speed, double max_acc){

	UDPmsg msg;

	msg.node_id		= id;
	msg.function	= CMD_BDC_GOTO;
	msg.length		= 16;
	SET_FLOAT(&(msg.data[0]), x);
	SET_FLOAT(&(msg.data[4]), y);
	SET_FLOAT(&(msg.data[8]), max_speed);
	SET_FLOAT(&(msg.data[12]), max_acc);

	UDPdriver::send(&msg);

	move.inProgress = true;
	move.finished = false;
	move.error = false;

}


void node_BDC::BDC_TURN(double angle, double max_speed, double max_acc){

	UDPmsg msg;

	msg.node_id		= id;
	msg.function	= CMD_BDC_TURN;
	msg.length		= 12;
	SET_FLOAT(&(msg.data[0]), angle);
	SET_FLOAT(&(msg.data[4]), max_speed);
	SET_FLOAT(&(msg.data[8]), max_acc);

	UDPdriver::send(&msg);

	move.inProgress = true;
	move.finished = false;
	move.error = false;

}


void node_BDC::BDC_SET_SPEED(double v, double w){

	UDPmsg msg;

	msg.node_id		= id;
	msg.function	= CMD_BDC_SET_SPEED;
	msg.length		= 8;
	SET_FLOAT(&(msg.data[0]), v);
	SET_FLOAT(&(msg.data[4]), w);

	UDPdriver::send(&msg);

	move.inProgress = true;
	move.finished = false;
	move.error = false;

}


void node_BDC::INIT_PARAM(void){

	UDPmsg msg;

	msg.node_id		= id;
	msg.function	= CMD_INIT_PARAM;
	msg.length		= 0;

	UDPdriver::send(&msg);

}
