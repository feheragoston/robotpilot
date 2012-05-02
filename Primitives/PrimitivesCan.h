/*
 * PrimitivesCan.h
 *
 *  Created on: Feb 15, 2011
 *      Author: triszalo
 */


#ifndef PRIMITIVESCAN_H_
#define PRIMITIVESCAN_H_





//------------------------------ include ELEJE ------------------------------
#include "nodes/include/node_Broadcast.h"
#include "nodes/include/node_Gateway.h"
#include "nodes/include/node_Console.h"
#include "nodes/include/node_Deadreck.h"
#include "nodes/include/node_BDC.h"
#include "nodes/include/node_Input.h"
#include "nodes/include/node_Vacuum.h"
#include "nodes/include/node_Servo.h"
#include "nodes/include/node_Sonar.h"
#include "nodes/include/node_Power.h"

#include <fstream>

#include <math.h>

#include "Primitives.h"
//------------------------------ include VEGE ------------------------------





//------------------------------ PrimitivesCan ELEJE ------------------------------
class PrimitivesCan : public Primitives{

public:

	PrimitivesCan(Config* config);
	~PrimitivesCan();

	//----- Primitives ELEJE -----
	bool Init(void);

	bool Wait(long int useconds);

	bool GetStartButton(void);
	bool GetStopButton(void);
	bool GetMyColor(void);

	bool SetMotorSupply(bool powered);
	bool SetMotorSupplyInProgress(void);
	bool GetMotorSupply(void);

	bool CalibrateDeadreckoning(bool simulate = false);
	bool CalibrateDeadreckoningInProgress(void);

	bool SetSpeed(double v, double w);
	bool SetWheelSpeed(double vLeft, double vRight);
	bool SetSpeedInProgress(void);

	bool Go(double distance, double max_speed, double max_acc);
	bool GoTo(double x, double y, double max_speed, double max_acc);
	bool Turn(double angle, double max_speed, double max_acc);
	bool MotionInProgress(void);
	int GetMotionError(void);

	bool MotionStop(double dec = 0.0);
	bool MotionStopInProgress(void);

	void GetRobotPos(double* x, double* y, double* phi);
	long int GetOpponentPos(unsigned char n, double * x, double* y);
	void GetSpeed(double* v, double* w);
	void SetRobotPos(double x, double y, double phi);
	void SetOpponentPos(unsigned char n, double x, double y);

	void GetDistances(double distance[PROXIMITY_NUM]);

	bool GripperMove(bool left, double pos, double max_speed, double max_acc);
	bool GripperMoveInProgress(bool left);
	double GetGripperPos(bool left);
	bool GetGripperError(bool left);

	bool ClawMove(bool left, double pos, double max_speed, double max_acc);
	bool ClawMoveInProgress(bool left);
	double GetClawPos(bool left);
	bool GetClawError(bool left);

	bool ArmMove(double pos, double max_speed, double max_acc);
	bool ArmMoveInProgress();
	double GetArmPos();
	bool GetArmError();

	bool CalibrateConsole(void);
	bool CalibrateConsoleInProgress(void);

	bool ConsoleMove(double pos, double max_speed, double max_acc);
	bool ConsoleMoveInProgress(void);

	bool ConsoleStop(void);
	bool ConsoleStopInProgress(void);
	double GetConsolePos(void);

	bool Compressor(bool on);
	bool CompressorInProgress(void);
	bool GetCompressor(void);

	bool Valve(bool open);
	bool ValveInProgress(void);
	bool GetValve(void);
	//----- Primitives VEGE -----



	void EnterCritical(void);
	void ExitCritical(void);

	void evalMsg(UDPmsg* msg);
	void signalNewMsg(void);

	void KEEP_ALIVE_SEND(void);
	void KEEP_ALIVE_SLEEP(void);


private:


	void addNodesToCan(void);
	bool initNode(node* node);


	//----- recieve ELEJE -----
	sem_t				newMessageSemaphore;
	pthread_mutex_t		recieveMutex;
	//----- recieve VEGE -----


	//---------- node ELEJE ----------
	node_Broadcast*		broadcast;
	node_Gateway*		gateway;
	node_Console*		console;
	node_Deadreck*		deadreck;
	node_BDC*			bdc;
	node_Input*			input;
	node_Vacuum*		vacuum;
	node_Servo*			servo;
	node_Sonar*			sonar;
	node_Power*			power;
	//---------- node VEGE ----------


	//----- valtozo ELEJE -----
	char CanIp[16];

	bool bdcMotionError;

	double deadreckCheckXw;
	double deadreckCheckYw;
	double deadreckCheckPhiw;

	double sonarXOffset[SONAR_TRANSMITTER_COUNT];
	double sonarYOffset[SONAR_TRANSMITTER_COUNT];

	double T33WS[3][3];	//World-Sonar transzformacios matrix
	//----- valtozo VEGE -----


	//----- deadreckCalib ELEJE -----
	unsigned int deadreckCalibPhase;
	//----- deadreckCalib VEGE -----

	void detectActChange(void);

	void Calibrate_Unsafe(void);
	int DeadreckoningResetPos_Unsafe(void);

	void ConvRobotToWorld(double xr, double yr, double phir, double* xw, double* yw, double* phiw);
	void ConvWorldToRobot(double xw, double yw, double phiw, double* xr, double* yr, double* phir);


	bool GetMyColor_Unsafe(void);
	bool HasColor_Unsafe(void);
	void GetRobotPos_Unsafe(double* x, double* y, double* phi);
	long int GetOpponentPos_Unsafe(u8 num, double* x, double* y);
	void SetRobotPos_Unsafe(double x, double y, double phi);


};
//------------------------------ PrimitivesCan VEGE ------------------------------





#endif /*PRIMITIVESCAN_H_*/
