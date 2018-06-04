#!/bin/bash

source /usr/share/InnoRoute/tn_env.sh
source /usr/share/InnoRoute/tn_func_ll.sh

if [[ $# == 0 ]]; then
  echo "$0 <speed> is used to set the speed of the case fan"
  echo "Parameter <speed> can be between 1 and 63, or 100"
  echo "  1-63: Set the specified speed (default is 20)"
  echo "  100:  Cycle through all speeds, from slow to fast and back"
else
  echo "Setting the case fan duty cycle"

  if [[ $1 == 100 ]]; then
    # If no parameter is given, execute this example:
    echo "Playing a little with the fan speed, from slow to fast and back to slow (full range)"
    for i in `seq 1    63`; do i2cset -y 0 4 5 $i; sleep 0.1; done
    for i in `seq 63 -1 1`; do i2cset -y 0 4 5 $i; sleep 0.1; done
    echo "Returning to default setting (20)"
    i2cset -y 0 4 5 20
  else
    # If a parameter is given, take this as the speed value
    echo "Setting fan speed to $1"
    i2cset -y 0 4 5 $1
  fi

  echo "Done"
fi
