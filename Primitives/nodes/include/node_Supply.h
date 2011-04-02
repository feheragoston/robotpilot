/*
 * node_Supply.h
 *
 *  Created on: Feb 17, 2011
 *      Author: triszalo
 */


#ifndef NODE_SUPPLY_H_
#define NODE_SUPPLY_H_





//------------------------------ include ELEJE ------------------------------
#include "node.h"
//------------------------------ include VEGE ------------------------------





//------------------------------ node_Supply ELEJE ------------------------------
class node_Supply : public node{

public:

	node_Supply(void);
	~node_Supply();

	//----- mindegyik node-nak van ELEJE -----
	void evalMsg(UDPmsg* msg);
	//----- mindegyik node-nak van VEGE -----


	//----- function ELEJE -----
public:

	void MOTOR(bool powered);
	bool motor_inProgress;
	bool motor_finished;

private:

	static const unsigned int	CMD_MOTOR_ON			= 0x00;		//length = 0
	static const unsigned int	CMD_MOTOR_OFF			= 0x01;		//length = 0
	static const unsigned int	MSG_MOTOR_REPLY			= 0x00;		//length = 0

	static const unsigned int	MSG_STOP_BUTTON			= 0x0F;		//length = 1, data[0] = stop gomb

public:

	bool StopButton;
	//----- function VEGE -----


};
//------------------------------ node_Supply VEGE ------------------------------





#endif /*NODE_SUPPLY_H_*/
