/*
 * node_Console.h
 *
 *  Created on: Feb 17, 2011
 *      Author: triszalo
 */


#ifndef NODE_CONSOLE_H_
#define NODE_CONSOLE_H_





//------------------------------ include ELEJE ------------------------------
#include "node.h"
//------------------------------ include VEGE ------------------------------





//------------------------------ node_Console ELEJE ------------------------------
class node_Console : public node{

public:

	node_Console(void);
	~node_Console();

	//----- mindegyik node-nak van ELEJE -----
	void evalMsg(UDPmsg* msg);
	//----- mindegyik node-nak van VEGE -----


	//----- function ELEJE -----
public:

	void STOP(void);
	bool stop_inProgress;
	bool stop_finished;

	void SET_POS(double pos, double speed, double acc);
	bool setpos_inProgress;
	bool setpos_finished;

	void CALIBRATE(void);
	bool calibrate_inProgress;
	bool calibrate_finished;

private:

	static const unsigned int	CMD_STOP				= 0x00;		//length = 0
	static const unsigned int	MSG_STOP_REPLY			= 0x00;		//length = 0

	static const unsigned int	CMD_SET_POS				= 0x01;		//length = 12, data[0..11] = pos, speed, acc, mert float ertekek
	static const unsigned int	MSG_SET_POS_REPLY		= 0x01;		//length = 0

	static const unsigned int	CMD_CALIBRATE			= 0x02;		//length = 0
	static const unsigned int	MSG_CALIBRATE_REPLY		= 0x02;		//length = 0

	static const unsigned int	MSG_POS					= 0x0F;		//length = 4, data[0..3] = pos, mert float ertek

public:

	double pos;
	//----- function VEGE -----


};
//------------------------------ node_Console VEGE ------------------------------





#endif /*NODE_CONSOLE_H_*/
