#!/bin/bash

source /usr/share/InnoRoute/tn_env.sh

# System Controller has SMBus address 0x04

echo "### TrustNode Versions:"

let mainboard=`i2cget -y 0 4 0x07`
case $mainboard in
0)
  echo "* Main Board V1.1" ;;
1)
  echo "* Main Board V1.2";;
*)
  echo "* Main Board Version unknown";;
esac


printf "* System Controller Firmware Version %d\n" `i2cget -y 0 4 0x00`

let    major=`TNbar1 $(($C_BASE_ADDR_COMMON_LOWER*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_TN_MAJOR_REV))   | cut -d " " -f 6`
let    minor=`TNbar1 $(($C_BASE_ADDR_COMMON_LOWER*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_TN_MINOR_REV))   | cut -d " " -f 6`
let     user=`TNbar1 $(($C_BASE_ADDR_COMMON_LOWER*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_USER_REV))       | cut -d " " -f 6`
echo   "* TrustNode FPGA design revision is $major.$minor, with a user sub-version of $user"
if [[ $major -eq 0xEEEEEEEE || $minor -eq 0xEEEEEEEE || $user -eq 0xEEEEEEEE ]]; then
  echo " ** MMI Read Timeout **"
fi

let       day=$(((`TNbar1 $(($C_BASE_ADDR_COMMON_LOWER*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_BITGEN_TIME)) | cut -d " " -f 6` & 0xF8000000) >> 27))
let     month=$(((`TNbar1 $(($C_BASE_ADDR_COMMON_LOWER*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_BITGEN_TIME)) | cut -d " " -f 6` & 0x07800000) >> 23))
let      year=$(((`TNbar1 $(($C_BASE_ADDR_COMMON_LOWER*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_BITGEN_TIME)) | cut -d " " -f 6` & 0x007E0000) >> 17))
let      hour=$(((`TNbar1 $(($C_BASE_ADDR_COMMON_LOWER*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_BITGEN_TIME)) | cut -d " " -f 6` & 0x0001F000) >> 12))
let    minute=$(((`TNbar1 $(($C_BASE_ADDR_COMMON_LOWER*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_BITGEN_TIME)) | cut -d " " -f 6` & 0x00000FC0) >> 6))
let    second=$(((`TNbar1 $(($C_BASE_ADDR_COMMON_LOWER*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_BITGEN_TIME)) | cut -d " " -f 6` & 0x0000003F) >> 0))
printf   "  It was generated on %02d.%02d.20%02d %02d:%02d:%02d\n" $day $month $year $hour $minute $second
if [[ $day -eq 0xEEEEEEEE || $month -eq 0xEEEEEEEE || $year -eq 0xEEEEEEEE || $hour -eq 0xEEEEEEEE || $minute -eq 0xEEEEEEEE || $second -eq 0xEEEEEEEE ]]; then
  echo " ** MMI Read Timeout **"
fi

let    addrmap=`TNbar1 $(($C_BASE_ADDR_COMMON_LOWER*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_ADDR_MAP_REV)) | cut -d " " -f 6`
echo   "* MMI address map revision is $addrmap"
if [[ $addrmap -eq 0xEEEEEEEE ]]; then
  echo " ** MMI Read Timeout **"
fi

printf "* Linux image: "; cat /etc/banner | grep Trust
printf "  -> "; cat /proc/version

echo "### TrustNode ID:"

let    fpgaid0=`TNbar1 $(($C_BASE_ADDR_COMMON_LOWER*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_FPGA_ID0)) | cut -d " " -f 6`
let    fpgaid1=`TNbar1 $(($C_BASE_ADDR_COMMON_LOWER*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_FPGA_ID1)) | cut -d " " -f 6`
printf "* FPGA Device DNA is 0x%07x%08x\n" $fpgaid1 $fpgaid0
if [[ $fpgaid0 -eq 0xEEEEEEEE || $fpgaid1 -eq 0xEEEEEEEE ]]; then
  echo " ** MMI Read Timeout **"
fi

echo "### Configuration:"

let    feature_bitmap=`TNbar1 $(($C_BASE_ADDR_COMMON_LOWER*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_FEATURES_6TREE)) | cut -d " " -f 6`
let               st_en=$((($feature_bitmap &  1) >> 0))
let              st_mmi=$((($feature_bitmap &  2) >> 1))
let    st_uplink_bitmap=$((($feature_bitmap     ) >> 16))
printf "* 6Tree Enabled: $st_en (Display(0) or MMI(1):$st_mmi; Uplink Port Bitmap: 0x%04x)\n" $st_uplink_bitmap
if [[ $feature_bitmap -eq 0xEEEEEEEE ]]; then
  echo " ** MMI Read Timeout **"
