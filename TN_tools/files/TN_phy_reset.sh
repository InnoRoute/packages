#!/bin/bash

source /usr/share/InnoRoute/tn_env.sh
source /usr/share/InnoRoute/tn_func_ll.sh

if [[ $# == 0 ]]; then
  echo "$0 <ex> is used to hard reset all PHYs, thereby loosing their configuration"
  echo "The parameter <ex> can have any value. Without it, this help is displayed"
else
  echo "Hard-resetting the Ethernet PHYs"
  
  # Bit 0 for GPHYs, bit 1 for Alaskas -> combined value: 3
  i2cset -y 0 4 0x19 3
  sleep 1
  i2cset -y 0 4 0x19 0

  echo "Done"
fi
