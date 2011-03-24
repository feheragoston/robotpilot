/*
 * Control.h
 *
 *  Created on: Mar 24, 2011
 *      Author: agoston
 */

#ifndef CONTROL_H_
#define CONTROL_H_

#include <sys/time.h>
#include <string>

extern "C" {
#include "lua/lua.h"
#include "lua/lualib.h"
#include "lua/lauxlib.h"
}

#include "Config.h"
#include "Primitives/Primitives.h"

class Control {
public:
	Control(Config* config);
	virtual ~Control();
	/*
	 * Inicializalas, egyszer kell lefuttatni Run elott
	 */
	bool Init();
	/*
	 * Allapotgep inditasa
	 */
	void Run();

protected:
	Config* mConfig;
	static Primitives* mPrimitives;

	lua_State *L;

	struct timeval runStart; // Run indulasanak ideje
	struct timeval initStart; // Init indulasanak ideje
	struct timeval matchStart; // Start gomb megnyomasanak ideje
	static bool matchStarted; // Meccs elkezdodott
	static bool exitControl;

	void report_errors(lua_State *L, int status);
	static bool optbool(lua_State *L, int narg, bool d);

	static int LuaExit(lua_State *L);
	static int LuaWait(lua_State *L);
	static int LuaPrint(lua_State *L);

};

#endif /* CONTROL_H_ */
