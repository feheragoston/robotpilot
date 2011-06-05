/*
 * node_Deadreck.cpp
 *
 *  Created on: Feb 17, 2011
 *      Author: triszalo
 */





#include "include/node_Deadreck.h"

using namespace std;


node_Deadreck::node_Deadreck(void) : node(DEADRECK_ID, "node_Deadreck", DEADRECK_KEEP_ALIVE_MS, DEADRECK_SEND_PERIOD_TO_PC_MS, DEADRECK_SEND_PERIOD_TO_NODE_MS){


	//----- valtozo init ELEJE -----
	DeadreckPosX = 0;
	DeadreckPosY = 0;
	DeadreckPosPhi = 0;
	DeadreckV = 0;
	DeadreckW = 0;
	gettimeofday(&prevMsgTime, NULL);
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

			case MSG_INIT_PARAM_REPLY:
				initparamOK	= GET_BOOL(&(msg->data[0]), 0);
				sem_post(&initparamSemaphore);
				break;

			case MSG_DEADRECK_RESET_POS_REPLY:
				ResetPos.done = GET_BOOL(&(msg->data[0]), 0);
				ResetPos.inProgress = false;
				ResetPos.finished = true;
				if(ResetPos.done){
					DeadreckPosX	= 0;
					DeadreckPosY	= 0;
					DeadreckPosPhi	= 0;
					DeadreckV = 0;
					DeadreckW = 0;
				}
				cout << name << "\t___recv RESETPOS___:\t" << (ResetPos.done?"1":"0") << endl;
				break;

			case MSG_PERIODIC_TO_PC:
				//csak akkor taroljuk el a fogadott poziciokat, ha nincs folyamatban reset_pos
				if(!ResetPos.inProgress){

					struct timeval timeElapsed;
					TimeMeasure(&prevMsgTime, &timeElapsed);
					gettimeofday(&prevMsgTime, NULL);
					double dt = (double)timeElapsed.tv_sec + (double)timeElapsed.tv_usec/1000000;

					double tmpX =  GET_FLOAT(&(msg->data[0]));
					double tmpY =  GET_FLOAT(&(msg->data[4]));
					double tmpPhi	= GET_FLOAT(&(msg->data[8]));

					double dx = tmpX - DeadreckPosX;
					double dy = tmpY - DeadreckPosY;
					double dphi = AngleDist(tmpPhi, DeadreckPosPhi);
					double ds = sqrt(dx*dx + dy*dy);

					double alfa = atan2(dy, dx);
					double direction = fabs(AngleDist(alfa, tmpPhi));

					if (direction > M_PI_2) {
						ds *= -1.;
					}


					DeadreckV = ds / dt;
					DeadreckW = dphi / dt;

					DeadreckPosX	= tmpX;
					DeadreckPosY	= tmpY;
					DeadreckPosPhi	= tmpPhi;


				}
				break;

			default:
				break;


		}

	}

}


void node_Deadreck::DEADRECK_RESET_POS(void){

	UDPmsg msg;

	msg.node_id		= id;
	msg.function	= CMD_DEADRECK_RESET_POS;
	msg.length		= 0;

	UDPdriver::send(&msg);

	ResetPos.inProgress = true;
	ResetPos.finished = false;

	cout << name << "\t___send RESETPOS___" << endl;

}


void node_Deadreck::INIT_PARAM(void){

	UDPmsg msg;

	msg.node_id		= id;
	msg.function	= CMD_INIT_PARAM;
	msg.length		= 13;

	SET_FLOAT(&(msg.data[0]), DEADRECK_WHEEL_DISTANCE_REC);
	SET_FLOAT(&(msg.data[4]), DEADRECK_LEFT_ONE_INCREMENT_DISTANCE);
	SET_FLOAT(&(msg.data[8]), DEADRECK_RIGHT_ONE_INCREMENT_DISTANCE);
	SET_BOOL(&(msg.data[12]), 0, (DEADRECK_LEFT_IS_ROTATE_DIR_A != 0) ? true : false);
	SET_BOOL(&(msg.data[12]), 1, (DEADRECK_RIGHT_IS_ROTATE_DIR_A != 0) ? true : false);
	SET_BOOL(&(msg.data[12]), 2, (DEADRECK_IS_LEFT_EQEP1 != 0) ? true : false);

	UDPdriver::send(&msg);

}


void node_Deadreck::GET_POS(double* x, double* y, double* phi){

	*x = DeadreckPosX;
	*y = DeadreckPosY;
	*phi = DeadreckPosPhi;

}


void node_Deadreck::GET_SPEED(double* v, double* w){

	*v = DeadreckV;
	*w = DeadreckW;

}
