#!/bin/bash

source /usr/share/InnoRoute/tn_env.sh

echo "ensuring FPGA routes default to CPU"

  TNbar1 $(($C_BASE_ADDR_COMMON_LOWER*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_FEATURES_FLOWCACHE)) w 0x10 > /dev/null




