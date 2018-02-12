#!/bin/bash

source /usr/share/InnoRoute/tn_env.sh

echo "(Unmuting and) beeping once"

i2cset -y 0 0x04 0x03 0
TNbar1 $(($C_BASE_ADDR_PERIPH*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_PERIPH_BEEP)) w 1 > /dev/null
