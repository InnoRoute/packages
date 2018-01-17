#!/bin/bash

# TODO: Add busy checks at address: MSB at "C_BASE_ADDR_MDIO*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_MDIO_READ" should be 0!

source /usr/share/InnoRoute/tn_env.sh

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

read_alaska()
{
  # constant
  let page_reg=22

  # Set page
  TNbar1 $(($C_BASE_ADDR_MDIO*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_MDIO_WRITE)) w $((1*$TN_RGMII_WRITE+$alaska*$TN_RGMII_PHY+$page_reg*$TN_RGMII_REG+$page)) > /dev/null
  # Execute read
  TNbar1 $(($C_BASE_ADDR_MDIO*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_MDIO_WRITE)) w $((0*$TN_RGMII_WRITE+$alaska*$TN_RGMII_PHY+$reg*$TN_RGMII_REG+$write_data)) > /dev/null
  # Check for completion
  let read_data=`TNbar1 $(($C_BASE_ADDR_MDIO*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_MDIO_READ)) | cut -d " " -f 6`
  if [[ $read_data -eq 0xEEEEEEEE ]]; then
    echo " ** MMI Read Timeout **"
  fi
}

read_gphy()
{
  # Execute read
  TNbar1 $(($C_BASE_ADDR_MDIO*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_MDIO_WRITE)) w $((0*$TN_RGMII_WRITE+($gphy+16)*$TN_RGMII_PHY+$reg*$TN_RGMII_REG+$write_data)) > /dev/null
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

rm /tmp/gphy0.txt
rm /tmp/gphy1.txt
rm /tmp/gphy2.txt
rm /tmp/gphy3.txt
rm /tmp/gphy4.txt
rm /tmp/gphy5.txt
rm /tmp/gphy6.txt
rm /tmp/gphy7.txt
rm /tmp/gphy8.txt
rm /tmp/gphy9.txt
rm /tmp/gphy_mmd0.txt
rm /tmp/gphy_mmd1.txt
rm /tmp/gphy_mmd2.txt
rm /tmp/gphy_mmd3.txt
rm /tmp/gphy_mmd4.txt
rm /tmp/gphy_mmd5.txt
rm /tmp/gphy_mmd6.txt
rm /tmp/gphy_mmd7.txt
rm /tmp/gphy_mmd8.txt
rm /tmp/gphy_mmd9.txt
rm /tmp/alaska0.txt
rm /tmp/alaska1.txt

let page=0
let write_data=0x0000
for reg in `seq 0 31`; do
  for gphy   in `seq 0 9`; do read_gphy;   read_mdio_result; printf "Reg0x%02x:%08x\n" $reg $read_data | cut -c1-8,13-16 >> /tmp/gphy$gphy.txt; done
  for alaska in `seq 0 1`; do read_alaska; read_mdio_result; printf "Reg0x%02x:%08x\n" $reg $read_data | cut -c1-8,13-16 >> /tmp/alaska$alaska.txt;done
done

echo "Differences between GPHYs:"
for gphy in `seq 1 9`; do
  echo -e "GPHY0 vs. GPHY$gphy"
  diff -y --suppress-common-lines /tmp/gphy0.txt /tmp/gphy$gphy.txt
done

echo "Differences between Alaskas:"
echo "Alaska0 vs. Alaska1"
diff -y --suppress-common-lines /tmp/alaska0.txt /tmp/alaska1.txt

exit
########## MMD registers are not checked at this point ##########


# In addition to the STD registers, also the MMD registers might be read/set
#  -> GPHY: EEE, EEE-AN, EEPROM, LEDs, EEE Link-Fail Counters, WoL
# 1. Write (0x03/0x07/0x1E or) 0x1F to reg 0x0d (MMD Ctrl reg) - Address Type
# 2. Write register number to reg 0x0e (MMD Data reg)
# 3. Write, e.g., 0x401f to reg 0x0d (MMD Ctrl reg) - Data Type
# 4. Write or read reg 0x0e (MMD Data reg)
# Alaskas: Page select through register 0x16 (which is the EEPROM control register in the GPHY)

for phy in `seq 0 9`; do
  for mmdreg in 0x03,0x0000 \
                0x03,0x0001 \
                0x03,0x0014 \
                0x03,0x0016 \
                0x03,0x003C \
                0x03,0x003D \
                0x1E,0x0000 \
                0x1F,0x01E0 \
                0x1F,0x01E1 \
                0x1F,0x01E2 \
                0x1F,0x01E3 \
                0x1F,0x01E4 \
                0x1F,0x01E5 \
                0x1F,0x01E6 \
                0x1F,0x01E7 \
                0x1F,0x01EA \
                0x1F,0x01EB \
                0x1F,0x01EC \
                0x1F,0x01ED \
                0x1F,0x01EE \
                0x1F,0x0781 \
                0x1F,0x0783 \
                0x1F,0x0784 \
                0x1F,0x0785 \
                0x1F,0x0786 \
                0x1F,0x0787 \
                0x1F,0x0788 \
                0x1F,0x0789 \
                0x1F,0x078A \
                0x1F,0x078B \
                0x1F,0x078C \
                0x1F,0x078D \
                0x1F,0x078E \
                0x1F,0x0EB5 \
                0x1F,0x0EB7; do
    IFS=','
    set -- $mmdreg
    let devaddr=$1
    let regno=$2

	#printf "PHY=%d: %02x.%04x\n" $phy $devaddr $regno
    # Write Address Type to reg 0x0d (MMD Ctrl reg)
    let reg=0x0D
    let write_data=0x0000+$devaddr
    TNbar1 $(($C_BASE_ADDR_MDIO*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_MDIO_WRITE)) w $((1*$TN_RGMII_WRITE+(16+$phy)*$TN_RGMII_PHY+$reg*$TN_RGMII_REG+$write_data)) > /dev/null;
    # Write register number to reg 0x0e (MMD Data reg)
    let reg=0x0E
    let write_data=$regno
    TNbar1 $(($C_BASE_ADDR_MDIO*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_MDIO_WRITE)) w $((1*$TN_RGMII_WRITE+(16+$phy)*$TN_RGMII_PHY+$reg*$TN_RGMII_REG+$write_data)) > /dev/null;
    # Write Data Type to reg 0x0d (MMD Ctrl reg)
    let reg=0x0D
    let write_data=0x4000+$devaddr
    TNbar1 $(($C_BASE_ADDR_MDIO*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_MDIO_WRITE)) w $((1*$TN_RGMII_WRITE+(16+$phy)*$TN_RGMII_PHY+$reg*$TN_RGMII_REG+$write_data)) > /dev/null;
    # Read reg 0x0e (MMD Data reg)
    let reg=0x0E
    TNbar1 $(($C_BASE_ADDR_MDIO*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_MDIO_WRITE)) w $((0*$TN_RGMII_WRITE+(16+$phy)*$TN_RGMII_PHY+$reg*$TN_RGMII_REG+$write_data)) > /dev/null;
    # Get read data
    let read_data=`TNbar1 $(($C_BASE_ADDR_MDIO*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_MDIO_READ)) | cut -d " " -f 6`
    if [[ $read_data -eq 0xEEEEEEEE ]]; then
      echo " ** MMI Read Timeout **"
    fi
    printf "Reg %02X.%04XH:0x%08x\n" $devaddr $regno $read_data | cut -c1-15,20-23 >> /tmp/gphy_mmd$phy.txt
  done
done

echo "Dumping GPHY MMD register differences:"

for gphy in `seq 1 9`; do
  echo -e "GPHY0 vs. GPHY$gphy"
  diff -y --suppress-common-lines /tmp/gphy_mmd0.txt /tmp/gphy_mmd$gphy.txt
done
