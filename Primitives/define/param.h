#ifndef PARAM_H_
#define PARAM_H_





#include <math.h>





//----- konstans -----
#define SERVO_COUNT							6

#define MAGNET_COUNT						2

#define INPUT_ANALOG_COUNT					7
#define INPUT_DIGITAL_COUNT					10

#define ACT_STARTED							true
#define ACT_START_ERROR						false

#define MOTION_NO_ERROR						0
#define MOTION_ERROR						1

#define COLOR_RED							false
#define COLOR_BLUE							true

#define AREA_LENGTH_X						2100
#define AREA_LENGTH_Y						3000





//----- macro -----
//kesobb:
//AAA_BBB_CCC_X0:		AAA node-nal a BBB-CCC fuggveny X0, Y0, X1, Y1 pontjai, hogy egy egyenest ket ponttal adjunk meg
//AAA_BBB_CCC_GRAD:		AAA node-nal a BBB-CCC fuggveny meredeksege (dCCC / dBBB)
#define GET_GRAD(X0, Y0, X1, Y1)			( ((double)(Y1) - (double)(Y0)) / ((double)(X1) - (double)(X0)) )





//----- atszamitas -----
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

#define DEADRECK_KEEP_ALIVE_MS					1000
#define DEADRECK_SEND_PERIOD_TO_PC_MS			100
#define DEADRECK_SEND_PERIOD_TO_NODE_MS			5//3	//!!!ne legyen tul kicsi

#define BDC_KEEP_ALIVE_MS						1000
#define BDC_SEND_PERIOD_TO_PC_MS				100
#define BDC_SEND_PERIOD_TO_NODE_MS				0

#define INPUT_KEEP_ALIVE_MS						1000
#define INPUT_SEND_PERIOD_TO_PC_MS				100
#define INPUT_SEND_PERIOD_TO_NODE_MS			0

#define MAGNET_KEEP_ALIVE_MS					1000
#define MAGNET_SEND_PERIOD_TO_PC_MS				100
#define MAGNET_SEND_PERIOD_TO_NODE_MS			0

#define SERVO_KEEP_ALIVE_MS						1000
#define SERVO_SEND_PERIOD_TO_PC_MS				100
#define SERVO_SEND_PERIOD_TO_NODE_MS			0

#define SONAR_KEEP_ALIVE_MS						1000
#define SONAR_SEND_PERIOD_TO_PC_MS				100
#define SONAR_SEND_PERIOD_TO_NODE_MS			0

#define POWER_KEEP_ALIVE_MS						1000
#define POWER_SEND_PERIOD_TO_PC_MS				100
#define POWER_SEND_PERIOD_TO_NODE_MS			0





//----- gateway -----
#define CONSOLE_ON_CANB							1
#define DEADRECK_ON_CANB						1
#define BDC_ON_CANB								1
#define INPUT_ON_CANB							1
#define MAGNET_ON_CANB							1
#define SERVO_ON_CANB							1
#define SONAR_ON_CANB							1
#define POWER_ON_CANB							1





//----- magnet -----
#define MAGNET_PUSH							-1
#define MAGNET_OFF							0
#define MAGNET_PULL							1

#define MAGNET_LEFT_INDEX									0
#define MAGNET_RIGHT_INDEX									1

#define MAGNET_m24V							0x01	//01: -24V
#define MAGNET_0V							0x00	//00: 0V
#define MAGNET_p24V							0xFF	//FF: +24V

#define MAGNET_LEFT_PULL_IS_p24V			1
#define MAGNET_RIGHT_PULL_IS_p24V			0





//----- input -----
#define INPUT_DIGITAL_START_BUTTON_INDEX					6
#define INPUT_DIGITAL_COLOR_BUTTON_INDEX					7
#define INPUT_DIGITAL_FRONT_LEFT_LIMIT_SWITCH_INDEX			1
#define INPUT_DIGITAL_FRONT_RIGHT_LIMIT_SWITCH_INDEX		8
#define INPUT_DIGITAL_PAWN_IN_GRIPPER_INDEX					2
#define INPUT_DIGITAL_PLUS_0_INDEX							4
#define INPUT_DIGITAL_PLUS_1_INDEX							5
#define INPUT_DIGITAL_PLUS_2_INDEX							0
#define INPUT_DIGITAL_PLUS_3_INDEX							9
#define INPUT_DIGITAL_PLUS_4_INDEX							3

