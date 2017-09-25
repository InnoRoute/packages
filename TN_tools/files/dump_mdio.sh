#!/bin/bash

# TODO: Add busy checks at address: MSB at "TN_BASE_ADDR_MDIO+4" should be 0!

source /usr/share/InnoRoute/tn_env.sh

echo "Reading all standard register values from all Ethernet PHYs"

rm /tmp/gphy0_mdio.txt
rm /tmp/gphy1_mdio.txt
rm /tmp/gphy2_mdio.txt
rm /tmp/gphy3_mdio.txt
rm /tmp/gphy4_mdio.txt
rm /tmp/gphy5_mdio.txt
rm /tmp/gphy6_mdio.txt
rm /tmp/gphy7_mdio.txt
rm /tmp/gphy8_mdio.txt
rm /tmp/gphy9_mdio.txt
rm /tmp/gphy0_mdio_mmd.txt
rm /tmp/gphy1_mdio_mmd.txt
rm /tmp/gphy2_mdio_mmd.txt
rm /tmp/gphy3_mdio_mmd.txt
rm /tmp/gphy4_mdio_mmd.txt
rm /tmp/gphy5_mdio_mmd.txt
rm /tmp/gphy6_mdio_mmd.txt
rm /tmp/gphy7_mdio_mmd.txt
rm /tmp/gphy8_mdio_mmd.txt
rm /tmp/gphy9_mdio_mmd.txt
rm /tmp/alaska0_mdio.txt
rm /tmp/alaska1_mdio.txt

