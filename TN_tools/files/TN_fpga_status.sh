#!/bin/bash

source /usr/share/InnoRoute/tn_env.sh

echo -e "\n### Reading system status from mmi_common module:"

let    major=`TNbar1 $(($C_BASE_ADDR_TN_COMMON_L*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_TN_MAJOR_REV))   | cut -d " " -f 6`
let    minor=`TNbar1 $(($C_BASE_ADDR_TN_COMMON_L*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_TN_MINOR_REV))   | cut -d " " -f 6`
let     user=`TNbar1 $(($C_BASE_ADDR_TN_COMMON_L*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_USER_REV))       | cut -d " " -f 6`
echo   "TrustNode FPGA design revision is $major.$minor, with a user sub-version of $user"
if [[ $major -eq 0xEEEEEEEE || $minor -eq 0xEEEEEEEE || $user -eq 0xEEEEEEEE ]]; then
  echo " ** MMI Read Timeout **"
fi

let    addrmap=`TNbar1 $(($C_BASE_ADDR_TN_COMMON_L*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_ADDR_MAP_REV)) | cut -d " " -f 6`
echo   "MMI address map revision is $addrmap"
if [[ $addrmap -eq 0xEEEEEEEE ]]; then
  echo " ** MMI Read Timeout **"
fi

let    fpgaid0=`TNbar1 $(($C_BASE_ADDR_TN_COMMON_L*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_FPGA_ID0)) | cut -d " " -f 6`
let    fpgaid1=`TNbar1 $(($C_BASE_ADDR_TN_COMMON_L*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_FPGA_ID1)) | cut -d " " -f 6`
printf "Device DNA is 0x%07x%08x\n" $fpgaid1 $fpgaid0
if [[ $fpgaid0 -eq 0xEEEEEEEE || $fpgaid1 -eq 0xEEEEEEEE ]]; then
  echo " ** MMI Read Timeout **"
fi

let    efuse=`TNbar1 $(($C_BASE_ADDR_TN_COMMON_L*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_EFUSE)) | cut -d " " -f 6`
echo   "eFUSE key is $efuse"
if [[ $efuse -eq 0xEEEEEEEE ]]; then
  echo " ** MMI Read Timeout **"
fi

let       day=$(((`TNbar1 $(($C_BASE_ADDR_TN_COMMON_L*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_BITGEN_TIME)) | cut -d " " -f 6` & 0xF8000000) >> 27))
let     month=$(((`TNbar1 $(($C_BASE_ADDR_TN_COMMON_L*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_BITGEN_TIME)) | cut -d " " -f 6` & 0x07800000) >> 23))
let      year=$(((`TNbar1 $(($C_BASE_ADDR_TN_COMMON_L*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_BITGEN_TIME)) | cut -d " " -f 6` & 0x007E0000) >> 17))
let      hour=$(((`TNbar1 $(($C_BASE_ADDR_TN_COMMON_L*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_BITGEN_TIME)) | cut -d " " -f 6` & 0x0001F000) >> 12))
let    minute=$(((`TNbar1 $(($C_BASE_ADDR_TN_COMMON_L*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_BITGEN_TIME)) | cut -d " " -f 6` & 0x00000FC0) >> 6))
let    second=$(((`TNbar1 $(($C_BASE_ADDR_TN_COMMON_L*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_BITGEN_TIME)) | cut -d " " -f 6` & 0x0000003F) >> 0))
printf   "Bitstream generation start was at %02d.%02d.20%02d %02d:%02d:%02d\n" $day $month $year $hour $minute $second
if [[ $day -eq 0xEEEEEEEE || $month -eq 0xEEEEEEEE || $year -eq 0xEEEEEEEE || $hour -eq 0xEEEEEEEE || $minute -eq 0xEEEEEEEE || $second -eq 0xEEEEEEEE ]]; then
  echo " ** MMI Read Timeout **"
fi

####
let      config_check_bitmap=`TNbar1 $(($C_BASE_ADDR_TN_COMMON_L*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_CONFIG_CHECK)) | cut -d " " -f 6`
let           crc_error=$((($config_check_bitmap & 1) >> 0))
let           ecc_error=$((($config_check_bitmap & 2) >> 1))
let    single_ecc_error=$((($config_check_bitmap & 4) >> 2))
echo   "FPGA State: CRC Error=$crc_error, ECC Error=$ecc_error (Single=$single_ecc_error)"
if [[ $config_check_bitmap -eq 0xEEEEEEEE ]]; then
  echo " ** MMI Read Timeout **"
