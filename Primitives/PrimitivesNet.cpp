/*
 * PrimitivesNet.cpp
 *
 *  Created on: Mar 25, 2011
 *      Author: agoston
 */

#include "PrimitivesNet.h"

PrimitivesNet::PrimitivesNet(Config* config) :
	Primitives(config) {
	netConnection = new Net();
}

PrimitivesNet::~PrimitivesNet() {
}

bool PrimitivesNet::Init() {
	if (strlen(mConfig->NetIp) == 0 || !netConnection->ConnectToHost(13000, mConfig->NetIp)) {
		if (!netConnection->ConnectToHost(13000, "127.0.0.1")) {
			std::cerr << "Hiba a halozati kapcsolodaskor" << std::endl;
			return false;
		}
	}
	char buffer[256];
	strcpy(buffer, "robot");
	netConnection->Send(buffer, strlen(buffer));
	return true;
}

bool PrimitivesNet::CameraInit() {
	if (strlen(mConfig->NetIp) == 0 || !netConnection->ConnectToHost(13000, mConfig->NetIp)) {
		if (!netConnection->ConnectToHost(13000, "127.0.0.1")) {
			std::cerr << "Hiba a halozati kapcsolodaskor" << std::endl;
			return false;
		}
	}
	return true;
}

bool PrimitivesNet::Wait(long int useconds) {
	char buffer[255];

	while (true) {
		int size = netConnection->Receive(buffer, 255, useconds);
		useconds = 0;

		if (size < 0) {
			return false;
		}

		if (size == 0) {
			return true;
		}

		if (processMessage(buffer, size)) {
		}
	}

	return true;
}

bool PrimitivesNet::processMessage(const void* buffer, int size) {
	function_t* function = (function_t*) buffer;
	if (*function == MSG_REFRESHSTATUS && size == sizeof(msgstatus)) {
		msgstatus* data = (msgstatus*) buffer;
		robot.x = data->x;
		robot.y = data->y;
		robot.phi = data->phi;
		robot.v = data->v;
		robot.w = data->w;
		for (unsigned char i = 0; i < OPPONENT_NUM; i++) {
			opponent[i].x = data->ox[i];
			opponent[i].y = data->oy[i];
		}
		mStartButton = data->startButton;
		mStopButton = data->stopButton;
		mRobotColor = data->color;
		leftGripperPos = data->leftGripperPos;
		rightGripperPos = data->rightGripperPos;
		leftClawPos = data->leftClawPos;
		rightClawPos = data->rightClawPos;
		armPos = data->armPos;
		consolePos = data->consolePos;
		for (int i = 0; i < PROXIMITY_NUM; i++) {
			distances[i] = data->distances[i];
		}
	} else if (*function == MSG_CALIBRATEPOS && size == sizeof(msgb1)) {
		msgb1* data = (msgb1*) buffer;
		if (data->b1) {
			calibrateDeadreckoning.inprogress = false;
		}
	} else if (*function == MSG_GO && size == sizeof(msgb1)) {
		msgb1* data = (msgb1*) buffer;
		if (data->b1) {
			go.inprogress = false;
		}
	} else if (*function == MSG_GOTO && size == sizeof(msgb1)) {
		msgb1* data = (msgb1*) buffer;
		if (data->b1) {
			goTo.inprogress = false;
		}
	} else if (*function == MSG_TURN && size == sizeof(msgb1)) {
		msgb1* data = (msgb1*) buffer;
		if (data->b1) {
			turn.inprogress = false;
		}
	} else if (*function == MSG_MOTIONSTOP && size == sizeof(msgb1)) {
		msgb1* data = (msgb1*) buffer;
		if (data->b1) {
			motionStop.inprogress = false;
		}
	} else if (*function == MSG_CONSOLEMOVE && size == sizeof(msgb1)) {
		msgb1* data = (msgb1*) buffer;
		if (data->b1) {
			consoleMove.inprogress = true;
		}
	} else {
		printf("Unknown or invalid function: %d size: %d\n", *function, size);
		return false;
	}
	return true;
}

bool PrimitivesNet::CalibrateDeadreckoning(bool simulate) {
	if (calibrateDeadreckoning.inprogress) {
		return false;
	}
	msgb1 message;
	message.function = MSG_CALIBRATEPOS;
	message.b1 = simulate;
	netConnection->Send(&message, sizeof(msgb1));
	calibrateDeadreckoning.inprogress = true;
	return true;
}

bool PrimitivesNet::CalibrateDeadreckoningInProgress() {
	return calibrateDeadreckoning.inprogress;
}

bool PrimitivesNet::SetSpeed(double v, double w) {
	msgd2 message;
	message.function = MSG_SETSPEED;
	message.d1 = v;
	message.d2 = w;
	netConnection->Send(&message, sizeof(msgd2));
	return true;
}

