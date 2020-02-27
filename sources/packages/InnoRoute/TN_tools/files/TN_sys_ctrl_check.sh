#!/bin/bash

source /usr/share/InnoRoute/tn_env.sh
source /usr/share/InnoRoute/tn_func_ll.sh

# System Controller has SMBus address 0x04

let firmware=`i2cget -y 0 4 0x00`
if [[ "$firmware" -lt "15" ]]; then
  echo "Your system controller version $firmware is outdated, please contact InnoRoute for an update."
fi
