/*
 * node_Input.cpp
 *
 *  Created on: Feb 17, 2011
 *      Author: triszalo
 */





#include "include/node_Input.h"
#include "include/sharp_table.h"


using namespace std;


node_Input::node_Input(void){

	id = INPUT_ID;
	strcpy(name, "node_Input");


	//----- valtozo init ELEJE -----
	for(u8 i=0 ; i<INPUT_ANALOG_COUNT ; i++)
		analog[i] = 0;

	for(u8 i=0 ; i<INPUT_DIGITAL_COUNT ; i++)
		digital[i] = false;

	analog_has_long_sharp[0] = ((INPUT_LONG_SHARP_ON_ANALOG_0 != 0) ? true : false);
	analog_has_long_sharp[1] = ((INPUT_LONG_SHARP_ON_ANALOG_1 != 0) ? true : false);
	analog_has_long_sharp[2] = ((INPUT_LONG_SHARP_ON_ANALOG_2 != 0) ? true : false);
	analog_has_long_sharp[3] = ((INPUT_LONG_SHARP_ON_ANALOG_3 != 0) ? true : false);
	analog_has_long_sharp[4] = ((INPUT_LONG_SHARP_ON_ANALOG_4 != 0) ? true : false);
	analog_has_long_sharp[5] = ((INPUT_LONG_SHARP_ON_ANALOG_5 != 0) ? true : false);
	analog_has_long_sharp[6] = ((INPUT_LONG_SHARP_ON_ANALOG_6 != 0) ? true : false);

	digital_active_level[0] = ((INPUT_ACTIVE_LEVEL_DIGITAL_0 != 0) ? true : false);
	digital_active_level[1] = ((INPUT_ACTIVE_LEVEL_DIGITAL_1 != 0) ? true : false);
	digital_active_level[2] = ((INPUT_ACTIVE_LEVEL_DIGITAL_2 != 0) ? true : false);
	digital_active_level[3] = ((INPUT_ACTIVE_LEVEL_DIGITAL_3 != 0) ? true : false);
	digital_active_level[4] = ((INPUT_ACTIVE_LEVEL_DIGITAL_4 != 0) ? true : false);
	digital_active_level[5] = ((INPUT_ACTIVE_LEVEL_DIGITAL_5 != 0) ? true : false);
	digital_active_level[6] = ((INPUT_ACTIVE_LEVEL_DIGITAL_6 != 0) ? true : false);
	digital_active_level[7] = ((INPUT_ACTIVE_LEVEL_DIGITAL_7 != 0) ? true : false);
	digital_active_level[8] = ((INPUT_ACTIVE_LEVEL_DIGITAL_8 != 0) ? true : false);
	digital_active_level[9] = ((INPUT_ACTIVE_LEVEL_DIGITAL_9 != 0) ? true : false);

	digital_pullup[0] = ((INPUT_PULLUP_DIGITAL_01 != 0) ? true : false);
	digital_pullup[1] = ((INPUT_PULLUP_DIGITAL_23 != 0) ? true : false);
	digital_pullup[2] = ((INPUT_PULLUP_DIGITAL_45 != 0) ? true : false);
	digital_pullup[3] = ((INPUT_PULLUP_DIGITAL_6 != 0) ? true : false);
	digital_pullup[4] = ((INPUT_PULLUP_DIGITAL_7 != 0) ? true : false);
	digital_pullup[5] = ((INPUT_PULLUP_DIGITAL_8 != 0) ? true : false);
	digital_pullup[6] = ((INPUT_PULLUP_DIGITAL_9 != 0) ? true : false);
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

	//grad = (y-y0) / (x-x0)
	//y = (x-x0) * grad + y0
	double voltage = ((double)analog_value - INPUT_ANALOG_V_X0) * INPUT_ANALOG_V_GRAD + INPUT_ANALOG_V_Y0;
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

	//long
	if(analog_has_long_sharp[num])
		return (GET_SHARP_MM(analog[num], LONG_SHARP_MM_V, LONG_SHARP_TABLE_SIZE));

	//short
	else
		return (GET_SHARP_MM(analog[num], SHORT_SHARP_MM_V, SHORT_SHARP_TABLE_SIZE));

}


bool node_Input::GET_DIGITAL(u8 num){

	return ((digital[num] == digital_active_level[num]) ? true : false);

}
