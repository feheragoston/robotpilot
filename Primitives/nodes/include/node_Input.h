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


public:

	double distance_mm[INPUT_DISTANCE_COUNT];
	bool button[INPUT_BUTTON_COUNT];


};
//------------------------------ node_Input VEGE ------------------------------





#endif /*NODE_INPUT_H_*/
