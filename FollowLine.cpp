#include "FollowLine.h"

//bool FollowLine::FollowLine(double dist)
//{
//	FollowLine_CurState = FOLLOW_STATE_START;
//	if (Open_SerialPort("/dev/ttys0",B115200,0) != 0)
//		return false;
//	while (FollowLine_CurState != FOLLOW_STATE_STOP)
//	{
//		FollowLine_FSMRun(dist);
//		//Wait x us
//	}
//	Close_SerialPort();
//	return true;
//}

FollowLine::FollowLine(Primitives* pr)
{
	primi = pr;
	FollowLine_CurState = FOLLOW_STATE_START;
	Cur_Pos = position();
	Prev_Pos = position();
	Follow_Status = FOLLOW_STATE_OTHER_STOP;
	uart_try = 0;
	Calibrate_InProgress = false;
	Turn_InProgress = false;
	distance = 0;
	intersection_count = 0;
	turn_back = false;
	P = 0;
	D = 0;
	I = 0;
	Motor_Control = 0;
	Lin_Sens_Pos = 0;
	Prev_Lin_Sens_Pos = 0;
	SerialPort = 0;
};

FollowLine::~FollowLine()
{
	delete primi;
}

int FollowLine::Open_SerialPort(char* portname, int speed, int parity)
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

    tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;     // 8-bit chars
    // disable IGNBRK for mismatched speed tests; otherwise receive break
    // as \000 chars
    tty.c_iflag &= ~IGNBRK;         // ignore break signal
    tty.c_lflag = 0;                // no signaling chars, no echo,
                                    // no canonical processing
    tty.c_oflag = 0;                // no remapping, no delays
    tty.c_cc[VMIN]  = 0;            // read doesn't block
    tty.c_cc[VTIME] = 5;            // 0.5 seconds read timeout

    tty.c_iflag &= ~(IXON | IXOFF | IXANY); // shut off xon/xoff ctrl

    tty.c_cflag |= (CLOCAL | CREAD);// ignore modem controls,
                                    // enable reading
    tty.c_cflag &= ~(PARENB | PARODD);      // shut off parity
    tty.c_cflag |= parity;
    tty.c_cflag &= ~CSTOPB;
    tty.c_cflag &= ~CRTSCTS;

    if (tcsetattr (SerialPort, TCSANOW, &tty) != 0)
    	return -1;

    return 0;
}
void FollowLine::Close_SerialPort()
{
	close(SerialPort);
}
int FollowLine::SendByte_SerialPort(char data)
{
	int ret;
    ret = write(SerialPort, &data, 1);
    return ret;
}
int FollowLine::ReceiveByte_SerialPort(char* data)
{
	int n = read (SerialPort, data, sizeof data);
	return n;

}

bool FollowLine::Calibrate(void)
{
	return false;
}

bool FollowLine::GoAhead(void)
{
	//Csak 2 elem lehet az elágazás listában
	if (IntersectionList.size() != 2)
		return false;
	//Egy kicsit elore megyunk, h mar ne legyen elagazas
	primi->Go(FOLLOW_LINE_GO_DIST,FOLLOW_LINE_GO_SPEED, FOLLOW_LINE_GO_ACCEL);
	while (primi->MotionInProgress())
		primi->Wait(1);
	return true;
}

