#ifndef NODE_CARACOLE_H_
#define NODE_CARACOLE_H_





//------------------------------ include ELEJE ------------------------------
#include "node.h"
//------------------------------ include VEGE ------------------------------

#ifdef NAGY_ROBOT





//------------------------------ node_Caracole ELEJE ------------------------------
class node_Caracole : public node{

public:

	node_Caracole(void);
	~node_Caracole();

	void evalMsg(UDPmsg* msg);

	void INIT_PARAM(void);


	void CARACOLE_SET_SPEED(double speed, double acc);


	progress_t SetSpeed;


	double GET_SPEED(void);


private:

	float CaracoleSpeed;

};
//------------------------------ node_Caracole VEGE ------------------------------





#endif /*NAGY_ROBOT*/

#endif /*NODE_CARACOLE_H_*/
