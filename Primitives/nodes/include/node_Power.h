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


	void POWER_ACT_ON();
	void POWER_ACT_OFF();
	void POWER_CHG_ON();
	void POWER_CHG_OFF();
	void POWER_MAIN_OFF();

	void INIT_PARAM(void);

	progress_t act_on_off;
	progress_t chg_on_off;


	double GET_ACCU_VOLTAGE(void);
	bool GET_ACCU_CHARGING(void);
	bool GET_PLUG_IN(void);
	bool GET_STOP_BUTTON(void);


private:

	u16 PowerChargeVoltage;
	u16 PowerMainVoltage;
	u16 PowerActCurrent;
	u16 PowerMainCurrent;


};
//------------------------------ node_Power VEGE ------------------------------





#endif /*NODE_POWER_H_*/
