#ifndef PARAM_H_
#define PARAM_H_





#include <math.h>





//----- konstans -----
#define SERVO_COUNT							6

#define VACUUM_COUNT						2

#define INPUT_ANALOG_COUNT					7
#define INPUT_DIGITAL_COUNT					10
#define INPUT_DIGITAL_OUTPUT_COUNT			4
#define INPUT_DIGITAL_OUTPUT_MIN_INDEX		6

#define SONAR_TRANSMITTER_COUNT				3

#define ACT_STARTED							true
#define ACT_START_ERROR						false

#define MOTION_NO_ERROR						0
#define MOTION_ERROR						1

#define COLOR_PURPLE						-1
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
#define DEADRECK_SEND_PERIOD_TO_NODE_MS			1

#define BDC_KEEP_ALIVE_MS						1000
#define BDC_SEND_PERIOD_TO_PC_MS				100
#define BDC_SEND_PERIOD_TO_NODE_MS				0

#define INPUT_KEEP_ALIVE_MS						1000
#define INPUT_SEND_PERIOD_TO_PC_MS				100
#define INPUT_SEND_PERIOD_TO_NODE_MS			0

#define VACUUM_KEEP_ALIVE_MS					1000
#define VACUUM_SEND_PERIOD_TO_PC_MS				100
#define VACUUM_SEND_PERIOD_TO_NODE_MS			0

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
#define VACUUM_ON_CANB							1
#define SERVO_ON_CANB							1
#define SONAR_ON_CANB							1
#define POWER_ON_CANB							1





//----- vacuum -----
#define VACUUM_COMPRESSOR_INDEX				0
#define VACUUM_VALVE_INDEX					1

#define VACUUM_IS_COMPRESSOR_ON_M2				1
#define VACUUM_IS_COMPRESSOR_POLARITY_PLUS		1
#define VACUUM_IS_VALVE_POLARITY_PLUS			1





//----- input -----
#define INPUT_DIGITAL_START_BUTTON_INDEX					7	//ok
#define INPUT_DIGITAL_COLOR_RED_BUTTON_INDEX				9	//ok
#define INPUT_DIGITAL_COLOR_PURPLE_BUTTON_INDEX				8	//ok
#define INPUT_DIGITAL_FRONT_LEFT_LIMIT_SWITCH_INDEX			1
#define INPUT_DIGITAL_FRONT_RIGHT_LIMIT_SWITCH_INDEX		2
#define INPUT_DIGITAL_EYE_COLOR_INDEX						6	//??
#define INPUT_DIGITAL_RESET_PRESSURE_INDEX					3	//??
#define INPUT_DIGITAL_PLUS_0_INDEX							4
#define INPUT_DIGITAL_PLUS_1_INDEX							5
#define INPUT_DIGITAL_PLUS_2_INDEX							0


#define INPUT_DIGITAL_IS_OUTPUT_START_BUTTON				0
#define INPUT_DIGITAL_IS_OUTPUT_COLOR_RED_BUTTON			0
#define INPUT_DIGITAL_IS_OUTPUT_COLOR_PURPLE_BUTTON			0
#define INPUT_DIGITAL_IS_OUTPUT_FRONT_LEFT_LIMIT_SWITCH		0
#define INPUT_DIGITAL_IS_OUTPUT_FRONT_RIGHT_LIMIT_SWITCH	0
#define INPUT_DIGITAL_IS_OUTPUT_EYE_COLOR					0	//?? ide 1 kell
#define INPUT_DIGITAL_IS_OUTPUT_RESET_PRESSURE				0	//?? ide 1 kell
#define INPUT_DIGITAL_IS_OUTPUT_PLUS_0						0
#define INPUT_DIGITAL_IS_OUTPUT_PLUS_1						0
#define INPUT_DIGITAL_IS_OUTPUT_PLUS_2						0