#define INPUT_DIGITAL_START_BUTTON_ACTIVE_LEVEL				1
#define INPUT_DIGITAL_COLOR_BUTTON_ACTIVE_LEVEL				0	//aktiv a kek
#define INPUT_DIGITAL_FRONT_LEFT_LIMIT_SWITCH_ACTIVE_LEVEL	0
#define INPUT_DIGITAL_FRONT_RIGHT_LIMIT_SWITCH_ACTIVE_LEVEL	0
#define INPUT_DIGITAL_PAWN_IN_GRIPPER_ACTIVE_LEVEL			0
#define INPUT_DIGITAL_PLUS_0_ACTIVE_LEVEL					0
#define INPUT_DIGITAL_PLUS_1_ACTIVE_LEVEL					0
#define INPUT_DIGITAL_PLUS_2_ACTIVE_LEVEL					0
#define INPUT_DIGITAL_PLUS_3_ACTIVE_LEVEL					0
#define INPUT_DIGITAL_PLUS_4_ACTIVE_LEVEL					0


#define INPUT_ANALOG_LEFT_HIGH_SHARP_INDEX		4
#define INPUT_ANALOG_LEFT_LOW_SHARP_INDEX		5
#define INPUT_ANALOG_RIGHT_HIGH_SHARP_INDEX		2
#define INPUT_ANALOG_RIGHT_LOW_SHARP_INDEX		3
#define INPUT_ANALOG_LEFT_FRONT_SHARP_INDEX		1
#define INPUT_ANALOG_RIGHT_FRONT_SHARP_INDEX	0
#define INPUT_ANALOG_PLUS_0_INDEX				6





//----- servo -----
#define SERVO_GRIPPER_MAX_SPEED						1000	//deg/s
#define SERVO_GRIPPER_MAX_ACC						1000	//deg/s^2


#define SERVO_GRIPPER_LEFT_INDEX						1
#define SERVO_GRIPPER_RIGHT_INDEX						0
#define SERVO_LEFT_ARM_INDEX							3
#define SERVO_RIGHT_ARM_INDEX							2
#define SERVO_PLUS_0_INDEX								4
#define SERVO_PLUS_1_INDEX								5


#define SERVO_GRIPPER_LEFT_DEG_INCR_X0					0
#define SERVO_GRIPPER_LEFT_DEG_INCR_Y0					1136	//ok
#define SERVO_GRIPPER_LEFT_DEG_INCR_X1					90
#define SERVO_GRIPPER_LEFT_DEG_INCR_Y1					2073	//ok
#define SERVO_GRIPPER_LEFT_LIMIT_LOW_POS_INCR			1030	//ok	//-10deg
#define SERVO_GRIPPER_LEFT_LIMIT_HIGH_POS_INCR			2335	//ok	//+115deg

#define SERVO_GRIPPER_RIGHT_DEG_INCR_X0					0
#define SERVO_GRIPPER_RIGHT_DEG_INCR_Y0					1875	//ok
#define SERVO_GRIPPER_RIGHT_DEG_INCR_X1					90
#define SERVO_GRIPPER_RIGHT_DEG_INCR_Y1					990	//ok
#define SERVO_GRIPPER_RIGHT_LIMIT_LOW_POS_INCR			740	//ok	//+115deg
#define SERVO_GRIPPER_RIGHT_LIMIT_HIGH_POS_INCR			1975	//ok	//-10deg

#define SERVO_LEFT_ARM_DEG_INCR_X0						0
#define SERVO_LEFT_ARM_DEG_INCR_Y0						1021	//ok, fuggoleges
#define SERVO_LEFT_ARM_DEG_INCR_X1						90
#define SERVO_LEFT_ARM_DEG_INCR_Y1						1802	//ok, vizszintes
#define SERVO_LEFT_ARM_LIMIT_LOW_POS_INCR				1020	//ok
#define SERVO_LEFT_ARM_LIMIT_HIGH_POS_INCR				2200	//ok

#define SERVO_RIGHT_ARM_DEG_INCR_X0						0
#define SERVO_RIGHT_ARM_DEG_INCR_Y0						1812	//ok, fuggoleges
#define SERVO_RIGHT_ARM_DEG_INCR_X1						90
#define SERVO_RIGHT_ARM_DEG_INCR_Y1						1066	//ok, vizszintes
#define SERVO_RIGHT_ARM_LIMIT_LOW_POS_INCR				700	//ok
#define SERVO_RIGHT_ARM_LIMIT_HIGH_POS_INCR				1820	//ok

#define SERVO_PLUS_0_DEG_INCR_X0						0
#define SERVO_PLUS_0_DEG_INCR_Y0						1000
#define SERVO_PLUS_0_DEG_INCR_X1						180
#define SERVO_PLUS_0_DEG_INCR_Y1						2000
#define SERVO_PLUS_0_LIMIT_LOW_POS_INCR					700
#define SERVO_PLUS_0_LIMIT_HIGH_POS_INCR				2200

