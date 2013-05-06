/*
 * PrimitivesCan.cpp
 *
 *  Created on: Feb 15, 2011
 *      Author: triszalo
 */





#include "PrimitivesCan.h"

using namespace std;


void* recieveLoop(void* primitivescan_ptr){

	UDPmsg msg;
	PrimitivesCan* pc = (PrimitivesCan*)primitivescan_ptr;

	//folyamatosan, mert ez uj szal lesz
	while(1){

		//ha jott valami, feldolgozzuk
		if(UDPdriver::recieve(&msg, 0, 0)){

			//ENTER CRITICAL
			pc->EnterCritical();

			pc->evalMsg(&msg);
			pc->signalNewMsg();

			//EXIT CRITICAL
			pc->ExitCritical();

		}

	}

	return NULL;

}


void* sendLoop(void* primitivescan_ptr){

	PrimitivesCan* pc = (PrimitivesCan*)primitivescan_ptr;

	//folyamatosan, mert ez uj szal lesz
	while(1){

		//kuldunk egy keepAlive uzenetet mindenkinek
		pc->KEEP_ALIVE_SEND();

		//alszunk
		pc->KEEP_ALIVE_SLEEP();

	}

	return NULL;

}


PrimitivesCan::PrimitivesCan(Config* config) : Primitives(config){


	//---------- valtozo ELEJE ----------
	strcpy(CanIp, config->CanIp);

	dcwheelMotionError	= false;

#ifdef KIS_ROBOT
	mFollowLine = new FollowLine(this);
	Follow_InProgress = false;
	Follow_ts_valid = false;
#endif

	deadreckCheckXw		= 0;
	deadreckCheckYw		= 0;
	deadreckCheckPhiw	= 0;

	for(int i=0 ; i < SONAR_TRANSMITTER_COUNT ; i++){
		sonarXOffset[i] = 0;
		sonarYOffset[i] = 0;
	}

	deadreckCalibPhase		= 0;
	//---------- valtozo VEGE ----------


	//---------- node ELEJE ----------
	gateway		= new node_Gateway();
	broadcast	= new node_Broadcast();

	console		= new node_Console();
	input		= new node_Input();
	deadreck	= new node_Deadreck();
	dcwheel		= new node_DCWheel();
	servo		= new node_Servo();
	sonar		= new node_Sonar();
	#ifdef NAGY_ROBOT
	caracole	= new node_Caracole();
	firewheel	= new node_Firewheel();
	power		= new node_Power();
	#endif
	//---------- node VEGE ----------


}


void PrimitivesCan::addNodesToCan(void){


	if(!CONSOLE_ON_CANB)	gateway->GATEWAY_ADD_NODE_CANA(CONSOLE_ID);
	else					gateway->GATEWAY_ADD_NODE_CANB(CONSOLE_ID);

	if(!INPUT_ON_CANB)		gateway->GATEWAY_ADD_NODE_CANA(INPUT_ID);
	else					gateway->GATEWAY_ADD_NODE_CANB(INPUT_ID);

	if(!DEADRECK_ON_CANB)	gateway->GATEWAY_ADD_NODE_CANA(DEADRECK_ID);
	else					gateway->GATEWAY_ADD_NODE_CANB(DEADRECK_ID);

	if(!DCWHEEL_ON_CANB)	gateway->GATEWAY_ADD_NODE_CANA(DCWHEEL_ID);
	else					gateway->GATEWAY_ADD_NODE_CANB(DCWHEEL_ID);

	if(!SERVO_ON_CANB)		gateway->GATEWAY_ADD_NODE_CANA(SERVO_ID);
	else					gateway->GATEWAY_ADD_NODE_CANB(SERVO_ID);

	//SONAR-nak nem kuldunk semmit, pontosabban GATEWAY-nek nem adunk senkit a CAN_B-re, így oda tuti nem kuld, de fogadni fog
	//if(!SONAR_ON_CANB)		gateway->GATEWAY_ADD_NODE_CANA(SONAR_ID);
	//else					gateway->GATEWAY_ADD_NODE_CANB(SONAR_ID);
	
	#ifdef NAGY_ROBOT

	if(!CARACOLE_ON_CANB)	gateway->GATEWAY_ADD_NODE_CANA(CARACOLE_ID);
	else					gateway->GATEWAY_ADD_NODE_CANB(CARACOLE_ID);

	if(!FIREWHEEL_ON_CANB)	gateway->GATEWAY_ADD_NODE_CANA(FIREWHEEL_ID);
	else					gateway->GATEWAY_ADD_NODE_CANB(FIREWHEEL_ID);

	if(!POWER_ON_CANB)		gateway->GATEWAY_ADD_NODE_CANA(POWER_ID);
	else					gateway->GATEWAY_ADD_NODE_CANB(POWER_ID);
	
	#endif


}


bool PrimitivesCan::initNode(node* node){

	//ha nem sikeres a PING
	if(!node->PINGprocess())
		return false;

	//ha nem sikeres az INIT_PARAM
	if(!node->INITPARAMprocess())
		return false;

	//ha nem a BROADCAST_KEEP_ALIVE_MS erteket hasznaljuk
	if(!USE_BROADCAST_KEEP_ALIVE_MS)
		node->SET_KEEP_ALIVE_MS();

	//ha nem a BROADCAST_SEND_PERIOD_TO_PC_MS erteket hasznaljuk
	if(!USE_BROADCAST_SEND_PERIOD_TO_PC_MS)
		node->SET_SEND_PERIOD_TO_PC_MS();

	//ha nem a BROADCAST_SEND_PERIOD_TO_NODE_MS erteket hasznaljuk
	if(!USE_BROADCAST_SEND_PERIOD_TO_NODE_MS)
		node->SET_SEND_PERIOD_TO_NODE_MS();

	//minden OK
	return true;

}


