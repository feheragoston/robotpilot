/*
 * node_Servo.cpp
 *
 *  Created on: Feb 17, 2011
 *      Author: triszalo
 */





#include "include/node_Servo.h"

using namespace std;


node_Servo::node_Servo(void){

	id = SERVO_ID;
	strcpy(name, "node_Servo");


	//----- valtozo init ELEJE -----
	for(unsigned int i=0 ; i<SERVO_COUNT ; i++)
		move_inProgress[i] = false;

	for(unsigned int i=0 ; i<SERVO_COUNT ; i++)
		move_finished[i] = false;
	//----- valtozo init VEGE -----

}


node_Servo::~node_Servo(){

}


void node_Servo::evalMsg(UDPmsg* msg){

	unsigned char num;


	//ha o a cimzett, csak akkor dolgozza fel
	if(msg->node_id == id){

		switch(msg->function){

			case MSG_PING_REPLY:
				sem_post(&pingSemaphore);
				break;

			case MSG_SERVO_SET_POS_REPLY:
				num = (unsigned char)(msg->data[0]);
				move_inProgress[num] = false;
				move_finished[num] = true;
				break;

			default:
				break;


		}

	}

}


void node_Servo::SERVO_SET_POS(unsigned int num, double pos, double speed, double acc){

	float*	tmp;

	UDPmsg msg;

	msg.node_id		= id;
	msg.function	= CMD_SERVO_SET_POS;

	msg.length		= 13;
	msg.data[0] = num;
	tmp = (float*)(&(msg.data[1]));		*tmp = (float)pos;
	tmp = (float*)(&(msg.data[5]));		*tmp = (float)speed;
	tmp = (float*)(&(msg.data[9]));		*tmp = (float)acc;

	UDPdriver::send(&msg);

	move_inProgress[num] = true;
	move_finished[num] = false;

}
