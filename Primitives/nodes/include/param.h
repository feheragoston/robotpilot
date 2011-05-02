#ifndef PARAM_H_
#define PARAM_H_





#define PING_REPLY_MAX_WAIT_TIME_SEC		1


#define SERVO_COUNT							6


#define MAGNET_COUNT						2

#define MAGNET_POLARITY_PULL				0x01	//01: pull	(vonz)
#define MAGNET_POLARITY_OFF					0x00	//00: off	(kikapcsolt)
#define MAGNET_POLARITY_PUSH				0xFF	//FF: push	(taszit)


#define INPUT_DISTANCE_COUNT				6	//6 tavolsagerzekelo
#define INPUT_BUTTON_COUNT					8	//Start, Color, RearLeft, RearRight, stb.

#define INPUT_DISTANCE_MULTIPLIER			(1.0)	//ezzel kell felszorozni a kapott distance_num ertekeket, hogy distance_mm legyen


#define ACT_INPROGRESS						1
#define ACT_FINISHED						0
#define ACT_ERROR							-1


#define SERVO_GRIPPER_LEFT_INDEX					0
#define SERVO_GRIPPER_RIGHT_INDEX					1
#define SERVO_LEFT_ARM_INDEX						2
#define SERVO_RIGHT_ARM_INDEX						3

#define MAGNET_LEFT_INDEX							0
#define MAGNET_RIGHT_INDEX							1

#define INPUT_BUTTON_START_INDEX					0
#define INPUT_BUTTON_COLOR_INDEX					1
#define INPUT_BUTTON_FRONT_LEFT_INDEX				2
#define INPUT_BUTTON_FRONT_RIGHT_INDEX				3
#define INPUT_BUTTON_REAR_LEFT_INDEX				4
#define INPUT_BUTTON_REAR_RIGHT_INDEX				5




#define SERVO_GRIPPER_MAX_SPEED				100
#define SERVO_GRIPPER_MAX_ACC				10


#define KEEP_ALIVE_PERIOD_MS				20



#define AREA_LENGTH_X							2100
#define AREA_LENGTH_Y							3000
#define ROBOT_DISTANCE_ON_FRONT_WALL			200
#define ROBOT_DISTANCE_ON_REAR_WALL				150

#define DEADRECK_CALIB_SPEED_ABS				0.1
#define DEADRECK_CALIB_OMEGA_ABS				0.2
#define DEADRECK_CALIB_BETA_ABS				0.08
#define DEADRECK_START_DISTANCE_X				250
#define DEADRECK_START_DISTANCE_Y				250


#define KEEP_ALIVE_MS							1000
#define SEND_PERIOD_TO_PC_MS					100
#define SEND_PERIOD_TO_NODE_MS					10



//gomboknál ON_STATE-ek, hogy melyikre van ON, melyikre OFF



#endif	//PARAM_H_