bool PrimitivesCan::Init(void){


	//---------- UDP ELEJE ----------
	if(!UDPconn::init(CanIp))	return false;
	UDPdriver::init();
	//---------- UDP VEGE ----------


	//---------- recieve ELEJE ----------
	sem_init(&newMessageSemaphore, 0, 0);
	pthread_mutex_init(&recieveMutex, NULL);
	pthread_t rLoop;
	pthread_create(&rLoop, NULL, recieveLoop, this);
	//---------- recieve VEGE ----------


	//---------- node ELEJE ----------
	if(!initNode(gateway)	&& INIT_RETURN_FALSE_IF_ERROR)		return false;
	addNodesToCan();
	if(!initNode(console)	&& INIT_RETURN_FALSE_IF_ERROR)		return false;
	if(!initNode(input)		&& INIT_RETURN_FALSE_IF_ERROR)		return false;
	if(!initNode(deadreck)	&& INIT_RETURN_FALSE_IF_ERROR)		return false;
	if(!initNode(dcwheel)	&& INIT_RETURN_FALSE_IF_ERROR)		return false;
	if(!initNode(servo)		&& INIT_RETURN_FALSE_IF_ERROR)		return false;
	#ifdef NAGY_ROBOT
	if(!initNode(caracole)	&& INIT_RETURN_FALSE_IF_ERROR)		return false;
	if(!initNode(firewheel)	&& INIT_RETURN_FALSE_IF_ERROR)		return false;
	if(!initNode(power)		&& INIT_RETURN_FALSE_IF_ERROR)		return false;
	#endif
	//SONAR-nak nem kuldunk semmit
	//if(!initNode(sonar)		&& INIT_RETURN_FALSE_IF_ERROR)		return false;

	if(USE_BROADCAST_KEEP_ALIVE_MS)				broadcast->SET_KEEP_ALIVE_MS();
	if(USE_BROADCAST_SEND_PERIOD_TO_PC_MS)		broadcast->SET_SEND_PERIOD_TO_PC_MS();
	if(USE_BROADCAST_SEND_PERIOD_TO_NODE_MS)	broadcast->SET_SEND_PERIOD_TO_NODE_MS();
	//---------- node VEGE ----------


	//---------- keepAlive ELEJE ----------
	pthread_t sLoop;
	pthread_create(&sLoop, NULL, sendLoop, this);
	//---------- keepAlive VEGE ----------


	//---------- szemafor nullazasa ELEJE ----------
	sem_init(&newMessageSemaphore, 0, 0);
	//---------- szemafor nullazasa VEGE ----------



	//aktuator tap alapbol be- vagy kikapcsolva
	if(START_ACTUATOR_IN_INIT)		broadcast->START_ACTUATOR();
	else							broadcast->STOP_ACTUATOR();



	//minden OK
	return true;


}


PrimitivesCan::~PrimitivesCan(){

}

#ifdef NAGY_ROBOT
bool PrimitivesCan::Wait(long int useconds){

	timespec ts;
	clock_gettime(CLOCK_REALTIME, &ts);

	long int wait_nsec = (useconds % 1000000) * 1000;
	long int wait_sec = useconds / 1000000;

	//ha nincs tulcsordulas nanosec-ben
	if(1000000000 - wait_nsec > ts.tv_nsec){
		ts.tv_nsec += wait_nsec;
		ts.tv_sec += wait_sec;
	}

	//ha tulcsordulas van a nanosec-ben
	else{
		ts.tv_nsec = (ts.tv_nsec + wait_nsec) - 1000000000;
		ts.tv_sec += wait_sec + 1;
	}

	sem_timedwait(&newMessageSemaphore, &ts);

	return true;

}
#endif

#ifdef KIS_ROBOT
bool PrimitivesCan::Wait(long int useconds){

	timespec ts, ts_temp;
	clock_gettime(CLOCK_REALTIME, &ts);
	int error;

	if (Follow_InProgress)
	{
		if (!Follow_ts_valid)
		{
			long int wait_nsec = (FOLLOW_PERIOD_US % 1000000) * 1000;
			long int wait_sec = FOLLOW_PERIOD_US / 1000000;

			//ha nincs tulcsordulas nanosec-ben
			if(1000000000 - wait_nsec > Follow_next_ts.tv_nsec)
			{
				Follow_next_ts.tv_nsec += wait_nsec;
				Follow_next_ts.tv_sec += wait_sec;
			}
			else //ha tulcsordulas van a nanosec-ben
			{
				Follow_next_ts.tv_nsec = (Follow_next_ts.tv_nsec + wait_nsec) - 1000000000;
				Follow_next_ts.tv_sec += wait_sec + 1;
			}
			Follow_ts_valid = true;
		}
	}

	long int wait_nsec = (useconds % 1000000) * 1000;
	long int wait_sec = useconds / 1000000;

	//ha nincs tulcsordulas nanosec-ben
	if(1000000000 - wait_nsec > ts.tv_nsec){
		ts.tv_nsec += wait_nsec;
		ts.tv_sec += wait_sec;
	}

	//ha tulcsordulas van a nanosec-ben
	else{
		ts.tv_nsec = (ts.tv_nsec + wait_nsec) - 1000000000;
		ts.tv_sec += wait_sec + 1;
	}

	if (Follow_InProgress)
	{
		//Ha Follow_next_ts ideje kisebb, akkor az o idejeig fogunk varni
		if ((Follow_next_ts.tv_sec < ts.tv_sec) || ((Follow_next_ts.tv_sec == ts.tv_sec) && (Follow_next_ts.tv_nsec < ts.tv_nsec)))
		{
			//Hatha bantja az idot a szemafor
			ts_temp = Follow_next_ts;
			error = sem_timedwait(&newMessageSemaphore, &ts_temp);
			//Ha a szemafor-nal timeout volt, futtatjuk az FSM-et
			if (error == ETIMEDOUT)
			{
				mFollowLine->FSM_Run();
				//Ha leallt az FSM
				if (mFollowLine->Follow_Status != 0)
				{
					Follow_InProgress = false;
					Follow_ts_valid = false;
				}
				else //FSM-et meg futtatni kell
				{
					long int wait_nsec = (FOLLOW_PERIOD_US % 1000000) * 1000;
					long int wait_sec = FOLLOW_PERIOD_US / 1000000;

					//ha nincs tulcsordulas nanosec-ben
					if(1000000000 - wait_nsec > Follow_next_ts.tv_nsec)
					{
						Follow_next_ts.tv_nsec += wait_nsec;
						Follow_next_ts.tv_sec += wait_sec;
					}

					//ha tulcsordulas van a nanosec-ben
					else
					{
						Follow_next_ts.tv_nsec = (Follow_next_ts.tv_nsec + wait_nsec) - 1000000000;
						Follow_next_ts.tv_sec += wait_sec + 1;
					}
				}
			}
		}
		else
			error = sem_timedwait(&newMessageSemaphore, &ts);
	}

	return true;

}
#endif

