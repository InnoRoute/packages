#!/bin/bash

source /usr/share/InnoRoute/tn_env.sh
source /usr/share/InnoRoute/tn_func_ll.sh

if [[ $# == 0 ]]; then
  echo "$0 <string> [<string>] is used to set the system time"
  echo "If just one parameter is given, its value is ignored and"
  echo "a prompt to enter the time is shown."
  echo "If both values are given, it is assumed that they comply to"
  echo "the format Year-Month-Day Hour:Minute:Second"
  echo "Example: $0 2017-05-17 13:45:67"
elif [[ $# == 2 ]]; then
  echo "Setting time to '$1'"
  date -s "$1"
  hwclock -w

  sleep 1
  date
  sleep 1
  date
else
  echo "Please enter the current date and time in the following format:"
  echo "Year-Month-Day Hour:Minute:Second"
  echo "Example:"
  echo "2017-05-17 13:45:67"
  read time
  echo "Setting time to '$time'"
  date -s "$time"
  hwclock -w

  sleep 1
  date
  sleep 1
  date
fi
