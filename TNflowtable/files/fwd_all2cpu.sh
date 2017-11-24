#!/bin/bash
#configures flowcache to forward all packages to CPU, overrides the hardware-ethernetswitch.
#M.Ulbricht InnoRoute 2017 ulbricht@innoroute.de
if [[ $# == 0 ]]; then
	echo "please provide 0/1 for enable or disable"
	exit 0
else
	if [[ $1 == 0 ]]; then
	echo "disabling CPU forward..."
	TNflowtable del -I0 -O16
	TNflowtable del -I1 -O17
	TNflowtable del -I2 -O18
	TNflowtable del -I3 -O19
	TNflowtable del -I4 -O20
	TNflowtable del -I5 -O21
	TNflowtable del -I6 -O22
	TNflowtable del -I7 -O23
	TNflowtable del -I8 -O24
	TNflowtable del -I9 -O25
	else
	echo "enabling CPU forward..."
	TNflowtable add -I0 -O16
	TNflowtable add -I1 -O17
	TNflowtable add -I2 -O18
	TNflowtable add -I3 -O19
	TNflowtable add -I4 -O20
	TNflowtable add -I5 -O21
	TNflowtable add -I6 -O22
	TNflowtable add -I7 -O23
	TNflowtable add -I8 -O24
	TNflowtable add -I9 -O25
	fi

echo "done"
fi

