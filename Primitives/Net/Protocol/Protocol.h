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
#define MSG_GO 2
#define MSG_GOTO 3
#define MSG_TURN 4
#define MSG_MOTIONSTOP 5

typedef struct msgstatus {
	int function;
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
	int function;
	bool b1;
} msgb1;

typedef struct msgd1 {
	int function;
	double d1;
} msgd1;

typedef struct msgd2 {
	int function;
	double d1;
	double d2;
} msgd2;

typedef struct msgd3 {
	int function;
	double d1;
	double d2;
	double d3;
} msgd3;

typedef struct msgd4 {
	int function;
	double d1;
	double d2;
	double d3;
	double d4;
} msgd4;

#endif /* PROTOCOL_H_ */
