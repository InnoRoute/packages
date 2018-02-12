#!/bin/bash

# I2C/SMBus address of the PLL is 0x69

if [[ $# == 1 ]]; then
  let reg=$1
  printf "Executing Read Access to PLL register %d/0x%04x\n" $reg $reg
  pllctl 0x69 $1
elif [[ $# == 2 ]]; then
  let reg=$1
  let write_data=$2
  printf "Executing Write Access to PLL register %d/0x%04x\n" $reg $reg
  printf "Writing 0x%02x\n" $write_data
  pllctl 0x69 $1 $2
else
  echo "Script to access the PLL."
  echo "USE: TN_phy_access.sh REG [WRDATA]"
  echo "   REG      Number of the register to access"
  echo "   WRDATA   Write Data. If specified, a write access is performed."
  echo "            Otherwise a read access is performed."
fi
