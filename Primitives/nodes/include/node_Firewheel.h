#ifndef NODE_FIREWHEEL_H_
#define NODE_FIREWHEEL_H_





//------------------------------ include ELEJE ------------------------------
#include "node.h"
//------------------------------ include VEGE ------------------------------

#ifdef NAGY_ROBOT





//------------------------------ node_Firewheel ELEJE ------------------------------
class node_Firewheel : public node{

public:

	node_Firewheel(void);
	~node_Firewheel();

	void evalMsg(UDPmsg* msg);

	void INIT_PARAM(void);


	void FIREWHEEL_SET_SPEED(double speed, double acc);


	progress_t SetSpeed;


	double GET_SPEED(void);


private:

	float FirewheelSpeed;

};
//------------------------------ node_Firewheel VEGE ------------------------------





#endif /*NAGY_ROBOT*/

#endif /*NODE_FIREWHEEL_H_*/
