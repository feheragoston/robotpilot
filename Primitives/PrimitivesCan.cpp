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

	bdcMotionError		= false;

	deadreckCheckX		= 0;
	deadreckCheckY		= 0;
	deadreckCheckPhi	= 0;

	sonarXOffset		= 0;
	sonarYOffset		= 0;

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
	if(START_ACTUATOR_IN_INIT)		broadcast->START_ACTUATOR();
	else							broadcast->STOP_ACTUATOR();



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


bool PrimitivesCan::PawnInGripper(void){

	EnterCritical();

	bool ret = input->GET_DIGITAL(INPUT_DIGITAL_PAWN_IN_GRIPPER_INDEX);

	ExitCritical();

	return ret;

}


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

			//ha megvan valamelyik utkozeskapcsolo
			if(	input->GET_DIGITAL(INPUT_DIGITAL_FRONT_LEFT_LIMIT_SWITCH_INDEX) ||
				input->GET_DIGITAL(INPUT_DIGITAL_FRONT_RIGHT_LIMIT_SWITCH_INDEX))
				deadreckCalibPhase = 2;

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
	if(	bdc->AnyStop.inProgress ||
		bdc->AnyMotion.inProgress ||
		bdc->SetSpeed.inProgress){
		ret = ACT_START_ERROR;
	}

	//ha elindithatjuk
	else{
		bdc->BDC_SET_SPEED(v, w);
		ret = ACT_STARTED;
	}


	ExitCritical();

	return ret;

}


bool PrimitivesCan::SetSpeedInProgress(void){

	EnterCritical();

	bool ret = bdc->SetSpeed.inProgress;

	ExitCritical();

	return ret;

}