//aktív-e
#define INPUT_DIGITAL_OUTPUT_START_BUTTON_INIT_STATE				0
#define INPUT_DIGITAL_OUTPUT_COLOR_RED_BUTTON_INIT_STATE			0
#define INPUT_DIGITAL_OUTPUT_COLOR_PURPLE_BUTTON_INIT_STATE			0
#define INPUT_DIGITAL_OUTPUT_FRONT_LEFT_LIMIT_SWITCH_INIT_STATE		0
#define INPUT_DIGITAL_OUTPUT_FRONT_RIGHT_LIMIT_SWITCH_INIT_STATE	0
#define INPUT_DIGITAL_OUTPUT_EYE_COLOR_INIT_STATE					0
#define INPUT_DIGITAL_OUTPUT_RESET_PRESSURE_INIT_STATE				0
#define INPUT_DIGITAL_OUTPUT_PLUS_0_INIT_STATE						0
#define INPUT_DIGITAL_OUTPUT_PLUS_1_INIT_STATE						0
#define INPUT_DIGITAL_OUTPUT_PLUS_2_INIT_STATE						0

//kimenetnél is aktív szintet jelent
#define INPUT_DIGITAL_START_BUTTON_ACTIVE_LEVEL				1
#define INPUT_DIGITAL_COLOR_RED_BUTTON_ACTIVE_LEVEL			0
#define INPUT_DIGITAL_COLOR_PURPLE_BUTTON_ACTIVE_LEVEL		0
#define INPUT_DIGITAL_FRONT_LEFT_LIMIT_SWITCH_ACTIVE_LEVEL	0
#define INPUT_DIGITAL_FRONT_RIGHT_LIMIT_SWITCH_ACTIVE_LEVEL	0
#define INPUT_DIGITAL_EYE_COLOR_ACTIVE_LEVEL				1	//1: piros
#define INPUT_DIGITAL_RESET_PRESSURE_ACTIVE_LEVEL			1
#define INPUT_DIGITAL_PLUS_0_ACTIVE_LEVEL					0
#define INPUT_DIGITAL_PLUS_1_ACTIVE_LEVEL					0
#define INPUT_DIGITAL_PLUS_2_ACTIVE_LEVEL					0


#define INPUT_ANALOG_LEFT_FRONT_SHARP_INDEX		1
#define INPUT_ANALOG_RIGHT_FRONT_SHARP_INDEX	0
#define INPUT_ANALOG_PRESSURE_INDEX				3
#define INPUT_ANALOG_PLUS_0_INDEX				6
#define INPUT_ANALOG_PLUS_1_INDEX				4
#define INPUT_ANALOG_PLUS_2_INDEX				5
#define INPUT_ANALOG_PLUS_3_INDEX				2





//----- servo -----
#define SERVO_GRIPPER_MAX_SPEED						1000	//deg/s
#define SERVO_GRIPPER_MAX_ACC						1000	//deg/s^2


#define SERVO_LEFT_GRIPPER_INDEX						4	//ok
#define SERVO_RIGHT_GRIPPER_INDEX						1	//ok
#define SERVO_LEFT_CLAW_INDEX							5	//ok
#define SERVO_RIGHT_CLAW_INDEX							2	//ok
#define SERVO_ARM_INDEX									3	//ok
#define SERVO_PLUS_0_INDEX								0


#define SERVO_LEFT_GRIPPER_DEG_INCR_X0					0
#define SERVO_LEFT_GRIPPER_DEG_INCR_Y0					905		//ok
#define SERVO_LEFT_GRIPPER_DEG_INCR_X1					90
#define SERVO_LEFT_GRIPPER_DEG_INCR_Y1					1845	//ok
#define SERVO_LEFT_GRIPPER_LIMIT_LOW_POS_INCR			905		//ok	//0deg, benn
#define SERVO_LEFT_GRIPPER_LIMIT_HIGH_POS_INCR			2200	//ok	//kinn
#define SERVO_LEFT_GRIPPER_LIMIT_CURRENT_MA				500		//??	//mA