void PrimitivesCan::EnterCritical(void){

	//lock
	pthread_mutex_lock(&recieveMutex);

}


void PrimitivesCan::ExitCritical(void){

	//unlock
	pthread_mutex_unlock(&recieveMutex);

}


bool PrimitivesCan::GetStartButton(void){

	EnterCritical();

	bool ret = input->GET_DIGITAL(INPUT_DIGITAL_START_BUTTON_INDEX);

	ExitCritical();

	return ret;

}


bool PrimitivesCan::GetStopButton(void){

	EnterCritical();

#ifdef NAGY_ROBOT
	bool ret = power->GET_STOP_BUTTON();
#else	//KIS_ROBOT
	//nincs külön aktuátortáp, soha nem lesz Vészstop benyomva, amit a szoftvernek érzékelnie kéne
	bool ret = false;
#endif

	ExitCritical();

	return ret;

}


int8_t PrimitivesCan::GetMyColor(void){

	EnterCritical();

	int8_t ret = GetMyColor_Unsafe();

	ExitCritical();

	return ret;

}


#ifdef NAGY_ROBOT
bool PrimitivesCan::GetBallPresent(void){

	EnterCritical();

	bool ret = input->GET_DIGITAL(INPUT_DIGITAL_BALL_PRESENT_INDEX);

	ExitCritical();

	return ret;

}
#endif


#ifdef NAGY_ROBOT
bool PrimitivesCan::SetMotorSupply(bool powered){

	EnterCritical();

	bool ret;


	//ha folyamatban van valami, amire ezt nem indithatjuk el
	if(power->ActOnOff.inProgress){
		ret = ACT_START_ERROR;
	}

	//ha elindithatjuk
	else{

		if(powered)	power->POWER_ACT_ON();
		else		power->POWER_ACT_OFF();

		ret = ACT_STARTED;

	}


	ExitCritical();

	return ret;

}


bool PrimitivesCan::SetMotorSupplyInProgress(void){

	EnterCritical();

	bool ret = power->ActOnOff.inProgress;

	ExitCritical();

	return ret;

}


bool PrimitivesCan::GetMotorSupply(void){

	EnterCritical();

	bool ret;

	power->GET_ACT_ON(&ret);

	ExitCritical();

	return ret;

}
#else	//KIS_ROBOT
bool PrimitivesCan::SetMotorSupply(bool powered){

	EnterCritical();

	//nincs külön aktuátortáp, azt mondjuk, hogy sikerült elindítani
	bool ret = ACT_STARTED;

	ExitCritical();

	return ret;

}


bool PrimitivesCan::SetMotorSupplyInProgress(void){

	EnterCritical();

	//nincs külön aktuátortáp, azt mondjuk, hogy nincs folyamatban
	bool ret = false;

	ExitCritical();

	return ret;

}


bool PrimitivesCan::GetMotorSupply(void){

	EnterCritical();

	//nincs külön aktuátortáp, azt mondjuk, hogy jelen van
	bool ret = true;

	ExitCritical();

	return ret;

}
#endif


bool PrimitivesCan::CalibrateDeadreckoning(bool simulate){

	EnterCritical();

	bool ret;


	//ha csak szimulacio
	if(simulate){
		Calibrate_Unsafe();
		ret = true;
	}


	//ha nem csak szimulacio
	else{

		//ha folyamatban van valami, amire ezt nem indithatjuk el
		if(deadreckCalibPhase != 0){
			ret = ACT_START_ERROR;
		}

		//ha elindithatjuk
		else{
			deadreckCalibPhase = 1;
			ret = ACT_STARTED;
		}

	}


	ExitCritical();

	return ret;

}


bool PrimitivesCan::CalibrateDeadreckoningInProgress(void){

	EnterCritical();

	bool ret;


	switch(deadreckCalibPhase){

		//varunk az egyik kapcsolora
		case 1:

#if 0	//ütközéskapcsoló
			//ha megvan valamelyik utkozeskapcsolo
			if(	input->GET_DIGITAL(INPUT_DIGITAL_REAR_LEFT_LIMIT_SWITCH_INDEX) ||
				input->GET_DIGITAL(INPUT_DIGITAL_REAR_RIGHT_LIMIT_SWITCH_INDEX))
				deadreckCalibPhase = 2;
#else	//színkapcsoló
			//ha megvan a színkapcsoló
			if(HasColor_Unsafe())
				deadreckCalibPhase = 2;
#endif

			ret = true;

			break;

		//reset
		case 2:
			deadreck->DEADRECK_RESET_POS();
			deadreckCalibPhase = 3;
			ret = true;
			break;

		//varunk a reset-re
		case 3:

			//ha folyamatban van a reset
			if(deadreck->ResetPos.inProgress)
				ret = true;

			//ha megvolt a reset
			else{
				Calibrate_Unsafe();
				deadreckCalibPhase = 0;
				ret = false;
			}

			break;

		//ilyen nem lehet, azt mondjuk, hogy folyamatban van
		default:
			deadreckCalibPhase = 0;
			ret = true;
			break;


	}


	//cout << "dp\t" << deadreckCalibPhase << endl;


	ExitCritical();

	return ret;

}


