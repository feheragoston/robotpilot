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
#include "nodes/include/node_Caracole.h"
#include "nodes/include/node_Firewheel.h"
#include "nodes/include/node_Deadreck.h"
#include "nodes/include/node_DCWheel.h"
#include "nodes/include/node_Input.h"
#include "nodes/include/node_Servo.h"
#include "nodes/include/node_Sonar.h"
#include "nodes/include/node_Power.h"

#include <fstream>

#include <math.h>

#include <errno.h>

#include "Primitives.h"

#ifdef KIS_ROBOT
#include "../FollowLine.h"
#endif

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
	int8_t GetMyColor(void);
#ifdef NAGY_ROBOT
	bool GetBallPresent(void);
#endif

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
	bool GoArc(double x, double y, double r, double phi, double max_w, double max_acc);
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

#ifdef NAGY_ROBOT
	double GetBallColorVoltage(void);

	bool ArmMove(bool front, double pos, double max_speed, double max_acc);
	bool ArmMoveInProgress(bool front);
	double GetArmPos(bool front);
	bool GetArmError(bool front);

	bool ContractorMove(double pos, double max_speed, double max_acc);
	bool ContractorMoveInProgress();
	double GetContractorPos();
	bool GetContractorError();

	bool FlipperMove(double pos, double max_speed, double max_acc);
	bool FlipperMoveInProgress();
	double GetFlipperPos();
	bool GetFlipperError();

	bool SelectorMove(double pos, double max_speed, double max_acc);
	bool SelectorMoveInProgress();
	double GetSelectorPos();
	bool GetSelectorError();

	bool FireStopperMove(double pos, double max_speed, double max_acc);
	bool FireStopperMoveInProgress();
	double GetFireStopperPos();
	bool GetFireStopperError();
#else	//KIS_ROBOT
	bool GripperMove(bool low, bool left, double pos, double max_speed, double max_acc);
	bool GripperMoveInProgress(bool low, bool left);
	bool GetGripperError(bool low, bool left);
	double GetGripperPos(bool low, bool left);
#endif

	bool CalibrateConsole(void);
	bool CalibrateConsoleInProgress(void);

	bool ConsoleMove(double pos, double max_speed, double max_acc);
	bool ConsoleMoveInProgress(void);

	bool ConsoleStop(void);
	bool ConsoleStopInProgress(void);
	double GetConsolePos(void);

#ifdef KIS_ROBOT
	/**
	 * Vonalkovetes kovetes inditasa
	 * @param dist [mm]
	 * @return true: folyamat elindult, false: hiba tortent
	 */
	bool FollowLine_Follow(double dist);

	/**
	 * Vonalkovetes kovetes allapotanak lekerdezese
	 * @return true: folyamatban van, false: nincs folyamatban
	 */
	bool FollowLine_FollowInProgress();
	/**
	 * legutobbi kovetes leallasanak oka
	 * @return 0: nem allt le a vonalkovetes, 1: elerte a tavolsagot, 2: elagazasba utkozott, 3: egyeb hiba
	 */
	int FollowLine_GetFollowError();
	/**
	 * Vonalkovetes fix threshold beallitas
	 * @return true: siker, false: hiba tortent
	 */
	bool FollowLine_FixThreshold(uint16_t threshold);
	/**
	 * Vonalkovetes kalibralas
	 * @return true: folyamat elindult, false: hiba tortent
	 */
	bool FollowLine_Calibrate();

	/**
	 * CalibrateFollowLine allapota
	 * @return true: folyamatban van, false: nincs folyamatban
	 */
	bool FollowLine_CalibrateInProgress();
#else //NAGY_ROBOT
	bool CaracoleSetSpeed(double speed, double max_acc);
	bool CaracoleSetSpeedInProgress(void);
	double GetCaracoleSpeed(void);

	bool FirewheelSetSpeed(double speed, double max_acc);
	bool FirewheelSetSpeedInProgress(void);
	double GetFirewheelSpeed(void);
#endif
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
	node_Input*			input;
	node_Deadreck*		deadreck;
	node_DCWheel*		dcwheel;
	node_Servo*			servo;
	node_Sonar*			sonar;
	#ifdef NAGY_ROBOT
	node_Caracole*		caracole;
	node_Firewheel*		firewheel;
	node_Power*			power;
	#endif
	//---------- node VEGE ----------

#ifdef KIS_ROBOT
	FollowLine* mFollowLine;

	bool Follow_InProgress;
	timespec Follow_next_ts;
	bool Follow_ts_valid;

#endif

	//----- valtozo ELEJE -----
	char CanIp[16];

	bool dcwheelMotionError;

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


	int8_t GetMyColor_Unsafe(void);
	bool HasColor_Unsafe(void);
	void GetRobotPos_Unsafe(double* x, double* y, double* phi);
	long int GetOpponentPos_Unsafe(u8 num, double* x, double* y);
	void SetRobotPos_Unsafe(double x, double y, double phi);


};
//------------------------------ PrimitivesCan VEGE ------------------------------





#endif /*PRIMITIVESCAN_H_*/
