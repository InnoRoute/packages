#!/bin/bash

source /usr/share/InnoRoute/tn_env.sh
source /usr/share/InnoRoute/tn_func_ll.sh
bitmask=0xffffff
if [[ $# -ne 1 ]]; then
  echo 'please provide an parameter eg "1"'
else
  if [ -z "$(lspci -mm -d 10ee:0000)" ]; then echo "No PCIe connection to FPGA. Exiting."; exit 1; fi

	tn_ll_mmi_read $C_BASE_ADDR_NET_LOWER $C_SUB_ADDR_NET_SPEED
	let net_speed=$(( $bitmask & $read_data ))
	tn_ll_mmi_read $C_BASE_ADDR_NET_LOWER $C_SUB_ADDR_NET_SPEED_DEBUG1
	let net_speed_dbg1=$(( $bitmask & $read_data ))
	tn_ll_mmi_read $C_BASE_ADDR_NET_LOWER $C_SUB_ADDR_NET_SPEED_DEBUG2
	let net_speed_dbg2=$(( $bitmask & $read_data ))
	if [[ ( "$net_speed" -eq "$net_speed_dbg1" && "$net_speed" -eq "$net_speed_dbg2" && "$net_speed_dbg1" -eq "$net_speed_dbg2" ) ]]; then
		exit 0
	else
		printf "ERROR: PHY settings not matching speed:%x dbg1:%x dbg2:%x\n" $net_speed $net_speed_dbg1 $net_speed_dbg2
		exit 1
	fi

exit 0
fi