bool PrimitivesCan::SetSpeed(double v, double w){

	EnterCritical();

	bool ret;


	//ha folyamatban van valami, amire ezt nem indithatjuk el
	if(	dcwheel->AnyStop.inProgress ||
		dcwheel->AnyMotion.inProgress ||
		dcwheel->AnySpeed.inProgress){
		ret = ACT_START_ERROR;
	}

	//ha elindithatjuk
	else{
		dcwheel->DCWHEEL_SET_SPEED(v, w);
		ret = ACT_STARTED;
	}


	ExitCritical();

	return ret;

}


bool PrimitivesCan::SetWheelSpeed(double vLeft, double vRight){

	EnterCritical();

	bool ret;


	//ha folyamatban van valami, amire ezt nem indithatjuk el
	if(	dcwheel->AnyStop.inProgress ||
		dcwheel->AnyMotion.inProgress ||
		dcwheel->AnySpeed.inProgress){
		ret = ACT_START_ERROR;
	}

	//ha elindithatjuk
	else{
		dcwheel->DCWHEEL_SET_WHEELSPEED(vLeft, vRight);
		ret = ACT_STARTED;
	}


	ExitCritical();

	return ret;

}


bool PrimitivesCan::SetSpeedInProgress(void){

	EnterCritical();

	bool ret = dcwheel->AnySpeed.inProgress;

	ExitCritical();

	return ret;

}


bool PrimitivesCan::Go(double distance, double max_speed, double max_acc){

	EnterCritical();

	bool ret;


	//ha folyamatban van valami, amire ezt nem indithatjuk el
	if(	dcwheel->AnyStop.inProgress ||
		dcwheel->AnyMotion.inProgress ||
		dcwheel->AnySpeed.inProgress){
		ret = ACT_START_ERROR;
	}

	//ha elindithatjuk
	else{
		dcwheel->DCWHEEL_GO(distance, max_speed, max_acc);
		dcwheelMotionError = MOTION_NO_ERROR;	//nincs hiba
		ret = ACT_STARTED;
	}


	ExitCritical();

	return ret;

}


bool PrimitivesCan::GoTo(double x, double y, double max_speed, double max_acc){

	EnterCritical();

	bool ret;
	double xr, yr, phir;
	double xw, yw, phiw;


	//ha folyamatban van valami, amire ezt nem indithatjuk el
	if(	dcwheel->AnyStop.inProgress ||
		dcwheel->AnyMotion.inProgress ||
		dcwheel->AnySpeed.inProgress){
		ret = ACT_START_ERROR;
	}

	//ha elindithatjuk
	else{
		ConvWorldToRobot(x, y, 0, &xr, &yr, &phir);
		dcwheel->DCWHEEL_GOTO(xr, yr, max_speed, max_acc);
		dcwheelMotionError = MOTION_NO_ERROR;	//nincs hiba
		GetRobotPos_Unsafe(&xw, &yw, &phiw);
		ret = ACT_STARTED;
	}


	ExitCritical();

	return ret;

}
bool PrimitivesCan::GoArc(double x, double y, double r, double phi, double max_w, double max_acc){
	EnterCritical();

	bool ret;

	//ha folyamatban van valami, amire ezt nem indithatjuk el
	if(	dcwheel->AnyStop.inProgress ||
		dcwheel->AnyMotion.inProgress ||
		dcwheel->AnySpeed.inProgress){
		ret = ACT_START_ERROR;
	}

	//ha elindithatjuk
	else{
		dcwheel->DCWHEEL_SET_SPEED(0.,0.);	//eloszor nullaval megy, kezzel lesz novelve
		dcwheelMotionError = MOTION_NO_ERROR;	//nincs hiba
		ret = ACT_STARTED;
	}


	ExitCritical();

	return ret;
}

bool PrimitivesCan::Turn(double angle, double max_speed, double max_acc){

	EnterCritical();

	bool ret;


	//ha folyamatban van valami, amire ezt nem indithatjuk el
	if(	dcwheel->AnyStop.inProgress ||
		dcwheel->AnyMotion.inProgress ||
		dcwheel->AnySpeed.inProgress){
		ret = ACT_START_ERROR;
	}

	//ha elindithatjuk
	else{
		dcwheel->DCWHEEL_TURN(angle, max_speed, max_acc);
		dcwheelMotionError = MOTION_NO_ERROR;	//nincs hiba
		ret = ACT_STARTED;
	}


	ExitCritical();

	return ret;

}


bool PrimitivesCan::MotionInProgress(void){

	EnterCritical();

	bool ret = dcwheel->AnyMotion.inProgress;

	//ha utkozes kovetkezett be
	if(ret && (	input->GET_DIGITAL(INPUT_DIGITAL_REAR_LEFT_LIMIT_SWITCH_INDEX) ||
				input->GET_DIGITAL(INPUT_DIGITAL_REAR_RIGHT_LIMIT_SWITCH_INDEX)))
		dcwheelMotionError = MOTION_ERROR;

	if (!ret && dcwheel->AnyMotion.done == 0) {
		dcwheelMotionError = MOTION_ERROR;
	}

	ExitCritical();

	return ret;

}


int PrimitivesCan::GetMotionError(void){

	EnterCritical();

	int ret = (dcwheelMotionError ? MOTION_ERROR : MOTION_NO_ERROR);

	ExitCritical();

	return ret;

}


bool PrimitivesCan::MotionStop(double dec){

	EnterCritical();

	bool ret;


	//ha folyamatban van valami, amire ezt nem indithatjuk el
	if(	dcwheel->AnyStop.inProgress){
		ret = ACT_START_ERROR;
	}

	//ha elindithatjuk
	else{

		//MotionStop hivasnal az osszes tobbi mozgast leallitjuk
		dcwheel->AnyMotion.inProgress = false;

		if(dec != 0)	dcwheel->DCWHEEL_STOP(dec);
		else			dcwheel->DCWHEEL_HARD_STOP();

		ret = ACT_STARTED;
	}


	ExitCritical();

	return ret;

}


