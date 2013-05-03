#ifndef PARAM_H_
#define PARAM_H_





#include <math.h>



#define NAGY_ROBOT
// #define KIS_ROBOT



//----- konstans -----
#define SERVO_COUNT							6

#ifdef NAGY_ROBOT
#define INPUT_ANALOG_COUNT					7
#define INPUT_DIGITAL_COUNT					10
#define INPUT_DIGITAL_OUTPUT_COUNT			4
#define INPUT_DIGITAL_OUTPUT_MIN_INDEX		6
#endif

#define SONAR_TRANSMITTER_COUNT				3

#define ACT_STARTED							true
#define ACT_START_ERROR						false

#define MOTION_NO_ERROR						0
#define MOTION_ERROR						1

#define COLOR_BLUE							-1
#define COLOR_UNKNOWN						0
#define COLOR_RED							1

#define AREA_LENGTH_X						2000
#define AREA_LENGTH_Y						3000



//----- macro -----
//kesobb:
//AAA_BBB_CCC_X0:		AAA node-nal a BBB-CCC fuggveny X0, Y0, X1, Y1 pontjai, hogy egy egyenest ket ponttal adjunk meg
//AAA_BBB_CCC_GRAD:		AAA node-nal a BBB-CCC fuggveny meredeksege (dCCC / dBBB)
#define GET_GRAD(X0, Y0, X1, Y1)			( ((double)(Y1) - (double)(Y0)) / ((double)(X1) - (double)(X0)) )





//----- atszamitas -----
#ifdef NAGY_ROBOT
#define MESV_TO_ANALOG(mesv)					((u16)((float)(mesv) / 3 * 0x0FFF))			//3V-hoz a 12 bites ADC legnagyobb erteke
#define ANALOG_TO_MESV(analog)					((float)(analog) / 0x0FFF * 3)				//12 bites ADC legnagyobb ertekehez 3V


#define POWER_V_TO_MESV(v)						((float)(v) * 10 / (10 + 100))				//feszultsegmeresekhez 10k-100k leosztas
#define POWER_A_TO_MESV(a)						((float)(a) * (-2.5/30) + 2.5)				//0A -> 2.5V, 30A -> 0V

#define POWER_MESV_TO_V(mesv)					((float)(mesv) * (10 + 100) / 10)			//feszultsegmeresekhez 10k-100k leosztas
#define POWER_MESV_TO_A(mesv)					((float)(mesv) * (-30/2.5) + 30)			//2.5V -> 0A, 0V -> 30A

#define POWER_V_TO_ANALOG(v)					MESV_TO_ANALOG(POWER_V_TO_MESV(v))
#define POWER_A_TO_ANALOG(a)					MESV_TO_ANALOG(POWER_A_TO_MESV(a))

#define POWER_ANALOG_TO_V(analog)				POWER_MESV_TO_V(ANALOG_TO_MESV(analog))
#define POWER_ANALOG_TO_A(analog)				POWER_MESV_TO_A(ANALOG_TO_MESV(analog))


#define INPUT_MESV_TO_V(mesv)					((float)(mesv) * (22 + 22) / 22)			//feszultsegmeresekhez 22k-22k leosztas
#define INPUT_V_TO_MESV(v)						((float)(v) * 22 / (22 + 22))				//feszultsegmeresekhez 22k-22k leosztas

#define INPUT_V_TO_ANALOG(v)					MESV_TO_ANALOG(INPUT_V_TO_MESV(v))
#define INPUT_ANALOG_TO_V(analog)				INPUT_MESV_TO_V(ANALOG_TO_MESV(analog))
#endif





//----- valtoztathato -----
#define START_ACTUATOR_IN_INIT					0

#define INIT_RETURN_FALSE_IF_ERROR				false

#define PING_REPLY_MAX_WAIT_TIME_SEC			1
#define INIT_PARAM_REPLY_MAX_WAIT_TIME_SEC		1





//----- can -----
#define KEEP_ALIVE_PERIOD_MS					100


#define BROADCAST_KEEP_ALIVE_MS					500
#define BROADCAST_SEND_PERIOD_TO_PC_MS			100
#define BROADCAST_SEND_PERIOD_TO_NODE_MS		10

#define USE_BROADCAST_KEEP_ALIVE_MS				true
#define USE_BROADCAST_SEND_PERIOD_TO_PC_MS		false
#define USE_BROADCAST_SEND_PERIOD_TO_NODE_MS	false


#define GATEWAY_KEEP_ALIVE_MS					1000
#define GATEWAY_SEND_PERIOD_TO_PC_MS			100
#define GATEWAY_SEND_PERIOD_TO_NODE_MS			0

#define CONSOLE_KEEP_ALIVE_MS					1000
#define CONSOLE_SEND_PERIOD_TO_PC_MS			100
#define CONSOLE_SEND_PERIOD_TO_NODE_MS			0

