#include "FollowLine.h"

using namespace std;

FollowLine::FollowLine(Primitives* pr)
{
	primi = pr;
	FollowLine_CurState = FOLLOW_STATE_START;
	Cur_Pos = position();
	Prev_Pos = position();
	Follow_Status = FOLLOW_STATE_OTHER_STOP;
	Calibrate_InProgress = false;
	distance = 0;
	intersection_count = 0;
	noline_count = 0;
	turn_back = false;
	Follow_Distance = 0;
	mask = 0;
	P = 0;
	D = 0;
	I = 0;
	Motor_Control = 0;
	Error_Sens_Pos = 0;
	Prev_Error_Sens_Pos = 0;
	SerialPort = 0;
};

FollowLine::~FollowLine()
{
	delete primi;
}

int32_t FollowLine::Open_SerialPort(const char* portname, int32_t speed)
{
    struct termios tty;

	SerialPort = open (portname, O_RDWR | O_NOCTTY | O_SYNC);
	if (SerialPort < 0)
		return -1;

    memset (&tty, 0, sizeof tty);
    if (tcgetattr (SerialPort, &tty) != 0)
        return -1;

    cfsetospeed (&tty, speed);
    cfsetispeed (&tty, speed);

    // disable IGNBRK for mismatched speed tests; otherwise receive break
    // as \000 chars
    tty.c_iflag &= ~IGNBRK;         // ignore break signal
    tty.c_lflag = 0;                // no signaling chars, no echo,
                                    // no canonical processing
    tty.c_oflag = 0;                // no remapping, no delays
    tty.c_cc[VMIN]  = 0;            // read doesn't block
    tty.c_cc[VTIME] = 5;            // 0.5 seconds read timeout for at least one byte
    tty.c_iflag &= ~(IXON | IXOFF | IXANY); // shut off xon/xoff ctrl

    tty.c_cflag |= (CLOCAL | CREAD);// ignore modem controls, enable reading
    //8N1 flags
    tty.c_cflag &= ~PARENB;
    tty.c_cflag &= ~CSTOPB;
    tty.c_cflag &= ~CRTSCTS;
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8;

    if (tcsetattr (SerialPort, TCSANOW, &tty) != 0)
    	return -1;

    return 0;
}
void FollowLine::Close_SerialPort()
{
	close(SerialPort);
}

int32_t FollowLine::SendByte_SerialPort(char data)
{
	int32_t ret = write(SerialPort, &data, 1);
    return ret;
}

int32_t FollowLine::ReceiveByte_SerialPort(char* data)
{
	int32_t n = read(SerialPort, data, sizeof(data));
	return n;
}

void FollowLine::Follow_Init(double dist)
{
	Follow_Distance = dist;
	FollowLine_CurState = FOLLOW_STATE_START;
}

bool FollowLine::Calibrate(void)
{
	uint32_t temp;
	uint32_t Sens_Pos;
	uint32_t Sens_Temp = 0;
	uint16_t threshold = 50;
	bool success = true;
	Calibrate_InProgress = true;
	mask = 0;
	if (Open_SerialPort("/dev/ttyO1",B115200) != 0)
		success =  false;
	for (int32_t cycle = 0; threshold < 5000; cycle++)
	{
		if (!ControlSensor(MAX_UART_TRY,&temp,PARANCS_GLOBAL_TH_ID,threshold & 0x00ff,(threshold & 0xff00) >> 8 ))
			success = false;
		if (!ControlSensor(MAX_UART_TRY,&Sens_Pos,PARANCS_LEKERDEZES_ID, 0, 0))
			success = false;
		if (threshold < 1000)
		{
			for (int32_t i = 0; i< 32; i++)
			{
				Sens_Temp = (Sens_Pos >> i) & 0x1;
				if (Sens_Temp == 0)
					mask |= (int32_t)exp2(i);
			}
		}
		else if (threshold > 4500)
		{
			for (int32_t i = 0; i< 32; i++)
			{
				Sens_Temp = (Sens_Pos >> i) & 0x1;
				if (Sens_Temp == 1)
					mask |= (int32_t)exp2(i);
			}
		}
		threshold += 200;
	}
	Close_SerialPort();
	Calibrate_InProgress = false;
	return success;
}

