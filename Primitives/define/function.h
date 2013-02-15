#ifndef FUNCTION_H_
#define FUNCTION_H_



/**************************************************/
/** function define-ok                           **/
/**************************************************/

/******************************/
/** minden node              **/
/******************************/

//nem lehet broadcast
#define MSG_PERIODIC_TO_NODE				0x15	//driver.c "SEND_PERIOD függvények" reszben
#define MSG_PERIODIC_TO_PC					0x16	//driver.c "SEND_PERIOD függvények" reszben

#define CMD_INIT_PARAM						0x17	//driver.c "EVAL függvények" reszben
#define MSG_INIT_PARAM_REPLY				0x17	//driver.c "INIT_PARAM_REPLY függvények" reszben

#define CMD_PING							0x1F	//driver.c "eval_common" fuggvenyben
#define MSG_PING_REPLY						0x1F	//driver.c "PING" fuggvenyben


//lehet broadcast
// #define CMD_NODE_START_CANBOOTLOADER		0x10	//driver.c "eval_common" fuggvenyben (Gateway-nek nincs)

#define CMD_CORE_RESET						0x18	//driver.c "eval_common" fuggvenyben

#define CMD_START_ACTUATOR					0x19	//driver.c "eval_common" fuggvenyben
#define CMD_STOP_ACTUATOR					0x1A	//driver.c "eval_common" fuggvenyben

#define CMD_KEEP_ALIVE						0x1B	//driver.c "eval_common" fuggvenyben
#define CMD_SET_KEEP_ALIVE_MS				0x1C	//driver.c "eval_common" fuggvenyben

#define CMD_SET_SEND_PERIOD_TO_NODE_MS		0x1D	//driver.c "eval_common" fuggvenyben
#define CMD_SET_SEND_PERIOD_TO_PC_MS		0x1E	//driver.c "eval_common" fuggvenyben


/******************************/
/** node specifikus          **/
/******************************/

//GATEWAY_ID
#define CMD_GATEWAY_REMOVE_NODE				0x00
#define MSG_GATEWAY_REMOVE_NODE_REPLY		0x00

#define CMD_GATEWAY_ADD_NODE_CANA			0x01
#define MSG_GATEWAY_ADD_NODE_CANA_REPLY		0x01

#define CMD_GATEWAY_ADD_NODE_CANB			0x02
#define MSG_GATEWAY_ADD_NODE_CANB_REPLY		0x02

// #define CMD_GATEWAY_ADD_BOOTLOADER_ID		0x03
// #define MSG_GATEWAY_ADD_BOOTLOADER_ID_REPLY	0x03


//CONSOLE_ID
#define CMD_CONSOLE_STOP					0x00
#define MSG_CONSOLE_STOP_REPLY				0x00

#define CMD_CONSOLE_SET_POS					0x01
#define MSG_CONSOLE_SET_POS_REPLY			0x01

#define CMD_CONSOLE_CALIBRATE				0x02
#define MSG_CONSOLE_CALIBRATE_REPLY			0x02


//CARACOLE_ID
#define CMD_CARACOLE_SET_SPEED				0x00
#define MSG_CARACOLE_SET_SPEED_REPLY		0x00


//FIREWHEEL_ID
#define CMD_FIREWHEEL_SET_SPEED				0x00
#define MSG_FIREWHEEL_SET_SPEED_REPLY		0x00


//DEADRECK_ID
#define CMD_DEADRECK_RESET_POS				0x00
#define MSG_DEADRECK_RESET_POS_REPLY		0x00


//DCWHEEL_ID
#define CMD_DCWHEEL_STOP					0x00
#define MSG_DCWHEEL_STOP_REPLY				0x00

#define CMD_DCWHEEL_HARD_STOP				0x01
#define MSG_DCWHEEL_HARD_STOP_REPLY			0x01

#define CMD_DCWHEEL_GO						0x02
#define MSG_DCWHEEL_GO_REPLY				0x02

#define CMD_DCWHEEL_GOTO					0x03
#define MSG_DCWHEEL_GOTO_REPLY				0x03

#define CMD_DCWHEEL_TURN					0x04
#define MSG_DCWHEEL_TURN_REPLY				0x04

#define CMD_DCWHEEL_SET_SPEED				0x05
#define MSG_DCWHEEL_SET_SPEED_REPLY			0x05


//INPUT_ID
#define CMD_INPUT_SET_OUTPUT				0x00
#define MSG_INPUT_SET_OUTPUT_REPLY			0x00


//SERVO_ID
#define CMD_SERVO_SET_POS					0x00
#define MSG_SERVO_SET_POS_REPLY				0x00


//SONAR_ID


//POWER_ID
#define CMD_POWER_ACT_ON					0x00
#define MSG_POWER_ACT_ON_REPLY				0x00

#define CMD_POWER_ACT_OFF					0x01
#define MSG_POWER_ACT_OFF_REPLY				0x01

#define CMD_POWER_MAIN_OFF					0x02
#define MSG_POWER_MAIN_OFF_REPLY			0x02



#endif	//FUNCTION_H_
