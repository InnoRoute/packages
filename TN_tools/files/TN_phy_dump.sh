#!/bin/bash

source /usr/share/InnoRoute/tn_env.sh
source /usr/share/InnoRoute/tn_func_ll.sh

if [[ $# == 0 ]]; then
  echo "$0 <ex> is used to dump all PHY register contents to files and compare them"
  echo "The parameter <ex> can have any value. Without it, this help is displayed"
else

  # TODO: Add busy checks at address: MSB at "C_BASE_ADDR_MDIO*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_MDIO_READ" should be 0!
  
  echo "Cleaning up"
  
  for phy in `seq 0 9`; do
    if [[ -e /tmp/gphy$(($phy & 0x1F)).txt ]]; then
         rm /tmp/gphy$(($phy & 0x1F)).txt;
    fi;
  done
  for phy in `seq 0 9`; do
    if [[ -e /tmp/gphy_mmd$(($phy & 0x1F)).txt ]]; then
         rm /tmp/gphy_mmd$(($phy & 0x1F)).txt;
    fi;
  done
  for phy in `seq 10 11`; do
    if [[ -e /tmp/alaska$(($phy & 0x1F)).txt ]]; then
         rm /tmp/alaska$(($phy & 0x1F)).txt;
    fi;
  done
  
  echo "Reading all standard register values from all Ethernet PHYs"
  
  let page=0
  let write_data=0x0000
  for reg in `seq 0 31`; do
    for phy in `seq 0 9`;   do
      tn_ll_read_phy $phy $page $reg;
      printf "Reg0x%02x:%08x\n" $(($reg & 0x1F)) $read_data | cut -c1-8,13-16 >> /tmp/gphy$(($phy & 0x1F)).txt;
      echo -n "."
    done
    for phy in `seq 10 11`; do
      tn_ll_read_phy $phy $page $reg;
      printf "Reg0x%02x:%08x\n" $(($reg & 0x1F)) $read_data | cut -c1-8,13-16 >> /tmp/alaska$(($phy & 0x1F)).txt;
      echo -n "."
    done
  done
  
  echo -e "\nDifferences between GPHYs:"
  for phy in `seq 1 9`; do
    echo -e "GPHY0 vs. GPHY$(($phy & 0x1F))"
    diff -y --suppress-common-lines /tmp/gphy0.txt /tmp/gphy$(($phy & 0x1F)).txt
  done
  
  echo "Differences between Alaskas:"
  echo "Alaska0 vs. Alaska1"
  diff -y --suppress-common-lines /tmp/alaska10.txt /tmp/alaska11.txt
  
  exit
  ########## MMD registers are not checked at this point ##########
  
  
  # In addition to the STD registers, also the MMD registers might be read/set
  #  -> GPHY: EEE, EEE-AN, EEPROM, LEDs, EEE Link-Fail Counters, WoL
  # 1. Write (0x03/0x07/0x1E or) 0x1F to reg 0x0d (MMD Ctrl reg) - Address Type
  # 2. Write register number to reg 0x0e (MMD Data reg)
  # 3. Write, e.g., 0x401f to reg 0x0d (MMD Ctrl reg) - Data Type
  # 4. Write or read reg 0x0e (MMD Data reg)
  # Alaskas: Page select through register 0x16 (which is the EEPROM control register in the GPHY)
  
  for phy in `seq 0 9`; do
    for mmdreg in 0x03,0x0000 \
                  0x03,0x0001 \
                  0x03,0x0014 \
                  0x03,0x0016 \
                  0x03,0x003C \
                  0x03,0x003D \
                  0x1E,0x0000 \
                  0x1F,0x01E0 \
                  0x1F,0x01E1 \
                  0x1F,0x01E2 \
                  0x1F,0x01E3 \
                  0x1F,0x01E4 \
                  0x1F,0x01E5 \
                  0x1F,0x01E6 \
                  0x1F,0x01E7 \
                  0x1F,0x01EA \
                  0x1F,0x01EB \
                  0x1F,0x01EC \
                  0x1F,0x01ED \
                  0x1F,0x01EE \
                  0x1F,0x0781 \
                  0x1F,0x0783 \
                  0x1F,0x0784 \
                  0x1F,0x0785 \
                  0x1F,0x0786 \
                  0x1F,0x0787 \
                  0x1F,0x0788 \
                  0x1F,0x0789 \
                  0x1F,0x078A \
                  0x1F,0x078B \
                  0x1F,0x078C \
                  0x1F,0x078D \
                  0x1F,0x078E \
                  0x1F,0x0EB5 \
                  0x1F,0x0EB7; do
      IFS=','
      set -- $mmdreg
      let devaddr=$1
      let regno=$2
  
      #printf "PHY=%d: %02x.%04x\n" $(($phy & 0x1F)) $devaddr $regno
      # Write Address Type to reg 0x0d (MMD Ctrl reg)
      let reg=0x0D
      let write_data=0x0000+$devaddr
      tn_ll_write_phy $phy $page $reg $write_data
      # Write register number to reg 0x0e (MMD Data reg)
      let reg=0x0E
      let write_data=$regno
      tn_ll_write_phy $phy $page $reg $write_data
      # Write Data Type to reg 0x0d (MMD Ctrl reg)
      let reg=0x0D
      let write_data=0x4000+$devaddr
      tn_ll_write_phy $phy $page $reg $write_data
      # Read reg 0x0e (MMD Data reg)
      let reg=0x0E
      tn_ll_read_phy $phy $page $reg
      if [[ $status -ne 0 ]]; then
        echo " ** MMI Read Timeout **"
      fi
      printf "Reg %02X.%04XH:0x%08x\n" $devaddr $regno $read_data | cut -c1-15,20-23 >> /tmp/gphy_mmd$(($phy & 0x1F)).txt
      echo -n "."
    done
  done
  
  echo "Dumping GPHY MMD register differences:"
  
  for phy in `seq 1 9`; do
    echo -e "GPHY0 vs. GPHY$(($phy & 0x1F))"
    diff -y --suppress-common-lines /tmp/gphy_mmd0.txt /tmp/gphy_mmd$(($phy & 0x1F)).txt
  done

  echo "Done"
fi
