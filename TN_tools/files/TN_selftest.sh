#!/bin/bash

source /usr/share/InnoRoute/tn_env.sh

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
    printf " ** MMI Read Timeout ** read_alaska(Alaska %d, page %d, reg %d, write data %04x)\n" $alaska $page $reg $write_data
  fi
}

read_gphy()
{
  # Execute read
  TNbar1 $(($C_BASE_ADDR_MDIO*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_MDIO_WRITE)) w $((0*$TN_RGMII_WRITE+($gphy+16)*$TN_RGMII_PHY+$reg*$TN_RGMII_REG+$write_data)) > /dev/null
  # Check for completion
  let read_data=`TNbar1 $(($C_BASE_ADDR_MDIO*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_MDIO_READ)) | cut -d " " -f 6`
  if [[ $read_data -eq 0xEEEEEEEE ]]; then
    printf " ** MMI Read Timeout ** read_gphy(GPHY %d, reg %d, write data %04x)\n" $gphy $reg $write_data
  fi
}

read_mdio_result()
{
  let read_data=`TNbar1 $(($C_BASE_ADDR_MDIO*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_MDIO_READ)) | cut -d " " -f 6`
  if [[ $read_data -eq 0xEEEEEEEE ]]; then
    printf " ** MMI Read Timeout ** read_mdio_result()\n"
  fi
}

# Check System Controller response (at least PHY resets implemented, so version 10)
if [[ `i2cget -y 0 4 0x00` -lt 10 ]]; then
  echo "FAILED System Controller Version Check.";
  exit;
fi

# Check USB devices:
# lsusb (filtered): lsusb -d 0403:6010 -> Bus 001 Device 003: ID 0403:6010 Future Technology Devices International, Ltd FT2232C Dual USB-UART/FIFO IC

# Load specific bitstream, using
/usr/share/InnoRoute/TN_fpga_config_jtag.sh /usr/share/InnoRoute/tn_combined.bit
/usr/share/InnoRoute/TN_fpga_config_lpc.sh /usr/share/InnoRoute/tn_display_loopback.bin
/usr/share/InnoRoute/TN_fpga_config_opto.sh /usr/share/InnoRoute/tn_atom_ctrl.bin
/usr/sbin/TN_opto_prog /usr/share/InnoRoute/trustnode_top.bit # Add bitstream with all connections set to "2", which has the RW test register
#  each time checking the successful load count and bitstream error in SysCtrl

# lspci (filtered): lspci -n -s 01:00.0 -> 01:00.0 0200: 10ee:0000 (rev 02)

# PCIe BAR1 Test
for wr_data in 0xC0FFEE12 0x1C0FFEE2 0x12C0FFEE; do
  TNbar1 $(($C_BASE_ADDR_TN_COMMON_L*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_RW_TEST)) w $wr_data >> /dev/null
  let    wr_result=`TNbar1 $(($C_BASE_ADDR_TN_COMMON_L*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_RW_TEST)) w $wr_data | cut -d " " -f 6`
  if [[ $wr_result -eq 0xEEEEEEEE ]]; then
    echo " ** MMI Write Timeout -> FPGA Bitstream Test FAILED **";
    exit;
  fi
  if [[ $wr_result -ne $wr_data ]]; then
    printf " ** Bad MMI Write Data (0x%08x isn't 0x%08x) -> FPGA Bitstream Test FAILED **\n" $wr_result $wr_data;
    exit;
  fi
  TNbar1 $(($C_BASE_ADDR_TN_COMMON_L*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_RW_TEST)) >> /dev/null
  let    rd_result=`TNbar1 $(($C_BASE_ADDR_TN_COMMON_L*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_RW_TEST)) | cut -d " " -f 6`
  if [[ $rd_result -eq 0xEEEEEEEE ]]; then
    echo " ** MMI Read Timeout -> FPGA Bitstream Test FAILED **";
    exit;
  fi
  if [[ $rd_result -ne $wr_data ]]; then
    printf " ** Bad MMI Read Data (0x%08x isn't 0x%08x) -> FPGA Bitstream Test FAILED **\n" $rd_result $wr_data;
    exit;
  fi
