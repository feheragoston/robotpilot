/*
 * Control.cpp
 *
 *  Created on: Mar 24, 2011
 *      Author: agoston
 */

#include "Control.h"

using namespace std;

Config* Control::mConfig = NULL;
Primitives* Control::mPrimitives = NULL;
PrimitivesNet* Control::mCamera = NULL;
Server* Control::mServer = NULL;
int Control::logfile = NULL;
bool Control::matchStarted = false;
bool Control::exitControl = false;
msgpawns* Control::pawns = NULL;
std::list<Obstacle*> Control::obstacles = std::list<Obstacle*>();
std::list<Obstacle*> Control::dynObstacles = std::list<Obstacle*>();
bool Control::sendDynObstacles = false;
Circle* Control::opponent = NULL;
double Control::angry = 0.;
bool Control::simulate = false;
bool Control::safeMotion = false;
int Control::deployFields[36] = {
		-15, -12, -14, -14, -12, -15,
		-11, -13, -10, -10, -13, -11,
		 -8,  -2,  -1,  -1,  -2,  -8,
		 -4,  -7,  -3,  -3,  -7,  -4,
		 -9,  -5,  -6,  -6,  -5,  -9,
		-18, -16, -17, -17, -16, -18
};
struct timeval Control::runStart = {0, 0};
struct timeval Control::initStart = {0, 0};
struct timeval Control::matchStart = {0, 0};
struct timeval Control::sleepCalled = {0, 0};
long int Control::timeToSleep = 0;

#define ROBOT_POINT_NUM 6
double Control::robotBody[][2] = {
		{ 140,  140 + 15},
		{-105,  140 + 15},
		{-160,   65},
		{-160,  -65},
		{-105, -140 - 15},
		{ 140, -140 - 15}
};

Control::Control(Config* config) {
	mConfig = config;

	if (strlen(mConfig->LogFile)) {
		logfile = open(mConfig->LogFile, O_WRONLY | O_CREAT | O_TRUNC, 0664);

		if (logfile < 0) {
			cerr << "Error opening logfile: " << mConfig->LogFile << " " << logfile << endl;
			logfile = NULL;
		}
	}

	this->L = luaL_newstate();
	luaL_openlibs(L);

	static const luaL_Reg controllib[] = {
		{"gettimeofday", c_gettimeofday},
		{"getelapsedtime", c_getelapsedtime},
		{"exit", c_exit},
		{"wait", c_wait},
		{"process", c_process},
		{"runparallel", c_runparallel},
		{"simulate", c_simulate},
		{"print", c_print},

		{"GetStartButton", GetStartButton},
		{"GetStopButton", GetStopButton},
		{"GetMyColor", GetMyColor},
		{"PawnInGripper", PawnInGripper},
		{"GetDistances", GetDistances},
		{"SetMotorSupply", SetMotorSupply},

		{"CalibrateDeadreckoning", CalibrateDeadreckoning},
		{"RefineDeadreckoning", RefineDeadreckoning},
		{"SetSpeed", SetSpeed},
		{"Go", Go},
		{"GoSafe", Go},
		{"GoTo", GoTo},
		{"GoToSafe", GoTo},
		{"Turn", Turn},
		{"TurnSafe", Turn},
		{"TurnTo", TurnTo},
		{"TurnToSafe", TurnTo},
		{"MotionInProgress", MotionInProgress},
		{"GetMotionError", GetMotionError},
		{"MotionStop", MotionStop},
		{"MotionStopInProgress", MotionStopInProgress},
		{"GetRobotPos", GetRobotPos},
		{"SetRobotPos", SetRobotPos},
		{"GetOpponentPos", GetOpponentPos},

		{"GripperMove", GripperMove},
		{"GripperMoveInProgress", GripperMoveInProgress},
		{"CalibrateConsole", CalibrateConsole},
		{"ConsoleMove", ConsoleMove},
		{"ConsoleMoveInProgress", ConsoleMoveInProgress},
		{"ConsoleStop", ConsoleStop},
		{"ConsoleStopInProgress", ConsoleStopInProgress},
		{"GetConsolePos", GetConsolePos},
		{"ArmMove", ArmMove},
		{"ArmMoveInProgress", ArmMoveInProgress},
		{"Magnet", Magnet},
		{"MagnetInProgress", MagnetInProgress},

		{"StartMatch", StartMatch},
		{"RefreshPawnPositions", RefreshPawnPositions},
		{"RefreshPawnPositionsInProgress", RefreshPawnPositionsInProgress},
		{"RefreshPawnPositionsFinished", RefreshPawnPositionsFinished},
		{"FindPawn", FindPawn},
		{"GetDeployPoint", GetDeployPoint},
		{"SetDeployPointPriority", SetDeployPointPriority},

		{NULL, NULL}
	};

	luaL_register(L, "control", controllib);

	lua_pushnumber(L, ROBOT_RADIUS);
	lua_setfield(L, LUA_GLOBALSINDEX, "ROBOT_RADIUS");
	lua_pushnumber(L, ROBOT_WIDTH);
	lua_setfield(L, LUA_GLOBALSINDEX, "ROBOT_WIDTH");
	lua_pushnumber(L, ROBOT_FRONT);
	lua_setfield(L, LUA_GLOBALSINDEX, "ROBOT_FRONT");
	lua_pushnumber(L, ROBOT_FRONT_MAX);
	lua_setfield(L, LUA_GLOBALSINDEX, "ROBOT_FRONT_MAX");
	lua_pushnumber(L, ROBOT_FRONT_PAWN);
	lua_setfield(L, LUA_GLOBALSINDEX, "ROBOT_FRONT_PAWN");
	lua_pushnumber(L, ROBOT_BACK);
	lua_setfield(L, LUA_GLOBALSINDEX, "ROBOT_BACK");
	lua_pushnumber(L, PAWN_RADIUS);
	lua_setfield(L, LUA_GLOBALSINDEX, "PAWN_RADIUS");

	luaL_dostring(L, "package.path = package.path .. \";./Pilot/?.lua\"; c = control; p = require(\"pilot\");");

	matchStarted = false;
	exitControl = false;

	pawns = new msgpawns();
	pawns->function = MSG_PAWNS;
	pawns->num = 10;
	for (int i = 0; i < 10; i++) {
		if (i < 5) {
			pawns->pawns[i].x = 690 + i * 280;
			pawns->pawns[i].y = 300;
		} else {
			pawns->pawns[i].x = 690 + (i - 5) * 280;
			pawns->pawns[i].y = 2700;
		}
		pawns->pawns[i].type = 1;
	}

	if (obstacles.empty()) {
		// Akadalyok definialasa
		// Falak
		obstacles.push_back(new Line(0, 0, 2100, 0));
		obstacles.push_back(new Line(0, 0, 0, 3000));
		obstacles.push_back(new Line(0, 3000, 2100, 3000));
		obstacles.push_back(new Line(2100, 0, 2100, 3000));

		// start vedo falak
		obstacles.push_back(new Line(400, 0, 400, 400));
		obstacles.push_back(new Line(400, 3000, 400, 2600));

		// biztos terulet falai
		obstacles.push_back(new Line(1850, 450, 2100, 450));
		obstacles.push_back(new Line(1850, 1150, 2100, 1150));
		obstacles.push_back(new Line(1850, 1850, 2100, 1850));
		obstacles.push_back(new Line(1850, 2550, 2100, 2550));
		obstacles.push_back(new Line(1980, 450, 1980, 1150));
		obstacles.push_back(new Line(1980, 1850, 1980, 2550));
	}
	// ellenfelet alapbol kirakjuk a palyarol
	opponent = new Circle(-1000, -1000, 1);
}

