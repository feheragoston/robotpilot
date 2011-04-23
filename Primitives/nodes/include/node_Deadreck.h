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

	//----- mindegyik node-nak van ELEJE -----
	void evalMsg(UDPmsg* msg);
	//----- mindegyik node-nak van VEGE -----


	//----- function TX ELEJE -----
public:

	void RESET_POS(void);
	bool resetpos_inProgress;
	bool resetpos_finished;

private:

	static const unsigned int	CMD_RESET_POS			= 0x00;		//length = 0
	static const unsigned int	MSG_RESET_POS_REPLY		= 0x00;		//length = 0

	static const unsigned int	MSG_POS					= 0x0F;		//length = 12, data[0..11] = x, y, phi, mert float ertekek

public:

	double pos_x;
	double pos_y;
	double pos_phi;
	//----- function RX VEGE -----


};
//------------------------------ node_Deadreck VEGE ------------------------------





#endif /*NODE_DEADRECK_H_*/
