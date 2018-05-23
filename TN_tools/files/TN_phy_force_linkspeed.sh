#!/bin/bash
#todo: add alaska operation
source /usr/share/InnoRoute/tn_env.sh
write_alaska()
{
  # constant
  let page_reg=22

  # Set page
  TNbar1 $(($C_BASE_ADDR_MDIO*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_MDIO_WRITE)) w $((1*$TN_RGMII_WRITE+($phy & 0x1F)*$TN_RGMII_PHY+$page_reg*$TN_RGMII_REG+($page & 0xFFFF))) > /dev/null
  # Execute write
  TNbar1 $(($C_BASE_ADDR_MDIO*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_MDIO_WRITE)) w $((1*$TN_RGMII_WRITE+($phy & 0x1F)*$TN_RGMII_PHY+($reg & 0x1F)*$TN_RGMII_REG+($write_data & 0xFFFF))) > /dev/null
  # Check for completion
  let read_data=`TNbar1 $(($C_BASE_ADDR_MDIO*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_MDIO_READ)) | cut -d " " -f 6`
  if [[ $read_data -eq 0xEEEEEEEE ]]; then
    echo " ** MMI Read Timeout **"
  fi
}

write_gphy()
{
  # Execute write
  TNbar1 $(($C_BASE_ADDR_MDIO*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_MDIO_WRITE)) w $((1*$TN_RGMII_WRITE+(($phy & 0x1F)+16)*$TN_RGMII_PHY+($reg & 0x1F)*$TN_RGMII_REG+($write_data & 0xFFFF))) > /dev/null
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
  TNbar1 $(($C_BASE_ADDR_MDIO*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_MDIO_WRITE)) w $((1*$TN_RGMII_WRITE+($phy & 0x1F)*$TN_RGMII_PHY+$page_reg*$TN_RGMII_REG+($page & 0xFFFF))) > /dev/null
  # Execute read
  TNbar1 $(($C_BASE_ADDR_MDIO*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_MDIO_WRITE)) w $((0*$TN_RGMII_WRITE+($phy & 0x1F)*$TN_RGMII_PHY+($reg & 0x1F)*$TN_RGMII_REG+($write_data & 0xFFFF))) > /dev/null
  # Check for completion
  let read_data=`TNbar1 $(($C_BASE_ADDR_MDIO*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_MDIO_READ)) | cut -d " " -f 6`
  if [[ $read_data -eq 0xEEEEEEEE ]]; then
    echo " ** MMI Read Timeout **"
  fi
}

read_gphy()
{
  # Execute read
  TNbar1 $(($C_BASE_ADDR_MDIO*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_MDIO_WRITE)) w $((0*$TN_RGMII_WRITE+(($phy & 0x1F)+16)*$TN_RGMII_PHY+($reg & 0x1F)*$TN_RGMII_REG+($write_data & 0xFFFF))) > /dev/null
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

echo -e "set FPGA linkspeed, parameter:linkspeed [0,1,2]\n\t0=10Mbit/s\n\t1=100MBit/s\n\t2=1000Mbit/s"
if [ -z "${C_SUB_ADDR_MAC_SPEED+xxx}" ]; then 
	echo "Setting linkspeed is not supported with this tn_env-setting." 
	exit 0
fi
if [[ $# != 1 ]]; then
  echo ""
  
else
 if [ "$1" -eq "0" ]; then #10Mbit/s
  let page=0x0
  let reg=0x4
  let write_data=0x841
  for phy   in `seq 0 9`; do 
  	write_gphy
  done
  let page=0x0
  let reg=0x9
  let write_data=0x0
  for phy   in `seq 0 9`; do 
  	write_gphy
  done
 fi
 if [ "$1" -eq "1" ]; then #100Mbit/s
  let page=0x0
  let reg=0x4
  let write_data=0x901
  for phy   in `seq 0 9`; do 
  	write_gphy
  done
  let page=0x0
  let reg=0x9
  let write_data=0x0
  for phy   in `seq 0 9`; do 
  	write_gphy
  done
 fi
 if [ "$1" -eq "2" ]; then #1000Mbit/s
  let page=0x0
  let reg=0x4
  let write_data=0x801
  for phy   in `seq 0 9`; do 
  	write_gphy
  done
  let page=0x0
  let reg=0x9
  let write_data=0x200
  for phy   in `seq 0 9`; do 
  	write_gphy
  done
 fi
 let page=0x0
 let reg=0x0
 let write_data=0x1200
  for phy   in `seq 0 9`; do 
  	write_gphy
  done
 TN_linkspeed.sh $1
fi



