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

	UDPconn::init(config->CanIp);
	UDPdriver::init();


	//---------- valtozo ELEJE ----------
	deadreckPosOffsetX		= 0;
	deadreckPosOffsetY		= 0;
	deadreckPosOffsetPhi	= 0;

	deadreckCalibPhase		= 0;
	goToWallPhase			= 0;
	//---------- valtozo VEGE ----------


	//---------- mutex ELEJE ----------
	pthread_mutex_init(&recieveMutex, NULL);
	//---------- mutex VEGE ----------


	//---------- node ELEJE ----------
	gateway		= new node_Gateway();
	broadcast	= new node_Broadcast();

	console		= new node_Console();
	deadreck	= new node_Deadreck();
	bdc			= new node_BDC();
	input		= new node_Input();
	magnet		= new node_Magnet();
	servo		= new node_Servo();
	sonar		= new node_Sonar();
	power		= new node_Power();
	//---------- node VEGE ----------


	//---------- recieve ELEJE ----------
	sem_init(&newMessageSemaphore, 0, 0);
	pthread_t rLoop;
	pthread_create(&rLoop, NULL, recieveLoop, this);
	//---------- recieve VEGE ----------


	//---------- gateway PINGprocess ELEJE ----------
	gateway->PINGprocess();
	//---------- gateway PINGprocess VEGE ----------


	//---------- add node ELEJE ----------
	gateway->GATEWAY_ADD_NODE_CANA(CONSOLE_ID);
	gateway->GATEWAY_ADD_NODE_CANA(DEADRECK_ID);
	gateway->GATEWAY_ADD_NODE_CANA(BDC_ID);
	gateway->GATEWAY_ADD_NODE_CANA(INPUT_ID);
	gateway->GATEWAY_ADD_NODE_CANA(MAGNET_ID);
	gateway->GATEWAY_ADD_NODE_CANA(SERVO_ID);
	gateway->GATEWAY_ADD_NODE_CANA(SONAR_ID);
	gateway->GATEWAY_ADD_NODE_CANA(POWER_ID);
	//---------- add node VEGE ----------


	//---------- node PINGprocess ELEJE ----------
	console->PINGprocess();
	deadreck->PINGprocess();
	bdc->PINGprocess();
	input->PINGprocess();
	magnet->PINGprocess();
	servo->PINGprocess();
	sonar->PINGprocess();
	power->PINGprocess();
	//---------- node PINGprocess VEGE ----------


	//---------- idozites ELEJE ----------
	broadcast->SET_KEEP_ALIVE_MS(KEEP_ALIVE_MS);
	broadcast->SET_SEND_PERIOD_TO_PC_MS(SEND_PERIOD_TO_PC_MS);
	broadcast->SET_SEND_PERIOD_TO_NODE_MS(SEND_PERIOD_TO_NODE_MS);
	//---------- idozites VEGE ----------


	//---------- keepAlive ELEJE ----------
	pthread_t sLoop;
	pthread_create(&sLoop, NULL, sendLoop, this);
	//---------- keepAlive VEGE ----------


	//---------- szemafor nullazasa ELEJE ----------
	sem_init(&newMessageSemaphore, 0, 0);
	//---------- szemafor nullazasa VEGE ----------

}


PrimitivesCan::~PrimitivesCan(){

}


bool PrimitivesCan::Init(void){

	return true;

}


bool PrimitivesCan::Wait(long int useconds){

	timespec ts;
	clock_gettime(CLOCK_REALTIME, &ts);

	long int wait_nsec = (useconds % 1000) * 1000;
	long int wait_sec = useconds / 1000;

	//ha nincs tulcsordulas nanosec-ben
	if(1000000 - wait_nsec > ts.tv_nsec){
		ts.tv_nsec += wait_nsec;
		ts.tv_sec += wait_sec;
	}

	//ha tulcsordulas van a nanosec-ben
	else{
		ts.tv_nsec = (ts.tv_nsec + wait_nsec) - 1000000;
		ts.tv_sec += wait_sec + 1;
	}

	sem_timedwait(&newMessageSemaphore, &ts);

	return true;

}


