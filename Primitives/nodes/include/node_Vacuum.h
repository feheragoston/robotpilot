/*
 * node_Vacuum.h
 *
 *  Created on: May 02, 2012
 *      Author: triszalo
 */


#ifndef NODE_VACUUM_H_
#define NODE_VACUUM_H_





//------------------------------ include ELEJE ------------------------------
#include "node.h"
//------------------------------ include VEGE ------------------------------





//------------------------------ node_Vacuum ELEJE ------------------------------
class node_Vacuum : public node{

public:

	node_Vacuum(void);
	~node_Vacuum();

	void evalMsg(UDPmsg* msg);

	void INIT_PARAM(void);

	void VACUUM_SET(u8 num, bool on);
	bool GET_ON(u8 num);


	progress_t Set[VACUUM_COUNT];


private:

	bool Vacuum_CmdOn[VACUUM_COUNT];
	bool Vacuum_On[VACUUM_COUNT];


};
//------------------------------ node_Vacuum VEGE ------------------------------





#endif /*NODE_VACUUM_H_*/