#define SERVO_PLUS_1_DEG_INCR_X0						0
#define SERVO_PLUS_1_DEG_INCR_Y0						1000
#define SERVO_PLUS_1_DEG_INCR_X1						180
#define SERVO_PLUS_1_DEG_INCR_Y1						2000
#define SERVO_PLUS_1_LIMIT_LOW_POS_INCR					700
#define SERVO_PLUS_1_LIMIT_HIGH_POS_INCR				2200





//----- deadreck -----
//az Y falra kell rasimitani
#define DEADRECK_CALIB_DISTANCE_X				((double)167.4)	//[mm]
#define DEADRECK_CALIB_DISTANCE_Y				((double)174)	//[mm]
#define DEADRECK_CALIB_PHI						((double)53.75 * M_PI / 180)	//[rad]

// originalt megoldasok
//#define DEADRECK_WHEEL_DISTANCE_REC					((double)1 / 261.274198830535)
//#define DEADRECK_LEFT_ONE_INCREMENT_DISTANCE		((double)1 / 67.517166666666)
//#define DEADRECK_RIGHT_ONE_INCREMENT_DISTANCE		((double)1 / 67.517166666666)

// elso kalibracio utan mar igy:
#define DEADRECK_WHEEL_DISTANCE_REC					((double)1 / 262.0157386)
#define DEADRECK_LEFT_ONE_INCREMENT_DISTANCE		((double)1 / 67.59514464)
#define DEADRECK_RIGHT_ONE_INCREMENT_DISTANCE		((double)1 / 67.43936839)
#define DEADRECK_LEFT_IS_ROTATE_DIR_A				0
#define DEADRECK_RIGHT_IS_ROTATE_DIR_A				1
#define DEADRECK_IS_LEFT_EQEP1						0





//----- power -----
#define POWER_MAIN_VOLTAGE_SHUTDOWN_LEVEL							20
#define POWER_MAIN_VOLTAGE_STOPBUTTON_LEVEL							3
#define POWER_MAIN_VOLTAGE_LOW_SHUTDOWN_TIME_MS						1000

#define POWER_MAIN_CURRENT_SHUTDOWN_LEVEL							4
#define POWER_ACT_CURRENT_SHUTDOWN_LEVEL							8
#define POWER_MAIN_HIGH_CURRENT_TIME_MS								5
#define POWER_ACT_HIGH_CURRENT_TIME_MS								5

#define POWER_ACCU_VOLTAGE_OK_LEVEL									23

#define POWER_CHARGE_VOLTAGE_READY_LEVEL							26
#define POWER_CHARGE_VOLTAGE_PLUGGED_IN_LEVEL						12





//----- bdc -----
#define BDC_GEARBOX									66
#define BDC_INCR_PER_MOTORROT						3000
#define BDC_INCR_PER_WHEELROT						(BDC_GEARBOX * BDC_INCR_PER_MOTORROT)

#define BDC_INCR_PER_MM								((double)494317 / 400)
#define BDC_WHEEL_DISTRICT_MM						((double)BDC_INCR_PER_WHEELROT / BDC_INCR_PER_MM)
#define BDC_WHEEL_DIAMETER_MM						((double)BDC_WHEEL_DISTRICT_MM / M_PI)

#define BDC_INCR_TO_MM(incr)						((double)(incr) / BDC_INCR_PER_MM)

#define BDC_INCR_PER_FULL_TURN						((double)6723500 / 10)
#define BDC_MM_PER_FULL_TURN						BDC_INCR_TO_MM(BDC_INCR_PER_FULL_TURN)

#define BDC_WHEEL_DISTANCE							((double)BDC_MM_PER_FULL_TURN / M_PI)

#define BDC_SEC_PER_MIN								60

#define BDC_CONV_INCR_TO_MM(incr)					((double)(incr) / BDC_INCR_PER_MM)
#define BDC_CONV_RPM_TO_MMS(rpm)					((double)(rpm) / BDC_SEC_PER_MIN * BDC_INCR_PER_MOTORROT / BDC_INCR_PER_MM)
#define BDC_CONV_MOTORROTS2_TO_MMS2(motorrots2)		((double)(motorrots2) * BDC_INCR_PER_MOTORROT / BDC_INCR_PER_MM)

