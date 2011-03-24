/*
 * Control.h
 *
 *  Created on: Mar 24, 2011
 *      Author: agoston
 */

#ifndef CONTROL_H_
#define CONTROL_H_

#include <sys/time.h>
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

	struct timeval runStart; // Run indulasanak ideje
	struct timeval initStart; // Init indulasanak ideje
	struct timeval matchStart; // Start gomb megnyomasanak ideje
	bool matchStarted; // Meccs elkezdodott

	bool Operate(); // ez meghivodik minden lefutasra
};

#endif /* CONTROL_H_ */
