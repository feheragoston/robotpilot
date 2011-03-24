/*
 * Primitives.cpp
 *
 *  Created on: Mar 24, 2011
 *      Author: agoston
 */

#include "Primitives.h"

Primitives::Primitives(Config* config) {
	mConfig = config;
}

Primitives::~Primitives() {

}

bool Primitives::Init() {
	gettimeofday(&lastWait, NULL);

	return true;
}

bool Primitives::Wait() {
	struct timeval currentTime;
	struct timeval timeDiff;
	gettimeofday(&currentTime, NULL);

	tools::timeval_subtract(&timeDiff, &currentTime, &lastWait);

	if (timeDiff.tv_sec > 0) {
		std::cerr << "Eltelt ido tul sok: " << timeDiff.tv_sec << std::endl;
		gettimeofday(&lastWait, NULL);
		return true;
	} else if (timeDiff.tv_usec > 50000) {
		gettimeofday(&lastWait, NULL);
		return true;
	}
	//std::cout << "Primitives::Wait " << 50000 - timeDiff.tv_usec << std::endl;
	usleep(50000 - timeDiff.tv_usec);
	gettimeofday(&lastWait, NULL);
	return true;
}
