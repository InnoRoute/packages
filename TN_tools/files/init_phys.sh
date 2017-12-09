#!/bin/bash

# TODO: Add busy checks at address: MSB at "C_BASE_ADDR_MDIO*$C_BASE_ADDR_FACTOR+4" should be 0!

source /usr/share/InnoRoute/tn_env.sh

echo "Initializing both Alaska PHYs via MDIO"

# INIT both alaskas: Page 2, Reg 21 to 0x1066
#  * first change page in first Alaska, Reg. 22, Page 2
#  * then write to Page 2, Reg. 21, Value 0x1066
let reg=22
let write_data=0x0002
TNbar1 $(($C_BASE_ADDR_MDIO*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_MDIO_WRITE)) w $(($TN_RGMII_WRITE+0*$TN_RGMII_PHY+$reg*$TN_RGMII_REG+$write_data)) > /dev/null
TNbar1 $(($C_BASE_ADDR_MDIO*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_MDIO_WRITE)) w $(($TN_RGMII_WRITE+1*$TN_RGMII_PHY+$reg*$TN_RGMII_REG+$write_data)) > /dev/null
let reg=21
let write_data=0x1066
TNbar1 $(($C_BASE_ADDR_MDIO*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_MDIO_WRITE)) w $(($TN_RGMII_WRITE+0*$TN_RGMII_PHY+$reg*$TN_RGMII_REG+$write_data)) > /dev/null
TNbar1 $(($C_BASE_ADDR_MDIO*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_MDIO_WRITE)) w $(($TN_RGMII_WRITE+1*$TN_RGMII_PHY+$reg*$TN_RGMII_REG+$write_data)) > /dev/null

# Alaska Page 0, Reg 18: enable interrupt at speed/duplex/link state change
let reg=22
let write_data=0x0000
TNbar1 $(($C_BASE_ADDR_MDIO*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_MDIO_WRITE)) w $(($TN_RGMII_WRITE+0*$TN_RGMII_PHY+$reg*$TN_RGMII_REG+$write_data)) > /dev/null
TNbar1 $(($C_BASE_ADDR_MDIO*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_MDIO_WRITE)) w $(($TN_RGMII_WRITE+1*$TN_RGMII_PHY+$reg*$TN_RGMII_REG+$write_data)) > /dev/null
let reg=18
let write_data=0x6400                                                 
TNbar1 $(($C_BASE_ADDR_MDIO*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_MDIO_WRITE)) w $(($TN_RGMII_WRITE+0*$TN_RGMII_PHY+$reg*$TN_RGMII_REG+$write_data)) > /dev/null
TNbar1 $(($C_BASE_ADDR_MDIO*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_MDIO_WRITE)) w $(($TN_RGMII_WRITE+1*$TN_RGMII_PHY+$reg*$TN_RGMII_REG+$write_data)) > /dev/null

