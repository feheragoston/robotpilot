/*
 * Control.cpp
 *
 *  Created on: Mar 24, 2011
 *      Author: agoston
 */

//TODO: szimulacio kiszedheto
// Java
//TODO: minden aktuator lua kezelesben default seb/gyorsulasok

#include "Control.h"

using namespace std;

Config* Control::mConfig = NULL;
Primitives* Control::mPrimitives = NULL;
Server* Control::mServer = NULL;
nokia_server* Control::ns = NULL;
unsigned int Control::nokia_sent = 0;
int Control::logfile = 0;
bool Control::matchStarted = false;
bool Control::exitControl = false;
obstacleList Control::obstacles = obstacleList();
obstacleList Control::highObstacles = obstacleList();
obstacleList Control::dynObstacles = obstacleList(); // nem is kell
obstacleList Control::robotObstacles = obstacleList();
obstacleList Control::robotHighObstacles = obstacleList();
obstacleList Control::collisionObstacles = obstacleList();
bool Control::sendObstacles[MAX_CONNECTIONS] = {true, true, true, true, true, true, true, true, true, true};
bool Control::sendDynObstacles[MAX_CONNECTIONS] = {true, true, true, true, true, true, true, true, true, true};
Circle* Control::opponent[OPPONENT_NUM] = {NULL, NULL};
double Control::opponent_x[OPPONENT_NUM] = {-1100., -1100.};
double Control::opponent_y[OPPONENT_NUM] = {-1100., -1100.};
double Control::angry[OPPONENT_NUM] = {0., 0.};
bool Control::simulate = false;
bool Control::safeMotion = false;

struct timeval Control::runStart = {0, 0};
struct timeval Control::initStart = {0, 0};
struct timeval Control::matchStart = {0, 0};

bool Control::logObstacles = true;
bool Control::logDynObstacles = true;

//ezek a robot karjai, simulate hasznalja utkozesdetektalashoz.
//TODO: update ha kell
#define ROBOT_POINT_NUM 10
double Control::robotBody[][2] = {
		{ 155,  145},
		{  55,  145},
		{  25,  170},
		{ -25,  170},
		{-135,  100},
		{-135, -100},
		{ -25, -170},
		{  25, -170},
		{  55, -145},
		{ 155, -145}
};

Control::Control(Config* config) {
	mConfig = config;

	if (strlen(mConfig->LogFile)) {
		logfile = open(mConfig->LogFile, O_WRONLY | O_CREAT | O_TRUNC, 0664);

		if (logfile < 0) {
			cerr << "Error opening logfile: " << mConfig->LogFile << " " << logfile << endl;
			logfile = 0;
		}
	}

	this->L = luaL_newstate();
	luaL_openlibs(L);

	static const luaL_Reg controllib[] = {
		{"gettimeofday", c_gettimeofday},
		{"getelapsedtime", c_getelapsedtime},
		{"matchtime", c_matchtime},
		{"exit", c_exit},
		{"wait", c_wait},
		{"simulate", c_simulate},
		{"in_simulate", c_in_simulate},
		{"print", c_print},
		{"music", c_music},

		{"GetStartButton", l_GetStartButton},
		{"GetStopButton", l_GetStopButton},
		{"GetMyColor", l_GetMyColor},
		{"GetDistances", l_GetDistances},
		{"SetMotorSupply", l_SetMotorSupply},

		{"CalibrateDeadreckoning", l_CalibrateDeadreckoning},
		{"SetSpeed", l_SetSpeed},
		{"Go", l_Go},
		{"GoSafe", l_Go},
		{"GoTo", l_GoTo},
		{"GoToSafe", l_GoTo},
		{"Turn", l_Turn},
		{"TurnSafe", l_Turn},
		{"TurnTo", l_TurnTo},
		{"TurnToOri", l_TurnToOri},
		{"TurnToSafe", l_TurnTo},
		{"MotionInProgress", l_MotionInProgress},
		{"GetMotionError", l_GetMotionError},
		{"MotionStop", l_MotionStop},
		{"MotionStopInProgress", l_MotionStopInProgress},
		{"GetRobotPos", l_GetRobotPos},
		{"SetRobotPos", l_SetRobotPos},
		{"GetOpponentPos", l_GetOpponentPos},

#ifdef NAGY_ROBOT
		{"ArmMove", l_ArmMove},
		{"ArmMoveInProgress", l_ArmMoveInProgress},
		{"GetArmPos", l_GetArmPos},
		{"SelectorMove", l_SelectorMove},
		{"SelectorMoveInProgress", l_SelectorMoveInProgress},
		{"GetSelectorPos", l_GetSelectorPos},
		{"FlipperMove", l_FlipperMove},
		{"FlipperMoveInProgress", l_FlipperMoveInProgress},
		{"GetFlipperPos", l_GetFlipperPos},
		{"ContractorMove", l_ContractorMove},
		{"ContractorMoveInProgress", l_ContractorMoveInProgress},
		{"GetContractorPos", l_GetContractorPos},
		{"FireStopperMove", l_FireStopperMove},
		{"FireStopperMoveInProgress", l_FireStopperMoveInProgress},
		{"GetFireStopperPos", l_GetFireStopperPos},
#else	//KIS_ROBOT
		{"GripperMove", l_GripperMove},
		{"GripperMoveInProgress", l_GripperMoveInProgress},
		{"GetGripperPos", l_GetGripperPos},
#endif


#ifdef NAGY_ROBOT
		{"GetBallColorVoltage", l_GetBallColorVoltage},
		{"GetBallPresent", l_GetBallPresent},
		{"GetBallColor", l_GetBallColor},

		{"CaracoleSetSpeed", l_CaracoleSetSpeed},
		{"CaracoleSetSpeedInProgress", l_CaracoleSetSpeedInProgress},
		{"GetCaracoleSpeed", l_GetCaracoleSpeed},

		{"FirewheelSetSpeed", l_FirewheelSetSpeed},
		{"FirewheelSetSpeedInProgress", l_FirewheelSetSpeedInProgress},
		{"GetFirewheelSpeed", l_GetFirewheelSpeed},

		{"FireStopperMove", l_FireStopperMove},
		{"FireStopperMoveInProgress", l_FireStopperMoveInProgress},
		{"GetFireStopperPos", l_GetFireStopperPos},

#endif

		{"CalibrateConsole", l_CalibrateConsole},
		{"CalibrateConsoleInProgress", l_CalibrateConsoleInProgress},
		{"ConsoleMove", l_ConsoleMove},
		{"ConsoleMoveInProgress", l_ConsoleMoveInProgress},
		{"ConsoleStop", l_ConsoleStop},
		{"ConsoleStopInProgress", l_ConsoleStopInProgress},
		{"GetConsolePos", l_GetConsolePos},

		{"StartMatch", l_StartMatch},

		{"AddTestObstacles", l_AddTestObstacles},
		{"ClearDynObstacles", l_ClearDynObstacles},

#ifdef KIS_ROBOT
		{"FollowLine_Follow", l_FollowLine_Follow},
		{"FollowLine_FollowInProgress", l_FollowLine_FollowInProgress},
		{"FollowLine_GetFollowError", l_FollowLine_GetFollowError},
		{"FollowLine_Turn", l_FollowLine_Turn},
		{"FollowLine_TurnInProgress", l_FollowLine_TurnInProgress},
		{"FollowLine_Calibrate", l_FollowLine_Calibrate},
		{"FollowLine_CalibrateInProgress", l_FollowLine_CalibrateInProgress},
#endif
		{NULL, NULL}
	};

	luaL_register(L, "control", controllib);

	luaC_export(L, BLUE);
	luaC_export(L, RED);
	luaC_export(L, BALL_NOT_PRESENT);
	luaC_export(L, BALL_WHITE);
	luaC_export(L, BALL_RED);
	luaC_export(L, ROBOT_RADIUS);
	luaC_export(L, ROBOT_WIDTH);
	luaC_export(L, ROBOT_FRONT);
	luaC_export(L, ROBOT_FRONT_MAX);
	luaC_export(L, ROBOT_BACK);
	luaC_export(L, MAX_DEC);
	luaC_export(L, MAX_DISTANCE);
	luaC_export(L, OPPONENT_NUM);
	luaC_export(L, SONAR_TIMEOUT);

	luaC_export(L, PRIMITIVES_WAIT);

	luaL_dostring(L, "package.path = package.path .. \";./Pilot/?.lua\"; c = control; p = require(\"pilot\");");

	matchStarted = false;
	exitControl = false;

	if (obstacles.empty()) {
		//TODO: Peti: ezek aktualizalasa az uj palyahoz

		// Akadalyok definialasa
		// start vedo falak
		obstacles.push_back(new Line(500, 0, 500, 400));
		obstacles.push_back(new Line(500, AREA_LENGTH, 500, AREA_LENGTH - 400));

		// hold falak
		obstacles.push_back(new Line(AREA_WIDTH - 750, 370, AREA_WIDTH, 325));
		obstacles.push_back(new Line(AREA_WIDTH - 750, AREA_LENGTH - 370, AREA_WIDTH, AREA_LENGTH - 325));

		// palmafa
		highObstacles.push_back(new Circle(1000, 1500, 20));

		// totemek
		obstacles.push_back(new Line( 875,  975, 1125,  975));
		obstacles.push_back(new Line( 875,  975,  875, 1225));
		obstacles.push_back(new Line( 875, 1225, 1125, 1225));
		obstacles.push_back(new Line(1125,  975, 1125, 1225));

		obstacles.push_back(new Line( 875, 1775, 1125, 1775));
		obstacles.push_back(new Line( 875, 1775,  875, 2025));
		obstacles.push_back(new Line( 875, 2025, 1125, 2025));
		obstacles.push_back(new Line(1125, 1775, 1125, 2025));

		// totemek kozepe
		highObstacles.push_back(new Line( 965, 1065, 1035, 1065));
		highObstacles.push_back(new Line( 965, 1065,  965, 1135));
		highObstacles.push_back(new Line( 965, 1135, 1035, 1135));
		highObstacles.push_back(new Line(1035, 1065, 1035, 1135));

		highObstacles.push_back(new Line( 965, 1865, 1035, 1865));
		highObstacles.push_back(new Line( 965, 1865,  965, 1935));
		highObstacles.push_back(new Line( 965, 1935, 1035, 1935));
		highObstacles.push_back(new Line(1035, 1865, 1035, 1935));

		// Falak
		obstacles.push_back(new Line(0, 0, AREA_WIDTH, 0));
		obstacles.push_back(new Line(0, 0, 0, AREA_LENGTH));
		obstacles.push_back(new Line(0, AREA_LENGTH, AREA_WIDTH, AREA_LENGTH));
		obstacles.push_back(new Line(AREA_WIDTH, 0, AREA_WIDTH, AREA_LENGTH));

		// Gombok
		highObstacles.push_back(new Line(AREA_WIDTH - BUTTON_LENGTH, 640, AREA_WIDTH, 640));
		highObstacles.push_back(new Line(AREA_WIDTH - BUTTON_LENGTH, 1117, AREA_WIDTH, 1117));
		highObstacles.push_back(new Line(AREA_WIDTH - BUTTON_LENGTH, 1883, AREA_WIDTH, 1883));
		highObstacles.push_back(new Line(AREA_WIDTH - BUTTON_LENGTH, 2360, AREA_WIDTH, 2360));
	}
	// ellenfelet alapbol kirakjuk a palyarol
	for (int i = 0; i < OPPONENT_NUM; i++) {
		opponent[i] = new Circle(-1000, -1000, 1);
	}
}

