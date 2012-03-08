#!/bin/bash

DATE=`date +%Y.%m.%d_%H-%M-%S`
LOGDIR=/home/agoston/work/eurobot/log

mkdir "$LOGDIR/$DATE"
scp robot@geode:/tmp/robotpilot.log "$LOGDIR/$DATE"
scp robot@geode:/tmp/control.log "$LOGDIR/$DATE"

