/*
 * PrimitivesCan.h
 *
 *  Created on: Feb 15, 2011
 *      Author: triszalo
 */


#ifndef PRIMITIVESCAN_H_
#define PRIMITIVESCAN_H_





//------------------------------ define ELEJE ------------------------------
#define	ROBOTPILOT
//------------------------------ define VEGE ------------------------------





//------------------------------ include ELEJE ------------------------------
#include "nodes/include/node_Broadcast.h"
#include "nodes/include/node_Gateway.h"
#include "nodes/include/node_Console.h"
#include "nodes/include/node_Deadreck.h"
#include "nodes/include/node_Drive.h"
#include "nodes/include/node_Input.h"
#include "nodes/include/node_Magnet.h"
#include "nodes/include/node_Servo.h"
#include "nodes/include/node_Sonar.h"
#include "nodes/include/node_Supply.h"

#include <fstream>

#include <sys/time.h>
#include <math.h>

#ifdef	ROBOTPILOT
	#include "Primitives.h"
#endif
//------------------------------ include VEGE ------------------------------





//------------------------------ PrimitivesCan ELEJE ------------------------------
#ifdef	ROBOTPILOT
class PrimitivesCan : public Primitives{
#else
class PrimitivesCan{
#endif

public:

	#ifdef	ROBOTPILOT
		PrimitivesCan(Config* config);
	#else
		PrimitivesCan();
	#endif
	~PrimitivesCan();

	bool Wait(long int useconds);

	void EnterCritical(void);
	void ExitCritical(void);

	int Go(double distance, double max_speed, double max_acc);
	int GoTo(double x, double y, double max_speed, double max_acc);
	int Turn(double angle, double max_speed, double max_acc);
	int SetSpeed(double v, double w);
	int MotionStop(double dec);
	int CalibrateDeadreckoning(bool simulate);
	int MotorSupply(bool powered);
	int SetGripperPos(double pos);
	int SetConsolePos(double pos, double speed, double acc);
	int CalibrateConsole(void);
	int ConsoleStop(void);
	double GetConsolePos(void);
	int SetArmPos(bool left, double pos, double speed, double acc);
	int Magnet(bool left, int polarity);
	bool GetStartButton(void);
	bool GetStopButton(void);
	bool GetMyColor(void);
	void GetRobotPos(double* x, double* y, double* phi);
	void GetOpponentPos(double* x, double* y);
	void SetRobotPos(double x, double y, double phi);
	void GetDistances(double distance[6]);


	void evalMsg(UDPmsg* msg);
	void signalNewMsg(void);

	void KEEP_ALIVE_SEND(void);
	void KEEP_ALIVE_SLEEP(void);


private:


	//----- int visszateresi ertekek ELEJE -----
	static const int	ACT_INPROGRESS		= 1;
	static const int	ACT_FINISHED		= 0;
	static const int	ACT_ERROR			= -1;
	//----- int visszateresi ertekek VEGE -----


	//----- recieve ELEJE -----
	sem_t				newMessageSemaphore;
	pthread_mutex_t		recieveMutex;
	//----- recieve VEGE -----


	//----- keepAlive ELEJE -----
	static const unsigned int keepAlivePeriod_ms	= 20;
	//----- keepAlive ELEJE -----


	//---------- node ELEJE ----------
	node_Broadcast*		broadcast;
	node_Gateway*		gateway;
	node_Console*		console;
	node_Deadreck*		deadreck;
	node_Drive*			drive;
	node_Input*			input;
	node_Magnet*		magnet;
	node_Servo*			servo;
	node_Sonar*			sonar;
	node_Supply*		supply;
	//---------- node VEGE ----------


	//----- valtozo ELEJE -----
	double deadreckPosOffsetX;
	double deadreckPosOffsetY;
	double deadreckPosOffsetPhi;
	//----- valtozo VEGE -----


	//----- index ELEJE -----
	static const unsigned int	SERVO_GRIPPER_LEFT_INDEX		= 0;
	static const unsigned int	SERVO_GRIPPER_RIGHT_INDEX		= 1;
	static const unsigned int	SERVO_LEFT_ARM_INDEX			= 2;
	static const unsigned int	SERVO_RIGHT_ARM_INDEX			= 3;

	static const unsigned int	MAGNET_LEFT_INDEX				= 0;
	static const unsigned int	MAGNET_RIGHT_INDEX				= 1;

	static const unsigned int	BUTTON_START_INDEX				= 0;
	static const unsigned int	BUTTON_COLOR_INDEX				= 1;
	static const unsigned int	BUTTON_FRONT_LEFT_INDEX			= 2;
	static const unsigned int	BUTTON_FRONT_RIGHT_INDEX		= 3;
	static const unsigned int	BUTTON_REAR_LEFT_INDEX			= 4;
	static const unsigned int	BUTTON_REAR_RIGHT_INDEX			= 5;
	//----- index VEGE -----


	//----- define ELEJE -----
	static const bool			COLOR_RED						= true;
	static const bool			COLOR_BLUE						= !COLOR_RED;
	//----- define VEGE -----


	//----- deadreckCalib ELEJE -----
	static const double areaLengthX					= 2100;
	static const double areaLengthY					= 3000;
	static const double onFrontWallRobotDistance	= 200;
	static const double onRearWallRobotDistance		= 150;

	static const double deadreckCalibSpeedAbs		= 0.1;
	static const double deadreckCalibOmegaAbs		= 0.2;
	static const double deadreckCalibBetaAbs		= 0.08;
	static const double deadreckStartDistanceX		= 250;
	static const double deadreckStartDistanceY		= 250;

	unsigned int deadreckCalibPhase;
	int GoToWall(double speed, double omega);
	unsigned int goToWallPhase;
	//----- deadreckCalib VEGE -----


};
//------------------------------ PrimitivesCan VEGE ------------------------------





#endif /*PRIMITIVESCAN_H_*/
