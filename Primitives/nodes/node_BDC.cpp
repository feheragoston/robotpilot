/*
 * node_BDC.cpp
 *
 *  Created on: Feb 17, 2011
 *      Author: triszalo
 */





#include "include/node_BDC.h"

using namespace std;


node_BDC::node_BDC(void) : node(BDC_ID, "node_BDC", BDC_KEEP_ALIVE_MS, BDC_SEND_PERIOD_TO_PC_MS, BDC_SEND_PERIOD_TO_NODE_MS){


	//----- valtozo init ELEJE -----
	//----- valtozo init VEGE -----


}


node_BDC::~node_BDC(){

}


void node_BDC::evalMsg(UDPmsg* msg){

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

			case MSG_BDC_STOP_REPLY:
			case MSG_BDC_HARD_STOP_REPLY:
				AnyStop.done = GET_BOOL(&(msg->data[0]), 0);
				AnyStop.inProgress = false;
				AnyStop.finished = true;
				cout << name << "\t___recv ANYSTOP:\t" << (AnyStop.done?"1":"0") << endl;
				break;

			case MSG_BDC_GO_REPLY:
				AnyMotion.done = GET_BOOL(&(msg->data[0]), 0);
				AnyMotion.inProgress = false;
				AnyMotion.finished = true;
				cout << name << "\t___recv GO:\t" << (AnyMotion.done?"1":"0") << endl;
				break;

			case MSG_BDC_GOTO_REPLY:
				AnyMotion.done = GET_BOOL(&(msg->data[0]), 0);
				AnyMotion.inProgress = false;
				AnyMotion.finished = true;
				cout << name << "\t___recv GOTO:\t" << (AnyMotion.done?"1":"0") << endl;
				break;

			case MSG_BDC_TURN_REPLY:
				AnyMotion.done = GET_BOOL(&(msg->data[0]), 0);
				AnyMotion.inProgress = false;
				AnyMotion.finished = true;
				cout << name << "\t___recv TURN:\t" << (AnyMotion.done?"1":"0") << endl;
				break;

			case MSG_BDC_SET_SPEED_REPLY:
				AnySpeed.done = GET_BOOL(&(msg->data[0]), 0);
				AnySpeed.inProgress = false;
				AnySpeed.finished = true;
				cout << name << "\t___recv SETSPEED:\t" << (AnySpeed.done?"1":"0") << endl;
				break;

			default:
				break;


		}

	}

}


void node_BDC::BDC_STOP(double acc){

	UDPmsg msg;

	msg.node_id		= id;
	msg.function	= CMD_BDC_STOP;
	msg.length		= 2;
	SET_U16(&(msg.data[0]), BDC_CONV_ACC(acc));

	UDPdriver::send(&msg);

	AnyStop.inProgress = true;
	AnyStop.finished = false;

	cout << name << "\t___send STOP___:\t" << BDC_CONV_ACC(acc) << endl;

}


void node_BDC::BDC_HARD_STOP(void){

	UDPmsg msg;

	msg.node_id		= id;
	msg.function	= CMD_BDC_HARD_STOP;
	msg.length		= 0;

	UDPdriver::send(&msg);

	AnyStop.inProgress = true;
	AnyStop.finished = false;

	cout << name << "\t___send HARDSTOP___\t" << endl;

}


void node_BDC::BDC_GO(double distance, double max_speed, double max_acc){

	UDPmsg msg;

	msg.node_id		= id;
	msg.function	= CMD_BDC_GO;
	msg.length		= 8;
	SET_FLOAT(&(msg.data[0]), distance);
	SET_U16(&(msg.data[4]), BDC_CONV_SPEED(max_speed));
	SET_U16(&(msg.data[6]), BDC_CONV_ACC(max_acc));

	UDPdriver::send(&msg);

	AnyMotion.inProgress = true;
	AnyMotion.finished = false;

	cout << name << "\t___send GO___:\t" << distance << "\t" << BDC_CONV_SPEED(max_speed) << "\t" << BDC_CONV_ACC(max_acc) << endl;

}


void node_BDC::BDC_GOTO(double x, double y, double max_speed, double max_acc){

	UDPmsg msg;

	msg.node_id		= id;
	msg.function	= CMD_BDC_GOTO;
	msg.length		= 12;
	SET_FLOAT(&(msg.data[0]), x);
	SET_FLOAT(&(msg.data[4]), y);
	SET_U16(&(msg.data[8]), BDC_CONV_SPEED(max_speed));
	SET_U16(&(msg.data[10]), BDC_CONV_ACC(max_acc));

	UDPdriver::send(&msg);

	AnyMotion.inProgress = true;
	AnyMotion.finished = false;

	cout << name << "\t___send GOTO___:\t" << x << "\t" << y << "\t" << BDC_CONV_SPEED(max_speed) << "\t" << BDC_CONV_ACC(max_acc) << endl;

}