bool PrimitivesCan::MotionStopInProgress(void){

	EnterCritical();

	bool ret = dcwheel->AnyStop.inProgress;

	ExitCritical();

	return ret;

}


void PrimitivesCan::GetRobotPos(double* x, double* y, double* phi){

	EnterCritical();

	GetRobotPos_Unsafe(x, y, phi);

	ExitCritical();

}


long int PrimitivesCan::GetOpponentPos(unsigned char n, double * x, double* y){

	EnterCritical();

	long int ret = GetOpponentPos_Unsafe(n, x, y);

	ExitCritical();

	return ret;

}


void PrimitivesCan::GetSpeed(double* v, double* w){

	EnterCritical();

	deadreck->GET_SPEED(v, w);

	ExitCritical();

}


void PrimitivesCan::SetRobotPos(double x, double y, double phi){

	EnterCritical();

	SetRobotPos_Unsafe(x, y, phi);

	ExitCritical();

}


void PrimitivesCan::SetOpponentPos(unsigned char n, double x, double y){

	EnterCritical();

	double tmpX, tmpY;

	GetOpponentPos_Unsafe(n, &tmpX, &tmpY);

	sonarXOffset[n] = x - tmpX;
	sonarYOffset[n] = y - tmpY;

	ExitCritical();

}


void PrimitivesCan::GetDistances(double distance[PROXIMITY_NUM]){

	EnterCritical();

	distance[0] = input->GET_DISTANCE(INPUT_ANALOG_RIGHT_FRONT_SHARP_INDEX);
	distance[1] = input->GET_DISTANCE(INPUT_ANALOG_LEFT_FRONT_SHARP_INDEX);
	distance[2] = input->GET_DISTANCE(INPUT_ANALOG_RIGHT_REAR_SHARP_INDEX);
	distance[3] = input->GET_DISTANCE(INPUT_ANALOG_LEFT_REAR_SHARP_INDEX);

	ExitCritical();

}


#ifdef NAGY_ROBOT
double PrimitivesCan::GetBallColorVoltage(void){

	EnterCritical();

	double voltage = input->GET_VOLTAGE(INPUT_ANALOG_BALL_COLOR_CNY70_INDEX);

	ExitCritical();

	return voltage;

}


bool PrimitivesCan::ArmMove(bool front, double pos, double max_speed, double max_acc){

	EnterCritical();

	bool ret;
	u8 num = (front ? SERVO_FRONT_ARM_INDEX : SERVO_REAR_ARM_INDEX);

	if(servo->Setpos[num].inProgress){
		ret = ACT_START_ERROR;
	}

	//ha elindithatjuk
	else{
		servo->SERVO_SET_POS(num, pos, max_speed, max_acc);
		ret = ACT_STARTED;
	}


	ExitCritical();

	return ret;

}


bool PrimitivesCan::ArmMoveInProgress(bool front){

	EnterCritical();

	u8 num = (front ? SERVO_FRONT_ARM_INDEX : SERVO_REAR_ARM_INDEX);
	bool ret = servo->Setpos[num].inProgress;

	ExitCritical();

	return ret;

}


double PrimitivesCan::GetArmPos(bool front){

	EnterCritical();

	u8 num = (front ? SERVO_FRONT_ARM_INDEX : SERVO_REAR_ARM_INDEX);
	double ret = servo->GET_POS(num);

	ExitCritical();

	return ret;

}


bool PrimitivesCan::GetArmError(bool front){

	EnterCritical();

	u8 num = (front ? SERVO_FRONT_ARM_INDEX : SERVO_REAR_ARM_INDEX);
	bool ret = servo->GET_ERROR(num);

	ExitCritical();

	return ret;

}


bool PrimitivesCan::ContractorMove(double pos, double max_speed, double max_acc){

	EnterCritical();

	bool ret;
	u8 num = SERVO_CONTRACTOR_INDEX;

	if(servo->Setpos[num].inProgress){
		ret = ACT_START_ERROR;
	}

	//ha elindithatjuk
	else{
		servo->SERVO_SET_POS(num, pos, max_speed, max_acc);
		ret = ACT_STARTED;
	}


	ExitCritical();

	return ret;

}


bool PrimitivesCan::ContractorMoveInProgress(){

	EnterCritical();

	u8 num = SERVO_CONTRACTOR_INDEX;
	bool ret = servo->Setpos[num].inProgress;

	ExitCritical();

	return ret;

}


double PrimitivesCan::GetContractorPos(){

	EnterCritical();

	u8 num = SERVO_CONTRACTOR_INDEX;
	double ret = servo->GET_POS(num);

	ExitCritical();

	return ret;

}


bool PrimitivesCan::GetContractorError(){

	EnterCritical();

	u8 num = SERVO_CONTRACTOR_INDEX;
	bool ret = servo->GET_ERROR(num);

	ExitCritical();

	return ret;

}


bool PrimitivesCan::FlipperMove(double pos, double max_speed, double max_acc){

	EnterCritical();

	bool ret;
	u8 num = SERVO_FLIPPER_INDEX;

	if(servo->Setpos[num].inProgress){
		ret = ACT_START_ERROR;
	}

	//ha elindithatjuk
	else{
		servo->SERVO_SET_POS(num, pos, max_speed, max_acc);
		ret = ACT_STARTED;
	}


	ExitCritical();

	return ret;

}


bool PrimitivesCan::FlipperMoveInProgress(void){

	EnterCritical();

	u8 num = SERVO_FLIPPER_INDEX;
	bool ret = servo->Setpos[num].inProgress;

	ExitCritical();

	return ret;

}


double PrimitivesCan::GetFlipperPos(void){

	EnterCritical();

	u8 num = SERVO_FLIPPER_INDEX;
	double ret = servo->GET_POS(num);

	ExitCritical();

	return ret;

}


