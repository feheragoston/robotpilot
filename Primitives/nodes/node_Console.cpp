/*
 * node_Console.cpp
 *
 *  Created on: Feb 17, 2011
 *      Author: triszalo
 */





#include "include/node_Console.h"

using namespace std;


node_Console::node_Console(void) : node(CONSOLE_ID, "node_Console", CONSOLE_KEEP_ALIVE_MS, CONSOLE_SEND_PERIOD_TO_PC_MS, CONSOLE_SEND_PERIOD_TO_NODE_MS){


	//----- valtozo init ELEJE -----
	ConsolePos = 0;
	//----- valtozo init VEGE -----


}


node_Console::~node_Console(){

}


void node_Console::evalMsg(UDPmsg* msg){

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
				ConsolePos	= GET_U32(&(msg->data[0]));
				break;

			case MSG_CONSOLE_STOP_REPLY:
				Stop.done = GET_BOOL(&(msg->data[0]), 0);
				Stop.inProgress = false;
				Stop.finished = true;
				cout << name << "\t___recv STOP___:\t" << (Stop.done?"1":"0") << endl;
				break;

			case MSG_CONSOLE_SET_POS_REPLY:
				SetPos.done = GET_BOOL(&(msg->data[0]), 0);
				SetPos.inProgress = false;
				SetPos.finished = true;
				cout << name << "\t___recv SETPOS___:\t" << (SetPos.done?"1":"0") << endl;
				break;

			case MSG_CONSOLE_CALIBRATE_REPLY:
				Calibrate.done = GET_BOOL(&(msg->data[0]), 0);
				Calibrate.inProgress = false;
				Calibrate.finished = true;
				cout << name << "\t___recv CALIB___:\t" << (Calibrate.done?"1":"0") << endl;
				break;

			default:
				break;


		}

	}

}


void node_Console::CONSOLE_STOP(void){

	UDPmsg msg;

	msg.node_id		= id;
	msg.function	= CMD_CONSOLE_STOP;
	msg.length		= 0;

	UDPdriver::send(&msg);

	Stop.inProgress = true;
	Stop.finished = false;

	cout << name << "\t___send STOP___" << endl;

}


void node_Console::CONSOLE_SET_POS(double pos, double speed, double acc){

	UDPmsg msg;

	msg.node_id		= id;
	msg.function	= CMD_CONSOLE_SET_POS;
	msg.length		= 12;

	SET_U32(&(msg.data[0]), CONSOLE_CONV_MM_TO_INCR(pos));
	SET_U32(&(msg.data[4]), CONSOLE_CONV_MMS_TO_INCRS(speed) / 100);	//incr/10ms
	//SET_U32(&(msg.data[8]), CONSOLE_CONV_MMS2_TO_INCRS2(acc));
	SET_U32(&(msg.data[8]), acc);	//1..50

	UDPdriver::send(&msg);

	SetPos.inProgress = true;
	SetPos.finished = false;

	cout << name << "\t___send SETPOS___:\t" << CONSOLE_CONV_MM_TO_INCR(pos) << "\t" << CONSOLE_CONV_MMS_TO_INCRS(speed) << "\t" << CONSOLE_CONV_MMS2_TO_INCRS2(acc) << endl;

}


void node_Console::CONSOLE_CALIBRATE(void){

	UDPmsg msg;

	msg.node_id		= id;
	msg.function	= CMD_CONSOLE_CALIBRATE;
	msg.length		= 0;

	UDPdriver::send(&msg);

	Calibrate.inProgress = true;
	Calibrate.finished = false;

	cout << name << "\t___send CALIB___" << endl;

}


void node_Console::INIT_PARAM(void){

	UDPmsg msg;

	msg.node_id		= id;
	msg.function	= CMD_INIT_PARAM;
	msg.length		= 30;

	SET_U32(&(msg.data[0]), CONSOLE_CONTOLLER_LIMIT_LOW_POS_INCR);
	SET_U32(&(msg.data[4]), CONSOLE_CONTOLLER_LIMIT_HIGH_POS_INCR);
	SET_U32(&(msg.data[8]), CONSOLE_HARDWARE_LIMIT_LOW_POS_INCR);
	SET_U32(&(msg.data[12]), CONSOLE_HARDWARE_LIMIT_HIGH_POS_INCR);
	SET_FLOAT(&(msg.data[16]), CONSOLE_CONTROLLER_PARAMETER_P);
	SET_FLOAT(&(msg.data[20]), CONSOLE_CONTROLLER_PARAMETER_I);
	SET_FLOAT(&(msg.data[24]), CONSOLE_CONTROLLER_PARAMETER_D);
	SET_U8(&(msg.data[28]), CONSOLE_PWM_MODE);
	SET_BOOL(&(msg.data[29]), 0, (CONSOLE_LIMIT_SWITCH_IS_ACTIVE_HIGH != 0) ? true : false);
	SET_BOOL(&(msg.data[29]), 1, (CONSOLE_IS_ENC_EQEP1 != 0) ? true : false);
	SET_BOOL(&(msg.data[29]), 2, (CONSOLE_MOTOR_PLUS_UP != 0) ? true : false);
	SET_BOOL(&(msg.data[29]), 3, (CONSOLE_ENC_IS_ROTATE_DIR_A != 0) ? true : false);

	UDPdriver::send(&msg);

}


double node_Console::GET_POS(void){

	return CONSOLE_CONV_INCR_TO_MM(ConsolePos);

}
