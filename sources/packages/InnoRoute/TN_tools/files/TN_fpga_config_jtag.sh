#!/bin/bash

source /usr/share/InnoRoute/tn_env.sh
source /usr/share/InnoRoute/tn_func_ll.sh

if [[ $# == 0 ]]; then
  echo "$0 <bitstream.bit> is used to load a bitstream to the FPGA using JTAG"
  echo "Beware: this resets all configuration values in the FPGA"
  echo "Parameter <bitstream.bit> is the name (and path) of the bitstream to be loaded"
else
  export bitstream=$1
  
  echo "Sending bitstream '$bitstream' via JTAG"
  echo "The message 'No JTAG chain found' means that a JTAG cable is currently plugged."
  
  # JTAG programming (any bitstream)
  # If access fails, either the FT2232H EEPROM was overwritten or a JTAG cable is connected
  
  xc3sprog -c ftdi -J 0 -p0 $bitstream
  
  # for programming the flash add: -I<ISF file>
  echo "All FPGA-internal state/tables cleared"

  echo "Done"
fi
