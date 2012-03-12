/*
 * Primitives.h
 *
 *  Created on: Mar 24, 2011
 *      Author: agoston
 */

#ifndef PRIMITIVES_H_
#define PRIMITIVES_H_

#include <sys/time.h>
#include <math.h>
#include "../Config.h"
#include "../tools.h"
#include "../geom.h"

typedef struct progress {
	progress() : inprogress(false), finished(false) {}
	bool inprogress;
	bool finished;
} progress;

typedef struct position {
	position() : x(0.), y(0.), phi(0.), v(0.), w(0.), a(0.) {}
	double x; // x pozicio
	double y; // y pozicio
	double phi; // elfordulas szoge
	double v; // sebesseg
	double w; // szogsebesseg
	double a; // gyorsulas
} position;

class Primitives {
public:
	Primitives(Config* config);
	Primitives(Primitives* source);
	virtual ~Primitives();

	/**
	 * inicializalja a primitives eszkozt
	 * @return sikeres volt-e az inicializalas
	 */
	virtual bool Init();

	/**
	 * varakozas a kovetkezo megszakitasig
	 * @param useconds maximum varakozasi ido microsecundumban
	 * @return false ha hiba tortent
	 */
	virtual bool Wait(long int useconds);

	/**
	 * Start gomb allapotanak lekerdezese
	 * @return igaz, ha le van nyomva
	 */
	virtual bool GetStartButton();

	/**
	 * Stop gomb allapotanak lekerdezese
	 * @return igaz, ha le van nyomva
	 */
	virtual bool GetStopButton();

	/**
	 * A szinkapcsolo allapotanak lekerdezese
	 * @return false: piros, true: kek
	 */
	virtual bool GetMyColor();

	/**
	 * Annak lekerdezese, hogy van-e paraszt a gripper-ek kozott
	 * @return igaz, ha van benn paraszt
	 */
	virtual bool PawnInGripper();

	/**
	 * motortap be/ki kapcsolasa
	 * @param powered be/ki
	 * @return true: folyamat elindult, false: hiba tortent
	 */
	virtual bool SetMotorSupply(bool powered);

	/**
	 * motortap valtoztatas allapotanak lekerdezese
	 * @return true: folyamatban van, false: nincs folyamatban
	 */
	virtual bool SetMotorSupplyInProgress();

	/**
	 * motortap allapotanak lekerdezese
	 * @return be/ki
	 */
	virtual bool GetMotorSupply();

	///////////////////////////////////////////////////////////////////////
	// MOZGASOK, POZICIOK
	///////////////////////////////////////////////////////////////////////

	/**
	 * kalibracio vegrehajtasa
	 * @param simulate true eseten nem mozog, csak beallitja a kezdopoziciot
	 * @return true: folyamat elindult, false: hiba tortent
	 */
	virtual bool CalibrateDeadreckoning(bool simulate = false);

	/**
	 * kalibracio allapota
	 * @return true: folyamatban van, false: nincs folyamatban
	 */
	virtual bool CalibrateDeadreckoningInProgress();

	/**
	 * sebessegertekek beallitasa
	 * @param v [mm/s]
	 * @param w [rad/s]
	 * @return true: folyamat elindult, false: hiba tortent
	 */
	virtual bool SetSpeed(double v, double w);

	/**
	 * sebessegertekek beallitasa
	 * @param vLeft [mm/s]
	 * @param vRight [mm/s]
	 * @return true: folyamat elindult, false: hiba tortent
	 */
	virtual bool SetWheelSpeed(double vLeft, double vRight);

	/**
	 * sebessegbeallitasok (SetSpeed, SetWheelSpeed) allapotanak lekerdezese
	 * @return true: folyamatban van, false: nincs folyamatban
	 */
	virtual bool SetSpeedInProgress();

	/**
	 * egyenes haladas
	 * @param distance [mm]
	 * @param max_speed [mm/s]
	 * @param max_acc [mm/s^2]
	 * @return true: folyamat elindult, false: hiba tortent
	 */
	virtual bool Go(double distance, double max_speed, double max_acc);

