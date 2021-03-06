#!/bin/bash

source /usr/share/InnoRoute/tn_env.sh
source /usr/share/InnoRoute/tn_func_ll.sh

if [[ $# == 0 ]]; then
  echo "$0 <phase0> .. <phase11> is used to set the RGMII data-clock phase shift"
  echo "Parameter <phase*> each can be between 0 and 31"
  echo "The default values are defined in the bitstream constraints"
  echo "In total 12 parameters are expected"
elif [[ $# == 12 ]]; then
  if [ -z "$(lspci -mm -d 10ee:0000)" ]; then echo "No PCIe connection to FPGA. Exiting."; exit 1; fi

  # write data range: 0-31
  for i in `seq 1 12`; do
    let phase=$1
    shift
    let write_data=$(( $(( $(($phase & 0x1F)) << 16)) | $((1 << $(($i-1)) )) ))
    echo "Setting phase value $phase for port $i"
    tn_ll_mmi_write $C_BASE_ADDR_NET_LOWER $C_SUB_ADDR_NET_TAP $write_data
  done
  echo "Done"
else
  echo "$0: wrong number of parameters ($#)"
fi
