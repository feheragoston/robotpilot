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

	analog_has_long_sharp[INPUT_ANALOG_LEFT_FRONT_SHARP_INDEX]		= true;
	analog_has_long_sharp[INPUT_ANALOG_RIGHT_FRONT_SHARP_INDEX]		= true;
	analog_has_long_sharp[INPUT_ANALOG_LEFT_REAR_SHARP_INDEX]		= true;
	analog_has_long_sharp[INPUT_ANALOG_RIGHT_REAR_SHARP_INDEX]		= true;
	analog_has_long_sharp[INPUT_ANALOG_BALL_COLOR_CNY70_INDEX]		= false;
	analog_has_long_sharp[INPUT_ANALOG_PLUS_0_INDEX]				= false;
	analog_has_long_sharp[INPUT_ANALOG_PLUS_1_INDEX]				= false;

	digital_active_level[INPUT_DIGITAL_START_BUTTON_INDEX]				= ((INPUT_DIGITAL_START_BUTTON_ACTIVE_LEVEL != 0)				? true : false);
	digital_active_level[INPUT_DIGITAL_COLOR_RED_BUTTON_INDEX]			= ((INPUT_DIGITAL_COLOR_RED_BUTTON_ACTIVE_LEVEL != 0)			? true : false);
	digital_active_level[INPUT_DIGITAL_COLOR_PURPLE_BUTTON_INDEX]		= ((INPUT_DIGITAL_COLOR_PURPLE_BUTTON_ACTIVE_LEVEL != 0)		? true : false);
	digital_active_level[INPUT_DIGITAL_FRONT_LEFT_LIMIT_SWITCH_INDEX]	= ((INPUT_DIGITAL_FRONT_LEFT_LIMIT_SWITCH_ACTIVE_LEVEL != 0)	? true : false);
	digital_active_level[INPUT_DIGITAL_FRONT_RIGHT_LIMIT_SWITCH_INDEX]	= ((INPUT_DIGITAL_FRONT_RIGHT_LIMIT_SWITCH_ACTIVE_LEVEL != 0)	? true : false);
	digital_active_level[INPUT_DIGITAL_REAR_LEFT_LIMIT_SWITCH_INDEX]	= ((INPUT_DIGITAL_REAR_LEFT_LIMIT_SWITCH_ACTIVE_LEVEL != 0)		? true : false);
	digital_active_level[INPUT_DIGITAL_REAR_RIGHT_LIMIT_SWITCH_INDEX]	= ((INPUT_DIGITAL_REAR_RIGHT_LIMIT_SWITCH_ACTIVE_LEVEL != 0)	? true : false);
	digital_active_level[INPUT_DIGITAL_BALL_PRESENT_INDEX]				= ((INPUT_DIGITAL_BALL_PRESENT_ACTIVE_LEVEL != 0)				? true : false);
	digital_active_level[INPUT_DIGITAL_PLUS_0_INDEX]					= ((INPUT_DIGITAL_PLUS_0_ACTIVE_LEVEL != 0)						? true : false);
	digital_active_level[INPUT_DIGITAL_PLUS_1_INDEX]					= ((INPUT_DIGITAL_PLUS_1_ACTIVE_LEVEL != 0)						? true : false);

	digital_is_output[INPUT_DIGITAL_START_BUTTON_INDEX]					= ((INPUT_DIGITAL_IS_OUTPUT_START_BUTTON != 0)				? true : false);
	digital_is_output[INPUT_DIGITAL_COLOR_RED_BUTTON_INDEX]				= ((INPUT_DIGITAL_IS_OUTPUT_COLOR_RED_BUTTON != 0)			? true : false);
	digital_is_output[INPUT_DIGITAL_COLOR_PURPLE_BUTTON_INDEX]			= ((INPUT_DIGITAL_IS_OUTPUT_COLOR_PURPLE_BUTTON != 0)		? true : false);
	digital_is_output[INPUT_DIGITAL_FRONT_LEFT_LIMIT_SWITCH_INDEX]		= ((INPUT_DIGITAL_IS_OUTPUT_FRONT_LEFT_LIMIT_SWITCH != 0)	? true : false);
	digital_is_output[INPUT_DIGITAL_FRONT_RIGHT_LIMIT_SWITCH_INDEX]		= ((INPUT_DIGITAL_IS_OUTPUT_FRONT_RIGHT_LIMIT_SWITCH != 0)	? true : false);
	digital_is_output[INPUT_DIGITAL_REAR_LEFT_LIMIT_SWITCH_INDEX]		= ((INPUT_DIGITAL_IS_OUTPUT_REAR_LEFT_LIMIT_SWITCH != 0)	? true : false);
	digital_is_output[INPUT_DIGITAL_REAR_RIGHT_LIMIT_SWITCH_INDEX]		= ((INPUT_DIGITAL_IS_OUTPUT_REAR_RIGHT_LIMIT_SWITCH != 0)	? true : false);
	digital_is_output[INPUT_DIGITAL_PLUS_0_INDEX]						= ((INPUT_DIGITAL_IS_OUTPUT_PLUS_0 != 0)					? true : false);
	digital_is_output[INPUT_DIGITAL_PLUS_1_INDEX]						= ((INPUT_DIGITAL_IS_OUTPUT_PLUS_1 != 0)					? true : false);
	digital_is_output[INPUT_DIGITAL_BALL_PRESENT_INDEX]					= ((INPUT_DIGITAL_IS_OUTPUT_BALL_PRESENT != 0)				? true : false);

	digital_output_init_state[INPUT_DIGITAL_START_BUTTON_INDEX]				= INPUT_DIGITAL_OUTPUT_START_BUTTON_INIT_STATE;
	digital_output_init_state[INPUT_DIGITAL_COLOR_RED_BUTTON_INDEX]			= INPUT_DIGITAL_OUTPUT_COLOR_RED_BUTTON_INIT_STATE;
	digital_output_init_state[INPUT_DIGITAL_COLOR_PURPLE_BUTTON_INDEX]		= INPUT_DIGITAL_OUTPUT_COLOR_PURPLE_BUTTON_INIT_STATE;
	digital_output_init_state[INPUT_DIGITAL_FRONT_LEFT_LIMIT_SWITCH_INDEX]	= INPUT_DIGITAL_OUTPUT_FRONT_LEFT_LIMIT_SWITCH_INIT_STATE;
	digital_output_init_state[INPUT_DIGITAL_FRONT_RIGHT_LIMIT_SWITCH_INDEX]	= INPUT_DIGITAL_OUTPUT_FRONT_RIGHT_LIMIT_SWITCH_INIT_STATE;
	digital_output_init_state[INPUT_DIGITAL_REAR_LEFT_LIMIT_SWITCH_INDEX]	= INPUT_DIGITAL_OUTPUT_REAR_LEFT_LIMIT_SWITCH_INIT_STATE;
	digital_output_init_state[INPUT_DIGITAL_REAR_RIGHT_LIMIT_SWITCH_INDEX]	= INPUT_DIGITAL_OUTPUT_REAR_RIGHT_LIMIT_SWITCH_INIT_STATE;
	digital_output_init_state[INPUT_DIGITAL_PLUS_0_INDEX]					= INPUT_DIGITAL_OUTPUT_PLUS_0_INIT_STATE;
	digital_output_init_state[INPUT_DIGITAL_PLUS_1_INDEX]					= INPUT_DIGITAL_OUTPUT_PLUS_1_INIT_STATE;
	digital_output_init_state[INPUT_DIGITAL_BALL_PRESENT_INDEX]				= INPUT_DIGITAL_OUTPUT_BALL_PRESENT_INIT_STATE;
	//----- valtozo init VEGE -----


}