fi

let    feature=`TNbar1 $(($C_BASE_ADDR_COMMON_LOWER*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_FEATURES_ETH_SW)) | cut -d " " -f 6`
echo   "* Ethernet Switch Enabled: $feature"
if [[ $feature -eq 0xEEEEEEEE ]]; then
  echo " ** MMI Read Timeout **"
fi
let    feature_bitmap=`TNbar1 $(($C_BASE_ADDR_COMMON_LOWER*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_FEATURES_FLOWCACHE)) | cut -d " " -f 6`
let             fc_en=$((($feature_bitmap &  1) >> 0))
let         fc_emh_en=$((($feature_bitmap &  2) >> 1))
let          fc_ls_en=$((($feature_bitmap &  4) >> 2))
let         fc_ema_en=$((($feature_bitmap &  8) >> 3))
let   default_is_proc=$((($feature_bitmap & 16) >> 4))
echo   "Flow Cache Enabled: $fc_en (EMH:$fc_emh_en, LinSearch:$fc_ls_en, EMA:$fc_ema_en, DefaultIsProc:$default_is_proc)"
if [[ $feature_bitmap -eq 0xEEEEEEEE ]]; then
  echo " ** MMI Read Timeout **"
fi
let    feature_bitmap=`TNbar1 $(($C_BASE_ADDR_COMMON_LOWER*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_FEATURES_ACC_DP)) | cut -d " " -f 6`
let          accdp_en=$((($feature_bitmap &          1) >>  0))
let         accdp_cnt=$((($feature_bitmap &     0xFF00) >>  8))
let     accdp_entries=$((($feature_bitmap & 0xFFFF0000) >> 16))
echo   "Acceleration Datapath Enabled: $accdp_en (AccDP Count:$accdp_cnt, AccDP Entry Count:$accdp_entries)"
if [[ $feature_bitmap -eq 0xEEEEEEEE ]]; then
  echo " ** MMI Read Timeout **"
fi
let    feature=`TNbar1 $(($C_BASE_ADDR_COMMON_LOWER*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_FEATURES_TX_ROUTER)) | cut -d " " -f 6`
echo   "* TX Route Enabled: $feature"
if [[ $feature -eq 0xEEEEEEEE ]]; then
  echo " ** MMI Read Timeout **"
fi
let    feature=`TNbar1 $(($C_BASE_ADDR_COMMON_LOWER*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_FEATURES_SFP)) | cut -d " " -f 6`
echo   "* SFP Interface Enabled: $feature"
if [[ $feature -eq 0xEEEEEEEE ]]; then
  echo " ** MMI Read Timeout **"
fi
let    feature=`TNbar1 $(($C_BASE_ADDR_COMMON_LOWER*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_FEATURES_SODIMM)) | cut -d " " -f 6`
echo   "* SODIMM Interface Enabled: $feature"
if [[ $feature -eq 0xEEEEEEEE ]]; then
  echo " ** MMI Read Timeout **"
fi

let    param=`TNbar1 $(($C_BASE_ADDR_COMMON_LOWER*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_PARAM_PRT_CNT)) | cut -d " " -f 6`
echo   "* Physical Port Count: $param"
if [[ $param -eq 0xEEEEEEEE ]]; then
  echo " ** MMI Read Timeout **"
fi
let    param=`TNbar1 $(($C_BASE_ADDR_COMMON_LOWER*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_PARAM_DP_CNT)) | cut -d " " -f 6`
echo   "* Datapath Count (RX/TX): $param"
if [[ $param -eq 0xEEEEEEEE ]]; then
  echo " ** MMI Read Timeout **"
fi
let    param=`TNbar1 $(($C_BASE_ADDR_COMMON_LOWER*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_PARAM_BUF_CNT)) | cut -d " " -f 6`
echo   "* Segment buffer count: $param"
if [[ $param -eq 0xEEEEEEEE ]]; then
  echo " ** MMI Read Timeout **"
fi
let    param=`TNbar1 $(($C_BASE_ADDR_COMMON_LOWER*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_PARAM_RX_MIN)) | cut -d " " -f 6`
echo   "* Minimum Frame Size (RX/TX): $param"
if [[ $param -eq 0xEEEEEEEE ]]; then
  echo " ** MMI Read Timeout **"
