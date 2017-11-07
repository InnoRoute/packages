#!/bin/bash

source /usr/share/InnoRoute/tn_env.sh

echo "Beeping once"

TNbar1 $(($C_BASE_ADDR_PERIPH*256+0x20)) w 1
