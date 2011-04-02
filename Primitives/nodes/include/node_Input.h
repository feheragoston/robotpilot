/*
 * node_Input.h
 *
 *  Created on: Feb 17, 2011
 *      Author: triszalo
 */


#ifndef NODE_INPUT_H_
#define NODE_INPUT_H_





//------------------------------ include ELEJE ------------------------------
#include "node.h"
//------------------------------ include VEGE ------------------------------





//------------------------------ node_Input ELEJE ------------------------------
class node_Input : public node{

public:

	node_Input(void);
	~node_Input();

	//----- mindegyik node-nak van ELEJE -----
	void evalMsg(UDPmsg* msg);
	//----- mindegyik node-nak van VEGE -----


	//----- function ELEJE -----
public:

	static const unsigned int	DISTANCE_COUNT		= 6;	//6 tavolsagerzekelo
	static const unsigned int	BUTTON_COUNT		= 8;	//Start, Color, RearLeft, RearRight

private:

	static const unsigned int	MSG_BUTTON						= 0x0F;		//length = 1, data[0] = gombok bitenkent
	static const unsigned int	MSG_DISTANCE					= 0x0E;		//length = 12, data[0..11] = distance_num[0..5], mert u16 ertekek

public:

	double distance_mm[DISTANCE_COUNT];
	static const double			distanceMultiplier	= 1.0;	//ezzel kell felszorozni a kapott distance_num ertekeket, hogy distance_mm legyen

	bool button[BUTTON_COUNT];
	//----- function VEGE -----


};
//------------------------------ node_Input VEGE ------------------------------





#endif /*NODE_INPUT_H_*/