fi

let    temp=`TNbar1 $(($C_BASE_ADDR_TN_COMMON_L*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_FPGA_TEMP)) | cut -d " " -f 6`*504/4096-273
echo   "FPGA temperature is $temp degrees celsius"
if [[ $temp -eq 0xEEEEEEEE ]]; then
  echo " ** MMI Read Timeout **"
fi

####
let    fpga_alarm_bitmap=`TNbar1 $(($C_BASE_ADDR_TN_COMMON_L*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_FPGA_ALARM)) | cut -d " " -f 6`
let            alarm=$((($fpga_alarm_bitmap &  1) >> 0))
let        over_temp=$((($fpga_alarm_bitmap &  2) >> 1))
let        user_temp=$((($fpga_alarm_bitmap &  4) >> 2))
let     vccaux_alarm=$((($fpga_alarm_bitmap &  8) >> 3))
let     vccint_alarm=$((($fpga_alarm_bitmap & 16) >> 4))
let      vbram_alarm=$((($fpga_alarm_bitmap & 32) >> 5))
echo   "FPGA Alarm: $alarm (Overtemperature:$over_temp, User Temperature:$user_temp, AUX Voltage:$vccaux_alarm, Logic Voltage:$vccint_alarm, BRAM Voltage:$vbram_alarm)"
if [[ $fpga_alarm_bitmap -eq 0xEEEEEEEE ]]; then
  echo " ** MMI Read Timeout **"
fi

