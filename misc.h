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
//TODO: update
#define		AREA_WIDTH	2000	// palya szelessege [mm]
#define		AREA_LENGTH	3000	// palya hosszusaga [mm]

#define		BUTTON_LENGTH	48	// palackposta gombjanak belogo resze

#define		BLUE_OFFSET			0				// poziciok eltolasa
#define		RED_OFFSET			AREA_LENGTH		// poziciok eltolasa
#define		BLUE_ORIENTATION	1				// Y koordinatak iranya
#define		RED_ORIENTATION		-1				// koordinatak iranya

#define		ROBOT_RADIUS		175		// a robot fordulasahoz szugseges sugar [mm]
#define		ROBOT_WIDTH			165		// a robot szelessege / 2 [mm]
#define		ROBOT_FRONT			155		// a robot elejenek tavolsaga a kozeppontbol [mm]
#define		ROBOT_FRONT_MAX		335		// a robot elejenek tavolsaga nyitott megfogoval [mm]
#define		ROBOT_BACK			135		// a robot hatuljanak tavolsaga a kozeppontbol [mm]

#define		SONAR_TIMEOUT		1000	// sonartol kapott ertek ervenyessegi limitje [ms]

#define		PRIMITIVES_WAIT		50000	// alapertelmezett Primitives->Wait ido [us]

#define		OPPONENT_NUM		2		// nyilvantartott ellenfelek szama
#define		PROXIMITY_NUM		4		// tavolsagerzekelok szama

#define		COLLISION_ALL		0		// utkozes barmivel
#define		COLLISION_OBSTACLES	1		// utkozes akadalyokkal
#define		COLLISION_HIGH		2		// utkozes magas akadalyokkal
/////////////////
// Robot szine

#define	BLUE	-1
#define	RED		1

// Labda szinek
#define BALL_WHITE -1
#define BALL_RED 1
#define BALL_NOT_PRESENT 0

////////////////
// Ertelmezest segito valtozok, beavatkozokhoz

//bool-ok vajon mukodnek lua-ban?
#define FRONT true
#define REAR false

#define LEFT true
#define RIGHT false

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

#define SONAR_TX_RED		2098.0
#define SONAR_TY_RED		3098.0
#define SONAR_ALPHA_RED		M_PI

#define SONAR_TX_BLUE		-98.0
#define SONAR_TY_BLUE		-98.0
#define SONAR_ALPHA_BLUE	0.0

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
