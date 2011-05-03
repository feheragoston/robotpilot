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

typedef struct msgstatus {
	uint32_t function;
	double x;
	double y;
	double phi;
	double ox;
	double oy;
	bool startButton;
	bool stopButton;
	bool color;
} msgstatus;

typedef struct msgb1 {
	uint32_t function;
	bool b1;
} msgb1;

typedef struct msgd1 {
	uint32_t function;
	double d1;
} msgd1;

typedef struct msgd2 {
	uint32_t function;
	double d1;
	double d2;
} msgd2;

typedef struct msgd3 {
	uint32_t function;
	double d1;
	double d2;
	double d3;
} msgd3;

typedef struct msgd4 {
	uint32_t function;
	double d1;
	double d2;
	double d3;
	double d4;
} msgd4;

#endif /* PROTOCOL_H_ */
