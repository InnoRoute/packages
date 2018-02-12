#!/bin/bash

echo "Configuring Artix, expecting none or one parameter"

if [[ $# == 0 ]]; then
  # Set default bitstream name, if no parameter is given
  export bitstream=tn_atom_ctrl.bit
else
  # If a parameter is given, it is assumed that this is the bitstream name
  export bitstream=$1
fi
echo "Sending bitstream '$bitstream' via JTAG"
echo "The message 'No JTAG CHain found' means that a JTAG cable is currently plugged."

# JTAG programming (any bitstream)
# If access fails, either the FT2232H EEPROM was overwritten or a JTAG cable is connected

xc3sprog -c ftdi -J 0 -p0 $bitstream

# for programming the flash add: -I<ISF file>
