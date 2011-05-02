/*
 * node_Power.h
 *
 *  Created on: Feb 17, 2011
 *      Author: triszalo
 */


#ifndef NODE_POWER_H_
#define NODE_POWER_H_





//------------------------------ include ELEJE ------------------------------
#include "node.h"
//------------------------------ include VEGE ------------------------------





//------------------------------ node_Power ELEJE ------------------------------
class node_Power : public node{

public:

	node_Power(void);
	~node_Power();

	void evalMsg(UDPmsg* msg);


	void POWER_MOTOR_ON();
	void POWER_MOTOR_OFF();


	bool motor_on_off_inProgress;
	bool motor_on_off_finished;


	bool StopButton;


};
//------------------------------ node_Power VEGE ------------------------------





#endif /*NODE_POWER_H_*/
