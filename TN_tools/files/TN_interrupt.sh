#!/bin/bash

source /usr/share/InnoRoute/tn_env.sh
source /usr/share/InnoRoute/tn_func_ll.sh

if [[ $# == 0 ]]; then
  echo "$0 <ex> is used to read current value of the MDIO interrupt register,"
  echo "thereby clearing it"
  echo "The parameter <ex> can have any value. Without it, this help is displayed"
else
  tn_ll_mmi_read $C_BASE_ADDR_MDIO $C_SUB_ADDR_MDIO_INTERRUPT
  if [[ $read_data -eq 0xEEEEEEEE ]]; then
    echo " ** MMI Read Timeout **"
  else
    printf "TN MDIO interrupt register: 0x%08x\n" $read_data
  fi
  echo "Done"
fi
