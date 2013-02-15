#include "include/node_Caracole.h"

using namespace std;


node_Caracole::node_Caracole(void) : node(CARACOLE_ID, "node_Caracole", CARACOLE_KEEP_ALIVE_MS, CARACOLE_SEND_PERIOD_TO_PC_MS, CARACOLE_SEND_PERIOD_TO_NODE_MS){


	//----- valtozo init ELEJE -----
	CaracoleSpeed = 0;
	//----- valtozo init VEGE -----


}


node_Caracole::~node_Caracole(){

}


void node_Caracole::evalMsg(UDPmsg* msg){

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
				CaracoleSpeed	= GET_FLOAT(&(msg->data[0]));
				break;

			case MSG_CARACOLE_SET_SPEED_REPLY:
				SetSpeed.done = GET_BOOL(&(msg->data[0]), 0);
				SetSpeed.inProgress = false;
				SetSpeed.finished = true;
				cout << name << "\t___recv SETSPEED___:\t" << (SetSpeed.done?"1":"0") << endl;
				break;

			default:
				break;


		}

	}

}


void node_Caracole::CARACOLE_SET_SPEED(double speed, double acc){

	UDPmsg msg;

	msg.node_id		= id;
	msg.function	= CMD_CARACOLE_SET_SPEED;
	msg.length		= 8;

	SET_U32(&(msg.data[0]), CARACOLE_CONV_RADS_TO_INCRS(speed));
	SET_U32(&(msg.data[4]), CARACOLE_CONV_RADS2_TO_INCRS2(acc));

	UDPdriver::send(&msg);

	SetSpeed.inProgress = true;
	SetSpeed.finished = false;

	cout << name << "\t___send SETSPEED___:\t" << CARACOLE_CONV_RADS_TO_INCRS(speed) << "\t" << CARACOLE_CONV_RADS2_TO_INCRS2(acc) << endl;

}


void node_Caracole::INIT_PARAM(void){

	UDPmsg msg;

	msg.node_id		= id;
	msg.function	= CMD_INIT_PARAM;
	msg.length		= 13;

	SET_FLOAT(&(msg.data[0]), CARACOLE_CONTROLLER_PARAMETER_P);
	SET_FLOAT(&(msg.data[4]), CARACOLE_CONTROLLER_PARAMETER_I);
	SET_FLOAT(&(msg.data[8]), CARACOLE_CONTROLLER_PARAMETER_D);
	SET_BOOL(&(msg.data[12]), 0, (CARACOLE_IS_ENC_EQEP1 != 0) ? true : false);
	SET_BOOL(&(msg.data[12]), 1, (CARACOLE_MOTOR_PLUS_UP != 0) ? true : false);
	SET_BOOL(&(msg.data[12]), 2, (CARACOLE_ENC_IS_ROTATE_DIR_A != 0) ? true : false);

	UDPdriver::send(&msg);

}


double node_Caracole::GET_SPEED(void){

	return CARACOLE_CONV_INCRS_TO_RADS(CaracoleSpeed);

}
