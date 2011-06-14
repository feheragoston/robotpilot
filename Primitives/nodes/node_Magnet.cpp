/*
 * node_Magnet.cpp
 *
 *  Created on: Feb 17, 2011
 *      Author: triszalo
 */





#include "include/node_Magnet.h"

using namespace std;


node_Magnet::node_Magnet(void) : node(MAGNET_ID, "node_Magnet", MAGNET_KEEP_ALIVE_MS, MAGNET_SEND_PERIOD_TO_PC_MS, MAGNET_SEND_PERIOD_TO_NODE_MS){


	//----- valtozo init ELEJE -----
	Magnet_Voltage_Pull[MAGNET_LEFT_INDEX]		=	(MAGNET_LEFT_PULL_IS_p24V ? MAGNET_p24V : MAGNET_m24V);
	Magnet_Voltage_Off[MAGNET_LEFT_INDEX]		=	MAGNET_0V;
	Magnet_Voltage_Push[MAGNET_LEFT_INDEX]		=	(MAGNET_LEFT_PULL_IS_p24V ? MAGNET_m24V : MAGNET_p24V);

	Magnet_Voltage_Pull[MAGNET_RIGHT_INDEX]		=	(MAGNET_RIGHT_PULL_IS_p24V ? MAGNET_p24V : MAGNET_m24V);
	Magnet_Voltage_Off[MAGNET_RIGHT_INDEX]		=	MAGNET_0V;
	Magnet_Voltage_Push[MAGNET_RIGHT_INDEX]		=	(MAGNET_RIGHT_PULL_IS_p24V ? MAGNET_m24V : MAGNET_p24V);
	//----- valtozo init VEGE -----


}


node_Magnet::~node_Magnet(){

}


void node_Magnet::evalMsg(UDPmsg* msg){

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

			case MSG_MAGNET_SET_POLARITY_REPLY:
				num = GET_U8(&(msg->data[0]));
				SetPolarity[num].done = GET_BOOL(&(msg->data[1]), 0);
				SetPolarity[num].inProgress = false;
				SetPolarity[num].finished = true;
				cout << name << "\t___recv SETPOLARITY [" << (u32)num << "]___:\t" << (SetPolarity[num].done?"1":"0") << endl;
				break;

			default:
				break;


		}

	}

}


void node_Magnet::MAGNET_SET_POLARITY(u8 num, s8 polarity){

	UDPmsg msg;

	u8 voltage;

	if(polarity == MAGNET_PUSH)			voltage = Magnet_Voltage_Push[num];
	else if(polarity == MAGNET_OFF)		voltage = Magnet_Voltage_Off[num];
	else if(polarity == MAGNET_PULL)	voltage = Magnet_Voltage_Pull[num];

	msg.node_id		= id;
	msg.function	= CMD_MAGNET_SET_POLARITY;
	msg.length		= 2;
	SET_U8(&(msg.data[0]), num);
	SET_U8(&(msg.data[1]), voltage);

	UDPdriver::send(&msg);

	SetPolarity[num].inProgress = true;
	SetPolarity[num].finished = false;

	cout << name << "\t___send SETPOLARITY [" << (u32)num << "]___:\t" << (u32)voltage << endl;

}


void node_Magnet::INIT_PARAM(void){

	UDPmsg msg;

	msg.node_id		= id;
	msg.function	= CMD_INIT_PARAM;
	msg.length		= 0;

	UDPdriver::send(&msg);

}
