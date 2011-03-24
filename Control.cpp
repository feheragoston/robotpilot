/*
 * Control.cpp
 *
 *  Created on: Mar 24, 2011
 *      Author: agoston
 */

#include "Control.h"

Primitives* Control::mPrimitives = NULL;

Control::Control(Config* config) {
	mConfig = config;
}

Control::~Control() {
	if (mPrimitives) {
		delete mPrimitives;
	}
}

bool Control::Init() {
	gettimeofday(&runStart, NULL);
	gettimeofday(&initStart, NULL);
	gettimeofday(&matchStart, NULL);

	mPrimitives = new Primitives(mConfig);

	if (mPrimitives->Init()) {
		return true;
	}

	return false;
}

void Control::Run() {
	gettimeofday(&runStart, NULL);

	while (mPrimitives) {
		if (mPrimitives->Wait()) {
			if (!this->Operate())
				return;
		} else {
			return;
		}
	}
}

bool Control::Operate() {
	return false;
}