	/**
	 * egyenes haladas x,y pontba
	 * @param x a celpont x koordinataja [mm]
	 * @param y a celpont y koordinataja [mm]
	 * @param max_speed [mm/s]
	 * @param max_acc [mm/s^2]
	 * @return true: folyamat elindult, false: hiba tortent
	 */
	virtual bool GoTo(double x, double y, double max_speed, double max_acc);

	/**
	 * helyben fordulas
	 * @param angle [rad]
	 * @param max_speed [rad/s]
	 * @param max_acc [rad/s^2]
	 * @return true: folyamat elindult, false: hiba tortent
	 */
	virtual bool Turn(double angle, double max_speed, double max_acc);

	/**
	 * mozgasok (Go, GoTo, Turn) allapotanak lekerdezese
	 * @return true: folyamatban van, false: nincs folyamatban
	 */
	virtual bool MotionInProgress();

	/**
	 * legutobbi mozgas (Go, GoTo, Turn) hibajanak lekerdezese
	 * @return 0: nem tortent hiba, 1: ...
	 */
	virtual int GetMotionError();

	/**
	 * mozgas megszakitasa
	 * @param dec maximalis lassulas merteke [mm/s^2], 0 eseten hard stop
	 * @return true: folyamat elindult, false: hiba tortent
	 */
	virtual bool MotionStop(double dec = 0.0);

	/**
	 * megallas allapotanak lekerdezese
	 * @return true: folyamatban van, false: nincs folyamatban
	 */
	virtual bool MotionStopInProgress();

	/**
	 * A robot aktualis poziciojat es orientaciojat adja vissza
	 * A koordinatarendszer a kovetkezo:
	 *
	 *       |<------------- 3000 ------------->|
	 *
	 *  -    0------- y -----> -----------------
	 *  ^    |     |                      |     |
	 *  |    |piros|                      | kek |
	 *  |    |-----                        -----|
	 *  |    x                                  |
	 * 2100  |           |-O---|                |
	 *  |    |           |robot|-->             |
	 *  |    v           |-O---|                |
	 *  |    |                                  |
	 *  |    |                                  |
	 *  v    |                                  |
	 *  -     ----------------------------------
	 *
	 * Az orientacio (phi) a robot haladasi iranyanak
	 * az x tengellyel bezart pozitiv szoge.
	 * @param x [mm]
	 * @param y [mm]
	 * @param phi [rad]
	 */
	virtual void GetRobotPos(double* x, double* y, double* phi);

	/**
	 * ellenfel poziciojanak lekerdezese
	 * @param x [mm]
	 * @param y [mm]
	 * @return utolso frissites ota eltelt ido [ms]
	 */
	virtual long int GetOpponentPos(double * x, double* y);

	/**
	 * sebessegertekek lekerdezese
	 * @param v [mm/s]
	 * @param w [rad/s]
	 */
	virtual void GetSpeed(double* v, double* w);

	/**
	 * robot poziciojanak beallitasa
	 * @param x [mm]
	 * @param y [mm]
	 * @param phi [rad]
	 */
	virtual void SetRobotPos(double x, double y, double phi);

	/**
	 * ellenfel poziciojanak beallitasa
	 * @param x [mm]
	 * @param y [mm]
	 */
	virtual void SetOpponentPos(double x, double y);

	/**
	 * Visszaadja a tavolsagerzekelok erteket
	 * @param distance a hat tavolsagerzekelo erteke [mm]
	 */
	virtual void GetDistances(double distance[6]);

	///////////////////////////////////////////////////////////////////////
	// GRIPPER
	///////////////////////////////////////////////////////////////////////

	/**
	 * elso megfogo mozgatasa
	 * @param pos [deg], 0 zart helyzet, 90 nyitott helyzet
	 * @return true: folyamat elindult, false: hiba tortent
	 */
	virtual bool GripperMove(double pos);

	/**
	 * elso megfogo mozgatas allapota
	 * @return true: folyamatban van, false: nincs folyamatban
	 */
	virtual bool GripperMoveInProgress();

	/**
	 * elso megfogo szogenek lekerdezese
	 * @return [mm], zart helyzet 0
	 */
	virtual double GetGripperPos();