Control::~Control() {
	if (logfile) {
		close(logfile);
		logfile = 0;
	}
	if (mPrimitives) {
		delete mPrimitives;
	}
	if (mServer) {
		delete mServer;
	}
	if (ns) {
		delete ns;
	}
	lua_close(L);

	while (!obstacles.empty()) {
		delete obstacles.front();
		obstacles.pop_front();
	}
	while (!dynObstacles.empty()) {
		delete dynObstacles.front();
		dynObstacles.pop_front();
	}

	while (!robotObstacles.empty()) {
		delete robotObstacles.front();
		robotObstacles.pop_front();
	}

	while (!robotHighObstacles.empty()) {
		delete robotHighObstacles.front();
		robotHighObstacles.pop_front();
	}

	for (int i = 0; i < OPPONENT_NUM; i++) {
		delete opponent[i];
	}
}

bool Control::Init() {
	if (sizeof(msgstatus) > 255) {
		cout << "(Control) Fatal error: msgstatus is too big" << endl;
		return false;
	}

	gettimeofday(&runStart, NULL);
	gettimeofday(&initStart, NULL);
	gettimeofday(&matchStart, NULL);

	if (mConfig->PrimitivesCan) {
		mPrimitives = new PrimitivesCan(mConfig);
	} else if (mConfig->PrimitivesNet) {//ez a szim, nem kell
		mPrimitives = new PrimitivesNet(mConfig);
	} else {
		mPrimitives = new Primitives(mConfig);
	}

	if (mPrimitives->Init()) {
		mServer = new Server();
		mServer->SetMessageCallback(serverMessageCallback);
		mServer->Listen(13001);

		if (mConfig->NokiaServer) {
			ns = new nokia_server();
		}
		return true;
	}

	return false;
}

void Control::Run() {
	gettimeofday(&runStart, NULL);

	if (mPrimitives) {

		int s;

		if (strlen(mConfig->LuaFile)) {
			s = luaL_loadfile(L, mConfig->LuaFile);
			if (s == 0) {
				// execute Lua program
				s = lua_pcall(L, 0, LUA_MULTRET, 0);
			}
			report_errors(L, s);

			std::cout << "(Control) Finished running " << mConfig->LuaFile << std::endl;
		}

		//ez az interaktiv parancs mod

		fd_set rfd;
		char buf[2048];
		size_t nbytes = sizeof(buf) - 1;
		ssize_t bytes_read;
		struct timeval tv;
		tv.tv_sec = 0;
		tv.tv_usec = 0;
		string luaBuffer = "";

		while (!exitControl) {
			lua_getfield(L, LUA_GLOBALSINDEX, "control");
			lua_getfield(L, -1, "wait");
			lua_remove(L, -2);
			lua_pushinteger(L, PRIMITIVES_WAIT);
			s = lua_pcall(L, 1, 0, 0);
			report_errors(L, s);

			bool noread = true;
			while (true) {
				FD_ZERO(&rfd);
				FD_SET(0, &rfd);
				if (select(1, &rfd, 0, 0, &tv) > 0) {
					bytes_read = read(0, buf, nbytes);
					buf[bytes_read] = 0;
					luaBuffer.append(buf);
					noread = false;
				} else {
					if (noread && !luaBuffer.empty()) {
						// ha csak egy sortores van
						if (luaBuffer.find('\n') == luaBuffer.rfind('\n')) {
							// es nincs benne ;=
							if (luaBuffer.find_first_of(";=") == string::npos) {
								// akkor korbevesszuk Print()-el
								sprintf(buf, "control.print(%s);\n", luaBuffer.c_str());
								luaBuffer.clear();
								luaBuffer.append(buf);
							}
						}
						s = luaL_loadstring(L, luaBuffer.c_str());
						luaBuffer.clear();
						if (s == 0) {
							s = lua_pcall(L, 0, LUA_MULTRET, 0);
						}
						report_errors(L, s);
					}
					break;
				}
			}
		}
	}
}

