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

	bool Turn(bool turn);

	//TODO: ezt megirni
	bool Calibrate();

	void FSM_Run();
	void Follow_Init(double dist);

	bool Turn_InProgress;
	bool Calibrate_InProgress;

	int Follow_Status;
private:

	//Primitives osztaly, amivel egyuttmukodik
	Primitives* primi;

	position Cur_Pos, Prev_Pos;
	int P,D,I;
	int Motor_Control;
	unsigned int distance;
	bool turn_back;
	int Lin_Sens_Pos, Prev_Lin_Sens_Pos;
	unsigned int intersection_count;
	double Follow_Distance;
	short FollowLine_CurState;

	typedef struct Sens_Line
	{
		unsigned short from;
		unsigned short to;
		unsigned short avg;
	}Sens_Line;

	typedef std::list<Sens_Line> Sens_Line_List;

	//Ez a lista tartalmazza az eszlelt vonalakat
	Sens_Line_List IntersectionList;


	//File Descriptor a Soros Porthoz
	int SerialPort;

	//Serial Port Inicializalasa -> SerialPort
	//return: -1: hiba, 0: OK
	//speed: B115200,B57600,... -> termios.h
	//8bit, No parity, 1 Stop bit (8N1)
	int Open_SerialPort(char* portname, int speed);

	//Serial Port Lezarasa <- SerialPort
	//return: -1: hiba, 0: OK
	void Close_SerialPort();

	//1 Byte kuldese Serial Port-on
	//return: hany byte-ot kuldott
	int SendByte_SerialPort(char data);

	//Byte-ok fogadasa Serial Port-on
	//return: hany byte-ot fogadott
	int ReceiveByte_SerialPort(char* data);
};
#endif
