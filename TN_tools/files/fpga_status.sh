#!/bin/bash

source /usr/share/InnoRoute/tn_env.sh

echo -e "\n### Reading system status from mmi_common module:"

let    major=`TNbar1 $(($C_BASE_ADDR_TN_COMMON*256+$C_SUB_ADDR_COMMON_TN_MAJOR_REV))   | cut -d ":" -f 2 | cut -d " " -f 2`
let    minor=`TNbar1 $(($C_BASE_ADDR_TN_COMMON*256+$C_SUB_ADDR_COMMON_TN_MINOR_REV))   | cut -d ":" -f 2 | cut -d " " -f 2`
let     user=`TNbar1 $(($C_BASE_ADDR_TN_COMMON*256+$C_SUB_ADDR_COMMON_USER_REV))       | cut -d ":" -f 2 | cut -d " " -f 2`
echo   "TrustNode revision is $major.$minor, with a user sub-version of $user"

let    addrmap=`TNbar1 $(($C_BASE_ADDR_TN_COMMON*256+$C_SUB_ADDR_COMMON_ADDR_MAP_REV)) | cut -d ":" -f 2 | cut -d " " -f 2`
echo   "MMI address map revision is $addrmap"

let    fpgaid0=`TNbar1 $(($C_BASE_ADDR_TN_COMMON*256+$C_SUB_ADDR_COMMON_FPGA_ID0)) | cut -d ":" -f 2 | cut -d " " -f 2`
let    fpgaid1=`TNbar1 $(($C_BASE_ADDR_TN_COMMON*256+$C_SUB_ADDR_COMMON_FPGA_ID1)) | cut -d ":" -f 2 | cut -d " " -f 2`
printf "Device DNA is 0x%07x%08x\n" $fpgaid1 $fpgaid0

let    efuse=`TNbar1 $(($C_BASE_ADDR_TN_COMMON*256+$C_SUB_ADDR_COMMON_EFUSE)) | cut -d ":" -f 2 | cut -d " " -f 2`
echo   "eFUSE key is $efuse"

let       day=$(((`TNbar1 $(($C_BASE_ADDR_TN_COMMON*256+$C_SUB_ADDR_COMMON_BITGEN_TIME)) | cut -d ":" -f 2 | cut -d " " -f 2` & 0xF8000000) >> 27))
let     month=$(((`TNbar1 $(($C_BASE_ADDR_TN_COMMON*256+$C_SUB_ADDR_COMMON_BITGEN_TIME)) | cut -d ":" -f 2 | cut -d " " -f 2` & 0x07800000) >> 23))
let      year=$(((`TNbar1 $(($C_BASE_ADDR_TN_COMMON*256+$C_SUB_ADDR_COMMON_BITGEN_TIME)) | cut -d ":" -f 2 | cut -d " " -f 2` & 0x007E0000) >> 17))
let      hour=$(((`TNbar1 $(($C_BASE_ADDR_TN_COMMON*256+$C_SUB_ADDR_COMMON_BITGEN_TIME)) | cut -d ":" -f 2 | cut -d " " -f 2` & 0x0001F000) >> 12))
let    minute=$(((`TNbar1 $(($C_BASE_ADDR_TN_COMMON*256+$C_SUB_ADDR_COMMON_BITGEN_TIME)) | cut -d ":" -f 2 | cut -d " " -f 2` & 0x00000FC0) >> 6))
let    second=$(((`TNbar1 $(($C_BASE_ADDR_TN_COMMON*256+$C_SUB_ADDR_COMMON_BITGEN_TIME)) | cut -d ":" -f 2 | cut -d " " -f 2` & 0x0000003F) >> 0))
printf   "Bitstream generation start was at %02d.%02d.20%02d %02d:%02d:%02d\n" $day $month $year $hour $minute $second