//ez kuldi a java programnak mit rajzoljon
void Control::serverMessageCallback(int n, const void* message, msglen_t size) {
	if (size == 0) {
		sendObstacles[n] = true;
		sendDynObstacles[n] = true;
		return;
	}

	function_t* function = (function_t*) message;
	if (*function == MSG_REFRESHSTATUS && size == sizeof(msgb1)) {
		msgstatus response;
		response.function = MSG_REFRESHSTATUS;
		mPrimitives->GetRobotPos(&(response.x), &(response.y), &(response.phi));
		mPrimitives->GetSpeed(&(response.v), &(response.w));
		for (unsigned char i = 0; i < OPPONENT_NUM; i++) {
			Control::GetOpponentPos(i, &(response.ox[i]), &(response.oy[i]));
			response.oradius[i] = opponent[i]->getRadius();
		}
		response.startButton = mPrimitives->GetStartButton();
		response.stopButton = mPrimitives->GetStopButton();
		response.color = mPrimitives->GetMyColor();
		response.motorSupply = mPrimitives->GetMotorSupply();
#ifdef NAGY_ROBOT
		response.frontArmPos = mPrimitives->GetArmPos(true);
		response.rearArmPos = mPrimitives->GetArmPos(false);
		response.selectorPos = mPrimitives->GetSelectorPos();
		response.flipperPos = mPrimitives->GetFlipperPos();
		response.contractorPos = mPrimitives->GetContractorPos();
#else	//KIS_ROBOT
		response.lowLeftGripperPos = mPrimitives->GetGripperPos(true, true);
		response.lowRightGripperPos = mPrimitives->GetGripperPos(true, false);
		response.highLeftGripperPos = mPrimitives->GetGripperPos(false, true);
		response.highRightGripperPos = mPrimitives->GetGripperPos(false, false);
#endif
		response.consolePos = mPrimitives->GetConsolePos();
		mPrimitives->GetDistances(response.distances);
		mServer->Send(n, &response, sizeof(msgstatus));

		msgshapes rob;
		rob.function = MSG_SHAPES;
		rob.num = min(robotObstacles.size(), 14);
		rob.id = 0;
		rob.color[0] = 0;
		rob.color[1] = 255;
		rob.color[2] = 128;
		rob.color[3] = 255;
		obstacleIterator o = robotObstacles.begin();
		for (unsigned int i = 0; i < rob.num; i++) {
			(*o)->getObstacle(&rob.obstacles[i]);
			o++;
		}
		mServer->Send(n, &rob, sizeof(msgshapes));

		rob.num = min(robotHighObstacles.size(), 14);
		rob.id = 1;
		rob.color[0] = 255;
		rob.color[1] = 200;
		rob.color[2] = 200;
		rob.color[3] = 255;
		o = robotHighObstacles.begin();
		for (unsigned int i = 0; i < rob.num; i++) {
			(*o)->getObstacle(&rob.obstacles[i]);
			o++;
		}
		mServer->Send(n, &rob, sizeof(msgshapes));

		if (sendObstacles[n]) {
			sendObstacles[n] = false;

			msgshapes obs;
			obs.function = MSG_SHAPES;

			obs.id = 2;
			obs.num = min(obstacles.size(), 14);
			obs.color[0] = 255;
			obs.color[1] = 255;
			obs.color[2] = 0;
			obs.color[3] = 255;
			obstacleIterator o = obstacles.begin();
			for (unsigned int i = 0; i < obs.num; i++) {
				(*o)->getObstacle(&obs.obstacles[i]);
				o++;
			}
			mServer->Send(n, &obs, sizeof(msgshapes));

			obs.id = 3;
			obs.num = min(highObstacles.size(), 14);
			obs.color[0] = 255;
			obs.color[1] = 0;
			obs.color[2] = 255;
			obs.color[3] = 255;
			o = highObstacles.begin();
			for (unsigned int i = 0; i < obs.num; i++) {
				(*o)->getObstacle(&obs.obstacles[i]);
				o++;
			}
			mServer->Send(n, &obs, sizeof(msgshapes));
		}

		if (sendDynObstacles[n]) {
			sendDynObstacles[n] = false;

			msgshapes obs;
			obs.function = MSG_SHAPES;
			obs.id = 4;
			obs.num = min(dynObstacles.size(), 14);
			obs.color[0] = 255;
			obs.color[1] = 128;
			obs.color[2] = 0;
			obs.color[3] = 128;
			obstacleIterator o = dynObstacles.begin();
			for (unsigned int i = 0; i < obs.num; i++) {
				(*o)->getObstacle(&obs.obstacles[i]);
				o++;
			}
			mServer->Send(n, &obs, sizeof(msgshapes));

			if (collisionObstacles.size() > 0) {
				obs.id = 5;
				obs.num = min(collisionObstacles.size(), 14);
				obs.color[0] = 255;
				obs.color[1] = 0;
				obs.color[2] = 0;
				obs.color[3] = 255;
				o = collisionObstacles.begin();
				for (unsigned int i = 0; i < obs.num; i++) {
					(*o)->getObstacle(&obs.obstacles[i]);
					o++;
				}
				mServer->Send(n, &obs, sizeof(msgshapes));
			}
		}

	} else if (*function == MSG_VISIONTEST && size == 1) {
		msgd3 response;
		response.function = MSG_VISIONTEST;
		mPrimitives->GetRobotPos(&(response.d1), &(response.d2), &(response.d3));
		mServer->Send(n, &response, sizeof(msgd3));
	} else if (size != 0) {
		std::cout << "(Control) Unknown or invalid function: " << (int)*function << " size: " << (int)size << std::endl;
	}
}

//update az uj robotra, struktura java-val egyezen
void Control::log() {
	if (logfile) {
		unsigned int time = InitTime();
		write(logfile, &time, sizeof(unsigned int));
		time = MatchTime();
		write(logfile, &time, sizeof(unsigned int));

		msglen_t size;

		msgstatus status;
		status.function = MSG_REFRESHSTATUS;
		mPrimitives->GetRobotPos(&(status.x), &(status.y), &(status.phi));
		mPrimitives->GetSpeed(&(status.v), &(status.w));
		for (unsigned char i = 0; i < OPPONENT_NUM; i++) {
			Control::GetOpponentPos(i, &(status.ox[i]), &(status.oy[i]));
			status.oradius[i] = opponent[i]->getRadius();
		}
		status.startButton = mPrimitives->GetStartButton();
		status.stopButton = mPrimitives->GetStopButton();
		status.color = mPrimitives->GetMyColor();
		status.motorSupply = mPrimitives->GetMotorSupply();
#ifdef NAGY_ROBOT
		status.frontArmPos = mPrimitives->GetArmPos(true);
		status.rearArmPos = mPrimitives->GetArmPos(false);
		status.selectorPos = mPrimitives->GetSelectorPos();
		status.flipperPos = mPrimitives->GetFlipperPos();
		status.contractorPos = mPrimitives->GetContractorPos();
		status.firestopperPos = mPrimitives->GetFireStopperPos();
#else	//KIS_ROBOT
		status.lowLeftGripperPos = mPrimitives->GetGripperPos(true, true);
		status.lowRightGripperPos = mPrimitives->GetGripperPos(true, false);
		status.highLeftGripperPos = mPrimitives->GetGripperPos(false, true);
		status.highRightGripperPos = mPrimitives->GetGripperPos(false, false);
#endif
		status.consolePos = mPrimitives->GetConsolePos();
		mPrimitives->GetDistances(status.distances);
		size = sizeof(msgstatus);
		write(logfile, &size, sizeof(msglen_t));
		write(logfile, &status, size);

		msgshapes rob;
		rob.function = MSG_SHAPES;
		rob.num = min(robotObstacles.size(), 14);
		rob.id = 0;
		rob.color[0] = 0;
		rob.color[1] = 255;
		rob.color[2] = 128;
		rob.color[3] = 255;
		obstacleIterator o = robotObstacles.begin();
		for (unsigned int i = 0; i < rob.num; i++) {
			(*o)->getObstacle(&rob.obstacles[i]);
			o++;
		}
		size = sizeof(msgshapes);
		write(logfile, &size, sizeof(msglen_t));
		write(logfile, &rob, size);

		rob.num = min(robotHighObstacles.size(), 14);
		rob.id = 1;
		rob.color[0] = 255;
		rob.color[1] = 200;
		rob.color[2] = 200;
		rob.color[3] = 255;
		o = robotHighObstacles.begin();
		for (unsigned int i = 0; i < rob.num; i++) {
			(*o)->getObstacle(&rob.obstacles[i]);
			o++;
		}
		size = sizeof(msgshapes);
		write(logfile, &size, sizeof(msglen_t));
		write(logfile, &rob, size);

		if (logObstacles) {
			logObstacles = false;
			msgshapes obs;
			obs.function = MSG_SHAPES;

			obs.id = 2;
			obs.num = min(obstacles.size(), 14);
			obs.color[0] = 255;
			obs.color[1] = 255;
			obs.color[2] = 0;
			obs.color[3] = 255;
			obstacleIterator o = obstacles.begin();
			for (unsigned int i = 0; i < obs.num; i++) {
				(*o)->getObstacle(&obs.obstacles[i]);
				o++;
			}
			size = sizeof(msgshapes);
			write(logfile, &size, sizeof(msglen_t));
			write(logfile, &obs, size);

			obs.id = 3;
			obs.num = highObstacles.size();
			obs.color[0] = 255;
			obs.color[1] = 0;
			obs.color[2] = 255;
			obs.color[3] = 255;
			o = highObstacles.begin();
			for (unsigned int i = 0; i < obs.num; i++) {
				(*o)->getObstacle(&obs.obstacles[i]);
				o++;
			}
			size = sizeof(msgshapes);
			write(logfile, &size, sizeof(msglen_t));
			write(logfile, &obs, size);
		}
		if (logDynObstacles) {
			logDynObstacles = false;
			msgshapes obs;
			obs.function = MSG_SHAPES;
			obs.id = 4;
			obs.num = dynObstacles.size();
			obs.color[0] = 255;
			obs.color[1] = 128;
			obs.color[2] = 0;
			obs.color[3] = 128;
			obstacleIterator o = dynObstacles.begin();
			for (unsigned int i = 0; i < obs.num; i++) {
				(*o)->getObstacle(&obs.obstacles[i]);
				o++;
			}
			size = sizeof(msgshapes);
			write(logfile, &size, sizeof(msglen_t));
			write(logfile, &obs, size);

			if (collisionObstacles.size() > 0) {
				obs.id = 5;
				obs.num = collisionObstacles.size();
				obs.color[0] = 255;
				obs.color[1] = 0;
				obs.color[2] = 0;
				obs.color[3] = 255;
				o = collisionObstacles.begin();
				for (unsigned int i = 0; i < obs.num; i++) {
					(*o)->getObstacle(&obs.obstacles[i]);
					o++;
				}
				size = sizeof(msgshapes);
				write(logfile, &size, sizeof(msglen_t));
				write(logfile, &obs, size);
			}
		}

		function_t stop = 0xFF;
		if (write(logfile, &stop, sizeof(function_t)) < 0) {
			cerr << "Error writing log, closing log" << endl;
			close(logfile);
			logfile = 0;
		}
	}
}

