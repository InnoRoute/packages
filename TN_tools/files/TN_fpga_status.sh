#!/bin/bash

source /usr/share/InnoRoute/tn_env.sh
source /usr/share/InnoRoute/tn_func_ll.sh

# Should be converted to tn_ll_mmi_write/tn_ll_mmi_read calls

if [[ $# == 0 ]]; then
  echo "$0 <verbosity> is used to read current FPGA status,"
  echo "provided by the MMI_COMMON unit and others"
  echo "The parameter <verbosity> can between 0-2"
  echo "  0: Standard Output"
  echo "  1: Minimum Output"
  echo "  2: Maximum Output"
  echo "BEWARE: this script still uses the old unwrapped TNbar1 calls, leaving MMI Status undefined"
else
 
  echo -e "\n### Reading system status from common_mmi module:"
  
  let    major=`TNbar1 $(($C_BASE_ADDR_COMMON_LOWER*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_TN_MAJOR_REV))   | cut -d " " -f 6`
  let    minor=`TNbar1 $(($C_BASE_ADDR_COMMON_LOWER*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_TN_MINOR_REV))   | cut -d " " -f 6`
  let     user=`TNbar1 $(($C_BASE_ADDR_COMMON_LOWER*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_USER_REV))       | cut -d " " -f 6`
  echo   "TrustNode FPGA design revision is $major.$minor, with a user sub-version of $user"
  if [[ $major -eq 0xEEEEEEEE || $minor -eq 0xEEEEEEEE || $user -eq 0xEEEEEEEE ]]; then
    echo " ** MMI Read Timeout **"
  fi
  
  let    addrmap=`TNbar1 $(($C_BASE_ADDR_COMMON_LOWER*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_ADDR_MAP_REV)) | cut -d " " -f 6`
  echo   "MMI address map revision is $addrmap"
  if [[ $addrmap -eq 0xEEEEEEEE ]]; then
    echo " ** MMI Read Timeout **"
  fi
  
  let    fpgaid0=`TNbar1 $(($C_BASE_ADDR_COMMON_LOWER*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_FPGA_ID0)) | cut -d " " -f 6`
  let    fpgaid1=`TNbar1 $(($C_BASE_ADDR_COMMON_LOWER*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_FPGA_ID1)) | cut -d " " -f 6`
  printf "Device DNA is 0x%07x%08x\n" $fpgaid1 $fpgaid0
  if [[ $fpgaid0 -eq 0xEEEEEEEE || $fpgaid1 -eq 0xEEEEEEEE ]]; then
    echo " ** MMI Read Timeout **"
  fi
  
  let    efuse=`TNbar1 $(($C_BASE_ADDR_COMMON_LOWER*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_EFUSE)) | cut -d " " -f 6`
  echo   "eFUSE key is $efuse"
  if [[ $efuse -eq 0xEEEEEEEE ]]; then
    echo " ** MMI Read Timeout **"
  fi
  
  let       day=$(((`TNbar1 $(($C_BASE_ADDR_COMMON_LOWER*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_BITGEN_TIME)) | cut -d " " -f 6` & 0xF8000000) >> 27))
  let     month=$(((`TNbar1 $(($C_BASE_ADDR_COMMON_LOWER*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_BITGEN_TIME)) | cut -d " " -f 6` & 0x07800000) >> 23))
  let      year=$(((`TNbar1 $(($C_BASE_ADDR_COMMON_LOWER*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_BITGEN_TIME)) | cut -d " " -f 6` & 0x007E0000) >> 17))
  let      hour=$(((`TNbar1 $(($C_BASE_ADDR_COMMON_LOWER*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_BITGEN_TIME)) | cut -d " " -f 6` & 0x0001F000) >> 12))
  let    minute=$(((`TNbar1 $(($C_BASE_ADDR_COMMON_LOWER*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_BITGEN_TIME)) | cut -d " " -f 6` & 0x00000FC0) >> 6))
  let    second=$(((`TNbar1 $(($C_BASE_ADDR_COMMON_LOWER*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_BITGEN_TIME)) | cut -d " " -f 6` & 0x0000003F) >> 0))
  printf   "Bitstream generation start was at %02d.%02d.20%02d %02d:%02d:%02d\n" $day $month $year $hour $minute $second
  if [[ $day -eq 0xEEEEEEEE || $month -eq 0xEEEEEEEE || $year -eq 0xEEEEEEEE || $hour -eq 0xEEEEEEEE || $minute -eq 0xEEEEEEEE || $second -eq 0xEEEEEEEE ]]; then
    echo " ** MMI Read Timeout **"
  fi
  
  ####
  let      config_check_bitmap=`TNbar1 $(($C_BASE_ADDR_COMMON_LOWER*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_CONFIG_CHECK)) | cut -d " " -f 6`
  let           crc_error=$((($config_check_bitmap & 1) >> 0))
  let           ecc_error=$((($config_check_bitmap & 2) >> 1))
  let    single_ecc_error=$((($config_check_bitmap & 4) >> 2))
  echo   "FPGA State: CRC Error=$crc_error, ECC Error=$ecc_error (Single=$single_ecc_error)"
  if [[ $config_check_bitmap -eq 0xEEEEEEEE ]]; then
    echo " ** MMI Read Timeout **"
  fi
  
  let    temp=`TNbar1 $(($C_BASE_ADDR_COMMON_LOWER*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_FPGA_TEMP)) | cut -d " " -f 6`*504/4096-273
  echo   "FPGA temperature is $temp degrees celsius"
  if [[ $temp -eq 0xEEEEEEEE ]]; then
    echo " ** MMI Read Timeout **"
  fi
  
  ####
  let    fpga_alarm_bitmap=`TNbar1 $(($C_BASE_ADDR_COMMON_LOWER*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_FPGA_ALARM)) | cut -d " " -f 6`
  let            alarm=$((($fpga_alarm_bitmap &  1) >> 0))
  let        over_temp=$((($fpga_alarm_bitmap &  2) >> 1))
  let        user_temp=$((($fpga_alarm_bitmap &  4) >> 2))
  let     vccaux_alarm=$((($fpga_alarm_bitmap &  8) >> 3))
  let     vccint_alarm=$((($fpga_alarm_bitmap & 16) >> 4))
  let      vbram_alarm=$((($fpga_alarm_bitmap & 32) >> 5))
  if [[ $alarm -eq 0 ]]; then
    echo   "FPGA Alarm: 0";
  else
    echo   "FPGA Alarm: $alarm (Overtemperature:$over_temp, User Temperature:$user_temp, AUX Voltage:$vccaux_alarm, Logic Voltage:$vccint_alarm, BRAM Voltage:$vbram_alarm)";
  fi
  if [[ $fpga_alarm_bitmap -eq 0xEEEEEEEE ]]; then
    echo " ** MMI Read Timeout **"
  fi
  
  ####
  # Stop here, if a parameter was given -> used on logging in
  if [[ $1 -eq 1 ]]; then
    exit 0
  fi
  
  ####
  let    feature_bitmap=`TNbar1 $(($C_BASE_ADDR_COMMON_LOWER*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_FEATURES_6TREE)) | cut -d " " -f 6`
  let               st_en=$((($feature_bitmap &  1) >> 0))
  let              st_mmi=$((($feature_bitmap &  2) >> 1))
  let    st_uplink_bitmap=$((($feature_bitmap     ) >> 16))
  if [[ $st_en -eq 0 ]]; then
    echo   "6Tree Enabled: 0"
  else
    printf "6Tree Enabled: $st_en (Display(0) or MMI(1):$st_mmi; Uplink Port Bitmap: 0x%04x)\n" $st_uplink_bitmap
  fi
  if [[ $feature_bitmap -eq 0xEEEEEEEE ]]; then
    echo " ** MMI Read Timeout **"
  fi
  
  let    feature=`TNbar1 $(($C_BASE_ADDR_COMMON_LOWER*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_FEATURES_ETH_SW)) | cut -d " " -f 6`
  echo   "Ethernet Switch Enabled: $feature"
  if [[ $feature -eq 0xEEEEEEEE ]]; then
    echo " ** MMI Read Timeout **"
  fi
  let    feature_bitmap=`TNbar1 $(($C_BASE_ADDR_COMMON_LOWER*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_FEATURES_FLOWCACHE)) | cut -d " " -f 6`
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
  if [[ $feature_bitmap -eq 0xEEEEEEEE ]]; then
    echo " ** MMI Read Timeout **"
  fi
  let    feature_bitmap=`TNbar1 $(($C_BASE_ADDR_COMMON_LOWER*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_FEATURES_ACC_DP)) | cut -d " " -f 6`
  let          accdp_en=$((($feature_bitmap &          1)))
  let         accdp_cnt=$((($feature_bitmap &     0xFF00) >>  8))
  let     accdp_entries=$((($feature_bitmap & 0xFFFF0000) >> 16))
  if [[ $accdp_en -eq 0 ]]; then
    echo   "Acceleration Datapath Enabled: 0"
  else
    echo   "Acceleration Datapath Enabled: $accdp_en (AccDP Count:$accdp_cnt, AccDP Entry Count:$accdp_entries)"
  fi
  if [[ $feature_bitmap -eq 0xEEEEEEEE ]]; then
    echo " ** MMI Read Timeout **"
  fi
  let    feature=`TNbar1 $(($C_BASE_ADDR_COMMON_LOWER*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_FEATURES_TX_ROUTER)) | cut -d " " -f 6`
  echo   "TX Route Enabled: $feature"
  if [[ $feature -eq 0xEEEEEEEE ]]; then
    echo " ** MMI Read Timeout **"
  fi
  let    feature_bitmap=`TNbar1 $(($C_BASE_ADDR_COMMON_LOWER*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_FEATURES_SFP)) | cut -d " " -f 6`
  let            sfp_en=$((($feature_bitmap &          1)))
  let           sfp_cnt=$((($feature_bitmap &     0xFF00) >>  8))
  if [[ $sfp_en -eq 0 ]]; then
    echo   "SFP Interface Enabled: 0"
  else
    echo   "SFP Interface Enabled: $sfp_en ($sfp_cnt ports)"
  fi
  if [[ $feature -eq 0xEEEEEEEE ]]; then
    echo " ** MMI Read Timeout **"
  fi
  let    feature=`TNbar1 $(($C_BASE_ADDR_COMMON_LOWER*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_FEATURES_SODIMM)) | cut -d " " -f 6`
  echo   "SODIMM Interface Enabled: $feature"
  if [[ $feature -eq 0xEEEEEEEE ]]; then
    echo " ** MMI Read Timeout **"
  fi
  
  let    param=`TNbar1 $(($C_BASE_ADDR_COMMON_LOWER*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_PARAM_PRT_CNT)) | cut -d " " -f 6`
  echo   "Physical Port Count: $param"
  if [[ $param -eq 0xEEEEEEEE ]]; then
    echo " ** MMI Read Timeout **"
  fi
  let    param=`TNbar1 $(($C_BASE_ADDR_COMMON_LOWER*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_PARAM_DP_CNT)) | cut -d " " -f 6`
  echo   "Datapath Count (RX/TX): $param"
  if [[ $param -eq 0xEEEEEEEE ]]; then
    echo " ** MMI Read Timeout **"
  fi
  let    param=`TNbar1 $(($C_BASE_ADDR_COMMON_LOWER*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_PARAM_BUF_CNT)) | cut -d " " -f 6`
  echo   "Segment buffer count: $param"
  if [[ $param -eq 0xEEEEEEEE ]]; then
    echo " ** MMI Read Timeout **"
  fi
  let    param=`TNbar1 $(($C_BASE_ADDR_COMMON_LOWER*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_PARAM_RX_MIN)) | cut -d " " -f 6`
  echo   "Minimum Frame Size (RX/TX): $param"
  if [[ $param -eq 0xEEEEEEEE ]]; then
    echo " ** MMI Read Timeout **"
  fi
  let    param=`TNbar1 $(($C_BASE_ADDR_COMMON_LOWER*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_PARAM_RX_MAX)) | cut -d " " -f 6`
  echo   "Maximum Frame Size (RX): $param"
  if [[ $param -eq 0xEEEEEEEE ]]; then
    echo " ** MMI Read Timeout **"
  fi
  let    param=`TNbar1 $(($C_BASE_ADDR_COMMON_LOWER*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_PARAM_TX_PRE)) | cut -d " " -f 6`
  echo   "TX Preamble Length: $param"
  if [[ $param -eq 0xEEEEEEEE ]]; then
    echo " ** MMI Read Timeout **"
  fi
  let    param=`TNbar1 $(($C_BASE_ADDR_COMMON_LOWER*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_PARAM_TX_IFG)) | cut -d " " -f 6`
  echo   "TX Minimum Inter-Frame Gap: $param"
  if [[ $param -eq 0xEEEEEEEE ]]; then
    echo " ** MMI Read Timeout **"
  fi
  
  echo "System connection codings are 0=Unused, 1=Slave Serial Interface, 2=MMI, 3=6Tree Application, 4=User"
  
  let    param=`TNbar1 $(($C_BASE_ADDR_COMMON_LOWER*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_PARAM_DISPLAY)) | cut -d " " -f 6`
  echo   "Display Connection: $param"
  if [[ $param -eq 0xEEEEEEEE ]]; then
    echo " ** MMI Read Timeout **"
  fi
  let    param=`TNbar1 $(($C_BASE_ADDR_COMMON_LOWER*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_PARAM_LED_SYNCE)) | cut -d " " -f 6`
  echo   "SyncE LED Connection: $param"
  if [[ $param -eq 0xEEEEEEEE ]]; then
    echo " ** MMI Read Timeout **"
  fi
  let    param=`TNbar1 $(($C_BASE_ADDR_COMMON_LOWER*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_PARAM_LED_INTERNAL)) | cut -d " " -f 6`
  echo   "Internal LED Connection: $param"
  if [[ $param -eq 0xEEEEEEEE ]]; then
    echo " ** MMI Read Timeout **"
  fi
  let    param=`TNbar1 $(($C_BASE_ADDR_COMMON_LOWER*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_PARAM_BEEP)) | cut -d " " -f 6`
  echo   "Beeper Connection: $param"
  if [[ $param -eq 0xEEEEEEEE ]]; then
    echo " ** MMI Read Timeout **"
  fi
  let    param=`TNbar1 $(($C_BASE_ADDR_COMMON_LOWER*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_PARAM_PHY_RESET)) | cut -d " " -f 6`
  echo   "PHY Reset Connection: $param"
  if [[ $param -eq 0xEEEEEEEE ]]; then
    echo " ** MMI Read Timeout **"
  fi
  let    param=`TNbar1 $(($C_BASE_ADDR_COMMON_LOWER*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_PARAM_PINHEADER)) | cut -d " " -f 6`
  echo   "Internal Big Pinheader Connection: $param"
  if [[ $param -eq 0xEEEEEEEE ]]; then
    echo " ** MMI Read Timeout **"
  fi
  let    param=`TNbar1 $(($C_BASE_ADDR_COMMON_LOWER*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_PARAM_PMOD)) | cut -d " " -f 6`
  echo   "Internal PMod Connection: $param"
  if [[ $param -eq 0xEEEEEEEE ]]; then
    echo " ** MMI Read Timeout **"
  fi
  
  let    param=`TNbar1 $(($C_BASE_ADDR_COMMON_LOWER*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_PARAM_EXT_BYPASS)) | cut -d " " -f 6`
  echo   "External Bypass active: $param"
  if [[ $param -eq 0xEEEEEEEE ]]; then
    echo " ** MMI Read Timeout **"
  fi
  
  let    width=`TNbar1 $(($C_BASE_ADDR_COMMON_LOWER*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_WIDTH_HC_BAD)) | cut -d " " -f 6`
  echo   "MMI Header Creation Fragment Counter Width: $width"
  if [[ $width -eq 0xEEEEEEEE ]]; then
    echo " ** MMI Read Timeout **"
  fi
  let    width=$((2**`TNbar1 $(($C_BASE_ADDR_COMMON_LOWER*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_WIDTH_ETH_SW)) | cut -d " " -f 6`))
  echo   "Number of Ethernet Switch Entries per Datapath: $width"
  if [[ $width -eq 0xEEEEEEEE ]]; then
    echo " ** MMI Read Timeout **"
  fi
  let    width=`TNbar1 $(($C_BASE_ADDR_COMMON_LOWER*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_WIDTH_RX_STAT_BAD)) | cut -d " " -f 6`
  echo   "RX Statistics Counter Width: $width"
  if [[ $width -eq 0xEEEEEEEE ]]; then
    echo " ** MMI Read Timeout **"
  fi
  
  let    overflow_bitmap0=`TNbar1 $(($C_BASE_ADDR_COMMON_LOWER*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_FIFO_OVERFLOW0)) | cut -d " " -f 6`
  let    overflow_bitmap1=`TNbar1 $(($C_BASE_ADDR_COMMON_LOWER*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_FIFO_OVERFLOW1)) | cut -d " " -f 6`
  let    overflow_bitmap2=`TNbar1 $(($C_BASE_ADDR_COMMON_LOWER*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_FIFO_OVERFLOW2)) | cut -d " " -f 6`
  let    overflow_bitmap3=`TNbar1 $(($C_BASE_ADDR_COMMON_LOWER*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_FIFO_OVERFLOW3)) | cut -d " " -f 6`
  if [[ $overflow_bitmap0 -eq 0 && $overflow_bitmap1 -eq 0 && $overflow_bitmap2 -eq 0 && $overflow_bitmap3 -eq 0 ]]; then
    echo "FIFO Overflow Bitmap: 0"
  else
    printf "FIFO Overflow Bitmap: 0x%08x%08x%08x%08x\n" $overflow_bitmap3 $overflow_bitmap2 $overflow_bitmap1 $overflow_bitmap0
  fi
  if [[ $overflow_bitmap0 -eq 0xEEEEEEEE || $overflow_bitmap1 -eq 0xEEEEEEEE || $overflow_bitmap2 -eq 0xEEEEEEEE || $overflow_bitmap3 -eq 0xEEEEEEEE ]]; then
    echo " ** MMI Read Timeout **"
  fi
  
  let    underrun_bitmap0=`TNbar1 $(($C_BASE_ADDR_COMMON_LOWER*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_FIFO_UNDERRUN0)) | cut -d " " -f 6`
  let    underrun_bitmap1=`TNbar1 $(($C_BASE_ADDR_COMMON_LOWER*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_FIFO_UNDERRUN1)) | cut -d " " -f 6`
  let    underrun_bitmap2=`TNbar1 $(($C_BASE_ADDR_COMMON_LOWER*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_FIFO_UNDERRUN2)) | cut -d " " -f 6`
  let    underrun_bitmap3=`TNbar1 $(($C_BASE_ADDR_COMMON_LOWER*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_FIFO_UNDERRUN3)) | cut -d " " -f 6`
  if [[ $underrun_bitmap0 -eq 0 && $underrun_bitmap1 -eq 0 && $underrun_bitmap2 -eq 0 && $underrun_bitmap3 -eq 0 ]]; then
    echo "FIFO Underrun Bitmap: 0"
  else
    printf "FIFO Underrun Bitmap: 0x%08x%08x%08x%08x\n" $underrun_bitmap3 $underrun_bitmap2 $underrun_bitmap1 $underrun_bitmap0
  fi
  if [[ $underrun_bitmap0 -eq 0xEEEEEEEE || $underrun_bitmap1 -eq 0xEEEEEEEE || $underrun_bitmap2 -eq 0xEEEEEEEE || $underrun_bitmap3 -eq 0xEEEEEEEE ]]; then
    echo " ** MMI Read Timeout **"
  fi
  
  let    pcie_error_bitmap0=`TNbar1 $(($C_BASE_ADDR_COMMON_LOWER*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_PCIE_ERROR0)) | cut -d " " -f 6`
  let    pcie_error_bitmap1=`TNbar1 $(($C_BASE_ADDR_COMMON_LOWER*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_PCIE_ERROR1)) | cut -d " " -f 6`
  if [[ $pcie_error_bitmap1 -eq 0 && $pcie_error_bitmap0 -eq 0 ]]; then
    echo "PCIe Error Bitmap: 0"
  else
    printf "PCIe Error Bitmap: 0x%08x%08x\n" $pcie_error_bitmap1 $pcie_error_bitmap0
  fi
  if [[ $pcie_error_bitmap0 -eq 0xEEEEEEEE || $pcie_error_bitmap1 -eq 0xEEEEEEEE ]]; then
    echo " ** MMI Read Timeout **"
  fi
  
  let    pcie_timeouts=`TNbar1 $(($C_BASE_ADDR_COMMON_LOWER*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_PCIE_TIMEOUT_CNT)) | cut -d " " -f 6`
  echo   "Number of MMI Timeouts (Read/Write, saturating counter): $pcie_timeouts"
  if [[ $pcie_timeouts -eq 0xEEEEEEEE ]]; then
    echo " ** MMI Read Timeout **"
  fi
  
  tn_ll_mmi_read $C_BASE_ADDR_TM $C_SUB_ADDR_TM_PTR_CNT
  echo   "Packet Buffers in use: $read_data"
  if [[ $read_data -gt 3000 ]]; then
    echo " - ### Packet Buffer Underrun - System crashed ###"
  fi;
  if [[ $read_data -gt 2700 && $read_data -lt 3001 ]]; then
    echo " - Possibly tail dropping received packets"
  fi;
  
  tn_ll_mmi_read $C_BASE_ADDR_COMMON_LOWER $C_SUB_ADDR_COMMON_BUFFER_FULL
  printf "Packet Buffer Full: %d\n" $(($read_data & 1))
  if [[ $((($read_data & 0x00FF0000) >> 16)) -eq 3 ]]; then
    echo   "RX Datapaths ready.";
  else
    printf "RX Datapaths experience backpressure: 0x%x\n" $((($read_data & 0x00FF0000) >> 24))
  fi
  if [[ $((($read_data & 0xFF000000) >> 24)) -eq 3 ]]; then
    echo   "TX Datapaths ready.";
  else
    printf "TX Datapaths experience backpressure: 0x%x\n" $((($read_data & 0xFF000000) >> 24))
  fi
  
  tn_ll_mmi_read $C_BASE_ADDR_COMMON_LOWER $C_SUB_ADDR_COMMON_TXF_FULL
  if [[ $read_data -eq 0 ]]; then
    echo   "TX FIFO Full Port Bitmap (current): 0";
  else
    printf "TX FIFO Full Port Bitmap (current): 0x%08x\n" $read_data
  fi
  
  echo -e "\n### Current MMI status:"
  let    status=`TNbar1 $(($C_BASE_ADDR_BM*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_BM_MMI_STATUS))   | cut -d " " -f 6`             
  let     err_acc_rd=$((($status & 1) >> 0))                                                                   
  let     err_acc_wr=$((($status & 2) >> 1))                                                                  
  let    err_time_rd=$((($status & 4) >> 2))                                                                  
  let    err_time_wr=$((($status & 8) >> 3))
  let        err_cnt=$((($status & 0xFFF0) >> 4))
  let        acc_cnt=$((($status & 0xFFFF0000) >> 16))
  printf "MMI Statistics: 0x%04x accesses, thereof 0x%03x failing (Errors: AccessErrRd=%d,AccessErrWr=%d,TimeoutRd=%d,TimeoutWr=%d)\n" $acc_cnt $err_cnt $err_acc_rd $err_acc_wr $err_time_rd $err_time_wr
  
  let interrupt=`TNbar1 $(($C_BASE_ADDR_BM*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_BM_MMI_INTERRUPT))   | cut -d " " -f 6`
  printf "MMI Interrupt register is 0x%08x\n" $interrupt
  if [[ $addrmap -eq 0xEEEEEEEE ]]; then
    echo " ** MMI Read Timeout **"
  fi
  
  if [[ $1 -eq 2 ]]; then
    tn_ll_mmi_read $C_BASE_ADDR_COMMON_LOWER $C_SUB_ADDR_COMMON_CLK_MON_L
    let sys_if_freq=$read_data
    if [[ $sys_if_freq -ne 0 ]]; then
      let last_clk=$C_SUB_ADDR_COMMON_CLK_MON_H-$C_SUB_ADDR_COMMON_CLK_MON_L
      let last_clk=$last_clk/4-1
      echo "Clock frequencies in relation to 125 MHz PCIe MMI frequency:"
      for clk in `seq 0 $last_clk`; do
        tn_ll_mmi_read $C_BASE_ADDR_COMMON_LOWER $(($C_SUB_ADDR_COMMON_CLK_MON_L+4*$clk))
        let clk_freq=$read_data
        case $clk in
          0)
            printf "* SysIfClk:  %d (~1.5625 MHz)\n" $clk_freq
            ;;
          1)
            printf "* Pinheader: %d\n" $clk_freq
            ;;
          2)
            printf "* PMod:      %d\n" $clk_freq
            ;;
          3)
            printf "* FC_CAM:    %d (~100 MHz)\n" $clk_freq
            ;;
          4)
            printf "* SYS_RTC:   %d (~125 MHz)\n" $clk_freq
            ;;
          5)
            printf "* TX_CLK90:  %d (125 MHz, +/- 6250)\n" $clk_freq
            ;;
          6)
            printf "* SGMII:     %d (125 MHz, +/- 6250)\n" $clk_freq
            ;;
          7)
            printf "* TX_CLK:    %d (125 MHz, +/- 6250)\n" $clk_freq
            ;;
          8)
            printf "* ACCDP:     %d (~200 MHz)\n" $clk_freq
            ;;
          9)
            printf "* 200MHzREF: %d (~200 MHz)\n" $clk_freq
            ;;
         10)
            printf "* GPHY0:     %d (2.5/25/125 MHz, +/- 125/1250/6250)\n" $clk_freq
            ;;
         11)
            printf "* GPHY1:     %d (2.5/25/125 MHz, +/- 125/1250/6250)\n" $clk_freq
            ;;
         12)
            printf "* GPHY2:     %d (2.5/25/125 MHz, +/- 125/1250/6250)\n" $clk_freq
            ;;
         13)
            printf "* GPHY3:     %d (2.5/25/125 MHz, +/- 125/1250/6250)\n" $clk_freq
            ;;
         14)
            printf "* GPHY4:     %d (2.5/25/125 MHz, +/- 125/1250/6250)\n" $clk_freq
            ;;
         15)
            printf "* GPHY5:     %d (2.5/25/125 MHz, +/- 125/1250/6250)\n" $clk_freq
            ;;
         16)
            printf "* GPHY6:     %d (2.5/25/125 MHz, +/- 125/1250/6250)\n" $clk_freq
            ;;
         17)
            printf "* GPHY7:     %d (2.5/25/125 MHz, +/- 125/1250/6250)\n" $clk_freq
            ;;
         18)
            printf "* GPHY8:     %d (2.5/25/125 MHz, +/- 125/1250/6250)\n" $clk_freq
            ;;
         19)
            printf "* GPHY9:     %d (2.5/25/125 MHz, +/- 125/1250/6250)\n" $clk_freq
            ;;
         20)
            printf "* AlaskaA:   %d (2.5/25/125 MHz, +/- 125/1250/6250)\n" $clk_freq
            ;;
         21)
            printf "* AlaskaB:   %d (2.5/25/125 MHz, +/- 125/1250/6250)\n" $clk_freq
            ;;
         22)
            printf "* PLL GTP0:  %d (e.g., 125 MHz, +/- 12500)\n" $clk_freq
            ;;
         23)
            printf "* PLL SFP1:  %d (e.g., 125 MHz, +/- 12500)\n" $clk_freq
            ;;
         24)
            printf "* AXI_DP:    %d (~200 MHz)\n" $clk_freq
            ;;
          *)
            printf "* TBD(%d):   %d\n" $clk $clk_freq
            ;;
        esac
      done
    fi
  
    echo -e "\n### Reading system status from peripherals module"
  
    tn_ll_mmi_read $C_BASE_ADDR_PERIPH $C_SUB_ADDR_PERIPH_BUTTONS
    let peripherals_button_bitmap=$read_data
    let    button_front=$((($peripherals_button_bitmap & 0x0000FFFF)))
    let     button_user=$((($peripherals_button_bitmap & 0x00010000) >> 16))
    if [[ $button_front -eq 0 ]]; then
      echo   "Front Button Bitmap: 0";
    else
      printf "Front Button Bitmap: 0x%04x\n" $button_front
    fi
    echo   "User Button: $button_user"
    if [[ $peripherals_button_bitmap -eq 0xEEEEEEEE ]]; then
      echo " ** MMI Read Timeout **"
    fi
  
    tn_ll_mmi_read $C_BASE_ADDR_PERIPH $C_SUB_ADDR_PERIPH_GPIOS
    let peripherals_gpio_bitmap=$read_data
    let      pinheader1_gpio_in=$((($peripherals_gpio_bitmap & 0x000003FF)))
    let           pmod0_gpio_in=$((($peripherals_gpio_bitmap & 0x00003C00) >> 10))
    let     pinheader1_gpio_tri=$((($peripherals_gpio_bitmap & 0x03FF0000) >> 16))
    let          pmod0_gpio_tri=$((($peripherals_gpio_bitmap & 0x3C000000) >> 26))
    if [[ $pinheader1_gpio_in -eq 0 && $pinheader1_gpio_tri -eq 0 ]]; then
      echo   "Pinheader1 GPIO Bitmap: 0";
    else
      printf "Pinheader1 GPIO Bitmap: 0x%03x (Read Data Valid: 0x%03x)\n" $pinheader1_gpio_in $pinheader1_gpio_tri
    fi
    if [[ $pmod0_gpio_in -eq 0 && $pmod0_gpio_tri -eq 0 ]]; then
      echo   "PMOD0 GPIO Bitmap: 0";
    else
      printf "PMOD0 GPIO Bitmap: 0x%x (Read Data Valid: 0x%x)\n" $pmod0_gpio_in $pmod0_gpio_tri
    fi
    if [[ $peripherals_gpio_bitmap -eq 0xEEEEEEEE ]]; then
      echo " ** MMI Read Timeout **"
    fi
  
    echo -e "\n### Reading interrupt status from MDIO module"
  
    tn_ll_mmi_read $C_BASE_ADDR_MDIO $C_SUB_ADDR_MDIO_INTERRUPT
    let peripherals_interrupt_bitmap=$read_data
    let                 gphy_md_ints=$((($peripherals_interrupt_bitmap & 0x000003FF)))
    let              alaska_ptp_ints=$((($peripherals_interrupt_bitmap & 0x00000C00) >> 10))
    let    alaska_ptp_event_triggers=$((($peripherals_interrupt_bitmap & 0x00030000) >> 16))
    if [[ $gphy_md_ints -eq 0 ]]; then
      echo   "GPHY MDIO Interrupt Bitmap: 0";
    else
      printf "GPHY MDIO Interrupt Bitmap: 0x%03x\n" $gphy_md_ints
    fi
    if [[ $alaska_ptp_ints -eq 0 ]]; then
      echo   "Alaska PTP Interrupt Bitmap: 0";
    else
      printf "Alaska PTP Interrupt Bitmap: 0x%x\n" $alaska_ptp_ints
    fi
    if [[ $alaska_ptp_event_triggers -eq 0 ]]; then
      echo   "Alaska PTP Event Trigger Bitmap: 0";
    else
      printf "Alaska PTP Event Trigger Bitmap: 0x%x\n" $alaska_ptp_event_triggers
    fi
    if [[ $peripherals_interrupt_bitmap -eq 0xEEEEEEEE ]]; then
      echo " ** MMI Read Timeout **"
    fi
  
    echo -e "\n### Displaying active interrupts on HC modules"
  
    for prt in `seq 0 11`; do
      let hcbase=C_BASE_ADDR_HC_$prt
      tn_ll_mmi_read $hcbase $C_SUB_ADDR_HC_INTERRUPT
      if [[ $read_data -gt 0 ]]; then
        printf "* Port $prt: %d\n" $read_data
      fi
    done
    for prt in `seq 0 1`; do
      let hcbase=C_BASE_ADDR_HC_PCIE_$prt
      tn_ll_mmi_read $hcbase $C_SUB_ADDR_HC_INTERRUPT
      if [[ $read_data -gt 0 ]]; then
        printf "* PCIe Port $prt: %d\n" $read_data
      fi
    done
	
	echo "Done"
  fi
fi
