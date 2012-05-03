//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//
//	  misc.h
//	----------
//	
//	Az "egyeb" kategoriaba sorolhato definiciok
//	
//////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////// 

#ifndef MISC_H_
#define MISC_H_


///////////////////////
// Palya adatai (mm)

#define		AREA_WIDTH	2000	// palya szelessege [mm]
#define		AREA_LENGTH	3000	// palya hosszusaga [mm]

#define		BUTTON_LENGTH	44	// palackposta gombjanak belogo resze

#define		PURPLE_OFFSET		0				// poziciok eltolasa
#define		RED_OFFSET			AREA_LENGTH		// poziciok eltolasa
#define		PURPLE_ORIENTATION	1				// Y koordinatak iranya
#define		RED_ORIENTATION		-1				// koordinatak iranya

#define		ROBOT_RADIUS		175		// a robot fordulasahoz szugseges sugar [mm]
#define		ROBOT_WIDTH			165		// a robot szelessege / 2 [mm]
#define		ROBOT_FRONT			155		// a robot elejenek tavolsaga a kozeppontbol [mm]
#define		ROBOT_FRONT_MAX		335		// a robot elejenek tavolsaga nyitott megfogoval [mm]
#define		ROBOT_BACK			135		// a robot hatuljanak tavolsaga a kozeppontbol [mm]

#define		SONAR_TIMEOUT		1000	// sonartol kapott ertek ervenyessegi limitje [ms]

#define		PRIMITIVES_WAIT		50000	// alapertelmezett Primitives->Wait ido [us]

#define		OPPONENT_NUM		2		// nyilvantartott ellenfelek szama
#define		PROXIMITY_NUM		2		// tavolsagerzekelok szama
/////////////////
// Robot szine

#define	PURPLE	-1
#define	RED		1

/////////////////////////
// Robot kezdopozicioja

#define	FASTCALIB_STARTPOS_X		174.0	// mm
#define	FASTCALIB_STARTPOS_Y		178.0	// mm
#define	FASTCALIB_STARTPOS_PHI		43.0 	// deg

#define	CALIB_STARTPOS_X		133.0	// mm
#define	CALIB_STARTPOS_Y		133.0	// mm
#define	CALIB_STARTPOS_PHI		180.0 	// deg

///////////////////////////////////////////////////
// Szonar koordinatarendszer transzformacios adatai

#define SONAR_TX_BLUE		2202.0
#define SONAR_TY_BLUE		3102.0
#define SONAR_ALPHA_BLUE	M_PI

#define SONAR_TX_RED		-102.0
#define SONAR_TY_RED		-102.0
#define SONAR_ALPHA_RED		0.0

/////////////////////////////////////////////////////
// Szonar meresi pont a robot koordinatarendszereben

#define SONAR_X_ROBOT	0.0
#define SONAR_Y_ROBOT	0.0


////////////////////
// Haladas iranya
#define FORWARDS	1
#define	BACKWARDS	-1 

////////////////////////////
// EPSILON
#define EPSILON 10e-5
#define MAX_DISTANCE 1000000
#define MAX_DEC 2000 // maximalis lassulas merteke [mm/s^2]

#endif /*MISC_H_*/
