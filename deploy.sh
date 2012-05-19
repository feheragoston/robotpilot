#!/bin/bash

if grep "c\." Pilot/pilot.lua ; then
	echo "c. hivas pilotban!!!";
	exit 1
fi

scp Debug32bit/robotpilot robot@geode:
scp Pilot/pilot.lua robot@geode:Pilot
scp Pilot/calibration.lua robot@geode:Pilot
scp Pilot/functions.lua robot@geode:Pilot
scp Pilot/s3.lua robot@geode:Pilot
if [ -f "$1" ] ; then
	scp $1 robot@geode:$1
fi
