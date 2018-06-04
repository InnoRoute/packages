#!/bin/bash

source /usr/share/InnoRoute/tn_env.sh
source /usr/share/InnoRoute/tn_func_ll.sh

if [[ $# == 0 ]]; then
  echo "$0 <phase0> .. <phase11> is used to set the RGMII data-clock phase shift"
  echo "Parameter <phase*> each can be between 0 and 31"
  echo "The default values are defined in the bitstream constraints"
  echo "In total 12 parameters are expected"
elif [[ $# == 12 ]]; then
  # write data range: 0-31
  for i in `seq 1 12`; do
    let write_data=(((${$i} & 0x1F) << 16) | (1 << ($i-1)))
    echo "Writing $write_data to TAP address: ${$i} to port $i"
    tn_ll_mmi_write $C_BASE_ADDR_NET_LOWER $C_SUB_ADDR_NET_TAP $write_data
  done
  echo "Done"
else
  echo "$0: wrong number of parameters ($#)"
fi
