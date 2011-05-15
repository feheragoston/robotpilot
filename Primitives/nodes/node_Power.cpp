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
	PowerChargeVoltage = 0;
	PowerMainVoltage = 0;
	PowerActCurrent = 0;
	PowerMainCurrent = 0;
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
				PowerChargeVoltage = GET_U16(&(msg->data[0]));
				PowerMainVoltage = GET_U16(&(msg->data[2]));
				PowerActCurrent = GET_U16(&(msg->data[4]));
				PowerMainCurrent = GET_U16(&(msg->data[6]));
				break;

			case MSG_POWER_ACT_ON_REPLY:
			case MSG_POWER_ACT_OFF_REPLY:
				act_on_off.inProgress = false;
				act_on_off.finished = true;
				break;

			case MSG_POWER_CHG_ON_REPLY:
			case MSG_POWER_CHG_OFF_REPLY:
				chg_on_off.inProgress = false;
				chg_on_off.finished = true;
				break;

			default:
				break;


		}

	}

}


void node_Power::POWER_ACT_ON(void){

	UDPmsg msg;

	msg.node_id		= id;
	msg.function	= CMD_POWER_ACT_ON;
	msg.length		= 0;

	UDPdriver::send(&msg);

	act_on_off.inProgress = true;
	act_on_off.finished = false;

}


void node_Power::POWER_ACT_OFF(void){

	UDPmsg msg;

	msg.node_id		= id;
	msg.function	= CMD_POWER_ACT_OFF;
	msg.length		= 0;

	UDPdriver::send(&msg);

	act_on_off.inProgress = true;
	act_on_off.finished = false;

}


void node_Power::POWER_CHG_ON(void){

	UDPmsg msg;

	msg.node_id		= id;
	msg.function	= CMD_POWER_CHG_ON;
	msg.length		= 0;

	UDPdriver::send(&msg);

	chg_on_off.inProgress = true;
	chg_on_off.finished = false;

}


void node_Power::POWER_CHG_OFF(void){

	UDPmsg msg;

	msg.node_id		= id;
	msg.function	= CMD_POWER_CHG_OFF;
	msg.length		= 0;

	UDPdriver::send(&msg);

	chg_on_off.inProgress = true;
	chg_on_off.finished = false;

}


void node_Power::POWER_MAIN_OFF(void){

	UDPmsg msg;

	msg.node_id		= id;
	msg.function	= CMD_POWER_MAIN_OFF;
	msg.length		= 0;

	UDPdriver::send(&msg);

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
	return ((double)PowerChargeVoltage - POWER_ANALOG_V_X0) * POWER_ANALOG_V_GRAD + POWER_ANALOG_V_Y0;

}


bool node_Power::GET_ACCU_CHARGING(void){

	//!!! szamolni !!!
	return false;

}


bool node_Power::GET_PLUG_IN(void){

	//!!! szamolni !!!
	return false;

}


bool node_Power::GET_STOP_BUTTON(void){

	//!!! szamolni !!!
	return false;

}
