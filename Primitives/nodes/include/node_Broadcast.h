/*
 * node_Broadcast.h
 *
 *  Created on: Feb 24, 2011
 *      Author: triszalo
 */


#ifndef NODE_BROADCAST_H_
#define NODE_BROADCAST_H_





//------------------------------ include ELEJE ------------------------------
#include "node.h"
//------------------------------ include VEGE ------------------------------





//------------------------------ node_Broadcast ELEJE ------------------------------
class node_Broadcast : public node{

public:

	node_Broadcast(void);
	~node_Broadcast();

	void PINGprocess(void);	//node_Broadcast-nak nincs PINGprocess-e, felulirjuk


};
//------------------------------ node_Broadcast VEGE ------------------------------





#endif /*NODE_BROADCAST_H_*/
