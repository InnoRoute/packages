#!/bin/bash

while true;
do
	case "$(pidof netopeer2-server | wc -w)" in

	0)  echo "Restarting sysrepo:     $(date)"

		  killall -9 sysrepo-plugind
		  killall -9 sysrepod
		  killall -9 TNsysrepo
		  #killall -9 ptp4l
		  sleep 10
			#ptp4l -q -f /usr/share/InnoRoute/ptp.conf  -i TN0 -i TN1 -i TN2 -i TN3 -i TN4 -i TN5 -i TN6 -i TN7 -i TN8 -i TN9 -i TN10 -i TN11 -i TN12 -i TN13 -i TN14 -i TN15 &
		  sleep 10
			sysrepo-plugind &
			sleep 10
		  sysrepod  &
			sleep 10
			netopeer2-server -d &
			sleep 20
			TNsysrepo start &
			sleep 10 
			TN_config.sh load
		  ;;
	1)  # all ok
		  ;;
	*)  
		  ;;
	esac
	sleep 5
done
