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
	static Config* mConfig;
	static Primitives* mPrimitives;
	static PrimitivesNet* mCamera;
	static Server* mServer;

	static int logfile;

	static msgpawns* pawns;

	static std::list<Obstacle*> obstacles;
	static std::list<Obstacle*> dynObstacles;
	static bool sendDynObstacles;
	static double robotBody[][2];
	static Circle* opponent;
	static double opponent_x, opponent_y;
	static double angry;
	static int deployFields[36];

	static bool simulate;
	static bool safeMotion;

	lua_State *L;

	static struct timeval runStart; // Run indulasanak ideje
	static struct timeval initStart; // Init indulasanak ideje
	static struct timeval matchStart; // Start gomb megnyomasanak ideje
	static struct timeval sleepCalled; // utolso Sleep hivas ideje
	static long int timeToSleep;
	static bool matchStarted; // Meccs elkezdodott
	static bool exitControl;

	static bool logDeployFields;
	static bool logDynObstacles;
	static bool logPawns;
	static void log();

	static void setSafeMotion(lua_State *L);
	static long int refreshOpponent();
	static bool opponentTooClose();
	static bool obstacleCollision();
	static void removeCollidingDynamicObstacles(Obstacle* obstacle);
	static void addDynamicObstacle(Obstacle* obstacle);
	static bool pawnOnOurColor(double x, double y);

	/**
	 * futasido ezredmasodpercben
	 * @return [ms]
	 */
	static unsigned int RunTime();
	/**
	 * Init ota eltelt ido
	 * @return [ms]
	 */
	static unsigned int InitTime();
	/**
	 * Meccs kezdes ota eltelt ido vagy 0 ha nincs meccs
	 * @return [ms]
	 */
	static unsigned int MatchTime();

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

	static bool connectCamera();

	static int c_gettimeofday(lua_State *L);
	static int c_getelapsedtime(lua_State *L);

	static int c_exit(lua_State *L);
	static int c_wait(lua_State *L);
	static int c_process(lua_State *L);
	static int c_runparallel(lua_State *L);
	static int c_simulate(lua_State *L);
	static int c_print(lua_State *L);
	static int c_music(lua_State *L);

	static int l_GetStartButton(lua_State *L);
	static int l_GetStopButton(lua_State *L);
	static int l_GetMyColor(lua_State *L);
	static int l_PawnInGripper(lua_State *L);
	static int l_GetDistances(lua_State *L);
	static int l_GetDistance(lua_State *L);
	static int l_SetMotorSupply(lua_State *L);

	static int l_CalibrateDeadreckoning(lua_State *L);
	static int l_RefineDeadreckoning(lua_State *L);
	static int l_SetSpeed(lua_State *L);
	static int l_Go(lua_State *L);
	static int l_GoTo(lua_State *L);
	static int l_Turn(lua_State *L);
	static int l_TurnTo(lua_State *L);
	static int l_MotionInProgress(lua_State *L);
	static int l_GetMotionError(lua_State *L);
	static int l_MotionStop(lua_State *L);
	static int l_MotionStopInProgress(lua_State *L);
	static int l_GetRobotPos(lua_State *L);
	static int l_SetRobotPos(lua_State *L);
	static long int GetOpponentPos(double* ox, double* oy);
	static void SetOpponentPos(double ox, double oy);
	static int l_GetOpponentPos(lua_State *L);

	static int l_GripperMove(lua_State *L);
	static int l_GripperMoveInProgress(lua_State *L);
	static int l_CalibrateConsole(lua_State *L);
	static int l_ConsoleMove(lua_State *L);
	static int l_ConsoleMoveInProgress(lua_State *L);
	static int l_ConsoleStop(lua_State *L);
	static int l_ConsoleStopInProgress(lua_State *L);
	static int l_GetConsolePos(lua_State *L);
	static int l_ArmMove(lua_State *L);
	static int l_ArmMoveInProgress(lua_State *L);
	static int l_Magnet(lua_State *L);
	static int l_MagnetInProgress(lua_State *L);

	static int l_StartMatch(lua_State *L);
	static int l_RefreshPawnPositions(lua_State *L);
	static int l_RefreshPawnPositionsInProgress(lua_State *L);
	static int l_RefreshPawnPositionsFinished(lua_State *L);
	static int l_FindPawn(lua_State *L);
	static int l_GetDeployPoint(lua_State *L);
	static int l_SetDeployPointPriority(lua_State *L);
};

#endif /* CONTROL_H_ */
