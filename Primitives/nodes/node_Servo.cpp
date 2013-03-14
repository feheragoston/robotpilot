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
	Servo_Deg_Incr_x0[SERVO_FRONT_GRIPPER_INDEX]				= SERVO_FRONT_GRIPPER_DEG_INCR_X0;
	Servo_Deg_Incr_y0[SERVO_FRONT_GRIPPER_INDEX]				= SERVO_FRONT_GRIPPER_DEG_INCR_Y0;
	Servo_Deg_Incr_x1[SERVO_FRONT_GRIPPER_INDEX]				= SERVO_FRONT_GRIPPER_DEG_INCR_X1;
	Servo_Deg_Incr_y1[SERVO_FRONT_GRIPPER_INDEX]				= SERVO_FRONT_GRIPPER_DEG_INCR_Y1;
	Servo_Deg_Incr_grad[SERVO_FRONT_GRIPPER_INDEX]				= SERVO_GET_GRAD(SERVO_FRONT_GRIPPER_INDEX);
	Servo_Limit_Low_Pos_Incr[SERVO_FRONT_GRIPPER_INDEX]			= SERVO_FRONT_GRIPPER_LIMIT_LOW_POS_INCR;
	Servo_Limit_High_Pos_Incr[SERVO_FRONT_GRIPPER_INDEX]		= SERVO_FRONT_GRIPPER_LIMIT_HIGH_POS_INCR;
	Servo_Limit_Current_Ma[SERVO_FRONT_GRIPPER_INDEX]			= SERVO_FRONT_GRIPPER_LIMIT_CURRENT_MA;

	Servo_Deg_Incr_x0[SERVO_BACK_GRIPPER_INDEX]					= SERVO_BACK_GRIPPER_DEG_INCR_X0;
	Servo_Deg_Incr_y0[SERVO_BACK_GRIPPER_INDEX]					= SERVO_BACK_GRIPPER_DEG_INCR_Y0;
	Servo_Deg_Incr_x1[SERVO_BACK_GRIPPER_INDEX]					= SERVO_BACK_GRIPPER_DEG_INCR_X1;
	Servo_Deg_Incr_y1[SERVO_BACK_GRIPPER_INDEX]					= SERVO_BACK_GRIPPER_DEG_INCR_Y1;
	Servo_Deg_Incr_grad[SERVO_BACK_GRIPPER_INDEX]				= SERVO_GET_GRAD(SERVO_BACK_GRIPPER_INDEX);
	Servo_Limit_Low_Pos_Incr[SERVO_BACK_GRIPPER_INDEX]			= SERVO_BACK_GRIPPER_LIMIT_LOW_POS_INCR;
	Servo_Limit_High_Pos_Incr[SERVO_BACK_GRIPPER_INDEX]			= SERVO_BACK_GRIPPER_LIMIT_HIGH_POS_INCR;
	Servo_Limit_Current_Ma[SERVO_BACK_GRIPPER_INDEX]			= SERVO_BACK_GRIPPER_LIMIT_CURRENT_MA;

	Servo_Deg_Incr_x0[SERVO_SELECTOR_INDEX]						= SERVO_SELECTOR_DEG_INCR_X0;
	Servo_Deg_Incr_y0[SERVO_SELECTOR_INDEX]						= SERVO_SELECTOR_DEG_INCR_Y0;
	Servo_Deg_Incr_x1[SERVO_SELECTOR_INDEX]						= SERVO_SELECTOR_DEG_INCR_X1;
	Servo_Deg_Incr_y1[SERVO_SELECTOR_INDEX]						= SERVO_SELECTOR_DEG_INCR_Y1;
	Servo_Deg_Incr_grad[SERVO_SELECTOR_INDEX]					= SERVO_GET_GRAD(SERVO_SELECTOR_INDEX);
	Servo_Limit_Low_Pos_Incr[SERVO_SELECTOR_INDEX]				= SERVO_SELECTOR_LIMIT_LOW_POS_INCR;
	Servo_Limit_High_Pos_Incr[SERVO_SELECTOR_INDEX]				= SERVO_SELECTOR_LIMIT_HIGH_POS_INCR;
	Servo_Limit_Current_Ma[SERVO_SELECTOR_INDEX]				= SERVO_SELECTOR_LIMIT_CURRENT_MA;

	Servo_Deg_Incr_x0[SERVO_FIRESTOPPER_INDEX]					= SERVO_FIRESTOPPER_DEG_INCR_X0;
	Servo_Deg_Incr_y0[SERVO_FIRESTOPPER_INDEX]					= SERVO_FIRESTOPPER_DEG_INCR_Y0;
	Servo_Deg_Incr_x1[SERVO_FIRESTOPPER_INDEX]					= SERVO_FIRESTOPPER_DEG_INCR_X1;
	Servo_Deg_Incr_y1[SERVO_FIRESTOPPER_INDEX]					= SERVO_FIRESTOPPER_DEG_INCR_Y1;
	Servo_Deg_Incr_grad[SERVO_FIRESTOPPER_INDEX]				= SERVO_GET_GRAD(SERVO_FIRESTOPPER_INDEX);
	Servo_Limit_Low_Pos_Incr[SERVO_FIRESTOPPER_INDEX]			= SERVO_FIRESTOPPER_LIMIT_LOW_POS_INCR;
	Servo_Limit_High_Pos_Incr[SERVO_FIRESTOPPER_INDEX]			= SERVO_FIRESTOPPER_LIMIT_HIGH_POS_INCR;
	Servo_Limit_Current_Ma[SERVO_FIRESTOPPER_INDEX]				= SERVO_FIRESTOPPER_LIMIT_CURRENT_MA;

	Servo_Deg_Incr_x0[SERVO_FRONT_FLIPPER_INDEX]				= SERVO_FRONT_FLIPPER_DEG_INCR_X0;
	Servo_Deg_Incr_y0[SERVO_FRONT_FLIPPER_INDEX]				= SERVO_FRONT_FLIPPER_DEG_INCR_Y0;
	Servo_Deg_Incr_x1[SERVO_FRONT_FLIPPER_INDEX]				= SERVO_FRONT_FLIPPER_DEG_INCR_X1;
	Servo_Deg_Incr_y1[SERVO_FRONT_FLIPPER_INDEX]				= SERVO_FRONT_FLIPPER_DEG_INCR_Y1;
	Servo_Deg_Incr_grad[SERVO_FRONT_FLIPPER_INDEX]				= SERVO_GET_GRAD(SERVO_FRONT_FLIPPER_INDEX);
	Servo_Limit_Low_Pos_Incr[SERVO_FRONT_FLIPPER_INDEX]			= SERVO_FRONT_FLIPPER_LIMIT_LOW_POS_INCR;
	Servo_Limit_High_Pos_Incr[SERVO_FRONT_FLIPPER_INDEX]		= SERVO_FRONT_FLIPPER_LIMIT_HIGH_POS_INCR;
	Servo_Limit_Current_Ma[SERVO_FRONT_FLIPPER_INDEX]			= SERVO_FRONT_FLIPPER_LIMIT_CURRENT_MA;

	Servo_Deg_Incr_x0[SERVO_BACK_FLIPPER_INDEX]					= SERVO_BACK_FLIPPER_DEG_INCR_X0;
	Servo_Deg_Incr_y0[SERVO_BACK_FLIPPER_INDEX]					= SERVO_BACK_FLIPPER_DEG_INCR_Y0;
	Servo_Deg_Incr_x1[SERVO_BACK_FLIPPER_INDEX]					= SERVO_BACK_FLIPPER_DEG_INCR_X1;
	Servo_Deg_Incr_y1[SERVO_BACK_FLIPPER_INDEX]					= SERVO_BACK_FLIPPER_DEG_INCR_Y1;
	Servo_Deg_Incr_grad[SERVO_BACK_FLIPPER_INDEX]				= SERVO_GET_GRAD(SERVO_BACK_FLIPPER_INDEX);
	Servo_Limit_Low_Pos_Incr[SERVO_BACK_FLIPPER_INDEX]			= SERVO_BACK_FLIPPER_LIMIT_LOW_POS_INCR;
	Servo_Limit_High_Pos_Incr[SERVO_BACK_FLIPPER_INDEX]			= SERVO_BACK_FLIPPER_LIMIT_HIGH_POS_INCR;
	Servo_Limit_Current_Ma[SERVO_BACK_FLIPPER_INDEX]			= SERVO_BACK_FLIPPER_LIMIT_CURRENT_MA;

	for(u8 i=0 ; i<SERVO_COUNT ; i++){
		Servo_Pos_Incr[i]		= 1500;
		Servo_CmdPos_Incr[i]	= 1500;
	}
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
				Setpos[num].done = GET_BOOL(&(msg->data[1]), 0);
				Servo_Pos_Incr[num] = GET_U16(&(msg->data[2]));
				Setpos[num].inProgress = false;
				Setpos[num].finished = true;

				cout << name << "\t___recv SETPOS [" << (u32)num << "]___:\t" << (Setpos[num].done?"1":"0") << "\t" << (u32)Servo_Pos_Incr[num] << endl;

				break;

			default:
				break;


		}

	}

}