bool FollowLine::FixThreshold(uint16_t threshold)
{
	uint32_t temp;
	bool success = true;
	if (Open_SerialPort("/dev/ttyO1",B115200) != 0)
		success =  false;
	if (!ControlSensor(MAX_UART_TRY,&temp,PARANCS_GLOBAL_TH_ID,threshold & 0x00ff,(threshold & 0xff00) >> 8 ))
		success = false;
	Close_SerialPort();
	return success;
}

void FollowLine::MakeLineList(uint32_t Sens_Pos)
{
	uint32_t Sens_Temp;
	bool in_line = false;
	Sens_Line temp_line;
	IntersectionList.clear();
	for (int32_t i = 0; i < 32; i++)
	{
		Sens_Temp = (Sens_Pos >> i) & 0x1;
		if (Sens_Temp == 0 && !in_line)
		{
			//Kezdodik egy vonal
			temp_line.from = i;
			in_line = true;
			//From a mostani
		}
		else if (Sens_Temp == 1 && in_line)
		{
			//Vege egy vonalnak
			in_line = false;
			temp_line.to = i-1;
			temp_line.avg = (temp_line.from + temp_line.to)/2;
			IntersectionList.push_back(temp_line);
			//To az elozo
		}
	}
	if (in_line)
	{
		//Ha nem ert veget a vonal, zarjuk le a vegen
		temp_line.to = 31;
		temp_line.avg = (temp_line.from + temp_line.to)/2;
		IntersectionList.push_back(temp_line);
		in_line = false;
		//To az utolso
	}
}
void FollowLine::MaskRawSensData(uint32_t* Sens_Pos)
{
	for (int32_t i = 0; i < 32; i++)
	{
		if (i == 0 && (mask & 0x1))
		{
			//Ha hibas az elso szenzor, a 2. szenzor erteket kapja meg
			if (((*Sens_Pos) >> (1)) & 0x1)
				(*Sens_Pos) |= 0x00000001;
			else
				(*Sens_Pos) &= ~0x00000001;

		}
		else if (i == 31 && ((mask >> 31) & 0x1))
		{
			//Ha hibas az utolso szenzor, az utolso elotti erteket kapja meg
			if (((*Sens_Pos) >> (31)) & 0x1)
				(*Sens_Pos) |= (0x00000001 << 31);
			else
				(*Sens_Pos) &= ~(0x00000001 << 31);

		}
		else if ((mask >> i) & 0x1)
		{
			//Ha hibas az i-edik szenzor, akkor csak akkor kapjon a hibas szenzor 1 erteket, ha mellette ket 1-es van
			if (((*Sens_Pos) >> (i-1)) & 0x1)
			{
				if (((*Sens_Pos) >> (i+1)) & 0x1)
					(*Sens_Pos) |= (0x00000001 << i);
				else
					(*Sens_Pos) &= ~(0x00000001 << i);
			}
			else
				(*Sens_Pos) &= ~(0x00000001 << i);
		}
	}
}