//ha a meghivott fuggveny neveben benne van a Safe, safeMotion true lesz
void Control::setSafeMotion(lua_State *L) {
	lua_Debug ar;
	lua_getstack(L, 0, &ar);
	lua_getinfo(L, "nS", &ar);
	safeMotion = false;
//	if (ar.name != NULL) {
//		cout << "setSafeMotion: " << ar.name << endl;
//	} else {
//		cout << "setSafeMotion: NULL" << endl;
//	}
	if (ar.name == NULL || strstr(ar.name, "Safe") != NULL) {
		safeMotion = true;
	}
}

//TODO: opponent-re fix ROBOT_RADIUS-t hasznal, ez kis/nagy robotra azonos??
long int Control::refreshOpponent(unsigned char n) {
	if (n < OPPONENT_NUM) {
		double ox, oy;
		long int validity = Control::GetOpponentPos(n, &ox, &oy);
		opponent[n]->Set(ox, oy, ROBOT_RADIUS * 2.5 - angry[n]);	//sajat magunk is ebben van
		return validity;
	}
	return 0;
}

//ezt a motionInProgress hasznalja, eldonti hogy van e a mozgasunkhoz kepest ellenfel akivel utkoznenk
bool Control::opponentTooClose() {


	//elosszor ellenorizzuk, hogy az ellenfel meg a helyen van-e. ha igen, biztos nem utkozunk
	//TODO: update az idei szabalyra, hol van a kiindulo terulet?
	bool theyStuck = true;

	for (unsigned char i = 0; i < OPPONENT_NUM; i++) {
		double ox, oy;
		long int validity = Control::GetOpponentPos(i, &ox, &oy);
		if (!(validity < SONAR_TIMEOUT && ox < 500 && (oy < 500 || oy > 2500))) {
			theyStuck = false;
		}
	}

	if (theyStuck) {
		return false;
	}

	//ide jututnk ha az ellenfel mar nincs a helyen
	double x, y, phi, v, w;
	mPrimitives->GetRobotPos(&x, &y, &phi);
	mPrimitives->GetSpeed(&v, &w);

	//ha mar nagyon lassan megyunk, nem lehet gond
	//TODO: ez biztos jo lesz igy?
	if (fabs(v) < 10) {
		return false;
	}

	//distance> mostani sebesseggel milyen tavolsagra tud megallni, negativ ha hatrafele
	double distance = v * v / (2 * MAX_DEC);
	if (v < 0.) {
		distance += ROBOT_BACK;
		distance *= -1;
	} else {
		distance += ROBOT_FRONT;
	}

	//megnezzuk, hogy minden sonar meres ervenyes-e. ha nem, tavolsagmeroket hasznaljuk
	long int validity = 0;
	for (unsigned char i = 0; i < OPPONENT_NUM; i++) {
		validity = max(validity, refreshOpponent(i));
	}
	//TODO: ezt valahogy jelezni. iden is elofordulhat?
	//TODO: iden beloghat valami a tavolsagmerok ele? update pozicioja kozephez kepest

	if (validity > SONAR_TIMEOUT/* && !mPrimitives->ArmMoveInProgress()*/
			&& (x < 1600 || abs(phi) > M_PI / 2)) {  //itt mar a falnal vagyunk, ne nezze
		if (distance > 0) {
			distance += 100; // a tavolsagerzekelok hatrebb vannak
			double distances[PROXIMITY_NUM];
			mPrimitives->GetDistances(distances);
			if (distances[0] < distance || distances[1] < distance) {
				cout << "(Control) Sonar timeout, opponent too close!" << endl;
				// elhelyezzuk az ellenfelet, hogy legkozelebb ne probaljunk erre menni
				double ox = x + (distance + ROBOT_RADIUS * 2.5 - angry[0]) * cos(phi);
				double oy = y + (distance + ROBOT_RADIUS * 2.5 - angry[0]) * sin(phi);
				SetOpponentPos(0, ox, oy);
				return true;
			}
		}
		//TODO: ugyan ezt hatra
		return false;
	}

	//mukodik a meres, ellenorizzuk az ellenfeleket
	for (unsigned char i = 0; i < OPPONENT_NUM; i++) {
		if (opponent[i]->Intersect(x, y, x + distance * cos(phi), y + distance * sin(phi))) {
			if (!simulate && angry[i] < ROBOT_RADIUS) {
				angry[i] += 5.;
			}
			//std::cout << "(Control) Opponent " << i << " too close! angry: " << angry[i] << std::endl;
			return true;
		}
		if (!simulate && angry[i] > 0) {
			angry[i] -= 0.5;
		}
	}
	return false;
}

