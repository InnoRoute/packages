#!/bin/bash

source /usr/share/InnoRoute/tn_env.sh
source /usr/share/InnoRoute/tn_func_ll.sh

if [[ $# == 0 ]]; then
  echo "$0 <phy> <page> <reg> [<wrdata>] is used to write to or read from a PHY chip"
  echo "Parameter <phy> can be between 0 and 11"
  echo "Parameter value of <page> is ignored in case of GPHYs (PHY 0-9)"
  echo "Parameter <reg> defines the register to be accessed"
  echo "If parameter <wrdata> is given, a write access is performed, with <wrdata> as"
  echo "the write data, otherwise a read access is performed"
else
  # TODO: Add busy checks at address: MSB at "C_BASE_ADDR_MDIO*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_MDIO_READ" should be 0!
  
  if [[ $# == 3 ]]; then
    let phy=$1
    let page=$2
    let reg=$3
    let write_data=0
    printf "Executing Read Access to PHY %d, page %d, register %d/0x%02x\n" $(($phy & 0x1F)) $(($page & 0xFFFF)) $(($reg & 0x1F)) $(($reg & 0x1F))
    printf "Access to port %d\n" $(($phy & 0x1F))
    tn_ll_read_phy $phy $page $reg
    printf "Read Data %04x\n" $(( $read_data & 0xFFFF ))
    echo "Done"
  elif [[ $# == 4 ]]; then
    let phy=$1
    let page=$2
    let reg=$3
    let write_data=$4
    printf "Executing Write Access to PHY %d, page %d, register %d/0x%02x\n" $(($phy & 0x1F)) $(($page & 0xFFFF)) $(($reg & 0x1F)) $(($reg & 0x1F))
    printf "Writing 0x%04x\n" $(($write_data & 0xFFFF))
    printf "Access to port %d\n" $(($phy & 0x1F))
    tn_ll_write_phy $phy $page $reg $write_data
    echo "Done"
  else
    echo "$0: Bad number of parameters ($#)!"
  fi
fi