#ifdef NAGY_ROBOT
#define CARACOLE_KEEP_ALIVE_MS					1000
#define CARACOLE_SEND_PERIOD_TO_PC_MS			100
#define CARACOLE_SEND_PERIOD_TO_NODE_MS			0
#endif

#ifdef NAGY_ROBOT
#define FIREWHEEL_KEEP_ALIVE_MS					1000
#define FIREWHEEL_SEND_PERIOD_TO_PC_MS			100
#define FIREWHEEL_SEND_PERIOD_TO_NODE_MS		0
#endif

#define DEADRECK_KEEP_ALIVE_MS					1000
#define DEADRECK_SEND_PERIOD_TO_PC_MS			100
#define DEADRECK_SEND_PERIOD_TO_NODE_MS			1

#define DCWHEEL_KEEP_ALIVE_MS					1000
#define DCWHEEL_SEND_PERIOD_TO_PC_MS			100
#define DCWHEEL_SEND_PERIOD_TO_NODE_MS			0

#ifdef NAGY_ROBOT
#define INPUT_KEEP_ALIVE_MS						1000
#define INPUT_SEND_PERIOD_TO_PC_MS				100
#define INPUT_SEND_PERIOD_TO_NODE_MS			0
#endif

#define SERVO_KEEP_ALIVE_MS						1000
#define SERVO_SEND_PERIOD_TO_PC_MS				100
#define SERVO_SEND_PERIOD_TO_NODE_MS			0

#define SONAR_KEEP_ALIVE_MS						1000
#define SONAR_SEND_PERIOD_TO_PC_MS				100
#define SONAR_SEND_PERIOD_TO_NODE_MS			0

#ifdef NAGY_ROBOT
#define POWER_KEEP_ALIVE_MS						1000
#define POWER_SEND_PERIOD_TO_PC_MS				100
#define POWER_SEND_PERIOD_TO_NODE_MS			0
#endif





//----- gateway -----
#ifdef NAGY_ROBOT
#define CONSOLE_ON_CANB							1	//??
#define DEADRECK_ON_CANB						1	//??
#define DCWHEEL_ON_CANB							1	//??
#define INPUT_ON_CANB							1	//??
#define SERVO_ON_CANB							1	//??
#define SONAR_ON_CANB							1	//??
#define POWER_ON_CANB							1	//??
#define CARACOLE_ON_CANB						1	//??
#define FIREWHEEL_ON_CANB						1	//??
#else	//KIS_ROBOT
#define CONSOLE_ON_CANB							1	//??
#define DEADRECK_ON_CANB						1	//??
#define DCWHEEL_ON_CANB							1	//??
#define SERVO_ON_CANB							1	//??
#define SONAR_ON_CANB							1	//??
#endif





//----- input -----

#ifdef NAGY_ROBOT

#define INPUT_DIGITAL_START_BUTTON_INDEX					7	//ok
#define INPUT_DIGITAL_COLOR_RED_BUTTON_INDEX				1	//ok
#define INPUT_DIGITAL_COLOR_BLUE_BUTTON_INDEX				2	//ok
#define INPUT_DIGITAL_REAR_LEFT_LIMIT_SWITCH_INDEX			3	//ok
#define INPUT_DIGITAL_REAR_RIGHT_LIMIT_SWITCH_INDEX			6	//ok
#define INPUT_DIGITAL_BALL_PRESENT_INDEX					8	//ok
#define INPUT_DIGITAL_PLUS_0_INDEX							4
#define INPUT_DIGITAL_PLUS_1_INDEX							5
#define INPUT_DIGITAL_PLUS_2_INDEX							0
#define INPUT_DIGITAL_PLUS_3_INDEX							9


#define INPUT_DIGITAL_IS_OUTPUT_START_BUTTON				0	//ok
#define INPUT_DIGITAL_IS_OUTPUT_COLOR_RED_BUTTON			0	//ok
#define INPUT_DIGITAL_IS_OUTPUT_COLOR_BLUE_BUTTON			0	//ok
#define INPUT_DIGITAL_IS_OUTPUT_REAR_LEFT_LIMIT_SWITCH		0	//ok
#define INPUT_DIGITAL_IS_OUTPUT_REAR_RIGHT_LIMIT_SWITCH		0	//ok
#define INPUT_DIGITAL_IS_OUTPUT_BALL_PRESENT				0	//ok
#define INPUT_DIGITAL_IS_OUTPUT_PLUS_0						0
#define INPUT_DIGITAL_IS_OUTPUT_PLUS_1						0
#define INPUT_DIGITAL_IS_OUTPUT_PLUS_2						0
#define INPUT_DIGITAL_IS_OUTPUT_PLUS_3						0

