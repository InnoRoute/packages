#!/bin/bash

source /usr/share/InnoRoute/tn_env.sh

if [[ $# == 0 ]]; then
  let read_data=`TNbar1 $(($C_BASE_ADDR_MDIO*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_MDIO_INTERRUPT_EN)) | cut -d " " -f 6`

  if [[ $read_data -eq 0xEEEEEEEE ]]; then
    echo " ** MMI Read Timeout **"
  else
    printf "TN MDIO interrupt enable register: 0x%08x\n" $read_data
  fi
else
  let mask_val=$1

  echo "Writing $(printf '0x%08x' $mask_val) to TN MDIO interrupt enable register"
  TNbar1 $(($C_BASE_ADDR_MDIO*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_MDIO_INTERRUPT_EN)) w $mask_val
fi