void node_Servo::SERVO_SET_POS(u8 num, double pos, double speed, double acc){

	UDPmsg msg;

	Servo_CmdPos_Incr[num] = SERVO_CONV_POS(num, pos);

	msg.node_id		= id;
	msg.function	= CMD_SERVO_SET_POS;
	msg.length		= 7;

	SET_U8(&(msg.data[0]), num);
	SET_U16(&(msg.data[1]), Servo_CmdPos_Incr[num]);
	SET_U16(&(msg.data[3]), SERVO_CONV_SPEED(num, speed));
	SET_U16(&(msg.data[5]), SERVO_CONV_ACC(num, acc));

	UDPdriver::send(&msg);

	Setpos[num].inProgress = true;
	Setpos[num].finished = false;

	cout << name << "\t___send SETPOS [" << (u32)num << "]___:\t" << SERVO_CONV_POS(num, pos) << "\t" << SERVO_CONV_SPEED(num, speed) << "\t" << SERVO_CONV_ACC(num, acc) << endl;

}


void node_Servo::INIT_PARAM(void){

	UDPmsg msg;

	msg.node_id		= id;
	msg.function	= CMD_INIT_PARAM;

	msg.length		= 36;

	SET_U16(&(msg.data[0]), Servo_Limit_Low_Pos_Incr[0]);
	SET_U16(&(msg.data[2]), Servo_Limit_High_Pos_Incr[0]);
	SET_U16(&(msg.data[4]), Servo_Limit_Current_Ma[0]);

	SET_U16(&(msg.data[6]), Servo_Limit_Low_Pos_Incr[1]);
	SET_U16(&(msg.data[8]), Servo_Limit_High_Pos_Incr[1]);
	SET_U16(&(msg.data[10]), Servo_Limit_Current_Ma[1]);

	SET_U16(&(msg.data[12]), Servo_Limit_Low_Pos_Incr[2]);
	SET_U16(&(msg.data[14]), Servo_Limit_High_Pos_Incr[2]);
	SET_U16(&(msg.data[16]), Servo_Limit_Current_Ma[2]);

	SET_U16(&(msg.data[18]), Servo_Limit_Low_Pos_Incr[3]);
	SET_U16(&(msg.data[20]), Servo_Limit_High_Pos_Incr[3]);
	SET_U16(&(msg.data[22]), Servo_Limit_Current_Ma[3]);

	SET_U16(&(msg.data[24]), Servo_Limit_Low_Pos_Incr[4]);
	SET_U16(&(msg.data[26]), Servo_Limit_High_Pos_Incr[4]);
	SET_U16(&(msg.data[28]), Servo_Limit_Current_Ma[4]);

	SET_U16(&(msg.data[30]), Servo_Limit_Low_Pos_Incr[5]);
	SET_U16(&(msg.data[32]), Servo_Limit_High_Pos_Incr[5]);
	SET_U16(&(msg.data[34]), Servo_Limit_Current_Ma[5]);

	UDPdriver::send(&msg);

}


