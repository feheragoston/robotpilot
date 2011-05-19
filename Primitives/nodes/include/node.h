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

	node(u16 id, const char* name, u32 keepAlive_ms, u32 sendPeriodToPC_ms, u32 sendPeriodToNode_ms);
	~node();

	void CORE_RESET(void);
	void START_ACTUATOR(void);
	void STOP_ACTUATOR(void);
	void KEEP_ALIVE(void);
	void SET_KEEP_ALIVE_MS(void);
	void SET_SEND_PERIOD_TO_NODE_MS(void);
	void SET_SEND_PERIOD_TO_PC_MS(void);

	virtual void evalMsg(UDPmsg* msg);

	bool PINGprocess(void);
	bool INITPARAMprocess(void);
	
	
protected:

	u16		id;
	char	name[32];
	
	u32		keepAlive_ms;
	u32		sendPeriodToPC_ms;
	u32		sendPeriodToNode_ms;


	void PING(void);
	sem_t						pingSemaphore;

	virtual void INIT_PARAM(void);
	sem_t						initparamSemaphore;
	bool						initparamOK;


};
//------------------------------ node VEGE ------------------------------





#endif /*NODE_H_*/
