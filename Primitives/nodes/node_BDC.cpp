/*
 * node_BDC.cpp
 *
 *  Created on: Feb 17, 2011
 *      Author: triszalo
 */





#include "include/node_BDC.h"

using namespace std;


node_BDC::node_BDC(void) : node(BDC_ID, "node_BDC", BDC_KEEP_ALIVE_MS, BDC_SEND_PERIOD_TO_PC_MS, BDC_SEND_PERIOD_TO_NODE_MS){


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
	msg.length		= 2;
	SET_U16(&(msg.data[0]), BDC_CONV_ACC(acc));

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
	msg.length		= 8;
	SET_S32(&(msg.data[0]), BDC_CONV_DIST(distance));
	SET_U16(&(msg.data[4]), BDC_CONV_SPEED(max_speed));
	SET_U16(&(msg.data[6]), BDC_CONV_ACC(max_acc));

	UDPdriver::send(&msg);

	move.inProgress = true;
	move.finished = false;
	move.error = false;

}


void node_BDC::BDC_GOTO(double x, double y, double max_speed, double max_acc){

	UDPmsg msg;

	msg.node_id		= id;
	msg.function	= CMD_BDC_GOTO;
	msg.length		= 12;
	SET_FLOAT(&(msg.data[0]), x);
	SET_FLOAT(&(msg.data[4]), y);
	SET_U16(&(msg.data[8]), BDC_CONV_SPEED(max_speed));
	SET_U16(&(msg.data[10]), BDC_CONV_ACC(max_acc));

	UDPdriver::send(&msg);

	move.inProgress = true;
	move.finished = false;
	move.error = false;

}


void node_BDC::BDC_TURN(double angle, double max_speed, double max_acc){

	UDPmsg msg;

	msg.node_id		= id;
	msg.function	= CMD_BDC_TURN;
	msg.length		= 8;
	SET_FLOAT(&(msg.data[0]), angle);
	SET_U16(&(msg.data[4]), BDC_CONV_SPEED(max_speed));
	SET_U16(&(msg.data[6]), BDC_CONV_ACC(max_acc));

	UDPdriver::send(&msg);

	move.inProgress = true;
	move.finished = false;
	move.error = false;

}


void node_BDC::BDC_SET_SPEED(double v, double w){

	UDPmsg msg;

	msg.node_id		= id;
	msg.function	= CMD_BDC_SET_SPEED;
	msg.length		= 4;

	double r = v / w;			//w = v / r
	double t = 2 * r * M_PI / v;	//v = 2*r*Pi / t
	double dr = WHEEL_DISTANCE_MM / 2;
	s16 vLeft	= 2 * (r - dr) * M_PI / t;
	s16 vRight	= 2 * (r + dr) * M_PI / t;

	SET_S16(&(msg.data[0]), (BDC_IS_LEFT_MOTOR1 != 0) ? vLeft : vRight);
	SET_S16(&(msg.data[2]), (BDC_IS_LEFT_MOTOR1 != 0) ? vRight : vLeft);

	UDPdriver::send(&msg);

	move.inProgress = true;
	move.finished = false;
	move.error = false;

}


void node_BDC::INIT_PARAM(void){

	UDPmsg msg;

	msg.node_id		= id;
	msg.function	= CMD_INIT_PARAM;
	msg.length		= 5;
	SET_BOOL(&(msg.data[0]), 0, BDC_IS_LEFT_MOTOR1);
	SET_U32(&(msg.data[1]), BDC_ROBOT_FULL_TURN_INCR);

	UDPdriver::send(&msg);

}
