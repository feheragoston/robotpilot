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

	void INIT_PARAM(void);

	progress_t motor_on_off;


	double GET_ACCU_VOLTAGE(void);
	bool GET_ACCU_CHARGING(void);
	bool GET_PLUG_IN(void);
	bool GET_STOP_BUTTON(void);


private:

	u16 PowerAccuVoltage;
	bool PowerAccuCharging;
	bool PowerPlugIn;
	bool PowerStopButton;


};
//------------------------------ node_Power VEGE ------------------------------





#endif /*NODE_POWER_H_*/
