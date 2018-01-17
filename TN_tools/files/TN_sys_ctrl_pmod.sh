#!/bin/bash

echo "Setting System Controller PMod connection"
echo "(0=Clock Output, 1=FPGA UART, 2=Second Atom UART, others=High-Z)"

if [[ $# == 0 ]]; then
  echo "No parameter given: switching to Second Atom UART"
  let interface=2
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
    echo "Invalid input: switching to Second Atom UART"
    let interface=2
  fi
fi

# System Controller is at address 0x04
i2cset -y 0 0x04 0x1c $interface
