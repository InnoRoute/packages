#!/bin/bash

# TODO: Add busy checks at address: MSB at "C_BASE_ADDR_MDIO*$C_BASE_ADDR_FACTOR+4" should be 0!

source /usr/share/InnoRoute/tn_env.sh

echo "Initializing both types of PHYs via MDIO"

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

# Acknowledge operation by short blink
let reg=0x1B
let write_data=0x000F
for gphy in `seq 0 9`; do write_gphy; done
let write_data=0x0F00
for gphy in `seq 0 9`; do write_gphy; done

let page=3
let reg=16
let write_data=0x1AA9
for alaska in `seq 0 1`; do write_alaska; done
let write_data=0x1AA1
for alaska in `seq 0 1`; do write_alaska; done

### INIT all 10 GPHYs (register numbers in the manual are in hex)

# Auto-Negotiation Advertisement Register: Reg 0x04
# * Value 0xFE1: Setting all PHYs to advertise all technologies
let reg=0x04
let write_data=0x0FE1
for gphy in `seq 0 9`; do write_gphy; done

# Gigabit Control Register: Reg 0x09
# * Value 0x0300: Default Value
let reg=0x09
let write_data=0x0300
for gphy in `seq 0 9`; do write_gphy; done

# Physical Layer Control 1 Register: Reg 0x13
# * Value 0x0001: Default Value
let reg=0x13
let write_data=0x0001
for gphy in `seq 0 9`; do write_gphy; done

# Physical Layer Control 2 Register: Reg 0x14
# * Value 0x8006: Default Value
let reg=0x14
let write_data=0x8006
for gphy in `seq 0 9`; do write_gphy; done

# Media-Independent Interface Control Register: Reg 0x17
# * Value 0x8B00: Soft-strapping configured default value - Receive Timing Skew 0.0ns, Transmit Skew 1.5ns, 2.5V, Copper, RGMII
#let reg=0x17
#let write_data=0x8B00
#for gphy in `seq 0 9`; do write_gphy; done

# Interrupt Mask Register: Reg 0x19
# * Value 7: enable interrupt at speed/duplex/link state change
let reg=0x19
let write_data=0x0007
for gphy in `seq 0 9`; do write_gphy; done

# Control Register: Reg 0x00
# * Value 0x1200: Setting all PHYs to auto-negotiation and restarting it (MDIO.CTRL register)
let reg=0x00
let write_data=0x1200
for gphy in `seq 0 9`; do write_gphy; done

### INIT both alaskas (register 22 is the page register)

# Copper Auto-Negotiation Advertisement Register: Page 0, Reg 4
# * Value 0xFE1: Setting all PHYs to advertise all technologies
let page=0
let reg=4
let write_data=0x0FE1
for alaska in `seq 0 1`; do write_alaska; done

# Copper Specific Control Register 1: Page 0, Reg 16
# * Value 0x3060: Default value
let page=0
let reg=16
let write_data=0x3060
for alaska in `seq 0 1`; do write_alaska; done

# Copper Specific Interrupt Enable Register: Page 0, Reg 18
# * Value 0x6400: Enable interrupt at speed/duplex/link state change
let page=0
let reg=18
let write_data=0x6400
for alaska in `seq 0 1`; do write_alaska; done

# Copper Specific Control Register 2: Page 0, Reg 20
# * Value 0x0030: Accelerate 100BASE-T Link-Up
let page=0
let reg=20
let write_data=0x0030
for alaska in `seq 0 1`; do write_alaska; done

# Copper Specific Control Register 3: Page 0, Reg 26
# * Value 0x0240: Speed Up Gigabit Link Down Time (not IEEE compliant)
let page=0
let reg=26
let write_data=0x0240
for alaska in `seq 0 1`; do write_alaska; done

# MAC Specific Control Register 1: Page 2, Reg 16
# * Value 0x5C4D: Forward recovered 125 MHz clock only on link up and 100 or 1000Mbps
let page=2
let reg=16
let write_data=0x5C4D
for alaska in `seq 0 1`; do write_alaska; done

# MAC Specific Interrupt Enable Register: Page 2, Reg 18
# * Value 0x0080: Copper FIFO Over/Underflow Interrupt Enable
let page=2
let reg=18
let write_data=0x0080
for alaska in `seq 0 1`; do write_alaska; done

# MAC Specific Control Register 2 (RGMII): Page 2, Reg 21 to 0x1066
# * Value 0x1066: clock/data phase etc.
let page=2
let reg=21
let write_data=0x1066
for alaska in `seq 0 1`; do write_alaska; done

# General Control Register: Page 6, Reg 20
# * Value 0x1C0: Set PTP behaviour in General Control Register
let page=6
let reg=20
let write_data=0x01C0
for alaska in `seq 0 1`; do write_alaska; done

# Copper Control Register: Page 0, Reg 0
# * Value 0x1200: Setting all PHYs to auto-negotiation and restarting it
let page=0
let reg=0
let write_data=0x1200
for alaska in `seq 0 1`; do write_alaska; done

echo "Done"

#@todo Add GPHY RX/TX Skew Adjustment for the RGMIIs in the MII2CTRL register (1F.01EC)
#@todo Add Alaska RX Skew Adjustment for the RGMIIs in the register (21_2.5)
