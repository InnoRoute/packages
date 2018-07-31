#!/bin/bash

source /usr/share/InnoRoute/tn_env.sh
source /usr/share/InnoRoute/tn_func_ll.sh

if [[ $# == 0 ]]; then
  echo "$0 <clock> is used to set the system's reference clock"
  echo "Parameter <clock> can be 0 or 1"
  echo "  0: Default TCXO is used"
  echo "  1: AD9558 PLL is used"
else
  echo "Setting system's reference clock ..."

  if [[ $1 == 0 ]]; then
    echo "Switching to TCXO (default)"
    let clock=0
  else
    echo "Switching to PLL"
    let clock=1
  fi
  
  # System Controller is at address 0x04
  i2cset -y 0 0x04 0x01 $clock

  echo "Done"
fi
