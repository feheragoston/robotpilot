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

			//EXIT CRITICAL
			pc->ExitCritical();

			pc->signalNewMsg();

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


#ifdef	ROBOTPILOT
PrimitivesCan::PrimitivesCan(Config* config) : Primitives(config){
#else
PrimitivesCan::PrimitivesCan(){
#endif

	#ifdef	ROBOTPILOT
		UDPconn::init(config->CanIp);
	#else
		UDPconn::init("");
	#endif

	UDPdriver::init();


	//---------- valtozo ELEJE ----------
	deadreckPosOffsetX		= 0;
	deadreckPosOffsetY		= 0;
	deadreckPosOffsetPhi	= 0;

	deadreckCalibPhase		= 0;
	goToWallPhase			= 0;
	//---------- valtozo VEGE ----------


	//---------- node ELEJE ----------
	gateway		= new node_Gateway();
	broadcast	= new node_Broadcast();

	console		= new node_Console();
	deadreck	= new node_Deadreck();
	drive		= new node_Drive();
	input		= new node_Input();
	magnet		= new node_Magnet();
	servo		= new node_Servo();
	sonar		= new node_Sonar();
	supply		= new node_Supply();
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
	gateway->ADD_NODE_CANA(CONSOLE_ID);
	gateway->ADD_NODE_CANA(DEADRECK_ID);
	gateway->ADD_NODE_CANA(DRIVE_ID);
	gateway->ADD_NODE_CANA(INPUT_ID);
	gateway->ADD_NODE_CANA(MAGNET_ID);
	gateway->ADD_NODE_CANA(SERVO_ID);
	gateway->ADD_NODE_CANA(SONAR_ID);
	gateway->ADD_NODE_CANA(SUPPLY_ID);
	//---------- add node VEGE ----------


	//---------- node PINGprocess ELEJE ----------
	console->PINGprocess();
	deadreck->PINGprocess();
	drive->PINGprocess();
	input->PINGprocess();
	magnet->PINGprocess();
	servo->PINGprocess();
	sonar->PINGprocess();
	supply->PINGprocess();
	//---------- node PINGprocess VEGE ----------


	//---------- SET_KEEP_ALIVE_MS() ELEJE ----------
	gateway->SET_KEEP_ALIVE_MS(1000);
	broadcast->SET_KEEP_ALIVE_MS(1000);
	//---------- SET_KEEP_ALIVE_MS() VEGE ----------


	//---------- SET_SEND_PERIOD_MS() ELEJE ----------
	gateway->SET_SEND_PERIOD_MS(100);
	broadcast->SET_SEND_PERIOD_MS(100);
	//---------- SET_SEND_PERIOD_MS() VEGE ----------


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

	//ha folyamatban van valami, amire ezt nem indithatjuk el
	if(drive->stop_inProgress || drive->goto_inProgress || drive->turn_inProgress || drive->setspeed_inProgress){
		return ACT_ERROR;
	}

	//ha most vegzett
	else if(drive->go_finished){
		drive->go_finished = false;
		return ACT_FINISHED;
	}

	//ha most nem vegzett, es folyamatban
	else if(drive->go_inProgress){

		//ha utkozes van
		if(input->button[BUTTON_FRONT_LEFT_INDEX] || input->button[BUTTON_FRONT_RIGHT_INDEX] || input->button[BUTTON_REAR_LEFT_INDEX] || input->button[BUTTON_REAR_RIGHT_INDEX])
			return ACT_ERROR;

		return ACT_INPROGRESS;

	}

	//ha most nem vegzett, es nincs is folyamatban
	else{
		drive->GO(distance, max_speed, max_acc);
		return ACT_INPROGRESS;
	}

}


int PrimitivesCan::GoTo(double x, double y, double max_speed, double max_acc){

	//ha folyamatban van valami, amire ezt nem indithatjuk el
	if(drive->stop_inProgress || drive->go_inProgress || drive->turn_inProgress || drive->setspeed_inProgress){
		return ACT_ERROR;
	}

	//ha most vegzett
	else if(drive->goto_finished){
		drive->goto_finished = false;
		return ACT_FINISHED;
	}

	//ha most nem vegzett, es folyamatban
	else if(drive->goto_inProgress){

		//ha utkozes van
		if(input->button[BUTTON_FRONT_LEFT_INDEX] || input->button[BUTTON_FRONT_RIGHT_INDEX] || input->button[BUTTON_REAR_LEFT_INDEX] || input->button[BUTTON_REAR_RIGHT_INDEX])
			return ACT_ERROR;

		return ACT_INPROGRESS;

	}

	//ha most nem vegzett, es nincs is folyamatban
	else{
		drive->GOTO(x-deadreckPosOffsetX, y-deadreckPosOffsetY, max_speed, max_acc);
		return ACT_INPROGRESS;
	}

}


//angle [rad]
//max_speed [rad/s]
//max_acc [rad/s^2]
int PrimitivesCan::Turn(double angle, double max_speed, double max_acc){

	//ha folyamatban van valami, amire ezt nem indithatjuk el
	if(drive->stop_inProgress || drive->go_inProgress || drive->goto_inProgress || drive->setspeed_inProgress){
		return ACT_ERROR;
	}

	//ha most vegzett
	else if(drive->turn_finished){
		drive->turn_finished = false;
		return ACT_FINISHED;
	}

	//ha most nem vegzett, es folyamatban
	else if(drive->turn_inProgress){

		//ha utkozes van
		if(input->button[BUTTON_FRONT_LEFT_INDEX] || input->button[BUTTON_FRONT_RIGHT_INDEX] || input->button[BUTTON_REAR_LEFT_INDEX] || input->button[BUTTON_REAR_RIGHT_INDEX])
			return ACT_ERROR;

		return ACT_INPROGRESS;

	}

	//ha most nem vegzett, es nincs is folyamatban
	else{
		drive->TURN(angle, max_speed, max_acc);
		return ACT_INPROGRESS;
	}

}


//v [mm/s]
//w [rad/s]
int PrimitivesCan::SetSpeed(double v, double w){

	//ha folyamatban van valami, amire ezt nem indithatjuk el
	if(drive->stop_inProgress || drive->go_inProgress || drive->goto_inProgress || drive->turn_inProgress){
		return ACT_ERROR;
	}

	//ha most vegzett
	else if(drive->setspeed_finished){
		drive->setspeed_finished = false;
		return ACT_FINISHED;
	}

	//ha most nem vegzett, es folyamatban
	else if(drive->setspeed_inProgress){

		//ha utkozes van
		if(false)
			return ACT_ERROR;

		return ACT_INPROGRESS;

	}

	//ha most nem vegzett, es nincs is folyamatban
	else{
		drive->SET_SPEED(v, w);
		return ACT_INPROGRESS;
	}

}


//dec = 0 => hard stop
//dec [mm/s^2]
int PrimitivesCan::MotionStop(double dec = 0){

	//ha folyamatban van valami, amire ezt nem indithatjuk el
	if(false){
		return ACT_ERROR;
	}

	//ha most vegzett
	else if(drive->stop_finished){
		drive->stop_finished = false;
		return ACT_FINISHED;
	}

	//ha most nem vegzett, es folyamatban
	else if(drive->stop_inProgress){
		return ACT_INPROGRESS;
	}

	//ha most nem vegzett, es nincs is folyamatban
	else{
		drive->STOP(dec);
		return ACT_INPROGRESS;
	}

}


//simulate = true => nem mozog, csak beallitja a kezdopoziciot
int PrimitivesCan::CalibrateDeadreckoning(bool simulate = false){

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

		startX		= deadreckStartDistanceX;
		startY		= deadreckStartDistanceY;
		startPhi	= M_PI/2;
		onYWallPosX	= onRearWallRobotDistance;
		onXWallPosY	= onRearWallRobotDistance;

	}

	else{

		startX		= deadreckStartDistanceX;
		startY		= areaLengthY - deadreckStartDistanceY;
		startPhi	= -M_PI/2;
		onYWallPosX	= onRearWallRobotDistance;
		onXWallPosY	= areaLengthY - onRearWallRobotDistance;

	}




	//ha csak szimulacio
	if(simulate){
		SetRobotPos(startX, startY, startPhi);
		return ACT_FINISHED;
	}


	//ha nem csak szimulacio
	else{

		switch(deadreckCalibPhase){

			//start
			case 0:
				cout << "CalibrateDeadreckoning START" << endl;
				deadreckCalibPhase++;
				return ACT_INPROGRESS;

			//GoToWall() Y
			case 1:
				//ha rajta vagyunk
				if(GoToWall((-1)*deadreckCalibSpeedAbs, deadreckCalibOmegaAbs) == ACT_FINISHED){
					cout << "GoToWall() Y READY" << endl;
					GetRobotPos(&posX, &posY, &posPhi);
					SetRobotPos(onYWallPosX, posY, 0);
					deadreckCalibPhase++;
				}
				return ACT_INPROGRESS;

			//eljovunk az Y faltol, beallitjuk a sebesseget
			case 2:
				//ha beallitotta a sebesseget
				if(SetSpeed(deadreckCalibSpeedAbs, 0) == ACT_FINISHED){
					deadreckCalibPhase++;
				}
				return ACT_INPROGRESS;

			//eljovunk az Y faltol
			case 3:
				//ha eljottunk a faltol
				GetRobotPos(&posX, &posY, &posPhi);
				if(posX >= startX){
					deadreckCalibPhase++;
				}
				return ACT_INPROGRESS;

			//megallunk
			case 4:
				//ha megalltunk
				if(MotionStop(0) == ACT_FINISHED){
					deadreckCalibPhase++;
				}
				return ACT_INPROGRESS;

			//fordulunk +/- pi/2 fokot
			case 5:
				//ha elfordultunk
				if(Turn(startPhi, deadreckCalibOmegaAbs, deadreckCalibBetaAbs) == ACT_FINISHED){
					deadreckCalibPhase++;
				}
				return ACT_INPROGRESS;

			//GoToWall() X
			case 6:
				//ha rajta vagyunk
				if(GoToWall((-1)*deadreckCalibSpeedAbs, deadreckCalibOmegaAbs) == ACT_FINISHED){
					cout << "GoToWall() X READY" << endl;
					GetRobotPos(&posX, &posY, &posPhi);
					SetRobotPos(posX, onXWallPosY, startPhi);
					deadreckCalibPhase++;
				}
				return ACT_INPROGRESS;

			//eljovunk az X faltol, beallitjuk a sebesseget
			case 7:
				//ha beallitotta a sebesseget
				if(SetSpeed(deadreckCalibSpeedAbs, 0) == ACT_FINISHED){
					deadreckCalibPhase++;
				}
				return ACT_INPROGRESS;

			//eljovunk az Y faltol
			case 8:
				//ha eljottunk a faltol
				GetRobotPos(&posX, &posY, &posPhi);
				if(	((GetMyColor() == COLOR_RED) && (posY >= startY))	||	((GetMyColor() == COLOR_BLUE) && (posY <= startY))	){
					deadreckCalibPhase++;
				}
				return ACT_INPROGRESS;

			//megallunk
			case 9:
				//ha megalltunk
				if(MotionStop(0) == ACT_FINISHED){
					deadreckCalibPhase = 0;
					cout << "CalibrateDeadreckoning READY" << endl;
					return ACT_FINISHED;
				}
				return ACT_INPROGRESS;

			//nem lehet, hiba
			default:
				return ACT_ERROR;

		}

	}

}


