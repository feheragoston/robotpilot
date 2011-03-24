/*
 * robotpilot.cpp
 *
 *  Created on: Feb 16, 2011
 *      Author: agoston
 */

#include <iostream>
#include "Config.h"
#include "Control.h"

using namespace std;

int main(int argc, char *argv[]) {
	cout << "RobotPilot" << endl;

	Config* mConfig = new Config();
	if (mConfig->Parse(argc, argv)) {
		Control* mControl = new Control(mConfig);
		if (!mControl->Init()) {
			cerr << "Hiba a control inicializalasakor" << endl;
			return 1;
		}
		mControl->Run();

		delete mControl;
	}
	delete mConfig;

	return 0;
}
