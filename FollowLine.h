#ifndef LINEFOLLOW_H_
#define LINEFOLLOW_H_

#include "FollowLineDefines.h"
#include "stdlib.h"
#include "math.h"
#include "Primitives/Primitives.h"
#include <list>
#include <fcntl.h>
#include <termios.h>
#include <fstream>
#include <iostream>
#include <errno.h>
#include "stdio.h"
#include "string.h"
#include "poll.h"

class FollowLine {
public:
	FollowLine(Primitives* pr);
	~FollowLine();

	bool Calibrate();

	bool FixThreshold(uint16_t threshold);

	void FSM_Run();
	void Follow_Init(double dist);

	bool Calibrate_InProgress;

	int Follow_Status;
private:

	//Primitives osztaly, amivel egyuttmukodik
	Primitives* primi;

	void MaskRawSensData(uint32_t* Sens_Pos);
	bool ControlSensor(uint8_t max_try, uint32_t* Sens_Pos,char command, char dataL, char dataH);
	void MakeLineList(uint32_t Sens_Pos);

	typedef struct Sens_Line
	{
		uint16_t from;
		uint16_t to;
		uint16_t avg;
	}Sens_Line;

	position Cur_Pos, Prev_Pos;
	int32_t P,D,I;
	int32_t Motor_Control;
	uint32_t distance;
	bool turn_back;
	uint32_t Error_Sens_Pos, Prev_Error_Sens_Pos;
	uint8_t intersection_count, noline_count;
	double Follow_Distance;
	int16_t FollowLine_CurState;
	uint32_t mask;
	Sens_Line Prev_Line;


	typedef std::list<Sens_Line> Sens_Line_List;

	//Ez a lista tartalmazza az eszlelt vonalakat
	Sens_Line_List IntersectionList;


	//File Descriptor a Soros Porthoz
	int32_t SerialPort;

	//Serial Port Inicializalasa -> SerialPort
	//return: -1: hiba, 0: OK
	//speed: B115200,B57600,... -> termios.h
	//8bit, No parity, 1 Stop bit (8N1)
	int32_t Open_SerialPort(const char* portname, int32_t speed);

	//Serial Port Lezarasa <- SerialPort
	//return: -1: hiba, 0: OK
	void Close_SerialPort();

	//1 Byte kuldese Serial Port-on
	//return: hany byte-ot kuldott
	int32_t SendByte_SerialPort(char data);

	//Byte-ok fogadasa Serial Port-on
	//return: hany byte-ot fogadott
	int32_t ReceiveByte_SerialPort(char* data);
};
#endif