####
let      config_check_bitmap=`TNbar1 $(($C_BASE_ADDR_TN_COMMON*256+$C_SUB_ADDR_COMMON_CONFIG_CHECK)) | cut -d ":" -f 2 | cut -d " " -f 2`
let           crc_error=$((($config_check_bitmap & 1) >> 0))
let           ecc_error=$((($config_check_bitmap & 2) >> 1))
let    single_ecc_error=$((($config_check_bitmap & 4) >> 2))
echo   "FPGA State: CRC Error=$crc_error, ECC Error=$ecc_error (Single=$single_ecc_error)"

let    temp=`TNbar1 $(($C_BASE_ADDR_TN_COMMON*256+$C_SUB_ADDR_COMMON_FPGA_TEMP)) | cut -d ":" -f 2 | cut -d " " -f 2`*504/4096-273
echo   "FPGA temperature is $temp degrees celsius"

####
let    fpga_alarm_bitmap=`TNbar1 $(($C_BASE_ADDR_TN_COMMON*256+$C_SUB_ADDR_COMMON_FPGA_ALARM)) | cut -d ":" -f 2 | cut -d " " -f 2`
let            alarm=$((($fpga_alarm_bitmap &  1) >> 0))
let        over_temp=$((($fpga_alarm_bitmap &  2) >> 1))
let        user_temp=$((($fpga_alarm_bitmap &  4) >> 2))
let     vccaux_alarm=$((($fpga_alarm_bitmap &  8) >> 3))
let     vccint_alarm=$((($fpga_alarm_bitmap & 16) >> 4))
let      vbram_alarm=$((($fpga_alarm_bitmap & 32) >> 5))
echo   "FPGA Alarm: $alarm (Overtemperature:$over_temp, User Temperature:$user_temp, AUX Voltage:$vccaux_alarm, Logic Voltage:$vccint_alarm, BRAM Voltage:$vbram_alarm)"

####
let    feature_bitmap=`TNbar1 $(($C_BASE_ADDR_TN_COMMON*256+$C_SUB_ADDR_COMMON_FEATURES_6TREE)) | cut -d ":" -f 2 | cut -d " " -f 2`
let               st_en=$((($feature_bitmap &  1) >> 0))
let              st_mmi=$((($feature_bitmap &  2) >> 1))
let    st_uplink_bitmap=$((($feature_bitmap     ) >> 16))
printf "6Tree Enabled: $st_en (Display(0) or MMI(1):$st_mmi; Uplink Port Bitmap: 0x%04x\n" $st_uplink_bitmap

let    feature=`TNbar1 $(($C_BASE_ADDR_TN_COMMON*256+$C_SUB_ADDR_COMMON_FEATURES_ETH_SW)) | cut -d ":" -f 2 | cut -d " " -f 2`
echo   "Ethernet Switch: $feature"
let    feature=`TNbar1 $(($C_BASE_ADDR_TN_COMMON*256+$C_SUB_ADDR_COMMON_FEATURES_FLOWCACHE)) | cut -d ":" -f 2 | cut -d " " -f 2`
echo   "Flow Cache: $feature"
let    feature=`TNbar1 $(($C_BASE_ADDR_TN_COMMON*256+$C_SUB_ADDR_COMMON_FEATURES_ACC_DP)) | cut -d ":" -f 2 | cut -d " " -f 2`
echo   "Acceleration Datapath: $feature"
let    feature=`TNbar1 $(($C_BASE_ADDR_TN_COMMON*256+$C_SUB_ADDR_COMMON_FEATURES_TX_ROUTER)) | cut -d ":" -f 2 | cut -d " " -f 2`
echo   "TX Router: $feature"
let    feature=`TNbar1 $(($C_BASE_ADDR_TN_COMMON*256+$C_SUB_ADDR_COMMON_FEATURES_SFP)) | cut -d ":" -f 2 | cut -d " " -f 2`
echo   "SFP Interface: $feature"
let    feature=`TNbar1 $(($C_BASE_ADDR_TN_COMMON*256+$C_SUB_ADDR_COMMON_FEATURES_SODIMM)) | cut -d ":" -f 2 | cut -d " " -f 2`
echo   "SODIMM Interface: $feature"