//aktív-e
#define INPUT_DIGITAL_OUTPUT_START_BUTTON_INIT_STATE				0	//ok
#define INPUT_DIGITAL_OUTPUT_COLOR_RED_BUTTON_INIT_STATE			0	//ok
#define INPUT_DIGITAL_OUTPUT_COLOR_BLUE_BUTTON_INIT_STATE			0	//ok
#define INPUT_DIGITAL_OUTPUT_REAR_LEFT_LIMIT_SWITCH_INIT_STATE		0	//ok
#define INPUT_DIGITAL_OUTPUT_REAR_RIGHT_LIMIT_SWITCH_INIT_STATE		0	//ok
#define INPUT_DIGITAL_OUTPUT_BALL_PRESENT_INIT_STATE				0	//ok
#define INPUT_DIGITAL_OUTPUT_PLUS_0_INIT_STATE						0
#define INPUT_DIGITAL_OUTPUT_PLUS_1_INIT_STATE						0
#define INPUT_DIGITAL_OUTPUT_PLUS_2_INIT_STATE						0
#define INPUT_DIGITAL_OUTPUT_PLUS_3_INIT_STATE						0

//kimenetnél is aktív szintet jelent
#define INPUT_DIGITAL_START_BUTTON_ACTIVE_LEVEL				1	//ok
#define INPUT_DIGITAL_COLOR_RED_BUTTON_ACTIVE_LEVEL			1	//ok
#define INPUT_DIGITAL_COLOR_BLUE_BUTTON_ACTIVE_LEVEL		1	//ok
#define INPUT_DIGITAL_REAR_LEFT_LIMIT_SWITCH_ACTIVE_LEVEL	0	//ok
#define INPUT_DIGITAL_REAR_RIGHT_LIMIT_SWITCH_ACTIVE_LEVEL	0	//ok
#define INPUT_DIGITAL_BALL_PRESENT_ACTIVE_LEVEL				0	//??
#define INPUT_DIGITAL_PLUS_0_ACTIVE_LEVEL					0
#define INPUT_DIGITAL_PLUS_1_ACTIVE_LEVEL					0
#define INPUT_DIGITAL_PLUS_2_ACTIVE_LEVEL					0
#define INPUT_DIGITAL_PLUS_3_ACTIVE_LEVEL					0


#define INPUT_ANALOG_LEFT_FRONT_SHARP_INDEX		1	//??
#define INPUT_ANALOG_RIGHT_FRONT_SHARP_INDEX	2	//??
#define INPUT_ANALOG_LEFT_REAR_SHARP_INDEX		3	//??
#define INPUT_ANALOG_RIGHT_REAR_SHARP_INDEX		4	//??
#define INPUT_ANALOG_BALL_COLOR_CNY70_INDEX		6	//??
#define INPUT_ANALOG_PLUS_0_INDEX				5
#define INPUT_ANALOG_PLUS_1_INDEX				0

#endif





//----- servo -----

#ifdef NAGY_ROBOT

#define SERVO_FRONT_ARM_INDEX							4	//??
#define SERVO_REAR_ARM_INDEX							1	//??
#define SERVO_SELECTOR_INDEX							5	//??
#define SERVO_FIRESTOPPER_INDEX							0	//??
#define SERVO_FLIPPER_INDEX								3	//??
#define SERVO_CONTRACTOR_INDEX							2	//??


#define SERVO_FRONT_ARM_DEG_INCR_X0					0
#define SERVO_FRONT_ARM_DEG_INCR_Y0					1650	//ok
#define SERVO_FRONT_ARM_DEG_INCR_X1					90
#define SERVO_FRONT_ARM_DEG_INCR_Y1					790		//ok
#define SERVO_FRONT_ARM_LIMIT_LOW_POS_INCR			750		//ok
#define SERVO_FRONT_ARM_LIMIT_HIGH_POS_INCR			1855	//ok
#define SERVO_FRONT_ARM_LIMIT_CURRENT_MA			500		//??	//[mA]
#define SERVO_FRONT_ARM_START_POS_DEG				90		//ok

#define SERVO_REAR_ARM_DEG_INCR_X0					0
#define SERVO_REAR_ARM_DEG_INCR_Y0					1050	//ok
#define SERVO_REAR_ARM_DEG_INCR_X1					90
#define SERVO_REAR_ARM_DEG_INCR_Y1					1900	//ok
#define SERVO_REAR_ARM_LIMIT_LOW_POS_INCR			860		//ok
#define SERVO_REAR_ARM_LIMIT_HIGH_POS_INCR			1950	//ok
#define SERVO_REAR_ARM_LIMIT_CURRENT_MA				500		//??	//[mA]
#define SERVO_REAR_ARM_START_POS_DEG				90		//ok

#define SERVO_SELECTOR_DEG_INCR_X0						0
#define SERVO_SELECTOR_DEG_INCR_Y0						1740	//ok
#define SERVO_SELECTOR_DEG_INCR_X1						90
#define SERVO_SELECTOR_DEG_INCR_Y1						880		//ok
#define SERVO_SELECTOR_LIMIT_LOW_POS_INCR				875		//ok
#define SERVO_SELECTOR_LIMIT_HIGH_POS_INCR				2120	//ok
#define SERVO_SELECTOR_LIMIT_CURRENT_MA					500		//??	//[mA]
#define SERVO_SELECTOR_START_POS_DEG					22		//ok

