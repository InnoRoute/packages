#!/bin/bash

echo "Setting system clock reference, expecting none or one parameter"

if [[ $# == 0 ]]; then
  echo "Switching to TCXO (default)"
  let clock=0
else
  if [[ $1 == 0 ]]; then
    echo "Switching to TCXO (default)"
    let clock=0
  else
    echo "Switching to PLL"
    let clock=1
  fi
fi

# System Controller is at address 0x04
i2cset -y 0 0x04 0x01 $clock