bool PrimitivesNet::Go(double distance, double max_speed, double max_acc) {
	if (MotionInProgress() || MotionStopInProgress()) {
		return false;
	}
	msgd3 message;
	message.function = MSG_GO;
	message.d1 = distance;
	message.d2 = max_speed;
	message.d3 = max_acc;
	netConnection->Send(&message, sizeof(msgd3));
	go.inprogress = true;
	return true;
}

bool PrimitivesNet::GoTo(double x, double y, double max_speed, double max_acc) {
	if (MotionInProgress() || MotionStopInProgress()) {
		return false;
	}
	msgd4 message;
	message.function = MSG_GOTO;
	message.d1 = x;
	message.d2 = y;
	message.d3 = max_speed;
	message.d4 = max_acc;
	netConnection->Send(&message, sizeof(msgd4));
	goTo.inprogress = true;
	return true;
}

bool PrimitivesNet::Turn(double angle, double max_speed, double max_acc) {
	if (MotionInProgress() || MotionStopInProgress()) {
		return false;
	}
	msgd3 message;
	message.function = MSG_TURN;
	message.d1 = angle;
	message.d2 = max_speed;
	message.d3 = max_acc;
	netConnection->Send(&message, sizeof(msgd3));
	turn.inprogress = true;
	return true;
}

bool PrimitivesNet::MotionStop(double dec) {
	if (motionStop.inprogress) {
		return false;
	}
	msgd1 message;
	message.function = MSG_MOTIONSTOP;
	message.d1 = dec;
	netConnection->Send(&message, sizeof(msgd1));
	motionStop.inprogress = true;

	go.inprogress = false;
	goTo.inprogress = false;
	turn.inprogress = false;

	return true;
}

void PrimitivesNet::GetDistances(double distance[6]) {
	for (int i = 0; i < 6; i++) {
		distance[i] = distances[i];
	}
}

bool PrimitivesNet::GripperMove(bool left, double pos, double max_speed, double max_acc) {
	progress* p;
	uint8_t id;
	if (left) {
		p = &leftGripperMove;
		id = 0; // TODO
	} else {
		p = &rightGripperMove;
		id = 1; // TODO
	}
	if (p->inprogress) {
		return false;
	}
	msgservo message;
	message.function = MSG_SERVOMOVE;
	message.id = id;
	message.pos = pos;
	message.speed = max_speed;
	message.acc = max_acc;
	netConnection->Send(&message, sizeof(msgservo));
	p->inprogress = true;
	return true;
}

bool PrimitivesNet::GripperMoveInProgress(bool left) {
	progress* p;
	if (left) {
		p = &leftGripperMove;
	} else {
		p = &rightGripperMove;
	}
	return p->inprogress;
}

bool PrimitivesNet::ClawMove(bool left, double pos, double max_speed, double max_acc) {
	progress* p;
	uint8_t id;
	if (left) {
		p = &leftClawMove;
		id = 2; // TODO
	} else {
		p = &rightClawMove;
		id = 3; // TODO
	}
	if (p->inprogress) {
		return false;
	}
	msgservo message;
	message.function = MSG_SERVOMOVE;
	message.id = id;
	message.pos = pos;
	message.speed = max_speed;
	message.acc = max_acc;
	netConnection->Send(&message, sizeof(msgservo));
	p->inprogress = true;
	return true;
}

bool PrimitivesNet::ClawMoveInProgress(bool left) {
	progress* p;
	if (left) {
		p = &leftClawMove;
	} else {
		p = &rightClawMove;
	}
	return p->inprogress;
}

bool PrimitivesNet::ArmMove(double pos, double max_speed, double max_acc) {
	if (armMove.inprogress) {
		return false;
	}
	msgservo message;
	message.function = MSG_SERVOMOVE;
	message.id = 4; // TODO
	message.pos = pos;
	message.speed = max_speed;
	message.acc = max_acc;
	netConnection->Send(&message, sizeof(msgservo));
	armMove.inprogress = true;
	return true;
}

bool PrimitivesNet::ArmMoveInProgress() {
	return armMove.inprogress;
}

bool PrimitivesNet::CalibrateConsole() {
	msgb1 message;
	message.function = MSG_CALIBRATECONSOLE;
	message.b1 = true;
	netConnection->Send(&message, sizeof(msgb1));
	return true;
}

bool PrimitivesNet::ConsoleMove(double pos, double max_speed, double max_acc) {
	if (consoleMove.inprogress) {
		return false;
	}
	msgd3 message;
	message.function = MSG_CONSOLEMOVE;
	message.d1 = pos;
	message.d2 = max_speed;
	message.d3 = max_acc;
	netConnection->Send(&message, sizeof(msgd3));
	consoleMove.inprogress = true;
	return true;
}

bool PrimitivesNet::ConsoleMoveInProgress() {
	return consoleMove.inprogress;
}

bool PrimitivesNet::ConsoleStop() {
	msgb1 message;
	message.function = MSG_CONSOLESTOP;
	message.b1 = true;
	netConnection->Send(&message, sizeof(msgb1));
	consoleMove.inprogress = false;
	return true;
}

double PrimitivesNet::GetConsolePos() {
	return 0.;
}
