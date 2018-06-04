#!/bin/bash

source /usr/share/InnoRoute/tn_env.sh
source /usr/share/InnoRoute/tn_func_ll.sh

# Should be converted to tn_ll_mmi_write/tn_ll_mmi_read calls

if [[ $# == 0 ]]; then
  echo "$0 <ex> is used to execute a selftest that might alter all"
  echo "currenly configured values and might even lead to a non-responding system"
  echo "The parameter <ex> can have any value. Without it, this help is displayed"
  echo "BEWARE: this script still uses the old unwrapped TNbar1 calls, leaving MMI Status undefined"
  echo "It also needs to be updated to cover testcases for all user-facing scripts"
else

  TNbar1 $(($C_BASE_ADDR_COMMON_LOWER*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_TN_MAJOR_REV))   | cut -d " " -f 6
  echo "Checking System Controller"
  # Check System Controller response (at least PHY resets implemented, so version 10)
  if [[ `i2cget -y 0 4 0x00` -lt 10 ]]; then
    echo "FAILED System Controller Version Check.";
    exit;
  fi

  TNbar1 $(($C_BASE_ADDR_COMMON_LOWER*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_TN_MAJOR_REV))   | cut -d " " -f 6
  echo "Checking USB devices"
  # Check USB devices:
  # lsusb (filtered): lsusb -d 0403:6010 -> Bus 001 Device 003: ID 0403:6010 Future Technology Devices International, Ltd FT2232C Dual USB-UART/FIFO IC

  # Load specific bitstream, using
  TNbar1 $(($C_BASE_ADDR_COMMON_LOWER*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_TN_MAJOR_REV))   | cut -d " " -f 6
  echo "Load various bitstreams"
  /usr/share/InnoRoute/TN_fpga_config_jtag.sh /usr/share/InnoRoute/tn_combined.bit
  sleep 1
  /usr/share/InnoRoute/TN_fpga_config_lpc.sh /usr/share/InnoRoute/tn_display_loopback.bin
  sleep 1
  /usr/share/InnoRoute/TN_fpga_config_opto.sh /usr/share/InnoRoute/tn_atom_ctrl.bin
  sleep 1
  /usr/sbin/TN_opto_prog /usr/share/InnoRoute/bs_unstable.bit
  sleep 1
  # Add bitstream with all connections set to "2", which has the RW test register
  #  each time checking the successful load count and bitstream error in SysCtrl

  TNbar1 $(($C_BASE_ADDR_COMMON_LOWER*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_TN_MAJOR_REV))   | cut -d " " -f 6
  echo "Check PCIe"
  # lspci (filtered): lspci -n -s 01:00.0 -> 01:00.0 0200: 10ee:0000 (rev 02)

  TNbar1 $(($C_BASE_ADDR_COMMON_LOWER*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_TN_MAJOR_REV))   | cut -d " " -f 6
  echo "MMI R/W tests"
  # PCIe BAR1 Test
  for wr_data in 0xC0FFEE12 0x1C0FFEE2 0x12C0FFEE; do
    TNbar1 $(($C_BASE_ADDR_COMMON_LOWER*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_RW_TEST)) w $wr_data >> /dev/null
    let    wr_result=`TNbar1 $(($C_BASE_ADDR_COMMON_LOWER*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_RW_TEST)) | cut -d " " -f 6`
    if [[ $wr_result -eq 0xEEEEEEEE ]]; then
      echo " ** MMI Write Timeout -> FPGA Bitstream Test FAILED **";
      #exit;
    fi
    if [[ $wr_result -ne $wr_data ]]; then
      printf " ** Bad MMI Write Data (0x%08x isn't 0x%08x) -> FPGA Bitstream Test FAILED **\n" $wr_result $wr_data;
      #exit;
    fi
    TNbar1 $(($C_BASE_ADDR_COMMON_LOWER*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_RW_TEST)) >> /dev/null
    let    rd_result=`TNbar1 $(($C_BASE_ADDR_COMMON_LOWER*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_RW_TEST)) | cut -d " " -f 6`
    if [[ $rd_result -eq 0xEEEEEEEE ]]; then
      echo " ** MMI Read Timeout -> FPGA Bitstream Test FAILED **";
      #exit;
    fi
    if [[ $rd_result -ne $wr_data ]]; then
      printf " ** Bad MMI Read Data (0x%08x isn't 0x%08x) -> FPGA Bitstream Test FAILED **\n" $rd_result $wr_data;
      #exit;
    fi
    sleep 1
  done

  TNbar1 $(($C_BASE_ADDR_COMMON_LOWER*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_TN_MAJOR_REV))   | cut -d " " -f 6
  echo "Disable ARP etc."
  # PCIe BAR0 test (all of this could also be done with phy loopback, at different speeds: 10/100/1000)
  # Set network to silent mode
  /usr/share/InnoRoute/TN_nw_silent.sh 1

  TNbar1 $(($C_BASE_ADDR_COMMON_LOWER*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_TN_MAJOR_REV))   | cut -d " " -f 6
  echo "Network Loopback"
  # Set network port loopback (CPU->FPGA->CPU)
  TNbar1 $(($C_BASE_ADDR_COMMON_LOWER*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_TN_MAJOR_REV))   | cut -d " " -f 6
  echo 1 > /proc/TrustNode/TN_send2cpu
  # Get current packet counts
  #######################################################################
  TNbar1 $(($C_BASE_ADDR_COMMON_LOWER*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_TN_MAJOR_REV))   | cut -d " " -f 6
  echo "Send one packet to each virtual interface"
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
  TNbar1 $(($C_BASE_ADDR_COMMON_LOWER*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_TN_MAJOR_REV))   | cut -d " " -f 6
  echo "Disable loopback"
  # Disable loopback:
  TNbar1 $(($C_BASE_ADDR_COMMON_LOWER*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_TN_MAJOR_REV))   | cut -d " " -f 6
  echo 0 > /proc/TrustNode/TN_send2cpu

  TNbar1 $(($C_BASE_ADDR_COMMON_LOWER*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_TN_MAJOR_REV))   | cut -d " " -f 6
  echo "Read PHY responses"
  # Check response from all 12 PHYs via MDIO
  let page=0
  let reg=0
  let write_data=0x0000
  for phy in `seq 0 11`; do
    tn_ll_read_phy $phy $page $reg
    let mdio_read_data=`printf "%04x\n" $(( $read_data & 0xFFFF ))`
    printf "MDIO: read 0x%04x from port %d" $mdio_read_data $phy
  done

  TNbar1 $(($C_BASE_ADDR_COMMON_LOWER*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_TN_MAJOR_REV))   | cut -d " " -f 6
  echo "Reset PHYs"
  # Reset PHYs and let user check all PHY LEDs

  TNbar1 $(($C_BASE_ADDR_COMMON_LOWER*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_TN_MAJOR_REV))   | cut -d " " -f 6
  echo "Checking PLL status"
  # Check PLL response:
  pllctl 0x69 0x0000
  # Program PLL using INRpllload, which uses pllctl:
  TNbar1 $(($C_BASE_ADDR_COMMON_LOWER*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_TN_MAJOR_REV))   | cut -d " " -f 6
  echo "Programming PLL (volatile)"
  INRpllload /usr/share/InnoRoute/tn-pll-v1.2_synce_7.stp
  TNbar1 $(($C_BASE_ADDR_COMMON_LOWER*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_TN_MAJOR_REV))   | cut -d " " -f 6
  echo "Programming done"

  # Expect cables between any two physical ports,
  #  or set PHY-based loopback (CPU->FPGA->PHY->FPGA->CPU):
  TNbar1 $(($C_BASE_ADDR_COMMON_LOWER*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_TN_MAJOR_REV))   | cut -d " " -f 6
  echo "Set PHY Loopback"
  /usr/share/InnoRoute/TN_phy_loopback.sh 1
  # Optionaly: check RGMII status:
  TNbar1 $(($C_BASE_ADDR_COMMON_LOWER*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_TN_MAJOR_REV))   | cut -d " " -f 6
  # Send packets to FPGA
  # Check for received packets with timeout
  # Optionally: disable loopback:
  #/usr/share/InnoRoute/TN_phy_loopback.sh 0

  TNbar1 $(($C_BASE_ADDR_COMMON_LOWER*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_TN_MAJOR_REV))   | cut -d " " -f 6
  echo "Test all RGMII phase settings"
  # Determine optimal RGMII phase settings
  #/usr/share/InnoRoute/TN_phy_loopback.sh 1
#  for phase in `seq 0 31`; do
#    /usr/share/InnoRoute/TN_rgmii_phase.sh $phase $phase $phase $phase $phase $phase $phase $phase $phase $phase $phase $phase
    # Check counters
    # Send packet
    # Check counters (delta)
#  done
  TNbar1 $(($C_BASE_ADDR_COMMON_LOWER*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_TN_MAJOR_REV))   | cut -d " " -f 6
  echo "Disable loopback"
  /usr/share/InnoRoute/TN_phy_loopback.sh 0

  TNbar1 $(($C_BASE_ADDR_COMMON_LOWER*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_TN_MAJOR_REV))   | cut -d " " -f 6
  echo "Check ClkBuf LOS"
  # Check ClkBuf-LOS'
  if [[ $(( `i2cget -y 0 4 0x01` & 48 )) -ne 0 ]]; then
    echo "FAIL: at least one oscillator is down";
    exit
  fi

  TNbar1 $(($C_BASE_ADDR_COMMON_LOWER*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_TN_MAJOR_REV))   | cut -d " " -f 6
  echo "Checking PLL status"
  # Also check PLL status:
  /usr/share/InnoRoute/TN_pll_status.sh 0 | grep "NOT"

  TNbar1 $(($C_BASE_ADDR_COMMON_LOWER*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_TN_MAJOR_REV))   | cut -d " " -f 6
  echo "Front display checks"
  # Let user check fully enlightened display
  /usr/share/InnoRoute/TN_front_display.sh 3
  sleep 1
  /usr/share/InnoRoute/TN_front_display.sh 0
  sleep 1
  /usr/share/InnoRoute/TN_front_display.sh 1
  sleep 1
  /usr/share/InnoRoute/TN_front_display.sh 2
  sleep 1
  /usr/share/InnoRoute/TN_front_display.sh 3
  sleep 1
  /usr/share/InnoRoute/TN_front_display.sh 0x1 0x100 0x10000 0x1000000
  sleep 1
  /usr/share/InnoRoute/TN_front_display.sh 0xAAAAAAAA 0xAAAAAAAA 0xAAAAAAAA 0xAAAAAAAA 
  sleep 1
  /usr/share/InnoRoute/TN_front_display.sh 0x55555555 0x55555555 0x55555555 0x55555555 
  sleep 1
  /usr/share/InnoRoute/TN_front_display.sh 0x55555555 0x55555555 0x55555555 0x55555555 3
  sleep 1
  /usr/share/InnoRoute/TN_front_display.sh 0x55555555 0x55555555 0x55555555 0x55555555 2
  sleep 1
  /usr/share/InnoRoute/TN_front_display.sh 0x55555555 0x55555555 0x55555555 0x55555555 1
  sleep 1
  /usr/share/InnoRoute/TN_front_display.sh 0x55555555 0x55555555 0x55555555 0x55555555 0
  sleep 1
  /usr/share/InnoRoute/TN_front_display.sh 0xFF 0xFF 0xFF 0xFF 0xFF 0xFF 0xFF 0xFF 0xFF 0xFF 0xFF 0xFF 0xFF 0xFF 0xFF 0xFF
  sleep 1
  /usr/share/InnoRoute/TN_front_display.sh 0x0F 0x0F 0x0F 0x0F 0x0F 0x0F 0x0F 0x0F 0x0F 0x0F 0x0F 0x0F 0x0F 0x0F 0x0F 0x0F 0
  sleep 1
  /usr/share/InnoRoute/TN_front_display.sh 0xF0 0xF0 0xF0 0xF0 0xF0 0xF0 0xF0 0xF0 0xF0 0xF0 0xF0 0xF0 0xF0 0xF0 0xF0 0xF0 1
  sleep 1
  /usr/share/InnoRoute/TN_front_display.sh 0x0F 0x0F 0x0F 0x0F 0x0F 0x0F 0x0F 0x0F 0x0F 0x0F 0x0F 0x0F 0x0F 0x0F 0x0F 0x0F 2
  sleep 1
  /usr/share/InnoRoute/TN_front_display.sh 0xF0 0xF0 0xF0 0xF0 0xF0 0xF0 0xF0 0xF0 0xF0 0xF0 0xF0 0xF0 0xF0 0xF0 0xF0 0xF0 3

  TNbar1 $(($C_BASE_ADDR_COMMON_LOWER*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_TN_MAJOR_REV))   | cut -d " " -f 6
  echo "LED checks"
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

  TNbar1 $(($C_BASE_ADDR_COMMON_LOWER*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_TN_MAJOR_REV))   | cut -d " " -f 6
  echo "Beep checks"
  # Let user check beep:
  /usr/share/InnoRoute/TN_beep.sh 1
  /usr/share/InnoRoute/TN_beep.sh 1
  /usr/share/InnoRoute/TN_beep.sh 1

  TNbar1 $(($C_BASE_ADDR_COMMON_LOWER*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_TN_MAJOR_REV))   | cut -d " " -f 6
  echo "Fan checks"
  # Let user check FAN sounds:
  /usr/share/InnoRoute/TN_fan.sh 63
  sleep 1
  /usr/share/InnoRoute/TN_fan.sh 20
  sleep 1
  /usr/share/InnoRoute/TN_fan.sh 100
  # Also check for stuck fan and check fan speed for being in a reasonable range

  TNbar1 $(($C_BASE_ADDR_COMMON_LOWER*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_TN_MAJOR_REV))   | cut -d " " -f 6
  echo "Button checks"
  # Let user press all 16 front buttons: read via MMI
  /usr/share/InnoRoute/TN_buttons.sh 1

  # Let user press user button on the back

  # Check for interrupts (PLL, 2xAlaskas, GPHYs)

  # Check error bitmaps (common_mmi: FIFOs + PCIe Error Bitmap)

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

  TNbar1 $(($C_BASE_ADDR_COMMON_LOWER*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_TN_MAJOR_REV))   | cut -d " " -f 6
  echo "Report FPGA status"
  /usr/share/InnoRoute/TN_fpga_status.sh 1
  sleep 1
  /usr/share/InnoRoute/TN_fpga_status.sh 0
  sleep 1
  /usr/share/InnoRoute/TN_fpga_status.sh 2
  sleep 1
  TNbar1 $(($C_BASE_ADDR_COMMON_LOWER*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_TN_MAJOR_REV))   | cut -d " " -f 6
  echo "GPIO patterns"
  /usr/share/InnoRoute/TN_gpio.sh 0x00000000
  sleep 1
  /usr/share/InnoRoute/TN_gpio.sh 0x0000FFFF
  sleep 1
  /usr/share/InnoRoute/TN_gpio.sh 0xFFFF0000
  sleep 1
  /usr/share/InnoRoute/TN_gpio.sh 0xFFFFFFFF
  sleep 1
  TNbar1 $(($C_BASE_ADDR_COMMON_LOWER*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_TN_MAJOR_REV))   | cut -d " " -f 6
  echo "Interrupts"
  /usr/share/InnoRoute/TN_interrupt.sh 1
  /usr/share/InnoRoute/TN_interrupt_mask.sh 1
  sleep 1
  /usr/share/InnoRoute/TN_interrupt_mask.sh 1 0xFFFFFFFF
  sleep 1
  /usr/share/InnoRoute/TN_interrupt_mask.sh 1 0x00000000
  /usr/share/InnoRoute/TN_phy_interrupts.sh 1
  TNbar1 $(($C_BASE_ADDR_COMMON_LOWER*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_TN_MAJOR_REV))   | cut -d " " -f 6
  echo "Muting"
  /usr/share/InnoRoute/TN_mute.sh 1
  sleep 1
  TNbar1 $(($C_BASE_ADDR_COMMON_LOWER*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_TN_MAJOR_REV))   | cut -d " " -f 6
  echo "Unmuting"
  /usr/share/InnoRoute/TN_mute.sh 0
  TNbar1 $(($C_BASE_ADDR_COMMON_LOWER*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_TN_MAJOR_REV))   | cut -d " " -f 6
  echo "PCIe checks"
  /usr/share/InnoRoute/TN_pcie.sh 0
  TNbar1 $(($C_BASE_ADDR_COMMON_LOWER*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_TN_MAJOR_REV))   | cut -d " " -f 6
  echo "PHY access checks"
  /usr/share/InnoRoute/TN_phy_access.sh 0 0 0
  sleep 1
  /usr/share/InnoRoute/TN_phy_access.sh 0 0 0 0x1200
  TNbar1 $(($C_BASE_ADDR_COMMON_LOWER*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_TN_MAJOR_REV))   | cut -d " " -f 6
  echo "Dumping PHY registers"
  /usr/share/InnoRoute/TN_phy_dump.sh 0
  TNbar1 $(($C_BASE_ADDR_COMMON_LOWER*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_TN_MAJOR_REV))   | cut -d " " -f 6
  echo "Go through all PHY examples"
  /usr/share/InnoRoute/TN_phy_examples.sh 0
  sleep 1
  /usr/share/InnoRoute/TN_phy_examples.sh 1
  sleep 1
  /usr/share/InnoRoute/TN_phy_examples.sh 2
  sleep 1
  /usr/share/InnoRoute/TN_phy_examples.sh 3
  sleep 1
  /usr/share/InnoRoute/TN_phy_examples.sh 4
  sleep 1
  /usr/share/InnoRoute/TN_phy_examples.sh 5
  sleep 1
  /usr/share/InnoRoute/TN_phy_examples.sh 6
  sleep 1
  /usr/share/InnoRoute/TN_phy_examples.sh 7
  sleep 1
  /usr/share/InnoRoute/TN_phy_examples.sh 8
  sleep 1
  /usr/share/InnoRoute/TN_phy_examples.sh 9
  sleep 1
  /usr/share/InnoRoute/TN_phy_examples.sh 10
  TNbar1 $(($C_BASE_ADDR_COMMON_LOWER*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_TN_MAJOR_REV))   | cut -d " " -f 6
  echo "PHY reset"
  /usr/share/InnoRoute/TN_phy_reset.sh 1
  TNbar1 $(($C_BASE_ADDR_COMMON_LOWER*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_TN_MAJOR_REV))   | cut -d " " -f 6
  echo "PHY initialization"
  /usr/share/InnoRoute/TN_phy_init.sh 1
  TNbar1 $(($C_BASE_ADDR_COMMON_LOWER*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_TN_MAJOR_REV))   | cut -d " " -f 6
  echo "Check PLL access"
  /usr/share/InnoRoute/TN_pll_access.sh 0
  sleep 1
  /usr/share/InnoRoute/TN_pll_access.sh 0 0xd3
  TNbar1 $(($C_BASE_ADDR_COMMON_LOWER*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_TN_MAJOR_REV))   | cut -d " " -f 6
  echo "Dump PLL registers"
  /usr/share/InnoRoute/TN_pll_dump.sh 1
  TNbar1 $(($C_BASE_ADDR_COMMON_LOWER*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_TN_MAJOR_REV))   | cut -d " " -f 6
  echo "SysCtrl PMod functions"
  /usr/share/InnoRoute/TN_sys_ctrl_pmod.sh 0
  sleep 1
  /usr/share/InnoRoute/TN_sys_ctrl_pmod.sh 1
  sleep 1
  /usr/share/InnoRoute/TN_sys_ctrl_pmod.sh 2
  sleep 1
  /usr/share/InnoRoute/TN_sys_ctrl_pmod.sh 3
  TNbar1 $(($C_BASE_ADDR_COMMON_LOWER*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_TN_MAJOR_REV))   | cut -d " " -f 6
  echo "SysCtrl status"
  /usr/share/InnoRoute/TN_sys_ctrl_status.sh 1
  TNbar1 $(($C_BASE_ADDR_COMMON_LOWER*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_TN_MAJOR_REV))   | cut -d " " -f 6
  echo "Front display tests"
  /usr/share/InnoRoute/TN_text2display.sh test
  sleep 1
  /usr/share/InnoRoute/TN_text2display.sh "testtesttesttest"
  TNbar1 $(($C_BASE_ADDR_COMMON_LOWER*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_TN_MAJOR_REV))   | cut -d " " -f 6
  echo "RTC checks"
  /usr/share/InnoRoute/TN_time.sh 2017-05-17 13:45:67
  sleep 1
  /usr/share/InnoRoute/TN_time.sh 1
  TNbar1 $(($C_BASE_ADDR_COMMON_LOWER*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_TN_MAJOR_REV))   | cut -d " " -f 6
  echo "USB checks"
  /usr/share/InnoRoute/TN_usb.sh 1
  TNbar1 $(($C_BASE_ADDR_COMMON_LOWER*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_TN_MAJOR_REV))   | cut -d " " -f 6
  echo "Version checks"
  /usr/share/InnoRoute/TN_version.sh 1
  TNbar1 $(($C_BASE_ADDR_COMMON_LOWER*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_TN_MAJOR_REV))   | cut -d " " -f 6
  echo "Set network bridging"
  /usr/share/InnoRoute/TN_nw_bridge.sh 1
  TNbar1 $(($C_BASE_ADDR_COMMON_LOWER*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_TN_MAJOR_REV))   | cut -d " " -f 6
  echo "Read from all MMI registers"
  /usr/share/InnoRoute/example_read.sh
  TNbar1 $(($C_BASE_ADDR_COMMON_LOWER*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_TN_MAJOR_REV))   | cut -d " " -f 6
  echo "Change clock source"
  /usr/share/InnoRoute/TN_clock.sh 1
  sleep 1
  /usr/share/InnoRoute/TN_clock.sh 0
  sleep 1

  echo "Test all RGMII phase settings"
  # Determine optimal RGMII phase settings
  #/usr/share/InnoRoute/TN_phy_loopback.sh 1
  for phase in `seq 0 31`; do
    /usr/share/InnoRoute/TN_rgmii_phase.sh $phase $phase $phase $phase $phase $phase $phase $phase $phase $phase $phase $phase
    # Check counters
    # Send packet
    # Check counters (delta)
  done

  TNbar1 $(($C_BASE_ADDR_COMMON_LOWER*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_TN_MAJOR_REV))   | cut -d " " -f 6
  echo "Start BWM-NG"
  read

  /usr/share/InnoRoute/TN_nw_bridge.sh 1 1
fi
