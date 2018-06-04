#!/bin/bash

source /usr/share/InnoRoute/tn_env.sh
source /usr/share/InnoRoute/tn_func_ll.sh

if [[ $# == 0 ]]; then
  echo "$0 <ex> [<mask>] is used to read or change the value of the MDIO interrupt mask"
  echo "The value <ex> can have any value. If it is omitted, this help is shown."
  echo "If the parameter <mask> is given, a write is performed, with <mask> as the"
  echo "new interupt mask, otherwise the current values is displayed"
elif [[ $# == 1 ]]; then
  tn_ll_mmi_read $C_BASE_ADDR_MDIO $C_SUB_ADDR_MDIO_INTERRUPT_EN

  if [[ $read_data -eq 0xEEEEEEEE ]]; then
    echo " ** MMI Read Timeout **"
  else
    printf "TN MDIO interrupt enable register: 0x%08x\n" $read_data
  fi

  echo "Done"
else
  let mask_val=$2

  echo "Writing $(printf '0x%08x' $mask_val) to TN MDIO interrupt enable register"
  tn_ll_mmi_write $C_BASE_ADDR_MDIO $C_SUB_ADDR_MDIO_INTERRUPT_EN $mask_val

  echo "Done"
fi