#define SERVO_FIRESTOPPER_DEG_INCR_X0					0		//ok, kamu
#define SERVO_FIRESTOPPER_DEG_INCR_Y0					1100	//ok, megakaszt
#define SERVO_FIRESTOPPER_DEG_INCR_X1					90		//ok, kamu
#define SERVO_FIRESTOPPER_DEG_INCR_Y1					1780	//ok, átenged
#define SERVO_FIRESTOPPER_LIMIT_LOW_POS_INCR			800		//ok
#define SERVO_FIRESTOPPER_LIMIT_HIGH_POS_INCR			1780	//ok
#define SERVO_FIRESTOPPER_LIMIT_CURRENT_MA				500		//??	//[mA]
#define SERVO_FIRESTOPPER_START_POS_DEG					0		//ok

#define SERVO_FLIPPER_DEG_INCR_X0						0
#define SERVO_FLIPPER_DEG_INCR_Y0						1000	//??
#define SERVO_FLIPPER_DEG_INCR_X1						180
#define SERVO_FLIPPER_DEG_INCR_Y1						2000	//??
#define SERVO_FLIPPER_LIMIT_LOW_POS_INCR				700		//??
#define SERVO_FLIPPER_LIMIT_HIGH_POS_INCR				2200	//??
#define SERVO_FLIPPER_LIMIT_CURRENT_MA					500		//??	//[mA]
#define SERVO_FLIPPER_START_POS_DEG						0		//??

#define SERVO_CONTRACTOR_DEG_INCR_X0					(210-2*3)		//ok [mm] (lemezek vastagsága 3mm)
#define SERVO_CONTRACTOR_DEG_INCR_Y0					950		//ok
#define SERVO_CONTRACTOR_DEG_INCR_X1					(170-2*3)		//ok [mm] (lemezek vastagsága 3mm)
#define SERVO_CONTRACTOR_DEG_INCR_Y1					2050	//ok
#define SERVO_CONTRACTOR_LIMIT_LOW_POS_INCR				950		//ok
#define SERVO_CONTRACTOR_LIMIT_HIGH_POS_INCR			2050	//ok
#define SERVO_CONTRACTOR_LIMIT_CURRENT_MA				500		//??	//[mA]
#define SERVO_CONTRACTOR_START_POS_DEG					200		//ok

#else	//KIS_ROBOT

#define SERVO_LEFT_LOW_GRIPPER_INDEX					4	//??
#define SERVO_RIGHT_LOW_GRIPPER_INDEX					1	//??
#define SERVO_LEFT_HIGH_GRIPPER_INDEX					5	//??
#define SERVO_RIGHT_HIGH_GRIPPER_INDEX					3	//??
#define SERVO_PLUS_0_INDEX								0	//??
#define SERVO_PLUS_1_INDEX								2	//??


#define SERVO_LEFT_LOW_GRIPPER_DEG_INCR_X0				0
#define SERVO_LEFT_LOW_GRIPPER_DEG_INCR_Y0				1000	//??
#define SERVO_LEFT_LOW_GRIPPER_DEG_INCR_X1				180
#define SERVO_LEFT_LOW_GRIPPER_DEG_INCR_Y1				2000	//??
#define SERVO_LEFT_LOW_GRIPPER_LIMIT_LOW_POS_INCR		700		//??
#define SERVO_LEFT_LOW_GRIPPER_LIMIT_HIGH_POS_INCR		2200	//??
#define SERVO_LEFT_LOW_GRIPPER_LIMIT_CURRENT_MA			500		//??	//[mA]
#define SERVO_LEFT_LOW_GRIPPER_START_POS_DEG			0		//??

#define SERVO_RIGHT_LOW_GRIPPER_DEG_INCR_X0				0
#define SERVO_RIGHT_LOW_GRIPPER_DEG_INCR_Y0				1000	//??
#define SERVO_RIGHT_LOW_GRIPPER_DEG_INCR_X1				180
#define SERVO_RIGHT_LOW_GRIPPER_DEG_INCR_Y1				2000	//??
#define SERVO_RIGHT_LOW_GRIPPER_LIMIT_LOW_POS_INCR		700		//??
#define SERVO_RIGHT_LOW_GRIPPER_LIMIT_HIGH_POS_INCR		2200	//??
#define SERVO_RIGHT_LOW_GRIPPER_LIMIT_CURRENT_MA		500		//??	//[mA]
#define SERVO_RIGHT_LOW_GRIPPER_START_POS_DEG			0		//??

#define SERVO_LEFT_HIGH_GRIPPER_DEG_INCR_X0				0
#define SERVO_LEFT_HIGH_GRIPPER_DEG_INCR_Y0				1000	//??
#define SERVO_LEFT_HIGH_GRIPPER_DEG_INCR_X1				180
#define SERVO_LEFT_HIGH_GRIPPER_DEG_INCR_Y1				2000	//??
#define SERVO_LEFT_HIGH_GRIPPER_LIMIT_LOW_POS_INCR		700		//??
#define SERVO_LEFT_HIGH_GRIPPER_LIMIT_HIGH_POS_INCR		2200	//??
#define SERVO_LEFT_HIGH_GRIPPER_LIMIT_CURRENT_MA		500		//??	//[mA]
#define SERVO_LEFT_HIGH_GRIPPER_START_POS_DEG			0		//??

