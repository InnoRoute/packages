#!/bin/bash

source /usr/share/InnoRoute/tn_env.sh

echo "Reading system status from Artix: PHY interrupts etc."

# Get TrustNode status from Artix:
# bits0-9=GPHY.MDINT
# bits10-11=Alaska.PTPINT
# bits16-17=AlaskaPtpEventTrigger

# bit20=crm_locked

# bit21=sodimm_calib_complete
# bits24-25=sfp_present,
# bits26-27=sfp_rx_los
# bits28-29=sfp_tx_fault

TNbar1 $(($TN_BASE_ADDR_PERIPHERIALS+4))