//obstacleCollision: megmondja hogy epp osszeutkozunk-e valamivel, mechanikai elemek allasatol fuggoen
//frissiti a robotObstacles es robotHighObstacles-t, simulate mode-ban hasznaljuk
bool Control::obstacleCollision() {
	double x, y, phi, lg, rg, lc, rc;
	mPrimitives->GetRobotPos(&x, &y, &phi);

	//TODO: ezt a mostani mechanikara frissiteni

	/*
	lg = mPrimitives->GetArmPos(true);
	rg = mPrimitives->GetArmPos(false);
	lc = mPrimitives->GetClawPos(true);
	rc = mPrimitives->GetClawPos(false);

	while (!robotObstacles.empty()) {
		delete robotObstacles.front();
		robotObstacles.pop_front();
	}
	while (!robotHighObstacles.empty()) {
		delete robotHighObstacles.front();
		robotHighObstacles.pop_front();
	}

	if (lc > 0) {
		//129
		// atvaltjuk radianba
		lc = lc * M_PI / 180.;
		// a kar kezdopontja
		double x1 = cos(phi) * 137 - sin(phi) * 133 + x;
		double y1 = sin(phi) * 137 + cos(phi) * 133 + y;
		// a kar kozeppontja 0 pozicioban
		double x2 = cos(phi) * 137 - sin(phi) * 4 + x;
		double y2 = sin(phi) * 137 + cos(phi) * 4 + y;
		// eltoljuk a forgatas kozeppontjaval
		double xo = x2 - x1;
		double yo = y2 - y1;
		// elforgatjuk es visszatoljuk
		x2 = cos(lc) * xo - sin(lc) * yo + x1;
		y2 = sin(lc) * xo + cos(lc) * yo + y1;
		robotHighObstacles.push_back(new Line(x1, y1, x2, y2));

		// a kar vegpontja 0 pozicioban
		double x3 = cos(phi) * 115 - sin(phi) * -73 + x;
		double y3 = sin(phi) * 115 + cos(phi) * -73 + y;
		// eltoljuk a forgatas kozeppontjaval
		xo = x3 - x1;
		yo = y3 - y1;
		// elforgatjuk es visszatoljuk
		x3 = cos(lc) * xo - sin(lc) * yo + x1;
		y3 = sin(lc) * xo + cos(lc) * yo + y1;
		robotHighObstacles.push_back(new Line(x2, y2, x3, y3));

		if (checkLine(x1, y1, x2, y2, COLLISION_HIGH)
				|| checkLine(x2, y2, x3, y3, COLLISION_HIGH)) {
			return true;
		}
	}
	if (rc > 0) {
		// atvaltjuk radianba es negaljuk, mert a jobb kar ellentetesen forog
		rc = -rc * M_PI / 180.;
		// a kar kezdopontja
		double x1 = cos(phi) * 137 - sin(phi) * -133 + x;
		double y1 = sin(phi) * 137 + cos(phi) * -133 + y;
		// a kar vegpontja 0 pozicioban
		double x2 = cos(phi) * 137 - sin(phi) * -4 + x;
		double y2 = sin(phi) * 137 + cos(phi) * -4 + y;
		// eltoljuk a forgatas kozeppontjaval
		double xo = x2 - x1;
		double yo = y2 - y1;
		// elforgatjuk es visszatoljuk
		x2 = cos(rc) * xo - sin(rc) * yo + x1;
		y2 = sin(rc) * xo + cos(rc) * yo + y1;
		robotHighObstacles.push_back(new Line(x1, y1, x2, y2));

		// a kar vegpontja 0 pozicioban
		double x3 = cos(phi) * 115 - sin(phi) * 73 + x;
		double y3 = sin(phi) * 115 + cos(phi) * 73 + y;
		// eltoljuk a forgatas kozeppontjaval
		xo = x3 - x1;
		yo = y3 - y1;
		// elforgatjuk es visszatoljuk
		x3 = cos(rc) * xo - sin(rc) * yo + x1;
		y3 = sin(rc) * xo + cos(rc) * yo + y1;
		robotHighObstacles.push_back(new Line(x2, y2, x3, y3));

		if (checkLine(x1, y1, x2, y2, COLLISION_HIGH)
				|| checkLine(x2, y2, x3, y3, COLLISION_HIGH)) {
			return true;
		}
	}

	for (int j = 0; j < ROBOT_POINT_NUM; j++) {
		int j2 = (j+1)%ROBOT_POINT_NUM;
		double rx1 = robotBody[j][0];
		double ry1 = robotBody[j][1];
		double rx2 = robotBody[j2][0];
		double ry2 = robotBody[j2][1];

		if (j == 0 || j == ROBOT_POINT_NUM) {
			rx1 += sin(gripperPos) * 105;
		}
		if (j2 == 0 || j2 == ROBOT_POINT_NUM) {
			rx2 += sin(gripperPos) * 105;
		}

		double x1 = cos(phi) * rx1 - sin(phi) * ry1 + x;
		double y1 = sin(phi) * rx1 + cos(phi) * ry1 + y;
		double x2 = cos(phi) * rx2 - sin(phi) * ry2 + x;
		double y2 = sin(phi) * rx2 + cos(phi) * ry2 + y;

		robotObstacles.push_back(new Line(x1, y1, x2, y2));

		if (checkLine(x1, y1, x2, y2, COLLISION_OBSTACLES)) {
			return true;
		}
	}

	if (lg > 0) {
		// atvaltjuk radianba
		lg = lg * M_PI / 180.;
		// a kar kezdopontja
		double x1 = cos(phi) * 137 - sin(phi) * 133 + x;
		double y1 = sin(phi) * 137 + cos(phi) * 133 + y;
		// a kar kozeppontja 0 pozicioban
		double x2 = cos(phi) * 137 - sin(phi) * 5 + x;
		double y2 = sin(phi) * 137 + cos(phi) * 5 + y;
		// eltoljuk a forgatas kozeppontjaval
		double xo = x2 - x1;
		double yo = y2 - y1;
		// elforgatjuk es visszatoljuk
		x2 = cos(lg) * xo - sin(lg) * yo + x1;
		y2 = sin(lg) * xo + cos(lg) * yo + y1;
		robotObstacles.push_back(new Line(x1, y1, x2, y2));

		// a kar vegpontja 0 pozicioban
		double x3 = cos(phi) * 127 - sin(phi) * -75 + x;
		double y3 = sin(phi) * 127 + cos(phi) * -75 + y;
		// eltoljuk a forgatas kozeppontjaval
		xo = x3 - x1;
		yo = y3 - y1;
		// elforgatjuk es visszatoljuk
		x3 = cos(lg) * xo - sin(lg) * yo + x1;
		y3 = sin(lg) * xo + cos(lg) * yo + y1;
		robotObstacles.push_back(new Line(x2, y2, x3, y3));

		if (checkLine(x1, y1, x2, y2, COLLISION_OBSTACLES)
				|| checkLine(x2, y2, x3, y3, COLLISION_OBSTACLES)) {
			return true;
		}
	}
	if (rg > 0) {
		// atvaltjuk radianba es negaljuk, mert a jobb kar ellentetesen forog
		rg = -rg * M_PI / 180.;
		// a kar kezdopontja
		double x1 = cos(phi) * 137 - sin(phi) * -133 + x;
		double y1 = sin(phi) * 137 + cos(phi) * -133 + y;
		// a kar vegpontja 0 pozicioban
		double x2 = cos(phi) * 137 - sin(phi) * -5 + x;
		double y2 = sin(phi) * 137 + cos(phi) * -5 + y;
		// eltoljuk a forgatas kozeppontjaval
		double xo = x2 - x1;
		double yo = y2 - y1;
		// elforgatjuk es visszatoljuk
		x2 = cos(rg) * xo - sin(rg) * yo + x1;
		y2 = sin(rg) * xo + cos(rg) * yo + y1;
		robotObstacles.push_back(new Line(x1, y1, x2, y2));

		// a kar vegpontja 0 pozicioban
		double x3 = cos(phi) * 127 - sin(phi) * 75 + x;
		double y3 = sin(phi) * 127 + cos(phi) * 75 + y;
		// eltoljuk a forgatas kozeppontjaval
		xo = x3 - x1;
		yo = y3 - y1;
		// elforgatjuk es visszatoljuk
		x3 = cos(rg) * xo - sin(rg) * yo + x1;
		y3 = sin(rg) * xo + cos(rg) * yo + y1;
		robotObstacles.push_back(new Line(x2, y2, x3, y3));

		if (checkLine(x1, y1, x2, y2, COLLISION_OBSTACLES)
				|| checkLine(x2, y2, x3, y3, COLLISION_OBSTACLES)) {
			return true;
		}
	}
	*/
	return false;
}


//letett tornyok, mar nem kell
void Control::addDynamicObstacle(Obstacle* obstacle) {
	double x, y, phi;
	mPrimitives->GetRobotPos(&x, &y, &phi);

	for (int j = 0; j < ROBOT_POINT_NUM; j++) {
		int j2 = (j+1)%ROBOT_POINT_NUM;
		double x1 = cos(phi) * robotBody[j][0] - sin(phi) * robotBody[j][1] + x;
		double y1 = sin(phi) * robotBody[j][0] + cos(phi) * robotBody[j][1] + y;
		double x2 = cos(phi) * robotBody[j2][0] - sin(phi) * robotBody[j2][1] + x;
		double y2 = sin(phi) * robotBody[j2][0] + cos(phi) * robotBody[j2][1] + y;
		if (obstacle->Intersect(x1, y1, x2, y2)) {
			delete obstacle;
			return;
		}
	}
	dynObstacles.push_back(obstacle);
	logDynObstacles = true;
	for (int i = 0; i < MAX_CONNECTIONS; i++) {
		sendDynObstacles[i] = true;
	}
	//cout << "(Control) Number of dynamic obstacles: " << dynObstacles.size() << endl;
}

void Control::clearCollisionObstacles() {
	collisionObstacles.clear();
	logDynObstacles = true;
	for (int i = 0; i < MAX_CONNECTIONS; i++) {
		sendDynObstacles[i] = true;
	}
}

unsigned int Control::RunTime() {
	struct timeval operateTime;
	struct timeval timeDiff;
	unsigned int runTime;

	gettimeofday(&operateTime, NULL);
	tools::timeval_subtract(&timeDiff, &operateTime, &runStart);
	runTime = (timeDiff.tv_usec / 1000) + timeDiff.tv_sec * 1000;
	return runTime;
}

unsigned int Control::InitTime() {
	struct timeval operateTime;
	struct timeval timeDiff;
	unsigned int runTime;

	gettimeofday(&operateTime, NULL);
	tools::timeval_subtract(&timeDiff, &operateTime, &initStart);
	runTime = (timeDiff.tv_usec / 1000) + timeDiff.tv_sec * 1000;
	return runTime;
}

unsigned int Control::MatchTime() {
	if (!matchStarted) {
		return 0;
	}
	struct timeval operateTime;
	struct timeval timeDiff;
	unsigned int runTime;

	gettimeofday(&operateTime, NULL);

	tools::timeval_subtract(&timeDiff, &operateTime, &matchStart);

	runTime = (timeDiff.tv_usec / 1000) + timeDiff.tv_sec * 1000;
	return runTime;
}

bool Control::checkLine(double x1, double y1, double x2, double y2, int mode) {
	if (mode == COLLISION_ALL) {
		for (unsigned char i = 0; i < OPPONENT_NUM; i++) {
			if (opponent[i]->Intersect(x1, y1, x2, y2)) {
				return true;
			}
		}
	}

	if (mode == COLLISION_ALL || mode == COLLISION_OBSTACLES) {
		for (obstacleIterator i = obstacles.begin(); i != obstacles.end(); i++) {
			if ((*i)->Intersect(x1, y1, x2, y2)) {
				clearCollisionObstacles();
				collisionObstacles.push_back(*i);
				return true;
			}
		}

		//TODO: ezeket hasznaljuk most??
		for (obstacleIterator i = dynObstacles.begin(); i != dynObstacles.end(); i++) {
			if ((*i)->Intersect(x1, y1, x2, y2)) {
				clearCollisionObstacles();
				collisionObstacles.push_back(*i);
				return true;
			}
		}
	}

	//TODO: ezt csak ha HIGH_OBSTACLES modban hivjuk

	for (obstacleIterator i = highObstacles.begin(); i != highObstacles.end(); i++) {
		if ((*i)->Intersect(x1, y1, x2, y2)) {
			clearCollisionObstacles();
			collisionObstacles.push_back(*i);
			return true;
		}
	}

	return false;
}

