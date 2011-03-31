/*
 * Primitives.h
 *
 *  Created on: Mar 24, 2011
 *      Author: agoston
 */

#ifndef PRIMITIVES_H_
#define PRIMITIVES_H_

#include <sys/time.h>
#include "../Config.h"
#include "../tools.h"

class Primitives {
public:
	Primitives(Config* config);
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
	 * motortap be/ki kapcsolasa
	 * @param powered be/ki
	 */
	virtual int MotorSupply(bool powered);

	///////////////////////////////////////////////////////////////////////
	// MOZGASOK, POZICIOK
	///////////////////////////////////////////////////////////////////////

	/**
	 * kalibracio vegrehajtasa
	 * @param simulate true eseten nem mozog, csak beallitja a kezdopoziciot
	 * @return 0: folyamatban, 1: sikeres, -1: sikertelen
	 */
	virtual int CalibrateDeadreckoning(bool simulate = false);

	/**
	 * sebessegertekek beallitasa
	 * @param v [mm/s]
	 * @param w [rad/s]
	 * @return sebesseg beallitas sikeres
	 */
	virtual int SetSpeed(double v, double w);

	/**
	 * egyenes haladas
	 * @param distance [mm]
	 * @param max_speed [mm/s]
	 * @param max_acc [mm/s^2]
	 * @return 0: folyamatban, 1: sikeres, -1: sikertelen
	 */
	virtual int Go(double distance, double max_speed, double max_acc);

	/**
	 * egyenes haladas x,y pontba
	 * @param x a celpont x koordinataja [mm]
	 * @param y a celpont y koordinataja [mm]
	 * @param max_speed [mm/s]
	 * @param max_acc [mm/s^2]
	 * @return 0: folyamatban, 1: sikeres, -1: sikertelen
	 */
	virtual int GoTo(double x, double y, double max_speed, double max_acc);

	/**
	 * helyben fordulas
	 * @param angle [rad]
	 * @param max_speed [rad/s]
	 * @param max_acc [rad/s^2]
	 * @return 0: folyamatban, 1: sikeres, -1: sikertelen
	 */
	virtual int Turn(double angle, double max_speed, double max_acc);

	/**
	 * mozgas megszakitasa
	 * @param dec maximalis lassulas merteke [mm/s^2], 0 eseten hard stop
	 * @return 0: folyamatban, 1: sikeres, -1: sikertelen
	 */
	virtual int MotionStop(double dec = 0.0);

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
	 */
	virtual void GetOpponentPos(double * x, double* y);

	/**
	 * robot poziciojanak beallitasa
	 * @param x [mm]
	 * @param y [mm]
	 * @param phi [rad]
	 */
	virtual void SetRobotPos(double x, double y, double phi);

	/**
	 * Visszaadja a tavolsagerzekelok erteket
	 * @param distance a hat tavolsagerzekelo erteke [mm]
	 */
	virtual void GetDistances(double distance[6]);

	///////////////////////////////////////////////////////////////////////
	// GRIPPER
	///////////////////////////////////////////////////////////////////////

	/**
	 * elso megfogo szogenek beallitasa
	 * @param pos [deg]
	 * @return 0: folyamatban, 1: sikeres, -1: sikertelen
	 */
	virtual int SetGripperPos(double pos);

	///////////////////////////////////////////////////////////////////////
	// CONSOLE
	///////////////////////////////////////////////////////////////////////

	/**
	 * Konzol kalibralas
	 * @return 0: folyamatban, 1: sikeres, -1: sikertelen
	 */
	virtual int CalibrateConsole();

	/**
	 * konzol mozgatas
	 * @param pos abszolut pozicio [mm], also pozicio 0
	 * @param max_speed [mm/s]
	 * @param max_acc [mm/s^2]
	 * @return 0: folyamatban, 1: sikeres, -1: sikertelen
	 */
	virtual int SetConsolePos(double pos, double max_speed, double max_acc);

	/**
	 * Konzol mozgatas leallitasa
	 */
	virtual int ConsoleStop();

	/**
	 * Konzol pozicio lekerdezese
	 * @return [mm], also pozicio 0
	 */
	virtual double GetConsolePos();

	///////////////////////////////////////////////////////////////////////
	// ARM (elorol hatrafele nezve bal oldali, jobb oldali)
	///////////////////////////////////////////////////////////////////////

	/**
	 * Kar poziciojanak beallitasa
	 * @param left bal/jobb kar mozgatasa
	 * @param pos abszolut pozicio [deg], 0 alaphelyzet (fuggoleges), lefele no
	 * @param max_speed [deg/s]
	 * @param max_acc [deg/s^2]
	 * @return 0: folyamatban, 1: sikeres, -1: sikertelen
	 */
	virtual int SetArmPos(bool left, double pos, double max_speed, double max_acc);

	/**
	 * Magnes polaritasanak valtoztatasa
	 * @param left bal/jobb magnes
	 * @param polarity 1: vonzas, 0: kikapcsolt, -1: taszitas
	 */
	virtual int Magnet(bool left, int polarity);

protected:
	/**
	 * konfiguraciot tartalmazo objektum
	 */
	Config* mConfig;

	/**
	 * az utolso Wait fuggveny lefutasanak idejet tarolja
	 */
	struct timeval lastWait;

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
};

#endif /* PRIMITIVES_H_ */
