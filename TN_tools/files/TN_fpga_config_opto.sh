#!/bin/bash

source /usr/share/InnoRoute/tn_env.sh
source /usr/share/InnoRoute/tn_func_ll.sh

if [[ $# == 0 ]]; then
  echo "$0 <bitstream.bin> is used to load a bitstream to the FPGA using OPTO mode"
  echo "Beware: this resets all configuration values in the FPGA"
  echo "Parameter <bitstream.bin> is the name (and path) of the bitstream to be loaded"
else
  # If a parameter is given, it is assumed that this is the bitstream name
  export bitstream=$1
  
  echo "Sending bitstream '$bitstream' via OPTO/Fast Serial"
  
  # Set address of the LPC slave (0x84)
  # echo 132 > /proc/TN_LPC_addr
  
  # Initialize bitstream programming mode (via LPC)
  # echo 3 > /proc/TN_LPC_ctl
  
  # Initialize bitstream programming mode (via SMBus)
  i2cset -y 0 4 2 1
  
  # Wait for SMBus accesses to be executed (might be skipped)
  let wait=`i2cget -y 0 4 2`
  
  # Linux image version 0.6+ (before it was ftdi_opto_tool):
  # Send bitstream via FT2232H OPTO/Fast Serial mode (bin file -> synthesized with CONFIG_MODE=S_SERIAL and unset BITSTREAM.CONFIG.SPI_BUSWIDTH)
  INR_ftdi_opto_tool $bitstream
  
  # Exit bitstream programming mode (via LPC)
  # echo 4 > /proc/TN_LPC_ctl
  
  # Exit bitstream programming mode (via SMBus)
  i2cset -y 0 4 2 2
  
  # Wait for SMBus accesses to be executed (might be skipped)
  let wait=`i2cget -y 0 4 2`
  
  # Reset SMBus FSM (might be skipped)
  i2cset -y 0 4 2 0
  
  echo "Check programming results:"
  let biterr=`i2cget -y 0 4 2`
  if [[ $(( $biterr & 140 )) -eq 0 ]]; then
    echo "-> No error";
  fi
  if [[ $(( $biterr & 128 )) -eq 128 ]]; then
    echo "-> Pinout ID warning";
  fi
  if [[ $(( $biterr & 3 )) -eq 1 ]]; then
    echo "-> Error configuring from Flash";
  fi
  if [[ $(( $biterr & 3 )) -eq 2 ]]; then
    echo "-> Error configuring via LPC";
  fi
  if [[ $(( $biterr & 3 )) -eq 3 ]]; then
    echo "-> Error configuring via OPTO";
  fi
  if [[ $(( $biterr & 4 )) -gt 0 ]]; then
    echo "-> Cause: INIT_B went low";
  fi
  if [[ $(( $biterr & 8 )) -gt 0 ]]; then
    echo "-> Cause: DONE timeout";
  fi
  echo "All FPGA-internal state/tables cleared"

  echo "Done"
fi