bool FollowLine::Turn(void)
{
	Sens_Line_List::iterator center, side;
	double angle;
	//Csak 2 elem lehet az elágazás listában
	if (IntersectionList.size() != 2)
		return false;
	Turn_InProgress = true;
	center = IntersectionList.begin();
	side = IntersectionList.end();
	//Kozeptol valo elteres, 0-> bal oldal, 280-> jobb oldal
	if (abs(center->avg - 140) > abs(side->avg - 140))
	{
		center = IntersectionList.end();
		side = IntersectionList.begin();
	}
	angle = (FOLLOW_LINE_TURN_CONST * double(abs(side->avg - 140)) / 140.0) * (M_PI/2);
	//+90° ha idoben csokken a ket elagazas tavolsaga
	if (turn_back == true)
		angle += M_PI/2;
	primi->Turn(angle,FOLLOW_LINE_TURN_SPEED,FOLLOW_LINE_TURN_ACCEL);
	while (primi->MotionInProgress())
		primi->Wait(1);
	//Egy kicsit elore megyunk, h mar ne legyen elagazas
	primi->Go(FOLLOW_LINE_GO_DIST,FOLLOW_LINE_GO_SPEED, FOLLOW_LINE_GO_ACCEL);
	while (primi->MotionInProgress())
		primi->Wait(1);
	Turn_InProgress = false;
	return true;
}

