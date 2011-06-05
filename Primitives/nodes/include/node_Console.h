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

	void evalMsg(UDPmsg* msg);

	void INIT_PARAM(void);


	void CONSOLE_STOP(void);
	void CONSOLE_SET_POS(double pos, double speed, double acc);
	void CONSOLE_CALIBRATE(void);


	progress_t Stop;
	progress_t SetPos;
	progress_t Calibrate;


	double GET_POS(void);


private:

	u32 ConsolePos;

};
//------------------------------ node_Console VEGE ------------------------------





#endif /*NODE_CONSOLE_H_*/