void PrimitivesCan::EnterCritical(void){

	//lock
	pthread_mutex_lock(&recieveMutex);

}


void PrimitivesCan::ExitCritical(void){

	//unlock
	pthread_mutex_unlock(&recieveMutex);

}


//distance [mm]
//max_speed [mm/s]
//max_acc [mm/s^2]
int PrimitivesCan::Go(double distance, double max_speed, double max_acc){

	EnterCritical();

	int ret;


	//ha folyamatban van valami, amire ezt nem indithatjuk el
	if(bdc->stop_inProgress){
		ret = ACT_ERROR;
	}

	//ha most vegzett
	else if(bdc->move_finished){
		bdc->move_finished = false;
		ret = ACT_FINISHED;
	}

	//ha most nem vegzett, es folyamatban
	else if(bdc->move_inProgress){

		//ha utkozes van
		if(	input->button[INPUT_BUTTON_FRONT_LEFT_INDEX] ||
			input->button[INPUT_BUTTON_FRONT_RIGHT_INDEX] ||
			input->button[INPUT_BUTTON_REAR_LEFT_INDEX] ||
			input->button[INPUT_BUTTON_REAR_RIGHT_INDEX])
				ret = ACT_ERROR;

		//ha nincs utkozes
		else
			ret = ACT_INPROGRESS;

	}

	//ha most nem vegzett, es nincs is folyamatban
	else{
		bdc->BDC_GO(distance, max_speed, max_acc);
		ret = ACT_INPROGRESS;
	}


	ExitCritical();

	return ret;

}


int PrimitivesCan::GoTo(double x, double y, double max_speed, double max_acc){

	EnterCritical();

	int ret;


	//ha folyamatban van valami, amire ezt nem indithatjuk el
	if(bdc->stop_inProgress){
		ret = ACT_ERROR;
	}

	//ha most vegzett
	else if(bdc->move_finished){
		bdc->move_finished = false;
		ret =  ACT_FINISHED;
	}

	//ha most nem vegzett, es folyamatban
	else if(bdc->move_inProgress){

		//ha utkozes van
		if(	input->button[INPUT_BUTTON_FRONT_LEFT_INDEX] ||
			input->button[INPUT_BUTTON_FRONT_RIGHT_INDEX] ||
			input->button[INPUT_BUTTON_REAR_LEFT_INDEX] ||
			input->button[INPUT_BUTTON_REAR_RIGHT_INDEX])
				ret =  ACT_ERROR;

		//ha nincs utkozes
		else
			ret = ACT_INPROGRESS;

	}

	//ha most nem vegzett, es nincs is folyamatban
	else{
		bdc->BDC_GOTO(x-deadreckPosOffsetX, y-deadreckPosOffsetY, max_speed, max_acc);
		ret =  ACT_INPROGRESS;
	}


	ExitCritical();

	return ret;

}


//angle [rad]
//max_speed [rad/s]
//max_acc [rad/s^2]
int PrimitivesCan::Turn(double angle, double max_speed, double max_acc){

	EnterCritical();

	int ret;


	//ha folyamatban van valami, amire ezt nem indithatjuk el
	if(bdc->stop_inProgress){
		ret = ACT_ERROR;
	}

	//ha most vegzett
	else if(bdc->move_finished){
		bdc->move_finished = false;
		ret = ACT_FINISHED;
	}

	//ha most nem vegzett, es folyamatban
	else if(bdc->move_inProgress){

		//ha utkozes van
		if(	input->button[INPUT_BUTTON_FRONT_LEFT_INDEX] ||
			input->button[INPUT_BUTTON_FRONT_RIGHT_INDEX] ||
			input->button[INPUT_BUTTON_REAR_LEFT_INDEX] ||
			input->button[INPUT_BUTTON_REAR_RIGHT_INDEX])
				ret = ACT_ERROR;

		//ha nincs utkozes
		else
			ret = ACT_INPROGRESS;

	}

	//ha most nem vegzett, es nincs is folyamatban
	else{
		bdc->BDC_TURN(angle, max_speed, max_acc);
		ret = ACT_INPROGRESS;
	}


	ExitCritical();

	return ret;

}