####
# Stop here, if a parameter was given -> used on logging in
if [[ $# -gt 0 ]]; then 
	exit 0
fi

####
let    feature_bitmap=`TNbar1 $(($C_BASE_ADDR_TN_COMMON_L*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_FEATURES_6TREE)) | cut -d " " -f 6`
let               st_en=$((($feature_bitmap &  1) >> 0))
let              st_mmi=$((($feature_bitmap &  2) >> 1))
let    st_uplink_bitmap=$((($feature_bitmap     ) >> 16))
printf "6Tree Enabled: $st_en (Display(0) or MMI(1):$st_mmi; Uplink Port Bitmap: 0x%04x)\n" $st_uplink_bitmap
if [[ $feature_bitmap -eq 0xEEEEEEEE ]]; then
  echo " ** MMI Read Timeout **"
fi

let    feature=`TNbar1 $(($C_BASE_ADDR_TN_COMMON_L*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_FEATURES_ETH_SW)) | cut -d " " -f 6`
echo   "Ethernet Switch Enabled: $feature"
if [[ $feature -eq 0xEEEEEEEE ]]; then
  echo " ** MMI Read Timeout **"
fi
let    feature=`TNbar1 $(($C_BASE_ADDR_TN_COMMON_L*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_FEATURES_FLOWCACHE)) | cut -d " " -f 6`
echo   "Flow Cache Enabled: $feature"
if [[ $feature -eq 0xEEEEEEEE ]]; then
  echo " ** MMI Read Timeout **"
fi
let    feature=`TNbar1 $(($C_BASE_ADDR_TN_COMMON_L*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_FEATURES_ACC_DP)) | cut -d " " -f 6`
echo   "Acceleration Datapath Enabled: $feature"
if [[ $feature -eq 0xEEEEEEEE ]]; then
  echo " ** MMI Read Timeout **"
fi
let    feature=`TNbar1 $(($C_BASE_ADDR_TN_COMMON_L*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_FEATURES_TX_ROUTER)) | cut -d " " -f 6`
echo   "TX Route Enabled: $feature"
if [[ $feature -eq 0xEEEEEEEE ]]; then
  echo " ** MMI Read Timeout **"
fi
let    feature=`TNbar1 $(($C_BASE_ADDR_TN_COMMON_L*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_FEATURES_SFP)) | cut -d " " -f 6`
echo   "SFP Interface Enabled: $feature"
if [[ $feature -eq 0xEEEEEEEE ]]; then
  echo " ** MMI Read Timeout **"
fi
let    feature=`TNbar1 $(($C_BASE_ADDR_TN_COMMON_L*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_FEATURES_SODIMM)) | cut -d " " -f 6`
echo   "SODIMM Interface Enabled: $feature"
if [[ $feature -eq 0xEEEEEEEE ]]; then
  echo " ** MMI Read Timeout **"
fi

let    param=`TNbar1 $(($C_BASE_ADDR_TN_COMMON_L*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_PARAM_PRT_CNT)) | cut -d " " -f 6`
echo   "Physical Port Count: $param"
if [[ $param -eq 0xEEEEEEEE ]]; then
  echo " ** MMI Read Timeout **"
fi
let    param=`TNbar1 $(($C_BASE_ADDR_TN_COMMON_L*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_PARAM_DP_CNT)) | cut -d " " -f 6`
echo   "Datapath Count (RX/TX): $param"
if [[ $param -eq 0xEEEEEEEE ]]; then
  echo " ** MMI Read Timeout **"
fi
let    param=`TNbar1 $(($C_BASE_ADDR_TN_COMMON_L*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_PARAM_BUF_CNT)) | cut -d " " -f 6`
echo   "Segment buffer count: $param"
if [[ $param -eq 0xEEEEEEEE ]]; then
  echo " ** MMI Read Timeout **"
fi
let    param=`TNbar1 $(($C_BASE_ADDR_TN_COMMON_L*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_PARAM_RX_MIN)) | cut -d " " -f 6`
echo   "Minimum Frame Size (RX/TX): $param"
if [[ $param -eq 0xEEEEEEEE ]]; then
  echo " ** MMI Read Timeout **"
fi
let    param=`TNbar1 $(($C_BASE_ADDR_TN_COMMON_L*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_PARAM_RX_MAX)) | cut -d " " -f 6`
echo   "Maximum Frame Size (RX): $param"
if [[ $param -eq 0xEEEEEEEE ]]; then
  echo " ** MMI Read Timeout **"
fi
let    param=`TNbar1 $(($C_BASE_ADDR_TN_COMMON_L*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_PARAM_TX_PRE)) | cut -d " " -f 6`
echo   "TX Preamble Length: $param"
if [[ $param -eq 0xEEEEEEEE ]]; then
  echo " ** MMI Read Timeout **"
fi
let    param=`TNbar1 $(($C_BASE_ADDR_TN_COMMON_L*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_PARAM_TX_IFG)) | cut -d " " -f 6`
echo   "TX Minimum Inter-Frame Gap: $param"
if [[ $param -eq 0xEEEEEEEE ]]; then
  echo " ** MMI Read Timeout **"
fi

echo "System connection codings are 0=Unused, 1=Slave Serial Interface, 2=MMI, 3=6Tree Application, 4=User"

let    param=`TNbar1 $(($C_BASE_ADDR_TN_COMMON_L*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_PARAM_DISPLAY)) | cut -d " " -f 6`
echo   "Display Connection: $param"
if [[ $param -eq 0xEEEEEEEE ]]; then
  echo " ** MMI Read Timeout **"
fi
let    param=`TNbar1 $(($C_BASE_ADDR_TN_COMMON_L*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_PARAM_LED_SYNCE)) | cut -d " " -f 6`
echo   "SyncE LED Connection: $param"
if [[ $param -eq 0xEEEEEEEE ]]; then
  echo " ** MMI Read Timeout **"
fi
let    param=`TNbar1 $(($C_BASE_ADDR_TN_COMMON_L*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_PARAM_LED_INTERNAL)) | cut -d " " -f 6`
echo   "Internal LED Connection: $param"
if [[ $param -eq 0xEEEEEEEE ]]; then
  echo " ** MMI Read Timeout **"
fi
let    param=`TNbar1 $(($C_BASE_ADDR_TN_COMMON_L*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_PARAM_BEEP)) | cut -d " " -f 6`
echo   "Beeper Connection: $param"
if [[ $param -eq 0xEEEEEEEE ]]; then
  echo " ** MMI Read Timeout **"
fi
let    param=`TNbar1 $(($C_BASE_ADDR_TN_COMMON_L*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_PARAM_PHY_RESET)) | cut -d " " -f 6`
echo   "PHY Reset Connection: $param"
if [[ $param -eq 0xEEEEEEEE ]]; then
  echo " ** MMI Read Timeout **"
fi
let    param=`TNbar1 $(($C_BASE_ADDR_TN_COMMON_L*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_PARAM_PINHEADER)) | cut -d " " -f 6`
echo   "Internal Big Pinheader Connection: $param"
if [[ $param -eq 0xEEEEEEEE ]]; then
  echo " ** MMI Read Timeout **"
fi
let    param=`TNbar1 $(($C_BASE_ADDR_TN_COMMON_L*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_PARAM_PMOD)) | cut -d " " -f 6`
echo   "Internal PMod Connection: $param"
if [[ $param -eq 0xEEEEEEEE ]]; then
  echo " ** MMI Read Timeout **"
fi

let    param=`TNbar1 $(($C_BASE_ADDR_TN_COMMON_L*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_PARAM_EXT_BYPASS)) | cut -d " " -f 6`
echo   "External Bypass active: $param"
if [[ $param -eq 0xEEEEEEEE ]]; then
  echo " ** MMI Read Timeout **"
fi

let    width=`TNbar1 $(($C_BASE_ADDR_TN_COMMON_L*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_WIDTH_HC_BAD)) | cut -d " " -f 6`
echo   "MMI Header Creation Fragment Counter Width: $width"
if [[ $width -eq 0xEEEEEEEE ]]; then
  echo " ** MMI Read Timeout **"
fi
let    width=$((2**`TNbar1 $(($C_BASE_ADDR_TN_COMMON_L*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_WIDTH_ETH_SW)) | cut -d " " -f 6`))
echo   "Number of Ethernet Switch Entries per Datapath: $width"
if [[ $width -eq 0xEEEEEEEE ]]; then
  echo " ** MMI Read Timeout **"
fi
let    width=`TNbar1 $(($C_BASE_ADDR_TN_COMMON_L*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_WIDTH_RX_STAT_BAD)) | cut -d " " -f 6`
echo   "RX Statistics Counter Width: $width"
if [[ $width -eq 0xEEEEEEEE ]]; then
  echo " ** MMI Read Timeout **"
fi

let    overflow_bitmap0=`TNbar1 $(($C_BASE_ADDR_TN_COMMON_L*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_FIFO_OVERFLOW0)) | cut -d " " -f 6`
let    overflow_bitmap1=`TNbar1 $(($C_BASE_ADDR_TN_COMMON_L*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_FIFO_OVERFLOW1)) | cut -d " " -f 6`
let    overflow_bitmap2=`TNbar1 $(($C_BASE_ADDR_TN_COMMON_L*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_FIFO_OVERFLOW2)) | cut -d " " -f 6`
let    overflow_bitmap3=`TNbar1 $(($C_BASE_ADDR_TN_COMMON_L*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_FIFO_OVERFLOW3)) | cut -d " " -f 6`
printf "FIFO Overflow Bitmap: 0x%08x%08x%08x%08x\n" $overflow_bitmap3 $overflow_bitmap2 $overflow_bitmap1 $overflow_bitmap0
if [[ $overflow_bitmap0 -eq 0xEEEEEEEE || $overflow_bitmap1 -eq 0xEEEEEEEE || $overflow_bitmap2 -eq 0xEEEEEEEE || $overflow_bitmap3 -eq 0xEEEEEEEE ]]; then
  echo " ** MMI Read Timeout **"
fi

let    underrun_bitmap0=`TNbar1 $(($C_BASE_ADDR_TN_COMMON_L*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_FIFO_UNDERRUN0)) | cut -d " " -f 6`
let    underrun_bitmap1=`TNbar1 $(($C_BASE_ADDR_TN_COMMON_L*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_FIFO_UNDERRUN1)) | cut -d " " -f 6`
let    underrun_bitmap2=`TNbar1 $(($C_BASE_ADDR_TN_COMMON_L*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_FIFO_UNDERRUN2)) | cut -d " " -f 6`
let    underrun_bitmap3=`TNbar1 $(($C_BASE_ADDR_TN_COMMON_L*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_FIFO_UNDERRUN3)) | cut -d " " -f 6`
printf "FIFO Underrun Bitmap: 0x%08x%08x%08x%08x\n" $underrun_bitmap3 $underrun_bitmap2 $underrun_bitmap1 $underrun_bitmap0
if [[ $underrun_bitmap0 -eq 0xEEEEEEEE || $underrun_bitmap1 -eq 0xEEEEEEEE || $underrun_bitmap2 -eq 0xEEEEEEEE || $underrun_bitmap3 -eq 0xEEEEEEEE ]]; then
  echo " ** MMI Read Timeout **"
fi

let    pcie_error_bitmap0=`TNbar1 $(($C_BASE_ADDR_TN_COMMON_L*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_PCIE_ERROR0)) | cut -d " " -f 6`
let    pcie_error_bitmap1=`TNbar1 $(($C_BASE_ADDR_TN_COMMON_L*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_PCIE_ERROR1)) | cut -d " " -f 6`
printf "PCIe Error Bitmap: 0x%08x%08x\n" $pcie_error_bitmap1 $pcie_error_bitmap0
if [[ $pcie_error_bitmap0 -eq 0xEEEEEEEE || $pcie_error_bitmap1 -eq 0xEEEEEEEE ]]; then
  echo " ** MMI Read Timeout **"
fi

let    pcie_timeouts=`TNbar1 $(($C_BASE_ADDR_TN_COMMON_L*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_PCIE_TIMEOUT_CNT)) | cut -d " " -f 6`
echo   "Number of MMI Timeouts (Read/Write): $pcie_timeouts"
if [[ $pcie_timeouts -eq 0xEEEEEEEE ]]; then
  echo " ** MMI Read Timeout **"
fi

let    tm_buffer_full=`TNbar1 $(($C_BASE_ADDR_TN_COMMON_L*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_BUFFER_FULL)) | cut -d " " -f 6`
echo   "Packet Buffer Full: $tm_buffer_full"
if [[ $tm_buffer_full -eq 0xEEEEEEEE ]]; then
  echo " ** MMI Read Timeout **"
fi

let    txf_full_bitmap=`TNbar1 $(($C_BASE_ADDR_TN_COMMON_L*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_TXF_FULL)) | cut -d " " -f 6`
printf "TX FIFO Full Port Bitmap: 0x%08x\n" $txf_full_bitmap
if [[ $txf_full_bitmap -eq 0xEEEEEEEE ]]; then
  echo " ** MMI Read Timeout **"
fi

echo -e "\n### Reading system status from peripherals module"

let    peripherals_brightness=`TNbar1 $(($C_BASE_ADDR_PERIPH*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_PERIPH_BRIGHTNESS)) | cut -d " " -f 6`
echo   "Display Brightness: $peripherals_brightness"
if [[ $peripherals_brightness -eq 0xEEEEEEEE ]]; then
  echo " ** MMI Read Timeout **"
fi

let    peripherals_button_bitmap=`TNbar1 $(($C_BASE_ADDR_PERIPH*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_PERIPH_BUTTONS)) | cut -d " " -f 6`
let    button_front=$((($peripherals_button_bitmap & 0x0000FFFF) >>  0))
let     button_user=$((($peripherals_button_bitmap & 0x00010000) >> 16))
printf "Front Button Bitmap: 0x%04x\n" $button_front
echo   "User Button: $button_user"
if [[ $peripherals_button_bitmap -eq 0xEEEEEEEE ]]; then
  echo " ** MMI Read Timeout **"
fi

let    peripherals_gpio_bitmap=`TNbar1 $(($C_BASE_ADDR_PERIPH*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_PERIPH_GPIOS)) | cut -d " " -f 6`
let     pinheader1_gpio_in=$((($peripherals_gpio_bitmap & 0x000003FF) >>  0))
let          pmod0_gpio_in=$((($peripherals_gpio_bitmap & 0x00003C00) >> 10))
let    pinheader1_gpio_tri=$((($peripherals_gpio_bitmap & 0x03FF0000) >> 16))
let         pmod0_gpio_tri=$((($peripherals_gpio_bitmap & 0x3C000000) >> 26))
printf "Pinheader1 GPIO Bitmap: 0x%03x (Read Data Valid: 0x%03x)\n" $pinheader1_gpio_in $pinheader1_gpio_tri
printf "PMOD0 GPIO Bitmap: 0x%x (Read Data Valid: 0x%x)\n" $pmod0_gpio_in $pmod0_gpio_tri
if [[ $peripherals_gpio_bitmap -eq 0xEEEEEEEE ]]; then
  echo " ** MMI Read Timeout **"
fi

echo -e "\n### Reading interrupt status from MDIO module"

let    peripherals_interrupt_bitmap=`TNbar1 $(($C_BASE_ADDR_MDIO*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_MDIO_INTERRUPTS)) | cut -d " " -f 6`
let                 gphy_md_ints=$((($peripherals_interrupt_bitmap & 0x000003FF) >>  0))
let              alaska_ptp_ints=$((($peripherals_interrupt_bitmap & 0x00000C00) >> 10))
let    alaska_ptp_event_triggers=$((($peripherals_interrupt_bitmap & 0x00030000) >> 16))
printf "GPHY MDIO Interrupt Bitmap: 0x%03x\n" $gphy_md_ints
printf "Alaska PTP Interrupt Bitmap: 0x%x\n" $alaska_ptp_ints
printf "Alaska PTP Event Trigger Bitmap: 0x%x\n" $alaska_ptp_event_triggers
if [[ $peripherals_interrupt_bitmap -eq 0xEEEEEEEE ]]; then
  echo " ** MMI Read Timeout **"
fi