bool FollowLine::ControlSensor(uint8_t max_try, uint32_t* Sens_Pos, char command, char dataL, char dataH)
{
	uint32_t Sens_Pos_Temp;
	int32_t uart_try, num, ret;
	char read[6];
	struct pollfd fd[1];
	fd->fd = SerialPort;
	fd->events = POLLRDNORM | POLLIN;
	*Sens_Pos = 0;
	Sens_Pos_Temp = 0;
	uart_try = 0;
	while (uart_try < max_try)
	{
		uart_try++;
		SendByte_SerialPort(DEV_ID);
		SendByte_SerialPort(command);
		SendByte_SerialPort(dataL);
		SendByte_SerialPort(dataH);
		SendByte_SerialPort(0);
		ret = poll(fd,1,500);	//500ms wait
		if (ret > 0)
			num = ReceiveByte_SerialPort(read);
		if (num < 6)
		{
			ret = poll(fd,1,500);	//500ms wait
			if (ret > 0)
				num += ReceiveByte_SerialPort(&read[num]);
		}
		if (num ==6 && read[0] == DEV_ID  && read[1] == command)
		{
			Sens_Pos_Temp = ((uint32_t)read[5]) << 24;
			*Sens_Pos |= Sens_Pos_Temp;
			Sens_Pos_Temp = ((uint32_t)read[4]) << 16;
			*Sens_Pos |= Sens_Pos_Temp;
			Sens_Pos_Temp = ((uint32_t)read[3]) << 8;
			*Sens_Pos |= Sens_Pos_Temp;
			*Sens_Pos |= (uint32_t)read[2];
			return true;
		}
	}
	return false;
}
void FollowLine::FSM_Run()
{
	if (Open_SerialPort("/dev/ttyO1",B115200) != 0)
	{
		primi->SetWheelSpeed(0,0);
		//ReceiveByte_SerialPort-ban levo timeout miatt itt kevesebb byte-ot nem kaphatunk, csak 0-t, v 6-ot
		Follow_Status = FOLLOW_STATE_SERIALCONNECT_STOP;
		FollowLine_CurState = FOLLOW_STATE_STOP;
		return;
	}
	switch (FollowLine_CurState)
	{
		case FOLLOW_STATE_START:
			primi->GetRobotPos(&Prev_Pos.x,&Prev_Pos.y,&Prev_Pos.phi);
			Cur_Pos = Prev_Pos;
			distance = 0;
			Follow_Status = FOLLOW_STATE_FOLLOW_RUNNING;
			intersection_count = 0;
			noline_count = 0;
			turn_back = false;
			Prev_Line.avg = 16;
			Prev_Line.from = 15;
			Prev_Line.to = 17;
			P = 0;
			D = 0;
			I = 0;
			FollowLine_CurState = FOLLOW_STATE_CHECK;
			break;
		case FOLLOW_STATE_CHECK:
			uint32_t Sens_Pos;
			Sens_Line selected;
			//Alapbol a kovetkezo allapot a PID es nincs hiba
			FollowLine_CurState = FOLLOW_STATE_PID;
			//Tavolsag vizsgalat***************************
			Prev_Pos = Cur_Pos;
			primi->GetRobotPos(&Cur_Pos.x,&Cur_Pos.y,&Cur_Pos.phi);
			distance += sqrt(pow((Cur_Pos.x-Prev_Pos.x),2) + pow((Cur_Pos.y-Prev_Pos.y),2));
			distance++;
			if (distance >= Follow_Distance)
			{
				//Elertuk a kivant tavolsagot
				primi->SetWheelSpeed(0,0);
				Follow_Status = FOLLOW_STATE_DISTANCE_STOP;
				FollowLine_CurState = FOLLOW_STATE_STOP;
				break;
			}
			//Szenzorok beolvasasa UART-on*****************
			Prev_Error_Sens_Pos = Error_Sens_Pos;
			if (!ControlSensor(MAX_UART_TRY,&Sens_Pos,PARANCS_LEKERDEZES_ID, 0, 0))
			{
				primi->SetWheelSpeed(0,0);
				Follow_Status = FOLLOW_STATE_SERIALDATA_STOP;
				FollowLine_CurState = FOLLOW_STATE_STOP;
				break;
			}
			//Nyers szenzor adatok -> vonal lista**********
			MaskRawSensData(&Sens_Pos);
			MakeLineList(Sens_Pos);
			//Kiveszi a keskeny es nagyon szeles vonalakat, 3 bitnel keskenyebbeket, 5 bitnel szelesebbeket
			if (IntersectionList.size() >= 1)
			{
				Sens_Line_List::iterator act = IntersectionList.begin();
				while (act != IntersectionList.end())
				{
					if (((act->to - act->from) < 2) || ((act->to - act->from) > 4))
						act = IntersectionList.erase(act);
					else
						act++;
				}
			}
			//Ha nem maradt elem, akkor nincs vonal
			if (IntersectionList.size() == 0)
			{
				IntersectionList.push_back(Prev_Line);
				noline_count++;
			}
			else
				noline_count = 0;
			if (noline_count == FOLLOW_NOLINE_LIMIT)
			{
				noline_count = 0;
				primi->SetWheelSpeed(0,0);
				Follow_Status = FOLLOW_STATE_NOLINE_STOP;
				FollowLine_CurState = FOLLOW_STATE_STOP;
				break;
			}
			//Ha ezutan is tobb elem van a listaban akkor elagazas gyanu++
			if (IntersectionList.size() > 1)
				intersection_count++;
			else
				intersection_count = 0;
			//Ha FOLLOW_INTERSECTION_LIMIT-szer volt elagazas, akkor elagazas miatt kilep az FSM
			if (intersection_count == FOLLOW_INTERSECTION_LIMIT )
			{
				intersection_count = 0;
				//TODO:csak akkor alljon meg, ha pozicio alapjan is elagazas van
				primi->SetWheelSpeed(0,0);
				Follow_Status = FOLLOW_STATE_INTERSECTION_STOP;
				FollowLine_CurState = FOLLOW_STATE_STOP;
				break;
			}
			//Ha tobb elem van a listaban, akkor az elozo mintavetelkor kovetet elemhez legkozelebbit valasztjuk
			if (IntersectionList.size() > 1)
			{
				uint16_t min;
				min = abs(Prev_Line.avg - IntersectionList.begin()->avg);
				for (Sens_Line_List::iterator act = ++IntersectionList.begin(); act != IntersectionList.end(); ++act)
				{
					if (min > abs(Prev_Line.avg - act->avg))
						selected = (Sens_Line)(*act);
				}
			}
			else
				selected = (Sens_Line)(*IntersectionList.begin());
			//Hibajel:Error_Sens_Pos, 16 jelenti a kozepet
			Error_Sens_Pos = 16 - selected.avg;
			Prev_Line = selected;
			break;
		case FOLLOW_STATE_PID:
			P = Error_Sens_Pos;
			D = (Error_Sens_Pos-Prev_Error_Sens_Pos);
			I += Error_Sens_Pos;
			Motor_Control = (int32_t)((double)P*FOLLOW_PID_P + (double)D*FOLLOW_PID_D + (double)I*FOLLOW_PID_I);
			if (Motor_Control > FOLLOW_MAX_SPEED)
				Motor_Control = FOLLOW_MAX_SPEED;
			if (Motor_Control < -FOLLOW_MAX_SPEED)
				Motor_Control = -FOLLOW_MAX_SPEED;
			FollowLine_CurState = FOLLOW_STATE_MOTOR;
			break;
		case FOLLOW_STATE_MOTOR:
			//TODO:Hol lesz a szenzor, bal-jobb rendben van-e igy a motorok kezelese, ellenorizni
			if (Motor_Control > 0)
			{
				//cout << "Bal Motor: " << FOLLOW_MAX_SPEED << endl;
				//cout << "Jobb Motor: " << (FOLLOW_MAX_SPEED-(double)Motor_Control) << endl;
				primi->SetWheelSpeed(FOLLOW_MAX_SPEED, FOLLOW_MAX_SPEED-(double)Motor_Control);
			}
			else
			{
				//cout << "Bal Motor: " << (FOLLOW_MAX_SPEED+(double)Motor_Control) << endl;
				//cout << "Jobb Motor: " << FOLLOW_MAX_SPEED << endl;
				primi->SetWheelSpeed(FOLLOW_MAX_SPEED+Motor_Control, FOLLOW_MAX_SPEED);
			}
			FollowLine_CurState = FOLLOW_STATE_CHECK;
			break;
		case FOLLOW_STATE_STOP:
			break;
	}
	Close_SerialPort();
}
