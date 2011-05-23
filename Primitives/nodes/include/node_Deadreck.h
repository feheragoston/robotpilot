/*
 * node_Deadreck.h
 *
 *  Created on: Feb 17, 2011
 *      Author: triszalo
 */


#ifndef NODE_DEADRECK_H_
#define NODE_DEADRECK_H_





//------------------------------ include ELEJE ------------------------------
#include "node.h"
//------------------------------ include VEGE ------------------------------





//------------------------------ node_Deadreck ELEJE ------------------------------
class node_Deadreck : public node{

public:

	node_Deadreck(void);
	~node_Deadreck();

	void evalMsg(UDPmsg* msg);

	void INIT_PARAM(void);


	void DEADRECK_RESET_POS(void);


	void GET_POS(double* x, double* y, double* phi);

	progress_t reset_pos;


private:

	double DeadreckPosX;
	double DeadreckPosY;
	double DeadreckPosPhi;


};
//------------------------------ node_Deadreck VEGE ------------------------------





#endif /*NODE_DEADRECK_H_*/