//motortap be/ki kapcsolasa
int PrimitivesCan::MotorSupply(bool powered){

	//ha most vegzett
	if(supply->motor_finished){
		supply->motor_finished = false;
		return ACT_FINISHED;
	}

	//ha most nem vegzet, es folyamatban
	else if(supply->motor_inProgress){
		return ACT_INPROGRESS;
	}

	//ha most nem vegzett, es nincs is folyamatban
	else{
		supply->MOTOR(powered);
		return ACT_INPROGRESS;
	}

}


//pos [deg]
int PrimitivesCan::SetGripperPos(double pos){

	//ha most vegzett mindegyik szervo
	if(servo->setservo_finished[SERVO_GRIPPER_LEFT_INDEX] && servo->setservo_finished[SERVO_GRIPPER_RIGHT_INDEX]){
		servo->setservo_finished[SERVO_GRIPPER_LEFT_INDEX] = false;
		servo->setservo_finished[SERVO_GRIPPER_RIGHT_INDEX] = false;
		return ACT_FINISHED;
	}

	//ha most nem vegzett mindegyik, valamelyik folyamatban
	else if(servo->setservo_inProgress[SERVO_GRIPPER_LEFT_INDEX] || servo->setservo_inProgress[SERVO_GRIPPER_RIGHT_INDEX]){
		return ACT_INPROGRESS;
	}

	//ha most nem vegzett mindegyik, es nincs is folyamatban semelyik sem
	else{
		servo->SET_SERVO(SERVO_GRIPPER_LEFT_INDEX, pos, 999999999, 999999999);	//nagy sebesseg es gyorsulas megengedett
		servo->SET_SERVO(SERVO_GRIPPER_RIGHT_INDEX, pos, 999999999, 999999999);	//nagy sebesseg es gyorsulas megengedett
		return ACT_INPROGRESS;
	}

}