#define SERVO_RIGHT_HIGH_GRIPPER_DEG_INCR_X0			0
#define SERVO_RIGHT_HIGH_GRIPPER_DEG_INCR_Y0			1000	//??
#define SERVO_RIGHT_HIGH_GRIPPER_DEG_INCR_X1			180
#define SERVO_RIGHT_HIGH_GRIPPER_DEG_INCR_Y1			2000	//??
#define SERVO_RIGHT_HIGH_GRIPPER_LIMIT_LOW_POS_INCR		700		//??
#define SERVO_RIGHT_HIGH_GRIPPER_LIMIT_HIGH_POS_INCR	2200	//??
#define SERVO_RIGHT_HIGH_GRIPPER_LIMIT_CURRENT_MA		500		//??	//[mA]
#define SERVO_RIGHT_HIGH_GRIPPER_START_POS_DEG			0		//??

#define SERVO_PLUS_0_DEG_INCR_X0						0
#define SERVO_PLUS_0_DEG_INCR_Y0						1000
#define SERVO_PLUS_0_DEG_INCR_X1						180
#define SERVO_PLUS_0_DEG_INCR_Y1						2000
#define SERVO_PLUS_0_LIMIT_LOW_POS_INCR					700
#define SERVO_PLUS_0_LIMIT_HIGH_POS_INCR				2200
#define SERVO_PLUS_0_LIMIT_CURRENT_MA					500		//??	//[mA]
#define SERVO_PLUS_0_START_POS_DEG						0		//??

#define SERVO_PLUS_1_DEG_INCR_X0						0
#define SERVO_PLUS_1_DEG_INCR_Y0						1000
#define SERVO_PLUS_1_DEG_INCR_X1						180
#define SERVO_PLUS_1_DEG_INCR_Y1						2000
#define SERVO_PLUS_1_LIMIT_LOW_POS_INCR					700
#define SERVO_PLUS_1_LIMIT_HIGH_POS_INCR				2200
#define SERVO_PLUS_1_LIMIT_CURRENT_MA					500		//??	//[mA]
#define SERVO_PLUS_1_START_POS_DEG						0		//??

#endif






//----- deadreck -----

#ifdef NAGY_ROBOT

//?? ugy kell rasimitani, mintha menne a palyara, ket mesterdarabbal
#define DEADRECK_CALIB_DISTANCE_X				((double)59.9+290/2)	//[mm]				//??
#define DEADRECK_CALIB_DISTANCE_Y				((double)114.8+60/2)	//[mm]				//??
#define DEADRECK_CALIB_PHI						((double)90.0 * M_PI / 180)	//[rad]			//??

//?? saját menés nélkül
#define DEADRECK_WHEEL_DISTANCE_REC					((double)1 / 359.5)		//[1/mm]		//ok, megmérve
#define DEADRECK_LEFT_ONE_INCREMENT_DISTANCE		((double)359.5 / (15781-(-3856)))	//[1/mm]		//ok, gumi nélkül
#define DEADRECK_RIGHT_ONE_INCREMENT_DISTANCE		((double)359.5 / (15781-(-3856)))	//[1/mm]		//ok, gumi nélkül
#define DEADRECK_LEFT_IS_ROTATE_DIR_A				0										//ok
#define DEADRECK_RIGHT_IS_ROTATE_DIR_A				1										//ok
#define DEADRECK_IS_LEFT_EQEP1						1										//ok

#else	//KIS_ROBOT

//?? ugy kell rasimitani, mintha menne a palyara, ket mesterdarabbal
#define DEADRECK_CALIB_DISTANCE_X				((double)59.9+290/2)	//[mm]				//??
#define DEADRECK_CALIB_DISTANCE_Y				((double)114.8+60/2)	//[mm]				//??
#define DEADRECK_CALIB_PHI						((double)90.0 * M_PI / 180)	//[rad]			//??

//?? saját menés nélkül
#define DEADRECK_WHEEL_DISTANCE_REC					((double)1 / 266.4655)	//[1/mm]		//??
#define DEADRECK_LEFT_ONE_INCREMENT_DISTANCE		((double)1 / 67.3033)	//[1/mm]		//??
#define DEADRECK_RIGHT_ONE_INCREMENT_DISTANCE		((double)1 / 67.310)	//[1/mm]		//??
#define DEADRECK_LEFT_IS_ROTATE_DIR_A				0										//??
#define DEADRECK_RIGHT_IS_ROTATE_DIR_A				1										//??
#define DEADRECK_IS_LEFT_EQEP1						1										//??

#endif





