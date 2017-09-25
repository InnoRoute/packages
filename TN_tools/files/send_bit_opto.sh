#!/bin/bash

echo "Configuring Artix, expecting none or one parameter"

if [[ $# == 0 ]]; then
  # Set default bitstream name, if no parameter is given
  export bitstream=tn_toplevel.bin
else
  # If a parameter is given, it is assumed that this is the bitstream name
  export bitstream=$1
fi
echo "Sending bitstream '$bitstream' via OPTO/Fast Serial"

# Set address of the LPC slave (0x84)
# echo 132 > /proc/TN_LPC_addr

# Initialize bitstream programming mode (via LPC)
# echo 3 > /proc/TN_LPC_ctl

# Initialize bitstream programming mode (via SMBus)
i2cset -y 0 4 2 1

# Wait for SMBus accesses to be executed (might be skipped)
i2cget -y 0 4 2

# Linux image version 0.6+ (before it was ftdi_opto_tool):
# Send bitstream via FT2232H OPTO/Fast Serial mode (bin file -> synthesized with CONFIG_MODE=S_SERIAL and unset BITSTREAM.CONFIG.SPI_BUSWIDTH)
time INR_ftdi_opto_tool $bitstream

# Exit bitstream programming mode (via LPC)
# echo 4 > /proc/TN_LPC_ctl

# Exit bitstream programming mode (via SMBus)
i2cset -y 0 4 2 2

# Wait for SMBus accesses to be executed (might be skipped)
i2cget -y 0 4 2

# Reset SMBus FSM (might be skipped)
i2cset -y 0 4 2 0

echo "Updates of the PCIe core require a Linux restart, not a power down"
