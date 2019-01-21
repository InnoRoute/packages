#!/bin/bash

source /usr/share/InnoRoute/tn_env.sh
source /usr/share/InnoRoute/tn_func_ll.sh

if [[ $# == 0 ]]; then
  echo "$0 <loopback> is used to set the MAC-to-MAC loopback in the Ethernet PHY chips"
  echo "Parameter <loopback> can be between 0 or 1"
  echo "  0: no loopback (default)"
  echo "  1: loopback"
else
  
  # TODO: Add busy checks at address: MSB at "C_BASE_ADDR_MDIO*$C_BASE_ADDR_FACTOR+4" should be 0!
  
  # If a parameter is given, it is assumed that this select between loopback (1) and normal operation (0)
  if [[ $1 == 1 ]]; then
    let write_data=0x4000
    echo "Enabling loopback (TXMAC->PHY->RXMAC) ..."
  else
    let write_data=0x1200
    echo "Disabling loopback - instead enabling Auto-Negotiation and restarting it ..."
  fi
    
  # Control Register: Reg 0x00
  let page=0
  let reg=0
  for phy in `seq 0 11`; do tn_ll_write_phy $phy $page $reg $write_data; done
  
  echo "Done"
fi