# GPHY Init: enable interrupt at speed/duplex/link state change
let reg=25
let write_data=0x0007
TNbar1 $(($C_BASE_ADDR_MDIO*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_MDIO_WRITE)) w $((1*$TN_RGMII_WRITE+16*$TN_RGMII_PHY+$reg*$TN_RGMII_REG+$write_data)) > /dev/null
TNbar1 $(($C_BASE_ADDR_MDIO*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_MDIO_WRITE)) w $((1*$TN_RGMII_WRITE+17*$TN_RGMII_PHY+$reg*$TN_RGMII_REG+$write_data)) > /dev/null
TNbar1 $(($C_BASE_ADDR_MDIO*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_MDIO_WRITE)) w $((1*$TN_RGMII_WRITE+18*$TN_RGMII_PHY+$reg*$TN_RGMII_REG+$write_data)) > /dev/null
TNbar1 $(($C_BASE_ADDR_MDIO*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_MDIO_WRITE)) w $((1*$TN_RGMII_WRITE+19*$TN_RGMII_PHY+$reg*$TN_RGMII_REG+$write_data)) > /dev/null
TNbar1 $(($C_BASE_ADDR_MDIO*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_MDIO_WRITE)) w $((1*$TN_RGMII_WRITE+20*$TN_RGMII_PHY+$reg*$TN_RGMII_REG+$write_data)) > /dev/null
TNbar1 $(($C_BASE_ADDR_MDIO*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_MDIO_WRITE)) w $((1*$TN_RGMII_WRITE+21*$TN_RGMII_PHY+$reg*$TN_RGMII_REG+$write_data)) > /dev/null
TNbar1 $(($C_BASE_ADDR_MDIO*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_MDIO_WRITE)) w $((1*$TN_RGMII_WRITE+22*$TN_RGMII_PHY+$reg*$TN_RGMII_REG+$write_data)) > /dev/null
TNbar1 $(($C_BASE_ADDR_MDIO*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_MDIO_WRITE)) w $((1*$TN_RGMII_WRITE+23*$TN_RGMII_PHY+$reg*$TN_RGMII_REG+$write_data)) > /dev/null
TNbar1 $(($C_BASE_ADDR_MDIO*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_MDIO_WRITE)) w $((1*$TN_RGMII_WRITE+24*$TN_RGMII_PHY+$reg*$TN_RGMII_REG+$write_data)) > /dev/null
TNbar1 $(($C_BASE_ADDR_MDIO*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_MDIO_WRITE)) w $((1*$TN_RGMII_WRITE+25*$TN_RGMII_PHY+$reg*$TN_RGMII_REG+$write_data)) > /dev/null

echo "Setting all PHYs to advertise all technologies (MDIO.AN_ADV register)"
let reg=4
let write_data=0x0FE1
TNbar1 $(($C_BASE_ADDR_MDIO*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_MDIO_WRITE)) w $((1*$TN_RGMII_WRITE+16*$TN_RGMII_PHY+$reg*$TN_RGMII_REG+$write_data)) > /dev/null
TNbar1 $(($C_BASE_ADDR_MDIO*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_MDIO_WRITE)) w $((1*$TN_RGMII_WRITE+17*$TN_RGMII_PHY+$reg*$TN_RGMII_REG+$write_data)) > /dev/null
TNbar1 $(($C_BASE_ADDR_MDIO*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_MDIO_WRITE)) w $((1*$TN_RGMII_WRITE+18*$TN_RGMII_PHY+$reg*$TN_RGMII_REG+$write_data)) > /dev/null
TNbar1 $(($C_BASE_ADDR_MDIO*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_MDIO_WRITE)) w $((1*$TN_RGMII_WRITE+19*$TN_RGMII_PHY+$reg*$TN_RGMII_REG+$write_data)) > /dev/null
TNbar1 $(($C_BASE_ADDR_MDIO*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_MDIO_WRITE)) w $((1*$TN_RGMII_WRITE+20*$TN_RGMII_PHY+$reg*$TN_RGMII_REG+$write_data)) > /dev/null
TNbar1 $(($C_BASE_ADDR_MDIO*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_MDIO_WRITE)) w $((1*$TN_RGMII_WRITE+21*$TN_RGMII_PHY+$reg*$TN_RGMII_REG+$write_data)) > /dev/null
TNbar1 $(($C_BASE_ADDR_MDIO*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_MDIO_WRITE)) w $((1*$TN_RGMII_WRITE+22*$TN_RGMII_PHY+$reg*$TN_RGMII_REG+$write_data)) > /dev/null
TNbar1 $(($C_BASE_ADDR_MDIO*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_MDIO_WRITE)) w $((1*$TN_RGMII_WRITE+23*$TN_RGMII_PHY+$reg*$TN_RGMII_REG+$write_data)) > /dev/null
TNbar1 $(($C_BASE_ADDR_MDIO*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_MDIO_WRITE)) w $((1*$TN_RGMII_WRITE+24*$TN_RGMII_PHY+$reg*$TN_RGMII_REG+$write_data)) > /dev/null
TNbar1 $(($C_BASE_ADDR_MDIO*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_MDIO_WRITE)) w $((1*$TN_RGMII_WRITE+25*$TN_RGMII_PHY+$reg*$TN_RGMII_REG+$write_data)) > /dev/null
let write_data=0x0FE1
TNbar1 $(($C_BASE_ADDR_MDIO*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_MDIO_WRITE)) w $((1*$TN_RGMII_WRITE+0*$TN_RGMII_PHY+$reg*$TN_RGMII_REG+$write_data)) > /dev/null
TNbar1 $(($C_BASE_ADDR_MDIO*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_MDIO_WRITE)) w $((1*$TN_RGMII_WRITE+1*$TN_RGMII_PHY+$reg*$TN_RGMII_REG+$write_data)) > /dev/null

