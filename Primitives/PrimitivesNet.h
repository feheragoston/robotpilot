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

	progress pawnRefresh;
	msgpawns* pawns;
};

#endif /* PRIMITIVESNET_H_ */
