#!/bin/bash

source /usr/share/InnoRoute/tn_env.sh

echo "Setting internal pin header values, expecting none or one parameter"

if [[ $# == 0 ]]; then
  # Set all pins to high, if no parameter is given
  let gpio=0xFFFFFFFF
else
  # If a parameter is given, it is assumed that this is the GPIO output value
  let gpio=$1
fi
printf "Setting GPIO headers to drive 0x%04x\n" $gpio

# Sets the output values of the GPIO headers: bis0-9=UserGPIO, bits10-13=Artix-PMod
TNbar1 $(($C_BASE_ADDR_PERIPH*256+0x0C)) w $gpio
