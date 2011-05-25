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
#include <list>

extern "C" {
#include "lua/lua.h"
#include "lua/lualib.h"
#include "lua/lauxlib.h"
}

#include "Config.h"
#include "Primitives/Primitives.h"
#include "Primitives/PrimitivesCan.h"
#include "Primitives/PrimitivesNet.h"
#include "Server.h"
#include "Primitives/Net/Protocol/Protocol.h"
#include "Obstacles/Circle.h"
#include "Obstacles/Line.h"

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

	static void serverMessageCallback(int n, const void* message, msglen_t size);

protected:
	Config* mConfig;
	static Primitives* mPrimitives;
	static PrimitivesNet* mCamera;
	static Server* mServer;

	static msgpawns* pawns;

	static std::list<Obstacle*> obstacles;
	static double robotBody[][2];
	static Circle* opponent;
	static double angry;
	static int deployFields[30];

	static bool simulate;

	lua_State *L;

	struct timeval runStart; // Run indulasanak ideje
	struct timeval initStart; // Init indulasanak ideje
	struct timeval matchStart; // Start gomb megnyomasanak ideje
	static bool matchStarted; // Meccs elkezdodott
	static bool exitControl;

	static void log();

	static void refreshOpponent();
	static bool opponentTooClose();
	static bool obstacleCollision();
	/**
	 * Szakasz utkozesenek tesztelese ellenfellel es akadalyokkal
	 * @param x1
	 * @param y1
	 * @param x2
	 * @param y2
	 * @return true ha nincs utkozes
	 */
	static bool checkLine(double x1, double y1, double x2, double y2);

	static void report_errors(lua_State *L, int status);
	static bool optbool(lua_State *L, int narg, bool d);

	// lua exception handling
	// http://lua-users.org/wiki/FinalizedExceptions
	static int safecall(lua_State *L);
	static int protect(lua_State *L);
	static int finalize(lua_State *L);
	static int do_nothing(lua_State *L);
	static int newtry(lua_State *L);

	static int LuaExit(lua_State *L);
	static int LuaWait(lua_State *L);
	static int LuaControl(lua_State *L);
	static int LuaRunParallel(lua_State *L);
	static int LuaSimulate(lua_State *L);
	static int LuaPrint(lua_State *L);
	static int LuaTest(lua_State *L);

	static int LuaGetStartButton(lua_State *L);
	static int LuaGetStopButton(lua_State *L);
	static int LuaGetMyColor(lua_State *L);
	static int LuaPawnInGripper(lua_State *L);
	static int LuaMotorSupply(lua_State *L);

	static int LuaCalibrateDeadreckoning(lua_State *L);
	static int LuaSetSpeed(lua_State *L);
	static int LuaGo(lua_State *L);
	static int LuaGoTo(lua_State *L);
	static int LuaTurn(lua_State *L);
	static int LuaTurnTo(lua_State *L);
	static int LuaMotionStop(lua_State *L);
	static int LuaGetRobotPos(lua_State *L);
	static int LuaGetOpponentPos(lua_State *L);

	static int LuaSetGripperPos(lua_State *L);
	static int LuaCalibrateConsole(lua_State *L);
	static int LuaSetConsolePos(lua_State *L);
	static int LuaConsoleStop(lua_State *L);
	static int LuaGetConsolePos(lua_State *L);
	static int LuaSetArmPos(lua_State *L);
	static int LuaMagnet(lua_State *L);

	static int LuaRefreshPawnPositions(lua_State *L);
	static int LuaFindPawn(lua_State *L);
	static int LuaGetDeployPoint(lua_State *L);
	static int LuaSetDeployPointPriority(lua_State *L);
};

#endif /* CONTROL_H_ */
