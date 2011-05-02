/*
 * node_Servo.h
 *
 *  Created on: Feb 17, 2011
 *      Author: triszalo
 */


#ifndef NODE_SERVO_H_
#define NODE_SERVO_H_





//------------------------------ include ELEJE ------------------------------
#include "node.h"
//------------------------------ include VEGE ------------------------------





//------------------------------ node_Servo ELEJE ------------------------------
class node_Servo : public node{

public:

	node_Servo(void);
	~node_Servo();

	void evalMsg(UDPmsg* msg);


	void SERVO_SET_POS(unsigned int num, double pos, double speed, double acc);


	bool move_inProgress[SERVO_COUNT];
	bool move_finished[SERVO_COUNT];


};
//------------------------------ node_Servo VEGE ------------------------------





#endif /*NODE_SERVO_H_*/