//----- power -----

#ifdef NAGY_ROBOT

#define POWER_MAIN_VOLTAGE_SHUTDOWN_LEVEL							18		//??
#define POWER_MAIN_VOLTAGE_STOPBUTTON_LEVEL							3		//??
#define POWER_MAIN_VOLTAGE_LOW_SHUTDOWN_TIME_MS						1000	//??

#define POWER_MAIN_CURRENT_SHUTDOWN_LEVEL							4		//??
#define POWER_ACT_CURRENT_SHUTDOWN_LEVEL							8		//??
#define POWER_MAIN_HIGH_CURRENT_TIME_MS								5		//??
#define POWER_ACT_HIGH_CURRENT_TIME_MS								5		//??

#define POWER_ACCU_VOLTAGE_OK_LEVEL									23		//??

#define POWER_CHARGE_VOLTAGE_READY_LEVEL							26		//??
#define POWER_CHARGE_VOLTAGE_PLUGGED_IN_LEVEL						12		//??

#endif





//----- dcwheel -----

#ifdef NAGY_ROBOT
//????
#define DCWHEEL_GEARBOX								66
#define DCWHEEL_INCR_PER_MOTORROT					3000
#define DCWHEEL_INCR_PER_MM							((double)494317 / 400) //Odett, de most ugyanaz
#define DCWHEEL_INCR_PER_FULL_TURN					((double)6723500 / 10) // Odett, nincs hasznalva
#define DCWHEEL_WHEEL_DISTANCE						161.5 // kezzel mert ertek
#else	//KIS_ROBOT
//????
#define DCWHEEL_GEARBOX								66
#define DCWHEEL_INCR_PER_MOTORROT					3000
#define DCWHEEL_INCR_PER_MM							((double)494317 / 400) //Odett, de most ugyanaz
#define DCWHEEL_INCR_PER_FULL_TURN					((double)6723500 / 10) // Odett, nincs hasznalva
#define DCWHEEL_WHEEL_DISTANCE						161.5 // kezzel mert ertek
#endif

#define DCWHEEL_INCR_PER_WHEELROT					(DCWHEEL_GEARBOX * DCWHEEL_INCR_PER_MOTORROT)

#define DCWHEEL_WHEEL_DISTRICT_MM					((double)DCWHEEL_INCR_PER_WHEELROT / DCWHEEL_INCR_PER_MM)
#define DCWHEEL_WHEEL_DIAMETER_MM					((double)DCWHEEL_WHEEL_DISTRICT_MM / M_PI)

#define DCWHEEL_INCR_TO_MM(incr)					((double)(incr) / DCWHEEL_INCR_PER_MM)

#define DCWHEEL_MM_PER_FULL_TURN					DCWHEEL_INCR_TO_MM(DCWHEEL_INCR_PER_FULL_TURN)

#define DCWHEEL_SEC_PER_MIN							60

#define DCWHEEL_CONV_INCR_TO_MM(incr)				((double)(incr) / DCWHEEL_INCR_PER_MM)
#define DCWHEEL_CONV_RPM_TO_MMS(rpm)				((double)(rpm) / DCWHEEL_SEC_PER_MIN * DCWHEEL_INCR_PER_MOTORROT / DCWHEEL_INCR_PER_MM)
#define DCWHEEL_CONV_MOTORROTS2_TO_MMS2(motorrots2)	((double)(motorrots2) * DCWHEEL_INCR_PER_MOTORROT / DCWHEEL_INCR_PER_MM)

#define DCWHEEL_CONV_MM_TO_INCR(mm)					((double)(mm) * DCWHEEL_INCR_PER_MM)
#define DCWHEEL_CONV_MMS_TO_RPM(mms)				((double)(mms) * DCWHEEL_SEC_PER_MIN * DCWHEEL_INCR_PER_MM / DCWHEEL_INCR_PER_MOTORROT)
#define DCWHEEL_CONV_MMS2_TO_MOTORROTS2(mms2)		((double)(mms2) * DCWHEEL_INCR_PER_MM / DCWHEEL_INCR_PER_MOTORROT)

#define DCWHEEL_CONV_RADS_TO_RPM(rads)				((double)(rads) * DCWHEEL_WHEEL_DISTANCE / 2 / DCWHEEL_CONV_RPM_TO_MMS(1))
#define DCWHEEL_CONV_RADS2_TO_MOTORROTS2(rads2)		((double)(rads2) * DCWHEEL_WHEEL_DISTANCE / 2 / DCWHEEL_CONV_RPM_TO_MMS(1) / 60)

#define DCWHEEL_CONV_ACC(acc)						((u16)DCWHEEL_CONV_MMS2_TO_MOTORROTS2(acc))
#define DCWHEEL_CONV_SPEED(speed)					((s16)DCWHEEL_CONV_MMS_TO_RPM(speed))
//#define DCWHEEL_CONV_DIST(dist)						((s32)DCWHEEL_CONV_MM_TO_INCR(dist))