//control.print fuggvenyt hivja a lua error-al
void Control::report_errors(lua_State *L, int status) {
	if (status != 0) {
		//std::cerr << "-- " << lua_tostring(L, -1) << std::endl;
		//lua_pop(L, 1); // remove error message
		lua_getfield(L, LUA_GLOBALSINDEX, "control");
		lua_getfield(L, -1, "print");
		lua_remove(L, -2);
		lua_insert(L, -2);
		lua_pushstring(L, "-- lua error:");
		lua_insert(L, -2);
		lua_pcall(L, 2, 0, 0);
	}
}

//lua-bol adott indexu parametert bool-la konvertal, ha nem bool volt default d ertek lesz
bool Control::optbool(lua_State *L, int narg, bool d) {
	if (lua_isboolean(L, narg)) {
		return lua_toboolean(L, narg);
	}
	return d;
}


int Control::c_gettimeofday(lua_State *L) {
	gettimeofday((struct timeval*)lua_newuserdata(L, sizeof(struct timeval)), NULL);
	return 1;
}

int Control::c_getelapsedtime(lua_State *L) {
	if (lua_isuserdata(L, 1)) {
		struct timeval *since;
		since = (struct timeval*)lua_touserdata(L, 1);
		struct timeval operateTime;
		struct timeval timeDiff;

		gettimeofday(&operateTime, NULL);

		tools::timeval_subtract(&timeDiff, &operateTime, since);
		lua_pushinteger(L, timeDiff.tv_usec + timeDiff.tv_sec * 1000 * 1000);
		return 1;
	} else {
		lua_pushnil(L);
		return 1;
	}
}

int Control::c_matchtime(lua_State *L) {
	lua_pushinteger(L, MatchTime());
	return 1;
}

int Control::c_exit(lua_State *L) {
	exitControl = true;
	luaL_error(L, "(Control) exit() called, exiting");
	return 0;
}

int Control::c_wait(lua_State *L) {
	long int useconds = luaL_optinteger(L, 1, PRIMITIVES_WAIT);

	/* Primitives statuszfrissitest varunk */
	if (!mPrimitives->Wait(useconds)) {
		exitControl = true;
		return luaL_error(L, "(Control) Primitives->Wait failed, exiting");
	}

	/* logolunk es a csatlakozott klienseket feldolgozzuk */
	log();
	mServer->Process();
	//TODO: nokiat ideire update
	if (mConfig->NokiaServer && (InitTime() > nokia_sent)) {
		double x, y, phi;
		ns->pause_sending();
		mPrimitives->GetRobotPos(&x, &y, &phi);
		ns->set_location(x, y, phi);
		mPrimitives->GetOpponentPos(0, &x, &y);
		ns->set_opponent("Tom", x, y, 0, ROBOT_RADIUS);
		mPrimitives->GetOpponentPos(1, &x, &y);
		ns->set_opponent("Jerry", x, y, 0, ROBOT_RADIUS);
		ns->send();
		nokia_sent = InitTime() + 100;
	}

	/* statusz ellenorzesek */
	if (mPrimitives->GetStopButton()) {
		if (mPrimitives->GetMotorSupply()) {
			mPrimitives->SetMotorSupply(false);
		}
		if (!mPrimitives->SetMotorSupplyInProgress()) {
			exitControl = true;
			int fp = open("/tmp/playlist", O_WRONLY | O_CREAT | O_TRUNC, 0664);
			write(fp, "stop", 4);
			close(fp);
		}
		return luaL_error(L, "(Control) Stop button, exiting");
	} else if (MatchTime() > 90000) {
		if (mPrimitives->GetMotorSupply()) {
			mPrimitives->SetMotorSupply(false);
		}
		if (!mPrimitives->SetMotorSupplyInProgress()) {
			exitControl = true;
			int fp = open("/tmp/playlist", O_WRONLY | O_CREAT | O_TRUNC, 0664);
			write(fp, "matchover", 9);
			close(fp);
		}
		cout << "(Control) Meccs ido letelt, kilepunk" << endl;
		return luaL_error(L, "(Control) Match over, exiting");
	}

	return 0;
}

int Control::c_simulate(lua_State *L) {
	simulate = true;
	// lementjuk a rendes primitivest
	Primitives* realPrimitives = mPrimitives;
	// letrehozzuk a szimulalo primitivest allapot masolassal
	mPrimitives = new Primitives(realPrimitives);
	mPrimitives->Init();

	double x, y;
	for (unsigned char i = 0; i < OPPONENT_NUM; i++) {
		Control::GetOpponentPos(i, &x, &y);
		mPrimitives->SetOpponentPos(i, x, y);
	}

	// meghivjuk a parameterul kapott fuggvenyt
	int s = lua_pcall(L, lua_gettop(L) - 1, LUA_MULTRET, 0);
	if (s != 0) {
		// nem sikerult a szimulacio
		std::cout << "(Control) Simulation failed: " << lua_tostring(L, -1) << std::endl;
		lua_pop(L, 1); // remove error message
		lua_pushboolean(L, false);
	} else {
		// sikeres szimulacio
		lua_pushboolean(L, true);
	}
	delete mPrimitives;
	simulate = false;
	// visszaallitjuk a rendes primitivest
	mPrimitives = realPrimitives;
	// frissitjuk az allapotat
	lua_getfield(L, LUA_GLOBALSINDEX, "control");
	lua_getfield(L, -1, "wait");
	lua_remove(L, -2);
	lua_call(L, 0, 0);
	return 1;
}

int Control::c_in_simulate(lua_State *L) {
	lua_pushboolean(L, simulate);
	return 1;
}

int Control::c_print(lua_State *L) {
	int argc = lua_gettop(L);
	msgprint message;
	message.function = MSG_PRINT;
	int msgpos = 0;
	message.text[msgpos] = 0;
	for (int n = 1; n <= argc; ++n) {
		if (n > 1) {
			std::cout << " ";
			if (msgpos < 254) {
				message.text[msgpos] = ' ';
				msgpos++;
				message.text[msgpos] = 0;
			}
		}
		if (!strcmp(luaL_typename(L, n), "string")) {
			const char* s = lua_tostring(L, n);
			std::cout << s;
			if (msgpos + strlen(s) < 254) {
				strcpy(message.text+msgpos, s);
			}
		} else if (!strcmp(luaL_typename(L, n), "number")) {
			double x = lua_tonumber(L, n);
			std::cout << x;
			if (msgpos < 240) {
				sprintf(message.text+msgpos, "%.3f", x);
			}
		} else if (!strcmp(luaL_typename(L, n), "boolean")) {
			bool b = lua_toboolean(L, n);
			if (b) {
				std::cout << "true";
				strcpy(message.text+msgpos, "true");
			} else {
				std::cout << "false";
				strcpy(message.text+msgpos, "false");
			}
		} else {
			std::cout << luaL_typename(L, n);
			strcpy(message.text+msgpos, luaL_typename(L, n));
		}
		msgpos += strlen(message.text+msgpos);
	}
	if (argc > 0) {
		std::cout << std::endl;
		message.text[msgpos] = 0;
		mServer->Send(0, &message, msgpos + 1 + sizeof(function_t));
	}
	return 0;
}

//music csak a playlist file-ba irja a kapott string-et, ezt valamilyen proginak kene lejatszani
int Control::c_music(lua_State *L) {
	if (lua_isstring(L, 1)) {
		const char* s = lua_tostring(L, 1);
		int fp = open("/tmp/playlist", O_WRONLY | O_CREAT | O_TRUNC, 0664);
		write(fp, s, strlen(s));
		close(fp);
	}
	return 0;
}

int Control::l_GetStartButton(lua_State *L) {
	bool b = mPrimitives->GetStartButton();
	lua_pushboolean(L, b);
	return 1;
}

int Control::l_GetStopButton(lua_State *L) {
	bool b = mPrimitives->GetStopButton();
	lua_pushboolean(L, b);
	return 1;
}

int Control::l_GetMyColor(lua_State *L) {
	lua_pushinteger(L, mPrimitives->GetMyColor());
	return 1;
}

//4 tavolsagmero szenzor adatot adja lua-nak
int Control::l_GetDistances(lua_State *L) {
	double dist[PROXIMITY_NUM];
	mPrimitives->GetDistances(dist);
	for (int i = 0; i < PROXIMITY_NUM; i++) {
		lua_pushnumber(L, dist[i]);
	}
	return PROXIMITY_NUM;
}

int Control::l_SetMotorSupply(lua_State *L) {
	bool powered = lua_toboolean(L, 1);
	lua_settop(L, 0);
	if (mPrimitives->SetMotorSupply(powered)) {
		while (mPrimitives->SetMotorSupplyInProgress()) {
			c_wait(L);
		}
		lua_pushboolean(L, true);
		return 1;
	}
	lua_pushboolean(L, false);
	return 1;
}

int Control::l_CalibrateDeadreckoning(lua_State *L) {
	bool simulate = optbool(L, 1, false);
	lua_settop(L, 0);
	if (mPrimitives->CalibrateDeadreckoning(simulate)) {
		while (mPrimitives->CalibrateDeadreckoningInProgress()) {
			c_wait(L);
		}
		lua_pushboolean(L, true);
		return 1;
	}
	lua_pushboolean(L, false);
	return 1;
}

