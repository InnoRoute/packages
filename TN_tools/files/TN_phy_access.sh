#!/bin/bash

# TODO: Add busy checks at address: MSB at "C_BASE_ADDR_MDIO*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_MDIO_READ" should be 0!

source /usr/share/InnoRoute/tn_env.sh

write_alaska()
{
  # constant
  let page_reg=22

  # Set page
  TNbar1 $(($C_BASE_ADDR_MDIO*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_MDIO_WRITE)) w $((1*$TN_RGMII_WRITE+$phy*$TN_RGMII_PHY+$page_reg*$TN_RGMII_REG+$page)) > /dev/null
  # Execute write
  TNbar1 $(($C_BASE_ADDR_MDIO*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_MDIO_WRITE)) w $((1*$TN_RGMII_WRITE+$phy*$TN_RGMII_PHY+$reg*$TN_RGMII_REG+$write_data)) > /dev/null
  # Check for completion
  let read_data=`TNbar1 $(($C_BASE_ADDR_MDIO*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_MDIO_READ)) | cut -d " " -f 6`
  if [[ $read_data -eq 0xEEEEEEEE ]]; then
    echo " ** MMI Read Timeout **"
  fi
}

write_gphy()
{
  # Execute write
  TNbar1 $(($C_BASE_ADDR_MDIO*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_MDIO_WRITE)) w $((1*$TN_RGMII_WRITE+($phy+16)*$TN_RGMII_PHY+$reg*$TN_RGMII_REG+$write_data)) > /dev/null
  # Check for completion
  let read_data=`TNbar1 $(($C_BASE_ADDR_MDIO*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_MDIO_READ)) | cut -d " " -f 6`
  if [[ $read_data -eq 0xEEEEEEEE ]]; then
    echo " ** MMI Read Timeout **"
  fi
}

read_alaska()
{
  # constant
  let page_reg=22

  # Set page
  TNbar1 $(($C_BASE_ADDR_MDIO*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_MDIO_WRITE)) w $((1*$TN_RGMII_WRITE+$phy*$TN_RGMII_PHY+$page_reg*$TN_RGMII_REG+$page)) > /dev/null
  # Execute read
  TNbar1 $(($C_BASE_ADDR_MDIO*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_MDIO_WRITE)) w $((0*$TN_RGMII_WRITE+$phy*$TN_RGMII_PHY+$reg*$TN_RGMII_REG+$write_data)) > /dev/null
  # Check for completion
  let read_data=`TNbar1 $(($C_BASE_ADDR_MDIO*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_MDIO_READ)) | cut -d " " -f 6`
  if [[ $read_data -eq 0xEEEEEEEE ]]; then
    echo " ** MMI Read Timeout **"
  fi
}

read_gphy()
{
  # Execute read
  TNbar1 $(($C_BASE_ADDR_MDIO*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_MDIO_WRITE)) w $((0*$TN_RGMII_WRITE+($phy+16)*$TN_RGMII_PHY+$reg*$TN_RGMII_REG+$write_data)) > /dev/null
  # Check for completion
  let read_data=`TNbar1 $(($C_BASE_ADDR_MDIO*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_MDIO_READ)) | cut -d " " -f 6`
  if [[ $read_data -eq 0xEEEEEEEE ]]; then
    echo " ** MMI Read Timeout **"
  fi
}

read_mdio_result()
{
  let read_data=`TNbar1 $(($C_BASE_ADDR_MDIO*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_MDIO_READ)) | cut -d " " -f 6`
  if [[ $read_data -eq 0xEEEEEEEE ]]; then
    echo " ** MMI Read Timeout **"
  fi
}

if [[ $# == 3 ]]; then
  let phy=$1
  let page=$2
  let reg=$3
  let write_data=0
  printf "Executing Read Access to PHY %d, page %d, register %d/0x%02x\n" $phy $page $reg $reg
  if [[ $1 -lt $TN_NO_OF_GPHYS ]]; then
    printf "Access to GPHY %d\n" $phy
    read_gphy
  else
    printf "Access to Alaska %d\n" $(( $phy - $TN_NO_OF_GPHYS ))
    read_alaska
  fi
  read_mdio_result	
  printf "Read Data %04x\n" $read_data
elif [[ $# == 4 ]]; then
  let phy=$1
  let page=$2
  let reg=$3
  let write_data=$4
  printf "Executing Write Access to PHY %d, page %d, register %d/0x%02x\n" $phy $page $reg $reg
  printf "Writing 0x%04x\n" $write_data
  if [[ $1 -lt $TN_NO_OF_GPHYS ]]; then
    printf "Access to GPHY %d\n" $phy
    write_gphy
  else
    printf "Access to Alaska %d\n" $(( $phy - $TN_NO_OF_GPHYS ))
    write_alaska
  fi
else
  echo "Script to access the Ethernet PHYs."
  echo "USE: TN_phy_access.sh PHY PAGE REG [WRDATA]"
  echo "   PHY      Number of the port [0..11]"
  echo "   PAGE     Number of the MDIO page, ignored for ports 0-9"
  echo "   REG      Number of the register to access"
  echo "   WRDATA   Write Data. If specified, a write access is performed."
  echo "            Otherwise a read access is performed."
fi
