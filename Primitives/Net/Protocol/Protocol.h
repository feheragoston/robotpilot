/*
 * Protocol.h
 *
 *  Created on: Mar 31, 2011
 *      Author: agoston
 */

#ifndef PROTOCOL_H_
#define PROTOCOL_H_

#define MSG_UNKNOWN 0
#define MSG_REFRESHSTATUS 1
#define MSG_CALIBRATEPOS 2
#define MSG_SETSPEED 3
#define MSG_GO 4
#define MSG_GOTO 5
#define MSG_TURN 6
#define MSG_MOTIONSTOP 7
#define MSG_GRIPPERMOVE 8
#define MSG_CALIBRATECONSOLE 9
#define MSG_CONSOLEMOVE 10
#define MSG_CONSOLESTOP 11
#define MSG_ARMMOVE 12
#define MSG_MAGNET 13
#define MSG_PAWNS 14

#define FIG_NOTHING 0
#define FIG_PAWN 1
#define FIG_PPAWN 2
#define FIG_KING 4
#define FIG_PKING 5
#define FIG_PPKING 6

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
	double ox;
	double oy;
	bool startButton;
	bool stopButton;
	bool color;
} msgstatus;

typedef struct msgpawn {
	float x;
	float y;
	uint8_t type;
} msgpawn;

typedef struct msgpawns {
	function_t function;
	float x[4];
	float y[4];
	uint8_t num;
	msgpawn pawns[19];
} msgpawns;

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

typedef struct msgarm {
	function_t function;
	bool left;
	double pos;
	double speed;
	double acc;
} msgarm;

typedef struct msgmagnet {
	function_t function;
	bool left;
	int8_t polarity;
} msgmagnet;

#pragma pack()

#endif /* PROTOCOL_H_ */
