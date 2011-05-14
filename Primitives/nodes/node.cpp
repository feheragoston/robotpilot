/*
 * node.cpp
 *
 *  Created on: Feb 18, 2011
 *      Author: triszalo
 */





#include "include/node.h"

using namespace std;



void node::CORE_RESET(void){

	UDPmsg msg;

	msg.node_id		= id;
	msg.function	= CMD_CORE_RESET;
	msg.length		= 0;

	UDPdriver::send(&msg);

}


void node::KEEP_ALIVE(void){

	UDPmsg msg;

	msg.node_id		= id;
	msg.function	= CMD_KEEP_ALIVE;
	msg.length		= 0;

	UDPdriver::send(&msg);

}


void node::SET_KEEP_ALIVE_MS(u16 keep_alive_ms){

	u16*	tmp;

	UDPmsg msg;

	msg.node_id		= id;
	msg.function	= CMD_SET_KEEP_ALIVE_MS;
	msg.length		= 2;
	tmp = (u16*)(&(msg.data[0]));		*tmp = keep_alive_ms;

	UDPdriver::send(&msg);

}


void node::SET_SEND_PERIOD_TO_NODE_MS(u16 period_ms){

	u16*	tmp;

	UDPmsg msg;

	msg.node_id		= id;
	msg.function	= CMD_SET_SEND_PERIOD_TO_NODE_MS;
	msg.length		= 2;
	tmp = (u16*)(&(msg.data[0]));		*tmp = period_ms;

	UDPdriver::send(&msg);

}


void node::SET_SEND_PERIOD_TO_PC_MS(u16 period_ms){

	u16*	tmp;

	UDPmsg msg;

	msg.node_id		= id;
	msg.function	= CMD_SET_SEND_PERIOD_TO_PC_MS;
	msg.length		= 2;
	tmp = (u16*)(&(msg.data[0]));		*tmp = period_ms;

	UDPdriver::send(&msg);

}


void node::PING(void){

	UDPmsg msg;

	msg.node_id		= id;
	msg.function	= CMD_PING;
	msg.length		= 0;

	UDPdriver::send(&msg);

}


void node::PINGprocess(void){

	sem_init(&pingSemaphore, 0, 0);

	PING();

	timespec ts;
	clock_gettime(CLOCK_REALTIME, &ts);
	ts.tv_sec += PING_REPLY_MAX_WAIT_TIME_SEC;

	if(sem_timedwait(&pingSemaphore, &ts) == 0)
		cout << "-> ping OK!\t" << name << "(" << id << ")" << endl;
	else
		cerr << "-> ping ERROR!\t" << name << "(" << id << ")" << endl;

}
