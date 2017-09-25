#!/bin/bash

source /usr/share/InnoRoute/tn_env.sh

echo "Beeping once"

TNbar1 $(($TN_BASE_ADDR_PERIPHERIALS+0x20)) w 1
