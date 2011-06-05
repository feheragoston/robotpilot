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
		opponent.x = data->ox;
		opponent.y = data->oy;
		mStartButton = data->startButton;
		mStopButton = data->stopButton;
		mRobotColor = data->color;
		mPawnInGripper = data->pawnInGripper;
		gripperPos = data->gripperPos;
		consolePos = data->consolePos;
		leftArmPos = data->leftArmPos;
		rightArmPos = data->rightArmPos;
	} else if (*function == MSG_CALIBRATEPOS && size == sizeof(msgb1)) {
		msgb1* data = (msgb1*) buffer;
		if (data->b1 && calibrateDeadreckoning.inprogress) {
			calibrateDeadreckoning.finished = true;
		}
	} else if (*function == MSG_GO && size == sizeof(msgb1)) {
		msgb1* data = (msgb1*) buffer;
		if (data->b1 && go.inprogress) {
			go.finished = true;
		}
	} else if (*function == MSG_GOTO && size == sizeof(msgb1)) {
		msgb1* data = (msgb1*) buffer;
		if (data->b1 && goTo.inprogress) {
			goTo.finished = true;
		}
	} else if (*function == MSG_TURN && size == sizeof(msgb1)) {
		msgb1* data = (msgb1*) buffer;
		if (data->b1 && turn.inprogress) {
			turn.finished = true;
		}
	} else if (*function == MSG_MOTIONSTOP && size == sizeof(msgb1)) {
		msgb1* data = (msgb1*) buffer;
		if (data->b1 && motionStop.inprogress) {
			motionStop.finished = true;
		}
	} else if (*function == MSG_GRIPPERMOVE && size == sizeof(msgb1)) {
		msgb1* data = (msgb1*) buffer;
		if (data->b1 && gripperMove.inprogress) {
			gripperMove.finished = true;
		}
	} else if (*function == MSG_CONSOLEMOVE && size == sizeof(msgb1)) {
		msgb1* data = (msgb1*) buffer;
		if (data->b1 && consoleMove.inprogress) {
			consoleMove.finished = true;
		}
	} else if (*function == MSG_ARMMOVE && size == sizeof(msgb1)) {
		msgb1* data = (msgb1*) buffer;
		if (data->b1 && leftArmMove.inprogress) {
			leftArmMove.finished = true;
		} else if (rightArmMove.inprogress) {
			rightArmMove.finished = true;
		}
	} else if (*function == MSG_POSREFINE && size == sizeof(msgd3)) {
		msgd3* data = (msgd3*) buffer;
		refineDelta.x = data->d1;
		refineDelta.y = data->d2;
		refineDelta.phi = data->d3;
		printf("POSREFINE: %f %f %f\n", refineDelta.x, refineDelta.y, refineDelta.phi);
		if (posRefine.inprogress) {
			posRefine.finished = true;
		}
	} else if (*function == MSG_PAWNS && size == sizeof(msgpawns)) {
		msgpawns* data = (msgpawns*) buffer;
		pawns->num = data->num;
		for (int i = 0; i < data->num; i++) {
			pawns->pawns[i].type = data->pawns[i].type;
			pawns->pawns[i].x = data->pawns[i].x;
			pawns->pawns[i].y = data->pawns[i].y;
		}
		if (pawnRefresh.inprogress) {
			pawnRefresh.finished = true;
		}
	} else {
		printf("Unknown or invalid function: %d size: %d\n", *function, size);
		return false;
	}
	return true;
}

int PrimitivesNet::CalibrateDeadreckoning(bool simulate) {
	if (calibrateDeadreckoning.inprogress) {
		if (calibrateDeadreckoning.finished) {
			calibrateDeadreckoning.inprogress = false;
			calibrateDeadreckoning.finished = false;
			return 1;
		}
	} else {
		msgb1 message;
		message.function = MSG_CALIBRATEPOS;
		message.b1 = simulate;
		netConnection->Send(&message, sizeof(msgb1));
		calibrateDeadreckoning.inprogress = true;
	}
	return 0;
}

int PrimitivesNet::SetSpeed(double v, double w) {
	msgd2 message;
	message.function = MSG_SETSPEED;
	message.d1 = v;
	message.d2 = w;
	netConnection->Send(&message, sizeof(msgd2));
	return 1;
}

int PrimitivesNet::Go(double distance, double max_speed, double max_acc) {
	if (go.inprogress) {
		if (go.finished) {
			go.inprogress = false;
			go.finished = false;
			return 1;
		}
	} else {
		msgd3 message;
		message.function = MSG_GO;
		message.d1 = distance;
		message.d2 = max_speed;
		message.d3 = max_acc;
		netConnection->Send(&message, sizeof(msgd3));
		go.inprogress = true;
	}
	return 0;
}

