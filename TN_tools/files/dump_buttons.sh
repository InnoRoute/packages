#!/bin/bash

source /usr/share/InnoRoute/tn_env.sh

echo "Front button and user event rear button state"
# Get button status: bits0-15=FrontPanel, bit16=UserEvent

printf "Buttons: 0x%08x\n" `TNbar1 $(($C_BASE_ADDR_PERIPH*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_PERIPH_BUTTONS)) | cut -d ":" -f 2`
