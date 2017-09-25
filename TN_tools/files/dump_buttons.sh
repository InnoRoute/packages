#!/bin/bash

source /usr/share/InnoRoute/tn_env.sh

echo "Front button and user event rear button state"
# Get button status: bits0-15=FrontPanel, bit16=UserEvent

printf "Buttons: 0x%08x\n" `TNbar1 $TN_BASE_ADDR_PERIPHERIALS | cut -d ":" -f 2`