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

	deadreckCheckX		= 0;
	deadreckCheckY		= 0;
	deadreckCheckPhi	= 0;

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

	//SONAR-nak nem kuldunk semmit, pontosabban GATEWAY-nek nem adunk senkit a CAN_B-re, így oda tuti nem kuld, de fogadni fog
	//if(!SONAR_ON_CANB)		gateway->GATEWAY_ADD_NODE_CANA(SONAR_ID);
	//else					gateway->GATEWAY_ADD_NODE_CANB(SONAR_ID);

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
	if(!initNode(input)		&& INIT_RETURN_FALSE_IF_ERROR)		return false;
	if(!initNode(magnet)	&& INIT_RETURN_FALSE_IF_ERROR)		return false;
	if(!initNode(servo)		&& INIT_RETURN_FALSE_IF_ERROR)		return false;
	if(!initNode(power)		&& INIT_RETURN_FALSE_IF_ERROR)		return false;
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
	if(SEND_START_ACTUATOR_IN_INIT)		broadcast->START_ACTUATOR();
	else								broadcast->STOP_ACTUATOR();



	//minden OK
	return true;


}


PrimitivesCan::~PrimitivesCan(){

}


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
	if(	bdc->AnyStop.inProgress ||
		bdc->GoTo.inProgress ||
		bdc->Turn.inProgress ||
		bdc->SetSpeed.inProgress){
		ret = ACT_ERROR;
	}

	//ha most vegzett
	else if(bdc->Go.finished){

		//hiba volt-e
		if(bdc->Go.done)	ret = ACT_FINISHED;
		else				ret = ACT_ERROR;

		bdc->Go.finished = false;

	}

	//ha most nem vegzett, es folyamatban
	else if(bdc->Go.inProgress){

		//ha lejart a megbeszelt timeout
		if(readyMoveTO()){
			bdc->Go.inProgress = false;
			ret = ACT_ERROR;
		}

		//ha utkozes van
		else if(	input->GET_DIGITAL(INPUT_DIGITAL_FRONT_LEFT_LIMIT_SWITCH_INDEX) ||
					input->GET_DIGITAL(INPUT_DIGITAL_FRONT_RIGHT_LIMIT_SWITCH_INDEX))
				ret = ACT_ERROR;

		//ha nincs utkozes
		else
			ret = ACT_INPROGRESS;

	}

	//ha most nem vegzett, es nincs is folyamatban
	else{
		bdc->BDC_GO(distance, max_speed, max_acc);
		setMoveTO(distance, max_speed, max_acc);	//megbeszelt TO
		ret = ACT_INPROGRESS;
	}


	ExitCritical();

	return ret;

}


