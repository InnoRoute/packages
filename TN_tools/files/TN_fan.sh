#!/bin/bash

echo "Setting the top fan duty cycle, expecting none or one parameter"

if [[ $# == 0 ]]; then
  # If no parameter is given, execute this example:
  echo "Playing a little with the fan speed, from slow to fast and back to slow (full range)"
  for i in `seq 20    63`; i2cset -y 0 4 5 $i; sleep 1; done
  for i in `seq 63 -1 20`; i2cset -y 0 4 5 $i; sleep 1; done
else
  # If a parameter is given, take this as the speed value
  echo "Setting fan speed to $1"
  i2cset -y 0 4 5 $1
fi