int Control::l_SetSpeed(lua_State *L) {
	double v = luaL_optnumber(L, 1, 0);
	double w = luaL_optnumber(L, 2, 0);
	lua_settop(L, 0);
	if (mPrimitives->SetSpeed(v, w)) {
		while (mPrimitives->SetSpeedInProgress()) {
			c_wait(L);
		}
		lua_pushboolean(L, true);
		return 1;
	}
	lua_pushboolean(L, false);
	return 1;
}

int Control::l_Go(lua_State *L) {
	double distance = luaL_optnumber(L, 1, 1000);
	double speed = luaL_optnumber(L, 2, 100);
	double acc = luaL_optnumber(L, 3, 100);

	if (isnan(distance)) {
		cout << "(Control) Go(nan)" << endl;
		lua_pushboolean(L, false);
		return 1;
	}

	if (mPrimitives->Go(distance, speed, acc)) {
		setSafeMotion(L);
		lua_pushboolean(L, true);
		return 1;
	}
	lua_pushboolean(L, false);
	return 1;
}

int Control::l_GoTo(lua_State *L) {
	double x = lua_tonumber(L, 1);
	double y = lua_tonumber(L, 2);
	double speed = luaL_optnumber(L, 3, 100);
	double acc = luaL_optnumber(L, 4, 100);

	if (isnan(x) || isnan(y)) {
		cout << "(Control) GoTo(nan, nan)" << endl;
		lua_pushboolean(L, false);
		return 1;
	}

	if (mPrimitives->GoTo(x, y, speed, acc)) {
		setSafeMotion(L);
		lua_pushboolean(L, true);
		return 1;
	}
	lua_pushboolean(L, false);
	return 1;
}

int Control::l_Turn(lua_State *L) {
	double angle = luaL_optnumber(L, 1, M_PI_2);
	double speed = luaL_optnumber(L, 2, 2);
	double acc = luaL_optnumber(L, 3, 2);

	if (isnan(angle)) {
		cout << "(Control) Turn(nan)" << endl;
		lua_pushboolean(L, false);
		return 1;
	}

	if (mPrimitives->Turn(angle, speed, acc)) {
		setSafeMotion(L);
		lua_pushboolean(L, true);
		return 1;
	}
	lua_pushboolean(L, false);
	return 1;
}

int Control::l_TurnTo(lua_State *L) {
	double x = lua_tonumber(L, 1);
	double y = lua_tonumber(L, 2);
	double speed = luaL_optnumber(L, 3, 2);
	double acc = luaL_optnumber(L, 4, 2);

	if (isnan(x) || isnan(y)) {
		cout << "(Control) TurnTo(nan, nan)" << endl;
		lua_pushboolean(L, false);
		return 1;
	}

	double mx, my, mphi;
	mPrimitives->GetRobotPos(&mx, &my, &mphi);

	double angle = atan2(y - my, x - mx) - mphi;

	while (angle > M_PI) {
		angle -= M_PI * 2;
	}
	while (angle < -M_PI) {
		angle += M_PI * 2;
	}
	if (mPrimitives->Turn(angle, speed, acc)) {
		setSafeMotion(L);
		lua_pushboolean(L, true);
		return 1;
	}
	lua_pushboolean(L, false);
	return 1;
}

int Control::l_TurnToOri(lua_State *L) {
	double angle = lua_tonumber(L, 1);
	double speed = luaL_optnumber(L, 2, 2);
	double acc = luaL_optnumber(L, 3, 2);

	if (isnan(angle)) {
		cout << "(Control) TurnToOri(nan)" << endl;
		lua_pushboolean(L, false);
		return 1;
	}

	double mx, my, mphi;
	mPrimitives->GetRobotPos(&mx, &my, &mphi);

	//kulonbseg a mostani orientaciotol
	angle = angle - mphi;

	while (angle > M_PI) {
		angle -= M_PI * 2;
	}
	while (angle < -M_PI) {
		angle += M_PI * 2;
	}
	if (mPrimitives->Turn(angle, speed, acc)) {
		setSafeMotion(L);
		lua_pushboolean(L, true);
		return 1;
	}
	lua_pushboolean(L, false);
	return 1;
}

int Control::l_MotionInProgress(lua_State *L) {
	if (safeMotion) {
		if (opponentTooClose()) {
			return luaL_error(L, "(Control) Opponent too close");
		}
		if (simulate) {
			if (obstacleCollision()) {
				return luaL_error(L, "(Control) Obstacle collision");
			}
		}
		if (mPrimitives->GetMotionError() > 0) {
			return luaL_error(L, "(Control) Safe motion failed");
		}
	}
	lua_pushboolean(L, mPrimitives->MotionInProgress());
	return 1;
}

int Control::l_GetMotionError(lua_State *L) {
	lua_pushinteger(L, mPrimitives->GetMotionError());
	return 1;
}

int Control::l_MotionStop(lua_State *L) {
	double dec = luaL_optnumber(L, 1, 0);
	lua_pushboolean(L, mPrimitives->MotionStop(dec));
	return 1;
}

int Control::l_MotionStopInProgress(lua_State *L) {
	lua_pushboolean(L, mPrimitives->MotionStopInProgress());
	return 1;
}

int Control::l_GetRobotPos(lua_State *L) {
	double x, y, phi;
	mPrimitives->GetRobotPos(&x, &y, &phi);
	lua_pushnumber(L, x);
	lua_pushnumber(L, y);
	lua_pushnumber(L, phi);
	return 3;
}

int Control::l_SetRobotPos(lua_State *L) {
	double x = lua_tonumber(L, 1);
	double y = lua_tonumber(L, 2);
	double phi = lua_tonumber(L, 3);
	mPrimitives->SetRobotPos(x, y, phi);
	return 0;
}

long int Control::GetOpponentPos(unsigned char n, double* ox, double* oy) {
	if (n < OPPONENT_NUM) {
		long int ret = mPrimitives->GetOpponentPos(n, ox, oy);
		if (simulate || ret > SONAR_TIMEOUT || RunTime() < SONAR_TIMEOUT) {
			/*
			 * ha szimulalunk
			 * vagy ha a szonar timeoutol
			 * vagy ha meg nem telt el SONAR_TIMEOUT ido (ilyenkor a sonar 0,0-at ad, es ervenyes adatnak vennenk)
			 * akkor nem frissitunk
			 */
			*ox = opponent_x[n];
			*oy = opponent_y[n];
		} else {
			SetOpponentPos(n, *ox, *oy);
		}
		return ret;
	}
	return 0;
}

void Control::SetOpponentPos(unsigned char n, double ox, double oy) {
	if (n < OPPONENT_NUM) {
		opponent_x[n] = ox;
		opponent_y[n] = oy;
	}
}

int Control::l_GetOpponentPos(lua_State *L) {
	unsigned char n = luaL_optinteger(L, 1, 0);
	double x, y;
	long int validity = Control::GetOpponentPos(n, &x, &y);
	lua_pushnumber(L, x);
	lua_pushnumber(L, y);
	lua_pushinteger(L, validity);
	return 3;
}

#ifdef NAGY_ROBOT
int Control::l_ArmMove(lua_State *L) {
	bool left = lua_toboolean(L, 1);
	double pos = luaL_optnumber(L, 2, 0);
	double speed = luaL_optnumber(L, 3, 1000);
	double acc = luaL_optnumber(L, 4, 850);
	lua_pushboolean(L, mPrimitives->ArmMove(left, pos, speed, acc));
	return 1;
}

int Control::l_ArmMoveInProgress(lua_State *L) {
	bool left = lua_toboolean(L, 1);
	lua_pushboolean(L, mPrimitives->ArmMoveInProgress(left));
	return 1;
}

int Control::l_GetArmPos(lua_State *L) {
	bool left = lua_toboolean(L, 1);
	lua_pushnumber(L, mPrimitives->GetArmPos(left));
	return 1;
}

int Control::l_SelectorMove(lua_State *L) {
	double pos = luaL_optnumber(L, 1, 0);
	double speed = luaL_optnumber(L, 2, 100);
	double acc = luaL_optnumber(L, 3, 100);
	lua_pushboolean(L, mPrimitives->SelectorMove(pos, speed, acc));
	return 1;
}

int Control::l_SelectorMoveInProgress(lua_State *L) {
	lua_pushboolean(L, mPrimitives->SelectorMoveInProgress());
	return 1;
}

int Control::l_GetSelectorPos(lua_State *L) {
	lua_pushnumber(L, mPrimitives->GetSelectorPos());
	return 1;
}

