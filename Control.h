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
#include "nokia_server.h"

#define luaC_export(L,s)	lua_pushnumber(L, s); lua_setfield(L, LUA_GLOBALSINDEX, #s)

typedef std::list<Obstacle*> obstacleList;
typedef std::list<Obstacle*>::iterator obstacleIterator;

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
	static nokia_server* ns;
	static unsigned int nokia_sent;

	static int logfile;

	static obstacleList obstacles;
	static obstacleList highObstacles;
	static obstacleList dynObstacles;
	static obstacleList collisionObstacles;
	static bool sendObstacles[MAX_CONNECTIONS];
	static bool sendDynObstacles[MAX_CONNECTIONS];
	static double robotBody[][2];
	static obstacleList robotObstacles;
	static obstacleList robotHighObstacles;
	static Circle* opponent[OPPONENT_NUM];
	static double opponent_x[OPPONENT_NUM], opponent_y[OPPONENT_NUM];
	static double angry[OPPONENT_NUM];

	static bool simulate;
	static bool safeMotion;

	lua_State *L;

	static struct timeval runStart; // Run indulasanak ideje
	static struct timeval initStart; // Init indulasanak ideje
	static struct timeval matchStart; // Start gomb megnyomasanak ideje
	static bool matchStarted; // Meccs elkezdodott
	static bool exitControl;

	static bool logObstacles;
	static bool logDynObstacles;
	static void log();

	static void setSafeMotion(lua_State *L);
	static long int refreshOpponent(unsigned char n);
	static bool opponentTooClose();
	static bool obstacleCollision();
	static void addDynamicObstacle(Obstacle* obstacle);
	static void clearCollisionObstacles();

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
	 * @param mode COLLISION_ALL, COLLISION_OBSTACLES, COLLISION_HIGH
	 * @return true ha van utkozes
	 */
	static bool checkLine(double x1, double y1, double x2, double y2, int mode = COLLISION_ALL);

	static void report_errors(lua_State *L, int status);
	static bool optbool(lua_State *L, int narg, bool d);

	static bool connectCamera();

	static int c_gettimeofday(lua_State *L);
	static int c_getelapsedtime(lua_State *L);
	static int c_matchtime(lua_State *L);

	static int c_exit(lua_State *L);
	static int c_wait(lua_State *L);
	static int c_simulate(lua_State *L);
	static int c_in_simulate(lua_State *L);
	static int c_print(lua_State *L);
	static int c_music(lua_State *L);

	static int l_GetStartButton(lua_State *L);
	static int l_GetStopButton(lua_State *L);
	static int l_GetMyColor(lua_State *L);
	static int l_GetDistances(lua_State *L);
	static int l_SetMotorSupply(lua_State *L);

	static int l_CalibrateDeadreckoning(lua_State *L);
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
	static long int GetOpponentPos(unsigned char n, double* ox, double* oy);
	static void SetOpponentPos(unsigned char n, double ox, double oy);
	static int l_GetOpponentPos(lua_State *L);

#ifdef NAGY_ROBOT
	static int l_ArmMove(lua_State *L);
	static int l_ArmMoveInProgress(lua_State *L);
	static int l_GetArmPos(lua_State *L);
	static int l_SelectorMove(lua_State *L);
	static int l_SelectorMoveInProgress(lua_State *L);
	static int l_GetSelectorPos(lua_State *L);
	static int l_FlipperMove(lua_State *L);
	static int l_FlipperMoveInProgress(lua_State *L);
	static int l_GetFlipperPos(lua_State *L);
	static int l_GetFlipperError(lua_State *L);
	static int l_ContractorMove(lua_State *L);
	static int l_ContractorMoveInProgress(lua_State *L);
	static int l_GetContractorPos(lua_State *L);
	static int l_GetContractorError(lua_State *L);
	static int l_FireStopperMove(lua_State *L);
	static int l_FireStopperMoveInProgress(lua_State *L);
	static int l_GetFireStopperPos(lua_State *L);
#else	//KIS_ROBOT
	static int l_GripperMove(lua_State *L);
	static int l_GripperMoveInProgress(lua_State *L);
	static int l_GetGripperPos(lua_State *L);
#endif

	static int l_CalibrateConsole(lua_State *L);
	static int l_CalibrateConsoleInProgress(lua_State *L);
	static int l_ConsoleMove(lua_State *L);
	static int l_ConsoleMoveInProgress(lua_State *L);
	static int l_ConsoleStop(lua_State *L);
	static int l_ConsoleStopInProgress(lua_State *L);
	static int l_GetConsolePos(lua_State *L);

#ifdef NAGY_ROBOT
	static int l_GetBallColorVoltage(lua_State *L);
	static int l_GetBallPresent(lua_State *L);

	static int l_CaracoleSetSpeed(lua_State *L);
	static int l_CaracoleSetSpeedInProgress(lua_State *L);
	static int l_GetCaracoleSpeed(lua_State *L);

	static int l_FirewheelSetSpeed(lua_State *L);
	static int l_FirewheelSetSpeedInProgress(lua_State *L);
	static int l_GetFirewheelSpeed(lua_State *L);
#endif

	static int l_StartMatch(lua_State *L);

	static int l_AddTestObstacles(lua_State *L);
	static int l_ClearDynObstacles(lua_State *L);
};

#endif /* CONTROL_H_ */
