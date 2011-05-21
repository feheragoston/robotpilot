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

	void INIT_PARAM(void);


	void SERVO_SET_POS(u8 num, double pos, double speed, double acc);


	progress_t move[SERVO_COUNT];


private:

	double Servo_Deg_Incr_x0[SERVO_COUNT];
	u16 Servo_Deg_Incr_y0[SERVO_COUNT];
	double Servo_Deg_Incr_x1[SERVO_COUNT];
	u16 Servo_Deg_Incr_y1[SERVO_COUNT];
	double Servo_Deg_Incr_grad[SERVO_COUNT];
	u16 Servo_Limit_Low_Pos_Incr[SERVO_COUNT];
	u16 Servo_Limit_High_Pos_Incr[SERVO_COUNT];

	double SERVO_GET_GRAD(u8 num);
	double SERVO_DEG_TO_INCR(u8 num, double deg);
	u16 SERVO_CONV_POS(u8 num, double pos);
	u16 SERVO_CONV_SPEED(u8 num, double speed);
	u16 SERVO_CONV_ACC(u8 num, double acc);

};
//------------------------------ node_Servo VEGE ------------------------------





#endif /*NODE_SERVO_H_*/
