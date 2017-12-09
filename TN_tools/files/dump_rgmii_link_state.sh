#!/bin/bash

source /usr/share/InnoRoute/tn_env.sh

for gphy in `seq 0 9`; do
  if [[ `TNbar1 $((($C_BASE_ADDR_RGMII+$gphy)*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_RGMII_CLK_SPEED)) | cut -d ":" -f 2 | cut -d " " -f 2` -eq 3 ]]; then
    echo -e "GPHY $gphy is down"
  else
    let speed=`TNbar1 $((($C_BASE_ADDR_RGMII+$gphy)*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_RGMII_CLK_SPEED)) | cut -d ":" -f 2 | cut -d " " -f 2`+1
    let duplex=`TNbar1 $((($C_BASE_ADDR_RGMII+$gphy)*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_RGMII_DPX_STAT)) | cut -d ":" -f 2 | cut -d " " -f 2`
	if [[ $duplex -eq 1 ]]; then
      echo -e "GPHY $gphy is up: 10^$speed Mbps, Full Duplex";
	else
      echo -e "GPHY $gphy is up: 10^$speed Mbps, Half Duplex";
	fi
  fi
done
for alaska in `seq 10 11`; do
  if [[ `TNbar1 $((($C_BASE_ADDR_RGMII+$alaska)*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_RGMII_LINK_STAT)) | cut -d ":" -f 2 | cut -d " " -f 2` -eq 0 ]]; then
    echo -e "Alaska $alaska is down"
  else
    let speed=`TNbar1 $((($C_BASE_ADDR_RGMII+$alaska)*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_RGMII_CLK_SPEED)) | cut -d ":" -f 2 | cut -d " " -f 2`+1
    let duplex=`TNbar1 $((($C_BASE_ADDR_RGMII+$alaska)*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_RGMII_DPX_STAT)) | cut -d ":" -f 2 | cut -d " " -f 2`
	if [[ $duplex -eq 1 ]]; then
      echo -e "Alaska $alaska is up: 10^$speed Mbps, Full Duplex";
	else
      echo -e "Alaska $alaska is up: 10^$speed Mbps, Half Duplex";
	fi
  fi
done
