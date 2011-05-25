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
	mPawnInGripper = false;
}

Primitives::Primitives(Primitives* source) {
	mConfig = source->mConfig;

	mStartButton = source->mStartButton;
	mStopButton = source->mStopButton;
	mRobotColor = !source->mRobotColor;

	//robot = source->robot;
	//opponent = source->opponent;
	robot.x = source->robot.x;
	robot.y = source->robot.y;
	robot.phi = source->robot.phi;
	opponent.x = source->opponent.x;
	opponent.y = source->opponent.y;
}

Primitives::~Primitives() {

}

bool Primitives::Init() {
	return true;
}

bool Primitives::Wait(long int useconds) {
	double timediff = (double) useconds / 1000000.;

	double gotodist = 0.;
	double godist = 0.;
	double turnAngle = 0.;

	if (goTo.inprogress) {
		// A celpontot ravetitjuk a robot orientacioja altal kijelolt egyenesre
		// ehhez a ponthoz vesszuk az egyenes haladas tavolsagat
		double stmp = (target.x - robot.x) * cos(robot.phi) + (target.y - robot.y) * sin(robot.phi);
		double dx = stmp * cos(robot.phi);
		double dy = stmp * sin(robot.phi);
		// kiszamitjuk a celpont es a robot orientacioja kozotti szogkulonbseget
		double dp = AngleCorrect(atan2(target.y - robot.y, target.x - robot.x) - robot.phi);

		gotodist = sqrt(dx * dx + dy * dy);
		// ha kozel vagyunk szoget mar nem korrigalunk
		if (gotodist < 50.) {
			dp = 0;
		}

		if (fabs(dp) > 0.001) {
			double speed = fabs(robot.w);
			if (speed * speed / (2 * 2) < fabs(dp)) {
				speed += 2 * timediff;
			} else {
				speed -= 2 * timediff;
			}
			if (speed > 2) {
				speed = 2;
			}
			if (dp < 0)
				speed *= -1;
			robot.w = speed;
		} else {
			robot.w = 0;
		}

		double speed = fabs(robot.v);
		if (speed * speed / (2 * target.a) < gotodist) {
			speed += target.a * timediff;
		} else {
			speed -= target.a * timediff;
		}
		if (speed > target.v) {
			speed = target.v;
		}
		robot.v = speed;
	}

	if (go.inprogress) {
		double dx = target.x - robot.x;
		double dy = target.y - robot.y;
		godist = sqrt(dx * dx + dy * dy);
		double speed = fabs(robot.v);
		if (speed * speed / (2 * target.a) < godist) {
			speed += target.a * timediff;
		} else {
			speed -= target.a * timediff;
		}
		if (speed > target.v) {
			speed = target.v;
		}
		// eloszor kiszamoljuk, hogy merre kell elindulni
		// ha mar mozgunk az iranyt nem szamoljuk ujra
		if (robot.v == 0.) {
			double dp = AngleCorrect(atan2(target.y - robot.y, target.x - robot.x) - robot.phi);
			if (fabs(dp) > M_PI_2) {
				speed *= -1;
			}
		} else if (robot.v < 0) {
			speed *= -1;
		}
		robot.v = speed;
	}

	if (turn.inprogress) {
		turnAngle = AngleCorrect(target.phi - robot.phi);
		double speed = fabs(robot.w);
		if (speed * speed / (2 * target.a) < fabs(turnAngle)) {
			speed += target.a * timediff;
		} else {
			speed -= target.a * timediff;
		}
		if (speed > target.w) {
			speed = target.w;
		}
		if (turnAngle < 0)
			speed *= -1;
		robot.w = speed;
	}

	if (fabs(robot.v) > EPSILON || fabs(robot.w) > EPSILON) {
		double dist = robot.v * timediff;
		double angle = robot.w * timediff;
		double dx = robot.v / robot.w * (cos(robot.phi) - cos(robot.phi + robot.w * timediff));
		double dy = -robot.v / robot.w * (sin(robot.phi) - sin(robot.phi + robot.w * timediff));
		if (goTo.inprogress) {
			if (gotodist - fabs(dist) < 3e-10) {
				dist = gotodist;
				goTo.finished = true;
				robot.v = 0;
				robot.w = 0;
			}
		}
		if (go.inprogress) {
			if (fabs(godist) - fabs(dist) < 3e-10) {
				dist = godist;
				go.finished = true;
				robot.v = 0;
			}
		}
		if (turn.inprogress) {
			if (fabs(turnAngle) - fabs(angle) < 10e-5) {
				angle = turnAngle;
				turn.finished = true;
				robot.w = 0;
			}
		}

		if (fabs(angle) < EPSILON) {
			robot.x += cos(robot.phi) * dist;
			robot.y += sin(robot.phi) * dist;
		} else if (fabs(dist) < EPSILON) {
			robot.phi += angle;
		} else {
			robot.x += dx;
			robot.y += dy;
			robot.phi += angle;
		}
	}

	//usleep(useconds);
	return true;
}

