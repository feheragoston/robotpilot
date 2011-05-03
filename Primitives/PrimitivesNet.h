/*
 * PrimitivesNet.h
 *
 *  Created on: Mar 25, 2011
 *      Author: agoston
 */

#ifndef PRIMITIVESNET_H_
#define PRIMITIVESNET_H_

#include "Primitives.h"
#include "Net/Net.h"
#include "Net/Protocol/Protocol.h"

typedef struct progress {
	progress() : inprogress(false), finished(false) {}
	bool inprogress;
	bool finished;
} progress;

typedef struct position {
	position() : x(0.), y(0.), phi(0.) {}
	double x;
	double y;
	double phi;
} position;

class PrimitivesNet: public Primitives {
public:
	PrimitivesNet(Config* config);
	virtual ~PrimitivesNet();

	bool Init();

	bool Wait(long int useconds);

	int CalibrateDeadreckoning(bool simulate);
	int SetSpeed(double v, double w);
	int Go(double distance, double max_speed, double max_acc);
	int GoTo(double x, double y, double max_speed, double max_acc);
	int Turn(double angle, double max_speed, double max_acc);
	int MotionStop(double dec);
	void GetRobotPos(double* x, double* y, double* phi);
	void GetOpponentPos(double * x, double* y);

	int SetGripperPos(double pos);
	int CalibrateConsole();
	int SetConsolePos(double pos, double max_speed, double max_acc);
	int ConsoleStop();
	double GetConsolePos();

protected:
	// a halozatbol jovo uzenet feldolgozasa
	bool processMessage(const void* buffer, int size);

	Net* netConnection;

	progress calibrateDeadreckoning;
	progress go;
	progress goTo;
	progress turn;
	progress motionStop;

	progress gripperMove;
	progress consoleMove;

	position robot, opponent;
};

#endif /* PRIMITIVESNET_H_ */