void node_BDC::BDC_TURN(double angle, double max_speed, double max_acc){

	UDPmsg msg;

	msg.node_id		= id;
	msg.function	= CMD_BDC_TURN;
	msg.length		= 8;
	SET_FLOAT(&(msg.data[0]), angle);
	SET_U16(&(msg.data[4]), BDC_CONV_OMEGA(max_speed));
	SET_U16(&(msg.data[6]), BDC_CONV_BETA(max_acc));

	UDPdriver::send(&msg);

	AnyMotion.inProgress = true;
	AnyMotion.finished = false;

	cout << name << "\t___send TURN___:\t" << angle << "\t" << BDC_CONV_OMEGA(max_speed) << "\t" << BDC_CONV_BETA(max_acc) << endl;

}


void node_BDC::BDC_SET_SPEED(double v, double w){

	UDPmsg msg;

	msg.node_id		= id;
	msg.function	= CMD_BDC_SET_SPEED;
	msg.length		= 4;

	//v = (vLeft + vRight) / 2
	//w = (vRight - vLeft) / BDC_WHEEL_DISTANCEyy
	s16 vLeft	= (s16)(v - w * BDC_WHEEL_DISTANCE / 2);
	s16 vRight	= (s16)(v - w * BDC_WHEEL_DISTANCE / 2);

	//bal motornak meg kell forditani a sebesseget
	SET_S16(&(msg.data[0]), BDC_CONV_SPEED((BDC_IS_LEFT_MOTOR1 != 0) ? -vLeft : vRight));
	SET_S16(&(msg.data[2]), BDC_CONV_SPEED((BDC_IS_LEFT_MOTOR1 != 0) ? vRight : -vLeft));

	UDPdriver::send(&msg);

	AnySpeed.inProgress = true;
	AnySpeed.finished = false;

	cout << name << "\t___send SETSPEED___:\t" << BDC_CONV_SPEED(((BDC_IS_LEFT_MOTOR1 != 0) ? -vLeft : vRight)) << "\t" << BDC_CONV_SPEED(((BDC_IS_LEFT_MOTOR1 != 0) ? vRight : -vLeft)) << endl;

}


void node_BDC::BDC_SET_WHEELSPEED(double vLeft, double vRight){

	UDPmsg msg;

	msg.node_id		= id;
	msg.function	= CMD_BDC_SET_SPEED;
	msg.length		= 4;

	//bal motornak meg kell forditani a sebesseget
	SET_S16(&(msg.data[0]), (s16)((BDC_IS_LEFT_MOTOR1 != 0) ? -vLeft : vRight));
	SET_S16(&(msg.data[2]), (s16)((BDC_IS_LEFT_MOTOR1 != 0) ? vRight : -vLeft));

	UDPdriver::send(&msg);

	AnySpeed.inProgress = true;
	AnySpeed.finished = false;

	cout << name << "\t___send SETWHEELSPEED___:\t" << (s16)(((BDC_IS_LEFT_MOTOR1 != 0) ? -vLeft : vRight)) << "\t" << (s16)(((BDC_IS_LEFT_MOTOR1 != 0) ? vRight : -vLeft)) << endl;

}


void node_BDC::INIT_PARAM(void){

	UDPmsg msg;

	msg.node_id		= id;
	msg.function	= CMD_INIT_PARAM;
	msg.length		= 29;
	SET_BOOL(&(msg.data[0]), 0, BDC_IS_LEFT_MOTOR1);
	SET_FLOAT(&(msg.data[1]), BDC_WHEEL_DISTANCE);
	SET_FLOAT(&(msg.data[5]), BDC_ROBOT_FULL_TURN_INCR);
	SET_FLOAT(&(msg.data[9]), BDC_RPM_TO_MMPS);
	SET_FLOAT(&(msg.data[13]), BDC_MM_TO_INCR);
	SET_FLOAT(&(msg.data[17]), BDC_PRE_BRAKE_TIME);
	SET_FLOAT(&(msg.data[21]), BDC_OE_CONTROL_P);
	SET_FLOAT(&(msg.data[25]), BDC_OE_CONTROL_I);

	UDPdriver::send(&msg);

}
