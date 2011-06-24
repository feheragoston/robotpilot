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

	bool CalibrateDeadreckoning(bool simulate);
	bool CalibrateDeadreckoningInProgress();
	bool SetSpeed(double v, double w);
	bool Go(double distance, double max_speed, double max_acc);
	bool GoTo(double x, double y, double max_speed, double max_acc);
	bool Turn(double angle, double max_speed, double max_acc);
	bool MotionStop(double dec);
	void GetDistances(double distance[6]);

	bool GripperMove(double pos);
	bool GripperMoveInProgress();
	bool CalibrateConsole();
	bool ConsoleMove(double pos, double max_speed, double max_acc);
	bool ConsoleMoveInProgress();
	bool ConsoleStop();
	double GetConsolePos();
	bool ArmMove(bool left, double pos, double max_speed, double max_acc);
	bool ArmMoveInProgress(bool left);
	bool Magnet(bool left, int polarity);

	bool RefineDeadreckoning(double x, double y, double phi);
	bool RefineDeadreckoningInProgress();
	void GetRefineData(double* dx, double* dy, double* dphi);
	bool RefreshPawnPositions(msgpawns* pawns, function_t f, double x, double y, double phi);
	bool RefreshPawnPositionsInProgress();
	bool RefreshPawnPositionsFinished();

protected:
	// a halozatbol jovo uzenet feldolgozasa
	bool processMessage(const void* buffer, int size);

	Net* netConnection;

	progress posRefine;
	position refineDelta;
	double dx, dy, dphi;
	progress pawnRefresh;
	msgpawns* pawns;
	double distances[6];
};

#endif /* PRIMITIVESNET_H_ */