//v [mm/s]
//w [rad/s]
int PrimitivesCan::SetSpeed(double v, double w){

	EnterCritical();

	int ret;


	//ha folyamatban van valami, amire ezt nem indithatjuk el
	if(bdc->stop_inProgress){
		ret = ACT_ERROR;
	}

	//ha most vegzett
	else if(bdc->move_finished){
		bdc->move_finished = false;
		ret = ACT_FINISHED;
	}

	//ha most nem vegzett, es folyamatban
	else if(bdc->move_inProgress){

		//ha utkozes van
		if(false)
			ret = ACT_ERROR;

		//ha nincs utkozes
		else
			ret = ACT_INPROGRESS;

	}

	//ha most nem vegzett, es nincs is folyamatban
	else{
		bdc->BDC_SET_SPEED(v, w);
		ret = ACT_INPROGRESS;
	}


	ExitCritical();

	return ret;

}


//dec = 0 => hard stop
//dec [mm/s^2]
int PrimitivesCan::MotionStop(double dec = 0){

	EnterCritical();

	int ret;


	//ha folyamatban van valami, amire ezt nem indithatjuk el
	if(false){
		ret = ACT_ERROR;
	}

	//ha most vegzett
	else if(bdc->stop_finished){
		bdc->stop_finished = false;
		ret = ACT_FINISHED;
	}

	//ha most nem vegzett, es folyamatban
	else if(bdc->stop_inProgress){
		ret = ACT_INPROGRESS;
	}

	//ha most nem vegzett, es nincs is folyamatban
	else{

		if(dec != 0)	bdc->BDC_STOP(dec);
		else			bdc->BDC_HARD_STOP();

		ret = ACT_INPROGRESS;
	}


	ExitCritical();

	return ret;

}