done

# PCIe BAR0 test (all of this could also be done with phy loopback, at different speeds: 10/100/1000)
# Set network to silent mode
/usr/share/InnoRoute/TN_nw_silent.sh
# Set network port loopback (CPU->FPGA->CPU)
echo 1 > /proc/TrustNode/TN_send2cpu
# Get current packet counts
#######################################################################
# Send packets to FPGA:
tcpreplay -i TN0  /usr/share/InnoRoute/example_packet.pcapng
tcpreplay -i TN1  /usr/share/InnoRoute/example_packet.pcapng
tcpreplay -i TN2  /usr/share/InnoRoute/example_packet.pcapng
tcpreplay -i TN3  /usr/share/InnoRoute/example_packet.pcapng
tcpreplay -i TN4  /usr/share/InnoRoute/example_packet.pcapng
tcpreplay -i TN5  /usr/share/InnoRoute/example_packet.pcapng
tcpreplay -i TN6  /usr/share/InnoRoute/example_packet.pcapng
tcpreplay -i TN7  /usr/share/InnoRoute/example_packet.pcapng
tcpreplay -i TN8  /usr/share/InnoRoute/example_packet.pcapng
tcpreplay -i TN9  /usr/share/InnoRoute/example_packet.pcapng
tcpreplay -i TN10 /usr/share/InnoRoute/example_packet.pcapng
tcpreplay -i TN11 /usr/share/InnoRoute/example_packet.pcapng
tcpreplay -i TN12 /usr/share/InnoRoute/example_packet.pcapng
tcpreplay -i TN13 /usr/share/InnoRoute/example_packet.pcapng
tcpreplay -i TN14 /usr/share/InnoRoute/example_packet.pcapng
tcpreplay -i TN15 /usr/share/InnoRoute/example_packet.pcapng
# Check for received packets (delta) with timeout
#######################################################################
# Disable loopback:
echo 0 > /proc/TrustNode/TN_send2cpu

# Check response from all 12 PHYs via MDIO
let page=0
let reg=0
let write_data=0x0000
for gphy   in `seq 0 9`; do
  read_gphy;
  read_mdio_result;
  if [ `printf "%04x\n" $(( $read_data & 0xFFFF ))` != "1001" ]; then
    echo -e "MDIO FAIL - GPHY $gphy";
  fi;
done
for alaska in `seq 0 1`; do
  read_alaska;
  read_mdio_result;
  if [ `printf "%04x\n" $(( $read_data & 0xFFFF ))` != "1001" ]; then
    echo -e "MDIO FAIL - Alaska $alaska";
  fi;
done

# Reset PHYs and let user check all PHY LEDs

# Check PLL response:
pllctl 0x69 0x0000
# Program PLL using INRpllload, which uses pllctl:
echo "Programming PLL (volatile)"
INRpllload /usr/share/InnoRoute/TN-PLL-V1.2_synce.stp
echo "Programming done"

# Expect cables between any two physical ports,
#  or set PHY-based loopback (CPU->FPGA->PHY->FPGA->CPU):
/usr/share/InnoRoute/TN_phy_loopback.sh 1
# Optionaly: check RGMII status:
/usr/share/InnoRoute/TN_rgmii_status.sh
# Send packets to FPGA
# Check for received packets with timeout
# Optionally: disable loopback:
#/usr/share/InnoRoute/TN_phy_loopback.sh 0

# Determine optimal RGMII phase settings
#/usr/share/InnoRoute/TN_phy_loopback.sh 1
for phase in `seq 0 31`; do
  /usr/share/InnoRoute/TN_rgmii_phase.sh $phase $phase $phase $phase $phase $phase $phase $phase $phase $phase $phase $phase
  # Check counters
  # Send packet
  # Check counters (delta)
done
/usr/share/InnoRoute/TN_phy_loopback.sh 0

# Check ClkBuf-LOS'
if [[ $(( `i2cget -y 0 4 0x01` & 48 )) -ne 0 ]]; then
  echo "FAIL: at least one oscillator is down";
  exit
fi

# Also check PLL status:
/usr/share/InnoRoute/TN_pll_status.sh | grep "NOT"

