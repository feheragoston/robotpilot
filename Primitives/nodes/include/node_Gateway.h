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

	//----- mindegyik node-nak van ELEJE -----
	void evalMsg(UDPmsg* msg);
	//----- mindegyik node-nak van VEGE -----


	//----- function ELEJE -----
public:

	void REMOVE_NODE(unsigned int node_id);
	void ADD_NODE_CANA(unsigned int node_id);
	void ADD_NODE_CANB(unsigned int node_id);
	void ADD_BOOTLOADER_ID(unsigned int node_id);

private:

	static const unsigned int	CMD_REMOVE_NODE			= 0x00;	//length = 1, data[0] = node_id
	static const unsigned int	CMD_ADD_NODE_CANA		= 0x01;	//length = 1, data[0] = node_id
	static const unsigned int	CMD_ADD_NODE_CANB		= 0x02;
	static const unsigned int	CMD_ADD_BOOTLOADER_ID	= 0x03;	//length = 1, data[0] = node_id, amivel megegyezo CAN-re kell
	//----- function VEGE -----


};
//------------------------------ node_Gateway VEGE ------------------------------





#endif /*NODE_GATEWAY_H_*/
