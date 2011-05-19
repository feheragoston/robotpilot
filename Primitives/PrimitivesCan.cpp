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
	bdc			= new node_BDC();
	input		= new node_Input();
	magnet		= new node_Magnet();
	servo		= new node_Servo();
	sonar		= new node_Sonar();
	power		= new node_Power();
	//---------- node VEGE ----------


}


bool PrimitivesCan::init_node(node* node){

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
	if(!init_node(gateway)	&& INIT_RETURN_FALSE_IF_ERROR)		return false;
	if(!init_node(console)	&& INIT_RETURN_FALSE_IF_ERROR)		return false;
	if(!init_node(deadreck)	&& INIT_RETURN_FALSE_IF_ERROR)		return false;
	if(!init_node(bdc)		&& INIT_RETURN_FALSE_IF_ERROR)		return false;
	if(!init_node(input)	&& INIT_RETURN_FALSE_IF_ERROR)		return false;
	if(!init_node(magnet)	&& INIT_RETURN_FALSE_IF_ERROR)		return false;
	if(!init_node(servo)	&& INIT_RETURN_FALSE_IF_ERROR)		return false;
	if(!init_node(sonar)	&& INIT_RETURN_FALSE_IF_ERROR)		return false;
	if(!init_node(power)	&& INIT_RETURN_FALSE_IF_ERROR)		return false;

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


	//minden OK
	return true;


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

	EnterCritical();

	int ret;


	//ha folyamatban van valami, amire ezt nem indithatjuk el
	if(bdc->stop.inProgress){
		ret = ACT_ERROR;
	}

	//ha most vegzett
	else if(bdc->move.finished){

		//hiba volt-e
		if(bdc->move.error)	ret = ACT_ERROR;
		else				ret = ACT_FINISHED;

		bdc->move.finished = false;
		bdc->move.error = false;

	}

	//ha most nem vegzett, es folyamatban
	else if(bdc->move.inProgress){

		//ha utkozes van
		if(	input->GET_DIGITAL(INPUT_BUTTON_REAR_LEFT_INDEX) ||
			input->GET_DIGITAL(INPUT_BUTTON_REAR_RIGHT_INDEX))
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
	if(bdc->stop.inProgress){
		ret = ACT_ERROR;
	}

	//ha most vegzett
	else if(bdc->move.finished){

		//hiba volt-e
		if(bdc->move.error)	ret = ACT_ERROR;
		else				ret = ACT_FINISHED;

		bdc->move.finished = false;
		bdc->move.error = false;

	}

	//ha most nem vegzett, es folyamatban
	else if(bdc->move.inProgress){

		//ha utkozes van
		if(	input->GET_DIGITAL(INPUT_BUTTON_REAR_LEFT_INDEX) ||
			input->GET_DIGITAL(INPUT_BUTTON_REAR_RIGHT_INDEX))
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
	if(bdc->stop.inProgress){
		ret = ACT_ERROR;
	}

	//ha most vegzett
	else if(bdc->move.finished){

		//hiba volt-e
		if(bdc->move.error)	ret = ACT_ERROR;
		else				ret = ACT_FINISHED;

		bdc->move.finished = false;
		bdc->move.error = false;

	}

	//ha most nem vegzett, es folyamatban
	else if(bdc->move.inProgress){

		//ha utkozes van
		if(	input->GET_DIGITAL(INPUT_BUTTON_REAR_LEFT_INDEX) ||
			input->GET_DIGITAL(INPUT_BUTTON_REAR_RIGHT_INDEX))
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
	if(bdc->stop.inProgress){
		ret = ACT_ERROR;
	}

	//ha most vegzett
	else if(bdc->move.finished){

		//hiba volt-e
		if(bdc->move.error)	ret = ACT_ERROR;
		else				ret = ACT_FINISHED;

		bdc->move.finished = false;
		bdc->move.error = false;

	}

	//ha most nem vegzett, es folyamatban
	else if(bdc->move.inProgress){

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
	else if(bdc->stop.finished){

		//hiba volt-e
		if(bdc->stop.error)	ret = ACT_ERROR;
		else				ret = ACT_FINISHED;

		bdc->stop.finished = false;
		bdc->stop.error = false;

	}

	//ha most nem vegzett, es folyamatban
	else if(bdc->stop.inProgress){
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
				if((ret = GoToWall((-1)*DEADRECK_CALIB_SPEED_ABS, DEADRECK_CALIB_OMEGA_ABS)) == ACT_FINISHED){
					cout << "GoToWall() Y READY" << endl;
					GetRobotPos(&posX, &posY, &posPhi);
					SetRobotPos(onYWallPosX, posY, 0);
					deadreckCalibPhase++;
					ret = ACT_INPROGRESS;
				}
				break;

			//eljovunk az Y faltol, beallitjuk a sebesseget
			case 2:
				//ha beallitotta a sebesseget
				if((ret = SetSpeed(DEADRECK_CALIB_SPEED_ABS, 0)) == ACT_FINISHED){
					deadreckCalibPhase++;
					ret = ACT_INPROGRESS;
				}
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
				if((ret = MotionStop(0)) == ACT_FINISHED){
					deadreckCalibPhase++;
					ret = ACT_INPROGRESS;
				}
				break;

			//fordulunk +/- pi/2 fokot
			case 5:
				//ha elfordultunk
				if((ret = Turn(startPhi, DEADRECK_CALIB_OMEGA_ABS, DEADRECK_CALIB_BETA_ABS)) == ACT_FINISHED){
					deadreckCalibPhase++;
					ret = ACT_INPROGRESS;
				}
				break;

			//GoToWall() X
			case 6:
				//ha rajta vagyunk
				if((ret = GoToWall((-1)*DEADRECK_CALIB_SPEED_ABS, DEADRECK_CALIB_OMEGA_ABS)) == ACT_FINISHED){
					cout << "GoToWall() X READY" << endl;
					GetRobotPos(&posX, &posY, &posPhi);
					SetRobotPos(posX, onXWallPosY, startPhi);
					deadreckCalibPhase++;
					ret = ACT_INPROGRESS;
				}
				break;

			//eljovunk az X faltol, beallitjuk a sebesseget
			case 7:
				//ha beallitotta a sebesseget
				if((ret = SetSpeed(DEADRECK_CALIB_SPEED_ABS, 0)) == ACT_FINISHED){
					deadreckCalibPhase++;
					ret = ACT_INPROGRESS;
				}
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
				if((ret = MotionStop(0)) == ACT_FINISHED){
					deadreckCalibPhase = 0;
					cout << "CalibrateDeadreckoning READY" << endl;
					ret = ACT_FINISHED;
				}
				break;

			//nem lehet, hiba
			default:
				ret = ACT_ERROR;
				break;

		}

	}


	//ha hiba, alapallapotba
	if(ret == ACT_ERROR)
		deadreckCalibPhase = 0;


	ExitCritical();

	return ret;

}


//motortap be/ki kapcsolasa
int PrimitivesCan::MotorSupply(bool powered){

	EnterCritical();

	int ret;


	//ha most vegzett
	if(power->act_on_off.finished){

		//hiba volt-e
		if(power->act_on_off.error)	ret = ACT_ERROR;
		else						ret = ACT_FINISHED;

		power->act_on_off.finished = false;
		power->act_on_off.error = false;

	}

	//ha most nem vegzet, es folyamatban
	else if(power->act_on_off.inProgress){
		ret = ACT_INPROGRESS;
	}

	//ha most nem vegzett, es nincs is folyamatban
	else{

		if(powered)	power->POWER_ACT_ON();
		else		power->POWER_ACT_OFF();

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
	if(servo->move[SERVO_GRIPPER_LEFT_INDEX].finished && servo->move[SERVO_GRIPPER_RIGHT_INDEX].finished){

		//hiba volt-e
		if(	servo->move[SERVO_GRIPPER_LEFT_INDEX].error ||
			servo->move[SERVO_GRIPPER_RIGHT_INDEX].error)
			ret = ACT_ERROR;
		else
			ret = ACT_FINISHED;

		servo->move[SERVO_GRIPPER_LEFT_INDEX].finished = false;
		servo->move[SERVO_GRIPPER_RIGHT_INDEX].finished = false;

		servo->move[SERVO_GRIPPER_LEFT_INDEX].error = false;
		servo->move[SERVO_GRIPPER_RIGHT_INDEX].error = false;

	}

	//ha most nem vegzett mindegyik, valamelyik folyamatban
	else if(servo->move[SERVO_GRIPPER_LEFT_INDEX].inProgress || servo->move[SERVO_GRIPPER_RIGHT_INDEX].inProgress){
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
	if(console->calibrate.inProgress || console->stop.inProgress){
		ret = ACT_ERROR;
	}

	//ha most vegzett
	else if(console->move.finished){

		//hiba volt-e
		if(console->move.error)	ret = ACT_ERROR;
		else					ret = ACT_FINISHED;

		console->move.finished = false;
		console->move.error = false;

	}

	//ha most nem vegzett, es folyamatban
	else if(console->move.inProgress){
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
	if(console->move.inProgress || console->stop.inProgress){
		ret = ACT_ERROR;
	}

	//ha most vegzett
	else if(console->calibrate.finished){

		//hiba volt-e
		if(console->calibrate.error)	ret = ACT_ERROR;
		else							ret = ACT_FINISHED;

		console->calibrate.finished = false;
		console->calibrate.error = false;

	}

	//ha most nem vegzett, es folyamatban
	else if(console->calibrate.inProgress){
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
	else if(console->stop.finished){

		//hiba volt-e
		if(console->stop.error)	ret = ACT_ERROR;
		else					ret = ACT_FINISHED;

		console->stop.finished = false;
		console->stop.error = false;

	}

	//ha most nem vegzett, es folyamatban
	else if(console->stop.inProgress){
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

	double ret = console->GET_POS();

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
	u8 num = (left ? SERVO_LEFT_ARM_INDEX : SERVO_RIGHT_ARM_INDEX);


	//ha most vegzett
	if(servo->move[num].finished){

		//hiba volt-e
		if(servo->move[num].error)	ret = ACT_ERROR;
		else						ret = ACT_FINISHED;

		servo->move[num].finished = false;
		servo->move[num].error = false;

	}

	//ha most nem vegzett, es folyamatban
	else if(servo->move[num].inProgress){
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
	u8 num = (left ? MAGNET_LEFT_INDEX : MAGNET_RIGHT_INDEX);


	//ha most vegzett
	if(magnet->set_polarity[num].finished){
		magnet->set_polarity[num].finished = false;
		ret = ACT_FINISHED;
	}

	//ha most nem vegzett, es folyamatban
	else if(magnet->set_polarity[num].inProgress){

		//hiba volt-e
		if(magnet->set_polarity[num].error)	ret = ACT_ERROR;
		else								ret = ACT_FINISHED;

		magnet->set_polarity[num].finished = false;
		magnet->set_polarity[num].error = false;

	}

	//ha most nem vegzett, es nincs is folyamatban
	else{
		if(polarity == 1)		magnet->MAGNET_SET_POLARITY(num, MAGNET_PULL);
		else if(polarity == 0)	magnet->MAGNET_SET_POLARITY(num, MAGNET_OFF);
		else if(polarity == -1)	magnet->MAGNET_SET_POLARITY(num, MAGNET_PUSH);
		ret = ACT_INPROGRESS;
	}


	ExitCritical();

	return ret;

}


bool PrimitivesCan::GetStartButton(void){

	EnterCritical();

	bool ret = input->GET_DIGITAL(INPUT_BUTTON_START_INDEX);

	ExitCritical();

	return ret;

}


bool PrimitivesCan::GetStopButton(void){

	EnterCritical();

	bool ret = power->GET_STOP_BUTTON();

	ExitCritical();

	return ret;

}


bool PrimitivesCan::GetMyColor(void){

	EnterCritical();

	//active	active_is_red		color
	//
	//0			0					red
	//0			1					blue
	//1			0					blue
	//1			1					red
	bool is_red = input->GET_DIGITAL(INPUT_BUTTON_COLOR_INDEX) ^ INPUT_BUTTON_COLOR_IS_ACTIVE_RED;
	bool ret = (is_red ? COLOR_RED : COLOR_BLUE);

	ExitCritical();

	return ret;

}


void PrimitivesCan::GetRobotPos(double* x, double* y, double* phi){

	EnterCritical();

	double tmpX, tmpY, tmpPhi;

	deadreck->GET_POS(&tmpX, &tmpY, &tmpPhi);

	*x		= tmpX		+ deadreckPosOffsetX;
	*y		= tmpY		+ deadreckPosOffsetY;
	*phi	= tmpPhi	+ deadreckPosOffsetPhi;

	while(*phi > M_PI)
		*phi -= 2*M_PI;

	while(*phi <= -M_PI)
		*phi += 2*M_PI;


	ExitCritical();

}


// ellenfel poziciojanak lekerdezese
void PrimitivesCan::GetOpponentPos(double* x, double* y){

	EnterCritical();

	sonar->GET_POS(x, y);

	ExitCritical();

}


// robot poziciojanak beallitasa
void PrimitivesCan::SetRobotPos(double x, double y, double phi){

	EnterCritical();

	double tmpX, tmpY, tmpPhi;

	deadreck->GET_POS(&tmpX, &tmpY, &tmpPhi);

	deadreckPosOffsetX		= 	x	- tmpX;
	deadreckPosOffsetY		= 	y	- tmpY;
	deadreckPosOffsetPhi	= 	phi	- tmpPhi;

	ExitCritical();

}


// Visszaadja a tavolsagerzekelok erteket mm-ben
void PrimitivesCan::GetDistances(double distance[6]){

	EnterCritical();

	for(u8 i=0 ; i<6 ; i++)
		distance[i] = input->GET_DISTANCE(i);

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

	int ret;

	switch(goToWallPhase){

		//start
		case 0:
			goToWallPhase++;
			ret = ACT_INPROGRESS;

		//beallitjuk a sebesseget, amivel megyunk
		case 1:
			//ha beallitotta a sebesseget
			if((ret = SetSpeed(speedSigned, 0)) == ACT_FINISHED){
				goToWallPhase++;
				ret = ACT_INPROGRESS;
			}

		//varunk az utkozesre
		case 2:
			//ha bal utkozes
			if(input->GET_DIGITAL(INPUT_BUTTON_REAR_LEFT_INDEX))
				goToWallPhase = 3;
			else if(input->GET_DIGITAL(INPUT_BUTTON_REAR_RIGHT_INDEX))
				goToWallPhase = 4;
			ret = ACT_INPROGRESS;

		//bal utkozes volt, megallunk
		case 3:
			//ha megalltunk
			if((ret = MotionStop(0)) == ACT_FINISHED){
				goToWallPhase = 5;
				ret = ACT_INPROGRESS;
			}

		//jobb utkozes volt, megallunk
		case 4:
			//ha megalltunk
			if((ret = MotionStop(0)) == ACT_FINISHED){
				goToWallPhase = 6;
				ret = ACT_INPROGRESS;
			}

		//bal utkozes volt, megalltunk, rafordulunk a falra
		case 5:
			//ha beallitotta a sebesseget
			if((ret = SetSpeed(speedSigned / 2, omegaAbs)) == ACT_FINISHED){
				goToWallPhase = 7;
				ret = ACT_INPROGRESS;
			}

		//jobb utkozes volt, megalltunk, rafordulunk a falra
		case 6:
			//ha beallitotta a sebesseget
			if((ret = SetSpeed(speedSigned / 2, -omegaAbs)) == ACT_FINISHED){
				goToWallPhase = 7;
				ret = ACT_INPROGRESS;
			}

		//mindket utkozeskapcsolora varunk
		case 7:
			//ha mindket utkozeskapcsolo jelez
			if(input->GET_DIGITAL(INPUT_BUTTON_REAR_LEFT_INDEX) && input->GET_DIGITAL(INPUT_BUTTON_REAR_RIGHT_INDEX))
				goToWallPhase = 8;
			ret = ACT_INPROGRESS;

		//rajta vagyunk a falon, megallunk
		case 8:
			//ha megalltunk, ACT_FINISHED
			if((ret = MotionStop(0)) == ACT_FINISHED){
				goToWallPhase = 0;
				ret = ACT_FINISHED;
			}

		//nem lehet, hiba
		default:
			ret = ACT_ERROR;

	}


	//ha hiba
	if(ret == ACT_ERROR)
		goToWallPhase = 0;


	return ret;

}