int Control::l_FlipperMove(lua_State *L) {
	double pos = luaL_optnumber(L, 1, 0);
	double speed = luaL_optnumber(L, 2, 1000);
	double acc = luaL_optnumber(L, 3, 850);
	lua_pushboolean(L, mPrimitives->FlipperMove(pos, speed, acc));
	return 1;
}

int Control::l_FlipperMoveInProgress(lua_State *L) {
	lua_pushboolean(L, mPrimitives->FlipperMoveInProgress());
	return 1;
}

int Control::l_GetFlipperPos(lua_State *L) {
	lua_pushnumber(L, mPrimitives->GetFlipperPos());
	return 1;
}

int Control::l_ContractorMove(lua_State *L) {
	double pos = luaL_optnumber(L, 1, 0);
	double speed = luaL_optnumber(L, 2, 1000);
	double acc = luaL_optnumber(L, 3, 850);
	lua_pushboolean(L, mPrimitives->ContractorMove(pos, speed, acc));
	return 1;
}

int Control::l_ContractorMoveInProgress(lua_State *L) {
	lua_pushboolean(L, mPrimitives->ContractorMoveInProgress());
	return 1;
}

int Control::l_GetContractorPos(lua_State *L) {
	lua_pushnumber(L, mPrimitives->GetContractorPos());
	return 1;
}

int Control::l_FireStopperMove(lua_State *L) {
	double pos = luaL_optnumber(L, 1, 0);
	double speed = luaL_optnumber(L, 2, 700000);	//ez meg nem csordul tul
	double acc = luaL_optnumber(L, 3, 700000);
	lua_pushboolean(L, mPrimitives->FireStopperMove(pos, speed, acc));
	return 1;
}

int Control::l_FireStopperMoveInProgress(lua_State *L) {
	lua_pushboolean(L, mPrimitives->FireStopperMoveInProgress());
	return 1;
}

int Control::l_GetFireStopperPos(lua_State *L) {
	lua_pushnumber(L, mPrimitives->GetFireStopperPos());
	return 1;
}

int Control::l_GetBallColorVoltage(lua_State *L)
{
	lua_pushnumber(L, mPrimitives->GetBallColorVoltage());
	return 1;
}

int Control::l_GetBallPresent(lua_State *L)
{
	lua_pushboolean(L, mPrimitives->GetBallPresent());
	return 1;
}
int Control::l_GetBallColor(lua_State *L)
{
	//TODO: update valos ertekekre
	double voltage = mPrimitives->GetBallColorVoltage();
	if( voltage < 0.5)
		lua_pushnumber(L, BALL_NOT_PRESENT);
	else if( voltage >= 0.5 && voltage < 1.2)
		lua_pushnumber(L, BALL_RED);
	else
		lua_pushnumber(L, BALL_WHITE);

	return 1;
}

int Control::l_CaracoleSetSpeed(lua_State *L)
{
	double speed = luaL_optnumber(L, 1, 0);
	double max_acc = luaL_optnumber(L, 2, 0);
	lua_pushboolean(L, mPrimitives->CaracoleSetSpeed(speed, max_acc));
	return 1;
}

int Control::l_CaracoleSetSpeedInProgress(lua_State *L)
{
	lua_pushboolean(L, mPrimitives->CaracoleSetSpeedInProgress());
	return 1;
}
int Control::l_GetCaracoleSpeed(lua_State *L)
{
	lua_pushnumber(L, mPrimitives->GetCaracoleSpeed());
	return 1;
}

int Control::l_FirewheelSetSpeed(lua_State *L)
{
	double speed = luaL_optnumber(L, 1, 0);
	double max_acc = luaL_optnumber(L, 2, 0);
	lua_pushboolean(L, mPrimitives->FirewheelSetSpeed(speed, max_acc));
	return 1;
}

int Control::l_FirewheelSetSpeedInProgress(lua_State *L)
{
	lua_pushboolean(L, mPrimitives->FirewheelSetSpeedInProgress());
	return 1;
}
int Control::l_GetFirewheelSpeed(lua_State *L)
{
	lua_pushnumber(L, mPrimitives->GetFirewheelSpeed());
	return 1;
}

#else	//KIS_ROBOT
int Control::l_GripperMove(lua_State *L) {
	bool low = lua_toboolean(L, 1);
	bool left = lua_toboolean(L, 2);
	double pos = luaL_optnumber(L, 3, 0);
	double speed = luaL_optnumber(L, 4, 1000);
	double acc = luaL_optnumber(L, 5, 850);
	lua_pushboolean(L, mPrimitives->GripperMove(low, left, pos, speed, acc));
	return 1;
}

int Control::l_GripperMoveInProgress(lua_State *L) {
	bool low = lua_toboolean(L, 1);
	bool left = lua_toboolean(L, 2);
	lua_pushboolean(L, mPrimitives->GripperMoveInProgress(low, left));
	return 1;
}

int Control::l_GetGripperPos(lua_State *L) {
	bool low = lua_toboolean(L, 1);
	bool left = lua_toboolean(L, 2);
	lua_pushnumber(L, mPrimitives->GetGripperPos(low, left));
	return 1;
}
#endif

int Control::l_CalibrateConsole(lua_State *L) {
	lua_pushboolean(L, mPrimitives->CalibrateConsole());
	return 1;
}

int Control::l_CalibrateConsoleInProgress(lua_State *L) {
	lua_pushboolean(L, mPrimitives->CalibrateConsoleInProgress());
	return 1;
}

int Control::l_ConsoleMove(lua_State *L) {
	double pos = luaL_optnumber(L, 1, 0);
	double speed = luaL_optnumber(L, 2, 400);
	double acc = luaL_optnumber(L, 3, 10);
	lua_pushboolean(L, mPrimitives->ConsoleMove(pos, speed, acc));
	return 1;
}

int Control::l_ConsoleMoveInProgress(lua_State *L) {
	lua_pushboolean(L, mPrimitives->ConsoleMoveInProgress());
	return 1;
}

int Control::l_ConsoleStop(lua_State *L) {
	lua_pushboolean(L, mPrimitives->ConsoleStop());
	return 1;
}

int Control::l_ConsoleStopInProgress(lua_State *L) {
	lua_pushboolean(L, mPrimitives->ConsoleStopInProgress());
	return 1;
}

int Control::l_GetConsolePos(lua_State *L) {
	double pos = mPrimitives->GetConsolePos();
	lua_pushnumber(L, pos);
	return 1;
}

int Control::l_StartMatch(lua_State *L) {
	bool startTimer = optbool(L, 1, true);
	if (startTimer) {
		gettimeofday(&matchStart, NULL);
		matchStarted = true;
	}

	return 0;
}

int Control::l_AddTestObstacles(lua_State *L) {
	dynObstacles.push_back(new Circle(1000, 1500, 100));
	logDynObstacles = true;
	for (int i = 0; i < MAX_CONNECTIONS; i++) {
		sendDynObstacles[i] = true;
	}
	return 0;
}

int Control::l_ClearDynObstacles(lua_State *L) {
	while (!dynObstacles.empty()) {
		delete dynObstacles.front();
		dynObstacles.pop_front();
	}
	logDynObstacles = true;
	for (int i = 0; i < MAX_CONNECTIONS; i++) {
		sendDynObstacles[i] = true;
	}
	return 0;
}

#ifdef KIS_ROBOT
int Control::l_FollowLine_Follow(lua_State *L)
{
	double dist = lua_tonumber(L, 1);

	if (isnan(dist)) {
		cout << "(Control) FollowLine_Follow(nan)" << endl;
		lua_pushboolean(L, false);
		return 1;
	}

	if (mPrimitives->FollowLine_Follow(dist)) {
		setSafeMotion(L);
		lua_pushboolean(L, true);
		return 1;
	}
	lua_pushboolean(L, false);
	return 1;
}
int Control::l_FollowLine_FollowInProgress(lua_State *L)
{
	lua_pushboolean(L, mPrimitives->FollowLine_FollowInProgress());
	return 1;
}
int Control::l_FollowLine_GetFollowError(lua_State *L)
{
	lua_pushinteger(L, mPrimitives->FollowLine_GetFollowError());
	return 1;
}
int Control::l_FollowLine_Turn(lua_State *L)
{
	if (mPrimitives->FollowLine_Turn())
	{
		setSafeMotion(L);
		lua_pushboolean(L, true);
		return 1;
	}
	lua_pushboolean(L, false);
	return 1;
}
int Control::l_FollowLine_TurnInProgress(lua_State *L)
{
	lua_pushboolean(L, mPrimitives->FollowLine_TurnInProgress());
	return 1;
}
int Control::l_FollowLine_Calibrate(lua_State *L)
{
	lua_pushboolean(L, mPrimitives->FollowLine_Calibrate());
	return 1;
}
int Control::l_FollowLine_CalibrateInProgress(lua_State *L)
{
	lua_pushboolean(L, mPrimitives->FollowLine_CalibrateInProgress());
	return 1;
}
#endif

