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
		setservo_inProgress[i] = false;

	for(unsigned int i=0 ; i<SERVO_COUNT ; i++)
		setservo_finished[i] = false;
	//----- valtozo init VEGE -----

}


node_Servo::~node_Servo(){

}


void node_Servo::evalMsg(UDPmsg* msg){

	//ha o a cimzett, csak akkor dolgozza fel
	if(msg->node_id == id){

		switch(msg->function){

			case MSG_PING_REPLY:
				sem_post(&pingSemaphore);
				break;

			case MSG_SET_SERVO0_REPLY:
				setservo_inProgress[0] = false;
				setservo_finished[0] = true;
				break;

			case MSG_SET_SERVO1_REPLY:
				setservo_inProgress[1] = false;
				setservo_finished[1] = true;
				break;

			case MSG_SET_SERVO2_REPLY:
				setservo_inProgress[2] = false;
				setservo_finished[2] = true;
				break;

			case MSG_SET_SERVO3_REPLY:
				setservo_inProgress[3] = false;
				setservo_finished[3] = true;
				break;

			case MSG_SET_SERVO4_REPLY:
				setservo_inProgress[4] = false;
				setservo_finished[4] = true;
				break;

			case MSG_SET_SERVO5_REPLY:
				setservo_inProgress[5] = false;
				setservo_finished[5] = true;
				break;

			default:
				break;


		}

	}

}


void node_Servo::SET_SERVO(unsigned int num, double pos, double speed, double acc){

	float*	tmp;

	UDPmsg msg;

	msg.node_id		= id;

	switch(num){

		case 0:
			msg.function	= CMD_SET_SERVO0;
			break;

		case 1:
			msg.function	= CMD_SET_SERVO1;
			break;

		case 2:
			msg.function	= CMD_SET_SERVO2;
			break;

		case 3:
			msg.function	= CMD_SET_SERVO3;
			break;

		case 4:
			msg.function	= CMD_SET_SERVO4;
			break;

		case 5:
			msg.function	= CMD_SET_SERVO5;
			break;

		default:	//nem lehet, rossz parameter
			return;

	}

	msg.length		= 12;
	tmp = (float*)(&(msg.data[0]));		*tmp = (float)pos;
	tmp = (float*)(&(msg.data[4]));		*tmp = (float)speed;
	tmp = (float*)(&(msg.data[8]));		*tmp = (float)acc;

	UDPdriver::send(&msg);

	setservo_inProgress[num] = true;
	setservo_finished[num] = false;

}
