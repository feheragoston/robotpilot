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


void PrimitivesCan::addNodesToCan(void){


	if(!CONSOLE_ON_CANB)	gateway->GATEWAY_ADD_NODE_CANA(CONSOLE_ID);
	else					gateway->GATEWAY_ADD_NODE_CANB(CONSOLE_ID);

	if(!DEADRECK_ON_CANB)	gateway->GATEWAY_ADD_NODE_CANA(DEADRECK_ID);
	else					gateway->GATEWAY_ADD_NODE_CANB(DEADRECK_ID);

	if(!BDC_ON_CANB)		gateway->GATEWAY_ADD_NODE_CANA(BDC_ID);
	else					gateway->GATEWAY_ADD_NODE_CANB(BDC_ID);

	if(!INPUT_ON_CANB)		gateway->GATEWAY_ADD_NODE_CANA(INPUT_ID);
	else					gateway->GATEWAY_ADD_NODE_CANB(INPUT_ID);

	if(!MAGNET_ON_CANB)		gateway->GATEWAY_ADD_NODE_CANA(MAGNET_ID);
	else					gateway->GATEWAY_ADD_NODE_CANB(MAGNET_ID);

	if(!SERVO_ON_CANB)		gateway->GATEWAY_ADD_NODE_CANA(SERVO_ID);
	else					gateway->GATEWAY_ADD_NODE_CANB(SERVO_ID);

	if(!SONAR_ON_CANB)		gateway->GATEWAY_ADD_NODE_CANA(SONAR_ID);
	else					gateway->GATEWAY_ADD_NODE_CANB(SONAR_ID);

	if(!POWER_ON_CANB)		gateway->GATEWAY_ADD_NODE_CANA(POWER_ID);
	else					gateway->GATEWAY_ADD_NODE_CANB(POWER_ID);


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
	if(!initNode(deadreck)	&& INIT_RETURN_FALSE_IF_ERROR)		return false;
	if(!initNode(bdc)		&& INIT_RETURN_FALSE_IF_ERROR)		return false;
	if(!initNode(input)	&& INIT_RETURN_FALSE_IF_ERROR)			return false;
	if(!initNode(magnet)	&& INIT_RETURN_FALSE_IF_ERROR)		return false;
	if(!initNode(servo)	&& INIT_RETURN_FALSE_IF_ERROR)			return false;
	if(!initNode(sonar)	&& INIT_RETURN_FALSE_IF_ERROR)			return false;
	if(!initNode(power)	&& INIT_RETURN_FALSE_IF_ERROR)			return false;

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



	//ha aktuator tap alapbol bekapcsolva
	if(SEND_START_ACTUATOR_IN_INIT)
		broadcast->START_ACTUATOR();



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
		if(bdc->move.done)	ret = ACT_FINISHED;
		else				ret = ACT_ERROR;

		bdc->move.finished = false;

	}

	//ha most nem vegzett, es folyamatban
	else if(bdc->move.inProgress){

		//ha utkozes van
		if(	input->GET_DIGITAL(INPUT_DIGITAL_FRONT_LEFT_LIMIT_SWITCH_INDEX) ||
			input->GET_DIGITAL(INPUT_DIGITAL_FRONT_RIGHT_LIMIT_SWITCH_INDEX))
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
		if(bdc->move.done)	ret = ACT_FINISHED;
		else				ret = ACT_ERROR;

		bdc->move.finished = false;

	}

	//ha most nem vegzett, es folyamatban
	else if(bdc->move.inProgress){

		//ha utkozes van
		if(	input->GET_DIGITAL(INPUT_DIGITAL_FRONT_LEFT_LIMIT_SWITCH_INDEX) ||
			input->GET_DIGITAL(INPUT_DIGITAL_FRONT_RIGHT_LIMIT_SWITCH_INDEX))
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

	int ret = Turn_Unsafe(angle, max_speed, max_acc);

	ExitCritical();

	return ret;

}


//v [mm/s]
//w [rad/s]
int PrimitivesCan::SetSpeed(double v, double w){

	EnterCritical();

	int ret = SetSpeed_Unsafe(v, w);

	ExitCritical();

	return ret;

}


