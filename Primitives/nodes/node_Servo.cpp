/*
 * node_Servo.cpp
 *
 *  Created on: Feb 17, 2011
 *      Author: triszalo
 */





#include "include/node_Servo.h"

using namespace std;


node_Servo::node_Servo(void) : node(SERVO_ID, "node_Servo", SERVO_KEEP_ALIVE_MS, SERVO_SEND_PERIOD_TO_PC_MS, SERVO_SEND_PERIOD_TO_NODE_MS){


	//----- valtozo init ELEJE -----
	Servo_Deg_Incr_x0[0]		= SERVO_0_DEG_INCR_X0;
	Servo_Deg_Incr_y0[0]		= SERVO_0_DEG_INCR_Y0;
	Servo_Deg_Incr_x1[0]		= SERVO_0_DEG_INCR_X1;
	Servo_Deg_Incr_y1[0]		= SERVO_0_DEG_INCR_Y1;
	Servo_Deg_Incr_grad[0]		= SERVO_0_DEG_INCR_GRAD;

	Servo_Deg_Incr_x0[1]		= SERVO_1_DEG_INCR_X0;
	Servo_Deg_Incr_y0[1]		= SERVO_1_DEG_INCR_Y0;
	Servo_Deg_Incr_x1[1]		= SERVO_1_DEG_INCR_X1;
	Servo_Deg_Incr_y1[1]		= SERVO_1_DEG_INCR_Y1;
	Servo_Deg_Incr_grad[1]		= SERVO_1_DEG_INCR_GRAD;

	Servo_Deg_Incr_x0[2]		= SERVO_2_DEG_INCR_X0;
	Servo_Deg_Incr_y0[2]		= SERVO_2_DEG_INCR_Y0;
	Servo_Deg_Incr_x1[2]		= SERVO_2_DEG_INCR_X1;
	Servo_Deg_Incr_y1[2]		= SERVO_2_DEG_INCR_Y1;
	Servo_Deg_Incr_grad[2]		= SERVO_2_DEG_INCR_GRAD;

	Servo_Deg_Incr_x0[3]		= SERVO_3_DEG_INCR_X0;
	Servo_Deg_Incr_y0[3]		= SERVO_3_DEG_INCR_Y0;
	Servo_Deg_Incr_x1[3]		= SERVO_3_DEG_INCR_X1;
	Servo_Deg_Incr_y1[3]		= SERVO_3_DEG_INCR_Y1;
	Servo_Deg_Incr_grad[3]		= SERVO_3_DEG_INCR_GRAD;

	Servo_Deg_Incr_x0[4]		= SERVO_4_DEG_INCR_X0;
	Servo_Deg_Incr_y0[4]		= SERVO_4_DEG_INCR_Y0;
	Servo_Deg_Incr_x1[4]		= SERVO_4_DEG_INCR_X1;
	Servo_Deg_Incr_y1[4]		= SERVO_4_DEG_INCR_Y1;
	Servo_Deg_Incr_grad[4]		= SERVO_4_DEG_INCR_GRAD;

	Servo_Deg_Incr_x0[5]		= SERVO_5_DEG_INCR_X0;
	Servo_Deg_Incr_y0[5]		= SERVO_5_DEG_INCR_Y0;
	Servo_Deg_Incr_x1[5]		= SERVO_5_DEG_INCR_X1;
	Servo_Deg_Incr_y1[5]		= SERVO_5_DEG_INCR_Y1;
	Servo_Deg_Incr_grad[5]		= SERVO_5_DEG_INCR_GRAD;
	//----- valtozo init VEGE -----


}


node_Servo::~node_Servo(){

}


void node_Servo::evalMsg(UDPmsg* msg){

	u8 num;


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

			case MSG_SERVO_SET_POS_REPLY:
				num = GET_U8(&(msg->data[0]));
				move[num].error = GET_BOOL(&(msg->data[1]), 0);
				move[num].inProgress = false;
				move[num].finished = true;
				break;

			default:
				break;


		}

	}

}


void node_Servo::SERVO_SET_POS(u8 num, double pos, double speed, double acc){

	UDPmsg msg;

	msg.node_id		= id;
	msg.function	= CMD_SERVO_SET_POS;
	msg.length		= 13;

	SET_U8(&(msg.data[0]), num);
	SET_U16(&(msg.data[1]), SERVO_DEG_TO_INCR(num, pos));
	SET_U16(&(msg.data[5]), SERVO_DEG_TO_INCR(num, speed));
	SET_U16(&(msg.data[9]), SERVO_DEG_TO_INCR(num, acc));

	UDPdriver::send(&msg);

	move[num].inProgress = true;
	move[num].finished = false;
	move[num].error = false;

}


void node_Servo::INIT_PARAM(void){

	UDPmsg msg;

	msg.node_id		= id;
	msg.function	= CMD_INIT_PARAM;

	msg.length		= 24;

	SET_U16(&(msg.data[0]), SERVO_0_HARDWARE_LIMIT_LOW_POS_INCR);
	SET_U16(&(msg.data[2]), SERVO_0_HARDWARE_LIMIT_HIGH_POS_INCR);

	SET_U16(&(msg.data[4]), SERVO_1_HARDWARE_LIMIT_LOW_POS_INCR);
	SET_U16(&(msg.data[6]), SERVO_1_HARDWARE_LIMIT_HIGH_POS_INCR);

	SET_U16(&(msg.data[8]), SERVO_2_HARDWARE_LIMIT_LOW_POS_INCR);
	SET_U16(&(msg.data[10]), SERVO_2_HARDWARE_LIMIT_HIGH_POS_INCR);

	SET_U16(&(msg.data[12]), SERVO_3_HARDWARE_LIMIT_LOW_POS_INCR);
	SET_U16(&(msg.data[14]), SERVO_3_HARDWARE_LIMIT_HIGH_POS_INCR);

	SET_U16(&(msg.data[16]), SERVO_4_HARDWARE_LIMIT_LOW_POS_INCR);
	SET_U16(&(msg.data[18]), SERVO_4_HARDWARE_LIMIT_HIGH_POS_INCR);

	SET_U16(&(msg.data[20]), SERVO_5_HARDWARE_LIMIT_LOW_POS_INCR);
	SET_U16(&(msg.data[22]), SERVO_5_HARDWARE_LIMIT_HIGH_POS_INCR);

	UDPdriver::send(&msg);

}


u16 node_Servo::SERVO_DEG_TO_INCR(u8 num, double deg){

	return (u16)((deg - Servo_Deg_Incr_x0[num]) * Servo_Deg_Incr_grad[num] + Servo_Deg_Incr_y0[num]);

}
