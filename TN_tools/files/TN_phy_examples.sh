#!/bin/bash

# TODO: Add busy checks at address: MSB at "C_BASE_ADDR_MDIO*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_MDIO_READ" should be 0!

source /usr/share/InnoRoute/tn_env.sh

echo "Executing an MDIO example, expecting one parameter"

write_alaska()
{
  # constant
  let page_reg=22

  # Set page
  TNbar1 $(($C_BASE_ADDR_MDIO*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_MDIO_WRITE)) w $((1*$TN_RGMII_WRITE+($alaska & 0x1F)*$TN_RGMII_PHY+$page_reg*$TN_RGMII_REG+($page & 0xFFFF))) > /dev/null
  # Execute write
  TNbar1 $(($C_BASE_ADDR_MDIO*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_MDIO_WRITE)) w $((1*$TN_RGMII_WRITE+($alaska & 0x1F)*$TN_RGMII_PHY+($reg & 0x1F)*$TN_RGMII_REG+($write_data & 0xFFFF))) > /dev/null
  # Check for completion
  let read_data=`TNbar1 $(($C_BASE_ADDR_MDIO*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_MDIO_READ)) | cut -d " " -f 6`
  if [[ $read_data -eq 0xEEEEEEEE ]]; then
    echo " ** MMI Read Timeout **"
  fi
}