bool PrimitivesCan::GetFlipperError(){

	EnterCritical();

	u8 num = SERVO_FLIPPER_INDEX;
	bool ret = servo->GET_ERROR(num);

	ExitCritical();

	return ret;

}


bool PrimitivesCan::SelectorMove(double pos, double max_speed, double max_acc){

	EnterCritical();

	bool ret;
	u8 num = SERVO_SELECTOR_INDEX;

	if(servo->Setpos[num].inProgress){
		ret = ACT_START_ERROR;
	}

	//ha elindithatjuk
	else{
		servo->SERVO_SET_POS(num, pos, max_speed, max_acc);
		ret = ACT_STARTED;
	}


	ExitCritical();

	return ret;

}


bool PrimitivesCan::SelectorMoveInProgress(){

	EnterCritical();

	u8 num = SERVO_SELECTOR_INDEX;
	bool ret = servo->Setpos[num].inProgress;

	ExitCritical();

	return ret;

}


double PrimitivesCan::GetSelectorPos(){

	EnterCritical();

	u8 num = SERVO_SELECTOR_INDEX;
	double ret = servo->GET_POS(num);

	ExitCritical();

	return ret;

}


bool PrimitivesCan::GetSelectorError(){

	EnterCritical();

	u8 num = SERVO_SELECTOR_INDEX;
	bool ret = servo->GET_ERROR(num);

	ExitCritical();

	return ret;

}


bool PrimitivesCan::FireStopperMove(double pos, double max_speed, double max_acc){

	EnterCritical();

	bool ret;
	u8 num = SERVO_FIRESTOPPER_INDEX;


	//ha folyamatban van valami, amire ezt nem indithatjuk el
	if(servo->Setpos[num].inProgress){
		ret = ACT_START_ERROR;
	}

	//ha elindithatjuk
	else{
		servo->SERVO_SET_POS(num, pos, max_speed, max_acc);
		ret = ACT_STARTED;
	}


	ExitCritical();

	return ret;

}


bool PrimitivesCan::FireStopperMoveInProgress(){

	EnterCritical();

	u8 num = SERVO_FIRESTOPPER_INDEX;
	bool ret = servo->Setpos[num].inProgress;

	ExitCritical();

	return ret;

}


double PrimitivesCan::GetFireStopperPos(){

	EnterCritical();

	u8 num = SERVO_FIRESTOPPER_INDEX;
	double ret = servo->GET_POS(num);

	ExitCritical();

	return ret;

}


bool PrimitivesCan::GetFireStopperError(void){

	EnterCritical();

	u8 num = SERVO_FIRESTOPPER_INDEX;
	bool ret = servo->GET_ERROR(num);

	ExitCritical();

	return ret;

}
#else	//KIS_ROBOT

bool PrimitivesCan::GripperMove(bool low, bool left, double pos, double max_speed, double max_acc){

	u8 num;

	EnterCritical();

	bool ret;

	if(low){
		if(left)	num = SERVO_LEFT_LOW_GRIPPER_INDEX;
		else		num = SERVO_RIGHT_HIGH_GRIPPER_INDEX;
	}
	else{
		if(left)	num = SERVO_LEFT_HIGH_GRIPPER_INDEX;
		else		num = SERVO_RIGHT_HIGH_GRIPPER_INDEX;
	}

	if(servo->Setpos[num].inProgress){
		ret = ACT_START_ERROR;
	}

	//ha elindithatjuk
	else{
		servo->SERVO_SET_POS(num, pos, max_speed, max_acc);
		ret = ACT_STARTED;
	}


	ExitCritical();

	return ret;

}


bool PrimitivesCan::GripperMoveInProgress(bool low, bool left){

	u8 num;

	EnterCritical();

	if(low){
		if(left)	num = SERVO_LEFT_LOW_GRIPPER_INDEX;
		else		num = SERVO_RIGHT_HIGH_GRIPPER_INDEX;
	}
	else{
		if(left)	num = SERVO_LEFT_HIGH_GRIPPER_INDEX;
		else		num = SERVO_RIGHT_HIGH_GRIPPER_INDEX;
	}

	bool ret = servo->Setpos[num].inProgress;

	ExitCritical();

	return ret;

}


double PrimitivesCan::GetGripperPos(bool low, bool left){

	u8 num;

	EnterCritical();

	if(low){
		if(left)	num = SERVO_LEFT_LOW_GRIPPER_INDEX;
		else		num = SERVO_RIGHT_HIGH_GRIPPER_INDEX;
	}
	else{
		if(left)	num = SERVO_LEFT_HIGH_GRIPPER_INDEX;
		else		num = SERVO_RIGHT_HIGH_GRIPPER_INDEX;
	}

	double ret = servo->GET_POS(num);

	ExitCritical();

	return ret;

}


bool PrimitivesCan::GetGripperError(bool low, bool left){

	u8 num;

	EnterCritical();

	if(low){
		if(left)	num = SERVO_LEFT_LOW_GRIPPER_INDEX;
		else		num = SERVO_RIGHT_HIGH_GRIPPER_INDEX;
	}
	else{
		if(left)	num = SERVO_LEFT_HIGH_GRIPPER_INDEX;
		else		num = SERVO_RIGHT_HIGH_GRIPPER_INDEX;
	}

	bool ret = servo->GET_ERROR(num);

	ExitCritical();

	return ret;

}

#endif


bool PrimitivesCan::CalibrateConsole(void){

	EnterCritical();

	bool ret;


	//ha folyamatban van valami, amire ezt nem indithatjuk el
	if(	console->Calibrate.inProgress ||
		console->SetPos.inProgress ||
		console->Stop.inProgress){
		ret = ACT_START_ERROR;
	}

	//ha elindithatjuk
	else{
		console->CONSOLE_CALIBRATE();
		ret = ACT_STARTED;
	}


	ExitCritical();

	return ret;

}


