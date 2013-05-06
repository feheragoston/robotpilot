/*
 * Primitives.h
 *
 *  Created on: Mar 24, 2011
 *      Author: agoston
 */

#ifndef PRIMITIVES_H_
#define PRIMITIVES_H_

#include <stdint.h>
#include <sys/time.h>
#include <math.h>
#include "../Config.h"
#include "../tools.h"
#include "../geom.h"
#include "define/param.h"


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
	 * @return -1: lila, 1: piros, 0: undefined
	 */
	virtual int8_t GetMyColor();

#ifdef NAGY_ROBOT
	/**
	 * A labda jelenletenek lekerdezese
	 * @return 1: labda, 0: ures
	 */
	virtual bool GetBallPresent();
#endif

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
	 * 2000  |           |-O---|                |
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
	 * GetRobotPos egesz szamokra
	 * @param x [mm]
	 * @param y [mm]
	 * @param phi [deg]
	 */
	void GetRobotPos(int* x, int* y, int* phi);

	/**
	 * ellenfel poziciojanak lekerdezese
	 * @param n az ellenfel robot szama (0..OPPONENT_NUM-1)
	 * @param x [mm]
	 * @param y [mm]
	 * @return utolso frissites ota eltelt ido [ms]
	 */
	virtual long int GetOpponentPos(unsigned char n, double * x, double* y);

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
	 * @param n az ellenfel robot szama (0..OPPONENT_NUM-1)
	 * @param x [mm]
	 * @param y [mm]
	 */
	virtual void SetOpponentPos(unsigned char n, double x, double y);

#ifdef NAGY_ROBOT
	/**
	 * Visszaadja a labdaerzekelo CNY70 feszultsegerteket
	 */
	virtual double GetBallColorVoltage(void);
#endif

	/**
	 * Visszaadja a tavolsagerzekelok erteket
	 * 0: jobb elso
	 * 1: bal elso
	 * 2: jobb hatso
	 * 3: bal hatso
	 * @param distance a PROXIMITY_NUM db tavolsagerzekelo erteke [mm]
	 */
	virtual void GetDistances(double distance[PROXIMITY_NUM]);

