#ifndef ID_H_
#define ID_H_



/**************************************************/
/** ID-k                                         **/
/**************************************************/

//kulon
#define PC_ID						0
#define GATEWAY_ID					1
#define BROADCAST_ID				2
// #define BOOTLOADER_ID				30	//nincs már CANBOOT


//node
#define CONSOLE_ID					3
#define DEADRECK_ID					4
#define DCWHEEL_ID					5
#define INPUT_ID					6
#define SERVO_ID					8
#define SONAR_ID					9
#define POWER_ID					10

#define MAX_NODE_ID					10
#define MAX_NODE_COUNT				(MAX_NODE_ID+1)



#endif	//ID_H_
