#ifndef FUNCTION_H_
#define FUNCTION_H_





//------------------------------ function ------------------------------
//0x00-0x05:	node-nak es gateway-nek is:	node-specifikus parancs, es reply
//0x10-0x10:	csak node-nak:				mindegyiknek ugyanaz
//0x11-0x13:	node-nak es gateway-nek is:	node-specifikus
//0x1A-0x1F:	node-nak es gateway-nek is


//---------- common node ----------
#define CMD_NODE_START_CANBOOTLOADER		0x10	//driver.c "eval_common" fuggvenyben


//---------- all ----------
#define MSG_PERIODIC_TO_NODE				0x15	//driver.c "SEND_PERIOD" reszben
#define MSG_PERIODIC_TO_PC					0x16	//driver.c "SEND_PERIOD" reszben

#define CMD_INIT_PARAM						0x17	//spec.h "valtozo", es driver.c "EVAL" reszben
#define MSG_INIT_PARAM_REPLY				0x17	//driver.c "REPLY" reszben


//---------- common ----------
#define CMD_CORE_RESET						0x18	//driver.c "eval_common" fuggvenyben

#define CMD_START_ACTUATOR					0x19	//driver.c "eval_common" fuggvenyben
#define CMD_STOP_ACTUATOR					0x1A	//driver.c "eval_common" fuggvenyben

#define CMD_KEEP_ALIVE						0x1B	//driver.c "eval_common" fuggvenyben
#define CMD_SET_KEEP_ALIVE_MS				0x1C	//driver.c "eval_common" fuggvenyben

#define CMD_SET_SEND_PERIOD_TO_NODE_MS		0x1D	//driver.c "eval_common" fuggvenyben
#define CMD_SET_SEND_PERIOD_TO_PC_MS		0x1E	//driver.c "eval_common" fuggvenyben

#define CMD_PING							0x1F	//driver.c "eval_common" fuggvenyben
#define MSG_PING_REPLY						0x1F	//driver.c "eval_common" fuggvenyben


//---------- GATEWAY_ID ----------
#define CMD_GATEWAY_REMOVE_NODE				0x00	//driver.c "EVAL" reszben
#define CMD_GATEWAY_ADD_NODE_CANA			0x01	//driver.c "EVAL" reszben
#define CMD_GATEWAY_ADD_NODE_CANB			0x02	//driver.c "EVAL" reszben
#define CMD_GATEWAY_ADD_BOOTLOADER_ID		0x03	//driver.c "EVAL" reszben


//---------- CONSOLE_ID ----------
#define CMD_CONSOLE_STOP					0x00	//spec.h "spec fuggvenyei" reszben
#define MSG_CONSOLE_STOP_REPLY				0x00	//driver.c "REPLY" reszben

#define CMD_CONSOLE_SET_POS					0x01	//spec.h "spec fuggvenyei" reszben
#define MSG_CONSOLE_SET_POS_REPLY			0x01	//driver.c "REPLY" reszben

#define CMD_CONSOLE_CALIBRATE				0x02	//spec.h "spec fuggvenyei" reszben
#define MSG_CONSOLE_CALIBRATE_REPLY			0x02	//driver.c "REPLY" reszben


//---------- DEADRECK_ID ----------
#define CMD_DEADRECK_RESET_POS				0x00	//spec.h "spec fuggvenyei" reszben
#define MSG_DEADRECK_RESET_POS_REPLY		0x00	//driver.c "REPLY" reszben


//---------- BDC_ID ----------
#define CMD_BDC_STOP						0x00	//spec.h "spec fuggvenyei" reszben
#define MSG_BDC_STOP_REPLY					0x00	//driver.c "REPLY" reszben

#define CMD_BDC_HARD_STOP					0x01	//spec.h "spec fuggvenyei" reszben
#define MSG_BDC_HARD_STOP_REPLY				0x01	//driver.c "REPLY" reszben

#define CMD_BDC_GO							0x02	//spec.h "spec fuggvenyei" reszben
#define MSG_BDC_GO_REPLY					0x02	//driver.c "REPLY" reszben

#define CMD_BDC_GOTO						0x03	//spec.h "spec fuggvenyei" reszben
#define MSG_BDC_GOTO_REPLY					0x03	//driver.c "REPLY" reszben

#define CMD_BDC_TURN						0x04	//spec.h "spec fuggvenyei" reszben
#define MSG_BDC_TURN_REPLY					0x04	//driver.c "REPLY" reszben

#define CMD_BDC_SET_SPEED					0x05	//spec.h "spec fuggvenyei" reszben
#define MSG_BDC_SET_SPEED_REPLY				0x05	//driver.c "REPLY" reszben


//---------- INPUT_ID ----------


//---------- VACUUM_ID ----------
#define CMD_VACUUM_SET						0x00	//spec.h "spec fuggvenyei" reszben
#define MSG_VACUUM_SET_REPLY				0x00	//driver.c "REPLY" reszben


//---------- SERVO_ID ----------
#define CMD_SERVO_SET_POS					0x00	//spec.h "spec fuggvenyei" reszben
#define MSG_SERVO_SET_POS_REPLY				0x00	//driver.c "REPLY" reszben


//---------- SONAR_ID ----------


//---------- POWER_ID ----------
#define CMD_POWER_ACT_ON					0x00	//spec.h "spec fuggvenyei" reszben
#define MSG_POWER_ACT_ON_REPLY				0x00	//driver.c "REPLY" reszben

#define CMD_POWER_ACT_OFF					0x01	//spec.h "spec fuggvenyei" reszben
#define MSG_POWER_ACT_OFF_REPLY				0x01	//driver.c "REPLY" reszben

#define CMD_POWER_MAIN_OFF					0x02	//spec.h "spec fuggvenyei" reszben
#define MSG_POWER_MAIN_OFF_REPLY			0x02	//driver.c "REPLY" reszben
//------------------------------ function VEGE ------------------------------





#endif	//FUNCTION_H_
