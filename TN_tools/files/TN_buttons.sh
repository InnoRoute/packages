#!/bin/bash

source /usr/share/InnoRoute/tn_env.sh

echo "Front button and user event rear button state"
# Get button status: bits0-15=FrontPanel, bit16=UserEvent

let buttons=`TNbar1 $(($C_BASE_ADDR_PERIPH*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_PERIPH_BUTTONS)) | cut -d " " -f 6`
if [[ $(( $buttons & 0x10000 )) -gt 0 ]]; then
  printf "Front Buttons: 0x%04x, User Event Button: 1\n" $(( $buttons & 0xFFFF ))
else
  printf "Front Buttons: 0x%04x, User Event Button: 0\n" $(( $buttons & 0xFFFF ))
fi
if [[ $buttons -eq 0xEEEEEEEE ]]; then
  echo " ** MMI Read Timeout **"
fi
