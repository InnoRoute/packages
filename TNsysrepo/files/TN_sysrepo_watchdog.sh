#!/bin/bash

while true;
do
	case "$(pidof netopeer2-server | wc -w)" in

	0)  echo "Restarting sysrepo:     $(date)"

		  killall -9 sysrepo-plugind
		  killall -9 sysrepod
		  killall -9 TNsysrepo
		  sleep 10
			sysrepo-plugind &
			sleep 10
		  sysrepod  &
			sleep 10
			netopeer2-server -d &
			sleep 10
			TNsysrepo start &
		  ;;
	1)  # all ok
		  ;;
	*)  
		  ;;
	esac
	sleep 20
done
