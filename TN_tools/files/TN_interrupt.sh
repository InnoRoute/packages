#!/bin/bash



source /usr/share/InnoRoute/tn_env.sh
echo "Read PCI-core interrupt status register"
  let read_data=`TNbar1 $(($C_BASE_ADDR_MDIO*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_MDIO_INTERRUPT)) | cut -d " " -f 6`
  if [[ $read_data -eq 0xEEEEEEEE ]]; then
    echo " ** MMI Read Timeout **"
  fi
printf "0x%x\n" $read_data



