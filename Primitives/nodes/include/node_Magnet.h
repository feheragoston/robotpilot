/*
 * node_Magnet.h
 *
 *  Created on: Feb 17, 2011
 *      Author: triszalo
 */


#ifndef NODE_MAGNET_H_
#define NODE_MAGNET_H_





//------------------------------ include ELEJE ------------------------------
#include "node.h"
//------------------------------ include VEGE ------------------------------





//------------------------------ node_Magnet ELEJE ------------------------------
class node_Magnet : public node{

public:

	node_Magnet(void);
	~node_Magnet();

	void evalMsg(UDPmsg* msg);

	void INIT_PARAM(void);

	void MAGNET_SET_POLARITY(u8 num, u8 polarity);


	progress_t set_polarity[MAGNET_COUNT];


};
//------------------------------ node_Magnet VEGE ------------------------------





#endif /*NODE_MAGNET_H_*/