let write_data=0x0000
for reg in `seq 0 31`; do
  TNbar1 $(($TN_BASE_ADDR_MDIO+0)) w $((0*$TN_RGMII_WRITE+16*$TN_RGMII_PHY+$reg*$TN_RGMII_REG+$write_data)) > /dev/null;
  printf "Reg0x%02x:%08x\n" $reg `TNbar1 $(($TN_BASE_ADDR_MDIO+4))| cut -d " " -f 6` | cut -c1-8,13-16 >> /tmp/gphy0_mdio.txt;
  TNbar1 $(($TN_BASE_ADDR_MDIO+0)) w $((0*$TN_RGMII_WRITE+17*$TN_RGMII_PHY+$reg*$TN_RGMII_REG+$write_data)) > /dev/null;
  printf "Reg0x%02x:%08x\n" $reg `TNbar1 $(($TN_BASE_ADDR_MDIO+4))| cut -d " " -f 6` | cut -c1-8,13-16 >> /tmp/gphy1_mdio.txt;
  TNbar1 $(($TN_BASE_ADDR_MDIO+0)) w $((0*$TN_RGMII_WRITE+18*$TN_RGMII_PHY+$reg*$TN_RGMII_REG+$write_data)) > /dev/null;
  printf "Reg0x%02x:%08x\n" $reg `TNbar1 $(($TN_BASE_ADDR_MDIO+4))| cut -d " " -f 6` | cut -c1-8,13-16 >> /tmp/gphy2_mdio.txt;
  TNbar1 $(($TN_BASE_ADDR_MDIO+0)) w $((0*$TN_RGMII_WRITE+19*$TN_RGMII_PHY+$reg*$TN_RGMII_REG+$write_data)) > /dev/null;
  printf "Reg0x%02x:%08x\n" $reg `TNbar1 $(($TN_BASE_ADDR_MDIO+4))| cut -d " " -f 6` | cut -c1-8,13-16 >> /tmp/gphy3_mdio.txt;
  TNbar1 $(($TN_BASE_ADDR_MDIO+0)) w $((0*$TN_RGMII_WRITE+20*$TN_RGMII_PHY+$reg*$TN_RGMII_REG+$write_data)) > /dev/null;
  printf "Reg0x%02x:%08x\n" $reg `TNbar1 $(($TN_BASE_ADDR_MDIO+4))| cut -d " " -f 6` | cut -c1-8,13-16 >> /tmp/gphy4_mdio.txt;
  TNbar1 $(($TN_BASE_ADDR_MDIO+0)) w $((0*$TN_RGMII_WRITE+21*$TN_RGMII_PHY+$reg*$TN_RGMII_REG+$write_data)) > /dev/null;
  printf "Reg0x%02x:%08x\n" $reg `TNbar1 $(($TN_BASE_ADDR_MDIO+4))| cut -d " " -f 6` | cut -c1-8,13-16 >> /tmp/gphy5_mdio.txt;
  TNbar1 $(($TN_BASE_ADDR_MDIO+0)) w $((0*$TN_RGMII_WRITE+22*$TN_RGMII_PHY+$reg*$TN_RGMII_REG+$write_data)) > /dev/null;
  printf "Reg0x%02x:%08x\n" $reg `TNbar1 $(($TN_BASE_ADDR_MDIO+4))| cut -d " " -f 6` | cut -c1-8,13-16 >> /tmp/gphy6_mdio.txt;
  TNbar1 $(($TN_BASE_ADDR_MDIO+0)) w $((0*$TN_RGMII_WRITE+23*$TN_RGMII_PHY+$reg*$TN_RGMII_REG+$write_data)) > /dev/null;
  printf "Reg0x%02x:%08x\n" $reg `TNbar1 $(($TN_BASE_ADDR_MDIO+4))| cut -d " " -f 6` | cut -c1-8,13-16 >> /tmp/gphy7_mdio.txt;
  TNbar1 $(($TN_BASE_ADDR_MDIO+0)) w $((0*$TN_RGMII_WRITE+24*$TN_RGMII_PHY+$reg*$TN_RGMII_REG+$write_data)) > /dev/null;
  printf "Reg0x%02x:%08x\n" $reg `TNbar1 $(($TN_BASE_ADDR_MDIO+4))| cut -d " " -f 6` | cut -c1-8,13-16 >> /tmp/gphy8_mdio.txt;
  TNbar1 $(($TN_BASE_ADDR_MDIO+0)) w $((0*$TN_RGMII_WRITE+25*$TN_RGMII_PHY+$reg*$TN_RGMII_REG+$write_data)) > /dev/null;
  printf "Reg0x%02x:%08x\n" $reg `TNbar1 $(($TN_BASE_ADDR_MDIO+4))| cut -d " " -f 6` | cut -c1-8,13-16 >> /tmp/gphy9_mdio.txt;

  TNbar1 $(($TN_BASE_ADDR_MDIO+0)) w $((0*$TN_RGMII_WRITE+0*$TN_RGMII_PHY+$reg*$TN_RGMII_REG+$write_data)) > /dev/null;
  printf "Reg0x%02x:%08x\n" $reg `TNbar1 $(($TN_BASE_ADDR_MDIO+4))| cut -d " " -f 6` | cut -c1-8,13-16 >> /tmp/alaska0_mdio.txt;
  TNbar1 $(($TN_BASE_ADDR_MDIO+0)) w $((0*$TN_RGMII_WRITE+1*$TN_RGMII_PHY+$reg*$TN_RGMII_REG+$write_data)) > /dev/null;
  printf "Reg0x%02x:%08x\n" $reg `TNbar1 $(($TN_BASE_ADDR_MDIO+4))| cut -d " " -f 6` | cut -c1-8,13-16 >> /tmp/alaska1_mdio.txt;
done
echo "Differences between GPHYs:"
echo "GPHY0 vs. GPHY1"
diff -y --suppress-common-lines /tmp/gphy0_mdio.txt /tmp/gphy1_mdio.txt
echo "GPHY0 vs. GPHY2"
diff -y --suppress-common-lines /tmp/gphy0_mdio.txt /tmp/gphy2_mdio.txt
echo "GPHY0 vs. GPHY3"
diff -y --suppress-common-lines /tmp/gphy0_mdio.txt /tmp/gphy3_mdio.txt
echo "GPHY0 vs. GPHY4"
diff -y --suppress-common-lines /tmp/gphy0_mdio.txt /tmp/gphy4_mdio.txt
echo "GPHY0 vs. GPHY5"
diff -y --suppress-common-lines /tmp/gphy0_mdio.txt /tmp/gphy5_mdio.txt
echo "GPHY0 vs. GPHY6"
diff -y --suppress-common-lines /tmp/gphy0_mdio.txt /tmp/gphy6_mdio.txt
echo "GPHY0 vs. GPHY7"
diff -y --suppress-common-lines /tmp/gphy0_mdio.txt /tmp/gphy7_mdio.txt
echo "GPHY0 vs. GPHY8"
diff -y --suppress-common-lines /tmp/gphy0_mdio.txt /tmp/gphy8_mdio.txt
echo "GPHY0 vs. GPHY9"
diff -y --suppress-common-lines /tmp/gphy0_mdio.txt /tmp/gphy9_mdio.txt