	///////////////////////////////////////////////////////////////////////
	// CONSOLE
	///////////////////////////////////////////////////////////////////////

	/**
	 * Konzol kalibralas
	 * @return true: folyamat elindult, false: hiba tortent
	 */
	virtual bool CalibrateConsole();

	/**
	 * CalibrateConsole allapota
	 * @return true: folyamatban van, false: nincs folyamatban
	 */
	virtual bool CalibrateConsoleInProgress();

	/**
	 * konzol mozgatas
	 * @param pos abszolut pozicio [mm], also pozicio 0
	 * @param max_speed [mm/s]
	 * @param max_acc [mm/s^2]
	 * @return true: folyamat elindult, false: hiba tortent
	 */
	virtual bool ConsoleMove(double pos, double max_speed, double max_acc);

	/**
	 * konzol mozgatas allapota
	 * @return true: folyamatban van, false: nincs folyamatban
	 */
	virtual bool ConsoleMoveInProgress();

	/**
	 * Konzol mozgatas leallitasa
	 * @return true: folyamat elindult, false: hiba tortent
	 */
	virtual bool ConsoleStop();

	/**
	 * ConsoleStop allapota
	 * @return true: folyamatban van, false: nincs folyamatban
	 */
	virtual bool ConsoleStopInProgress();

	/**
	 * Konzol pozicio lekerdezese
	 * @return [mm], also pozicio 0
	 */
	virtual double GetConsolePos();

	///////////////////////////////////////////////////////////////////////
	// ARM (elorol hatrafele nezve bal oldali, jobb oldali)
	///////////////////////////////////////////////////////////////////////

	/**
	 * Kar mozgatasa
	 * @param left bal/jobb kar mozgatasa
	 * @param pos abszolut pozicio [deg], 0 alaphelyzet (fuggoleges), lefele no
	 * @param max_speed [deg/s]
	 * @param max_acc [deg/s^2]
	 * @return true: folyamat elindult, false: hiba tortent
	 */
	virtual bool ArmMove(bool left, double pos, double max_speed, double max_acc);

	/**
	 * Kar mozgatas allapota
	 * @param left bal/jobb kar allapota
	 * @return true: folyamatban van, false: nincs folyamatban
	 */
	virtual bool ArmMoveInProgress(bool left);

	/**
	 * Kar poziciojanak lekerdezese
	 * @param left bal/jobb kar
	 * @return abszolut pozicio [deg], 0 alaphelyzet (fuggoleges), lefele no
	 */
	virtual double GetArmPos(bool left);

	/**
	 * Magnes polaritasanak valtoztatasa
	 * @param left bal/jobb magnes
	 * @param polarity 1: vonzas, 0: kikapcsolt, -1: taszitas
	 * @return true: folyamat elindult, false: hiba tortent
	 */
	virtual bool Magnet(bool left, int polarity);

	/**
	 * Magnes polaritas valtas allapota
	 * @param left bal/jobb magnes
	 * @return true: folyamatban van, false: nincs folyamatban
	 */
	virtual bool MagnetInProgress(bool left);

protected:
	/**
	 * konfiguraciot tartalmazo objektum
	 */
	Config* mConfig;

	/**
	 * a szinkapcsolo allasat tarolja
	 */
	bool mRobotColor;
	/**
	 * a startgomb allapotat tarolja
	 */
	bool mStartButton;
	/**
	 * a stopgomb allapotat tarolja
	 */
	bool mStopButton;

	/**
	 * a parasztkapcsolo allapotat tarolja
	 */
	bool mPawnInGripper;

	/**
	 * a robot es az ellenfel pozicioja
	 */
	position robot, opponent;

	/**
	 * mozgasoknal a celpozicio
	 */
	position target;

	progress calibrateDeadreckoning;
	progress go;
	progress goTo;
	progress turn;
	progress motionStop;

	progress gripperMove;
	double gripperPos;
	progress consoleMove;
	double consolePos;
	progress leftArmMove, rightArmMove;
	double leftArmPos, rightArmPos;

};

#endif /* PRIMITIVES_H_ */
