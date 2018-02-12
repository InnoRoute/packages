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
printf "Setting LEDs:"
if [[ $(( $leds & 1 )) -eq 1 ]]; then
  echo "-> Internal LED0 on";
else
  echo "-> Internal LED0 off";
fi
if [[ $(( $leds & 2 )) -eq 2 ]]; then
  echo "-> Internal LED1 on";
else
  echo "-> Internal LED1 off";
fi
if [[ $(( $leds & 4 )) -eq 4 ]]; then
  echo "-> Internal LED2 on";
else
  echo "-> Internal LED2 off";
fi
if [[ $(( $leds & 8 )) -eq 8 ]]; then
  echo "-> Internal LED3 on";
else
  echo "-> Internal LED3 off";
fi
if [[ $(( $leds & 16 )) -eq 16 ]]; then
  echo "-> Internal LED4 on";
else
  echo "-> Internal LED4 off";
fi
if [[ $(( $leds & 32 )) -eq 32 ]]; then
  echo "-> Internal LED5 on";
else
  echo "-> Internal LED5 off";
fi
if [[ $(( $leds & 64 )) -eq 64 ]]; then
  echo "-> Internal LED6 on";
else
  echo "-> Internal LED6 off";
fi
if [[ $(( $leds & 128 )) -eq 128 ]]; then
  echo "-> Internal LED7 on";
else
  echo "-> Internal LED7 off";
fi

if [[ $(( $leds & 1024 )) -eq 1024 ]]; then
  echo "-> Blinking SyncE LEDs (processor message)";
else
  if [[ $(( $leds & 256 )) -eq 256 ]]; then
    echo "-> SyncE A on";
  else
    echo "-> SyncE A off";
  fi
  if [[ $(( $leds & 512 )) -eq 512 ]]; then
    echo "-> SyncE B on";
  else
    echo "-> SyncE B off";
  fi
fi

# Set LEDs (all active-high): bits0-7=UserLEDs, bits8-9=AlaskaUserLed(right)
TNbar1 $(($C_BASE_ADDR_PERIPH*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_PERIPH_LEDS)) w $leds > /dev/null
