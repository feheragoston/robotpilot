/*
 * node_Sonar.h
 *
 *  Created on: Feb 17, 2011
 *      Author: triszalo
 */


#ifndef NODE_SONAR_H_
#define NODE_SONAR_H_





//------------------------------ include ELEJE ------------------------------
#include "node.h"
//------------------------------ include VEGE ------------------------------





//------------------------------ node_Sonar ELEJE ------------------------------
class node_Sonar : public node{

public:

	node_Sonar(void);
	~node_Sonar();

	void evalMsg(UDPmsg* msg);

	void INIT_PARAM(void);

	long int GET_POS(u8 num, double* x, double* y);


private:

	double SonarPosX[SONAR_TRANSMITTER_COUNT];
	double SonarPosY[SONAR_TRANSMITTER_COUNT];
	double SonarVariance[SONAR_TRANSMITTER_COUNT];
	struct timeval prevMsgTime;


};
//------------------------------ node_Sonar VEGE ------------------------------





#endif /*NODE_SONAR_H_*/
