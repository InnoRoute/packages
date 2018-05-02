#!/bin/bash

# TODO: Add busy checks at address: MSB at "C_BASE_ADDR_MDIO*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_MDIO_READ" should be 0!

source /usr/share/InnoRoute/tn_env.sh

read_alaska()
{
  # constant
  let page_reg=22

  # Set page
  TNbar1 $(($C_BASE_ADDR_MDIO*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_MDIO_WRITE)) w $((1*$TN_RGMII_WRITE+($alaska & 0x1F)*$TN_RGMII_PHY+$page_reg*$TN_RGMII_REG+($page & 0xFFFF))) > /dev/null
  # Execute read
  TNbar1 $(($C_BASE_ADDR_MDIO*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_MDIO_WRITE)) w $((0*$TN_RGMII_WRITE+($alaska & 0x1F)*$TN_RGMII_PHY+($reg & 0x1F)*$TN_RGMII_REG+($write_data & 0xFFFF))) > /dev/null
  # Check for completion
  let read_data=`TNbar1 $(($C_BASE_ADDR_MDIO*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_MDIO_READ)) | cut -d " " -f 6`
  if [[ $read_data -eq 0xEEEEEEEE ]]; then
    echo " ** MMI Read Timeout **"
  fi
}

read_gphy()
{
  # Execute read
  TNbar1 $(($C_BASE_ADDR_MDIO*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_MDIO_WRITE)) w $((0*$TN_RGMII_WRITE+(($gphy & 0x1F)+16)*$TN_RGMII_PHY+($reg & 0x1F)*$TN_RGMII_REG+($write_data & 0xFFFF))) > /dev/null
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

echo "Reading all standard register values from all Ethernet PHYs"

let page=0
let write_data=0x0000

printf "PHY: "
for gphy   in `seq 0 9`; do
  printf "|GPHY%02i|"  $(($gphy & 0x1F));
done
for alaska in `seq 0 1`; do
  printf "|ALASK%i|" $(($alaska & 0x1F));
done

printf "\nINT: "
reg=26 #int register
for gphy   in `seq 0 9`; do
  read_gphy;
  read_mdio_result;
  printf "|0x%s|"  $(echo "$(printf "%04x  " $read_data)" |cut -c4-8);
done

reg=19
for alaska in `seq 0 1`; do
  read_alaska;
  read_mdio_result;
  printf "|0x%s|" $(echo "$(printf "%04x  " $read_data)" |cut -c3-8);
done

reg=25
printf "\nMSK: "
for gphy   in `seq 0 9`; do
  read_gphy;
  read_mdio_result;
  printf "|0x%s|"  $(echo "$(printf "%04x  " $read_data)" |cut -c4-8);
done

reg=18
for alaska in `seq 0 1`; do
  read_alaska;
  read_mdio_result;
  printf "|0x%s|" $(echo "$(printf "%04x  " $read_data)" |cut -c3-8);
done

printf "\n"

#Clear all Interrupts:
#  for phy in `seq  0  9`; do /usr/share/InnoRoute/TN_phy_access.sh $(($phy & 0x1F))  0 0x1A; done
#  for phy in `seq 10 11`; do /usr/share/InnoRoute/TN_phy_access.sh $(($phy & 0x1F))  0   19; done
#  for phy in `seq 10 11`; do /usr/share/InnoRoute/TN_phy_access.sh $(($phy & 0x1F)) 14    8; done

#for alaska in `seq 0 1`; do
#  read_alaska;
#  read_mdio_result;
#  printf "Reg0x%02x:%08x\n" $(($reg & 0x1F)) $read_data | cut -c1-8,13-16;
#done
