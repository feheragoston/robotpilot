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

	//----- mindegyik node-nak van ELEJE -----
	void evalMsg(UDPmsg* msg);
	//----- mindegyik node-nak van VEGE -----


	//----- function ELEJE -----
public:

	static const unsigned int	MAGNET_COUNT		= 2;

	void SET_MAGNET(unsigned int num, int polarity);
	bool setmagnet_inProgress[MAGNET_COUNT];
	bool setmagnet_finished[MAGNET_COUNT];
	static const unsigned char	polarityPull	= 0x01;	//01: pull	(vonz)
	static const unsigned char	polarityOff		= 0x00;	//00: off	(kikapcsolt)
	static const unsigned char	polarityPush	= 0xFF;	//FF: push	(taszit)

private:

	static const unsigned int	CMD_SET_MAGNET0					= 0x03;	//length = 1, data[0] = polarity
	static const unsigned int	MSG_SET_MAGNET0_REPLY			= 0x03;	//length = 0

	static const unsigned int	CMD_SET_MAGNET1					= 0x04;
	static const unsigned int	MSG_SET_MAGNET1_REPLY			= 0x04;
	//----- function VEGE -----


};
//------------------------------ node_Magnet VEGE ------------------------------





#endif /*NODE_MAGNET_H_*/