let    param=`TNbar1 $(($C_BASE_ADDR_TN_COMMON*256+$C_SUB_ADDR_COMMON_PARAM_PRT_CNT)) | cut -d ":" -f 2 | cut -d " " -f 2`
echo   "Physical Port Count: $param"
let    param=`TNbar1 $(($C_BASE_ADDR_TN_COMMON*256+$C_SUB_ADDR_COMMON_PARAM_DP_CNT)) | cut -d ":" -f 2 | cut -d " " -f 2`
echo   "Datapath Count (RX/TX): $param"
let    param=`TNbar1 $(($C_BASE_ADDR_TN_COMMON*256+$C_SUB_ADDR_COMMON_PARAM_BUF_CNT)) | cut -d ":" -f 2 | cut -d " " -f 2`
echo   "Segment buffer count: $param"
let    param=`TNbar1 $(($C_BASE_ADDR_TN_COMMON*256+$C_SUB_ADDR_COMMON_PARAM_RX_MIN)) | cut -d ":" -f 2 | cut -d " " -f 2`
echo   "Minimum Frame Size (RX/TX): $param"
let    param=`TNbar1 $(($C_BASE_ADDR_TN_COMMON*256+$C_SUB_ADDR_COMMON_PARAM_RX_MAX)) | cut -d ":" -f 2 | cut -d " " -f 2`
echo   "Maximum Frame Size (RX): $param"
let    param=`TNbar1 $(($C_BASE_ADDR_TN_COMMON*256+$C_SUB_ADDR_COMMON_PARAM_TX_PRE)) | cut -d ":" -f 2 | cut -d " " -f 2`
echo   "TX Preamble Length: $param"
let    param=`TNbar1 $(($C_BASE_ADDR_TN_COMMON*256+$C_SUB_ADDR_COMMON_PARAM_TX_IFG)) | cut -d ":" -f 2 | cut -d " " -f 2`
echo   "TX Minimum Inter-Frame Gap: $param"

echo "System connections: 0=Unused, 1=Slave Serial Interface, 2=MMI, 3=6Tree Application, 4=User"

let    param=`TNbar1 $(($C_BASE_ADDR_TN_COMMON*256+$C_SUB_ADDR_COMMON_PARAM_DISPLAY)) | cut -d ":" -f 2 | cut -d " " -f 2`
echo   "Display Connection: $param"
let    param=`TNbar1 $(($C_BASE_ADDR_TN_COMMON*256+$C_SUB_ADDR_COMMON_PARAM_LED_SYNCE)) | cut -d ":" -f 2 | cut -d " " -f 2`
echo   "SyncE LED Connection: $param"
let    param=`TNbar1 $(($C_BASE_ADDR_TN_COMMON*256+$C_SUB_ADDR_COMMON_PARAM_LED_INTERNAL)) | cut -d ":" -f 2 | cut -d " " -f 2`
echo   "Internal LED Connection: $param"
let    param=`TNbar1 $(($C_BASE_ADDR_TN_COMMON*256+$C_SUB_ADDR_COMMON_PARAM_BEEP)) | cut -d ":" -f 2 | cut -d " " -f 2`
echo   "Beeper Connection: $param"
let    param=`TNbar1 $(($C_BASE_ADDR_TN_COMMON*256+$C_SUB_ADDR_COMMON_PARAM_PHY_RESET)) | cut -d ":" -f 2 | cut -d " " -f 2`
echo   "PHY Reset Connection: $param"
let    param=`TNbar1 $(($C_BASE_ADDR_TN_COMMON*256+$C_SUB_ADDR_COMMON_PARAM_PINHEADER)) | cut -d ":" -f 2 | cut -d " " -f 2`
echo   "Internal Big Pinheader Connection: $param"
let    param=`TNbar1 $(($C_BASE_ADDR_TN_COMMON*256+$C_SUB_ADDR_COMMON_PARAM_PMOD)) | cut -d ":" -f 2 | cut -d " " -f 2`
echo   "Internal PMod Connection: $param"

