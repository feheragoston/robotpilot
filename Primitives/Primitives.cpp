/*
 * Primitives.cpp
 *
 *  Created on: Mar 24, 2011
 *      Author: agoston
 */

#include "Primitives.h"

Primitives::Primitives(Config* config) {
	mConfig = config;

	mStartButton = false;
	mStopButton = false;
	mRobotColor = false;
}

Primitives::~Primitives() {

}

bool Primitives::Init() {
	gettimeofday(&lastWait, NULL);

	return true;
}

bool Primitives::Wait(long int useconds) {
	struct timeval currentTime;
	struct timeval timeDiff;
	gettimeofday(&currentTime, NULL);

	tools::timeval_subtract(&timeDiff, &currentTime, &lastWait);

	if (timeDiff.tv_sec > 0) {
		std::cerr << "Eltelt ido tul sok: " << timeDiff.tv_sec << std::endl;
		gettimeofday(&lastWait, NULL);
		return true;
	} else if (timeDiff.tv_usec > useconds) {
		gettimeofday(&lastWait, NULL);
		return true;
	}
	//std::cout << "Primitives::Wait " << 50000 - timeDiff.tv_usec << std::endl;
	usleep(useconds - timeDiff.tv_usec);
	gettimeofday(&lastWait, NULL);
	return true;
}

bool Primitives::GetStartButton() {
	return mStartButton;
}

bool Primitives::GetStopButton() {
	return mStopButton;
}

bool Primitives::GetMyColor() {
	return mRobotColor;
}

int Primitives::MotorSupply(bool powered) {
	return 1;
}

int Primitives::CalibrateDeadreckoning(bool simulate) {
	return 1;
}

int Primitives::SetSpeed(double v, double w) {
	return 1;
}

int Primitives::Go(double distance, double max_speed, double max_acc) {
	return 1;
}

int Primitives::GoTo(double x, double y, double max_speed, double max_acc) {
	return 1;
}

int Primitives::Turn(double angle, double max_speed, double max_acc) {
	return 1;
}

int Primitives::MotionStop(double dec) {
	return 1;
}

void Primitives::GetRobotPos(double* x, double* y, double* phi) {
}

void Primitives::GetOpponentPos(double * x, double* y) {
}

void Primitives::SetRobotPos(double x, double y, double phi) {
}

void Primitives::GetDistances(double distance[6]) {
}

int Primitives::SetGripperPos(double pos) {
	return 1;
}

int Primitives::CalibrateConsole() {
	return 1;
}

int Primitives::SetConsolePos(double pos, double max_speed, double max_acc) {
	return 1;
}

int Primitives::ConsoleStop() {
	return 1;
}

double Primitives::GetConsolePos() {
	return 0;
}

int Primitives::SetArmPos(bool left, double pos, double max_speed, double max_acc) {
	return 1;
}

int Primitives::Magnet(bool left, int polarity) {
	return 1;
}