double node_Servo::SERVO_GET_GRAD(u8 num){

	return GET_GRAD(Servo_Deg_Incr_x0[num], Servo_Deg_Incr_y0[num], Servo_Deg_Incr_x1[num], Servo_Deg_Incr_y1[num]);

}


u16 node_Servo::SERVO_DEG_TO_INCR(u8 num, double deg){

	//grad = (y - y0) / (x - x0)
	//y = (x - x0) * grad + y0
	return (u16)((deg - Servo_Deg_Incr_x0[num]) * Servo_Deg_Incr_grad[num] + Servo_Deg_Incr_y0[num]);

}


double node_Servo::SERVO_INCR_TO_DEG(u8 num, u16 incr){

	//grad = (y - y0) / (x - x0)
	//x = (y - y0) / grad + x0
	return ((double)incr - Servo_Deg_Incr_y0[num]) / Servo_Deg_Incr_grad[num] + Servo_Deg_Incr_x0[num];

}


u16 node_Servo::SERVO_CONV_POS(u8 num, double pos){

	return SERVO_DEG_TO_INCR(num, pos);

}


u16 node_Servo::SERVO_CONV_SPEED(u8 num, double speed){

	//grad = dy / dx
	//dy = dx * grad
	return (u16)fabs(speed * Servo_Deg_Incr_grad[num]);

}


u16 node_Servo::SERVO_CONV_ACC(u8 num, double acc){

	//grad = dy / dx
	//dy = dx * grad
	return (u16)fabs(acc * Servo_Deg_Incr_grad[num]);

}


double node_Servo::GET_POS(u8 num){

	return SERVO_INCR_TO_DEG(num, Servo_Pos_Incr[num]);

}


bool node_Servo::GET_ERROR(u8 num){

	return (!Setpos[num].done);

}
