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

	//----- mindegyik node-nak van ELEJE -----
	void evalMsg(UDPmsg* msg);
	//----- mindegyik node-nak van VEGE -----


	//----- function ELEJE -----
public:

	static const unsigned int	SERVO_COUNT		= 6;

	void SET_SERVO(unsigned int num, double pos, double speed, double acc);
	bool setservo_inProgress[SERVO_COUNT];
	bool setservo_finished[SERVO_COUNT];

private:

	static const unsigned int	CMD_SET_SERVO0					= 0x00;	//length = 12, data[0..11] = pos, speed, acc, mert float ertekek
	static const unsigned int	MSG_SET_SERVO0_REPLY			= 0x00;	//length = 0

	static const unsigned int	CMD_SET_SERVO1					= 0x01;
	static const unsigned int	MSG_SET_SERVO1_REPLY			= 0x01;

	static const unsigned int	CMD_SET_SERVO2					= 0x02;
	static const unsigned int	MSG_SET_SERVO2_REPLY			= 0x02;

	static const unsigned int	CMD_SET_SERVO3					= 0x03;
	static const unsigned int	MSG_SET_SERVO3_REPLY			= 0x03;

	static const unsigned int	CMD_SET_SERVO4					= 0x04;
	static const unsigned int	MSG_SET_SERVO4_REPLY			= 0x04;

	static const unsigned int	CMD_SET_SERVO5					= 0x05;
	static const unsigned int	MSG_SET_SERVO5_REPLY			= 0x05;
	//----- function VEGE -----


};
//------------------------------ node_Servo VEGE ------------------------------





#endif /*NODE_SERVO_H_*/