node_Input::~node_Input(){

}


void node_Input::evalMsg(UDPmsg* msg){

	u8 num;


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

			case MSG_INPUT_SET_OUTPUT_REPLY:
				num = GET_U8(&(msg->data[0]));
				SetOutput[num].done = GET_BOOL(&(msg->data[1]), 0);
				SetOutput[num].inProgress = false;
				SetOutput[num].finished = true;

				cout << name << "\t___recv SETOUTPUT [" << (u32)num << "]___:\t" << (SetOutput[num].done?"1":"0") << endl;

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

	bool st;

	msg.node_id		= id;
	msg.function	= CMD_INIT_PARAM;
	msg.length		= 1;

	for(int i=0 ; i<INPUT_DIGITAL_OUTPUT_COUNT ; i++){

		SET_BOOL(&(msg.data[0]), 0+2*i, digital_is_output[INPUT_DIGITAL_OUTPUT_MIN_INDEX + i]);

		st = digital_output_init_state[INPUT_DIGITAL_OUTPUT_MIN_INDEX + i];
		//ha aktív alacsony, akkor negálunk
		if(digital_active_level[INPUT_DIGITAL_OUTPUT_MIN_INDEX + i] == 0)	st = !st;

		SET_BOOL(&(msg.data[0]), 1+2*i, st);
	}

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
		return table[size-1][0];	//max

	//ha csak nagyobb van a tablazatban
	else if(i==0)
		return table[0][0];	//min

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


double node_Input::GET_VOLTAGE(u8 num){

	return INPUT_ANALOG_TO_V(analog[num]);

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


void node_Input::SET_DIGITAL(u8 num, bool on){

	UDPmsg msg;

	//ha aktív alacsony, akkor negálunk
	if(digital_active_level[num] == 0)	on = !on;

	msg.node_id		= id;
	msg.function	= CMD_INPUT_SET_OUTPUT;
	msg.length		= 2;

	SET_U8(&(msg.data[0]), num);
	SET_BOOL(&(msg.data[1]), 0, on);

	UDPdriver::send(&msg);

	SetOutput[num].inProgress = true;
	SetOutput[num].finished = false;

	cout << name << "\t___send SETOUTPUT [" << (u32)num << "]___:\t" << (on ? "1" : "0") << endl;

}
