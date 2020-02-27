#!/bin/bash

source /usr/share/InnoRoute/tn_env.sh
source /usr/share/InnoRoute/tn_func_ll.sh
bitmask=0xffffff
if [[ $# -ne 1 ]]; then
  echo 'please provide an parameter eg "1"'
else
  if [ -z "$(lspci -mm -d 10ee:0000)" ]; then echo "No PCIe connection to FPGA. Exiting."; exit 1; fi

	tn_ll_mmi_read $C_BASE_ADDR_COMMON_LOWER $C_SUB_ADDR_COMMON_FEATURES_NET
	let net_speed=$(( 0x2 & $read_data ))
	
	if [[  "$net_speed" -gt "0" ]]; then
		exit 0
	else
		exit 1
	fi

exit 0
fi