let    param=`TNbar1 $(($C_BASE_ADDR_TN_COMMON*256+$C_SUB_ADDR_COMMON_PARAM_EXT_BYPASS)) | cut -d ":" -f 2 | cut -d " " -f 2`
echo   "External Bypass active: $param"

let    width=`TNbar1 $(($C_BASE_ADDR_TN_COMMON*256+$C_SUB_ADDR_COMMON_WIDTH_HC_BAD)) | cut -d ":" -f 2 | cut -d " " -f 2`
echo   "MMI Header Creation Fragment Counter Width: $width"
let    width=$((2**`TNbar1 $(($C_BASE_ADDR_TN_COMMON*256+$C_SUB_ADDR_COMMON_WIDTH_ETH_SW)) | cut -d ":" -f 2 | cut -d " " -f 2`))
echo   "Number of Ethernet Switch Entries per Datapath: $width"
let    width=`TNbar1 $(($C_BASE_ADDR_TN_COMMON*256+$C_SUB_ADDR_COMMON_WIDTH_RX_STAT_BAD)) | cut -d ":" -f 2 | cut -d " " -f 2`
echo   "RX Statistics Counter Width: $width"

let    overflow_bitmap0=`TNbar1 $(($C_BASE_ADDR_TN_COMMON*256+$C_SUB_ADDR_COMMON_FIFO_OVERFLOW0)) | cut -d ":" -f 2 | cut -d " " -f 2`
let    overflow_bitmap1=`TNbar1 $(($C_BASE_ADDR_TN_COMMON*256+$C_SUB_ADDR_COMMON_FIFO_OVERFLOW1)) | cut -d ":" -f 2 | cut -d " " -f 2`
let    overflow_bitmap2=`TNbar1 $(($C_BASE_ADDR_TN_COMMON*256+$C_SUB_ADDR_COMMON_FIFO_OVERFLOW2)) | cut -d ":" -f 2 | cut -d " " -f 2`
let    overflow_bitmap3=`TNbar1 $(($C_BASE_ADDR_TN_COMMON*256+$C_SUB_ADDR_COMMON_FIFO_OVERFLOW3)) | cut -d ":" -f 2 | cut -d " " -f 2`
printf "FIFO Overflow Bitmap: 0x%08x%08x%08x%08x\n" $overflow_bitmap3 $overflow_bitmap2 $overflow_bitmap1 $overflow_bitmap0

let    underrun_bitmap0=`TNbar1 $(($C_BASE_ADDR_TN_COMMON*256+$C_SUB_ADDR_COMMON_FIFO_UNDERRUN0)) | cut -d ":" -f 2 | cut -d " " -f 2`
let    underrun_bitmap1=`TNbar1 $(($C_BASE_ADDR_TN_COMMON*256+$C_SUB_ADDR_COMMON_FIFO_UNDERRUN1)) | cut -d ":" -f 2 | cut -d " " -f 2`
let    underrun_bitmap2=`TNbar1 $(($C_BASE_ADDR_TN_COMMON*256+$C_SUB_ADDR_COMMON_FIFO_UNDERRUN2)) | cut -d ":" -f 2 | cut -d " " -f 2`
let    underrun_bitmap3=`TNbar1 $(($C_BASE_ADDR_TN_COMMON*256+$C_SUB_ADDR_COMMON_FIFO_UNDERRUN3)) | cut -d ":" -f 2 | cut -d " " -f 2`
printf "FIFO Underrun Bitmap: 0x%08x%08x%08x%08x\n" $underrun_bitmap3 $underrun_bitmap2 $underrun_bitmap1 $underrun_bitmap0

