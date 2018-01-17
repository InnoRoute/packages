#!/bin/bash

echo "Sending via LPC is currently not supported"
exit

echo "Configuring Artix, expecting none or one parameter"

if [[ $# == 0 ]]; then
  # Set default bitstream name, if no parameter is given
  export bitstream=tn_toplevel.bin
else
  # If a parameter is given, it is assumed that this is the bitstream name
  export bitstream=$1
fi
echo "Sending bitstream '$bitstream' via LPC"

# Set address of the LPC slave (0x84)
echo 132 > /proc/TN_LPC_addr
# Initialize bitstream programming mode
echo 1 > /proc/TN_LPC_ctl
# Send bitstream via LPC (bin file -> synthesized with CONFIG_MODE=S_SERIAL and unset BITSTREAM.CONFIG.SPI_BUSWIDTH)
time dd if=$bitstream of=/proc/TN_LPC_data bs=1
# Exit bitstream programming mode
echo 2 > /proc/TN_LPC_ctl

echo "Updates of the PCIe core require a Linux restart, not a power down"