//simulate = true => nem mozog, csak beallitja a kezdopoziciot
int PrimitivesCan::CalibrateDeadreckoning(bool simulate = false){

	EnterCritical();

	int ret;


	double posX, posY, posPhi;


	/**
	 *
	 *       |<------------- 3000 ------------->|
	 *
	 *  -    0------- y -----> -----------------
	 *  ^    |     |                      |     |
	 *  |    |piros|                      | kek |
	 *  |    |-----                        -----|
	 *  |    x                                  |
	 * 2100  |           |-O---|                |
	 *  |    |           |robot|-->             |
	 *  |    v           |-O---|                |
	 *  |    |                                  |
	 *  |    |                                  |
	 *  v    |                                  |
	 *  -     ----------------------------------
	 *
	 */


	double startX, startY, startPhi;
	double onYWallPosX, onXWallPosY;

	if(GetMyColor() == COLOR_RED){

		startX		= DEADRECK_START_DISTANCE_X;
		startY		= DEADRECK_START_DISTANCE_Y;
		startPhi	= M_PI/2;
		onYWallPosX	= ROBOT_DISTANCE_ON_REAR_WALL;
		onXWallPosY	= ROBOT_DISTANCE_ON_REAR_WALL;

	}

	else{

		startX		= DEADRECK_START_DISTANCE_X;
		startY		= AREA_LENGTH_Y - DEADRECK_START_DISTANCE_Y;
		startPhi	= -M_PI/2;
		onYWallPosX	= ROBOT_DISTANCE_ON_REAR_WALL;
		onXWallPosY	= AREA_LENGTH_Y - ROBOT_DISTANCE_ON_REAR_WALL;

	}




	//ha csak szimulacio
	if(simulate){
		SetRobotPos(startX, startY, startPhi);
		ret = ACT_FINISHED;
	}


	//ha nem csak szimulacio
	else{

		switch(deadreckCalibPhase){

			//start
			case 0:
				cout << "CalibrateDeadreckoning START" << endl;
				deadreckCalibPhase++;
				ret = ACT_INPROGRESS;
				break;

			//GoToWall() Y
			case 1:
				//ha rajta vagyunk
				if(GoToWall((-1)*DEADRECK_CALIB_SPEED_ABS, DEADRECK_CALIB_OMEGA_ABS) == ACT_FINISHED){
					cout << "GoToWall() Y READY" << endl;
					GetRobotPos(&posX, &posY, &posPhi);
					SetRobotPos(onYWallPosX, posY, 0);
					deadreckCalibPhase++;
				}
				ret = ACT_INPROGRESS;
				break;

			//eljovunk az Y faltol, beallitjuk a sebesseget
			case 2:
				//ha beallitotta a sebesseget
				if(SetSpeed(DEADRECK_CALIB_SPEED_ABS, 0) == ACT_FINISHED){
					deadreckCalibPhase++;
				}
				ret = ACT_INPROGRESS;
				break;

			//eljovunk az Y faltol
			case 3:
				//ha eljottunk a faltol
				GetRobotPos(&posX, &posY, &posPhi);
				if(posX >= startX){
					deadreckCalibPhase++;
				}
				ret = ACT_INPROGRESS;
				break;

			//megallunk
			case 4:
				//ha megalltunk
				if(MotionStop(0) == ACT_FINISHED){
					deadreckCalibPhase++;
				}
				ret = ACT_INPROGRESS;
				break;

			//fordulunk +/- pi/2 fokot
			case 5:
				//ha elfordultunk
				if(Turn(startPhi, DEADRECK_CALIB_OMEGA_ABS, DEADRECK_CALIB_BETA_ABS) == ACT_FINISHED){
					deadreckCalibPhase++;
				}
				ret = ACT_INPROGRESS;
				break;

			//GoToWall() X
			case 6:
				//ha rajta vagyunk
				if(GoToWall((-1)*DEADRECK_CALIB_SPEED_ABS, DEADRECK_CALIB_OMEGA_ABS) == ACT_FINISHED){
					cout << "GoToWall() X READY" << endl;
					GetRobotPos(&posX, &posY, &posPhi);
					SetRobotPos(posX, onXWallPosY, startPhi);
					deadreckCalibPhase++;
				}
				ret = ACT_INPROGRESS;
				break;

			//eljovunk az X faltol, beallitjuk a sebesseget
			case 7:
				//ha beallitotta a sebesseget
				if(SetSpeed(DEADRECK_CALIB_SPEED_ABS, 0) == ACT_FINISHED){
					deadreckCalibPhase++;
				}
				ret = ACT_INPROGRESS;
				break;

			//eljovunk az Y faltol
			case 8:
				//ha eljottunk a faltol
				GetRobotPos(&posX, &posY, &posPhi);
				if(	((GetMyColor() == COLOR_RED) && (posY >= startY))	||	((GetMyColor() == COLOR_BLUE) && (posY <= startY))	){
					deadreckCalibPhase++;
				}
				ret = ACT_INPROGRESS;
				break;

			//megallunk
			case 9:
				//ha megalltunk
				if(MotionStop(0) == ACT_FINISHED){
					deadreckCalibPhase = 0;
					cout << "CalibrateDeadreckoning READY" << endl;
					ret = ACT_FINISHED;
				}
				//ha meg nem alltunk meg
				else
					ret = ACT_INPROGRESS;
				break;

			//nem lehet, hiba
			default:
				ret = ACT_ERROR;
				break;

		}

	}


	ExitCritical();

	return ret;

}


//motortap be/ki kapcsolasa
int PrimitivesCan::MotorSupply(bool powered){

	EnterCritical();

	int ret;


	//ha most vegzett
	if(power->motor_on_off_finished){
		power->motor_on_off_finished = false;
		ret = ACT_FINISHED;
	}

	//ha most nem vegzet, es folyamatban
	else if(power->motor_on_off_inProgress){
		ret = ACT_INPROGRESS;
	}

	//ha most nem vegzett, es nincs is folyamatban
	else{

		if(powered)	power->POWER_MOTOR_ON();
		else		power->POWER_MOTOR_OFF();

		ret = ACT_INPROGRESS;
	}


	ExitCritical();

	return ret;

}


