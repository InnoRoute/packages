#!/bin/bash

source /usr/share/InnoRoute/tn_env.sh
source /usr/share/InnoRoute/tn_func_ll.sh

if [[ $# == 0 ]]; then
  echo "$0 <reg> [<wrdata>] is used to read from or write to the AD9558 PLL"
  echo "Parameter <reg> defineds the register to read from or write to"
  echo "If the parameter <wrdata> is given, a write access with this parameter's"
  echo "value as the write data is performed, otherwise a read access is performed"
else
  # I2C/SMBus address of the PLL is 0x69
  
  if [[ $# == 1 ]]; then
    let reg=$1
    printf "Executing Read Access to PLL register %d/0x%04x\n" $reg $reg
    pllctl 0x69 $1
  else
    let reg=$1
    let write_data=$2
    printf "Executing Write Access to PLL register %d/0x%04x\n" $reg $reg
    printf "Writing 0x%02x\n" $write_data
    pllctl 0x69 $1 $2
  fi

  echo "Done"
fi
