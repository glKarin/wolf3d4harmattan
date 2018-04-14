#!/bin/sh

DATADIR=/home/user/MyDocs/wolf3d
BINDIR=/home/user/developer/wolf3d-1.6
SLEEPTIME=30

if [ -d $DATADIR ]; then
	cd $DATADIR
	$BINDIR/keyon
	if [ x$1 = xt ]; then
		$BINDIR/wolf3d &
		sleep $SLEEPTIME
		killall wolf3d
		$BINDIR/keyoff
	else
		$BINDIR/wolf3d
		$BINDIR/keyoff
	fi
fi