//pos [deg]
int PrimitivesCan::SetGripperPos(double pos){

	EnterCritical();

	int ret;


	//ha most vegzett mindegyik szervo
	if(servo->move_finished[SERVO_GRIPPER_LEFT_INDEX] && servo->move_finished[SERVO_GRIPPER_RIGHT_INDEX]){
		servo->move_finished[SERVO_GRIPPER_LEFT_INDEX] = false;
		servo->move_finished[SERVO_GRIPPER_RIGHT_INDEX] = false;
		ret = ACT_FINISHED;
	}

	//ha most nem vegzett mindegyik, valamelyik folyamatban
	else if(servo->move_inProgress[SERVO_GRIPPER_LEFT_INDEX] || servo->move_inProgress[SERVO_GRIPPER_RIGHT_INDEX]){
		ret = ACT_INPROGRESS;
	}

	//ha most nem vegzett mindegyik, es nincs is folyamatban semelyik sem
	else{
		servo->SERVO_SET_POS(SERVO_GRIPPER_LEFT_INDEX, pos, SERVO_GRIPPER_MAX_SPEED, SERVO_GRIPPER_MAX_ACC);
		servo->SERVO_SET_POS(SERVO_GRIPPER_RIGHT_INDEX, pos, SERVO_GRIPPER_MAX_SPEED, SERVO_GRIPPER_MAX_ACC);
		ret = ACT_INPROGRESS;
	}


	ExitCritical();

	return ret;

}


//pos [mm], absolute, 0 also pozicio
//speed [mm/s]
//acc [mm/s^2]
int PrimitivesCan::SetConsolePos(double pos, double speed, double acc){

	EnterCritical();

	int ret;


	//ha folyamatban van valami, amire ezt nem indithatjuk el
	if(console->calibrate_inProgress || console->stop_inProgress){
		ret = ACT_ERROR;
	}

	//ha most vegzett
	else if(console->move_finished){
		console->move_finished = false;
		ret = ACT_FINISHED;
	}

	//ha most nem vegzett, es folyamatban
	else if(console->move_inProgress){
		ret = ACT_INPROGRESS;
	}

	//ha most nem vegzett, es nincs is folyamatban
	else{
		console->CONSOLE_SET_POS(pos, speed, acc);
		ret = ACT_INPROGRESS;
	}


	ExitCritical();

	return ret;

}


int PrimitivesCan::CalibrateConsole(void){

	EnterCritical();

	int ret;


	//ha folyamatban van valami, amire ezt nem indithatjuk el
	if(console->move_inProgress || console->stop_inProgress){
		ret = ACT_ERROR;
	}

	//ha most vegzett
	else if(console->calibrate_finished){
		console->calibrate_finished = false;
		ret = ACT_FINISHED;
	}

	//ha most nem vegzett, es folyamatban
	else if(console->calibrate_inProgress){
		ret = ACT_INPROGRESS;
	}

	//ha most nem vegzett, es nincs is folyamatban
	else{
		console->CONSOLE_CALIBRATE();
		ret = ACT_INPROGRESS;
	}


	ExitCritical();

	return ret;

}


int PrimitivesCan::ConsoleStop(void){

	EnterCritical();

	int ret;


	//ha folyamatban van valami, amire ezt nem indithatjuk el
	if(false){
		ret = ACT_ERROR;
	}

	//ha most vegzett
	else if(console->stop_finished){
		console->stop_finished = false;
		ret = ACT_FINISHED;
	}

	//ha most nem vegzett, es folyamatban
	else if(console->stop_inProgress){
		ret = ACT_INPROGRESS;
	}

	//ha most nem vegzett, es nincs is folyamatban
	else{
		console->CONSOLE_STOP();
		ret = ACT_INPROGRESS;
	}


	ExitCritical();

	return ret;

}