#define DCWHEEL_CONV_BETA(beta)						((u16)DCWHEEL_CONV_RADS2_TO_MOTORROTS2(beta))
#define DCWHEEL_CONV_OMEGA(omega)					((u16)DCWHEEL_CONV_RADS_TO_RPM(omega))


#ifdef NAGY_ROBOT
#define DCWHEEL_IS_LEFT_MOTOR1						1		//??
//#define DCWHEEL_WHEEL_DISTANCE						lsd feljebb
#define DCWHEEL_RPM_TO_MMPS							DCWHEEL_CONV_RPM_TO_MMS(1)
#define DCWHEEL_MM_TO_INCR							DCWHEEL_INCR_PER_MM
#define DCWHEEL_LEFT_IS_ROTATE_DIR_A				0		//??
#define DCWHEEL_RIGHT_IS_ROTATE_DIR_A				1		//??
#define DCWHEEL_LEFT_PLUS_FORWARD					1		//??
#define DCWHEEL_RIGHT_PLUS_FORWARD					1		//??
#define DCWHEEL_IS_LEFT_EQEP1						1		//??
#define DCWHEEL_OE_CONTROL_P						0.0007	//??
#define DCWHEEL_OE_CONTROL_I						0.0		//??
#define DCWHEEL_OE_CONTROL_D						0.2		//??
#else	//KIS_ROBOT
#define DCWHEEL_IS_LEFT_MOTOR1						1		//??
//#define DCWHEEL_WHEEL_DISTANCE						lsd feljebb
#define DCWHEEL_RPM_TO_MMPS							DCWHEEL_CONV_RPM_TO_MMS(1)
#define DCWHEEL_MM_TO_INCR							DCWHEEL_INCR_PER_MM
#define DCWHEEL_LEFT_IS_ROTATE_DIR_A				0		//??
#define DCWHEEL_RIGHT_IS_ROTATE_DIR_A				1		//??
#define DCWHEEL_LEFT_PLUS_FORWARD					1		//??
#define DCWHEEL_RIGHT_PLUS_FORWARD					1		//??
#define DCWHEEL_IS_LEFT_EQEP1						1		//??
#define DCWHEEL_OE_CONTROL_P						0.0007	//??
#define DCWHEEL_OE_CONTROL_I						0.0		//??
#define DCWHEEL_OE_CONTROL_D						0.2		//??
#endif





//----- console -----

#ifdef NAGY_ROBOT
#define CONSOLE_INCR_MM_X0					0								//ok
#define CONSOLE_INCR_MM_Y0					258.5							//ok
#define CONSOLE_INCR_MM_X1					8200							//ok
#define CONSOLE_INCR_MM_Y1					0								//ok
#else	//KIS_ROBOT
#define CONSOLE_INCR_MM_X0					0								//??
#define CONSOLE_INCR_MM_Y0					0								//??
#define CONSOLE_INCR_MM_X1					12967	//12967 incr = 100 mm	//??
#define CONSOLE_INCR_MM_Y1					100								//??
#endif

#define CONSOLE_INCR_MM_GRAD				GET_GRAD(CONSOLE_INCR_MM_X0, CONSOLE_INCR_MM_Y0, CONSOLE_INCR_MM_X1, CONSOLE_INCR_MM_Y1)

#define CONSOLE_CONV_INCR_TO_MM(incr)				(((double)(incr) - CONSOLE_INCR_MM_X0) * CONSOLE_INCR_MM_GRAD + CONSOLE_INCR_MM_Y0)

#define CONSOLE_CONV_MM_TO_INCR(mm)					((u32)(((double)(mm) - CONSOLE_INCR_MM_Y0) / CONSOLE_INCR_MM_GRAD + CONSOLE_INCR_MM_X0))
#define CONSOLE_CONV_MMS_TO_INCRS(mms)				((u32)((double)(mms) / CONSOLE_INCR_MM_GRAD))
#define CONSOLE_CONV_MMS2_TO_INCRS2(mms2)			((u32)((double)(mms2) / CONSOLE_INCR_MM_GRAD))