//pos [mm], absolute, 0 also pozicio
//speed [mm/s]
//acc [mm/s^2]
int PrimitivesCan::SetConsolePos(double pos, double speed, double acc){

	//ha folyamatban van valami, amire ezt nem indithatjuk el
	if(console->calibrate_inProgress || console->stop_inProgress){
		return ACT_ERROR;
	}

	//ha most vegzett
	else if(console->setpos_finished){
		console->setpos_finished = false;
		return ACT_FINISHED;
	}

	//ha most nem vegzett, es folyamatban
	else if(console->setpos_inProgress){
		return ACT_INPROGRESS;
	}

	//ha most nem vegzett, es nincs is folyamatban
	else{
		console->SET_POS(pos, speed, acc);
		return ACT_INPROGRESS;
	}

}


int PrimitivesCan::CalibrateConsole(void){

	//ha folyamatban van valami, amire ezt nem indithatjuk el
	if(console->setpos_inProgress || console->stop_inProgress){
		return ACT_ERROR;
	}

	//ha most vegzett
	else if(console->calibrate_finished){
		console->calibrate_finished = false;
		return ACT_FINISHED;
	}

	//ha most nem vegzett, es folyamatban
	else if(console->calibrate_inProgress){
		return ACT_INPROGRESS;
	}

	//ha most nem vegzett, es nincs is folyamatban
	else{
		console->CALIBRATE();
		return ACT_INPROGRESS;
	}

}