//[mm]
double PrimitivesCan::GetConsolePos(void){

	EnterCritical();

	double ret = console->pos;

	ExitCritical();

	return ret;

}


//left [bool]
//pos [deg], 0 alaphelyzet (fuggoleges), lefele no
//speed [deg/s]
//acc [deg/s^2]
int PrimitivesCan::SetArmPos(bool left, double pos, double speed, double acc){

	EnterCritical();


	int ret;
	unsigned int num;


	if(left)
		num = SERVO_LEFT_ARM_INDEX;
	else
		num = SERVO_RIGHT_ARM_INDEX;


	//ha most vegzett
	if(servo->move_finished[num]){
		servo->move_finished[num] = false;
		ret = ACT_FINISHED;
	}

	//ha most nem vegzett, es folyamatban
	else if(servo->move_inProgress[num]){
		ret = ACT_INPROGRESS;
	}

	//ha most nem vegzett, es nincs is folyamatban
	else{
		servo->SERVO_SET_POS(num, pos, speed, acc);
		ret = ACT_INPROGRESS;
	}


	ExitCritical();

	return ret;

}


//left [bool]
//polarity - 1 vonzas
//           0 kikapcsolt
//          -1 taszitas
int PrimitivesCan::Magnet(bool left, int polarity){

	EnterCritical();


	int ret;
	unsigned int num;


	if(left)
		num = MAGNET_LEFT_INDEX;
	else
		num = MAGNET_RIGHT_INDEX;


	//ha most vegzett
	if(magnet->set_polarity_finished[num]){
		magnet->set_polarity_finished[num] = false;
		ret = ACT_FINISHED;
	}

	//ha most nem vegzett, es folyamatban
	else if(magnet->set_polarity_inProgress[num]){
		ret = ACT_INPROGRESS;
	}

	//ha most nem vegzett, es nincs is folyamatban
	else{
		magnet->MAGNET_SET_POLARITY(num, polarity);
		ret = ACT_INPROGRESS;
	}


	ExitCritical();

	return ret;

}


bool PrimitivesCan::GetStartButton(void){

	EnterCritical();

	bool ret =input->button[INPUT_BUTTON_START_INDEX];

	ExitCritical();

	return ret;

}


bool PrimitivesCan::GetStopButton(void){

	EnterCritical();

	bool ret = power->StopButton;

	ExitCritical();

	return ret;

}


bool PrimitivesCan::GetMyColor(void){

	EnterCritical();

	bool ret = input->button[INPUT_BUTTON_COLOR_INDEX];

	ExitCritical();

	return ret;

}


void PrimitivesCan::GetRobotPos(double* x, double* y, double* phi){

	EnterCritical();


	*x		= deadreck->pos_x	+ deadreckPosOffsetX;
	*y		= deadreck->pos_y	+ deadreckPosOffsetY;
	*phi	= deadreck->pos_phi	+ deadreckPosOffsetPhi;

	while(*phi > M_PI)
		*phi -= 2*M_PI;

	while(*phi <= -M_PI)
		*phi += 2*M_PI;


	ExitCritical();

}


// ellenfel poziciojanak lekerdezese
void PrimitivesCan::GetOpponentPos(double* x, double* y){

	EnterCritical();

	*x		= sonar->pos_x;
	*y		= sonar->pos_y;

	ExitCritical();

}


// robot poziciojanak beallitasa
void PrimitivesCan::SetRobotPos(double x, double y, double phi){

	EnterCritical();

	deadreckPosOffsetX		= 	x	- deadreck->pos_x;
	deadreckPosOffsetY		= 	y	- deadreck->pos_y;
	deadreckPosOffsetPhi	= 	phi	- deadreck->pos_phi;

	ExitCritical();

}


// Visszaadja a tavolsagerzekelok erteket mm-ben
void PrimitivesCan::GetDistances(double distance[6]){

	EnterCritical();

	for(unsigned int i=0 ; i<6 ; i++)
		distance[i] = input->distance_mm[i];

	ExitCritical();

}


