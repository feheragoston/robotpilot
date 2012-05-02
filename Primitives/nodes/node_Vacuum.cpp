/*
 * node_Vacuum.cpp
 *
 *  Created on: May 02, 2012
 *      Author: triszalo
 */





#include "include/node_Vacuum.h"

using namespace std;


node_Vacuum::node_Vacuum(void) : node(VACUUM_ID, "node_Vacuum", VACUUM_KEEP_ALIVE_MS, VACUUM_SEND_PERIOD_TO_PC_MS, VACUUM_SEND_PERIOD_TO_NODE_MS){


	//----- valtozo init ELEJE -----
	for(u8 i=0 ; i<VACUUM_COUNT ; i++){
		Vacuum_CmdOn[i] = false;
		Vacuum_On[i] = false;
	}
	//----- valtozo init VEGE -----


}


node_Vacuum::~node_Vacuum(){

}


void node_Vacuum::evalMsg(UDPmsg* msg){

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

			case MSG_VACUUM_SET_REPLY:
				num = GET_U8(&(msg->data[0]));
				Vacuum_On[num] = Vacuum_CmdOn[num];
				Set[num].done = GET_BOOL(&(msg->data[1]), 0);
				Set[num].inProgress = false;
				Set[num].finished = true;
				cout << name << "\t___recv SET [" << (u32)num << "]___:\t" << (Set[num].done?"1":"0") << endl;
				break;

			default:
				break;


		}

	}

}


void node_Vacuum::VACUUM_SET(u8 num, bool on){

	UDPmsg msg;

	Vacuum_CmdOn[num] = on;

	msg.node_id		= id;
	msg.function	= CMD_VACUUM_SET;
	msg.length		= 2;
	SET_U8(&(msg.data[0]), num);
	SET_U8(&(msg.data[1]), Vacuum_CmdOn[num]?1:0);

	UDPdriver::send(&msg);

	Set[num].inProgress = true;
	Set[num].finished = false;

	cout << name << "\t___send SET [" << (u32)num << "]___:\t" << (Vacuum_CmdOn[num]?"1":"0") << endl;

}


void node_Vacuum::INIT_PARAM(void){

	UDPmsg msg;

	msg.node_id		= id;
	msg.function	= CMD_INIT_PARAM;
	msg.length		= 0;

	UDPdriver::send(&msg);

}


bool node_Vacuum::GET_ON(u8 num){

	return Vacuum_On[num];

}
