#!/bin/bash

source /usr/share/InnoRoute/tn_env.sh
source /usr/share/InnoRoute/tn_func_ll.sh

if [[ $# == 0 ]]; then
  echo "$0 <ex> is used to read the interrupt status from all Ethernet PHY chips,"
  echo "thereby clearing the interrupt registers in the Ethernet PHY chips"
  echo "The parameter <ex> can have any value. Without it, this help is displayed"
else

  # TODO: Add busy checks at address: MSB at "C_BASE_ADDR_MDIO*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_MDIO_READ" should be 0!
    
  echo "Reading all standard register values from all Ethernet PHYs"
  
  let page=0
  let write_data=0x0000
  
  printf "PHY: "
  for phy in `seq 0 9`; do
    printf "|GPHY%02i|"  $(($phy & 0x1F));
  done
  for phy in `seq 10 11`; do
    printf "|ALASK%i|" $(($phy & 0x1F));
  done
  
  printf "\nINT: "
  reg=26 #int register
  for phy in `seq 0 9`; do
    tn_ll_read_phy $phy $page $reg;
    printf "|0x%s|"  $(echo "$(printf "%04x  " $read_data)" |cut -c4-8);
  done
  
  reg=19
  for phy in `seq 10 11`; do
    tn_ll_read_phy $phy $page $reg;
    printf "|0x%s|" $(echo "$(printf "%04x  " $read_data)" |cut -c3-8);
  done
  
  reg=25
  printf "\nMSK: "
  for phy in `seq 0 9`; do
    tn_ll_read_phy $phy $page $reg;
    printf "|0x%s|"  $(echo "$(printf "%04x  " $read_data)" |cut -c4-8);
  done
  
  reg=18
  for phy in `seq 10 11`; do
    tn_ll_read_phy $phy $page $reg;
    printf "|0x%s|" $(echo "$(printf "%04x  " $read_data)" |cut -c3-8);
  done
  
  printf "\n"
  
  #Clear all Interrupts:
  #  for phy in `seq  0  9`; do /usr/share/InnoRoute/TN_phy_access.sh $(($phy & 0x1F))  0 0x1A; done
  #  for phy in `seq 10 11`; do /usr/share/InnoRoute/TN_phy_access.sh $(($phy & 0x1F))  0   19; done
  #  for phy in `seq 10 11`; do /usr/share/InnoRoute/TN_phy_access.sh $(($phy & 0x1F)) 14    8; done
  
  #for phy in `seq 10 11`; do
  #  tn_ll_read_phy $phy $page $reg;
  #  printf "Reg0x%02x:%08x\n" $(($reg & 0x1F)) $read_data | cut -c1-8,13-16;
  #done

  echo "Done"
fi