void PrimitivesCan::evalMsg(UDPmsg* msg){

	//cout << msg->node_id << "\t" << msg->function << "\t" << msg->length << "\t" << msg->data << endl;

	//osszes node-nak odaadjuk kiertekelesre
	//csak az fogja kiertekelni, akinek az azonositojaval megegyezik az msg->node_id
	//ha egy node a valosagban tobb funkciot megvalosit, akkor annak az osszes osztajat meghivjuk kiertekelesre
	gateway->evalMsg(msg);
	console->evalMsg(msg);
	deadreck->evalMsg(msg);
	bdc->evalMsg(msg);
	input->evalMsg(msg);
	magnet->evalMsg(msg);
	servo->evalMsg(msg);
	sonar->evalMsg(msg);
	power->evalMsg(msg);

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


int PrimitivesCan::GoToWall(double speedSigned, double omegaAbs){

	switch(goToWallPhase){

		//start
		case 0:
			goToWallPhase++;
			return ACT_INPROGRESS;

		//beallitjuk a sebesseget, amivel megyunk
		case 1:
			//ha beallitotta a sebesseget
			if(SetSpeed(speedSigned, 0) == ACT_FINISHED)
				goToWallPhase++;
			return ACT_INPROGRESS;

		//varunk az utkozesre
		case 2:
			//ha elorefele megyunk
			if(speedSigned > 0){
				//ha bal utkozes
				if(input->button[INPUT_BUTTON_FRONT_LEFT_INDEX])
					goToWallPhase = 3;
				else if(input->button[INPUT_BUTTON_FRONT_RIGHT_INDEX])
					goToWallPhase = 4;
			}
			//ha hatrafele megyunk
			else{
				//ha bal utkozes
				if(input->button[INPUT_BUTTON_REAR_LEFT_INDEX])
					goToWallPhase = 3;
				else if(input->button[INPUT_BUTTON_REAR_RIGHT_INDEX])
					goToWallPhase = 4;
			}
			return ACT_INPROGRESS;

		//bal utkozes volt, megallunk
		case 3:
			//ha megalltunk
			if(MotionStop(0) == ACT_FINISHED)
				goToWallPhase = 5;
			return ACT_INPROGRESS;

		//jobb utkozes volt, megallunk
		case 4:
			//ha megalltunk
			if(MotionStop(0) == ACT_FINISHED)
				goToWallPhase = 6;
			return ACT_INPROGRESS;

		//bal utkozes volt, megalltunk, rafordulunk a falra
		case 5:
			//ha beallitotta a sebesseget
			if(SetSpeed(speedSigned / 2, omegaAbs) == ACT_FINISHED)
				goToWallPhase = 7;
			return ACT_INPROGRESS;

		//jobb utkozes volt, megalltunk, rafordulunk a falra
		case 6:
			//ha beallitotta a sebesseget
			if(SetSpeed(speedSigned / 2, -omegaAbs) == ACT_FINISHED)
				goToWallPhase = 7;
			return ACT_INPROGRESS;

		//mindket utkozeskapcsolora varunk
		case 7:
			//ha elorefele megyunk
			if(speedSigned > 0){
				//ha mindket utkozeskapcsolo jelez
				if(input->button[INPUT_BUTTON_FRONT_LEFT_INDEX] && input->button[INPUT_BUTTON_FRONT_RIGHT_INDEX])
					goToWallPhase = 8;
			}
			//ha hatrafele megyunk
			else{
				//ha mindket utkozeskapcsolo jelez
				if(input->button[INPUT_BUTTON_REAR_LEFT_INDEX] && input->button[INPUT_BUTTON_REAR_RIGHT_INDEX])
					goToWallPhase = 8;
			}
			return ACT_INPROGRESS;

		//rajta vagyunk a falon, megallunk
		case 8:
			//ha megalltunk, ACT_FINISHED
			if(MotionStop(0) == ACT_FINISHED){
				goToWallPhase = 0;
				return ACT_FINISHED;
			}
			return ACT_INPROGRESS;

		//nem lehet, hiba
		default:
			return ACT_ERROR;

	}

}
