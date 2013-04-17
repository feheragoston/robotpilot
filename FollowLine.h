#ifndef LINEFOLLOW_H_
#define LINEFOLLOW_H_

#include "FollowLineDefines.h"
#include "stdlib.h"
#include "math.h"
#include "Primitives/Primitives.h"
#include <list>
#include <fcntl.h>
#include <termios.h>


class FollowLine {
public:
	FollowLine(Primitives* pr);

	~FollowLine();

	bool Turn(void);

	bool Calibrate();

	//TODO:GoAhead a Turn-be keruljon
	bool GoAhead(void);

	void FSM_Run(double dist);

	bool Turn_InProgress;
	bool Calibrate_InProgress;

	int Follow_Status;
private:

	Primitives* primi;

	short FollowLine_CurState;
	position Cur_Pos, Prev_Pos;
	int P,D,I;
	int Motor_Control;
	unsigned int distance;
	bool turn_back;
	int Lin_Sens_Pos, Prev_Lin_Sens_Pos;
	unsigned int intersection_count;
	int SerialPort;
	int uart_try;

	typedef struct Sens_Line
	{
		unsigned short from;
		unsigned short to;
		unsigned short avg;
	}Sens_Line;

	typedef std::list<Sens_Line> Sens_Line_List;

	Sens_Line_List IntersectionList;


	int Open_SerialPort(char* portname, int speed, int parity);
	void Close_SerialPort();
	int SendByte_SerialPort(char data);
	int ReceiveByte_SerialPort(char* data);

};
#endif