write_gphy()
{
  # Execute write
  TNbar1 $(($C_BASE_ADDR_MDIO*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_MDIO_WRITE)) w $((1*$TN_RGMII_WRITE+(($gphy & 0x1F)+16)*$TN_RGMII_PHY+($reg & 0x1F)*$TN_RGMII_REG+($write_data & 0xFFFF))) > /dev/null
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

if [[ $# == 0 ]]; then
  echo "Soft-resetting all PHYs (MDIO.CTRL register)"
  let page=0
  let reg=0
  let write_data=0x8000
  for gphy in `seq 0 9`; do write_gphy; done
  for alaska in `seq 0 1`; do write_alaska; done
elif [[ $1 == 0 ]]; then
  echo "Setting all PHYs to auto-negotiation and restarting it (MDIO.CTRL register)"
  let page=0
  let reg=0
  let write_data=0x1200
  for gphy in `seq 0 9`; do write_gphy; done
  for alaska in `seq 0 1`; do write_alaska; done
elif [[ $1 == 1 ]]; then
  echo "Setting all PHYs to disabled transceiver test mode, manual master/slave configuration, ... (MDIO.GCTRL register)"
  let page=0
  let reg=9
  let write_data=0x1A00
  let gphy=0
  write_gphy
  let write_data=0x1200
  let gphy=1
  write_gphy
  let write_data=0x1A00
  let gphy=2
  write_gphy
  let write_data=0x1200
  let gphy=3
  write_gphy
  let write_data=0x1A00
  let gphy=4
  write_gphy
  let write_data=0x1200
  let gphy=5
  write_gphy
  let write_data=0x1A00
  let gphy=6
  write_gphy
  let write_data=0x1200
  let gphy=7
  write_gphy
  let write_data=0x1A00
  let gphy=8
  write_gphy
  let write_data=0x1200
  let gphy=9
  write_gphy
  let write_data=0x1A00
  let alaska=0
  write_alaska
  let write_data=0x1200
  let alaska=1
  write_alaska
elif [[ $1 == 2 ]]; then
  echo "Setting all PHYs to disabled test loops, transmitter enabled, transmitter adjustment level of 0, no polarity inversion, no MDI-X mode, disabled 10BASE-Te amplitude, manual MDI/MDI-X (MDIO.PHYCTL1 register)"
  let page=0
  let reg=13
  let write_data=0x0001
  for gphy in `seq 0 9`; do write_gphy; done
  for alaska in `seq 0 1`; do write_alaska; done
elif [[ $1 == 3 ]]; then
  echo "Setting all PHYs to disabled auto-downspeed, 25MHz out, active-low SIGDET input, disabled sticky-bit handling, default ADC resolution, disabled power-consumption scaling based on link quality, disabled auto-negotiation power down, disabled EEE (MDIO.PHYCTL2 register)"
  let page=0
  let reg=14
  let write_data=0x0000
  for gphy in `seq 0 9`; do write_gphy; done
  for alaska in `seq 0 1`; do write_alaska; done
elif [[ $1 == 4 ]]; then
  echo "Setting all PHYs to RXCLK is active on link down, RXSKEW 0, 2V5, TXSKEW 0, CRS=TXRX_RX, copper RGMII mode (MDIO.MMICTRL register)"
  let page=0
  let reg=17
  let write_data=0x8800
  for gphy in `seq 0 9`; do write_gphy; done
  for alaska in `seq 0 1`; do write_alaska; done
elif [[ $1 == 5 ]]; then
  echo "Read GPHY0 counters"
  #first one of the select accesses, then the read execution, followed by reading from the read data register
  let reg=15
   #let write_data=0x0000 # Select Receive Error Count
   let write_data=0x0100 # Select RX Frame Count
   #let write_data=0x0200 # Select ESD Error Count
   #let write_data=0x0300 # Select SSD Error Count
   #let write_data=0x0400 # Select TX Error Count
   #let write_data=0x0500 # Select TX Frame Count
   #let write_data=0x0600 # Select Collision Frame Count
   #let write_data=0x0800 # Select Link Down Count
   #let write_data=0x0900 # Select Auto-Downspeed Count
  TNbar1 $(($C_BASE_ADDR_MDIO*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_MDIO_WRITE)) w $((1*$TN_RGMII_WRITE+16*$TN_RGMII_PHY+($reg & 0x1F)*$TN_RGMII_REG+($write_data & 0xFFFF))) > /dev/null
  sleep 1
  TNbar1 $(($C_BASE_ADDR_MDIO*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_MDIO_WRITE)) w $((0*$TN_RGMII_WRITE+16*$TN_RGMII_PHY+($reg & 0x1F)*$TN_RGMII_REG+($write_data & 0xFFFF))) > /dev/null # Execute Read
  # Get Read Data -> counter is the lower byte of the result
  read_mdio_result
  printf "Counter value = %d\n" $read_data
elif [[ $1 == 6 ]]; then
  echo "Read GPHY1 counters"
  #first one of the select accesses, then the read execution, followed by reading from the read data register
  let reg=15
   #let write_data=0x0000 # Select Receive Error Count
   let write_data=0x0100 # Select RX Frame Count
   #let write_data=0x0200 # Select ESD Error Count
   #let write_data=0x0300 # Select SSD Error Count
   #let write_data=0x0400 # Select TX Error Count
   #let write_data=0x0500 # Select TX Frame Count
   #let write_data=0x0600 # Select Collision Frame Count
   #let write_data=0x0800 # Select Link Down Count
   #let write_data=0x0900 # Select Auto-Downspeed Count
  TNbar1 $(($C_BASE_ADDR_MDIO*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_MDIO_WRITE)) w $((1*$TN_RGMII_WRITE+17*$TN_RGMII_PHY+($reg & 0x1F)*$TN_RGMII_REG+($write_data & 0xFFFF))) > /dev/null
  sleep 1
  TNbar1 $(($C_BASE_ADDR_MDIO*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_MDIO_WRITE)) w $((0*$TN_RGMII_WRITE+17*$TN_RGMII_PHY+($reg & 0x1F)*$TN_RGMII_REG+($write_data & 0xFFFF))) > /dev/null # Execute Read
  # Get Read Data -> counter is the lower byte of the result
  read_mdio_result
  printf "Counter value = %d\n" $read_data
 
### The following is not fully parameterized
  
elif [[ $1 == 7 ]]; then
  echo "GPHY0 LED tests (on-off)"
  read_mdio_result
  # check $read_data if 0x80000000 is set, then wait <to be done before every access!>
  TNbar1 $(($C_BASE_ADDR_MDIO*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_MDIO_WRITE)) w 0x061B000F > /dev/null # write to first GPHY, Reg. 1B, Val. 0x000F
  sleep 1
  TNbar1 $(($C_BASE_ADDR_MDIO*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_MDIO_WRITE)) w 0x061B0000 > /dev/null # write to first GPHY, Reg. 1B, Val. 0x0000
  TNbar1 $(($C_BASE_ADDR_MDIO*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_MDIO_WRITE)) w 0x02020000 > /dev/null # read from first GPHY, Reg. 1B
  read_mdio_result
  # check $read_data if 0x80000000 is set, then wait <to be done before every access!>
elif [[ $1 == 8 ]]; then
  echo "GPHY1 LED tests (on-off)"
  read_mdio_result
  # check $read_data if 0x80000000 is set, then wait <to be done before every access!>
  TNbar1 $(($C_BASE_ADDR_MDIO*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_MDIO_WRITE)) w 0x063B000F > /dev/null # write to second GPHY, Reg. 1B, Val. 0x000F
  sleep 1
  TNbar1 $(($C_BASE_ADDR_MDIO*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_MDIO_WRITE)) w 0x063B0000 > /dev/null # write to second GPHY, Reg. 1B, Val. 0x0000
  TNbar1 $(($C_BASE_ADDR_MDIO*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_MDIO_WRITE)) w 0x02220000 > /dev/null # read from second GPHY, Reg. 1B -> 10001000100000000000000000
  read_mdio_result
  # check $read_data if 0x80000000 is set, then wait <to be done before every access!>
elif [[ $1 == 9 ]]; then
  echo "Alaska0 LED test (blinking)"
  let page=0
  let write_data=0x0000
  TNbar1 $(($C_BASE_ADDR_MDIO*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_MDIO_WRITE)) w 0x04160003 > /dev/null # change page in first Alaska, Reg. 22, Page 3
  TNbar1 $(($C_BASE_ADDR_MDIO*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_MDIO_WRITE)) w 0x0410FBBB > /dev/null # write to first Alaska, Reg. 16, Page 3, Value 0xFBBB
  TNbar1 $(($C_BASE_ADDR_MDIO*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_MDIO_WRITE)) w 0x00160000 > /dev/null # read from first Alaska, Reg. 16, Page 3
  read_mdio_result
  # check $read_data if 0x80000000 is set, then wait <to be done before every access!>
else
  echo "Illegal parameter given"
fi
