#!/bin/bash

# TODO: Add busy checks at address: MSB at "TN_BASE_ADDR_MDIO+4" should be 0!

source /usr/share/InnoRoute/tn_env.sh

echo "Initializing both Alaska PHYs via MDIO"

# INIT both alaskas: Page 2, Reg 21 to 0x1066
#  * first change page in first Alaska, Reg. 22, Page 2
#  * then write to Page 2, Reg. 21, Value 0x1066
let write_data=0x0002
TNbar1 $(($TN_BASE_ADDR_MDIO+0)) w $(($TN_RGMII_WRITE+0*$TN_RGMII_PHY+22*$TN_RGMII_REG+$write_data))
TNbar1 $(($TN_BASE_ADDR_MDIO+0)) w $(($TN_RGMII_WRITE+1*$TN_RGMII_PHY+22*$TN_RGMII_REG+$write_data))
let write_data=0x1066                                                 
TNbar1 $(($TN_BASE_ADDR_MDIO+0)) w $(($TN_RGMII_WRITE+0*$TN_RGMII_PHY+21*$TN_RGMII_REG+$write_data))
TNbar1 $(($TN_BASE_ADDR_MDIO+0)) w $(($TN_RGMII_WRITE+1*$TN_RGMII_PHY+21*$TN_RGMII_REG+$write_data))
