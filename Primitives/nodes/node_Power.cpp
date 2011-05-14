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
	PowerAccuVoltage = 0;
	PowerAccuCharging = false;
	PowerPlugIn = false;
	PowerStopButton = false;
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

			case MSG_PERIODIC_TO_PC:
				PowerAccuVoltage = GET_U16(&(msg->data[0]));
				PowerAccuCharging = GET_BOOL(&(msg->data[2]), 0);
				PowerPlugIn = GET_BOOL(&(msg->data[2]), 1);
				PowerStopButton = GET_BOOL(&(msg->data[2]), 2);
				break;

			case MSG_POWER_MOTOR_ON_REPLY:
			case MSG_POWER_MOTOR_OFF_REPLY:
				motor_on_off.inProgress = false;
				motor_on_off.finished = true;
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

	motor_on_off.inProgress = true;
	motor_on_off.finished = false;

}


void node_Power::POWER_MOTOR_OFF(void){

	UDPmsg msg;

	msg.node_id		= id;
	msg.function	= CMD_POWER_MOTOR_OFF;
	msg.length		= 0;

	UDPdriver::send(&msg);

	motor_on_off.inProgress = true;
	motor_on_off.finished = false;

}


void node_Power::INIT_PARAM(void){

	UDPmsg msg;

	msg.node_id		= id;
	msg.function	= CMD_INIT_PARAM;
	msg.length		= 8;

	//grad = (y-y0) / (x-x0)
	//x = (y-y0) / grad + x0
	SET_U16(&(msg.data[0]), (u16)((POWER_VOLTAGE_WARNING_V - POWER_ANALOG_V_Y0) / POWER_ANALOG_V_GRAD + POWER_ANALOG_V_X0));
	SET_U16(&(msg.data[2]), (u16)((POWER_VOLTAGE_ERROR_V - POWER_ANALOG_V_Y0) / POWER_ANALOG_V_GRAD + POWER_ANALOG_V_X0));
	SET_U16(&(msg.data[4]), (u16)((POWER_CURRENT_WARNING_V - POWER_ANALOG_V_Y0) / POWER_ANALOG_V_GRAD + POWER_ANALOG_V_X0));
	SET_U16(&(msg.data[6]), (u16)((POWER_CURRENT_ERROR_V - POWER_ANALOG_V_Y0) / POWER_ANALOG_V_GRAD + POWER_ANALOG_V_X0));

	UDPdriver::send(&msg);

}


double node_Power::GET_ACCU_VOLTAGE(void){

	//grad = (y-y0) / (x-x0)
	//y = (x-x0) * grad + y0
	return ((double)PowerAccuVoltage - POWER_ANALOG_V_X0) * POWER_ANALOG_V_GRAD + POWER_ANALOG_V_Y0;

}


bool node_Power::GET_ACCU_CHARGING(void){

	return PowerAccuCharging;

}


bool node_Power::GET_PLUG_IN(void){

	return PowerPlugIn;

}


bool node_Power::GET_STOP_BUTTON(void){

	return PowerStopButton;

}
