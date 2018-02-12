#!/bin/bash



source /usr/share/InnoRoute/tn_env.sh

if [[ $# == 0 ]]; then
echo "Read PCI-core interrupt mask register"
  let read_data=`TNbar1 $(($C_BASE_ADDR_MDIO*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_MDIO_INTERRUPT_MASK)) | cut -d " " -f 6`
  if [[ $read_data -eq 0xEEEEEEEE ]]; then
    echo " ** MMI Read Timeout **"
  fi
printf "0x%x\n" $read_data
else
  let mask_val=$1
  echo "Write $(printf '0x%x' $mask_val) to PCI-core interrupt mask register"
  TNbar1 $(($C_BASE_ADDR_MDIO*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_MDIO_INTERRUPT_MASK))  w $mask_val
fi




