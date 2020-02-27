#!/bin/bash

source /usr/share/InnoRoute/tn_env.sh
source /usr/share/InnoRoute/tn_func_ll.sh

if [[ $# == 0 ]]; then
  echo "$0 <ex> is used to read the version numbers from all accessible system components"
  echo "The parameter <ex> can have any value. Without it, this help is displayed"
else
  # System Controller has SMBus address 0x04
  echo "### TrustNode Versions:"
  
  let mainboard=`i2cget -y 0 4 0x07`
  case $mainboard in
  0)
    echo " - Main Board V1.1" ;;
  1)
    echo " - Main Board V1.2";;
  *)
    echo " - Main Board Version unknown";;
  esac
  
  let firmware=`i2cget -y 0 4 0x00`
  printf  " - System Controller Firmware Version %d\n" $firmware
  case $firmware in
  [1-14])
    echo " -     -> FW too old - please update";;
  15)
    echo " -     -> FW2.0";;
  16)
    echo " -     -> FW2.1";;
  *)
    echo " -     -> TBD";;
  esac
  
  if [ -z "$(lspci -mm -d 10ee:0000)" ]; then echo "No PCIe connection to FPGA. Exiting."; exit 1; fi

  tn_ll_mmi_read $C_BASE_ADDR_COMMON_LOWER $C_SUB_ADDR_COMMON_TN_MAJOR_REV
  let    major=$read_data
  tn_ll_mmi_read $C_BASE_ADDR_COMMON_LOWER $C_SUB_ADDR_COMMON_TN_MINOR_REV
  let    minor=$read_data
  tn_ll_mmi_read $C_BASE_ADDR_COMMON_LOWER $C_SUB_ADDR_COMMON_USER_REV
  let     user=$read_data
  echo   "TrustNode FPGA design revision is $major.$minor, with a user sub-version of $user"
  
  tn_ll_mmi_read $C_BASE_ADDR_COMMON_LOWER $C_SUB_ADDR_COMMON_BITGEN_TIME
  let       day=$((($read_data & 0xF8000000) >> 27))
  let     month=$((($read_data & 0x07800000) >> 23))
  let      year=$((($read_data & 0x007E0000) >> 17))
  let      hour=$((($read_data & 0x0001F000) >> 12))
  let    minute=$((($read_data & 0x00000FC0) >> 6))
  let    second=$((($read_data & 0x0000003F) >> 0))
  printf   "Bitstream generation start was at %02d.%02d.20%02d %02d:%02d:%02d\n" $day $month $year $hour $minute $second

  tn_ll_mmi_read $C_BASE_ADDR_COMMON_LOWER $C_SUB_ADDR_COMMON_ADDR_MAP_REV
  let    addrmap=$(($read_data & 0x7FFFFFFF))
  echo   "MMI address map revision is $addrmap"
  
  printf " - Linux image: "; cat /etc/banner | grep Trust
  printf "  -> "; cat /proc/version
  
  echo "### TrustNode ID:"
  
  tn_ll_mmi_read $C_BASE_ADDR_COMMON_LOWER $C_SUB_ADDR_COMMON_FPGA_ID0
  let    fpgaid0=$read_data
  tn_ll_mmi_read $C_BASE_ADDR_COMMON_LOWER $C_SUB_ADDR_COMMON_FPGA_ID1
  let    fpgaid1=$read_data
  printf "Device DNA is 0x%07x%08x\n" $fpgaid1 $fpgaid0
  
  echo "### Configuration:"
  
  tn_ll_mmi_read $C_BASE_ADDR_COMMON_LOWER $C_SUB_ADDR_COMMON_FEATURES_6TREE
  let    feature_bitmap=$read_data
  let               st_en=$((($feature_bitmap &  1) >> 0))
  let              st_mmi=$((($feature_bitmap &  2) >> 1))
  let    st_uplink_bitmap=$((($feature_bitmap     ) >> 16))
  if [[ $st_en -eq 0 ]]; then
    echo   "6Tree Enabled: 0"
  else
    printf "6Tree Enabled: $st_en (Display(0) or MMI(1):$st_mmi; Uplink Port Bitmap: 0x%04x)\n" $st_uplink_bitmap
  fi
  
  tn_ll_mmi_read $C_BASE_ADDR_COMMON_LOWER $C_SUB_ADDR_COMMON_FEATURES_ETH_SW
  let    feature=$read_data
  echo   "Ethernet Switch Enabled: $feature"
  tn_ll_mmi_read $C_BASE_ADDR_COMMON_LOWER $C_SUB_ADDR_COMMON_FEATURES_FLOWCACHE
  let    feature_bitmap=$read_data
  let             fc_en=$((($feature_bitmap &  1) >> 0))
  let         fc_emh_en=$((($feature_bitmap &  2) >> 1))
  let          fc_ls_en=$((($feature_bitmap &  4) >> 2))
  let         fc_ema_en=$((($feature_bitmap &  8) >> 3))
  let   default_is_proc=$((($feature_bitmap & 16) >> 4))
  if [[ $fc_en -eq 0 ]]; then
    echo   "Flow Cache Enabled: 0"
  else
    echo   "Flow Cache Enabled: $fc_en (EMH:$fc_emh_en, LinSearch:$fc_ls_en, EMA:$fc_ema_en, DefaultIsProc:$default_is_proc)"
  fi
  tn_ll_mmi_read $C_BASE_ADDR_COMMON_LOWER $C_SUB_ADDR_COMMON_FEATURES_ACC_DP
  let    feature_bitmap=$read_data
  let          accdp_en=$((($feature_bitmap &          1)))
  let         accdp_cnt=$((($feature_bitmap &     0xFF00) >>  8))
  let     accdp_entries=$((($feature_bitmap & 0xFFFF0000) >> 16))
  if [[ $accdp_en -eq 0 ]]; then
    echo   "Acceleration Datapath Enabled: 0"
  else
    echo   "Acceleration Datapath Enabled: $accdp_en (AccDP Count:$accdp_cnt, AccDP Entry Count:$accdp_entries)"
  fi
  tn_ll_mmi_read $C_BASE_ADDR_COMMON_LOWER $C_SUB_ADDR_COMMON_FEATURES_TX_ROUTER
  let    feature=$read_data
  echo   "TX Route Enabled: $feature"
  tn_ll_mmi_read $C_BASE_ADDR_COMMON_LOWER $C_SUB_ADDR_COMMON_FEATURES_SFP
  let    feature_bitmap=$read_data
  let            sfp_en=$((($feature_bitmap &          1)))
  let           sfp_cnt=$((($feature_bitmap &     0xFF00) >>  8))
  if [[ $sfp_en -eq 0 ]]; then
    echo   "SFP Interface Enabled: 0"
  else
    echo   "SFP Interface Enabled: $sfp_en ($sfp_cnt ports)"
  fi
  tn_ll_mmi_read $C_BASE_ADDR_COMMON_LOWER $C_SUB_ADDR_COMMON_FEATURES_SODIMM
  let    feature=$read_data
  echo   "SODIMM Interface Enabled: $feature"
  
  tn_ll_mmi_read $C_BASE_ADDR_COMMON_LOWER $C_SUB_ADDR_COMMON_PARAM_PRT_CNT
  let    param=$read_data
  echo   "Physical Port Count: $param"
  tn_ll_mmi_read $C_BASE_ADDR_COMMON_LOWER $C_SUB_ADDR_COMMON_PARAM_DP_CNT
  let    param=$read_data
  echo   "Datapath Count (RX/TX): $param"
  tn_ll_mmi_read $C_BASE_ADDR_COMMON_LOWER $C_SUB_ADDR_COMMON_PARAM_BUF_CNT
  let    param=$read_data
  echo   "Segment buffer count: $param"
  tn_ll_mmi_read $C_BASE_ADDR_COMMON_LOWER $C_SUB_ADDR_COMMON_PARAM_RX_MIN
  let    param=$read_data
  echo   "Minimum Frame Size (RX/TX): $param"
  tn_ll_mmi_read $C_BASE_ADDR_COMMON_LOWER $C_SUB_ADDR_COMMON_PARAM_RX_MAX
  let    param=$read_data
  echo   "Maximum Frame Size (RX): $param"
  tn_ll_mmi_read $C_BASE_ADDR_COMMON_LOWER $C_SUB_ADDR_COMMON_PARAM_TX_PRE
  let    param=$read_data
  echo   "TX Preamble Length: $param"
  tn_ll_mmi_read $C_BASE_ADDR_COMMON_LOWER $C_SUB_ADDR_COMMON_PARAM_TX_IFG
  let    param=$read_data
  echo   "TX Minimum Inter-Frame Gap: $param"
  
  tn_ll_mmi_read $C_BASE_ADDR_COMMON_LOWER $C_SUB_ADDR_COMMON_PARAM_DISPLAY
  let    param=$read_data
  echo   "Display Connection: $param"
  tn_ll_mmi_read $C_BASE_ADDR_COMMON_LOWER $C_SUB_ADDR_COMMON_PARAM_LED_SYNCE
  let    param=$read_data
  echo   "SyncE LED Connection: $param"
  tn_ll_mmi_read $C_BASE_ADDR_COMMON_LOWER $C_SUB_ADDR_COMMON_PARAM_LED_INTERNAL
  let    param=$read_data
  echo   "Internal LED Connection: $param"
  tn_ll_mmi_read $C_BASE_ADDR_COMMON_LOWER $C_SUB_ADDR_COMMON_PARAM_BEEP
  let    param=$read_data
  echo   "Beeper Connection: $param"
  tn_ll_mmi_read $C_BASE_ADDR_COMMON_LOWER $C_SUB_ADDR_COMMON_PARAM_PHY_RESET
  let    param=$read_data
  echo   "PHY Reset Connection: $param"
  tn_ll_mmi_read $C_BASE_ADDR_COMMON_LOWER $C_SUB_ADDR_COMMON_PARAM_PINHEADER
  let    param=$read_data
  echo   "Internal Big Pinheader Connection: $param"
  tn_ll_mmi_read $C_BASE_ADDR_COMMON_LOWER $C_SUB_ADDR_COMMON_PARAM_PMOD
  let    param=$read_data
  echo   "Internal PMod Connection: $param"
  
  tn_ll_mmi_read $C_BASE_ADDR_COMMON_LOWER $C_SUB_ADDR_COMMON_PARAM_EXT_BYPASS
  let    param=$read_data
  echo   "External Bypass active: $param"
  
  tn_ll_mmi_read $C_BASE_ADDR_COMMON_LOWER $C_SUB_ADDR_COMMON_WIDTH_HC_BAD
  let    width=$read_data
  echo   "MMI Header Creation Fragment Counter Width: $width"
  tn_ll_mmi_read $C_BASE_ADDR_COMMON_LOWER $C_SUB_ADDR_COMMON_WIDTH_ETH_SW
  let    width=$((2**$read_data))
  echo   "Number of Ethernet Switch Entries per Datapath: $width"
  tn_ll_mmi_read $C_BASE_ADDR_COMMON_LOWER $C_SUB_ADDR_COMMON_WIDTH_RX_STAT_BAD
  let    width=$read_data
  echo   "RX Statistics Counter Width: $width"
fi