let    pcie_error_bitmap0=`TNbar1 $(($C_BASE_ADDR_TN_COMMON*256+$C_SUB_ADDR_COMMON_PCIE_ERROR0)) | cut -d ":" -f 2 | cut -d " " -f 2`
let    pcie_error_bitmap1=`TNbar1 $(($C_BASE_ADDR_TN_COMMON*256+$C_SUB_ADDR_COMMON_PCIE_ERROR1)) | cut -d ":" -f 2 | cut -d " " -f 2`
printf "PCIe Error Bitmap: 0x%08x%08x\n" $pcie_error_bitmap1 $pcie_error_bitmap0

let    pcie_timeouts=`TNbar1 $(($C_BASE_ADDR_TN_COMMON*256+$C_SUB_ADDR_COMMON_PCIE_TIMEOUT_CNT)) | cut -d ":" -f 2 | cut -d " " -f 2`
echo   "Number of MMI Timeouts: $pcie_timeouts"

let    tm_buffer_full=`TNbar1 $(($C_BASE_ADDR_TN_COMMON*256+$C_SUB_ADDR_COMMON_BUFFER_FULL)) | cut -d ":" -f 2 | cut -d " " -f 2`
echo   "TM Buffer Full: $tm_buffer_full"

let    txf_full_bitmap=`TNbar1 $(($C_BASE_ADDR_TN_COMMON*256+$C_SUB_ADDR_COMMON_TXF_FULL)) | cut -d ":" -f 2 | cut -d " " -f 2`
printf "TX FIFO Full Port Bitmap: 0x%08x\n" $txf_full_bitmap

echo -e "\n### Reading system status from peripherals module"

let    peripherals_button_bitmap=`TNbar1 $(($C_BASE_ADDR_PERIPH*256+$C_SUB_ADDR_PERIPH_BUTTONS)) | cut -d ":" -f 2 | cut -d " " -f 2`
let    button_front=$((($peripherals_button_bitmap & 0x0000FFFF) >>  0))
let     button_user=$((($peripherals_button_bitmap & 0x00010000) >> 16))
printf "Front Button Bitmap: 0x%04x\n" $button_front
echo   "User Button: $button_user"

let    peripherals_interrupt_bitmap=`TNbar1 $(($C_BASE_ADDR_PERIPH*256+$C_SUB_ADDR_PERIPH_INTERRUPTS)) | cut -d ":" -f 2 | cut -d " " -f 2`
let                 gphy_md_ints=$((($peripherals_interrupt_bitmap & 0x000003FF) >>  0))
let              alaska_ptp_ints=$((($peripherals_interrupt_bitmap & 0x00000C00) >> 10))
let    alaska_ptp_event_triggers=$((($peripherals_interrupt_bitmap & 0x00030000) >> 16))
printf "GPHY MDIO Interrupt Bitmap: 0x%03x\n" $gphy_md_ints
printf "Alaska PTP Interrupt Bitmap: 0x%x\n" $alaska_ptp_ints
printf "Alaska PTP Event Trigger Bitmap: 0x%x\n" $alaska_ptp_event_triggers

let    peripherals_gpio_bitmap=`TNbar1 $(($C_BASE_ADDR_PERIPH*256+$C_SUB_ADDR_PERIPH_GPIOS)) | cut -d ":" -f 2 | cut -d " " -f 2`
let     pinheader1_gpio_in=$((($peripherals_gpio_bitmap & 0x000003FF) >>  0))
let          pmod0_gpio_in=$((($peripherals_gpio_bitmap & 0x00003C00) >> 10))
let    pinheader1_gpio_tri=$((($peripherals_gpio_bitmap & 0x03FF0000) >> 16))
let         pmod0_gpio_tri=$((($peripherals_gpio_bitmap & 0x3C000000) >> 26))
printf "Pinheader1 GPIO Bitmap: 0x%03x (Read Data Valid: 0x%03x)\n" $pinheader1_gpio_in $pinheader1_gpio_tri
printf "PMOD0 GPIO Bitmap: 0x%x (Read Data Valid: 0x%x)\n" $pmod0_gpio_in $pmod0_gpio_tri