echo "Differences between Alaskas:"
echo "Alaska0 vs. Alaska1"
diff -y --suppress-common-lines /tmp/alaska0_mdio.txt /tmp/alaska1_mdio.txt

# In addition to the STD registers, also the MMD registers might be read/set
#  -> GPHY: EEE, EEE-AN, EEPROM, LEDs, EEE Link-Fail Counters, WoL
# 1. Write (0x03/0x07/0x1E or) 0x1F to reg 0x0d (MMD Ctrl reg) - Address Type
# 2. Write register number to reg 0x0e (MMD Data reg)
# 3. Write, e.g., 0x401f to reg 0x0d (MMD Ctrl reg) - Data Type
# 4. Write or read reg 0x0e (MMD Data reg)
# Alaskas: Page select through register 0x16 (which is the EEPROM control register in the GPHY)

exit

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
    TNbar1 $(($TN_BASE_ADDR_MDIO+0)) w $((1*$TN_RGMII_WRITE+(16+$phy)*$TN_RGMII_PHY+$reg*$TN_RGMII_REG+$write_data)) > /dev/null;
    # Write register number to reg 0x0e (MMD Data reg)
    let reg=0x0E
    let write_data=$regno
    TNbar1 $(($TN_BASE_ADDR_MDIO+0)) w $((1*$TN_RGMII_WRITE+(16+$phy)*$TN_RGMII_PHY+$reg*$TN_RGMII_REG+$write_data)) > /dev/null;
    # Write Data Type to reg 0x0d (MMD Ctrl reg)
    let reg=0x0D
    let write_data=0x4000+$devaddr
    TNbar1 $(($TN_BASE_ADDR_MDIO+0)) w $((1*$TN_RGMII_WRITE+(16+$phy)*$TN_RGMII_PHY+$reg*$TN_RGMII_REG+$write_data)) > /dev/null;
    # Read reg 0x0e (MMD Data reg)
    let reg=0x0E
    TNbar1 $(($TN_BASE_ADDR_MDIO+0)) w $((0*$TN_RGMII_WRITE+(16+$phy)*$TN_RGMII_PHY+$reg*$TN_RGMII_REG+$write_data)) > /dev/null;
    # Get read data
    printf "Reg %02X.%04XH:0x%08x\n" $devaddr $regno `TNbar1 $(($TN_BASE_ADDR_MDIO+4))| cut -d " " -f 6` | cut -c1-15,20-23 >> /tmp/gphy$(($phy))_mdio_mmd.txt;
  done
done

echo "Dumping GPHY MMD register differences:"

echo "GPHY0 vs. GPHY1"
diff -y --suppress-common-lines /tmp/gphy0_mdio_mmd.txt /tmp/gphy1_mdio_mmd.txt
echo "GPHY0 vs. GPHY2"
diff -y --suppress-common-lines /tmp/gphy0_mdio_mmd.txt /tmp/gphy2_mdio_mmd.txt
echo "GPHY0 vs. GPHY3"
diff -y --suppress-common-lines /tmp/gphy0_mdio_mmd.txt /tmp/gphy3_mdio_mmd.txt
echo "GPHY0 vs. GPHY4"
diff -y --suppress-common-lines /tmp/gphy0_mdio_mmd.txt /tmp/gphy4_mdio_mmd.txt
echo "GPHY0 vs. GPHY5"
diff -y --suppress-common-lines /tmp/gphy0_mdio_mmd.txt /tmp/gphy5_mdio_mmd.txt
echo "GPHY0 vs. GPHY6"
diff -y --suppress-common-lines /tmp/gphy0_mdio_mmd.txt /tmp/gphy6_mdio_mmd.txt
echo "GPHY0 vs. GPHY7"
diff -y --suppress-common-lines /tmp/gphy0_mdio_mmd.txt /tmp/gphy7_mdio_mmd.txt
echo "GPHY0 vs. GPHY8"
diff -y --suppress-common-lines /tmp/gphy0_mdio_mmd.txt /tmp/gphy8_mdio_mmd.txt
echo "GPHY0 vs. GPHY9"
diff -y --suppress-common-lines /tmp/gphy0_mdio_mmd.txt /tmp/gphy9_mdio_mmd.txt