int PrimitivesCan::ConsoleStop(void){

	//ha folyamatban van valami, amire ezt nem indithatjuk el
	if(false){
		return ACT_ERROR;
	}

	//ha most vegzett
	else if(console->stop_finished){
		console->stop_finished = false;
		return ACT_FINISHED;
	}

	//ha most nem vegzett, es folyamatban
	else if(console->stop_inProgress){
		return ACT_INPROGRESS;
	}

	//ha most nem vegzett, es nincs is folyamatban
	else{
		console->STOP();
		return ACT_INPROGRESS;
	}

}


//[mm]
double PrimitivesCan::GetConsolePos(void){

	return console->pos;

}


//left [bool]
//pos [deg], 0 alaphelyzet (fuggoleges), lefele no
//speed [deg/s]
//acc [deg/s^2]
int PrimitivesCan::SetArmPos(bool left, double pos, double speed, double acc){

	unsigned int servoNum;

	if(left)
		servoNum = SERVO_LEFT_ARM_INDEX;
	else
		servoNum = SERVO_RIGHT_ARM_INDEX;


	//ha most vegzett
	if(servo->setservo_finished[servoNum]){
		servo->setservo_finished[servoNum] = false;
		return ACT_FINISHED;
	}

	//ha most nem vegzett, es folyamatban
	else if(servo->setservo_inProgress[servoNum]){
		return ACT_INPROGRESS;
	}

	//ha most nem vegzett, es nincs is folyamatban
	else{
		servo->SET_SERVO(servoNum, pos, speed, acc);
		return ACT_INPROGRESS;
	}

}