bool PrimitivesCan::CalibrateConsoleInProgress(void){

	EnterCritical();

	bool ret = console->Calibrate.inProgress;

	ExitCritical();

	return ret;

}


bool PrimitivesCan::ConsoleMove(double pos, double max_speed, double max_acc){

	EnterCritical();

	bool ret;


	//ha folyamatban van valami, amire ezt nem indithatjuk el
	if(	console->Calibrate.inProgress ||
		console->SetPos.inProgress ||
		console->Stop.inProgress){
		ret = ACT_START_ERROR;
	}

	//ha elindithatjuk
	else{
		console->CONSOLE_SET_POS(pos, max_speed, max_acc);
		ret = ACT_STARTED;
	}


	ExitCritical();

	return ret;

}


bool PrimitivesCan::ConsoleMoveInProgress(void){

	EnterCritical();

	bool ret = console->SetPos.inProgress;

	ExitCritical();

	return ret;

}


bool PrimitivesCan::ConsoleStop(void){

	EnterCritical();

	bool ret;


	//ha folyamatban van valami, amire ezt nem indithatjuk el
	if(	console->Stop.inProgress){
		ret = ACT_START_ERROR;
	}

	//ha elindithatjuk
	else{
		console->CONSOLE_STOP();
		ret = ACT_STARTED;
	}


	ExitCritical();

	return ret;

}


bool PrimitivesCan::ConsoleStopInProgress(void){

	EnterCritical();

	bool ret = console->Stop.inProgress;

	ExitCritical();

	return ret;

}


double PrimitivesCan::GetConsolePos(void){

	EnterCritical();

	double ret = console->GET_POS();

	ExitCritical();

	return ret;

}


#ifdef NAGY_ROBOT
bool PrimitivesCan::CaracoleSetSpeed(double speed, double max_acc){

	EnterCritical();

	bool ret;


	//ha folyamatban van valami, amire ezt nem indithatjuk el
	if(caracole->SetSpeed.inProgress){
		ret = ACT_START_ERROR;
	}

	//ha elindithatjuk
	else{
		caracole->CARACOLE_SET_SPEED(speed, max_acc);
		ret = ACT_STARTED;
	}


	ExitCritical();

	return ret;

}


bool PrimitivesCan::CaracoleSetSpeedInProgress(void){

	EnterCritical();

	bool ret = caracole->SetSpeed.inProgress;

	ExitCritical();

	return ret;

}


double PrimitivesCan::GetCaracoleSpeed(void){

	EnterCritical();

	double ret = caracole->GET_SPEED();

	ExitCritical();

	return ret;

}


bool PrimitivesCan::FirewheelSetSpeed(double speed, double max_acc){

	EnterCritical();

	bool ret;


	//ha folyamatban van valami, amire ezt nem indithatjuk el
	if(firewheel->SetSpeed.inProgress){
		ret = ACT_START_ERROR;
	}

	//ha elindithatjuk
	else{
		firewheel->FIREWHEEL_SET_SPEED(speed, max_acc);
		ret = ACT_STARTED;
	}


	ExitCritical();

	return ret;

}


bool PrimitivesCan::FirewheelSetSpeedInProgress(void){

	EnterCritical();

	bool ret = firewheel->SetSpeed.inProgress;

	ExitCritical();

	return ret;

}


double PrimitivesCan::GetFirewheelSpeed(void){

	EnterCritical();

	double ret = firewheel->GET_SPEED();

	ExitCritical();

	return ret;

}
#endif


void PrimitivesCan::detectActChange(void){

	bool ActOn;

#ifdef NAGY_ROBOT
	bool change = power->GET_ACT_ON(&ActOn);
#else	//KIS_ROBOT
	bool change = false;
	ActOn = true;
#endif

	//ha valtozott az aktuatortap allapota
	if(change){

		//ha felkapcsolodott
		if(ActOn)
			broadcast->START_ACTUATOR();

		//ha lekapcsolodott
		else
			broadcast->STOP_ACTUATOR();

	}

}


void PrimitivesCan::evalMsg(UDPmsg* msg){

	//cout << msg->node_id << "\t" << msg->function << "\t" << msg->length << "\t" << msg->data << endl;

	//osszes node-nak odaadjuk kiertekelesre
	//csak az fogja kiertekelni, akinek az azonositojaval megegyezik az msg->node_id
	//ha egy node a valosagban tobb funkciot megvalosit, akkor annak az osszes osztajat meghivjuk kiertekelesre
	gateway->evalMsg(msg);
	console->evalMsg(msg);
	input->evalMsg(msg);
	deadreck->evalMsg(msg);
	dcwheel->evalMsg(msg);
	servo->evalMsg(msg);
	sonar->evalMsg(msg);

	//TODO: David ez mit csinal? ezek nem voltak ifdef-elve

#ifdef NAGY_ROBOT
	caracole->evalMsg(msg);
	firewheel->evalMsg(msg);
	power->evalMsg(msg);
#endif

	//figyeljuk az aktuatortap valtozasat
	detectActChange();


}


void PrimitivesCan::signalNewMsg(void){

	sem_post(&newMessageSemaphore);

}


void PrimitivesCan::KEEP_ALIVE_SEND(void){

	broadcast->KEEP_ALIVE();

}


void PrimitivesCan::KEEP_ALIVE_SLEEP(void){

	usleep(KEEP_ALIVE_PERIOD_MS * 1000);

}


void PrimitivesCan::ConvRobotToWorld(double xr, double yr, double phir, double* xw, double* yw, double* phiw){

	//elforgatom, majd eltolom
	*xw = deadreckCheckXw + xr * cos(deadreckCheckPhiw) - yr * sin(deadreckCheckPhiw);
	*yw = deadreckCheckYw + xr * sin(deadreckCheckPhiw) + yr * cos(deadreckCheckPhiw);
	*phiw = deadreckCheckPhiw + phir;

	while(*phiw > M_PI)
		*phiw -= 2*M_PI;

	while(*phiw <= -M_PI)
		*phiw += 2*M_PI;

}