void FollowLine::FSM_Run(double dist)
{
	switch (FollowLine_CurState)
	{
		case FOLLOW_STATE_START:
			primi->GetRobotPos(&Prev_Pos.x,&Prev_Pos.y,&Prev_Pos.phi);
			Cur_Pos = Prev_Pos;
			distance = 0;
			Follow_Status = FOLLOW_STATE_FOLLOW_RUNNING;
			intersection_count = 0;
			turn_back = false;
			P = 0;
			D = 0;
			I = 0;
			FollowLine_CurState = FOLLOW_STATE_CHECK;
			break;
		case FOLLOW_STATE_CHECK:
			uart_try = 0;
			char read[6];
			int num;
			unsigned int Sens_Pos, Sens_Pos_Temp;
			//Alapbol a kovetkezo allapot a PID es nincs hiba
			FollowLine_CurState = FOLLOW_STATE_PID;
			//Tavolsag vizsgalat***************************
			Prev_Pos = Cur_Pos;
			primi->GetRobotPos(&Cur_Pos.x,&Cur_Pos.y,&Cur_Pos.phi);
			distance += sqrt(pow((Cur_Pos.x-Prev_Pos.x),2) + pow((Cur_Pos.y-Prev_Pos.y),2));
			if (distance >= dist)
			{
				//Elertuk a kivant tavolsagot
				primi->SetWheelSpeed(0,0);
				Follow_Status = FOLLOW_STATE_DISTANCE_STOP;
				FollowLine_CurState = FOLLOW_STATE_STOP;
			}
			//Szenzorok beolvasasa UART-on*****************
			Prev_Lin_Sens_Pos = Lin_Sens_Pos;
			while (uart_try < MAX_UART_TRY)
			{
				uart_try++;
				SendByte_SerialPort(DEV_ID);
				SendByte_SerialPort(PARANCS_LEKERDEZES_ID);
				num = ReceiveByte_SerialPort(read);
				if (num < 6 || read[0] != DEV_ID  || read[1] != PARANCS_LEKERDEZES_ID)
				{
					primi->SetWheelSpeed(0,0);
					//ReceiveByte_SerialPort-ban levo timeout miatt itt kevesebb byte-ot nem kaphatunk, csak 0-t, v 6-ot
					Follow_Status = FOLLOW_STATE_OTHER_STOP;
					FollowLine_CurState = FOLLOW_STATE_STOP;
				}
				else
				{
					Sens_Pos_Temp = ((unsigned int)read[5]) << 24;
					Sens_Pos |= Sens_Pos_Temp;
					Sens_Pos_Temp = ((unsigned int)read[4]) << 16;
					Sens_Pos |= Sens_Pos_Temp;
					Sens_Pos_Temp = ((unsigned int)read[3]) << 8;
					Sens_Pos |= Sens_Pos_Temp;
					Sens_Pos |= (unsigned int)read[2];
					break;
				}
			}
			if (Follow_Status == FOLLOW_STATE_FOLLOW_RUNNING)
			{
				//TODO:Hibas elemek kiszurese
				//Elagazas vizsgalat***************************
				unsigned int Sens_Temp;
				bool in_line = false;
				Sens_Line temp_line;
				for (int i = 0; i < 32; i++)
				{
					Sens_Temp = Sens_Pos >> i;
					Sens_Temp = Sens_Temp & 0x1;
					if (Sens_Temp == 1 && !in_line)
					{
						temp_line.from = i;
						in_line = true;
						//From a mostani
					}
					else if (Sens_Temp == 0 && in_line)
					{
						in_line = false;
						temp_line.to = i-1;
						temp_line.avg = (temp_line.from + temp_line.to)/2;
						IntersectionList.insert(IntersectionList.end(),temp_line);
						//To az elozo
					}
				}
				if (in_line)
				{
					temp_line.to = 31;
					temp_line.avg = (temp_line.from + temp_line.to)/2;
					IntersectionList.insert(IntersectionList.end(),temp_line);
					in_line = false;
					//To az utolso
				}
				//Akkor tekintjuk elagazasnak, ha FOLLOW_INTERSECTION_LIMIT-szer talalunk 1-nel tobb elemet a listaban
				//TODO:Lehet-e tobb, mint 2 elagazas?
				unsigned short IntersectionAvgList[FOLLOW_INTERSECTION_LIMIT];
				Sens_Line_List::iterator path1, path2;
				if (IntersectionList.size() == 2)
				{
					path1 = IntersectionList.begin();
					path2 = IntersectionList.end();
					IntersectionAvgList[intersection_count] = abs(path1->avg - path2->avg);
					intersection_count++;
				}
				//TODO:Amig nem allitjuk le, de mar elagazast eszlelunk vmit kene csinalni az ertekekkel, mert a szab. igy nem lesz jo
				if (intersection_count == FOLLOW_INTERSECTION_LIMIT )
				{
					for (int i = 0; i < FOLLOW_INTERSECTION_LIMIT - 1; i++)
					{
						//Ha a ket elagazas kozott csokken a tavolsag, akkor fordulasnal visszafelé kell fordulni
						if (IntersectionAvgList[i] > IntersectionAvgList[i+1])
							turn_back = true;
					}
					primi->SetWheelSpeed(0,0);
					Follow_Status = FOLLOW_STATE_INTERSECTION_STOP;
					FollowLine_CurState = FOLLOW_STATE_STOP;
				}
				//Linearizalas: alapbol Sens_Pos: 15*2^28 <-> 15*2^0 --> Sens_Pos: 0 <-> 280
				//TODO: Ellenorizni, mert a hatarok kivulre is mehet, ha 5bit egy vonal!
				Lin_Sens_Pos = (int)(10*(log2(15/((double)Sens_Pos))+28));
			}
			break;
		case FOLLOW_STATE_PID:
			P = Lin_Sens_Pos;
			D = (Prev_Lin_Sens_Pos-Lin_Sens_Pos);
			I += Lin_Sens_Pos;
			Motor_Control = P*FOLLOW_PID_P + D*FOLLOW_PID_D + I*FOLLOW_PID_I;
			if (Motor_Control > FOLLOW_MAX_SPEED)
				Motor_Control = FOLLOW_MAX_SPEED;
			if (Motor_Control < -FOLLOW_MAX_SPEED)
				Motor_Control = -FOLLOW_MAX_SPEED;
			FollowLine_CurState = FOLLOW_STATE_MOTOR;
			break;
		case FOLLOW_STATE_MOTOR:
			//TODO:Hol lesz a szenzor, bal-jobb rendben van-e igy, ellenorizni
			//TODO:Jelezni kene, h a robot mozog, ahogy goto-nal is
			if (Motor_Control > 0)
				primi->SetWheelSpeed(FOLLOW_MAX_SPEED, FOLLOW_MAX_SPEED-(double)Motor_Control);
			else
				primi->SetWheelSpeed(FOLLOW_MAX_SPEED+Motor_Control, FOLLOW_MAX_SPEED);
			FollowLine_CurState = FOLLOW_STATE_CHECK;
			break;
		case FOLLOW_STATE_STOP:
			break;
	}
}
