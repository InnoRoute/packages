#!/bin/bash
source /usr/share/InnoRoute/tn_env.sh
source /usr/share/InnoRoute/tn_func_ll.sh

if [[ $# == 0 ]]; then
  echo "This script print currente mmi accesses"
  
else
	while true
	do
	tn_ll_mmi_read $C_BASE_ADDR_BM $C_SUB_ADDR_BM_MMI_ACCESSES
	if [ "$(( $read_data & 0x80000000 ))" -gt "0" ]; then
	printf "%x\n" $read_data
	fi

	done


fi
