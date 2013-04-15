/*
 * node_Gateway.h
 *
 *  Created on: Feb 16, 2011
 *      Author: triszalo
 */


#ifndef NODE_GATEWAY_H_
#define NODE_GATEWAY_H_





//------------------------------ include ELEJE ------------------------------
#include "node.h"
//------------------------------ include VEGE ------------------------------





//------------------------------ node_Gateway ELEJE ------------------------------
class node_Gateway : public node{

public:

	node_Gateway(void);
	~node_Gateway();

	void evalMsg(UDPmsg* msg);

	void INIT_PARAM(void);


	void GATEWAY_REMOVE_NODE(u8 node_id);
	void GATEWAY_ADD_NODE_CANA(u8 node_id);
	void GATEWAY_ADD_NODE_CANB(u8 node_id);


};
//------------------------------ node_Gateway VEGE ------------------------------





#endif /*NODE_GATEWAY_H_*/
