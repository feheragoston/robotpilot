/*
 * Control.cpp
 *
 *  Created on: Mar 24, 2011
 *      Author: agoston
 */

#include "Control.h"

Primitives* Control::mPrimitives = NULL;
bool Control::matchStarted = false;
bool Control::exitControl = false;

Control::Control(Config* config) {
	mConfig = config;
	this->L = luaL_newstate();
	luaL_openlibs(L);

	lua_register(L, "Exit", LuaExit);
	lua_register(L, "ControlWait", LuaWait);
	lua_register(L, "Print", LuaPrint);

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
	} else {
		mPrimitives = new Primitives(mConfig);
	}

	if (mPrimitives->Init()) {
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
			if (!mPrimitives->Wait()) {
				return;
			}
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
	return 0;
}

int Control::LuaWait(lua_State *L) {
	mPrimitives->Wait();
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
