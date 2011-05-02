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
#include "nodes/include/node_Magnet.h"
#include "nodes/include/node_Servo.h"
#include "nodes/include/node_Sonar.h"
#include "nodes/include/node_Power.h"

#include <fstream>

#include <sys/time.h>
#include <math.h>

#include "Primitives.h"
//------------------------------ include VEGE ------------------------------





//------------------------------ PrimitivesCan ELEJE ------------------------------
class PrimitivesCan : public Primitives{

public:

	PrimitivesCan(Config* config);
	~PrimitivesCan();

	bool Init(void);

	bool Wait(long int useconds);

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


	void EnterCritical(void);
	void ExitCritical(void);

	void evalMsg(UDPmsg* msg);
	void signalNewMsg(void);

	void KEEP_ALIVE_SEND(void);
	void KEEP_ALIVE_SLEEP(void);


private:


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
	node_Magnet*		magnet;
	node_Servo*			servo;
	node_Sonar*			sonar;
	node_Power*			power;
	//---------- node VEGE ----------


	//----- valtozo ELEJE -----
	double deadreckPosOffsetX;
	double deadreckPosOffsetY;
	double deadreckPosOffsetPhi;
	//----- valtozo VEGE -----


	//----- define ELEJE -----
	static const bool			COLOR_RED						= true;
	static const bool			COLOR_BLUE						= !COLOR_RED;
	//----- define VEGE -----


	//----- deadreckCalib ELEJE -----
	unsigned int deadreckCalibPhase;
	int GoToWall(double speed, double omega);
	unsigned int goToWallPhase;
	//----- deadreckCalib VEGE -----


};
//------------------------------ PrimitivesCan VEGE ------------------------------





#endif /*PRIMITIVESCAN_H_*/
