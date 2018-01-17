#!/bin/bash

source /usr/share/InnoRoute/tn_env.sh

for gphy in `seq 0 9`; do
  let link_state=`TNbar1 $((($C_BASE_ADDR_RGMII+$gphy)*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_RGMII_CLK_SPEED)) | cut -d " " -f 6`
  if [[ $link_state -eq 0xEEEEEEEE ]]; then
    echo " ** MMI Read Timeout **"
  fi
  if [[ $link_state -eq 3 ]]; then
    echo -e "GPHY $gphy is down"
  else
    let speed=`TNbar1 $((($C_BASE_ADDR_RGMII+$gphy)*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_RGMII_CLK_SPEED)) | cut -d " " -f 6`+1
    if [[ $speed -eq 0xEEEEEEEE ]]; then
      echo " ** MMI Read Timeout **"
    fi
    let duplex=`TNbar1 $((($C_BASE_ADDR_RGMII+$gphy)*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_RGMII_DPX_STAT)) | cut -d " " -f 6`
    if [[ $duplex -eq 0xEEEEEEEE ]]; then
      echo " ** MMI Read Timeout **"
    fi
	if [[ $duplex -eq 1 ]]; then
      echo -e "GPHY $gphy is up: 10^$speed Mbps, Full Duplex";
	else
      echo -e "GPHY $gphy is up: 10^$speed Mbps, Half Duplex";
	fi
  fi
done
for alaska in `seq 10 11`; do
  let link_state=`TNbar1 $((($C_BASE_ADDR_RGMII+$alaska)*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_RGMII_LINK_STAT)) | cut -d " " -f 6`
  if [[ $link_state -eq 0xEEEEEEEE ]]; then
    echo " ** MMI Read Timeout **"
  fi
  if [[ $link_state -eq 0 ]]; then
    echo -e "Alaska $alaska is down"
  else
    let speed=`TNbar1 $((($C_BASE_ADDR_RGMII+$alaska)*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_RGMII_CLK_SPEED)) | cut -d " " -f 6`+1
    if [[ $speed -eq 0xEEEEEEEE ]]; then
      echo " ** MMI Read Timeout **"
    fi
    let duplex=`TNbar1 $((($C_BASE_ADDR_RGMII+$alaska)*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_RGMII_DPX_STAT)) | cut -d " " -f 6`
    if [[ $duplex -eq 0xEEEEEEEE ]]; then
      echo " ** MMI Read Timeout **"
    fi
	if [[ $duplex -eq 1 ]]; then
      echo -e "Alaska $alaska is up: 10^$speed Mbps, Full Duplex";
	else
      echo -e "Alaska $alaska is up: 10^$speed Mbps, Half Duplex";
	fi
  fi
done
