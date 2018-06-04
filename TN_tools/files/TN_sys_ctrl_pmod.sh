#!/bin/bash

source /usr/share/InnoRoute/tn_env.sh
source /usr/share/InnoRoute/tn_func_ll.sh

if [[ $# == 0 ]]; then
  echo "$0 <con> is used to set the System Controller PMod connection"
  echo "Parameter <con> can be between 0 and 3"
  echo "  0:Clock Output"
  echo "  1:FPGA UART"
  echo "  2:Second Atom UART"
  echo "  3:High-Z"
else
  if [[ $1 == 0 ]]; then
    echo "Switching to Clock Output"
    let interface=0
  elif [[ $1 == 1 ]]; then
    echo "Switching to FPGA UART"
    let interface=1
  elif [[ $1 == 2 ]]; then
    echo "Switching to Atom UART1"
    let interface=2
  else
    echo "Switching to high-impedance mode"
    let interface=3
  fi
  
  # System Controller is at address 0x04
  i2cset -y 0 0x04 0x1c $interface

  echo "Done"
fi
