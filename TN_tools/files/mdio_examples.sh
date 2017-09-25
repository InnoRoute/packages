#!/bin/bash

# TODO: Add busy checks at address: MSB at "TN_BASE_ADDR_MDIO+4" should be 0!

source /usr/share/InnoRoute/tn_env.sh

echo "Executing an MDIO example, expecting one parameter"

if [[ $# == 0 ]]; then
  echo "Soft-resetting all PHYs (MDIO.CTRL register)"
  let reg=0
  let write_data=0x8000
  TNbar1 $(($TN_BASE_ADDR_MDIO+0)) w $((1*$TN_RGMII_WRITE+16*$TN_RGMII_PHY+$reg*$TN_RGMII_REG+$write_data))
  TNbar1 $(($TN_BASE_ADDR_MDIO+0)) w $((1*$TN_RGMII_WRITE+17*$TN_RGMII_PHY+$reg*$TN_RGMII_REG+$write_data))
  TNbar1 $(($TN_BASE_ADDR_MDIO+0)) w $((1*$TN_RGMII_WRITE+18*$TN_RGMII_PHY+$reg*$TN_RGMII_REG+$write_data))
  TNbar1 $(($TN_BASE_ADDR_MDIO+0)) w $((1*$TN_RGMII_WRITE+19*$TN_RGMII_PHY+$reg*$TN_RGMII_REG+$write_data))
  TNbar1 $(($TN_BASE_ADDR_MDIO+0)) w $((1*$TN_RGMII_WRITE+20*$TN_RGMII_PHY+$reg*$TN_RGMII_REG+$write_data))
  TNbar1 $(($TN_BASE_ADDR_MDIO+0)) w $((1*$TN_RGMII_WRITE+21*$TN_RGMII_PHY+$reg*$TN_RGMII_REG+$write_data))
  TNbar1 $(($TN_BASE_ADDR_MDIO+0)) w $((1*$TN_RGMII_WRITE+22*$TN_RGMII_PHY+$reg*$TN_RGMII_REG+$write_data))
  TNbar1 $(($TN_BASE_ADDR_MDIO+0)) w $((1*$TN_RGMII_WRITE+23*$TN_RGMII_PHY+$reg*$TN_RGMII_REG+$write_data))
  TNbar1 $(($TN_BASE_ADDR_MDIO+0)) w $((1*$TN_RGMII_WRITE+24*$TN_RGMII_PHY+$reg*$TN_RGMII_REG+$write_data))
  TNbar1 $(($TN_BASE_ADDR_MDIO+0)) w $((1*$TN_RGMII_WRITE+25*$TN_RGMII_PHY+$reg*$TN_RGMII_REG+$write_data))
  TNbar1 $(($TN_BASE_ADDR_MDIO+0)) w $((1*$TN_RGMII_WRITE+ 0*$TN_RGMII_PHY+$reg*$TN_RGMII_REG+$write_data))
  TNbar1 $(($TN_BASE_ADDR_MDIO+0)) w $((1*$TN_RGMII_WRITE+ 1*$TN_RGMII_PHY+$reg*$TN_RGMII_REG+$write_data))
elif [[ $1 == 0 ]]; then
  echo "Setting all PHYs to 1 Gbps full-duplex with auto-negotiation etc. (MDIO.CTRL register)"
  let reg=0
  let write_data=0x1340
  TNbar1 $(($TN_BASE_ADDR_MDIO+0)) w $((1*$TN_RGMII_WRITE+16*$TN_RGMII_PHY+$reg*$TN_RGMII_REG+$write_data))
  TNbar1 $(($TN_BASE_ADDR_MDIO+0)) w $((1*$TN_RGMII_WRITE+17*$TN_RGMII_PHY+$reg*$TN_RGMII_REG+$write_data))
  TNbar1 $(($TN_BASE_ADDR_MDIO+0)) w $((1*$TN_RGMII_WRITE+18*$TN_RGMII_PHY+$reg*$TN_RGMII_REG+$write_data))
  TNbar1 $(($TN_BASE_ADDR_MDIO+0)) w $((1*$TN_RGMII_WRITE+19*$TN_RGMII_PHY+$reg*$TN_RGMII_REG+$write_data))
  TNbar1 $(($TN_BASE_ADDR_MDIO+0)) w $((1*$TN_RGMII_WRITE+20*$TN_RGMII_PHY+$reg*$TN_RGMII_REG+$write_data))
  TNbar1 $(($TN_BASE_ADDR_MDIO+0)) w $((1*$TN_RGMII_WRITE+21*$TN_RGMII_PHY+$reg*$TN_RGMII_REG+$write_data))
  TNbar1 $(($TN_BASE_ADDR_MDIO+0)) w $((1*$TN_RGMII_WRITE+22*$TN_RGMII_PHY+$reg*$TN_RGMII_REG+$write_data))
  TNbar1 $(($TN_BASE_ADDR_MDIO+0)) w $((1*$TN_RGMII_WRITE+23*$TN_RGMII_PHY+$reg*$TN_RGMII_REG+$write_data))
  TNbar1 $(($TN_BASE_ADDR_MDIO+0)) w $((1*$TN_RGMII_WRITE+24*$TN_RGMII_PHY+$reg*$TN_RGMII_REG+$write_data))
  TNbar1 $(($TN_BASE_ADDR_MDIO+0)) w $((1*$TN_RGMII_WRITE+25*$TN_RGMII_PHY+$reg*$TN_RGMII_REG+$write_data))
  let write_data=0x9140
  TNbar1 $(($TN_BASE_ADDR_MDIO+0)) w $((1*$TN_RGMII_WRITE+ 0*$TN_RGMII_PHY+$reg*$TN_RGMII_REG+$write_data))
  TNbar1 $(($TN_BASE_ADDR_MDIO+0)) w $((1*$TN_RGMII_WRITE+ 1*$TN_RGMII_PHY+$reg*$TN_RGMII_REG+$write_data))
elif [[ $1 == 1 ]]; then
  echo "Setting all PHYs to disabled transceiver test mode, manual master/slave configuration, ... (MDIO.GCTRL register)"
  let reg=9
  let write_data=0x1A00
  TNbar1 $(($TN_BASE_ADDR_MDIO+0)) w $((1*$TN_RGMII_WRITE+16*$TN_RGMII_PHY+$reg*$TN_RGMII_REG+$write_data))
  let write_data=0x1200
  TNbar1 $(($TN_BASE_ADDR_MDIO+0)) w $((1*$TN_RGMII_WRITE+17*$TN_RGMII_PHY+$reg*$TN_RGMII_REG+$write_data))
  let write_data=0x1A00
  TNbar1 $(($TN_BASE_ADDR_MDIO+0)) w $((1*$TN_RGMII_WRITE+18*$TN_RGMII_PHY+$reg*$TN_RGMII_REG+$write_data))
  let write_data=0x1200
  TNbar1 $(($TN_BASE_ADDR_MDIO+0)) w $((1*$TN_RGMII_WRITE+19*$TN_RGMII_PHY+$reg*$TN_RGMII_REG+$write_data))
  let write_data=0x1A00
  TNbar1 $(($TN_BASE_ADDR_MDIO+0)) w $((1*$TN_RGMII_WRITE+20*$TN_RGMII_PHY+$reg*$TN_RGMII_REG+$write_data))
  let write_data=0x1200
  TNbar1 $(($TN_BASE_ADDR_MDIO+0)) w $((1*$TN_RGMII_WRITE+21*$TN_RGMII_PHY+$reg*$TN_RGMII_REG+$write_data))
  let write_data=0x1A00
  TNbar1 $(($TN_BASE_ADDR_MDIO+0)) w $((1*$TN_RGMII_WRITE+22*$TN_RGMII_PHY+$reg*$TN_RGMII_REG+$write_data))
  let write_data=0x1200
  TNbar1 $(($TN_BASE_ADDR_MDIO+0)) w $((1*$TN_RGMII_WRITE+23*$TN_RGMII_PHY+$reg*$TN_RGMII_REG+$write_data))
  let write_data=0x1A00
  TNbar1 $(($TN_BASE_ADDR_MDIO+0)) w $((1*$TN_RGMII_WRITE+24*$TN_RGMII_PHY+$reg*$TN_RGMII_REG+$write_data))
  let write_data=0x1200
  TNbar1 $(($TN_BASE_ADDR_MDIO+0)) w $((1*$TN_RGMII_WRITE+25*$TN_RGMII_PHY+$reg*$TN_RGMII_REG+$write_data))
  let write_data=0x1A00
  TNbar1 $(($TN_BASE_ADDR_MDIO+0)) w $((1*$TN_RGMII_WRITE+ 0*$TN_RGMII_PHY+$reg*$TN_RGMII_REG+$write_data))
  let write_data=0x1200
  TNbar1 $(($TN_BASE_ADDR_MDIO+0)) w $((1*$TN_RGMII_WRITE+ 1*$TN_RGMII_PHY+$reg*$TN_RGMII_REG+$write_data))
elif [[ $1 == 2 ]]; then
  echo "Setting all PHYs to disabled test loops, transmitter enabled, transmitter adjustment level of 0, no polarity inversion, no MDI-X mode, disabled 10BASE-Te amplitude, manual MDI/MDI-X (MDIO.PHYCTL1 register)"
  let reg=13
  let write_data=0x0001
  TNbar1 $(($TN_BASE_ADDR_MDIO+0)) w $((1*$TN_RGMII_WRITE+16*$TN_RGMII_PHY+$reg*$TN_RGMII_REG+$write_data))
  TNbar1 $(($TN_BASE_ADDR_MDIO+0)) w $((1*$TN_RGMII_WRITE+17*$TN_RGMII_PHY+$reg*$TN_RGMII_REG+$write_data))
  TNbar1 $(($TN_BASE_ADDR_MDIO+0)) w $((1*$TN_RGMII_WRITE+18*$TN_RGMII_PHY+$reg*$TN_RGMII_REG+$write_data))
  TNbar1 $(($TN_BASE_ADDR_MDIO+0)) w $((1*$TN_RGMII_WRITE+19*$TN_RGMII_PHY+$reg*$TN_RGMII_REG+$write_data))
  TNbar1 $(($TN_BASE_ADDR_MDIO+0)) w $((1*$TN_RGMII_WRITE+20*$TN_RGMII_PHY+$reg*$TN_RGMII_REG+$write_data))
  TNbar1 $(($TN_BASE_ADDR_MDIO+0)) w $((1*$TN_RGMII_WRITE+21*$TN_RGMII_PHY+$reg*$TN_RGMII_REG+$write_data))
  TNbar1 $(($TN_BASE_ADDR_MDIO+0)) w $((1*$TN_RGMII_WRITE+22*$TN_RGMII_PHY+$reg*$TN_RGMII_REG+$write_data))
  TNbar1 $(($TN_BASE_ADDR_MDIO+0)) w $((1*$TN_RGMII_WRITE+23*$TN_RGMII_PHY+$reg*$TN_RGMII_REG+$write_data))
  TNbar1 $(($TN_BASE_ADDR_MDIO+0)) w $((1*$TN_RGMII_WRITE+24*$TN_RGMII_PHY+$reg*$TN_RGMII_REG+$write_data))
  TNbar1 $(($TN_BASE_ADDR_MDIO+0)) w $((1*$TN_RGMII_WRITE+25*$TN_RGMII_PHY+$reg*$TN_RGMII_REG+$write_data))
  TNbar1 $(($TN_BASE_ADDR_MDIO+0)) w $((1*$TN_RGMII_WRITE+ 0*$TN_RGMII_PHY+$reg*$TN_RGMII_REG+$write_data))
  TNbar1 $(($TN_BASE_ADDR_MDIO+0)) w $((1*$TN_RGMII_WRITE+ 1*$TN_RGMII_PHY+$reg*$TN_RGMII_REG+$write_data))
elif [[ $1 == 3 ]]; then
  echo "Setting all PHYs to disabled auto-downspeed, 25MHz out, active-low SIGDET input, disabled sticky-bit handling, default ADC resolution, disabled power-consumption scaling based on link quality, disabled auto-negotiation power down, disabled EEE (MDIO.PHYCTL2 register)"
  let reg=14
  let write_data=0x0000
  TNbar1 $(($TN_BASE_ADDR_MDIO+0)) w $((1*$TN_RGMII_WRITE+16*$TN_RGMII_PHY+$reg*$TN_RGMII_REG+$write_data))
  TNbar1 $(($TN_BASE_ADDR_MDIO+0)) w $((1*$TN_RGMII_WRITE+17*$TN_RGMII_PHY+$reg*$TN_RGMII_REG+$write_data))
  TNbar1 $(($TN_BASE_ADDR_MDIO+0)) w $((1*$TN_RGMII_WRITE+18*$TN_RGMII_PHY+$reg*$TN_RGMII_REG+$write_data))
  TNbar1 $(($TN_BASE_ADDR_MDIO+0)) w $((1*$TN_RGMII_WRITE+19*$TN_RGMII_PHY+$reg*$TN_RGMII_REG+$write_data))
  TNbar1 $(($TN_BASE_ADDR_MDIO+0)) w $((1*$TN_RGMII_WRITE+20*$TN_RGMII_PHY+$reg*$TN_RGMII_REG+$write_data))
  TNbar1 $(($TN_BASE_ADDR_MDIO+0)) w $((1*$TN_RGMII_WRITE+21*$TN_RGMII_PHY+$reg*$TN_RGMII_REG+$write_data))
  TNbar1 $(($TN_BASE_ADDR_MDIO+0)) w $((1*$TN_RGMII_WRITE+22*$TN_RGMII_PHY+$reg*$TN_RGMII_REG+$write_data))
  TNbar1 $(($TN_BASE_ADDR_MDIO+0)) w $((1*$TN_RGMII_WRITE+23*$TN_RGMII_PHY+$reg*$TN_RGMII_REG+$write_data))
  TNbar1 $(($TN_BASE_ADDR_MDIO+0)) w $((1*$TN_RGMII_WRITE+24*$TN_RGMII_PHY+$reg*$TN_RGMII_REG+$write_data))
  TNbar1 $(($TN_BASE_ADDR_MDIO+0)) w $((1*$TN_RGMII_WRITE+25*$TN_RGMII_PHY+$reg*$TN_RGMII_REG+$write_data))
  TNbar1 $(($TN_BASE_ADDR_MDIO+0)) w $((1*$TN_RGMII_WRITE+ 0*$TN_RGMII_PHY+$reg*$TN_RGMII_REG+$write_data))
  TNbar1 $(($TN_BASE_ADDR_MDIO+0)) w $((1*$TN_RGMII_WRITE+ 1*$TN_RGMII_PHY+$reg*$TN_RGMII_REG+$write_data))
elif [[ $1 == 4 ]]; then
  echo "Setting all PHYs to RXCLK is active on link down, RXSKEW 0, 2V5, TXSKEW 0, CRS=TXRX_RX, copper RGMII mode (MDIO.MMICTRL register)"
  let reg=17
  let write_data=0x8800
  TNbar1 $(($TN_BASE_ADDR_MDIO+0)) w $((1*$TN_RGMII_WRITE+16*$TN_RGMII_PHY+$reg*$TN_RGMII_REG+$write_data))
  TNbar1 $(($TN_BASE_ADDR_MDIO+0)) w $((1*$TN_RGMII_WRITE+17*$TN_RGMII_PHY+$reg*$TN_RGMII_REG+$write_data))
  TNbar1 $(($TN_BASE_ADDR_MDIO+0)) w $((1*$TN_RGMII_WRITE+18*$TN_RGMII_PHY+$reg*$TN_RGMII_REG+$write_data))
  TNbar1 $(($TN_BASE_ADDR_MDIO+0)) w $((1*$TN_RGMII_WRITE+19*$TN_RGMII_PHY+$reg*$TN_RGMII_REG+$write_data))
  TNbar1 $(($TN_BASE_ADDR_MDIO+0)) w $((1*$TN_RGMII_WRITE+20*$TN_RGMII_PHY+$reg*$TN_RGMII_REG+$write_data))
  TNbar1 $(($TN_BASE_ADDR_MDIO+0)) w $((1*$TN_RGMII_WRITE+21*$TN_RGMII_PHY+$reg*$TN_RGMII_REG+$write_data))
  TNbar1 $(($TN_BASE_ADDR_MDIO+0)) w $((1*$TN_RGMII_WRITE+22*$TN_RGMII_PHY+$reg*$TN_RGMII_REG+$write_data))
  TNbar1 $(($TN_BASE_ADDR_MDIO+0)) w $((1*$TN_RGMII_WRITE+23*$TN_RGMII_PHY+$reg*$TN_RGMII_REG+$write_data))
  TNbar1 $(($TN_BASE_ADDR_MDIO+0)) w $((1*$TN_RGMII_WRITE+24*$TN_RGMII_PHY+$reg*$TN_RGMII_REG+$write_data))
  TNbar1 $(($TN_BASE_ADDR_MDIO+0)) w $((1*$TN_RGMII_WRITE+25*$TN_RGMII_PHY+$reg*$TN_RGMII_REG+$write_data))
  TNbar1 $(($TN_BASE_ADDR_MDIO+0)) w $((1*$TN_RGMII_WRITE+ 0*$TN_RGMII_PHY+$reg*$TN_RGMII_REG+$write_data))
  TNbar1 $(($TN_BASE_ADDR_MDIO+0)) w $((1*$TN_RGMII_WRITE+ 1*$TN_RGMII_PHY+$reg*$TN_RGMII_REG+$write_data))
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
  TNbar1 $(($TN_BASE_ADDR_MDIO+0)) w $((1*$TN_RGMII_WRITE+16*$TN_RGMII_PHY+$reg*$TN_RGMII_REG+$write_data))
  sleep 1
  TNbar1 $(($TN_BASE_ADDR_MDIO+0)) w $((0*$TN_RGMII_WRITE+16*$TN_RGMII_PHY+$reg*$TN_RGMII_REG+$write_data)) # Execute Read
  TNbar1 $(($TN_BASE_ADDR_MDIO+4)) # Get Read Data -> counter is the lower byte of the result
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
  TNbar1 $(($TN_BASE_ADDR_MDIO+0)) w $((1*$TN_RGMII_WRITE+17*$TN_RGMII_PHY+$reg*$TN_RGMII_REG+$write_data))
  sleep 1
  TNbar1 $(($TN_BASE_ADDR_MDIO+0)) w $((0*$TN_RGMII_WRITE+17*$TN_RGMII_PHY+$reg*$TN_RGMII_REG+$write_data)) # Execute Read
  TNbar1 $(($TN_BASE_ADDR_MDIO+4)) # Get Read Data -> counter is the lower byte of the result
  
### The following is not fully parameterized
  
elif [[ $1 == 7 ]]; then
  echo "GPHY0 LED tests (on-off)"
  TNbar1 $(($TN_BASE_ADDR_MDIO+4))              # Read -> check if 0x80000000 is set, then wait <to be done before every access!>
  TNbar1 $(($TN_BASE_ADDR_MDIO+0)) w 0x061B000F # write to first GPHY, Reg. 1B, Val. 0x000F
  sleep 1
  TNbar1 $(($TN_BASE_ADDR_MDIO+0)) w 0x061B0000 # write to first GPHY, Reg. 1B, Val. 0x0000
  TNbar1 $(($TN_BASE_ADDR_MDIO+0)) w 0x02020000 # read from first GPHY, Reg. 1B
  TNbar1 $(($TN_BASE_ADDR_MDIO+4))              # return read data (+ rest)
elif [[ $1 == 8 ]]; then
  echo "GPHY1 LED tests (on-off)"
  TNbar1 $(($TN_BASE_ADDR_MDIO+4))              # Read -> check if 0x80000000 is set, then wait <to be done before every access!>
  TNbar1 $(($TN_BASE_ADDR_MDIO+0)) w 0x063B000F # write to second GPHY, Reg. 1B, Val. 0x000F
  sleep 1
  TNbar1 $(($TN_BASE_ADDR_MDIO+0)) w 0x063B0000 # write to second GPHY, Reg. 1B, Val. 0x0000
  TNbar1 $(($TN_BASE_ADDR_MDIO+0)) w 0x02220000 # read from second GPHY, Reg. 1B -> 10001000100000000000000000
  TNbar1 $(($TN_BASE_ADDR_MDIO+4))              # return read data (+ rest)
elif [[ $1 == 9 ]]; then
  echo "Alaska0 LED test (blinking)"
  let write_data=0x0000
  TNbar1 $(($TN_BASE_ADDR_MDIO+0)) w 0x04160003 # change page in first Alaska, Reg. 22, Page 3
  TNbar1 $(($TN_BASE_ADDR_MDIO+0)) w 0x0410FBBB # write to first Alaska, Reg. 16, Page 3, Value 0xFBBB
  TNbar1 $(($TN_BASE_ADDR_MDIO+0)) w 0x00160000 # read from first Alaska, Reg. 16, Page 3
  TNbar1 $(($TN_BASE_ADDR_MDIO+4))              # return read data (+ rest)
else
  echo "Illegal parameter given"
fi
