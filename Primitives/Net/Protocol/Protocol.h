/*
 * Protocol.h
 *
 *  Created on: Mar 31, 2011
 *      Author: agoston
 */

#ifndef PROTOCOL_H_
#define PROTOCOL_H_

#include <stdint.h>

#include "../../../misc.h"

#define MSG_UNKNOWN 0
#define MSG_REFRESHSTATUS 1
#define MSG_CALIBRATEPOS 2
#define MSG_SETSPEED 3
#define MSG_GO 4
#define MSG_GOTO 5
#define MSG_TURN 6
#define MSG_MOTIONSTOP 7
//#define MSG_GRIPPERMOVE 8
#define MSG_CALIBRATECONSOLE 9
#define MSG_CONSOLEMOVE 10
#define MSG_CONSOLESTOP 11
//#define MSG_ARMMOVE 12
//#define MSG_MAGNET 13
//#define MSG_PAWNS 14
//#define MSG_DEPLOYPRIORITY 15
#define MSG_VISIONTEST 16
//#define MSG_POSREFINE 17
#define MSG_PRINT 18
#define MSG_OBSTACLES 19
//#define MSG_PAWNS_AT_START 20
#define MSG_SERVOMOVE 21
//#define MSG_COMPRESSOR 22
//#define MSG_VALVE 23
#define MSG_SHAPES 24

typedef uint8_t msglen_t;
typedef uint8_t function_t;

#pragma pack(1)
typedef struct msgstatus {
	function_t function;
	double x;
	double y;
	double phi;
	double v;
	double w;
	double ox[OPPONENT_NUM];
	double oy[OPPONENT_NUM];
	double oradius[OPPONENT_NUM];
	bool startButton;
	bool stopButton;
	int8_t color;
	bool motorSupply;
#ifdef NAGY_ROBOT
	double frontArmPos;
	double rearArmPos;
	double selectorPos;
	double flipperPos;
	double contractorPos;
	double firestopperPos;
	double firewheelSpeed;
	double caracoleSpeed;
#else	//KIS_ROBOT
	double lowLeftGripperPos;
	double lowRightGripperPos;
	double highLeftGripperPos;
	double highRightGripperPos;
#endif
	double consolePos;
	double distances[PROXIMITY_NUM];
} msgstatus;

typedef struct msgb1 {
	function_t function;
	bool b1;
} msgb1;

typedef struct msgd1 {
	function_t function;
	double d1;
} msgd1;

typedef struct msgd2 {
	function_t function;
	double d1;
	double d2;
} msgd2;

typedef struct msgd3 {
	function_t function;
	double d1;
	double d2;
	double d3;
} msgd3;

typedef struct msgd4 {
	function_t function;
	double d1;
	double d2;
	double d3;
	double d4;
} msgd4;

typedef struct msgservo {
	function_t function;
	uint8_t id;
	double pos;
	double speed;
	double acc;
} msgarm;

typedef struct msgprint {
	function_t function;
	char text[254];
} msgprint;

typedef struct msgobstacle {
	uint8_t type;
	float d[4];
} msgobstacle;

typedef struct msgobstacles {
	function_t function;
	uint8_t num;
	msgobstacle obstacles[14];
} msgobstacles;

typedef struct msgshapes {
	function_t function;
	uint8_t num;
	uint8_t id;
	uint8_t color[4];
	msgobstacle obstacles[14];
} msgshapes;

#pragma pack()

#endif /* PROTOCOL_H_ */
