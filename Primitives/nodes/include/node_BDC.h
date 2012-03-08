/*
 * node_BDC.h
 *
 *  Created on: Feb 17, 2011
 *      Author: triszalo
 */


#ifndef NODE_BDC_H_
#define NODE_BDC_H_





//------------------------------ include ELEJE ------------------------------
#include "node.h"
//------------------------------ include VEGE ------------------------------





//------------------------------ node_BDC ELEJE ------------------------------
class node_BDC : public node{

public:

	node_BDC(void);
	~node_BDC();

	void evalMsg(UDPmsg* msg);

	void INIT_PARAM(void);


	void BDC_STOP(double acc);
	void BDC_HARD_STOP(void);
	void BDC_GO(double distance, double max_speed, double max_acc);
	void BDC_GOTO(double x, double y, double max_speed, double max_acc);
	void BDC_TURN(double angle, double max_speed, double max_acc);
	void BDC_SET_SPEED(double v, double w);
	void BDC_SET_WHEELSPEED(double vLeft, double vRight);


	progress_t AnyStop;
	progress_t AnyMotion;
	progress_t SetSpeed;
	progress_t SetWheelSpeed;


};
//------------------------------ node_BDC VEGE ------------------------------





#endif /*NODE_BDC_H_*/