void PrimitivesCan::ConvWorldToRobot(double xw, double yw, double phiw, double* xr, double* yr, double* phir){

	//eltolom, majd elforgatom
	*xr = (xw - deadreckCheckXw) * cos(-deadreckCheckPhiw) - (yw - deadreckCheckYw) * sin(-deadreckCheckPhiw);
	*yr = (xw - deadreckCheckXw) * sin(-deadreckCheckPhiw) + (yw - deadreckCheckYw) * cos(-deadreckCheckPhiw);
	*phir = phiw - deadreckCheckPhiw;

	while(*phir > M_PI)
		*phir -= 2*M_PI;

	while(*phir <= -M_PI)
		*phir += 2*M_PI;

}


void PrimitivesCan::Calibrate_Unsafe(void){

	//blue
	if(GetMyColor_Unsafe() == COLOR_BLUE){
		SetRobotPos_Unsafe(DEADRECK_CALIB_DISTANCE_X, DEADRECK_CALIB_DISTANCE_Y, DEADRECK_CALIB_PHI);
		TM33Build(SONAR_TX_BLUE, SONAR_TY_BLUE, SONAR_ALPHA_BLUE, T33WS);
	}

	//red
	else{
		SetRobotPos_Unsafe(DEADRECK_CALIB_DISTANCE_X, AREA_LENGTH_Y - DEADRECK_CALIB_DISTANCE_Y, -DEADRECK_CALIB_PHI);
		TM33Build(SONAR_TX_RED, SONAR_TY_RED, SONAR_ALPHA_RED, T33WS);
	}

}


void PrimitivesCan::GetRobotPos_Unsafe(double* x, double* y, double* phi){

	double xr, yr, phir;

	deadreck->GET_POS(&xr, &yr, &phir);

	ConvRobotToWorld(xr, yr, phir, x, y, phi);

}


long int PrimitivesCan::GetOpponentPos_Unsafe(u8 num, double* x, double* y){

	long int ret;

	double sonarpos_S[3];	// Szonar pozicioadat
	double sonarpos_W[3];	// Szonar pozicioadat vilagkoordinatakban

	ret = sonar->GET_POS(num, (&sonarpos_S[0]), (&sonarpos_S[1]));
	sonarpos_S[2] = 1.0;

	M33V3Mult(T33WS, sonarpos_S, sonarpos_W);

	*x = sonarpos_W[0];
	*y = sonarpos_W[1];

	//offset hizzaadasa
	*x += sonarXOffset[num];
	*y += sonarYOffset[num];

	return ret;

}


void PrimitivesCan::SetRobotPos_Unsafe(double x, double y, double phi){

	double xr, yr, phir;

	deadreck->GET_POS(&xr, &yr, &phir);

	//ConvRobotToWorld() fuggvenyszamitasabol
	deadreckCheckPhiw = phi - phir;
	deadreckCheckXw = x - (xr * cos(deadreckCheckPhiw) - yr * sin(deadreckCheckPhiw));
	deadreckCheckYw = y - (xr * sin(deadreckCheckPhiw) + yr * cos(deadreckCheckPhiw));

}


int8_t PrimitivesCan::GetMyColor_Unsafe(void){

	bool red = input->GET_DIGITAL(INPUT_DIGITAL_COLOR_RED_BUTTON_INDEX);
	bool blue = input->GET_DIGITAL(INPUT_DIGITAL_COLOR_BLUE_BUTTON_INDEX);

	if(red)			return COLOR_RED;
	if(blue)		return COLOR_BLUE;
	return COLOR_UNKNOWN;	//ha nincs színünk

}


bool PrimitivesCan::HasColor_Unsafe(void){

	return (GetMyColor_Unsafe() != COLOR_UNKNOWN);

}

#ifdef KIS_ROBOT
bool PrimitivesCan::FollowLine_Follow(double dist)
{
	EnterCritical();

	bool ret;

	//ha folyamatban van valami, amire ezt nem indithatjuk el
	if(	Follow_InProgress ||
		mFollowLine->Calibrate_InProgress){
		ret = ACT_START_ERROR;
	}

	//ha elindithatjuk
	else{
		Follow_InProgress = true;
		mFollowLine->Follow_Init(dist);
		ret = ACT_STARTED;
	}


	ExitCritical();

	return ret;
}

bool PrimitivesCan::FollowLine_FollowInProgress()
{
	EnterCritical();

	int ret = Follow_InProgress;

	ExitCritical();

	return ret;
}

int PrimitivesCan::FollowLine_GetFollowError()
{
	EnterCritical();
	int error = mFollowLine->Follow_Status;

	ExitCritical();

	return error;
}

bool PrimitivesCan::FollowLine_FixThreshold(uint16_t threshold)
{
	EnterCritical();

	bool ret;

	//ha folyamatban van valami, amire ezt nem indithatjuk el
	if(	Follow_InProgress ||
		mFollowLine->Calibrate_InProgress){
		ret = ACT_START_ERROR;
	}

	//ha elindithatjuk
	else{
		ret = mFollowLine->FixThreshold(threshold);
	}


	ExitCritical();

	return ret;
}

bool PrimitivesCan::FollowLine_Calibrate()
{
	EnterCritical();

	bool ret;

	//ha folyamatban van valami, amire ezt nem indithatjuk el
	if(	Follow_InProgress ||
		mFollowLine->Calibrate_InProgress){
		ret = ACT_START_ERROR;
	}

	//ha elindithatjuk
	else{
		mFollowLine->Calibrate();
		ret = ACT_STARTED;
	}


	ExitCritical();

	return ret;
}

bool PrimitivesCan::FollowLine_CalibrateInProgress()
{
	EnterCritical();

	bool ret = mFollowLine->Calibrate_InProgress;

	ExitCritical();

	return ret;
}
#endif
