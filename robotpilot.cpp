/*
 * robotpilot.cpp
 *
 *  Created on: Feb 16, 2011
 *      Author: agoston
 */

#include <iostream>
#include "Config.h"

using namespace std;

int main(int argc, char *argv[]) {
	cout << "RobotPilot" << endl;

	Config* mConfig = new Config();
	if (mConfig->Parse(argc, argv)) {

	}

	return 0;
}
