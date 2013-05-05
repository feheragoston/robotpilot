#!/bin/bash

DATE=`date +%Y.%m.%d_%H-%M-%S`
LOGDIR=/home/workspace2/log

mkdir "$LOGDIR/$DATE"
scp robot@geode:/tmp/robotpilot.log "$LOGDIR/$DATE"
scp robot@geode:/tmp/control.log "$LOGDIR/$DATE"

