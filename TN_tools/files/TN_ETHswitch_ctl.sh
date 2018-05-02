#!/bin/bash

source /usr/share/InnoRoute/tn_env.sh

echo "Setting internal Ethernet switch common config, expecting none or 1 parameter"

if [[ $# != 1 ]]; then
  # Set all leds to high, if no parameter is given
  echo "HW switch 0:"
  TNbar1 $(($C_BASE_ADDR_ETH_SW_0*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_ETH_SW_COMMON)) w
  echo "HW switch 1:"
  TNbar1 $(($C_BASE_ADDR_ETH_SW_1*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_ETH_SW_COMMON)) w
else
  TNbar1 $(($C_BASE_ADDR_ETH_SW_0*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_ETH_SW_COMMON)) w $1 > /dev/null
  TNbar1 $(($C_BASE_ADDR_ETH_SW_1*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_ETH_SW_COMMON)) w $1 > /dev/null
fi



