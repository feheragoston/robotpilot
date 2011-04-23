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
//------------------------------ include VEGE ------------------------------





//------------------------------ id ELEJE ------------------------------
#define PC_ID						0
#define GATEWAY_ID					1
#define BROADCAST_ID				2
#define BOOTLOADER_ID				30


#define CONSOLE_ID					3
#define DEADRECK_ID					4
#define DRIVE_ID					5
#define INPUT_ID					6
#define MAGNET_ID					7
#define SERVO_ID					8
#define SONAR_ID					9
#define SUPPLY_ID					10
//------------------------------ id VEGE ------------------------------





//------------------------------ node ELEJE ------------------------------
class node{

public:


	//----- function TX ELEJE -----
	void CORE_RESET(void);
	void KEEP_ALIVE(void);
	void SET_KEEP_ALIVE_MS(unsigned short keep_alive_ms);
	void SET_SEND_PERIOD_MS(unsigned short period_ms);
	
	void PINGprocess(void);
	//----- function TX VEGE -----
	
	
protected:

	//----- mindegyik node-nak van ELEJE -----
	unsigned int	id;
	char			name[32];
	//----- mindegyik node-nak van VEGE -----



	//----- function ELEJE -----
	static const unsigned int	CMD_CORE_RESET					= 0x1B;	//length = 0
	
	static const unsigned int	CMD_KEEP_ALIVE					= 0x1C;	//length = 0
	
	static const unsigned int	CMD_SET_KEEP_ALIVE_MS			= 0x1D;	//length = 2, data[0..1] = keep_alive_ms, mert u16 ertek
	
	static const unsigned int	CMD_SET_SEND_PERIOD_MS			= 0x1E;	//length = 2, data[0..1] = period_ms, mert u16 ertek
	
	static const unsigned int	CMD_PING						= 0x1F;	//length = 0
	static const unsigned int	MSG_PING_REPLY					= 0x1F;	//length = 0


	void PING(void);
	sem_t						pingSemaphore;
	static const unsigned int	PING_REPLY_MAX_WAIT_TIME_SEC	= 1;
	//----- function VEGE -----


};
//------------------------------ node VEGE ------------------------------





#endif /*NODE_H_*/