#ifdef NAGY_ROBOT
	///////////////////////////////////////////////////////////////////////
	// ARM
	///////////////////////////////////////////////////////////////////////

	/**
	 * Oldalso megfogo kar mozgatasa
	 * @param front elso/hatso kar mozgatasa
	 * @param pos abszolut pozicio [deg], 0 zart
	 * @param max_speed [deg/s]
	 * @param max_acc [deg/s^2]
	 * @return true: folyamat elindult, false: hiba tortent
	 */
	virtual bool ArmMove(bool front, double pos, double max_speed, double max_acc);

	/**
	 * Oldalso megfogo kar mozgatas allapota
	 * @param front elso/hatso kar allapota
	 * @return true: folyamatban van, false: nincs folyamatban
	 */
	virtual bool ArmMoveInProgress(bool front);

	/**
	 * Legutobbi oldalso megfogo mozgatas hibajanak lekerdezese
	 * @param front elso/hatso kar
	 * @return true: hiba volt, false: nem volt hiba
	 */
	virtual bool GetArmError(bool front);

	/**
	 * Oldalso megfogo kar szogenek lekerdezese
	 * @param front elso/hatso kar allapota
	 * @return [deg], zart helyzet 0
	 */
	virtual double GetArmPos(bool front);

	///////////////////////////////////////////////////////////////////////
	// FLIPPER
	///////////////////////////////////////////////////////////////////////

	/**
	 * Oldalso megfogo kar mozgatasa
	 * @param pos abszolut pozicio [deg], 0 zart
	 * @param max_speed [deg/s]
	 * @param max_acc [deg/s^2]
	 * @return true: folyamat elindult, false: hiba tortent
	 */
	virtual bool FlipperMove(double pos, double max_speed, double max_acc);

	/**
	 * Oldalso megfogo kar mozgatas allapota
	 * @return true: folyamatban van, false: nincs folyamatban
	 */
	virtual bool FlipperMoveInProgress();

	/**
	 * Legutobbi oldalso megfogo mozgatas hibajanak lekerdezese
	 * @return true: hiba volt, false: nem volt hiba
	 */
	virtual bool GetFlipperError();

	/**
	 * Oldalso megfogo kar szogenek lekerdezese
	 * @return [deg], zart helyzet 0
	 */
	virtual double GetFlipperPos();

	///////////////////////////////////////////////////////////////////////
	// CONTRACTOR
	///////////////////////////////////////////////////////////////////////

	/**
	 * Oldalso megfogo karokat egymashoz kozelito szervo mozgatasa
	 * @param pos abszolut pozicio [deg], 0 zart
	 * @param max_speed [deg/s]
	 * @param max_acc [deg/s^2]
	 * @return true: folyamat elindult, false: hiba tortent
	 */
	virtual bool ContractorMove(double pos, double max_speed, double max_acc);

	/**
	 * Oldalso megfogo karokat egymashoz kozelito szervo mozgatas allapota
	 * @return true: folyamatban van, false: nincs folyamatban
	 */
	virtual bool ContractorMoveInProgress();

	/**
	 * Legutobbi oldalso megfogo karokat egymashoz kozelito szervo mozgatas hibajanak lekerdezese
	 * @return true: hiba volt, false: nem volt hiba
	 */
	virtual bool GetContractorError();

	/**
	 * Oldalso megfogo karokat egymashoz kozelito szervo mozgatas szogenek lekerdezese
	 * @return [deg], zart helyzet 0
	 */
	virtual double GetContractorPos();

	///////////////////////////////////////////////////////////////////////
	// SELECTOR
	///////////////////////////////////////////////////////////////////////

	/**
	 * Labda-szelektalo szervo mozgatasa
	 * @param pos abszolut pozicio [deg], 0 zart
	 * @param max_speed [deg/s]
	 * @param max_acc [deg/s^2]
	 * @return true: folyamat elindult, false: hiba tortent
	 */
	virtual bool SelectorMove(double pos, double max_speed, double max_acc);

	/**
	 * Labda-szelektalo szervo mozgatasanak allapota
	 * @return true: folyamatban van, false: nincs folyamatban
	 */
	virtual bool SelectorMoveInProgress();

	/**
	 * Legutobbi labda-szelektalo szervo mozgatas hibajanak lekerdezese
	 * @return true: hiba volt, false: nem volt hiba
	 */
	virtual bool GetSelectorError();

	/**
	 * Labda-szelektalo szervo szogenek lekerdezese
	 * @return [deg], zart helyzet 0
	 */
	virtual double GetSelectorPos();

	///////////////////////////////////////////////////////////////////////
	// FIRESTOPPER
	///////////////////////////////////////////////////////////////////////

	/**
	 * Kilovest megallito pocok mozgatasa
	 * @param pos abszolut pozicio [deg], 0 alaphelyzet (fuggoleges), lefele no
	 * @param max_speed [deg/s]
	 * @param max_acc [deg/s^2]
	 * @return true: folyamat elindult, false: hiba tortent
	 */
	virtual bool FireStopperMove(double pos, double max_speed, double max_acc);

	/**
	 * Kilovest megallito pocok mozgatas allapota
	 * @return true: folyamatban van, false: nincs folyamatban
	 */
	virtual bool FireStopperMoveInProgress();

	/**
	 * Legutobbi kilovest megallito pocok mozgatas hibajanak lekerdezese
	 * @return true: hiba volt, false: nem volt hiba
	 */
	virtual bool GetFireStopperError();

	/**
	 * Kilovest megallito pocok poziciojanak lekerdezese
	 * @return abszolut pozicio [deg], 0 alaphelyzet (fuggoleges), lefele no
	 */
	virtual double GetFireStopperPos();

#else	//KIS_ROBOT

	///////////////////////////////////////////////////////////////////////
	// GRIPPER
	///////////////////////////////////////////////////////////////////////

	/**
	 * Elulso megfogo kar mozgatasa
	 * @param low also/felso kar mozgatasa
	 * @param left bal/jobb kar mozgatasa
	 * @param pos abszolut pozicio [deg], 0 zart
	 * @param max_speed [deg/s]
	 * @param max_acc [deg/s^2]
	 * @return true: folyamat elindult, false: hiba tortent
	 */
	virtual bool GripperMove(bool low, bool left, double pos, double max_speed, double max_acc);

	/**
	 * Elulso megfogo kar mozgatas allapota
	 * @param low also/felso kar allapota
	 * @param left bal/jobb kar allapota
	 * @return true: folyamatban van, false: nincs folyamatban
	 */
	virtual bool GripperMoveInProgress(bool low, bool left);

	/**
	 * Legutobbi elulso megfogo mozgatas hibajanak lekerdezese
	 * @param low also/felso kar
	 * @param left bal/jobb kar
	 * @return true: hiba volt, false: nem volt hiba
	 */
	virtual bool GetGripperError(bool low, bool left);

	/**
	 * Elulso megfogo kar szogenek lekerdezese
	 * @param low also/felso kar
	 * @param left bal/jobb kar
	 * @return [deg], zart helyzet 0
	 */
	virtual double GetGripperPos(bool low, bool left);
