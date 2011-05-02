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


	void DEADRECK_RESET_POS(void);


	bool reset_pos_inProgress;
	bool reset_pos_finished;


	double pos_x;
	double pos_y;
	double pos_phi;


};
//------------------------------ node_Deadreck VEGE ------------------------------





#endif /*NODE_DEADRECK_H_*/
