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
	position() : x(0.), y(0.), phi(0.), v(0.), w(0.) {}
	double x;
	double y;
	double phi;
	double v;
	double w;
} position;

class PrimitivesNet: public Primitives {
public:
	PrimitivesNet(Config* config);
	virtual ~PrimitivesNet();

	bool Init();
	bool CameraInit();

	bool Wait(long int useconds);

	int CalibrateDeadreckoning(bool simulate);
	int SetSpeed(double v, double w);
	int Go(double distance, double max_speed, double max_acc);
	int GoTo(double x, double y, double max_speed, double max_acc);
	int Turn(double angle, double max_speed, double max_acc);
	int MotionStop(double dec);
	void GetRobotPos(double* x, double* y, double* phi);
	void GetOpponentPos(double * x, double* y);
	void GetSpeed(double* v, double* w);

	int SetGripperPos(double pos);
	int CalibrateConsole();
	int SetConsolePos(double pos, double max_speed, double max_acc);
	int ConsoleStop();
	double GetConsolePos();
	int SetArmPos(bool left, double pos, double max_speed, double max_acc);
	int Magnet(bool left, int polarity);

	int RefreshPawnPositions(msgpawns* pawns);

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
	progress leftArmMove, rightArmMove;

	progress pawnRefresh;
	msgpawns* pawns;

	position robot, opponent;
};

#endif /* PRIMITIVESNET_H_ */
