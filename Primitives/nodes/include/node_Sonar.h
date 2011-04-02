/*
 * node_Sonar.h
 *
 *  Created on: Feb 17, 2011
 *      Author: triszalo
 */


#ifndef NODE_SONAR_H_
#define NODE_SONAR_H_





//------------------------------ include ELEJE ------------------------------
#include "node.h"
//------------------------------ include VEGE ------------------------------





//------------------------------ node_Sonar ELEJE ------------------------------
class node_Sonar : public node{

public:

	node_Sonar(void);
	~node_Sonar();

	//----- mindegyik node-nak van ELEJE -----
	void evalMsg(UDPmsg* msg);
	//----- mindegyik node-nak van VEGE -----


	//----- function ELEJE -----
private:
	static const unsigned int	MSG_POS				= 0x0F;		//length = 8, data[0..7] = x, y, mert float ertekek

public:
	double pos_x;
	double pos_y;
	//----- function VEGE -----


};
//------------------------------ node_Sonar VEGE ------------------------------





#endif /*NODE_SONAR_H_*/