//dec = 0 => hard stop
//dec [mm/s^2]
int PrimitivesCan::MotionStop(double dec = 0){

	EnterCritical();

	int ret = MotionStop_Unsafe(dec);

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

	if(GetMyColor_Unsafe() == COLOR_RED){

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
		SetRobotPos_Unsafe(startX, startY, startPhi);
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
				if((ret = GoToWall(DEADRECK_CALIB_SPEED_ABS, DEADRECK_CALIB_OMEGA_ABS)) == ACT_FINISHED){
					cout << "GoToWall() Y READY" << endl;
					GetRobotPos_Unsafe(&posX, &posY, &posPhi);
					SetRobotPos_Unsafe(onYWallPosX, posY, 0);
					deadreckCalibPhase++;
					ret = ACT_INPROGRESS;
				}
				break;

			//eljovunk az Y faltol, beallitjuk a sebesseget
			case 2:
				//ha beallitotta a sebesseget
				if((ret = SetSpeed_Unsafe((-1)*DEADRECK_CALIB_SPEED_ABS, 0)) == ACT_FINISHED){
					deadreckCalibPhase++;
					ret = ACT_INPROGRESS;
				}
				break;

			//eljovunk az Y faltol
			case 3:
				//ha eljottunk a faltol
				GetRobotPos_Unsafe(&posX, &posY, &posPhi);
				if(posX >= startX){
					deadreckCalibPhase++;
				}
				ret = ACT_INPROGRESS;
				break;

			//megallunk
			case 4:
				//ha megalltunk
				if((ret = MotionStop_Unsafe(0)) == ACT_FINISHED){
					deadreckCalibPhase++;
					ret = ACT_INPROGRESS;
				}
				break;

			//fordulunk +/- pi/2 fokot
			case 5:
				//ha elfordultunk
				if((ret = Turn_Unsafe(startPhi, DEADRECK_CALIB_OMEGA_ABS, DEADRECK_CALIB_BETA_ABS)) == ACT_FINISHED){
					deadreckCalibPhase++;
					ret = ACT_INPROGRESS;
				}
				break;

			//GoToWall() X
			case 6:
				//ha rajta vagyunk
				if((ret = GoToWall(DEADRECK_CALIB_SPEED_ABS, DEADRECK_CALIB_OMEGA_ABS)) == ACT_FINISHED){
					cout << "GoToWall() X READY" << endl;
					GetRobotPos_Unsafe(&posX, &posY, &posPhi);
					SetRobotPos_Unsafe(posX, onXWallPosY, startPhi);
					deadreckCalibPhase++;
					ret = ACT_INPROGRESS;
				}
				break;

			//eljovunk az X faltol, beallitjuk a sebesseget
			case 7:
				//ha beallitotta a sebesseget
				if((ret = SetSpeed_Unsafe((-1)*DEADRECK_CALIB_SPEED_ABS, 0)) == ACT_FINISHED){
					deadreckCalibPhase++;
					ret = ACT_INPROGRESS;
				}
				break;

			//eljovunk az Y faltol
			case 8:
				//ha eljottunk a faltol
				GetRobotPos_Unsafe(&posX, &posY, &posPhi);
				if(	((GetMyColor_Unsafe() == COLOR_RED) && (posY >= startY))	||	((GetMyColor_Unsafe() == COLOR_BLUE) && (posY <= startY))	){
					deadreckCalibPhase++;
				}
				ret = ACT_INPROGRESS;
				break;

			//megallunk
			case 9:
				//ha megalltunk
				if((ret = MotionStop_Unsafe(0)) == ACT_FINISHED){
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
		if(power->act_on_off.done)	ret = ACT_FINISHED;
		else						ret = ACT_ERROR;

		power->act_on_off.finished = false;

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
		if(	servo->move[SERVO_GRIPPER_LEFT_INDEX].done &&
			servo->move[SERVO_GRIPPER_RIGHT_INDEX].done)
			ret = ACT_FINISHED;
		else
			ret = ACT_ERROR;

		servo->move[SERVO_GRIPPER_LEFT_INDEX].finished = false;
		servo->move[SERVO_GRIPPER_RIGHT_INDEX].finished = false;

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
		if(console->move.done)	ret = ACT_FINISHED;
		else					ret = ACT_ERROR;

		console->move.finished = false;

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
		if(console->calibrate.done)	ret = ACT_FINISHED;
		else						ret = ACT_ERROR;

		console->calibrate.finished = false;

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
		if(console->stop.done)	ret = ACT_FINISHED;
		else					ret = ACT_ERROR;

		console->stop.finished = false;

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
		if(servo->move[num].done)	ret = ACT_FINISHED;
		else							ret = ACT_ERROR;

		servo->move[num].finished = false;

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
		if(magnet->set_polarity[num].done)	ret = ACT_FINISHED;
		else								ret = ACT_ERROR;

		magnet->set_polarity[num].finished = false;

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

	bool ret = input->GET_DIGITAL(INPUT_DIGITAL_START_BUTTON_INDEX);

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

	bool ret = GetMyColor_Unsafe();

	ExitCritical();

	return ret;

}


void PrimitivesCan::GetRobotPos(double* x, double* y, double* phi){

	EnterCritical();

	GetRobotPos_Unsafe(x, y, phi);

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

	SetRobotPos_Unsafe(x, y, phi);

	ExitCritical();

}


// Visszaadja a tavolsagerzekelok erteket mm-ben
void PrimitivesCan::GetDistances(double distance[6]){

	EnterCritical();

	for(u8 i=0 ; i<6 ; i++)
		distance[i] = input->GET_DISTANCE(i);

	ExitCritical();

}


void PrimitivesCan::detectActChange(void){

	bool ActOn;

	//ha valtozott az aktuatortap allapota
	if(power->GET_ACT_ON(&ActOn)){

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
	deadreck->evalMsg(msg);
	bdc->evalMsg(msg);
	input->evalMsg(msg);
	magnet->evalMsg(msg);
	servo->evalMsg(msg);
	sonar->evalMsg(msg);
	power->evalMsg(msg);

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
			if(input->GET_DIGITAL(INPUT_DIGITAL_FRONT_LEFT_LIMIT_SWITCH_INDEX))
				goToWallPhase = 3;
			else if(input->GET_DIGITAL(INPUT_DIGITAL_FRONT_RIGHT_LIMIT_SWITCH_INDEX))
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
			if(input->GET_DIGITAL(INPUT_DIGITAL_FRONT_LEFT_LIMIT_SWITCH_INDEX) && input->GET_DIGITAL(INPUT_DIGITAL_FRONT_RIGHT_LIMIT_SWITCH_INDEX))
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


int PrimitivesCan::Turn_Unsafe(double angle, double max_speed, double max_acc){

	int ret;


	//ha folyamatban van valami, amire ezt nem indithatjuk el
	if(bdc->stop.inProgress){
		ret = ACT_ERROR;
	}

	//ha most vegzett
	else if(bdc->move.finished){

		//hiba volt-e
		if(bdc->move.done)	ret = ACT_FINISHED;
		else				ret = ACT_ERROR;

		bdc->move.finished = false;

	}

	//ha most nem vegzett, es folyamatban
	else if(bdc->move.inProgress){

		//ha utkozes van
		if(	input->GET_DIGITAL(INPUT_DIGITAL_FRONT_LEFT_LIMIT_SWITCH_INDEX) ||
			input->GET_DIGITAL(INPUT_DIGITAL_FRONT_RIGHT_LIMIT_SWITCH_INDEX))
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


	return ret;

}


int PrimitivesCan::SetSpeed_Unsafe(double v, double w){

	int ret;


	//ha folyamatban van valami, amire ezt nem indithatjuk el
	if(bdc->stop.inProgress){
		ret = ACT_ERROR;
	}

	//ha most vegzett
	else if(bdc->move.finished){

		//hiba volt-e
		if(bdc->move.done)	ret = ACT_FINISHED;
		else				ret = ACT_ERROR;

		bdc->move.finished = false;

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


	return ret;

}


int PrimitivesCan::MotionStop_Unsafe(double dec){

	int ret;


	//ha folyamatban van valami, amire ezt nem indithatjuk el
	if(false){
		ret = ACT_ERROR;
	}

	//ha most vegzett
	else if(bdc->stop.finished){

		//hiba volt-e
		if(bdc->stop.done)	ret = ACT_FINISHED;
		else				ret = ACT_ERROR;

		bdc->stop.finished = false;

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


	return ret;

}


bool PrimitivesCan::GetMyColor_Unsafe(void){

	bool ret = (input->GET_DIGITAL(INPUT_DIGITAL_COLOR_BUTTON_INDEX) ? COLOR_BLUE : COLOR_RED);

	return ret;

}


void PrimitivesCan::GetRobotPos_Unsafe(double* x, double* y, double* phi){

	double tmpX, tmpY, tmpPhi;

	deadreck->GET_POS(&tmpX, &tmpY, &tmpPhi);

	*x		= tmpX		+ deadreckPosOffsetX;
	*y		= tmpY		+ deadreckPosOffsetY;
	*phi	= tmpPhi	+ deadreckPosOffsetPhi;

	while(*phi > M_PI){
		*phi -= 2*M_PI;
		deadreckPosOffsetPhi -= 2*M_PI;
	}

	while(*phi < -M_PI){
		*phi += 2*M_PI;
		deadreckPosOffsetPhi += 2*M_PI;
	}

}


void PrimitivesCan::SetRobotPos_Unsafe(double x, double y, double phi){

	double tmpX, tmpY, tmpPhi;

	deadreck->GET_POS(&tmpX, &tmpY, &tmpPhi);

	deadreckPosOffsetX		= 	x	- tmpX;
	deadreckPosOffsetY		= 	y	- tmpY;
	deadreckPosOffsetPhi	= 	phi	- tmpPhi;

}