echo "Setting all PHYs to auto-negotiation and restarting it (MDIO.CTRL register)"
let reg=0
let write_data=0x1200
TNbar1 $(($C_BASE_ADDR_MDIO*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_MDIO_WRITE)) w $((1*$TN_RGMII_WRITE+16*$TN_RGMII_PHY+$reg*$TN_RGMII_REG+$write_data)) > /dev/null
TNbar1 $(($C_BASE_ADDR_MDIO*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_MDIO_WRITE)) w $((1*$TN_RGMII_WRITE+17*$TN_RGMII_PHY+$reg*$TN_RGMII_REG+$write_data)) > /dev/null
TNbar1 $(($C_BASE_ADDR_MDIO*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_MDIO_WRITE)) w $((1*$TN_RGMII_WRITE+18*$TN_RGMII_PHY+$reg*$TN_RGMII_REG+$write_data)) > /dev/null
TNbar1 $(($C_BASE_ADDR_MDIO*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_MDIO_WRITE)) w $((1*$TN_RGMII_WRITE+19*$TN_RGMII_PHY+$reg*$TN_RGMII_REG+$write_data)) > /dev/null
TNbar1 $(($C_BASE_ADDR_MDIO*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_MDIO_WRITE)) w $((1*$TN_RGMII_WRITE+20*$TN_RGMII_PHY+$reg*$TN_RGMII_REG+$write_data)) > /dev/null
TNbar1 $(($C_BASE_ADDR_MDIO*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_MDIO_WRITE)) w $((1*$TN_RGMII_WRITE+21*$TN_RGMII_PHY+$reg*$TN_RGMII_REG+$write_data)) > /dev/null
TNbar1 $(($C_BASE_ADDR_MDIO*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_MDIO_WRITE)) w $((1*$TN_RGMII_WRITE+22*$TN_RGMII_PHY+$reg*$TN_RGMII_REG+$write_data)) > /dev/null
TNbar1 $(($C_BASE_ADDR_MDIO*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_MDIO_WRITE)) w $((1*$TN_RGMII_WRITE+23*$TN_RGMII_PHY+$reg*$TN_RGMII_REG+$write_data)) > /dev/null
TNbar1 $(($C_BASE_ADDR_MDIO*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_MDIO_WRITE)) w $((1*$TN_RGMII_WRITE+24*$TN_RGMII_PHY+$reg*$TN_RGMII_REG+$write_data)) > /dev/null
TNbar1 $(($C_BASE_ADDR_MDIO*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_MDIO_WRITE)) w $((1*$TN_RGMII_WRITE+25*$TN_RGMII_PHY+$reg*$TN_RGMII_REG+$write_data)) > /dev/null
let write_data=0x1200
TNbar1 $(($C_BASE_ADDR_MDIO*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_MDIO_WRITE)) w $((1*$TN_RGMII_WRITE+0*$TN_RGMII_PHY+$reg*$TN_RGMII_REG+$write_data)) > /dev/null
TNbar1 $(($C_BASE_ADDR_MDIO*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_MDIO_WRITE)) w $((1*$TN_RGMII_WRITE+1*$TN_RGMII_PHY+$reg*$TN_RGMII_REG+$write_data)) > /dev/null

# Add GPHY RX/TX Skew Adjustment for the RGMIIs in the MII2CTRL register (1F.01EC)
# Add Alaska RX Skew Adjustment for the RGMIIs in the register (21_2.5)