#define BDC_CONV_MM_TO_INCR(mm)						((double)(mm) * BDC_INCR_PER_MM)
#define BDC_CONV_MMS_TO_RPM(mms)					((double)(mms) * BDC_SEC_PER_MIN * BDC_INCR_PER_MM / BDC_INCR_PER_MOTORROT)
#define BDC_CONV_MMS2_TO_MOTORROTS2(mms2)			((double)(mms2) * BDC_INCR_PER_MM / BDC_INCR_PER_MOTORROT)

#define BDC_CONV_RADS_TO_RPM(rads)					((double)(rads) * BDC_WHEEL_DISTANCE / 2 / BDC_CONV_RPM_TO_MMS(1))
#define BDC_CONV_RADS2_TO_MOTORROTS2(rads2)			((double)(rads2) * BDC_WHEEL_DISTANCE / 2 / BDC_CONV_RPM_TO_MMS(1) / 60)

#define BDC_CONV_ACC(acc)							((u16)BDC_CONV_MMS2_TO_MOTORROTS2(acc))
#define BDC_CONV_SPEED(speed)						((u16)BDC_CONV_MMS_TO_RPM(speed))
//#define BDC_CONV_DIST(dist)							((s32)BDC_CONV_MM_TO_INCR(dist))

#define BDC_CONV_BETA(beta)							((u16)BDC_CONV_RADS2_TO_MOTORROTS2(beta))
#define BDC_CONV_OMEGA(omega)						((u16)BDC_CONV_RADS_TO_RPM(omega))


#define BDC_IS_LEFT_MOTOR1							1
//#define BDC_WHEEL_DISTANCE							((double)BDC_MM_PER_FULL_TURN / M_PI)
#define BDC_ROBOT_FULL_TURN_INCR					BDC_INCR_PER_FULL_TURN
#define BDC_RPM_TO_MMPS								BDC_CONV_RPM_TO_MMS(1)
#define BDC_MM_TO_INCR								BDC_INCR_PER_MM
#define BDC_PRE_BRAKE_TIME							0.3	//sec
#define BDC_OE_CONTROL_P							6000
#define BDC_OE_CONTROL_I							5





//----- console -----
#define CONSOLE_INCR_MM_X0					0
#define CONSOLE_INCR_MM_Y0					0
#define CONSOLE_INCR_MM_X1					4112	//4112.07 incr = 1.0 mm
#define CONSOLE_INCR_MM_Y1					1
#define CONSOLE_INCR_MM_GRAD				GET_GRAD(CONSOLE_INCR_MM_X0, CONSOLE_INCR_MM_Y0, CONSOLE_INCR_MM_X1, CONSOLE_INCR_MM_Y1)

#define CONSOLE_CONV_INCR_TO_MM(incr)				(((double)(incr) - CONSOLE_INCR_MM_X0) * CONSOLE_INCR_MM_GRAD + CONSOLE_INCR_MM_Y0)

#define CONSOLE_CONV_MM_TO_INCR(mm)					((u32)(((double)(mm) - CONSOLE_INCR_MM_Y0) / CONSOLE_INCR_MM_GRAD + CONSOLE_INCR_MM_X0))
#define CONSOLE_CONV_MMS_TO_INCRS(mms)				((u32)((double)(mms) / CONSOLE_INCR_MM_GRAD))
#define CONSOLE_CONV_MMS2_TO_INCRS2(mms2)			((u32)((double)(mms2) / CONSOLE_INCR_MM_GRAD))


#define CONSOLE_CONTOLLER_LIMIT_LOW_POS_INCR		14000
#define CONSOLE_CONTOLLER_LIMIT_HIGH_POS_INCR		524000
#define CONSOLE_HARDWARE_LIMIT_LOW_POS_INCR			12000
#define CONSOLE_HARDWARE_LIMIT_HIGH_POS_INCR		526000
#define CONSOLE_CONTROLLER_PARAMETER_P				1.0
#define CONSOLE_CONTROLLER_PARAMETER_I				0.0
#define CONSOLE_CONTROLLER_PARAMETER_D				0.0
#define CONSOLE_PWM_MODE							3
#define CONSOLE_LIMIT_SWITCH_IS_ACTIVE_HIGH			1
#define CONSOLE_IS_ENC_EQEP1						1
#define CONSOLE_MOTOR_PLUS_UP						1
#define CONSOLE_ENC_IS_ROTATE_DIR_A					0





//----- sonar -----
#define SONAR_TX_BLUE		2202.0
#define SONAR_TY_BLUE		3102.0
#define SONAR_ALPHA_BLUE	M_PI

#define SONAR_TX_RED		-102.0
#define SONAR_TY_RED		-102.0
#define SONAR_ALPHA_RED		0.0





#endif	//PARAM_H_