Control::~Control() {
	if (logfile) {
		close(logfile);
		logfile = NULL;
	}
	if (mPrimitives) {
		delete mPrimitives;
	}
	if (mCamera) {
		delete mCamera;
	}
	if (mServer) {
		delete mServer;
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

	delete opponent;
}

bool Control::Init() {
	gettimeofday(&runStart, NULL);
	gettimeofday(&initStart, NULL);
	gettimeofday(&matchStart, NULL);

	if (mConfig->PrimitivesCan) {
		mPrimitives = new PrimitivesCan(mConfig);
	} else if (mConfig->PrimitivesNet) {
		mPrimitives = new PrimitivesNet(mConfig);
	} else {
		mPrimitives = new Primitives(mConfig);
	}

	if (mPrimitives->Init()) {
		mCamera = new PrimitivesNet(mConfig);
		if (mCamera->CameraInit()) {
			cout << "(Control) Connected to camera" << endl;
		} else {
			cout << "(Control) Error connecting to camera" << endl;
			delete mCamera;
			mCamera = NULL;
		}

		mServer = new Server();
		mServer->setMessageCallback(serverMessageCallback);
		mServer->Listen(13001);
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
			lua_pushinteger(L, 50000);
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

void Control::serverMessageCallback(int n, const void* message, msglen_t size) {
	function_t* function = (function_t*) message;
	if (*function == MSG_REFRESHSTATUS && size == sizeof(msgb1)) {
		msgstatus response;
		response.function = MSG_REFRESHSTATUS;
		mPrimitives->GetRobotPos(&(response.x), &(response.y), &(response.phi));
		mPrimitives->GetSpeed(&(response.v), &(response.w));
		mPrimitives->GetOpponentPos(&(response.ox), &(response.oy));
		response.oradius = opponent->getRadius();
		response.startButton = mPrimitives->GetStartButton();
		response.stopButton = mPrimitives->GetStopButton();
		response.color = mPrimitives->GetMyColor();
		response.pawnInGripper = mPrimitives->PawnInGripper();
		response.motorSupply = mPrimitives->GetMotorSupply();
		response.gripperPos = mPrimitives->GetGripperPos();
		response.consolePos = mPrimitives->GetConsolePos();
		response.leftArmPos = mPrimitives->GetArmPos(true);
		response.rightArmPos = mPrimitives->GetArmPos(false);
		mServer->Send(n, &response, sizeof(msgstatus));

		msgdeploypriority message;
		message.function = MSG_DEPLOYPRIORITY;
		for (int i = 0; i < 36; i++) {
			message.priority[i] = deployFields[i];
		}
		mServer->Send(n, &message, sizeof(msgdeploypriority));

		if (sendDynObstacles) {
			msgobstacles obstacles;
			obstacles.function = MSG_OBSTACLES;
			obstacles.num = 14;
			if (dynObstacles.size() < 14) {
				obstacles.num = dynObstacles.size();
			}
			std::list<Obstacle*>::iterator o = dynObstacles.begin();
			for (int i = 0; i < obstacles.num; i++) {
				(*o)->getObstacle(&obstacles.obstacles[i]);
				o++;
			}
			mServer->Send(n, &obstacles, sizeof(msgobstacles));
			sendDynObstacles = false;
		}

	} else if (*function == MSG_PAWNS && size == sizeof(msgb1)) {
		mServer->Send(n, pawns, sizeof(msgpawns));
	} else if (*function == MSG_VISIONTEST && size == 1) {
		msgd3 response;
		response.function = MSG_VISIONTEST;
		mPrimitives->GetRobotPos(&(response.d1), &(response.d2), &(response.d3));
		mServer->Send(n, &response, sizeof(msgd3));
	} else if (size != 0) {
		std::cout << "(Control) Unknown or invalid function: " << (int)*function << " size: " << (int)size << std::endl;
	}
}

void Control::log() {
	if (logfile) {
		unsigned int time = InitTime();
		write(logfile, &time, sizeof(unsigned int));
		time = MatchTime();
		write(logfile, &time, sizeof(unsigned int));

		msgstatus status;
		status.function = MSG_REFRESHSTATUS;
		mPrimitives->GetRobotPos(&(status.x), &(status.y), &(status.phi));
		mPrimitives->GetSpeed(&(status.v), &(status.w));
		mPrimitives->GetOpponentPos(&(status.ox), &(status.oy));
		status.oradius = opponent->getRadius();
		status.startButton = mPrimitives->GetStartButton();
		status.stopButton = mPrimitives->GetStopButton();
		status.color = mPrimitives->GetMyColor();
		status.pawnInGripper = mPrimitives->PawnInGripper();
		status.motorSupply = mPrimitives->GetMotorSupply();
		status.gripperPos = mPrimitives->GetGripperPos();
		status.consolePos = mPrimitives->GetConsolePos();
		status.leftArmPos = mPrimitives->GetArmPos(true);
		status.rightArmPos = mPrimitives->GetArmPos(false);
		write(logfile, &status, sizeof(msgstatus));

		msgdeploypriority priority;
		priority.function = MSG_DEPLOYPRIORITY;
		for (int i = 0; i < 36; i++) {
			priority.priority[i] = deployFields[i];
		}
		write(logfile, &priority, sizeof(msgdeploypriority));

		msgobstacles obstacles;
		obstacles.function = MSG_OBSTACLES;
		obstacles.num = 14;
		if (dynObstacles.size() < 14) {
			obstacles.num = dynObstacles.size();
		}
		std::list<Obstacle*>::iterator o = dynObstacles.begin();
		for (int i = 0; i < obstacles.num; i++) {
			(*o)->getObstacle(&obstacles.obstacles[i]);
			o++;
		}
		write(logfile, &obstacles, sizeof(msgobstacles));

		write(logfile, &pawns, sizeof(msgpawns));

		function_t stop = 0xFF;
		if (write(logfile, &stop, sizeof(function_t)) < 0) {
			cerr << "Error writing log, closing log" << endl;
			close(logfile);
			logfile = NULL;
		}
	}
}

void Control::setSafeMotion(lua_State *L) {
	lua_Debug ar;
	lua_getstack(L, 0, &ar);
	lua_getinfo(L, "nS", &ar);
	safeMotion = false;
	if (ar.name == NULL || strstr(ar.name, "Safe") != NULL) {
		safeMotion = true;
	}
}

long int Control::refreshOpponent() {
	double ox, oy;
	long int validity = mPrimitives->GetOpponentPos(&ox, &oy);
	opponent->Set(ox, oy, ROBOT_RADIUS * 2.5 - angry);
	return validity;
}

bool Control::opponentTooClose() {
	double x, y, phi, v, w;
	mPrimitives->GetRobotPos(&x, &y, &phi);
	mPrimitives->GetSpeed(&v, &w);

	if (refreshOpponent() > SONAR_TIMEOUT) {
		cout << "(Control) Sonar timeout!" << endl;
		return true;
	}

	if (fabs(v) < 0.01) {
		return false;
	}

	double distance = v * v / (2 * MAX_DEC);
	if (v < 0.) {
		distance += ROBOT_BACK;
		distance *= -1;
	} else {
		distance += ROBOT_FRONT;
	}

	/*
	msgd4 message;
	message.function = MSG_GO;
	message.d1 = x;
	message.d2 = y;
	message.d3 = x + distance * cos(phi);
	message.d4 = y + distance * sin(phi);
	mServer->Send(0, &message, sizeof(msgd4));
	*/
	if (opponent->Intersect(x, y, x + distance * cos(phi), y + distance * sin(phi))) {
		if (angry < ROBOT_RADIUS) {
			angry += 5.;
		}
		//std::cout << "(Control) Opponent too close! angry: " << angry << std::endl;
		return true;
	}
	if (!simulate && angry > 0) {
		angry -= 0.5;
	}
	return false;
}

bool Control::obstacleCollision() {
	double x, y, phi;
	mPrimitives->GetRobotPos(&x, &y, &phi);
	double gripperPos = mPrimitives->GetGripperPos();

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

		/*
		if (j == 0) {
			msgd4 message;
			message.function = MSG_GO;
			message.d1 = x1;
			message.d2 = y1;
			message.d3 = x2;
			message.d4 = y2;
			mServer->Send(0, &message, sizeof(msgd4));
		}
		*/

		for (std::list<Obstacle*>::iterator i = obstacles.begin(); i != obstacles.end(); i++) {
			if ((*i)->Intersect(x1, y1, x2, y2)) {
				(*i)->Print();
				return true;
			}
		}
		for (std::list<Obstacle*>::iterator i = dynObstacles.begin(); i != dynObstacles.end(); i++) {
			if ((*i)->Intersect(x1, y1, x2, y2)) {
				(*i)->Print();
				return true;
			}
		}
	}
	return false;
}

void Control::removeCollidingDynamicObstacles(Obstacle* obstacle) {
	std::list<Obstacle*>::iterator i = dynObstacles.begin();
	while (i != dynObstacles.end()) {
		if ((*i)->Intersect(obstacle)) {
			delete(*i);
			i = dynObstacles.erase(i);
			sendDynObstacles = true;
			//cout << "(Control) Dynamic obstacle removed" << endl;
		} else {
			i++;
		}
	}
}

void Control::addDynamicObstacle(Obstacle* obstacle) {
	removeCollidingDynamicObstacles(obstacle);

	double x, y, phi;
	mPrimitives->GetRobotPos(&x, &y, &phi);

	for (int j = 0; j < ROBOT_POINT_NUM; j++) {
		double x1 = cos(phi) * robotBody[j][0] - sin(phi) * robotBody[j][1] + x;
		double y1 = sin(phi) * robotBody[j][0] + cos(phi) * robotBody[j][1] + y;
		double x2 = cos(phi) * robotBody[(j+1)%ROBOT_POINT_NUM][0] - sin(phi) * robotBody[(j+1)%ROBOT_POINT_NUM][1] + x;
		double y2 = sin(phi) * robotBody[(j+1)%ROBOT_POINT_NUM][0] + cos(phi) * robotBody[(j+1)%ROBOT_POINT_NUM][1] + y;
		if (obstacle->Intersect(x1, y1, x2, y2)) {
			delete obstacle;
			return;
		}
	}
	dynObstacles.push_back(obstacle);
	sendDynObstacles = true;
	//cout << "(Control) Number of dynamic obstacles: " << dynObstacles.size() << endl;
}

bool Control::pawnOnOurColor(double x, double y) {
	if (y > 550 && y < 2450) {
		bool color = mPrimitives->GetMyColor();
		int mx = (int)x;
		int my = (int)y - 450;
		mx = mx % 700;
		my = my % 700;

		if (color) {
			mx = 700 - mx;
		}
		if (mx > 100 && mx < 250 && my > 450 && my < 600) {
			return true;
		}
		if (mx > 450 && mx < 600 && my > 100 && my < 250) {
			return true;
		}
	}
	return false;
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

bool Control::checkLine(double x1, double y1, double x2, double y2) {
	if (opponent->Intersect(x1, y1, x2, y2)) {
		return false;
	}
	for (std::list<Obstacle*>::iterator i = obstacles.begin(); i != obstacles.end(); i++) {
		if ((*i)->Intersect(x1, y1, x2, y2)) {
			return false;
		}
	}
	for (std::list<Obstacle*>::iterator i = dynObstacles.begin(); i != dynObstacles.end(); i++) {
		if ((*i)->Intersect(x1, y1, x2, y2)) {
			return false;
		}
	}
	return true;
}

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

int Control::c_exit(lua_State *L) {
	exitControl = true;
	luaL_error(L, "(Control) exit() called, exiting");
	return 0;
}

int Control::c_wait(lua_State *L) {
	long int useconds = luaL_optinteger(L, 1, 50000);

	/* Primitives statuszfrissitest varunk */
	if (!mPrimitives->Wait(useconds)) {
		exitControl = true;
		return luaL_error(L, "(Control) Primitives->Wait failed, exiting");
	}
	if (mCamera) {
		if (!mCamera->Wait(0)) {
			cout << "(Control) Camera disconnected" << endl;
			delete mCamera;
			mCamera = NULL;
		}
	}

	/* logolunk es a csatlakozott klienseket feldolgozzuk */
	log();
	mServer->Process();

	/* statusz ellenorzesek */
	if (mPrimitives->GetStopButton()) {
		if (mPrimitives->GetMotorSupply()) {
			mPrimitives->SetMotorSupply(false);
		}
		if (!mPrimitives->SetMotorSupplyInProgress()) {
			exitControl = true;
		}
		return luaL_error(L, "(Control) Stop button, exiting");
	} else if (MatchTime() > 90000) {
		if (mPrimitives->GetMotorSupply()) {
			mPrimitives->SetMotorSupply(false);
		}
		if (!mPrimitives->SetMotorSupplyInProgress()) {
			exitControl = true;
		}
		cout << "(Control) Meccs ido letelt, kilepunk" << endl;
		return luaL_error(L, "(Control) Match over, exiting");
	}

	double ox, oy;
	if (mPrimitives->GetOpponentPos(&ox, &oy) < SONAR_TIMEOUT) {
		Circle* opp = new Circle(ox, oy, ROBOT_WIDTH);
		removeCollidingDynamicObstacles(opp);
		delete opp;
	}

	return 0;
}

int Control::c_process(lua_State *L) {
	int i = lua_pushthread(L);
	lua_pop(L, 1);
	if (i == 1) {
		lua_getfield(L, LUA_GLOBALSINDEX, "control");
		lua_getfield(L, -1, "wait");
		lua_remove(L, -2);
		lua_call(L, 0, 0);
		return 0;
	} else {
		return lua_yield(L, 0);
	}
}

int Control::c_runparallel(lua_State *L) {
	std::list<lua_State*> threads;
	int argc = lua_gettop(L);
	// parameterek sorrendjet megforditjuk
	for (int n = 1; n < argc; ++n) {
		lua_insert(L, n);
	}
	// vegigmegyunk a parametereken hatulrol (eredetiben elorol)
	for (int n = 1; n <= argc; ++n) {
		if (lua_isfunction(L, -1)) {
			lua_State* N = lua_newthread(L); // ha funkcio, letrehozunk neki egy uj szalat
			lua_pop(L, 1); // kiszedjuk a stackbol a letrehozott szalat
			lua_xmove(L, N, 1); // atmozgatjuk a parameterul kapott funkciot a szalhoz
			threads.push_back(N); // hozzaadjuk a threadpoolhoz
		} else {
			if (threads.front() != NULL) {
				lua_xmove(L, threads.back(), 1); // ha mar van szalunk, hozzaadjuk a parametert
			} else {
				lua_pop(L, 1); // ha nincs szalunk kiszedjuk a stackbol
			}
		}
	}
	while (threads.size() > 0) {
		for (int i = threads.size(); i > 0; i--) {
			lua_State* N = threads.front();
			/* hibakereses
			 for (std::list<lua_State*>::iterator j = threads.begin(); j != threads.end(); j++) {
			 if (lua_status(*j) != LUA_YIELD && lua_status(*j) != 0) {
			 std::cout << i << " elrontott lua thread: " << lua_status(*j) << std::endl;
			 }
			 }
			 if (lua_status(N) != LUA_YIELD && lua_status(N) != 0) {
			 std::cout << "elotte: " << lua_status(N) << std::endl;
			 }
			 */
			int exit = lua_resume(N, lua_gettop(N) - 1);
			if (exit == LUA_YIELD) {
				threads.push_back(N);
			} else if (exit != 0) {
				if (strcmp(luaL_optstring(N, -1, "-"), "cannot resume non-suspended coroutine") == 0) {
					std::cout << "(Control) Parallel thread error: " << luaL_optstring(N, -1, "-") << std::endl;
					/* hibakereses
					 std::cout << "utana: " << lua_status(N) << std::endl;
					 for (std::list<lua_State*>::iterator j = threads.begin(); j != threads.end(); j++) {
					 if (lua_status(*j) != LUA_YIELD && lua_status(*j) != 0) {
					 std::cout << i << " futas utan elrontott lua thread: " << lua_status(*j) << std::endl;
					 }
					 }
					 */
				} else {
					luaL_error(L, luaL_optstring(N, -1, "-"));
				}
			} else {
				//lua_close(N);
			}
			threads.pop_front();
		}
		if (threads.size() > 0) {
			lua_getfield(L, LUA_GLOBALSINDEX, "control");
			lua_getfield(L, -1, "process");
			lua_remove(L, -2);
			lua_call(L, 0, 0);
		}
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

	double x, y, phi;
	realPrimitives->GetRobotPos(&x, &y, &phi);
	mPrimitives->SetRobotPos(x, y, phi);
	realPrimitives->GetOpponentPos(&x, &y);
	mPrimitives->SetOpponentPos(x, y);

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
	lua_getfield(L, -1, "process");
	lua_remove(L, -2);
	lua_call(L, 0, 0);
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
	std::cout << std::endl;
	message.text[msgpos] = 0;
	mServer->Send(0, &message, msgpos + 1 + sizeof(function_t));
	return 0;
}

int Control::GetStartButton(lua_State *L) {
	bool b = mPrimitives->GetStartButton();
	lua_pushboolean(L, b);
	return 1;
}

int Control::GetStopButton(lua_State *L) {
	bool b = mPrimitives->GetStopButton();
	lua_pushboolean(L, b);
	return 1;
}

int Control::GetMyColor(lua_State *L) {
	bool b = mPrimitives->GetMyColor();
	lua_pushboolean(L, b);
	return 1;
}

int Control::PawnInGripper(lua_State *L) {
	bool b = mPrimitives->PawnInGripper();
	lua_pushboolean(L, b);
	return 1;
}

int Control::GetDistances(lua_State *L) {
	double dist[6];
	mPrimitives->GetDistances(dist);
	for (int i = 0; i < 6; i++) {
		lua_pushnumber(L, dist[i]);
	}
	return 6;
}

int Control::SetMotorSupply(lua_State *L) {
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

int Control::CalibrateDeadreckoning(lua_State *L) {
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

int Control::RefineDeadreckoning(lua_State *L) {
	lua_settop(L, 0);
	if (!mCamera) {
		mCamera = new PrimitivesNet(mConfig);
		if (mCamera->CameraInit()) {
			cout << "(Control) Connected to camera" << endl;
		} else {
			cout << "(Control) Error connecting to camera" << endl;
			delete mCamera;
			mCamera = NULL;
			lua_pushboolean(L, false);
			return 1;
		}
	}
	double x, y, phi, dx, dy, dphi;
	mPrimitives->GetRobotPos(&x, &y, &phi);
	if (mCamera->RefineDeadreckoning(x, y, phi)) {
		while (mCamera->RefineDeadreckoningInProgress()) {
			c_wait(L);
		}
		mCamera->GetRefineData(&dx, &dy, &dphi);
		mPrimitives->GetRobotPos(&x, &y, &phi);
		mPrimitives->SetRobotPos(x + dx, y + dy, phi + dphi);
		lua_pushboolean(L, true);
		return 1;
	}
	lua_pushboolean(L, false);
	return 1;
}

int Control::SetSpeed(lua_State *L) {
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

int Control::Go(lua_State *L) {
	double distance = luaL_optnumber(L, 1, 1000);
	double speed = luaL_optnumber(L, 2, 500);
	double acc = luaL_optnumber(L, 3, 200);
	if (mPrimitives->Go(distance, speed, acc)) {
		setSafeMotion(L);
		lua_pushboolean(L, true);
		return 1;
	}
	lua_pushboolean(L, false);
	return 1;
}

int Control::GoTo(lua_State *L) {
	double x = lua_tonumber(L, 1);
	double y = lua_tonumber(L, 2);
	double speed = luaL_optnumber(L, 3, 500);
	double acc = luaL_optnumber(L, 4, 200);
	if (mPrimitives->GoTo(x, y, speed, acc)) {
		setSafeMotion(L);
		lua_pushboolean(L, true);
		return 1;
	}
	lua_pushboolean(L, false);
	return 1;
}

int Control::Turn(lua_State *L) {
	double angle = luaL_optnumber(L, 1, M_PI_2);
	double speed = luaL_optnumber(L, 2, 2);
	double acc = luaL_optnumber(L, 3, 2);
	if (mPrimitives->Turn(angle, speed, acc)) {
		setSafeMotion(L);
		lua_pushboolean(L, true);
		return 1;
	}
	lua_pushboolean(L, false);
	return 1;
}

int Control::TurnTo(lua_State *L) {
	double x = lua_tonumber(L, 1);
	double y = lua_tonumber(L, 2);
	double speed = luaL_optnumber(L, 3, 2);
	double acc = luaL_optnumber(L, 4, 2);

	double mx, my, mphi;
	mPrimitives->GetRobotPos(&mx, &my, &mphi);

	double angle = atan2f(y - my, x - mx) - mphi;

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

int Control::MotionInProgress(lua_State *L) {
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

int Control::GetMotionError(lua_State *L) {
	lua_pushinteger(L, mPrimitives->GetMotionError());
	return 1;
}

int Control::MotionStop(lua_State *L) {
	double dec = luaL_optnumber(L, 1, 0);
	lua_pushboolean(L, mPrimitives->MotionStop(dec));
	return 1;
}

int Control::MotionStopInProgress(lua_State *L) {
	lua_pushboolean(L, mPrimitives->MotionInProgress());
	return 1;
}

int Control::GetRobotPos(lua_State *L) {
	double x, y, phi;
	mPrimitives->GetRobotPos(&x, &y, &phi);
	lua_pushnumber(L, x);
	lua_pushnumber(L, y);
	lua_pushnumber(L, phi);
	return 3;
}

int Control::SetRobotPos(lua_State *L) {
	double x = lua_tonumber(L, 1);
	double y = lua_tonumber(L, 2);
	double phi = lua_tonumber(L, 3);
	mPrimitives->SetRobotPos(x, y, phi);
	return 0;
}

int Control::GetOpponentPos(lua_State *L) {
	double x, y;
	mPrimitives->GetOpponentPos(&x, &y);
	lua_pushnumber(L, x);
	lua_pushnumber(L, y);
	return 2;
}

int Control::GripperMove(lua_State *L) {
	double pos = luaL_optnumber(L, 1, 0);
	lua_pushboolean(L, mPrimitives->GripperMove(pos));
	return 1;
}

int Control::GripperMoveInProgress(lua_State *L) {
	lua_pushboolean(L, mPrimitives->GripperMoveInProgress());
	return 1;
}

int Control::CalibrateConsole(lua_State *L) {
	if (mPrimitives->CalibrateConsole()) {
		while (mPrimitives->CalibrateConsoleInProgress()) {
			c_wait(L);
		}
		lua_pushboolean(L, true);
		return 1;
	}
	lua_pushboolean(L, false);
	return 1;
}

int Control::ConsoleMove(lua_State *L) {
	double pos = luaL_optnumber(L, 1, 0);
	double speed = luaL_optnumber(L, 2, 35);
	double acc = luaL_optnumber(L, 3, 10);
	lua_pushboolean(L, mPrimitives->ConsoleMove(pos, speed, acc));
	return 1;
}

int Control::ConsoleMoveInProgress(lua_State *L) {
	lua_pushboolean(L, mPrimitives->ConsoleMoveInProgress());
	return 1;
}

int Control::ConsoleStop(lua_State *L) {
	lua_pushboolean(L, mPrimitives->ConsoleStop());
	return 1;
}

int Control::ConsoleStopInProgress(lua_State *L) {
	lua_pushboolean(L, mPrimitives->ConsoleStopInProgress());
	return 1;
}

int Control::GetConsolePos(lua_State *L) {
	double pos = mPrimitives->GetConsolePos();
	lua_pushnumber(L, pos);
	return 1;
}

int Control::ArmMove(lua_State *L) {
	bool left = lua_toboolean(L, 1);
	double pos = luaL_optnumber(L, 2, 0);
	double speed = luaL_optnumber(L, 3, 1000);
	double acc = luaL_optnumber(L, 4, 100);
	lua_pushboolean(L, mPrimitives->ArmMove(left, pos, speed, acc));
	return 1;
}

int Control::ArmMoveInProgress(lua_State *L) {
	bool left = lua_toboolean(L, 1);
	lua_pushboolean(L, mPrimitives->ArmMoveInProgress(left));
	return 1;
}

int Control::Magnet(lua_State *L) {
	bool left = lua_toboolean(L, 1);
	int polarity = luaL_optinteger(L, 2, 0);
	lua_pushboolean(L, mPrimitives->Magnet(left, polarity));
	return 1;
}

int Control::MagnetInProgress(lua_State *L) {
	bool left = lua_toboolean(L, 1);
	lua_pushboolean(L, mPrimitives->MagnetInProgress(left));
	return 1;
}

int Control::StartMatch(lua_State *L) {
	bool startTimer = optbool(L, 1, true);
	if (startTimer) {
		gettimeofday(&matchStart, NULL);
		matchStarted = true;
	}

	bool color = mPrimitives->GetMyColor();

	for (int i = 0; i < 36; i++) {
		int d = (i / 6 + i % 6) % 2;
		if ((color && d == 1) || (!color && d == 0)) {
			deployFields[i] = 0;
		}
	}

	int dir = 1;
	int offset = 0;
	if (color) {
		dir = -1;
		offset = 3000;
	}

	obstacles.push_back(new Circle(1885, offset + dir * 900, PAWN_RADIUS));
	obstacles.push_back(new Circle(1885, offset + dir * 2300, PAWN_RADIUS));

	return 0;
}

int Control::RefreshPawnPositions(lua_State *L) {
	if (!mCamera) {
		mCamera = new PrimitivesNet(mConfig);
		if (mCamera->CameraInit()) {
			cout << "(Control) Connected to camera" << endl;
		} else {
			cout << "(Control) Error connecting to camera" << endl;
			delete mCamera;
			mCamera = NULL;
			lua_pushboolean(L, false);
			return 1;
		}
	}
	double x, y, phi;
	mPrimitives->GetRobotPos(&x, &y, &phi);
	lua_pushboolean(L, mCamera->RefreshPawnPositions(pawns, x, y, phi));
	return 1;
}

int Control::RefreshPawnPositionsInProgress(lua_State *L) {
	if (!mCamera) {
		lua_pushboolean(L, false);
		return 1;
	}
	lua_pushboolean(L, mCamera->RefreshPawnPositionsInProgress());
	return 1;
}

int Control::RefreshPawnPositionsFinished(lua_State *L) {
	bool ret;
	if (!mCamera) {
		ret = false;
	} else {
		ret = mCamera->RefreshPawnPositionsFinished();
	}
	if (ret) {
		for (int i = 0; i < pawns->num; i++) {
			if (pawnOnOurColor(pawns->pawns[i].x, pawns->pawns[i].y)) {
				addDynamicObstacle(new Circle(pawns->pawns[i].x, pawns->pawns[i].y, PAWN_RADIUS));
			} else {
				Circle* pawn = new Circle(pawns->pawns[i].x, pawns->pawns[i].y, PAWN_RADIUS);
				removeCollidingDynamicObstacles(pawn);
				delete pawn;
			}
		}
	}
	lua_pushboolean(L, ret);
	return 1;
}

/**
 * 0: paraszt koordinatainak visszaadasa
 * 1: koordinatak gripperes felszedeshez
 * 2: koordinatak bal karhoz
 * 3: koordinatak jobb karhoz
 * 4: koordinatak oldalso parasztokhoz
 * @param L
 * @return
 */
int Control::FindPawn(lua_State *L) {
	int target = luaL_optinteger(L, 1, 0);
	double ignoreRadius = luaL_optnumber(L, 2, ROBOT_FRONT_MAX);
	double x, y, phi;
	mPrimitives->GetRobotPos(&x, &y, &phi);
	double minDist;
	int closest = pawns->num;
	for (int i = 0; i < pawns->num; i++) {
		msgpawn* pawn = &(pawns->pawns[i]);
		if (pawn->type == FIG_PAWN) {
			double dist = sqrt(sqr(pawn->x - x) + sqr(pawn->y - y));
			// tul kozeli parasztot nem probaljuk meg felszedni mert eltoljuk
			if (dist > ignoreRadius) {
				if (closest == pawns->num || dist < minDist) {
					// megnezzuk, hogy a mi mezonkon van-e
					if (pawnOnOurColor(pawn->x, pawn->y)) {
						continue;
					}

					minDist = dist;
					closest = i;
				}
			}
		}
	}
	if (closest < pawns->num) {
		double px = pawns->pawns[closest].x;
		double py = pawns->pawns[closest].y;
		lua_pushnumber(L, px);
		lua_pushnumber(L, py);
		if (target == 1) {
			double angle = atan2f(y - py, x - px);
			lua_pushnumber(L, px + cos(angle) * (ROBOT_FRONT_PAWN - 20));
			lua_pushnumber(L, py + sin(angle) * (ROBOT_FRONT_PAWN - 20));
			lua_pushnumber(L, minDist);
			return 5;
		} else if (target == 2) {
			double c2 = sqr(x - px) + sqr(y - py);
			double dx = cos(atan2(py - y, px - x) - asin(MAGNET_POS / sqrt(c2))) * sqrt(c2 - sqr(MAGNET_POS)) + x;
			double dy = sin(atan2(py - y, px - x) - asin(MAGNET_POS / sqrt(c2))) * sqrt(c2 - sqr(MAGNET_POS)) + y;
			lua_pushnumber(L, dx);
			lua_pushnumber(L, dy);
			lua_pushnumber(L, minDist);
			return 5;
		} else if (target == 3) {
			double c2 = sqr(x - px) + sqr(y - py);
			double dx = cos(atan2(py - y, px - x) + asin(MAGNET_POS / sqrt(c2))) * sqrt(c2 - sqr(MAGNET_POS)) + x;
			double dy = sin(atan2(py - y, px - x) + asin(MAGNET_POS / sqrt(c2))) * sqrt(c2 - sqr(MAGNET_POS)) + y;
			lua_pushnumber(L, dx);
			lua_pushnumber(L, dy);
			lua_pushnumber(L, minDist);
			return 5;
		} else if (target == 4) {
			lua_pushnumber(L, px);
			if (py > 1500) {
				lua_pushnumber(L, py - 400);
			} else {
				lua_pushnumber(L, py + 400);
			}
			lua_pushnumber(L, minDist);
			return 5;
		} else {
			lua_pushnumber(L, minDist);
			return 3;
		}
	} else {
		lua_pushboolean(L, false);
		return 1;
	}
}

/**
 * 0: lerakohely koordinatainak visszaadasa
 * 1: koordinatak gripperes lerakashoz
 * 2: koordinatak bal karhoz
 * 3: koordinatak jobb karhoz
 * @param L
 * @return
 */
int Control::GetDeployPoint(lua_State *L) {
	int target = luaL_optinteger(L, 1, 0);

	bool color = mPrimitives->GetMyColor();

	int dir = 1;
	int offset = 0;
	if (color) {
		dir = -1;
		offset = 3000;
	}

	int min = 1;
	int field = -1;

	for (int i = 0; i < 36; i++) {
		int d = (i / 6 + i % 6) % 2;
		if ((color && d == 0) || (!color && d == 1)) {
			if (deployFields[i] < min) {
				min = deployFields[i];
				field = i;
			}
		}
	}

	if (field != -1) {
		double x = field / 6 * 350 + 175;
		double y = field % 6 * 350 + 175 + 450;

		// vedett helyek keskenyebbek, nem a kozepukre rakunk
		if (field == 30 || field == 31 || field == 34 || field == 35) {
			x -= 40;
		}

		if (target == 1) {
			double deployDistance = ROBOT_FRONT_PAWN;
			if (x < 1050) {
				deployDistance = -ROBOT_FRONT_PAWN;
			}
			lua_pushnumber(L, x - deployDistance);
			lua_pushnumber(L, y);
			lua_pushnumber(L, x);
			lua_pushnumber(L, y);
		} else if (target == 2) {
			double deployDistance = MAGNET_POS;
			if (x < 1050) {
				deployDistance = -MAGNET_POS;
			}
			lua_pushnumber(L, x - deployDistance);
			lua_pushnumber(L, y);
			lua_pushnumber(L, x);
			lua_pushnumber(L, AREA_LENGTH);
		} else {
			lua_pushnumber(L, x);
			lua_pushnumber(L, y);
			lua_pushnumber(L, x);
			lua_pushnumber(L, y);
		}
		lua_pushinteger(L, field);
		lua_pushinteger(L, deployFields[field]);
	} else {
		lua_pushboolean(L, false);
		return 1;
	}
	return 6;
}

int Control::SetDeployPointPriority(lua_State *L) {
	int target = lua_tointeger(L, 1);
	int priority = lua_tointeger(L, 2);
	if (target < 0) {
		return 0;
	}
	if (priority < 1) {
		for (int i = 0; i < 36; i++) {
			if (deployFields[i] <= priority) {
				deployFields[i] -= 1;
			}
		}
		deployFields[target] = priority;
	} else if (priority == 1) {
		double x = target / 6 * 350 + 175;
		double y = target % 6 * 350 + 175 + 450;
		if (target == 30 || target == 31 || target == 34 || target == 35) {
			// vedett helyre statikus akadalyt teszunk, hogy veletlenul se szedjuk ki
			x -= 40;
			obstacles.push_back(new Circle(x, y, 100));
		} else {
			addDynamicObstacle(new Circle(x, y, 100));
		}
	}

	return 0;
}
