# Original file in repo/hg_sw/TrustNode/EmbeddedLinux/BuildRootPKGs/InnoRoute/TN_firstboot/files/
# The E at the end of the PLL configuration writes to EEPROM
# Make sure that /usr/share/InnoRoute/TN-PLL.stp exists!

#!/bin/bash
echo "Setting up Trustnode"
INRpllload /usr/share/InnoRoute/TN-PLL.stp E
sensors-detect --auto
INR_ftdi_eeprom -p 0x6010 -v 0x0403 -M
echo "Setup on firstboot done"
echo "Have fun with your TrustNode"
