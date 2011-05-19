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

#include "../../define/type.h"
#include "../../define/function.h"
#include "../../define/id.h"
#include "../../define/param.h"
//------------------------------ include VEGE ------------------------------





//------------------------------ node ELEJE ------------------------------
class node{

public:

	node(u16 id, const char* name);
	~node();

	void CORE_RESET(void);
	void START_ACTUATOR(void);
	void STOP_ACTUATOR(void);
	void KEEP_ALIVE(void);
	void SET_KEEP_ALIVE_MS(u16 keep_alive_ms);
	void SET_SEND_PERIOD_TO_NODE_MS(u16 period_ms);
	void SET_SEND_PERIOD_TO_PC_MS(u16 period_ms);

	virtual void evalMsg(UDPmsg* msg);

	void PINGprocess(void);
	void INITPARAMprocess(void);
	
	
protected:

	u16		id;
	char	name[32];
	

	void PING(void);
	sem_t						pingSemaphore;

	virtual void INIT_PARAM(void);
	sem_t						initparamSemaphore;
	bool						initparamOK;


};
//------------------------------ node VEGE ------------------------------





#endif /*NODE_H_*/
