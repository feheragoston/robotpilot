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

	void INIT_PARAM(void);


	void POWER_ACT_ON();
	void POWER_ACT_OFF();
	void POWER_MAIN_OFF();

	progress_t act_on_off;


	double GET_MAIN_VOLTAGE(void);
	bool GET_ACCU_VOLTAGE_OK(void);
	bool GET_STOP_BUTTON(void);


private:

	u16 PowerMainVoltage;
	bool PowerAccuVoltageOk;
	bool PowerStopButton;


};
//------------------------------ node_Power VEGE ------------------------------





#endif /*NODE_POWER_H_*/