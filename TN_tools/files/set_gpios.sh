#!/bin/bash

source /usr/share/InnoRoute/tn_env.sh

echo "Setting internal pin header values, expecting none or one parameter"

if [[ $# == 0 ]]; then
  # Set all pins to high, if no parameter is given
  let gpio=0x0000FFFF
else
  # If a parameter is given, it is assumed that this is the GPIO output value
  let gpio=$1
fi
printf "Setting GPIO headers to mask/drive 0x%04x\n" $gpio

# Sets the output values of the GPIO headers:
#   lower bits are output data (0-9=Pinheader, 10-13=PMod) and the
#   upper bits are tristate flags (16-25=Pinheader, 26-29=PMod)
#   Reading external signals is only possible, if tristate is active,
#   but driving the output data is only possible, if tristate is inactive

let     gpio=TNbar1 $(($C_BASE_ADDR_PERIPH*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_PERIPH_GPIOS)) w $gpio
printf "Current GPIO PIN State: 0x%08x\n" $gpio