fi
let    param=`TNbar1 $(($C_BASE_ADDR_COMMON_LOWER*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_PARAM_RX_MAX)) | cut -d " " -f 6`
echo   "* Maximum Frame Size (RX): $param"
if [[ $param -eq 0xEEEEEEEE ]]; then
  echo " ** MMI Read Timeout **"
fi
let    param=`TNbar1 $(($C_BASE_ADDR_COMMON_LOWER*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_PARAM_TX_PRE)) | cut -d " " -f 6`
echo   "* TX Preamble Length: $param"
if [[ $param -eq 0xEEEEEEEE ]]; then
  echo " ** MMI Read Timeout **"
fi
let    param=`TNbar1 $(($C_BASE_ADDR_COMMON_LOWER*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_PARAM_TX_IFG)) | cut -d " " -f 6`
echo   "* TX Minimum Inter-Frame Gap: $param"
if [[ $param -eq 0xEEEEEEEE ]]; then
  echo " ** MMI Read Timeout **"
fi

let    param=`TNbar1 $(($C_BASE_ADDR_COMMON_LOWER*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_PARAM_DISPLAY)) | cut -d " " -f 6`
echo   "* Display Connection: $param"
if [[ $param -eq 0xEEEEEEEE ]]; then
  echo " ** MMI Read Timeout **"
fi
let    param=`TNbar1 $(($C_BASE_ADDR_COMMON_LOWER*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_PARAM_LED_SYNCE)) | cut -d " " -f 6`
echo   "* SyncE LED Connection: $param"
if [[ $param -eq 0xEEEEEEEE ]]; then
  echo " ** MMI Read Timeout **"
fi
let    param=`TNbar1 $(($C_BASE_ADDR_COMMON_LOWER*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_PARAM_LED_INTERNAL)) | cut -d " " -f 6`
echo   "* Internal LED Connection: $param"
if [[ $param -eq 0xEEEEEEEE ]]; then
  echo " ** MMI Read Timeout **"
fi
let    param=`TNbar1 $(($C_BASE_ADDR_COMMON_LOWER*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_PARAM_BEEP)) | cut -d " " -f 6`
echo   "* Beeper Connection: $param"
if [[ $param -eq 0xEEEEEEEE ]]; then
  echo " ** MMI Read Timeout **"
fi
let    param=`TNbar1 $(($C_BASE_ADDR_COMMON_LOWER*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_PARAM_PHY_RESET)) | cut -d " " -f 6`
echo   "* PHY Reset Connection: $param"
if [[ $param -eq 0xEEEEEEEE ]]; then
  echo " ** MMI Read Timeout **"
fi
let    param=`TNbar1 $(($C_BASE_ADDR_COMMON_LOWER*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_PARAM_PINHEADER)) | cut -d " " -f 6`
echo   "* Internal Big Pinheader Connection: $param"
if [[ $param -eq 0xEEEEEEEE ]]; then
  echo " ** MMI Read Timeout **"
fi
let    param=`TNbar1 $(($C_BASE_ADDR_COMMON_LOWER*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_PARAM_PMOD)) | cut -d " " -f 6`
echo   "* Internal PMod Connection: $param"
if [[ $param -eq 0xEEEEEEEE ]]; then
  echo " ** MMI Read Timeout **"
fi

let    param=`TNbar1 $(($C_BASE_ADDR_COMMON_LOWER*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_PARAM_EXT_BYPASS)) | cut -d " " -f 6`
echo   "* External Bypass active: $param"
if [[ $param -eq 0xEEEEEEEE ]]; then
  echo " ** MMI Read Timeout **"
fi

let    width=`TNbar1 $(($C_BASE_ADDR_COMMON_LOWER*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_WIDTH_HC_BAD)) | cut -d " " -f 6`
echo   "* MMI Header Creation Fragment Counter Width: $width"
if [[ $width -eq 0xEEEEEEEE ]]; then
  echo " ** MMI Read Timeout **"
fi
let    width=$((2**`TNbar1 $(($C_BASE_ADDR_COMMON_LOWER*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_WIDTH_ETH_SW)) | cut -d " " -f 6`))
echo   "* Number of Ethernet Switch Entries per Datapath: $width"
if [[ $width -eq 0xEEEEEEEE ]]; then
  echo " ** MMI Read Timeout **"
fi
let    width=`TNbar1 $(($C_BASE_ADDR_COMMON_LOWER*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_COMMON_WIDTH_RX_STAT_BAD)) | cut -d " " -f 6`
echo   "* RX Statistics Counter Width: $width"
if [[ $width -eq 0xEEEEEEEE ]]; then
  echo " ** MMI Read Timeout **"
fi
