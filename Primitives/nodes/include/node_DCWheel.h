/*
 * node_DCWheel.h
 *
 *  Created on: Feb 17, 2011
 *      Author: triszalo
 */


#ifndef NODE_DCWHEEL_H_
#define NODE_DCWHEEL_H_





//------------------------------ include ELEJE ------------------------------
#include "node.h"
//------------------------------ include VEGE ------------------------------





//------------------------------ node_DCWheel ELEJE ------------------------------
class node_DCWheel : public node{

public:

	node_DCWheel(void);
	~node_DCWheel();

	void evalMsg(UDPmsg* msg);

	void INIT_PARAM(void);


	void DCWHEEL_STOP(double acc);
	void DCWHEEL_HARD_STOP(void);
	void DCWHEEL_GO(double distance, double max_speed, double max_acc);
	void DCWHEEL_GOTO(double x, double y, double max_speed, double max_acc);
	void DCWHEEL_TURN(double angle, double max_speed, double max_acc);
	void DCWHEEL_SET_SPEED(double v, double w);
	void DCWHEEL_SET_WHEELSPEED(double vLeft, double vRight);


	progress_t AnyStop;
	progress_t AnyMotion;
	progress_t AnySpeed;


};
//------------------------------ node_DCWheel VEGE ------------------------------





#endif /*NODE_DCWHEEL_H_*/
