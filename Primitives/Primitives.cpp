/*
 * Primitives.cpp
 *
 *  Created on: Mar 24, 2011
 *      Author: agoston
 */

#include "Primitives.h"

Primitives::Primitives(Config* config) {
	mConfig = config;

	mStartButton = true;
	mStopButton = false;
	mRobotColor = false;
	mPawnInGripper = false;

	gripperPos = 0;
	consolePos = 0;
	leftArmPos = 0;
	rightArmPos = 0;
}

Primitives::Primitives(Primitives* source) {
	mConfig = source->mConfig;

	mStartButton = source->GetStartButton();
	mStopButton = source->GetStopButton();
	mRobotColor = !source->GetMyColor();

	source->GetRobotPos(&robot.x, &robot.y, &robot.phi);
	source->GetOpponentPos(&opponent.x, &opponent.y);
	gripperPos = source->GetGripperPos();
	consolePos = source->GetConsolePos();
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

	if (motionStop.inprogress) {
		if (target.a == 0.) {
			robot.v = 0.;
			robot.w = 0.;
		} else {
			double speed = fabsf(robot.v);
			speed -= target.a * timediff;
			if (speed < 0.) {
				speed = 0.;
			}
			if (robot.v < 0)
				speed *= -1;
			robot.v = speed;
			robot.w = 0.;
		}

		if (robot.v == 0. && robot.w == 0.) {
			motionStop.inprogress = false;
		}
	}

	if (fabs(robot.v) > EPSILON || fabs(robot.w) > EPSILON) {
		double dist = robot.v * timediff;
		double angle = robot.w * timediff;
		double dx = -robot.v / robot.w * (sin(robot.phi) - sin(robot.phi + robot.w * timediff));
		double dy = robot.v / robot.w * (cos(robot.phi) - cos(robot.phi + robot.w * timediff));
		if (goTo.inprogress) {
			if (gotodist - fabs(dist) < 3e-10) {
				dist = gotodist;
				goTo.inprogress = false;
				robot.v = 0;
				robot.w = 0;
			}
		}
		if (go.inprogress) {
			if (fabs(godist) - fabs(dist) < 3e-10) {
				dist = godist;
				go.inprogress = false;
				robot.v = 0;
			}
		}
		if (turn.inprogress) {
			if (fabs(turnAngle) - fabs(angle) < 10e-5) {
				angle = turnAngle;
				turn.inprogress = false;
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

	if (mConfig->DebugSimulation) {
		usleep(useconds / 2);
	}
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

bool Primitives::SetMotorSupply(bool powered) {
	return true;
}

bool Primitives::SetMotorSupplyInProgress() {
	return false;
}

bool Primitives::GetMotorSupply() {
	return true;
}

bool Primitives::CalibrateDeadreckoning(bool simulate) {
	robot.x = 200;
	robot.y = 200;
	robot.phi = M_PI / 2;

	opponent.x = 200;
	opponent.y = 2800;
	return true;
}

bool Primitives::CalibrateDeadreckoningInProgress() {
	return false;
}

bool Primitives::SetSpeed(double v, double w) {
	robot.v = v;
	robot.w = w;
	return true;
}

bool Primitives::SetSpeedInProgress() {
	return false;
}

bool Primitives::Go(double distance, double max_speed, double max_acc) {
	if (MotionInProgress() || MotionStopInProgress()) {
		return false;
	}
	target.x = robot.x + cos(robot.phi) * distance;
	target.y = robot.y + sin(robot.phi) * distance;
	target.v = max_speed;
	target.a = max_acc;
	go.inprogress = true;
	return true;
}

bool Primitives::GoTo(double x, double y, double max_speed, double max_acc) {
	if (MotionInProgress() || MotionStopInProgress()) {
		return false;
	}
	target.x = x;
	target.y = y;
	target.v = max_speed;
	target.a = max_acc;
	goTo.inprogress = true;
	return true;
}

bool Primitives::Turn(double angle, double max_speed, double max_acc) {
	if (MotionInProgress() || MotionStopInProgress()) {
		return false;
	}
	target.phi = robot.phi + angle;
	target.w = max_speed;
	target.a = max_acc;
	turn.inprogress = true;
	return true;
}

bool Primitives::MotionInProgress() {
	return go.inprogress || goTo.inprogress || turn.inprogress;
}

int Primitives::GetMotionError() {
	return 0;
}

bool Primitives::MotionStop(double dec) {
	if (motionStop.inprogress) {
		return false;
	}
	target.a = dec;
	motionStop.inprogress = true;

	go.inprogress = false;
	goTo.inprogress = false;
	turn.inprogress = false;
	return true;
}

bool Primitives::MotionStopInProgress() {
	return motionStop.inprogress;
}

void Primitives::GetRobotPos(double* x, double* y, double* phi) {
	*x = robot.x;
	*y = robot.y;
	*phi = robot.phi;
}

long int Primitives::GetOpponentPos(double * x, double* y) {
	*x = opponent.x;
	*y = opponent.y;
	return 0;
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
	for (int i = 0; i < 6; i++) {
		distance[i] = 400;
	}
}

bool Primitives::GripperMove(double pos) {
	gripperPos = pos;
	return true;
}

bool Primitives::GripperMoveInProgress() {
	return false;
}

double Primitives::GetGripperPos() {
	return gripperPos;
}

bool Primitives::CalibrateConsole() {
	consolePos = 0.;
	return true;
}

bool Primitives::CalibrateConsoleInProgress() {
	return false;
}

bool Primitives::ConsoleMove(double pos, double max_speed, double max_acc) {
	consolePos = pos;
	return true;
}

bool Primitives::ConsoleMoveInProgress() {
	return false;
}

bool Primitives::ConsoleStop() {
	return true;
}

bool Primitives::ConsoleStopInProgress() {
	return false;
}

double Primitives::GetConsolePos() {
	return consolePos;
}

bool Primitives::ArmMove(bool left, double pos, double max_speed, double max_acc) {
	if (left) {
		leftArmPos = pos;
	} else {
		rightArmPos = pos;
	}
	return true;
}

bool Primitives::ArmMoveInProgress(bool left) {
	return false;
}

double Primitives::GetArmPos(bool left) {
	if (left) {
		return leftArmPos;
	}
	return rightArmPos;
}

bool Primitives::Magnet(bool left, int polarity) {
	return true;
}

bool Primitives::MagnetInProgress(bool left) {
	return false;
}
