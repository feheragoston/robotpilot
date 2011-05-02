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

	void MAGNET_SET_POLARITY(unsigned int num, char polarity);


	bool set_polarity_inProgress[MAGNET_COUNT];
	bool set_polarity_finished[MAGNET_COUNT];


};
//------------------------------ node_Magnet VEGE ------------------------------





#endif /*NODE_MAGNET_H_*/
