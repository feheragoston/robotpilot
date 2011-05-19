/*
 * node_Broadcast.cpp
 *
 *  Created on: Feb 24, 2011
 *      Author: triszalo
 */





#include "include/node_Broadcast.h"

using namespace std;


node_Broadcast::node_Broadcast(void) : node(BROADCAST_ID, "node_Broadcast", BROADCAST_KEEP_ALIVE_MS, BROADCAST_SEND_PERIOD_TO_PC_MS, BROADCAST_SEND_PERIOD_TO_NODE_MS){

}


node_Broadcast::~node_Broadcast(){

}


void node_Broadcast::PINGprocess(void){

	cerr << "NO PINGprocess for\t" << name << "(" << id << ")" << endl;

}


void node_Broadcast::INIT_PARAM(void){

	cerr << "NO INIT_PARAM for\t" << name << "(" << id << ")" << endl;

}
