#!/bin/bash

source /usr/share/InnoRoute/tn_env.sh
source /usr/share/InnoRoute/tn_func_ll.sh

if [[ $# == 0 ]]; then
  echo "$0 <beep> is used to beep once"
  echo "Parameter <beep> can have any value"
  echo "If <beep> is given, the SystemController is unmuted and a single beep is issued."
else

  if [ -z "$(lspci -mm -d 10ee:0000)" ]; then echo "No PCIe connection to FPGA. Exiting."; exit 1; fi

  echo "Unmuting"
  i2cset -y 0 0x04 0x03 0
  echo "Beeping ..."
  tn_ll_mmi_write $C_BASE_ADDR_PERIPH $C_SUB_ADDR_PERIPH_BEEP 1

  echo "Done"
fi