bool PrimitivesCan::Go(double distance, double max_speed, double max_acc){

	EnterCritical();

	bool ret;


	//ha folyamatban van valami, amire ezt nem indithatjuk el
	if(	bdc->AnyStop.inProgress ||
		bdc->AnyMotion.inProgress ||
		bdc->SetSpeed.inProgress){
		ret = ACT_START_ERROR;
	}

	//ha elindithatjuk
	else{
		bdc->BDC_GO(distance, max_speed, max_acc);
		bdcMotionError = MOTION_NO_ERROR;	//nincs hiba
		setMoveTO(distance, max_speed, max_acc);	//megbeszelt TO
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
	if(	bdc->AnyStop.inProgress ||
		bdc->AnyMotion.inProgress ||
		bdc->SetSpeed.inProgress){
		ret = ACT_START_ERROR;
	}

	//ha elindithatjuk
	else{
		ConvWorldToRobot(x, y, 0, &xr, &yr, &phir);
		bdc->BDC_GOTO(xr, yr, max_speed, max_acc);
		bdcMotionError = MOTION_NO_ERROR;	//nincs hiba
		GetRobotPos_Unsafe(&xw, &yw, &phiw);
		setMoveTO(sqrt((x-xw)*(x-xw) + (y-yw)*(y-yw)), max_speed, max_acc);	//megbeszelt TO
		ret = ACT_STARTED;
	}


	ExitCritical();

	return ret;

}


bool PrimitivesCan::Turn(double angle, double max_speed, double max_acc){

	EnterCritical();

	bool ret;


	//ha folyamatban van valami, amire ezt nem indithatjuk el
	if(	bdc->AnyStop.inProgress ||
		bdc->AnyMotion.inProgress ||
		bdc->SetSpeed.inProgress){
		ret = ACT_START_ERROR;
	}

	//ha elindithatjuk
	else{
		bdc->BDC_TURN(angle, max_speed, max_acc);
		bdcMotionError = MOTION_NO_ERROR;	//nincs hiba
		setMoveTO(angle, max_speed, max_acc);	//megbeszelt TO
		ret = ACT_STARTED;
	}


	ExitCritical();

	return ret;

}


bool PrimitivesCan::MotionInProgress(void){

	EnterCritical();

	//ha lejart a megbeszelt timeout
	if(readyMoveTO()){
		bdc->AnyMotion.inProgress = false;
		bdcMotionError = MOTION_ERROR;
	}

	bool ret = bdc->AnyMotion.inProgress;

	//ha utkozes kovetkezett be
	if(ret && (	input->GET_DIGITAL(INPUT_DIGITAL_FRONT_LEFT_LIMIT_SWITCH_INDEX) ||
				input->GET_DIGITAL(INPUT_DIGITAL_FRONT_RIGHT_LIMIT_SWITCH_INDEX)))
		bdcMotionError = MOTION_ERROR;

	ExitCritical();

	return ret;

}


int PrimitivesCan::GetMotionError(void){

	EnterCritical();

	int ret = (bdcMotionError ? MOTION_ERROR : MOTION_NO_ERROR);

	ExitCritical();

	return ret;

}


bool PrimitivesCan::MotionStop(double dec){

	EnterCritical();

	bool ret;


	//ha folyamatban van valami, amire ezt nem indithatjuk el
	if(	bdc->AnyStop.inProgress){
		ret = ACT_START_ERROR;
	}

	//ha elindithatjuk
	else{

		//MotionStop hivasnal az osszes tobbi mozgast leallitjuk
		bdc->AnyMotion.inProgress = false;

		if(dec != 0)	bdc->BDC_STOP(dec);
		else			bdc->BDC_HARD_STOP();

		ret = ACT_STARTED;
	}


	ExitCritical();

	return ret;

}


bool PrimitivesCan::MotionStopInProgress(void){

	EnterCritical();

	bool ret = bdc->AnyStop.inProgress;

	ExitCritical();

	return ret;

}


void PrimitivesCan::GetRobotPos(double* x, double* y, double* phi){

	EnterCritical();

	GetRobotPos_Unsafe(x, y, phi);

	ExitCritical();

}


long int PrimitivesCan::GetOpponentPos(double * x, double* y){

	EnterCritical();

	long int ret = GetOpponentPos_Unsafe(x, y);

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


void PrimitivesCan::SetOpponentPos(double x, double y){

	EnterCritical();

	double tmpX, tmpY;

	GetOpponentPos_Unsafe(&tmpX, &tmpY);

	sonarXOffset = x - tmpX;
	sonarYOffset = y - tmpY;

	ExitCritical();

}


void PrimitivesCan::GetDistances(double distance[6]){

	EnterCritical();

	for(u8 i=0 ; i<6 ; i++)
		distance[i] = input->GET_DISTANCE(i);

	ExitCritical();

}


bool PrimitivesCan::GripperMove(double pos){

	EnterCritical();

	bool ret;


	//ha folyamatban van valami, amire ezt nem indithatjuk el
	if(	servo->Setpos[SERVO_GRIPPER_LEFT_INDEX].inProgress ||
		servo->Setpos[SERVO_GRIPPER_RIGHT_INDEX].inProgress){
		ret = ACT_START_ERROR;
	}

	//ha elindithatjuk
	else{
		servo->SERVO_SET_POS(SERVO_GRIPPER_LEFT_INDEX, pos, SERVO_GRIPPER_MAX_SPEED, SERVO_GRIPPER_MAX_ACC);
		servo->SERVO_SET_POS(SERVO_GRIPPER_RIGHT_INDEX, pos, SERVO_GRIPPER_MAX_SPEED, SERVO_GRIPPER_MAX_ACC);
		ret = ACT_STARTED;
	}


	ExitCritical();

	return ret;

}


bool PrimitivesCan::GripperMoveInProgress(void){

	EnterCritical();

	bool ret =	servo->Setpos[SERVO_GRIPPER_LEFT_INDEX].inProgress ||
				servo->Setpos[SERVO_GRIPPER_RIGHT_INDEX].inProgress;

	ExitCritical();

	return ret;

}


double PrimitivesCan::GetGripperPos(void){

	EnterCritical();

	double ret = (servo->GET_POS(SERVO_GRIPPER_LEFT_INDEX) + servo->GET_POS(SERVO_GRIPPER_RIGHT_INDEX)) / 2;

	ExitCritical();

	return ret;

}


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
	if(	console->Calibrate.inProgress ||
		console->SetPos.inProgress ||
		console->Stop.inProgress){
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


bool PrimitivesCan::ArmMove(bool left, double pos, double max_speed, double max_acc){

	EnterCritical();

	bool ret;
	u8 num = (left ? SERVO_LEFT_ARM_INDEX : SERVO_RIGHT_ARM_INDEX);


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


bool PrimitivesCan::ArmMoveInProgress(bool left){

	EnterCritical();

	u8 num = (left ? SERVO_LEFT_ARM_INDEX : SERVO_RIGHT_ARM_INDEX);
	bool ret = servo->Setpos[num].inProgress;

	ExitCritical();

	return ret;

}


double PrimitivesCan::GetArmPos(bool left){

	EnterCritical();

	u8 num = (left ? SERVO_LEFT_ARM_INDEX : SERVO_RIGHT_ARM_INDEX);
	double ret = servo->GET_POS(num);

	ExitCritical();

	return ret;

}


bool PrimitivesCan::Magnet(bool left, int polarity){

	EnterCritical();

	bool ret;
	u8 num = (left ? MAGNET_LEFT_INDEX : MAGNET_RIGHT_INDEX);


	//ha folyamatban van valami, amire ezt nem indithatjuk el
	if(magnet->SetPolarity[num].inProgress){
		ret = ACT_START_ERROR;
	}

	//ha elindithatjuk
	else{
		magnet->MAGNET_SET_POLARITY(num, polarity);
		ret = ACT_STARTED;
	}


	ExitCritical();

	return ret;

}


bool PrimitivesCan::MagnetInProgress(bool left){

	EnterCritical();

	u8 num = (left ? MAGNET_LEFT_INDEX : MAGNET_RIGHT_INDEX);
	bool ret = magnet->SetPolarity[num].inProgress;

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


void PrimitivesCan::GetRobotPos_Unsafe(double* x, double* y, double* phi){

	double xr, yr, phir;

	deadreck->GET_POS(&xr, &yr, &phir);

	ConvRobotToWorld(xr, yr, phir, x, y, phi);

}


long int PrimitivesCan::GetOpponentPos_Unsafe(double* x, double* y){

	long int ret;

	double sonarpos_S[3];	// Szonar pozicioadat
	double sonarpos_W[3];	// Szonar pozicioadat vilagkoordinatakban

	ret = sonar->GET_POS((&sonarpos_S[0]), (&sonarpos_S[1]));
	sonarpos_S[2] = 1.0;

	M33V3Mult(T33WS, sonarpos_S, sonarpos_W);

	*x = sonarpos_W[0];
	*y = sonarpos_W[1];

	//offset hizzaadasa
	*x += sonarXOffset;
	*y += sonarYOffset;

	return ret;

}


void PrimitivesCan::SetRobotPos_Unsafe(double x, double y, double phi){

	double tmpX, tmpY, tmpPhi;

	GetRobotPos_Unsafe(&tmpX, &tmpY, &tmpPhi);

	deadreckCheckX		+= 	x	- tmpX;
	deadreckCheckY		+= 	y	- tmpY;
	deadreckCheckPhi	+= 	phi	- tmpPhi;

}


bool PrimitivesCan::GetMyColor_Unsafe(void){

	bool ret = (input->GET_DIGITAL(INPUT_DIGITAL_COLOR_BUTTON_INDEX) ? COLOR_BLUE : COLOR_RED);

	return ret;

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
