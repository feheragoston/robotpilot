/*
 * node_Power.cpp
 *
 *  Created on: Feb 17, 2011
 *      Author: triszalo
 */





#include "include/node_Power.h"

#ifdef NAGY_ROBOT

using namespace std;


node_Power::node_Power(void) : node(POWER_ID, "node_Power", POWER_KEEP_ALIVE_MS, POWER_SEND_PERIOD_TO_PC_MS, POWER_SEND_PERIOD_TO_NODE_MS){


	//----- valtozo init ELEJE -----
	PowerMainVoltage = 0;
	PowerAccuVoltageOk = true;
	PowerStopButton = false;
	PowerActOn = false;
	PowerActOn_prev = false;
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

			case MSG_INIT_PARAM_REPLY:
				initparamOK	= GET_BOOL(&(msg->data[0]), 0);
				sem_post(&initparamSemaphore);
				break;

			case MSG_PERIODIC_TO_PC:
				PowerMainVoltage = GET_U16(&(msg->data[0]));
				PowerAccuVoltageOk = GET_BOOL(&(msg->data[2]), 0);
				PowerStopButton = GET_BOOL(&(msg->data[2]), 1);
				PowerActOn = GET_BOOL(&(msg->data[2]), 2);

				break;

			case MSG_POWER_ACT_ON_REPLY:
			case MSG_POWER_ACT_OFF_REPLY:
				ActOnOff.done = GET_BOOL(&(msg->data[0]), 0);
				ActOnOff.inProgress = false;
				ActOnOff.finished = true;
				cout << name << "\t___recv ACTONOFF___:\t" << (ActOnOff.done?"1":"0") << endl;
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

	ActOnOff.inProgress = true;
	ActOnOff.finished = false;

	cout << name << "\t___send ACTON___" << endl;

}


void node_Power::POWER_ACT_OFF(void){

	UDPmsg msg;

	msg.node_id		= id;
	msg.function	= CMD_POWER_ACT_OFF;
	msg.length		= 0;

	UDPdriver::send(&msg);

	ActOnOff.inProgress = true;
	ActOnOff.finished = false;

	cout << name << "\t___send ACTOFF___" << endl;

}


void node_Power::POWER_MAIN_OFF(void){

	UDPmsg msg;

	msg.node_id		= id;
	msg.function	= CMD_POWER_MAIN_OFF;
	msg.length		= 0;

	UDPdriver::send(&msg);

	cout << name << "\t___send MAINOFF___" << endl;

}


void node_Power::INIT_PARAM(void){

	UDPmsg msg;

	msg.node_id		= id;
	msg.function	= CMD_INIT_PARAM;
	msg.length		= 20;

	SET_U16(&(msg.data[0]), POWER_V_TO_ANALOG(POWER_MAIN_VOLTAGE_SHUTDOWN_LEVEL));
	SET_U16(&(msg.data[2]), POWER_V_TO_ANALOG(POWER_MAIN_VOLTAGE_STOPBUTTON_LEVEL));
	SET_U16(&(msg.data[4]), POWER_MAIN_VOLTAGE_LOW_SHUTDOWN_TIME_MS);

	SET_U16(&(msg.data[6]), POWER_A_TO_ANALOG(POWER_MAIN_CURRENT_SHUTDOWN_LEVEL));
	SET_U16(&(msg.data[8]), POWER_A_TO_ANALOG(POWER_ACT_CURRENT_SHUTDOWN_LEVEL));
	SET_U16(&(msg.data[10]), POWER_MAIN_HIGH_CURRENT_TIME_MS);
	SET_U16(&(msg.data[12]), POWER_ACT_HIGH_CURRENT_TIME_MS);

	SET_U16(&(msg.data[14]), POWER_V_TO_ANALOG(POWER_ACCU_VOLTAGE_OK_LEVEL));

	SET_U16(&(msg.data[16]), POWER_V_TO_ANALOG(POWER_CHARGE_VOLTAGE_READY_LEVEL));
	SET_U16(&(msg.data[18]), POWER_V_TO_ANALOG(POWER_CHARGE_VOLTAGE_PLUGGED_IN_LEVEL));

	UDPdriver::send(&msg);

}


double node_Power::GET_MAIN_VOLTAGE(void){

	return POWER_ANALOG_TO_V(PowerMainVoltage);

}


bool node_Power::GET_ACCU_VOLTAGE_OK(void){

	return PowerAccuVoltageOk;

}


bool node_Power::GET_STOP_BUTTON(void){

	return PowerStopButton;

}


//visszaadja, hogy valtozott-e
bool node_Power::GET_ACT_ON(bool* ActOn){

	(*ActOn) = PowerActOn;

	bool ret;

	//ha vatozott
	if(PowerActOn_prev != PowerActOn)
		ret = true;

	//ha nem vatozott
	else
		ret = false;


	PowerActOn_prev = PowerActOn;

	return ret;

}

#endif /*NAGY_ROBOT*/