#define SERVO_RIGHT_GRIPPER_DEG_INCR_X0					90
#define SERVO_RIGHT_GRIPPER_DEG_INCR_Y0					1320	//ok
#define SERVO_RIGHT_GRIPPER_DEG_INCR_X1					0
#define SERVO_RIGHT_GRIPPER_DEG_INCR_Y1					2218	//ok
#define SERVO_RIGHT_GRIPPER_LIMIT_LOW_POS_INCR			1000	//ok	//kinn
#define SERVO_RIGHT_GRIPPER_LIMIT_HIGH_POS_INCR			2218	//ok	//0deg, benn
#define SERVO_RIGHT_GRIPPER_LIMIT_CURRENT_MA			500		//??	//mA

#define SERVO_LEFT_CLAW_DEG_INCR_X0						0
#define SERVO_LEFT_CLAW_DEG_INCR_Y0						905		//ok
#define SERVO_LEFT_CLAW_DEG_INCR_X1						90
#define SERVO_LEFT_CLAW_DEG_INCR_Y1						1845	//ok
#define SERVO_LEFT_CLAW_LIMIT_LOW_POS_INCR				905		//ok	//0deg, benn
#define SERVO_LEFT_CLAW_LIMIT_HIGH_POS_INCR				2200	//ok	//kinn
#define SERVO_LEFT_CLAW_LIMIT_CURRENT_MA				500		//??	//mA

#define SERVO_RIGHT_CLAW_DEG_INCR_X0					0
#define SERVO_RIGHT_CLAW_DEG_INCR_Y0					2218	//ok
#define SERVO_RIGHT_CLAW_DEG_INCR_X1					90
#define SERVO_RIGHT_CLAW_DEG_INCR_Y1					1310	//ok
#define SERVO_RIGHT_CLAW_LIMIT_LOW_POS_INCR				1000	//ok	//kinn
#define SERVO_RIGHT_CLAW_LIMIT_HIGH_POS_INCR			2218	//ok	//0deg, benn
#define SERVO_RIGHT_CLAW_LIMIT_CURRENT_MA				500		//??	//mA

#define SERVO_ARM_DEG_INCR_X0							0
#define SERVO_ARM_DEG_INCR_Y0							1060	//ok
#define SERVO_ARM_DEG_INCR_X1							90
#define SERVO_ARM_DEG_INCR_Y1							1975	//ok
#define SERVO_ARM_LIMIT_LOW_POS_INCR					1020	//ok
#define SERVO_ARM_LIMIT_HIGH_POS_INCR					2060	//ok
#define SERVO_ARM_LIMIT_CURRENT_MA						500		//??	//mA

#define SERVO_PLUS_0_DEG_INCR_X0						0
#define SERVO_PLUS_0_DEG_INCR_Y0						1000
#define SERVO_PLUS_0_DEG_INCR_X1						180
#define SERVO_PLUS_0_DEG_INCR_Y1						2000
#define SERVO_PLUS_0_LIMIT_LOW_POS_INCR					700
#define SERVO_PLUS_0_LIMIT_HIGH_POS_INCR				2200
#define SERVO_PLUS_0_LIMIT_CURRENT_MA					500		//??	//mA






//----- deadreck -----
//az Y falra kell rasimitani
#define DEADRECK_CALIB_DISTANCE_X				((double)153)	//[mm]
#define DEADRECK_CALIB_DISTANCE_Y				((double)159)	//[mm]
#define DEADRECK_CALIB_PHI						((double)49.87 * M_PI / 180)	//[rad]

// originalt megoldasok
//#define DEADRECK_WHEEL_DISTANCE_REC					((double)1 / 261.274198830535)
//#define DEADRECK_LEFT_ONE_INCREMENT_DISTANCE		((double)1 / 67.517166666666)
//#define DEADRECK_RIGHT_ONE_INCREMENT_DISTANCE		((double)1 / 67.517166666666)

