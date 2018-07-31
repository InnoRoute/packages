#!/bin/bash

source /usr/share/InnoRoute/tn_env.sh
source /usr/share/InnoRoute/tn_func_ll.sh

if [[ $# == 0 ]]; then
  echo "$0 <bitstream.bin> is used to load a bitstream to the FPGA using the LPC bus"
  echo "Beware: this resets all configuration values in the FPGA"
  echo "Parameter <bitstream.bin> is the name (and path) of the bitstream to be loaded"
else

  echo "$0: Configuration via LPC bus is currently not supported by the System Controller"
  exit

  # If a parameter is given, it is assumed that this is the bitstream name
  export bitstream=$1
  
  echo "Sending bitstream '$bitstream' via LPC"
  
  # Set address of the LPC slave (0x84)
  echo 132 > /proc/TN_LPC_addr
  # Initialize bitstream programming mode
  echo 1 > /proc/TN_LPC_ctl
  # Send bitstream via LPC (bin file -> synthesized with CONFIG_MODE=S_SERIAL and unset BITSTREAM.CONFIG.SPI_BUSWIDTH)
  dd if=$bitstream of=/proc/TN_LPC_data bs=1
  # Exit bitstream programming mode
  echo 2 > /proc/TN_LPC_ctl
  
  echo "Updates of the PCIe core require a Linux restart, not a power down"
  echo "All FPGA-internal state/tables cleared"

  echo "Done"
fi
