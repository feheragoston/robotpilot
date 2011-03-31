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
#include "Net/Protocol.h"

typedef struct {
	bool inprogress;
	bool finished;
} progress;

typedef struct {
	double x;
	double y;
	double phi;
} position;

class PrimitivesNet: public Primitives {
public:
	PrimitivesNet(Config* config);
	virtual ~PrimitivesNet();

	bool Init();

	bool Wait();

	int Go(double distance, double max_speed, double max_acc);
	int GoTo(double x, double y, double max_speed, double max_acc);
	int Turn(double angle, double max_speed, double max_acc);
	int MotionStop(double dec);
	void GetRobotPos(double* x, double* y, double* phi);
	void GetOpponentPos(double * x, double* y);

protected:
	Net* netConnection;

	progress go;
	progress goTo;
	progress turn;
	progress motionStop;

	position robot, opponent;
};

#endif /* PRIMITIVESNET_H_ */
