/*
 * node_Deadreck.cpp
 *
 *  Created on: Feb 17, 2011
 *      Author: triszalo
 */





#include "include/node_Deadreck.h"

using namespace std;


node_Deadreck::node_Deadreck(void){

	id = DEADRECK_ID;
	strcpy(name, "node_Deadreck");


	//----- valtozo init ELEJE -----
	pos_x = 0;
	pos_y = 0;
	pos_phi = 0;

	resetpos_inProgress = false;

	resetpos_finished = false;
	//----- valtozo init VEGE -----

}


node_Deadreck::~node_Deadreck(){

}


void node_Deadreck::evalMsg(UDPmsg* msg){

	//ha o a cimzett, csak akkor dolgozza fel
	if(msg->node_id == id){

		switch(msg->function){

			case MSG_PING_REPLY:
				sem_post(&pingSemaphore);
				break;

			case MSG_RESET_POS_REPLY:
				resetpos_inProgress = false;
				resetpos_finished = true;
				break;

			case MSG_POS:
				//csak akkor taroljuk el a fogadott poziciokat, ha nincs folyamatban reset_pos
				if(!resetpos_inProgress){
					pos_x	= *(float*)(&(msg->data[0]));
					pos_y	= *(float*)(&(msg->data[4]));
					pos_phi	= *(float*)(&(msg->data[8]));
				}
				break;

			default:
				break;


		}

	}

}


void node_Deadreck::RESET_POS(void){

	UDPmsg msg;

	msg.node_id		= id;
	msg.function	= CMD_RESET_POS;
	msg.length		= 0;

	UDPdriver::send(&msg);

	resetpos_inProgress = true;
	resetpos_finished = false;

}
