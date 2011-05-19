/*
 * node_Input.cpp
 *
 *  Created on: Feb 17, 2011
 *      Author: triszalo
 */





#include "include/node_Input.h"
#include "../define/sharp_table.h"


using namespace std;


node_Input::node_Input(void) : node(INPUT_ID, "node_Input", INPUT_KEEP_ALIVE_MS, INPUT_SEND_PERIOD_TO_PC_MS, INPUT_SEND_PERIOD_TO_NODE_MS){


	//----- valtozo init ELEJE -----
	for(u8 i=0 ; i<INPUT_ANALOG_COUNT ; i++)
		analog[i] = 0;

	for(u8 i=0 ; i<INPUT_DIGITAL_COUNT ; i++)
		digital[i] = false;

	digital_active_level[INPUT_DIGITAL_START_BUTTON_INDEX]				= ((INPUT_DIGITAL_START_BUTTON_ACTIVE_LEVEL != 0)				? true : false);
	digital_active_level[INPUT_DIGITAL_COLOR_BUTTON_INDEX]				= ((INPUT_DIGITAL_COLOR_BUTTON_ACTIVE_LEVEL != 0)				? true : false);
	digital_active_level[INPUT_DIGITAL_REAR_LEFT_LIMIT_SWITCH_INDEX]	= ((INPUT_DIGITAL_REAR_LEFT_LIMIT_SWITCH_ACTIVE_LEVEL != 0)		? true : false);
	digital_active_level[INPUT_DIGITAL_REAR_RIGHT_LIMIT_SWITCH_INDEX]	= ((INPUT_DIGITAL_REAR_RIGHT_LIMIT_SWITCH_ACTIVE_LEVEL != 0)	? true : false);
	digital_active_level[INPUT_DIGITAL_PLUS_0_INDEX]					= ((INPUT_DIGITAL_PLUS_0_ACTIVE_LEVEL != 0)						? true : false);
	digital_active_level[INPUT_DIGITAL_PLUS_1_INDEX]					= ((INPUT_DIGITAL_PLUS_1_ACTIVE_LEVEL != 0)						? true : false);
	digital_active_level[INPUT_DIGITAL_PLUS_2_INDEX]					= ((INPUT_DIGITAL_PLUS_2_ACTIVE_LEVEL != 0)						? true : false);
	digital_active_level[INPUT_DIGITAL_PLUS_3_INDEX]					= ((INPUT_DIGITAL_PLUS_3_ACTIVE_LEVEL != 0)						? true : false);
	digital_active_level[INPUT_DIGITAL_PLUS_4_INDEX]					= ((INPUT_DIGITAL_PLUS_4_ACTIVE_LEVEL != 0)						? true : false);
	digital_active_level[INPUT_DIGITAL_PLUS_5_INDEX]					= ((INPUT_DIGITAL_PLUS_5_ACTIVE_LEVEL != 0)						? true : false);

	digital_pullup[0]	= ((INPUT_PULLUP_DIGITAL_01 != 0)	? true : false);
	digital_pullup[1]	= ((INPUT_PULLUP_DIGITAL_23 != 0)	? true : false);
	digital_pullup[2]	= ((INPUT_PULLUP_DIGITAL_45 != 0)	? true : false);
	digital_pullup[3]	= ((INPUT_PULLUP_DIGITAL_6 != 0)	? true : false);
	digital_pullup[4]	= ((INPUT_PULLUP_DIGITAL_7 != 0)	? true : false);
	digital_pullup[5]	= ((INPUT_PULLUP_DIGITAL_8 != 0)	? true : false);
	digital_pullup[6]	= ((INPUT_PULLUP_DIGITAL_9 != 0)	? true : false);
	//----- valtozo init VEGE -----


}


node_Input::~node_Input(){

}


void node_Input::evalMsg(UDPmsg* msg){

	//ha o a cimzett, csak akkor dolgozza fel
	if(msg->node_id == id){

		switch(msg->function){

			case MSG_PING_REPLY:
				sem_post(&pingSemaphore);
				break;

			case MSG_INIT_PARAM_REPLY:
				initparamOK	= GET_BOOL(&(msg->data[0]), 0);
				sem_post(&initparamSemaphore);
				break;

			case MSG_PERIODIC_TO_PC:

				for(u8 i=0 ; i<INPUT_ANALOG_COUNT ; i++)
					analog[i] = GET_U16(&(msg->data[2*i]));

				for(u8 i=0 ; i<INPUT_DIGITAL_COUNT ; i++)
					digital[i] = GET_BOOL(&(msg->data[14+0]), i);

				break;

			default:
				break;


		}

	}

}


void node_Input::INIT_PARAM(void){

	UDPmsg msg;

	msg.node_id		= id;
	msg.function	= CMD_INIT_PARAM;
	msg.length		= 1;
	for(u8 i=0 ; i<INPUT_DIGITAL_PIN_COUNT ; i++)
		SET_BOOL(&(msg.data[0]), i, digital_pullup[i]);

	UDPdriver::send(&msg);

}


double node_Input::GET_SHARP_MM(u16 analog_value, double table[][2], u8 size){

	double voltage = INPUT_ANALOG_TO_V(analog_value);
	u8 i;
	double der;	//V-mm karakterisztika meredeksege



	//megkeressuk a tablaban a helyet, amelyik elott van
	for(i=0 ; i < size ; i++)
		if(table[i][1] < voltage)
			break;


	//ITT: ha i!=size, akkor az i-edik mar kisebb


	//ha csak kisebb van a tablazatban
	if(i==size)
		return table[size-1][1];	//max

	//ha csak nagyobb van a tablazatban
	else if(i==0)
		return table[0][1];	//min

	//ha van kisebb es nagyobb is a tablazatban
	//(i-1)-edik V meg nagyobb, i-edik V mar kisebb
	//(i-1)-edik mm meg kisebb, i-edik mm mar nagyobb
	else{

		//V-mm karakterisztika meredeksege
		//der = (y1 - y2) / (x1 - x2)
		der = (table[i-1][0] - table[i][0]) / (table[i-1][1] - table[i][1]);

		//der = (y - y0) / (x - x0)  ->  y = y0 + der * (x - x0)
		return table[i-1][0] + der * (voltage - table[i-1][1]);

	}

}


double node_Input::GET_DISTANCE(u8 num){

	//short
	return (GET_SHARP_MM(analog[num], SHORT_SHARP_MM_V, SHORT_SHARP_TABLE_SIZE));

}


bool node_Input::GET_DIGITAL(u8 num){

	return ((digital[num] == digital_active_level[num]) ? true : false);

}