#ifdef NAGY_ROBOT
#define CONSOLE_CONTOLLER_LIMIT_LOW_POS_INCR		200				//ok
#define CONSOLE_CONTOLLER_LIMIT_HIGH_POS_INCR		8200	//0mm	//ok
#define CONSOLE_HARDWARE_LIMIT_LOW_POS_INCR			100		//?mm	//ok
#define CONSOLE_HARDWARE_LIMIT_HIGH_POS_INCR		8248			//ok
#define CONSOLE_CONTROLLER_PARAMETER_P				0.2				//??
#define CONSOLE_CONTROLLER_PARAMETER_I				0.0				//??
#define CONSOLE_CONTROLLER_PARAMETER_D				0.0				//??
#define CONSOLE_PWM_MODE							3				//??
#define CONSOLE_LIMIT_SWITCH_IS_ACTIVE_HIGH			0				//??
#define CONSOLE_IS_ENC_EQEP1						1				//??
#define CONSOLE_MOTOR_PLUS_UP						1				//??
#define CONSOLE_ENC_IS_ROTATE_DIR_A					0				//??
#else	//KIS_ROBOT
#define CONSOLE_CONTOLLER_LIMIT_LOW_POS_INCR		220				//??
#define CONSOLE_CONTOLLER_LIMIT_HIGH_POS_INCR		20600			//??
#define CONSOLE_HARDWARE_LIMIT_LOW_POS_INCR			123		//1mm	//??
#define CONSOLE_HARDWARE_LIMIT_HIGH_POS_INCR		20600	//120mm	//??
#define CONSOLE_CONTROLLER_PARAMETER_P				0.2				//??
#define CONSOLE_CONTROLLER_PARAMETER_I				0.0				//??
#define CONSOLE_CONTROLLER_PARAMETER_D				0.0				//??
#define CONSOLE_PWM_MODE							3				//??
#define CONSOLE_LIMIT_SWITCH_IS_ACTIVE_HIGH			0				//??
#define CONSOLE_IS_ENC_EQEP1						1				//??
#define CONSOLE_MOTOR_PLUS_UP						1				//??
#define CONSOLE_ENC_IS_ROTATE_DIR_A					1				//??
#endif





//----- caracole -----

#ifdef NAGY_ROBOT

#define CARACOLE_INCR_RAD_X0					0		//??
#define CARACOLE_INCR_RAD_Y0					0		//??
#define CARACOLE_INCR_RAD_X1					100//4096	//??
#define CARACOLE_INCR_RAD_Y1					100//M_PI	//??
#define CARACOLE_INCR_RAD_GRAD				GET_GRAD(CARACOLE_INCR_RAD_X0, CARACOLE_INCR_RAD_Y0, CARACOLE_INCR_RAD_X1, CARACOLE_INCR_RAD_Y1)

#define CARACOLE_CONV_INCRS_TO_RADS(incrs)				((double)(incrs) * CARACOLE_INCR_RAD_GRAD)

#define CARACOLE_CONV_RADS_TO_INCRS(rads)				((double)(rads) / CARACOLE_INCR_RAD_GRAD)
#define CARACOLE_CONV_RADS2_TO_INCRS2(rads2)			((double)(rads2) / CARACOLE_INCR_RAD_GRAD)

#define CARACOLE_CONTROLLER_PARAMETER_P					0.01			//??
#define CARACOLE_CONTROLLER_PARAMETER_I					0.0002			//??
#define CARACOLE_CONTROLLER_PARAMETER_D					0.01			//??
#define CARACOLE_IS_ENC_EQEP1							1				//??
#define CARACOLE_MOTOR_PLUS_UP							1				//??
#define CARACOLE_ENC_IS_ROTATE_DIR_A					1				//??
#define CARACOLE_IS_MOTOR_OUTPUT1						0				//??

#endif





//----- firewheel -----

#ifdef NAGY_ROBOT

#define FIREWHEEL_INCR_RAD_X0					0		//??
#define FIREWHEEL_INCR_RAD_Y0					0		//??
#define FIREWHEEL_INCR_RAD_X1					100//4096	//??
#define FIREWHEEL_INCR_RAD_Y1					100//M_PI	//??
#define FIREWHEEL_INCR_RAD_GRAD				GET_GRAD(FIREWHEEL_INCR_RAD_X0, FIREWHEEL_INCR_RAD_Y0, FIREWHEEL_INCR_RAD_X1, FIREWHEEL_INCR_RAD_Y1)

#define FIREWHEEL_CONV_INCRS_TO_RADS(incrs)				((double)(incrs) * FIREWHEEL_INCR_RAD_GRAD)

#define FIREWHEEL_CONV_RADS_TO_INCRS(rads)				((double)(rads) / FIREWHEEL_INCR_RAD_GRAD)
#define FIREWHEEL_CONV_RADS2_TO_INCRS2(rads2)			((double)(rads2) / FIREWHEEL_INCR_RAD_GRAD)

#define FIREWHEEL_CONTROLLER_PARAMETER_P				0.001			//??
#define FIREWHEEL_CONTROLLER_PARAMETER_I				0.0				//??
#define FIREWHEEL_CONTROLLER_PARAMETER_D				0.0				//??
#define FIREWHEEL_IS_ENC_EQEP1							0				//??
#define FIREWHEEL_MOTOR_PLUS_FIRE						0				//??
#define FIREWHEEL_ENC_IS_ROTATE_DIR_A					1				//??
#define FIREWHEEL_IS_MOTOR_OUTPUT1						1				//??

#endif





//----- sonar -----
#define SONAR_TX_RED		2098.0
#define SONAR_TY_RED		3098.0
#define SONAR_ALPHA_RED		M_PI

#define SONAR_TX_BLUE		-98.0
#define SONAR_TY_BLUE		-98.0
#define SONAR_ALPHA_BLUE	0.0





#endif	//PARAM_H_