bool Primitives::PawnInGripper(void) {
	return mPawnInGripper;
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

bool Primitives::GetMotorSupply() {
	return true;
}

int Primitives::CalibrateDeadreckoning(bool simulate) {
	robot.x = 200;
	robot.y = 200;
	robot.phi = M_PI / 2;

	opponent.x = 200;
	opponent.y = 2800;
	return 1;
}

int Primitives::SetSpeed(double v, double w) {
	robot.v = v;
	robot.w = w;
	return 1;
}

int Primitives::Go(double distance, double max_speed, double max_acc) {
	if (go.inprogress) {
		if (go.finished) {
			go.inprogress = false;
			go.finished = false;
			return 1;
		}
	} else {
		target.x = robot.x + cos(robot.phi) * distance;
		target.y = robot.y + sin(robot.phi) * distance;
		target.v = max_speed;
		target.a = max_acc;
		go.inprogress = true;
	}
	return 0;
}

int Primitives::GoTo(double x, double y, double max_speed, double max_acc) {
	if (goTo.inprogress) {
		if (goTo.finished) {
			goTo.inprogress = false;
			goTo.finished = false;
			return 1;
		}
	} else {
		target.x = x;
		target.y = y;
		target.v = max_speed;
		target.a = max_acc;
		goTo.inprogress = true;
	}
	return 0;
}

int Primitives::Turn(double angle, double max_speed, double max_acc) {
	if (turn.inprogress) {
		if (turn.finished) {
			turn.inprogress = false;
			turn.finished = false;
			return 1;
		}
	} else {
		target.phi = robot.phi + angle;
		target.w = max_speed;
		target.a = max_acc;
		turn.inprogress = true;
	}
	return 0;
}

int Primitives::MotionStop(double dec) {
	if (motionStop.inprogress) {
		if (motionStop.finished) {
			motionStop.inprogress = false;
			motionStop.finished = false;

			goTo.finished = false;
			goTo.inprogress = false;
			go.finished = false;
			go.inprogress = false;
			turn.finished = false;
			turn.inprogress = false;
			return 1;
		}
	} else {
		target.a = dec;
		motionStop.inprogress = true;
	}
	return 0;
}

void Primitives::GetRobotPos(double* x, double* y, double* phi) {
	*x = robot.x;
	*y = robot.y;
	*phi = robot.phi;
}

void Primitives::GetOpponentPos(double * x, double* y) {
	*x = opponent.x;
	*y = opponent.y;
}

void Primitives::GetSpeed(double* v, double* w) {
	*v = robot.v;
	*w = robot.w;
}

void Primitives::SetRobotPos(double x, double y, double phi) {
	robot.x = x;
	robot.y = y;
	robot.phi = phi;
}

void Primitives::SetOpponentPos(double x, double y) {
	opponent.x = x;
	opponent.y = y;
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