# Let user check fully enlightened display
/usr/share/InnoRoute/TN_front_display.sh 0 0 0 0 0
sleep 1
/usr/share/InnoRoute/TN_front_display.sh 1
sleep 1
/usr/share/InnoRoute/TN_front_display.sh 2
sleep 1
/usr/share/InnoRoute/TN_front_display.sh 3
sleep 1
/usr/share/InnoRoute/TN_front_display.sh 0 0 0 0
sleep 1
/usr/share/InnoRoute/TN_front_display.sh
sleep 1
/usr/share/InnoRoute/TN_front_display.sh 0 0 0 0

# Let user check SyncE LEDs
sleep 1
# all off
/usr/share/InnoRoute/TN_led.sh 0
sleep 1
# both SyncE LEDs on
/usr/share/InnoRoute/TN_led.sh 768
sleep 1
# all off
/usr/share/InnoRoute/TN_led.sh 0
sleep 1
# both SyncE LEDs blinking
/usr/share/InnoRoute/TN_led.sh 1024

# Opened device: let user check user LEDs
sleep 1
# all off
/usr/share/InnoRoute/TN_led.sh 0
sleep 1
# all internal LEDs on
/usr/share/InnoRoute/TN_led.sh 255

# Let user check beep:
/usr/share/InnoRoute/TN_beep.sh
/usr/share/InnoRoute/TN_beep.sh
/usr/share/InnoRoute/TN_beep.sh

# Let user check FAN sounds:
/usr/share/InnoRoute/TN_fan.sh 63
sleep 1
/usr/share/InnoRoute/TN_fan.sh 20
# Also check for stuck fan and check fan speed for being in a reasonable range

# Let user press all 16 front buttons: read via MMI
/usr/share/InnoRoute/TN_buttons.sh

# Let user press user button on the back

# Check for interrupts (PLL, 2xAlaskas, GPHYs)

# Check error bitmaps (mmi_common: FIFOs + PCIe Error Bitmap)

# Check remaining two USB ports and SD-Card for connected devices

# Check for FPGA/Environment overtemperature (sensors-detect --auto -> sensors?)

# Check FPGA XADC alarms and eFuse key > 0

# Check for Atom shutdown due to overtemperature

# Check for SODIMM overtemperature

# Check for no detected atom module

# Check for USB Overcurrents (3.0, 2.0)			  

# Check for Atom WDT

# Check for bad serial interface frames

# Check for MMI timeouts

# No checking: Flash access, BNC ports, Admin port, SFPs, SODIMM, Atom Reset,
#  FPGA Reset, User-GPIO, FPGA-PMod, SysCtl-PMod, Battery (RTC, Enc),
#  bitstream programming (OPTO, JTAG, LPC), temperature monitor,
#  boot-time access to BIOS/UEFI

# Run all scripts not previously exectued:

/usr/share/InnoRoute/copy_of_firstboot.sh
/usr/share/InnoRoute/example_display_innoroute.sh
/usr/share/InnoRoute/example_display_selfnet.sh
/usr/share/InnoRoute/TN_clock.sh
/usr/share/InnoRoute/TN_fpga_status.sh
/usr/share/InnoRoute/TN_gpio.sh
/usr/share/InnoRoute/TN_mute.sh
/usr/share/InnoRoute/TN_mux_pll.sh
/usr/share/InnoRoute/TN_pcie.sh
/usr/share/InnoRoute/TN_phy_dump.sh
/usr/share/InnoRoute/TN_phy_examples.sh
/usr/share/InnoRoute/TN_phy_init.sh
/usr/share/InnoRoute/TN_phy_reset.sh
/usr/share/InnoRoute/TN_pll_dump.sh
/usr/share/InnoRoute/TN_sys_ctrl_pmod.sh
/usr/share/InnoRoute/TN_sys_ctrl_status.sh
/usr/share/InnoRoute/TN_text2display.sh
/usr/share/InnoRoute/TN_time.sh
/usr/share/InnoRoute/TN_usb.sh
/usr/share/InnoRoute/TN_version.sh

read

/usr/share/InnoRoute/TN_nw_bridge.sh