#endif

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
#ifdef KIS_ROBOT
	/**
	 * Vonalkovetes kovetes inditasa
	 * @param dist [mm]
	 * @return true: folyamat elindult, false: hiba tortent
	 */
	virtual bool FollowLine_Follow(double dist);

	/**
	 * Vonalkovetes kovetes allapotanak lekerdezese
	 * @return true: folyamatban van, false: nincs folyamatban
	 */
	virtual bool FollowLine_FollowInProgress();
	/**
	 * legutobbi kovetes leallasanak oka
	 * @return 0: nem allt le a vonalkovetes, 1: elerte a tavolsagot, 2: elagazasba utkozott, 3: egyeb hiba
	 */
	virtual int FollowLine_GetFollowError();
	/**
	 * Vonalkovetes fix threshold beallitas
	 * @return true: siker, false: hiba tortent
	 */
	virtual bool FollowLine_FixThreshold(uint16_t threshold);
	/**
	 * Vonalkovetes kalibralas
	 * @return true: folyamat elindult, false: hiba tortent
	 */
	virtual bool FollowLine_Calibrate();

	/**
	 * CalibrateFollowLine allapota
	 * @return true: folyamatban van, false: nincs folyamatban
	 */
	virtual bool FollowLine_CalibrateInProgress();
#else //NAGY_ROBOT
	///////////////////////////////////////////////////////////////////////
	// CARACOLE
	///////////////////////////////////////////////////////////////////////

	/**
	 * Csiga sebesseg-beallitasa
	 * @param speed [rad/s]
	 * @param max_acc [rad/s^2]
	 * @return true: folyamat elindult, false: hiba tortent
	 */
	virtual bool CaracoleSetSpeed(double speed, double max_acc);

	/**
	 * Csiga sebesseg-beallitas allapota
	 * @return true: folyamatban van, false: nincs folyamatban
	 */
	virtual bool CaracoleSetSpeedInProgress();

	/**
	 * Csiga sebesseg lekerdezese
	 * @return [rad]
	 */
	virtual double GetCaracoleSpeed();

	///////////////////////////////////////////////////////////////////////
	// FIREWHEEL
	///////////////////////////////////////////////////////////////////////

	/**
	 * Kilovo sebesseg-beallitasa
	 * @param speed [rad/s]
	 * @param max_acc [rad/s^2]
	 * @return true: folyamat elindult, false: hiba tortent
	 */
	virtual bool FirewheelSetSpeed(double speed, double max_acc);

	/**
	 * Kilovo sebesseg-beallitas allapota
	 * @return true: folyamatban van, false: nincs folyamatban
	 */
	virtual bool FirewheelSetSpeedInProgress();

	/**
	 * Kilovo sebesseg lekerdezese
	 * @return [rad]
	 */
	virtual double GetFirewheelSpeed();
#endif

protected:
	/**
	 * konfiguraciot tartalmazo objektum
	 */
	Config* mConfig;

	/**
	 * a szinkapcsolo allasat tarolja
	 */
	int8_t mRobotColor;
	/**
	 * a startgomb allapotat tarolja
	 */
	bool mStartButton;
	/**
	 * a stopgomb allapotat tarolja
	 */
	bool mStopButton;

	/**
	 * a robot es az ellenfel pozicioja
	 */
	position robot, opponent[OPPONENT_NUM];

	/**
	 * mozgasoknal a celpozicio
	 */
	position target;

	progress calibrateDeadreckoning;
	progress go;
	progress goTo;
	progress turn;
	progress motionStop;

#ifdef NAGY_ROBOT
	progress frontArmMove, backArmMove;
	double frontArmPos, rearArmPos;
	progress flipperMove;
	double flipperPos;
	progress contractorMove;
	double contractorPos;
	progress selectorMove;
	double selectorPos;
	progress caracoleSetSpeed;
	double caracoleSpeed;
	progress firewheelSetSpeed;
	double firewheelSpeed;
	progress fireStopperMove;
	double fireStopperPos;
#else	//KIS_ROBOT
	double leftLowGripperPos, rightLowGripperPos;
	double leftHighGripperPos, rightHighGripperPos;
	progress leftLowGripperMove, rightLowGripperMove;
	progress leftHighGripperMove, rightHighGripperMove;
#endif
	progress consoleMove;
	double consolePos;

};

#endif /* PRIMITIVES_H_ */
