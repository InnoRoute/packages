#!/bin/bash

source /usr/share/InnoRoute/tn_env.sh
source /usr/share/InnoRoute/tn_func_ll.sh

if [[ $# == 0 ]]; then
  echo "$0 <example> is used to execute a predefined MDIO access example"
  echo "Parameter <example> can be between 0 and 1"
  echo "  0:  all PHYs to auto-negotiation and restarting auto-negotiation"
  echo "  1:  all PHYs to disabled transceiver test mode, manual master/slave configuration, ..."
  echo "  2:  all PHYs to disabled test loops, transmitter enabled, transmitter adjustment level of 0, no polarity inversion, no MDI-X mode, disabled 10BASE-Te amplitude, manual MDI/MDI-X"
  echo "  3:  all PHYs to disabled auto-downspeed, 25MHz out, active-low SIGDET input, disabled sticky-bit handling, default ADC resolution, disabled power-consumption scaling based on link quality, disabled auto-negotiation power down, disabled EEE"
  echo "  4:  all PHYs to RXCLK is active on link down, RXSKEW 0, 2V5, TXSKEW 0, CRS=TXRX_RX, copper RGMII mode"
  echo "  5:  read GPHY0 counters"
  echo "  6:  read GPHY1 counters"
  echo "  7:  GPHY0 LED tests (on-off)"
  echo "  8:  GPHY1 LED tests (on-off)"
  echo "  9:  Alaska0 LED test (blinking)"
  echo "  10: Soft-resetting all PHYs"
else

  # TODO: Add busy checks at address: MSB at "C_BASE_ADDR_MDIO*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_MDIO_READ" should be 0!
  
  echo "Executing an MDIO example, expecting one parameter"
  
  if [[ $1 == 0 ]]; then
    echo "Setting all PHYs to auto-negotiation and restarting it (MDIO.CTRL register)"
    let page=0
    let reg=0
    let write_data=0x1200
    for phy in `seq 0 11`; do
      tn_ll_write_phy $phy $page $reg $write_data;
    done
    echo "Done"
  elif [[ $1 == 1 ]]; then
    echo "Setting all PHYs to disabled transceiver test mode,"
	echo "manual master/slave configuration, ... (MDIO.GCTRL register)"
    let page=0
    let reg=9
    let write_data=0x1A00
    let phy=0
    tn_ll_write_phy $phy $page $reg $write_data
    let write_data=0x1200
    let phy=1
    tn_ll_write_phy $phy $page $reg $write_data
    let write_data=0x1A00
    let phy=2
    tn_ll_write_phy $phy $page $reg $write_data
    let write_data=0x1200
    let phy=3
    tn_ll_write_phy $phy $page $reg $write_data
    let write_data=0x1A00
    let phy=4
    tn_ll_write_phy $phy $page $reg $write_data
    let write_data=0x1200
    let phy=5
    tn_ll_write_phy $phy $page $reg $write_data
    let write_data=0x1A00
    let phy=6
    tn_ll_write_phy $phy $page $reg $write_data
    let write_data=0x1200
    let phy=7
    tn_ll_write_phy $phy $page $reg $write_data
    let write_data=0x1A00
    let phy=8
    tn_ll_write_phy $phy $page $reg $write_data
    let write_data=0x1200
    let phy=9
    tn_ll_write_phy $phy $page $reg $write_data
    let write_data=0x1A00
    let phy=10
    tn_ll_write_phy $phy $page $reg $write_data
    let write_data=0x1200
    let phy=11
    tn_ll_write_phy $phy $page $reg $write_data
    echo "Done"
  elif [[ $1 == 2 ]]; then
    echo "Setting all PHYs to disabled test loops, transmitter enabled,"
	echo "transmitter adjustment level of 0, no polarity inversion,"
	echo "no MDI-X mode, disabled 10BASE-Te amplitude, manual MDI/MDI-X"
	echo "(MDIO.PHYCTL1 register)"
    let page=0
    let reg=13
    let write_data=0x0001
    for phy in `seq 0 9`; do
      tn_ll_write_phy $phy $page $reg $write_data;
    done
    for phy in `seq 0 1`; do
      tn_ll_write_phy $phy $page $reg $write_data;
    done
    echo "Done"
  elif [[ $1 == 3 ]]; then
    echo "Setting all PHYs to disabled auto-downspeed, 25MHz out,"
	echo "active-low SIGDET input, disabled sticky-bit handling,"
	echo "default ADC resolution, disabled power-consumption scaling"
	echo "based on link quality, disabled auto-negotiation power down,"
	echo "disabled EEE (MDIO.PHYCTL2 register)"
    let page=0
    let reg=14
    let write_data=0x0000
    for phy in `seq 0 9`; do 
      tn_ll_write_phy $phy $page $reg $write_data;
    done
    for phy in `seq 10 11`; do
      tn_ll_write_phy $phy $page $reg $write_data;
    done
    echo "Done"
  elif [[ $1 == 4 ]]; then
    echo "Setting all PHYs to RXCLK is active on link down, RXSKEW 0,"
	echo "2V5, TXSKEW 0, CRS=TXRX_RX, copper RGMII mode (MDIO.MMICTRL register)"
    let page=0
    let reg=17
    let write_data=0x8800
    for phy in `seq 0 9`; do
      tn_ll_write_phy $phy $page $reg $write_data;
    done
    for phy in `seq 10 11`; do
      tn_ll_write_phy $phy $page $reg $write_data;
    done
    echo "Done"
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
    tn_ll_mmi_write $C_BASE_ADDR_MDIO $C_SUB_ADDR_MDIO_WRITE $((1*$TN_RGMII_WRITE+16*$TN_RGMII_PHY+($reg & 0x1F)*$TN_RGMII_REG+($write_data & 0xFFFF)))
    sleep 1
    tn_ll_read_mdio_result
    # Get Read Data -> counter is the lower byte of the result
    printf "Counter value = %d\n" $(($read_data & 0xFFFF))
    echo "Done"
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
    tn_ll_mmi_write $C_BASE_ADDR_MDIO $C_SUB_ADDR_MDIO_WRITE $((1*$TN_RGMII_WRITE+17*$TN_RGMII_PHY+($reg & 0x1F)*$TN_RGMII_REG+($write_data & 0xFFFF)))
    sleep 1
    tn_ll_read_mdio_result
    # Get Read Data -> counter is the lower byte of the result
    printf "Counter value = %d\n" $(($read_data & 0xFFFF))
   
  ### The following is not fully parameterized
    
    echo "Done"
  elif [[ $1 == 7 ]]; then
    echo "GPHY0 LED tests (on-off)"
    #read_mdio_result
    # check $read_data if 0x80000000 is set, then wait <to be done before every access!>
    tn_ll_mmi_write $C_BASE_ADDR_MDIO $C_SUB_ADDR_MDIO_WRITE 0x061B000F # write to first GPHY, Reg. 1B, Val. 0x000F
    sleep 1
    tn_ll_mmi_write $C_BASE_ADDR_MDIO $C_SUB_ADDR_MDIO_WRITE 0x061B0000 # write to first GPHY, Reg. 1B, Val. 0x0000
    tn_ll_mmi_write $C_BASE_ADDR_MDIO $C_SUB_ADDR_MDIO_WRITE 0x02020000 # read from first GPHY, Reg. 1B
    #read_mdio_result
    # check $read_data if 0x80000000 is set, then wait <to be done before every access!>
    echo "Done"
  elif [[ $1 == 8 ]]; then
    echo "GPHY1 LED tests (on-off)"
    #read_mdio_result
    # check $read_data if 0x80000000 is set, then wait <to be done before every access!>
    tn_ll_mmi_write $C_BASE_ADDR_MDIO $C_SUB_ADDR_MDIO_WRITE 0x063B000F # write to second GPHY, Reg. 1B, Val. 0x000F
    sleep 1
    tn_ll_mmi_write $C_BASE_ADDR_MDIO $C_SUB_ADDR_MDIO_WRITE 0x063B0000 # write to second GPHY, Reg. 1B, Val. 0x0000
    tn_ll_mmi_write $C_BASE_ADDR_MDIO $C_SUB_ADDR_MDIO_WRITE 0x02220000 # read from second GPHY, Reg. 1B -> 10001000100000000000000000
    #read_mdio_result
    # check $read_data if 0x80000000 is set, then wait <to be done before every access!>
    echo "Done"
  elif [[ $1 == 9 ]]; then
    echo "Alaska0 LED test (blinking)"
    let page=0
    let write_data=0x0000
    tn_ll_mmi_write $C_BASE_ADDR_MDIO $C_SUB_ADDR_MDIO_WRITE 0x04160003 # change page in first Alaska, Reg. 22, Page 3
    tn_ll_mmi_write $C_BASE_ADDR_MDIO $C_SUB_ADDR_MDIO_WRITE 0x0410FBBB # write to first Alaska, Reg. 16, Page 3, Value 0xFBBB
    tn_ll_mmi_write $C_BASE_ADDR_MDIO $C_SUB_ADDR_MDIO_WRITE 0x00160000 # read from first Alaska, Reg. 16, Page 3
    #read_mdio_result
    # check $read_data if 0x80000000 is set, then wait <to be done before every access!>
    echo "Done"
  elif [[ $1 == 10 ]]; then
    echo "Soft-resetting all PHYs (MDIO.CTRL register)"
    let page=0
    let reg=0
    let write_data=0x8000
    for phy in `seq 0 9`; do
      tn_ll_write_phy $phy $page $reg $write_data;
    done
    for phy in `seq 10 11`; do
      tn_ll_write_phy $phy $page $reg $write_data;
    done
    echo "Done"
  else
    echo "Illegal parameter given"
  fi
fi
