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


	void GATEWAY_REMOVE_NODE(unsigned int node_id);
	void GATEWAY_ADD_NODE_CANA(unsigned int node_id);
	void GATEWAY_ADD_NODE_CANB(unsigned int node_id);
	void GATEWAY_ADD_BOOTLOADER_ID(unsigned int node_id);


};
//------------------------------ node_Gateway VEGE ------------------------------





#endif /*NODE_GATEWAY_H_*/
