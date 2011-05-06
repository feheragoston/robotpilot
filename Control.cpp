/*
 * Control.cpp
 *
 *  Created on: Mar 24, 2011
 *      Author: agoston
 */

#include "Control.h"

Primitives* Control::mPrimitives = NULL;
Server* Control::mServer = NULL;
bool Control::matchStarted = false;
bool Control::exitControl = false;

Control::Control(Config* config) {
	mConfig = config;
	this->L = luaL_newstate();
	luaL_openlibs(L);

	lua_register(L, "Exit", LuaExit);
	lua_register(L, "ControlWait", LuaWait);
	lua_register(L, "Control", LuaControl);
	lua_register(L, "RunParallel", LuaRunParallel);
	lua_register(L, "Print", LuaPrint);
	lua_register(L, "Test", LuaTest);
	lua_register(L, "Test1", LuaTest);
	lua_register(L, "Test2", LuaTest);

	lua_register(L, "GetStartButton", LuaGetStartButton);
	lua_register(L, "GetStopButton", LuaGetStopButton);
	lua_register(L, "GetMyColor", LuaGetMyColor);
	lua_register(L, "MotorSupply", LuaMotorSupply);

	lua_register(L, "CalibrateDeadreckoning", LuaCalibrateDeadreckoning);
	lua_register(L, "SetSpeed", LuaSetSpeed);
	lua_register(L, "Go", LuaGo);
	lua_register(L, "GoTo", LuaGoTo);
	lua_register(L, "Turn", LuaTurn);
	lua_register(L, "TurnTo", LuaTurnTo);
	lua_register(L, "MotionStop", LuaMotionStop);
	lua_register(L, "GetRobotPos", LuaGetRobotPos);
	lua_register(L, "GetOpponentPos", LuaGetOpponentPos);

	lua_register(L, "SetGripperPos", LuaSetGripperPos);
	lua_register(L, "CalibrateConsole", LuaCalibrateConsole);
	lua_register(L, "SetConsolePos", LuaSetConsolePos);
	lua_register(L, "ConsoleStop", LuaConsoleStop);
	lua_register(L, "GetConsolePos", LuaGetConsolePos);
	lua_register(L, "SetArmPos", LuaSetArmPos);

	matchStarted = false;
	exitControl = false;
}

Control::~Control() {
	if (mPrimitives) {
		delete mPrimitives;
	}
	lua_close(L);
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
		}

		fd_set rfd;
		char buf[2048];
		size_t nbytes = sizeof(buf) - 1;
		ssize_t bytes_read;
		struct timeval tv;
		tv.tv_sec = 0;
		tv.tv_usec = 0;
		std::string luaBuffer = "";

		while (!exitControl) {
			if (!mPrimitives->Wait(50000)) {
				return;
			}
			log();

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
					if (noread) {
						s = luaL_loadstring(L, luaBuffer.c_str());
						luaBuffer = "";
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
		mPrimitives->GetOpponentPos(&(response.ox), &(response.oy));
		response.startButton = mPrimitives->GetStartButton();
		response.stopButton = mPrimitives->GetStopButton();
		response.color = mPrimitives->GetMyColor();
		mServer->Send(n, &response, sizeof(msgstatus));
	} else {
		printf("Unknown or invalid function: %d size: %d\n", *function, size);
	}
}

void Control::log() {
	mServer->Process();
}

void Control::report_errors(lua_State *L, int status) {
	if (status != 0) {
		std::cerr << "-- " << lua_tostring(L, -1) << std::endl;
		lua_pop(L, 1); // remove error message
	}
}

bool Control::optbool(lua_State *L, int narg, bool d) {
	if (lua_isboolean(L, narg)) {
		return lua_toboolean(L, narg);
	}
	return d;
}

int Control::LuaExit(lua_State *L) {
	exitControl = true;
	luaL_error(L, "Exit() called, exiting\n");
	return 0;
}

int Control::LuaWait(lua_State *L) {
	long int useconds = luaL_optinteger(L, 1, 50000);
	if (!mPrimitives->Wait(useconds)) {
		exitControl = true;
		return luaL_error(L, "Primitives->Wait failed, exiting\n");
	}
	log();
	return 0;
}

int Control::LuaControl(lua_State *L) {
	int i = lua_pushthread(L);
	lua_pop(L, 1);
	if (i == 1) {
		lua_getfield(L, LUA_GLOBALSINDEX, "ControlWait");
		lua_call(L, 0, 0);
		return 0;
	} else {
		return lua_yield(L, 0);
	}
}

int Control::LuaRunParallel(lua_State *L) {
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
				std::cout << "Parallel thread error: " << luaL_optstring(N, -1, "-") << std::endl;
				/* hibakereses
				std::cout << "utana: " << lua_status(N) << std::endl;
				for (std::list<lua_State*>::iterator j = threads.begin(); j != threads.end(); j++) {
					if (lua_status(*j) != LUA_YIELD && lua_status(*j) != 0) {
						std::cout << i << " futas utan elrontott lua thread: " << lua_status(*j) << std::endl;
					}
				}
				*/
			}
			threads.pop_front();
		}
		if (threads.size() > 0) {
			lua_getfield(L, LUA_GLOBALSINDEX, "Control");
			lua_call(L, 0, 0);
		}
	}
	return 0;
}

int Control::LuaPrint(lua_State *L) {
	int argc = lua_gettop(L);
	for (int n = 1; n <= argc; ++n) {
		if (n > 1) {
			std::cout << " ";
		}
		if (!strcmp(luaL_typename(L, n), "string")) {
			const char* s = lua_tostring(L, n);
			std::cout << s;
		} else if (!strcmp(luaL_typename(L, n), "number")) {
			double x = lua_tonumber(L, n);
			std::cout << x;
		} else if (!strcmp(luaL_typename(L, n), "boolean")) {
			bool b = lua_toboolean(L, n);
			std::cout << b;
		} else {
			std::cout << luaL_typename(L, n);
		}
	}
	std::cout << std::endl;
	return 0;
}