//left [bool]
//polarity - 1 vonzas
//           0 kikapcsolt
//          -1 taszitas
int PrimitivesCan::Magnet(bool left, int polarity){

	unsigned int magnetNum;

	if(left)
		magnetNum = MAGNET_LEFT_INDEX;
	else
		magnetNum = MAGNET_RIGHT_INDEX;


	//ha most vegzett
	if(magnet->setmagnet_finished[magnetNum]){
		magnet->setmagnet_finished[magnetNum] = false;
		return ACT_FINISHED;
	}

	//ha most nem vegzett, es folyamatban
	else if(magnet->setmagnet_inProgress[magnetNum]){
		return ACT_INPROGRESS;
	}

	//ha most nem vegzett, es nincs is folyamatban
	else{
		magnet->SET_MAGNET(magnetNum, polarity);
		return ACT_INPROGRESS;
	}

}


bool PrimitivesCan::GetStartButton(void){

	return input->button[BUTTON_START_INDEX];

}


bool PrimitivesCan::GetStopButton(void){

	return supply->StopButton;

}


bool PrimitivesCan::GetMyColor(void){

	return input->button[BUTTON_COLOR_INDEX];

}


void PrimitivesCan::GetRobotPos(double* x, double* y, double* phi){

	*x		= deadreck->pos_x	+ deadreckPosOffsetX;
	*y		= deadreck->pos_y	+ deadreckPosOffsetY;
	*phi	= deadreck->pos_phi	+ deadreckPosOffsetPhi;

	while(*phi > M_PI)
		*phi -= 2*M_PI;

	while(*phi <= -M_PI)
		*phi += 2*M_PI;

}


// ellenfel poziciojanak lekerdezese
void PrimitivesCan::GetOpponentPos(double* x, double* y){

	*x		= sonar->pos_x;
	*y		= sonar->pos_y;

}


// robot poziciojanak beallitasa
void PrimitivesCan::SetRobotPos(double x, double y, double phi){

	deadreckPosOffsetX		= 	x	- deadreck->pos_x;
	deadreckPosOffsetY		= 	y	- deadreck->pos_y;
	deadreckPosOffsetPhi	= 	phi	- deadreck->pos_phi;

}


// Visszaadja a tavolsagerzekelok erteket mm-ben
void PrimitivesCan::GetDistances(double distance[6]){

	for(unsigned int i=0 ; i<6 ; i++)
		distance[i] = input->distance_mm[i];

}


void PrimitivesCan::evalMsg(UDPmsg* msg){

	//cout << msg->node_id << "\t" << msg->function << "\t" << msg->length << "\t" << msg->data << endl;

	//osszes node-nak odaadjuk kiertekelesre
	//csak az fogja kiertekelni, akinek az azonositojaval megegyezik az msg->node_id
	//ha egy node a valosagban tobb funkciot megvalosit, akkor annak az osszes osztajat meghivjuk kiertekelesre
	gateway->evalMsg(msg);
	console->evalMsg(msg);
	deadreck->evalMsg(msg);
	drive->evalMsg(msg);
	input->evalMsg(msg);
	magnet->evalMsg(msg);
	servo->evalMsg(msg);
	sonar->evalMsg(msg);
	supply->evalMsg(msg);

}


void PrimitivesCan::signalNewMsg(void){

	sem_post(&newMessageSemaphore);

}


void PrimitivesCan::KEEP_ALIVE_SEND(void){

	gateway->KEEP_ALIVE();
	broadcast->KEEP_ALIVE();

}


void PrimitivesCan::KEEP_ALIVE_SLEEP(void){

	usleep(keepAlivePeriod_ms * 1000);

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
				if(input->button[BUTTON_FRONT_LEFT_INDEX])
					goToWallPhase = 3;
				else if(input->button[BUTTON_FRONT_RIGHT_INDEX])
					goToWallPhase = 4;
			}
			//ha hatrafele megyunk
			else{
				//ha bal utkozes
				if(input->button[BUTTON_REAR_LEFT_INDEX])
					goToWallPhase = 3;
				else if(input->button[BUTTON_REAR_RIGHT_INDEX])
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
				if(input->button[BUTTON_FRONT_LEFT_INDEX] && input->button[BUTTON_FRONT_RIGHT_INDEX])
					goToWallPhase = 8;
			}
			//ha hatrafele megyunk
			else{
				//ha mindket utkozeskapcsolo jelez
				if(input->button[BUTTON_REAR_LEFT_INDEX] && input->button[BUTTON_REAR_RIGHT_INDEX])
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
