/*
 * node_Input.h
 *
 *  Created on: Feb 17, 2011
 *      Author: triszalo
 */


#ifndef NODE_INPUT_H_
#define NODE_INPUT_H_





//------------------------------ include ELEJE ------------------------------
#include "node.h"
//------------------------------ include VEGE ------------------------------





//------------------------------ node_Input ELEJE ------------------------------
class node_Input : public node{

public:

	node_Input(void);
	~node_Input();

	void evalMsg(UDPmsg* msg);

	void INIT_PARAM(void);

	double GET_DISTANCE(u8 analog_num);
	bool GET_DIGITAL(u8 digital_num);


private:

	u16 analog[INPUT_ANALOG_COUNT];
	bool digital[INPUT_DIGITAL_COUNT];

	bool analog_has_long_sharp[INPUT_ANALOG_COUNT];
	bool active_level_digital[INPUT_DIGITAL_COUNT];

	double GET_SHARP_MM(u16 analog_value, double table[][2], u8 size);


};
//------------------------------ node_Input VEGE ------------------------------





#endif /*NODE_INPUT_H_*/
