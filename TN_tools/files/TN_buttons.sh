#!/bin/bash

source /usr/share/InnoRoute/tn_env.sh
source /usr/share/InnoRoute/tn_func_ll.sh

if [[ $# == 0 ]]; then
  echo "$0 <ex> is used to read the state of the buttons on the front panel and at the back"
  echo "The parameter <ex> can have any value. Without it, this help is displayed"
else
  # Get button status: bits0-15=FrontPanel, bit16=UserEvent
  
  tn_ll_mmi_read $C_BASE_ADDR_PERIPH $C_SUB_ADDR_PERIPH_BUTTONS
  if [[ $(( $read_data & 0x10000 )) -gt 0 ]]; then
    printf "Front Buttons: 0x%04x, User Event Button: 1\n" $(( $read_data & 0xFFFF ))
  else
    printf "Front Buttons: 0x%04x, User Event Button: 0\n" $(( $read_data & 0xFFFF ))
  fi
  if [[ $read_data -eq 0xEEEEEEEE ]]; then
    echo " ** MMI Read Timeout **"
  fi

  echo "Done"
fi
