#!/bin/bash

source /usr/share/InnoRoute/tn_env.sh

echo "Setting internal and Alaska LEDs, expecting none or 1 parameter"

if [[ $# == 0 ]]; then
  # Set all leds to high, if no parameter is given
  let leds=0xFFFFFFFF
else
  # If a parameter is given, it is assumed that this is the leds value
  let leds=$1
fi
printf "Setting LEDs to 0x%04x\n" $leds

# Set LEDs (all active-high): bits0-7=UserLEDs, bits8-9=AlaskaUserLed(right)
TNbar1 $(($C_BASE_ADDR_PERIPH*256+0x08)) w $leds
