/*
 * node.cpp
 *
 *  Created on: Feb 18, 2011
 *      Author: triszalo
 */





#include "include/node.h"

using namespace std;



node::node(u16 id, const char* name, u32 keepAlive_ms, u32 sendPeriodToPC_ms, u32 sendPeriodToNode_ms){

	this->id = id;

	strcpy(this->name, name);

	this->keepAlive_ms = keepAlive_ms;
	this->sendPeriodToPC_ms = sendPeriodToPC_ms;
	this->sendPeriodToNode_ms = sendPeriodToNode_ms;

}



node::~node(){

}


void node::CORE_RESET(void){

	UDPmsg msg;

	msg.node_id		= id;
	msg.function	= CMD_CORE_RESET;
	msg.length		= 0;

	UDPdriver::send(&msg);

}


void node::START_ACTUATOR(void){

	UDPmsg msg;

	msg.node_id		= id;
	msg.function	= CMD_START_ACTUATOR;
	msg.length		= 0;

	UDPdriver::send(&msg);

}


void node::STOP_ACTUATOR(void){

	UDPmsg msg;

	msg.node_id		= id;
	msg.function	= CMD_STOP_ACTUATOR;
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


void node::SET_KEEP_ALIVE_MS(void){

	UDPmsg msg;

	msg.node_id		= id;
	msg.function	= CMD_SET_KEEP_ALIVE_MS;
	msg.length		= 2;
	SET_U16(&(msg.data[0]), keepAlive_ms);

	UDPdriver::send(&msg);

}


void node::SET_SEND_PERIOD_TO_NODE_MS(void){

	UDPmsg msg;

	msg.node_id		= id;
	msg.function	= CMD_SET_SEND_PERIOD_TO_NODE_MS;
	msg.length		= 2;
	SET_U16(&(msg.data[0]), sendPeriodToNode_ms);

	UDPdriver::send(&msg);

}


void node::SET_SEND_PERIOD_TO_PC_MS(void){

	UDPmsg msg;

	msg.node_id		= id;
	msg.function	= CMD_SET_SEND_PERIOD_TO_PC_MS;
	msg.length		= 2;
	SET_U16(&(msg.data[0]), sendPeriodToPC_ms);

	UDPdriver::send(&msg);

}


void node::PING(void){

	UDPmsg msg;

	msg.node_id		= id;
	msg.function	= CMD_PING;
	msg.length		= 0;

	UDPdriver::send(&msg);

}


void node::INIT_PARAM(void){

}


void node::evalMsg(UDPmsg* msg){

}


bool node::PINGprocess(void){

	sem_init(&pingSemaphore, 0, 0);

	PING();

	timespec ts;
	clock_gettime(CLOCK_REALTIME, &ts);
	ts.tv_sec += PING_REPLY_MAX_WAIT_TIME_SEC;

	//ha jott valasz
	if(sem_timedwait(&pingSemaphore, &ts) == 0){
		cout << "-> ping OK!\t\t" << name << "(" << id << ")" << endl;
		return true;
	}

	//ha nem jott valasz
	else{
		cerr << "-> ping TIMED OUT!\t" << name << "(" << id << ")" << endl;
		return false;
	}

}


bool node::INITPARAMprocess(void){

	sem_init(&initparamSemaphore, 0, 0);

	INIT_PARAM();

	timespec ts;
	clock_gettime(CLOCK_REALTIME, &ts);
	ts.tv_sec += INIT_PARAM_REPLY_MAX_WAIT_TIME_SEC;

	//ha jott valasz
	if(sem_timedwait(&initparamSemaphore, &ts) == 0){

		//ha rendben vannak a parameterek
		if(initparamOK){
			cout << "-> initparam OK!\t" << name << "(" << id << ")" << endl;
			return true;
		}

		//ha nincsenek rendben a parameterek
		else{
			cout << "-> initparam ERROR!\t" << name << "(" << id << ")" << endl;
			return false;
		}

	}

	//ha nem jott valasz
	else{
		cerr << "-> initparam TIMED OUT!\t" << name << "(" << id << ")" << endl;
		return false;
	}

}