int Control::LuaTest(lua_State *L) {
	std::cout << "test status: " << lua_status(L) << std::endl;
	return 0;

	lua_Debug ar;
	lua_getstack(L, 0, &ar);
	lua_getinfo(L, "nS", &ar);
	std::cout << "name: " << ar.name << " what: " << ar.what << " namewhat: " << ar.namewhat;

	int i = lua_pushthread(L);
	lua_pop(L, 1);
	std::cout << " " << i;

	std::cout << std::endl;
	return 0;
}

int Control::LuaGetStartButton(lua_State *L) {
	bool b = mPrimitives->GetStartButton();
	lua_pushboolean(L, b);
	return 1;
}

int Control::LuaGetStopButton(lua_State *L) {
	bool b = mPrimitives->GetStopButton();
	lua_pushboolean(L, b);
	return 1;
}

int Control::LuaGetMyColor(lua_State *L) {
	bool b = mPrimitives->GetMyColor();
	lua_pushboolean(L, b);
	return 1;
}

int Control::LuaMotorSupply(lua_State *L) {
	bool powered = lua_toboolean(L, 1);
	int i = mPrimitives->MotorSupply(powered);
	lua_pushinteger(L, i);
	return 1;
}

int Control::LuaCalibrateDeadreckoning(lua_State *L) {
	bool simulate = optbool(L, 1, false);
	int i = mPrimitives->CalibrateDeadreckoning(simulate);
	lua_pushinteger(L, i);
	return 1;
}

int Control::LuaSetSpeed(lua_State *L) {
	double v = luaL_optnumber(L, 1, 0);
	double w = luaL_optnumber(L, 2, 0);
	int i = mPrimitives->SetSpeed(v, w);
	lua_pushinteger(L, i);
	return 1;
}

int Control::LuaGo(lua_State *L) {
	double distance = luaL_optnumber(L, 1, 1000);
	double speed = luaL_optnumber(L, 2, 1000);
	double acc = luaL_optnumber(L, 3, 500);
	int i = mPrimitives->Go(distance, speed, acc);
	lua_pushinteger(L, i);
	return 1;
}

int Control::LuaGoTo(lua_State *L) {
	double x = lua_tonumber(L, 1);
	double y = lua_tonumber(L, 2);
	double speed = luaL_optnumber(L, 3, 1000);
	double acc = luaL_optnumber(L, 4, 500);
	int i = mPrimitives->GoTo(x, y, speed, acc);
	lua_pushinteger(L, i);
	return 1;
}

int Control::LuaTurn(lua_State *L) {
	double angle = luaL_optnumber(L, 1, M_PI_2);
	double speed = luaL_optnumber(L, 2, 2);
	double acc = luaL_optnumber(L, 3, 2);
	while (angle > M_PI) {
		angle -= M_PI * 2;
	}
	while (angle < -M_PI) {
		angle += M_PI * 2;
	}
	int i = mPrimitives->Turn(angle, speed, acc);
	lua_pushinteger(L, i);
	return 1;
}

int Control::LuaTurnTo(lua_State *L) {
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
	int i = mPrimitives->Turn(angle, speed, acc);
	lua_pushinteger(L, i);
	return 1;
}

int Control::LuaMotionStop(lua_State *L) {
	double dec = luaL_optnumber(L, 1, 0);
	int  i = mPrimitives->MotionStop(dec);
	lua_pushinteger(L, i);
	return 1;
}

int Control::LuaGetRobotPos(lua_State *L) {
	double x, y, phi;
	mPrimitives->GetRobotPos(&x, &y, &phi);
	lua_pushnumber(L, x);
	lua_pushnumber(L, y);
	lua_pushnumber(L, phi);
	return 3;
}

int Control::LuaGetOpponentPos(lua_State *L) {
	double x, y;
	mPrimitives->GetOpponentPos(&x, &y);
	lua_pushnumber(L, x);
	lua_pushnumber(L, y);
	return 2;
}

int Control::LuaSetGripperPos(lua_State *L) {
	double pos = luaL_optnumber(L, 1, 0);
	int i = mPrimitives->SetGripperPos(pos);
	lua_pushinteger(L, i);
	return 1;
}

int Control::LuaCalibrateConsole(lua_State *L) {
	int i = mPrimitives->CalibrateConsole();
	lua_pushinteger(L, i);
	return 1;
}

int Control::LuaSetConsolePos(lua_State *L) {
	double pos = luaL_optnumber(L, 1, 0);
	double speed = luaL_optnumber(L, 2, 2);
	double acc = luaL_optnumber(L, 3, 10);
	int i = mPrimitives->SetConsolePos(pos, speed, acc);
	lua_pushinteger(L, i);
	return 1;
}

int Control::LuaConsoleStop(lua_State *L) {
	int i = mPrimitives->ConsoleStop();
	lua_pushinteger(L, i);
	return 1;
}

int Control::LuaGetConsolePos(lua_State *L) {
	double pos = mPrimitives->GetConsolePos();
	lua_pushnumber(L, pos);
	return 1;
}

int Control::LuaSetArmPos(lua_State *L) {
	bool left = lua_toboolean(L, 1);
	double pos = luaL_optnumber(L, 2, 0);
	double speed = luaL_optnumber(L, 3, 2);
	double acc = luaL_optnumber(L, 4, 10);
	int i = mPrimitives->SetArmPos(left, pos, speed, acc);
	lua_pushinteger(L, i);
	return 1;
}