int PrimitivesCan::GoTo(double x, double y, double max_speed, double max_acc){

	EnterCritical();

	int ret;
	double xr, yr, phir;
	double xw, yw, phiw;


	//ha folyamatban van valami, amire ezt nem indithatjuk el
	if(	bdc->AnyStop.inProgress ||
		bdc->Go.inProgress ||
		bdc->Turn.inProgress ||
		bdc->SetSpeed.inProgress){
		ret = ACT_ERROR;
	}

	//ha most vegzett
	else if(bdc->GoTo.finished){

		//hiba volt-e
		if(bdc->GoTo.done)	ret = ACT_FINISHED;
		else				ret = ACT_ERROR;

		bdc->GoTo.finished = false;

	}

	//ha most nem vegzett, es folyamatban
	else if(bdc->GoTo.inProgress){

		//ha lejart a megbeszelt timeout
		if(readyMoveTO()){
			bdc->GoTo.inProgress = false;
			ret = ACT_ERROR;
		}

		//ha utkozes van
		else if(	input->GET_DIGITAL(INPUT_DIGITAL_FRONT_LEFT_LIMIT_SWITCH_INDEX) ||
					input->GET_DIGITAL(INPUT_DIGITAL_FRONT_RIGHT_LIMIT_SWITCH_INDEX))
				ret =  ACT_ERROR;

		//ha nincs utkozes
		else
			ret = ACT_INPROGRESS;

	}

	//ha most nem vegzett, es nincs is folyamatban
	else{
		ConvWorldToRobot(x, y, 0, &xr, &yr, &phir);
		bdc->BDC_GOTO(xr, yr, max_speed, max_acc);
		GetRobotPos_Unsafe(&xw, &yw, &phiw);
		setMoveTO(sqrt((x-xw)*(x-xw) + (y-yw)*(y-yw)), max_speed, max_acc);	//megbeszelt TO
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


int PrimitivesCan::DeadreckoningResetPos_Unsafe(void){

	int ret;


	//ha most vegzett
	if(deadreck->ResetPos.finished){

		//hiba volt-e
		if(deadreck->ResetPos.done)		ret = ACT_FINISHED;
		else							ret = ACT_ERROR;

		deadreck->ResetPos.finished = false;

	}

	//ha most nem vegzett, es folyamatban
	else if(deadreck->ResetPos.inProgress){
		ret = ACT_INPROGRESS;
	}

	//ha most nem vegzett, es nincs is folyamatban
	else{
		deadreck->DEADRECK_RESET_POS();
		ret =  ACT_INPROGRESS;
	}


	return ret;

}


void PrimitivesCan::Calibrate_Unsafe(void){

	//red
	if(GetMyColor_Unsafe() == COLOR_RED){
		deadreckCheckX		= DEADRECK_CALIB_DISTANCE_X;
		deadreckCheckY		= DEADRECK_CALIB_DISTANCE_Y;
		deadreckCheckPhi	= M_PI/2 - DEADRECK_CALIB_PHI;
		TM33Build(SONAR_TX_RED, SONAR_TY_RED, SONAR_ALPHA_RED, T33WS);
	}

	//blue
	else{
		deadreckCheckX		= DEADRECK_CALIB_DISTANCE_X;
		deadreckCheckY		= AREA_LENGTH_Y - DEADRECK_CALIB_DISTANCE_Y;
		deadreckCheckPhi	= -M_PI/2 + DEADRECK_CALIB_PHI;
		TM33Build(SONAR_TX_BLUE, SONAR_TY_BLUE, SONAR_ALPHA_BLUE, T33WS);
	}

}



//simulate = true => nem mozog, csak beallitja a kezdopoziciot
int PrimitivesCan::CalibrateDeadreckoning(bool simulate = false){

	EnterCritical();

	int ret;


	//ha csak szimulacio
	if(simulate){
		Calibrate_Unsafe();
		ret = ACT_FINISHED;
	}


	//ha nem csak szimulacio
	else{

		switch(deadreckCalibPhase){

			//start
			case 0:
				deadreckCalibPhase = 1;
				ret = ACT_INPROGRESS;
				break;

			//varunk az egyik kapcsolora
			case 1:

				//ha megvan valamelyik utkozeskapcsolo
				if(	input->GET_DIGITAL(INPUT_DIGITAL_FRONT_LEFT_LIMIT_SWITCH_INDEX) ||
					input->GET_DIGITAL(INPUT_DIGITAL_FRONT_RIGHT_LIMIT_SWITCH_INDEX))
					deadreckCalibPhase = 2;

				ret = ACT_INPROGRESS;

				break;

			//varunk a reset-re
			case 2:

				//ha megvolt a reset
				if((ret = DeadreckoningResetPos_Unsafe()) == ACT_FINISHED){
					cout << "BENN" << endl;
					Calibrate_Unsafe();
					deadreckCalibPhase = 0;
				}

				break;


		}


	}


	//cout << "dp\t" << deadreckCalibPhase << endl;


	//ha hiba
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
	if(power->ActOnOff.finished){

		//hiba volt-e
		if(power->ActOnOff.done)	ret = ACT_FINISHED;
		else						ret = ACT_ERROR;

		power->ActOnOff.finished = false;

	}

	//ha most nem vegzet, es folyamatban
	else if(power->ActOnOff.inProgress){
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
	if(servo->Setpos[SERVO_GRIPPER_LEFT_INDEX].finished && servo->Setpos[SERVO_GRIPPER_RIGHT_INDEX].finished){

		//hiba volt-e
		if(	servo->Setpos[SERVO_GRIPPER_LEFT_INDEX].done &&
			servo->Setpos[SERVO_GRIPPER_RIGHT_INDEX].done){
			//lementjuk Totinak a poziciot
			gripperPos = pos;
			ret = ACT_FINISHED;
		}
		else
			ret = ACT_ERROR;

		servo->Setpos[SERVO_GRIPPER_LEFT_INDEX].finished = false;
		servo->Setpos[SERVO_GRIPPER_RIGHT_INDEX].finished = false;

	}

	//ha most nem vegzett mindegyik, valamelyik folyamatban
	else if(	servo->Setpos[SERVO_GRIPPER_LEFT_INDEX].inProgress ||
				servo->Setpos[SERVO_GRIPPER_RIGHT_INDEX].inProgress){
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
	if(console->Calibrate.inProgress || console->Stop.inProgress){
		ret = ACT_ERROR;
	}

	//ha most vegzett
	else if(console->SetPos.finished){

		//hiba volt-e
		if(console->SetPos.done)	ret = ACT_FINISHED;
		else						ret = ACT_ERROR;

		console->SetPos.finished = false;

	}

	//ha most nem vegzett, es folyamatban
	else if(console->SetPos.inProgress){
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
	if(console->SetPos.inProgress || console->Stop.inProgress){
		ret = ACT_ERROR;
	}

	//ha most vegzett
	else if(console->Calibrate.finished){

		//hiba volt-e
		if(console->Calibrate.done)	ret = ACT_FINISHED;
		else						ret = ACT_ERROR;

		console->Calibrate.finished = false;

	}

	//ha most nem vegzett, es folyamatban
	else if(console->Calibrate.inProgress){
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
	else if(console->Stop.finished){

		//hiba volt-e
		if(console->Stop.done)	ret = ACT_FINISHED;
		else					ret = ACT_ERROR;

		console->Stop.finished = false;

	}

	//ha most nem vegzett, es folyamatban
	else if(console->Stop.inProgress){
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
	if(servo->Setpos[num].finished){

		//hiba volt-e
		if(servo->Setpos[num].done){
			//lementjuk Totinak a poziciot
			if(left)	leftArmPos = pos;
			else		rightArmPos = pos;
			ret = ACT_FINISHED;
		}
		else
			ret = ACT_ERROR;

		servo->Setpos[num].finished = false;

	}

	//ha most nem vegzett, es folyamatban
	else if(servo->Setpos[num].inProgress){
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
	if(magnet->SetPolarity[num].finished){

		//hiba volt-e
		if(magnet->SetPolarity[num].done)	ret = ACT_FINISHED;
		else								ret = ACT_ERROR;

		magnet->SetPolarity[num].finished = false;

	}

	//ha most nem vegzett, es folyamatban
	else if(magnet->SetPolarity[num].inProgress){
		ret = ACT_INPROGRESS;
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


bool PrimitivesCan::PawnInGripper(void){

	EnterCritical();

	bool ret = input->GET_DIGITAL(INPUT_DIGITAL_PAWN_IN_GRIPPER_INDEX);

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

	double sonarpos_S[3];	// Szonar pozicioadat
	double sonarpos_W[3];	// Szonar pozicioadat vilagkoordinatakban

	sonar->GET_POS((&sonarpos_S[0]), (&sonarpos_S[1]));
	sonarpos_S[2] = 1.0;

	M33V3Mult(T33WS, sonarpos_S, sonarpos_W);

	*x = sonarpos_W[0];
	*y = sonarpos_W[1];

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


bool PrimitivesCan::GetMotorSupply(void){

	EnterCritical();

	bool ret;

	power->GET_ACT_ON(&ret);

	ExitCritical();

	return ret;

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


void PrimitivesCan::ConvRobotToWorld(double xr, double yr, double phir, double* xw, double* yw, double* phiw){

	//elforgatom, majd eltolom
	*xw = deadreckCheckX + xr * cos(deadreckCheckPhi) - yr * sin(deadreckCheckPhi);
	*yw = deadreckCheckY + xr * sin(deadreckCheckPhi) + yr * cos(deadreckCheckPhi);
	*phiw = deadreckCheckPhi + phir;

	while(*phiw > M_PI)
		*phiw -= 2*M_PI;

	while(*phiw <= -M_PI)
		*phiw += 2*M_PI;

}


void PrimitivesCan::ConvWorldToRobot(double xw, double yw, double phiw, double* xr, double* yr, double* phir){

	//eltolom, majd elforgatom
	*xr = (xw - deadreckCheckX) * cos(-deadreckCheckPhi) - (yw - deadreckCheckY) * sin(-deadreckCheckPhi);
	*yr = (xw - deadreckCheckX) * sin(-deadreckCheckPhi) + (yw - deadreckCheckY) * cos(-deadreckCheckPhi);
	*phir = phiw - deadreckCheckPhi;

	while(*phir > M_PI)
		*phir -= 2*M_PI;

	while(*phir <= -M_PI)
		*phir += 2*M_PI;

}


int PrimitivesCan::GoToWall(double speedSigned, double omegaAbs){

	int ret;

	switch(goToWallPhase){

		//start
		case 0:
			goToWallPhase++;
			ret = ACT_INPROGRESS;
			break;

		//beallitjuk a sebesseget, amivel megyunk
		case 1:
			//ha beallitotta a sebesseget
			if((ret = SetSpeed_Unsafe(speedSigned, 0)) == ACT_FINISHED){
				goToWallPhase++;
				ret = ACT_INPROGRESS;
			}
			break;

		//varunk az utkozesre
		case 2:
			//ha bal utkozes
			if(input->GET_DIGITAL(INPUT_DIGITAL_FRONT_LEFT_LIMIT_SWITCH_INDEX))
				goToWallPhase = 3;
			else if(input->GET_DIGITAL(INPUT_DIGITAL_FRONT_RIGHT_LIMIT_SWITCH_INDEX))
				goToWallPhase = 4;
			ret = ACT_INPROGRESS;
			break;

		//bal utkozes volt, megallunk
		case 3:
			//ha megalltunk
			if((ret = MotionStop_Unsafe(0)) == ACT_FINISHED){
				goToWallPhase = 5;
				ret = ACT_INPROGRESS;
			}
			break;

		//jobb utkozes volt, megallunk
		case 4:
			//ha megalltunk
			if((ret = MotionStop_Unsafe(0)) == ACT_FINISHED){
				goToWallPhase = 6;
				ret = ACT_INPROGRESS;
			}
			break;

		//bal utkozes volt, megalltunk, rafordulunk a falra
		case 5:
			//ha beallitotta a sebesseget
			if((ret = SetSpeed_Unsafe(speedSigned / 2, omegaAbs)) == ACT_FINISHED){
				goToWallPhase = 7;
				ret = ACT_INPROGRESS;
			}
			break;

		//jobb utkozes volt, megalltunk, rafordulunk a falra
		case 6:
			//ha beallitotta a sebesseget
			if((ret = SetSpeed_Unsafe(speedSigned / 2, -omegaAbs)) == ACT_FINISHED){
				goToWallPhase = 7;
				ret = ACT_INPROGRESS;
			}
			break;

		//mindket utkozeskapcsolora varunk
		case 7:
			//ha mindket utkozeskapcsolo jelez
			if(input->GET_DIGITAL(INPUT_DIGITAL_FRONT_LEFT_LIMIT_SWITCH_INDEX) && input->GET_DIGITAL(INPUT_DIGITAL_FRONT_RIGHT_LIMIT_SWITCH_INDEX))
				goToWallPhase = 8;
			ret = ACT_INPROGRESS;
			break;

		//rajta vagyunk a falon, megallunk
		case 8:
			//ha megalltunk, ACT_FINISHED
			if((ret = MotionStop_Unsafe(0)) == ACT_FINISHED){
				goToWallPhase = 0;
				ret = ACT_FINISHED;
			}
			break;

		//nem lehet, hiba
		default:
			ret = ACT_ERROR;
			break;

	}


	cout << "gp\t" << goToWallPhase << endl;


	//ha hiba
	if(ret == ACT_ERROR)
		goToWallPhase = 0;


	return ret;

}


int PrimitivesCan::Turn_Unsafe(double angle, double max_speed, double max_acc){

	int ret;


	//ha folyamatban van valami, amire ezt nem indithatjuk el
	if(	bdc->AnyStop.inProgress ||
		bdc->Go.inProgress ||
		bdc->GoTo.inProgress ||
		bdc->SetSpeed.inProgress){
		ret = ACT_ERROR;
	}

	//ha most vegzett
	else if(bdc->Turn.finished){

		//hiba volt-e
		if(bdc->Turn.done)	ret = ACT_FINISHED;
		else				ret = ACT_ERROR;

		bdc->Turn.finished = false;

	}

	//ha most nem vegzett, es folyamatban
	else if(bdc->Turn.inProgress){

		//ha lejart a megbeszelt timeout
		if(readyMoveTO()){
			bdc->Turn.inProgress = false;
			ret = ACT_ERROR;
		}

		//ha utkozes van
		else if(	input->GET_DIGITAL(INPUT_DIGITAL_FRONT_LEFT_LIMIT_SWITCH_INDEX) ||
					input->GET_DIGITAL(INPUT_DIGITAL_FRONT_RIGHT_LIMIT_SWITCH_INDEX))
				ret = ACT_ERROR;

		//ha nincs utkozes
		else
			ret = ACT_INPROGRESS;

	}

	//ha most nem vegzett, es nincs is folyamatban
	else{
		bdc->BDC_TURN(angle, max_speed, max_acc);
		setMoveTO(angle, max_speed, max_acc);	//megbeszelt TO
		ret = ACT_INPROGRESS;
	}


	return ret;

}


int PrimitivesCan::SetSpeed_Unsafe(double v, double w){

	int ret;


	//ha folyamatban van valami, amire ezt nem indithatjuk el
	if(	bdc->AnyStop.inProgress ||
		bdc->Go.inProgress ||
		bdc->GoTo.inProgress ||
		bdc->Turn.inProgress){
		ret = ACT_ERROR;
	}

	//ha most vegzett
	else if(bdc->SetSpeed.finished){

		//hiba volt-e
		if(bdc->SetSpeed.done)	ret = ACT_FINISHED;
		else					ret = ACT_ERROR;

		bdc->SetSpeed.finished = false;

	}

	//ha most nem vegzett, es folyamatban
	else if(bdc->SetSpeed.inProgress){

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
	else if(bdc->AnyStop.finished){

		//hiba volt-e
		if(bdc->AnyStop.done)	ret = ACT_FINISHED;
		else					ret = ACT_ERROR;

		bdc->AnyStop.finished = false;

	}

	//ha most nem vegzett, es folyamatban
	else if(bdc->AnyStop.inProgress){
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

	double xr, yr, phir;

	deadreck->GET_POS(&xr, &yr, &phir);

	ConvRobotToWorld(xr, yr, phir, x, y, phi);

}


void PrimitivesCan::SetRobotPos_Unsafe(double x, double y, double phi){

	double tmpX, tmpY, tmpPhi;

	GetRobotPos_Unsafe(&tmpX, &tmpY, &tmpPhi);

	deadreckCheckX		+= 	x	- tmpX;
	deadreckCheckY		+= 	y	- tmpY;
	deadreckCheckPhi	+= 	phi	- tmpPhi;

}


void PrimitivesCan::setMoveTO(double s, double v, double a){

	double t;

	/*
		﻿Ha s_kiadott <= v_max^2/a_max

		akkor háromszög
		t = 2*sqrt(s_kiadott/a_max)

		egyébként trapéz
		t = v_max/a_max + s_kiadott/v_max
	 */

	s = fabs(s);
	a = a/3;

	if(s <= v*v / a)
		t = 2 * sqrt(s/a);
	else
		t = v/a + s/v;

	u32 tu = (u32)(t*1000000 * 1.2);	//felulbecsuljuk

	//u32 tu = (u32)5000000;

	moveTOsec	= tu / 1000000;
	moveTOusec	= tu % 1000000;

	gettimeofday(&moveStart, NULL);

}

bool PrimitivesCan::readyMoveTO(void){

	struct timeval elapsed;

	TimeMeasure(&moveStart, &elapsed);

	//ha lejart a megbeszelt timeout
	if((elapsed.tv_sec > moveTOsec) || ((elapsed.tv_sec == moveTOsec) && (elapsed.tv_usec > moveTOusec))){
		cout << "--- readyMoveTO ---" << endl;
		return true;
	}

	return false;

}


///////////////////////////////////////////////////////////////////
void PrimitivesCan::TimeMeasure(struct timeval *time_start, struct timeval *time_elapsed) {
			// Eltelt ido meresere szolgalo fuggveny.
			// Megmeri, hogy mennyi ido telt el time_start ota,
			// es beleirja time_elapsed-be.
			// Legegyszerubb hasznalata a kovetkezo:
			//
			//	struct timeval time_start, time_elapsed;
			//	gettimeofday(&time_start, NULL) {
			//  TimeMeasure(&time_start, &time_elapsed) {
			//

	struct timeval	time_now;
	struct timeval	time_temp;

	time_temp.tv_sec = time_start->tv_sec;
	time_temp.tv_usec = time_start->tv_usec;

	gettimeofday(&time_now, NULL);


	// Perform the carry for the later subtraction by updating time_start.
	if (time_now.tv_usec < time_temp.tv_usec) {
	int nsec = (time_temp.tv_usec - time_temp.tv_usec) / 1000000 + 1;
	time_temp.tv_usec -= 1000000 * nsec;
	time_temp.tv_sec += nsec;
	}
	if (time_now.tv_usec - time_temp.tv_usec > 1000000) {
	int nsec = (time_now.tv_usec - time_temp.tv_usec) / 1000000;
	time_temp.tv_usec += 1000000 * nsec;
	time_temp.tv_sec -= nsec;
	}

	// Compute the time remaining to wait. tv_usec is certainly positive.
	time_elapsed->tv_sec = time_now.tv_sec - time_temp.tv_sec;
	time_elapsed->tv_usec = time_now.tv_usec - time_temp.tv_usec;
}