// elso kalibracio utan, még lehet, hogy pontosítani kell
//saját menést beleszámítva:
//#define DEADRECK_WHEEL_DISTANCE_REC				((double)1 / 267.294)
//#define DEADRECK_LEFT_ONE_INCREMENT_DISTANCE		((double)1 / 66.78756)
//#define DEADRECK_RIGHT_ONE_INCREMENT_DISTANCE		((double)1 / 67.41111)
//saját menésnélkül
#define DEADRECK_WHEEL_DISTANCE_REC					((double)1 / 266.4655)
#define DEADRECK_LEFT_ONE_INCREMENT_DISTANCE		((double)1 / 67.3033)
#define DEADRECK_RIGHT_ONE_INCREMENT_DISTANCE		((double)1 / 67.310)
#define DEADRECK_LEFT_IS_ROTATE_DIR_A				0
#define DEADRECK_RIGHT_IS_ROTATE_DIR_A				1
#define DEADRECK_IS_LEFT_EQEP1						1





//----- power -----
#define POWER_MAIN_VOLTAGE_SHUTDOWN_LEVEL							18
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
#define BDC_OE_CONTROL_P							0.0007
#define BDC_OE_CONTROL_I							0.0





//----- console -----
#define CONSOLE_INCR_MM_X0					0
#define CONSOLE_INCR_MM_Y0					0
#define CONSOLE_INCR_MM_X1					12967	//12967 incr = 100 mm
#define CONSOLE_INCR_MM_Y1					100
#define CONSOLE_INCR_MM_GRAD				GET_GRAD(CONSOLE_INCR_MM_X0, CONSOLE_INCR_MM_Y0, CONSOLE_INCR_MM_X1, CONSOLE_INCR_MM_Y1)

#define CONSOLE_CONV_INCR_TO_MM(incr)				(((double)(incr) - CONSOLE_INCR_MM_X0) * CONSOLE_INCR_MM_GRAD + CONSOLE_INCR_MM_Y0)

#define CONSOLE_CONV_MM_TO_INCR(mm)					((u32)(((double)(mm) - CONSOLE_INCR_MM_Y0) / CONSOLE_INCR_MM_GRAD + CONSOLE_INCR_MM_X0))
#define CONSOLE_CONV_MMS_TO_INCRS(mms)				((u32)((double)(mms) / CONSOLE_INCR_MM_GRAD))
#define CONSOLE_CONV_MMS2_TO_INCRS2(mms2)			((u32)((double)(mms2) / CONSOLE_INCR_MM_GRAD))


#define CONSOLE_CONTOLLER_LIMIT_LOW_POS_INCR		220
#define CONSOLE_CONTOLLER_LIMIT_HIGH_POS_INCR		20600
#define CONSOLE_HARDWARE_LIMIT_LOW_POS_INCR			123		//1mm
#define CONSOLE_HARDWARE_LIMIT_HIGH_POS_INCR		20600	//120mm
#define CONSOLE_CONTROLLER_PARAMETER_P				0.2
#define CONSOLE_CONTROLLER_PARAMETER_I				0.0
#define CONSOLE_CONTROLLER_PARAMETER_D				0.0
#define CONSOLE_PWM_MODE							3
#define CONSOLE_LIMIT_SWITCH_IS_ACTIVE_HIGH			0
#define CONSOLE_IS_ENC_EQEP1						1
#define CONSOLE_MOTOR_PLUS_UP						1
#define CONSOLE_ENC_IS_ROTATE_DIR_A					1





//----- sonar -----
#define SONAR_TX_RED		2098.0
#define SONAR_TY_RED		3098.0
#define SONAR_ALPHA_RED		M_PI

#define SONAR_TX_PURPLE		-98.0
#define SONAR_TY_PURPLE		-98.0
#define SONAR_ALPHA_PURPLE	0.0





#endif	//PARAM_H_
