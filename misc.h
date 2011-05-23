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

#define		AREA_WIDTH	2100	// palya szelessege [mm]
#define		AREA_LENGTH	3000	// palya hosszusaga [mm]

#define		RED_OFFSET			0				// poziciok eltolasa
#define		BLUE_OFFSET			AREA_LENGTH		// poziciok eltolasa
#define		RED_ORIENTATION		1				// Y koordinatak iranya
#define		BLUE_ORIENTATION	-1				// koordinatak iranya

#define		ROBOT_RADIUS		175		// a robot fordulasahoz szugseges sugar [mm]
#define		ROBOT_WIDTH			140		// a robot szelessege / 2 [mm]
#define		ROBOT_FRONT			140		// a robot elejenek tavolsaga a kozeppontbol [mm]
#define		ROBOT_FRONT_MAX		240		// a robot elejenek tavolsaga nyitott megfogoval [mm]
#define		ROBOT_FRONT_PAWN	170		// paraszt felszedeshez a paraszt kozepetol valo tavolsag [mm]
#define		ROBOT_BACK			160		// a robot hatuljanak tavolsaga a kozeppontbol [mm]

/////////////////
// Robot szine

#define	RED		0
#define	BLUE	1

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
#define MAX_DEC 3000 // maximalis lassulas merteke [mm/s^2]

#endif /*MISC_H_*/
