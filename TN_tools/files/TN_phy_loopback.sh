#!/bin/bash

# TODO: Add busy checks at address: MSB at "C_BASE_ADDR_MDIO*$C_BASE_ADDR_FACTOR+4" should be 0!

source /usr/share/InnoRoute/tn_env.sh

if [[ $# == 0 ]]; then
  # Set loopback
  let write_data=0x4000
  echo "Enabling loopback (TXMAC->PHY->RXMAC) ..."
else
  # If a parameter is given, it is assumed that this select between loopback (1) and normal operation (0)
  if [[ $1 == 1 ]]; then
    let write_data=0x4000
    echo "Enabling loopback (TXMAC->PHY->RXMAC) ..."
  else
    let write_data=0x1200
    echo "Disabling loopback - instead enabling Auto-Negotiation and restarting it ..."
  fi
fi

write_alaska()
{
  # constant
  let page_reg=22

  # Set page
  TNbar1 $(($C_BASE_ADDR_MDIO*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_MDIO_WRITE)) w $((1*$TN_RGMII_WRITE+$alaska*$TN_RGMII_PHY+$page_reg*$TN_RGMII_REG+$page)) > /dev/null
  # Execute write
  TNbar1 $(($C_BASE_ADDR_MDIO*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_MDIO_WRITE)) w $((1*$TN_RGMII_WRITE+$alaska*$TN_RGMII_PHY+$reg*$TN_RGMII_REG+$write_data)) > /dev/null
  # Check for completion
  let read_data=`TNbar1 $(($C_BASE_ADDR_MDIO*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_MDIO_READ)) | cut -d " " -f 6`
  if [[ $read_data -eq 0xEEEEEEEE ]]; then
    echo " ** MMI Read Timeout **"
  fi
}

write_gphy()
{
  # Execute write
  TNbar1 $(($C_BASE_ADDR_MDIO*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_MDIO_WRITE)) w $((1*$TN_RGMII_WRITE+($gphy+16)*$TN_RGMII_PHY+$reg*$TN_RGMII_REG+$write_data)) > /dev/null
  # Check for completion
  let read_data=`TNbar1 $(($C_BASE_ADDR_MDIO*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_MDIO_READ)) | cut -d " " -f 6`
  if [[ $read_data -eq 0xEEEEEEEE ]]; then
    echo " ** MMI Read Timeout **"
  fi
}

### 10 GPHYs (register numbers in the manual are in hex)

# Control Register: Reg 0x00
# * Value 0x1200: Setting all PHYs to auto-negotiation and restarting it (MDIO.CTRL register)
let reg=0x00
for gphy in `seq 0 9`; do write_gphy; done

### 2 Alaskas (register 22 is the page register)

# Copper Control Register: Page 0, Reg 0
# * Value 0x1200: Setting all PHYs to auto-negotiation and restarting it
let page=0
let reg=0
for alaska in `seq 0 1`; do write_alaska; done

echo "Done"
