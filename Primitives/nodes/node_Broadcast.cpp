/*
 * node_Broadcast.cpp
 *
 *  Created on: Feb 24, 2011
 *      Author: triszalo
 */





#include "include/node_Broadcast.h"

using namespace std;


node_Broadcast::node_Broadcast(void){

	id = BROADCAST_ID;
	strcpy(name, "node_Broadcast");

}


node_Broadcast::~node_Broadcast(){

}


void node_Broadcast::PING(void){

	cerr << "NO PING for\t" << name << "(" << id << ")" << endl;

}


void node_Broadcast::PINGprocess(void){

	cerr << "NO PINGprocess for\t" << name << "(" << id << ")" << endl;

}
