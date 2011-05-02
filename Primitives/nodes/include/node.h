/*
 * node.h
 *
 *  Created on: Feb 18, 2011
 *      Author: triszalo
 */


#ifndef NODE_H_
#define NODE_H_





//------------------------------ include ELEJE ------------------------------
#include <semaphore.h>
#include "../../UDP/UDPdriver.h"

#include "function.h"
#include "id.h"
#include "param.h"
//------------------------------ include VEGE ------------------------------





//------------------------------ node ELEJE ------------------------------
class node{

public:

	void CORE_RESET(void);
	void KEEP_ALIVE(void);
	void SET_KEEP_ALIVE_MS(unsigned short keep_alive_ms);
	void SET_SEND_PERIOD_TO_NODE_MS(unsigned short period_ms);
	void SET_SEND_PERIOD_TO_PC_MS(unsigned short period_ms);
	
	void PINGprocess(void);
	
	
protected:

	unsigned int	id;
	char			name[32];
	

	void PING(void);
	sem_t						pingSemaphore;


};
//------------------------------ node VEGE ------------------------------





#endif /*NODE_H_*/
