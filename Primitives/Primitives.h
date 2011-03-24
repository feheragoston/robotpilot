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

	virtual bool Init();
	// inicializalja a primitives eszkozt
	// visszateresi ertek false ha nem sikerult

	virtual bool Wait();
	// varakozas a kovetkezo megszakitasig
	// visszateresi ertek false ha hiba tortent, ekkor kilepunk

protected:
	Config* mConfig;

	struct timeval lastWait;
};

#endif /* PRIMITIVES_H_ */