int PrimitivesNet::GoTo(double x, double y, double max_speed, double max_acc) {
	if (goTo.inprogress) {
		if (goTo.finished) {
			goTo.inprogress = false;
			goTo.finished = false;
			return 1;
		}
	} else {
		msgd4 message;
		message.function = MSG_GOTO;
		message.d1 = x;
		message.d2 = y;
		message.d3 = max_speed;
		message.d4 = max_acc;
		netConnection->Send(&message, sizeof(msgd4));
		goTo.inprogress = true;
	}
	return 0;
}

int PrimitivesNet::Turn(double angle, double max_speed, double max_acc) {
	if (turn.inprogress) {
		if (turn.finished) {
			turn.inprogress = false;
			turn.finished = false;
			return 1;
		}
	} else {
		msgd3 message;
		message.function = MSG_TURN;
		message.d1 = angle;
		message.d2 = max_speed;
		message.d3 = max_acc;
		netConnection->Send(&message, sizeof(msgd3));
		turn.inprogress = true;
	}
	return 0;
}

int PrimitivesNet::MotionStop(double dec) {
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
		msgd1 message;
		message.function = MSG_MOTIONSTOP;
		message.d1 = dec;
		netConnection->Send(&message, sizeof(msgd1));
		motionStop.inprogress = true;
	}
	return 0;
}

int PrimitivesNet::SetGripperPos(double pos) {
	if (gripperMove.inprogress) {
		if (gripperMove.finished) {
			gripperMove.inprogress = false;
			gripperMove.finished = false;
			return 1;
		}
	} else {
		msgd1 message;
		message.function = MSG_GRIPPERMOVE;
		message.d1 = pos;
		netConnection->Send(&message, sizeof(msgd1));
		gripperMove.inprogress = true;
	}
	return 0;
}

int PrimitivesNet::CalibrateConsole() {
	msgb1 message;
	message.function = MSG_CALIBRATECONSOLE;
	message.b1 = true;
	netConnection->Send(&message, sizeof(msgb1));
	return 1;
}

int PrimitivesNet::SetConsolePos(double pos, double max_speed, double max_acc) {
	if (consoleMove.inprogress) {
		if (consoleMove.finished) {
			consoleMove.inprogress = false;
			consoleMove.finished = false;
			return 1;
		}
	} else {
		msgd3 message;
		message.function = MSG_CONSOLEMOVE;
		message.d1 = pos;
		message.d2 = max_speed;
		message.d3 = max_acc;
		netConnection->Send(&message, sizeof(msgd3));
		consoleMove.inprogress = true;
	}
	return 0;
}

int PrimitivesNet::ConsoleStop() {
	msgb1 message;
	message.function = MSG_CONSOLESTOP;
	message.b1 = true;
	netConnection->Send(&message, sizeof(msgb1));
	return 1;
}

double PrimitivesNet::GetConsolePos() {
	return 0.;
}

int PrimitivesNet::SetArmPos(bool left, double pos, double max_speed, double max_acc) {
	progress* p;
	if (left) {
		p = &leftArmMove;
	} else {
		p = &rightArmMove;
	}
	if (p->inprogress) {
		if (p->finished) {
			p->inprogress = false;
			p->finished = false;
			return 1;
		}
	} else {
		msgarm message;
		message.function = MSG_ARMMOVE;
		message.left = left;
		message.pos = pos;
		message.speed = max_speed;
		message.acc = max_acc;
		netConnection->Send(&message, sizeof(msgarm));
		p->inprogress = true;
	}
	return 0;
}

int PrimitivesNet::Magnet(bool left, int polarity) {
	msgmagnet message;
	message.function = MSG_MAGNET;
	message.left = left;
	message.polarity = polarity;
	netConnection->Send(&message, sizeof(msgmagnet));
	return 1;
}

int PrimitivesNet::RefineDeadreckoning(double x, double y, double phi, double* dx, double* dy, double* dphi) {
	if (posRefine.inprogress) {
		if (posRefine.finished) {
			posRefine.inprogress = false;
			posRefine.finished = false;
			*dx = refineDelta.x;
			*dy = refineDelta.y;
			*dphi = refineDelta.phi;
			return 1;
		}
	} else {
		msgd3 message;
		message.function = MSG_POSREFINE;
		message.d1 = x;
		message.d2 = y;
		message.d3 = phi;
		netConnection->Send(&message, sizeof(msgd3));
		posRefine.inprogress = true;
	}
	*dx = 0;
	*dy = 0;
	*dphi = 0;
	return 0;
}

int PrimitivesNet::RefreshPawnPositions(msgpawns* pawns, double x, double y, double phi) {
	this->pawns = pawns;
	if (pawnRefresh.inprogress) {
		if (pawnRefresh.finished) {
			pawnRefresh.inprogress = false;
			pawnRefresh.finished = false;
			return 1;
		}
	} else {
		msgd3 message;
		message.function = MSG_PAWNS;
		message.d1 = x;
		message.d2 = y;
		message.d3 = phi;
		netConnection->Send(&message, sizeof(msgd3));
		pawnRefresh.inprogress = true;
	}
	return 0;
}
