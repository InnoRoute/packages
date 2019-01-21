#!/bin/bash
source /usr/share/InnoRoute/tn_env.sh
source /usr/share/InnoRoute/tn_func_ll.sh

sleep 5
mask=0xfffff
logall=0
TXF_FULL_last=0
TXF_ACT_last=0
TXF_error_last=0
TXF_FULL=0
TXF_ACT=0
TXF_error=0
PKT_BUF_full=0
PKT_BUF_full_last=0
tn_ll_mmi_read $C_BASE_ADDR_COMMON_LOWER $C_SUB_ADDR_COMMON_TN_MAJOR_REV
echo "$read_data"
while true
do
	TXF_FULL_last=$TXF_FULL
	TXF_ACT_last=$TXF_ACT
	PKT_BUF_full_last=$PKT_BUF_full
	tn_ll_mmi_read $C_BASE_ADDR_COMMON_LOWER $C_SUB_ADDR_COMMON_BUFFER_FULL
	let PKT_BUF_full=$(($read_data & 1))
	TXF_error_last=$(( $TXF_FULL_last & ~$TXF_ACT_last ))
	tn_ll_mmi_read $C_BASE_ADDR_COMMON_LOWER $C_SUB_ADDR_COMMON_TXF_FULL
	let TXF_FULL=$read_data
	tn_ll_mmi_read $C_BASE_ADDR_COMMON_LOWER $C_SUB_ADDR_COMMON_TXF_ACTIVITY
	let TXF_ACT=$read_data
	TXF_error=$(( $TXF_FULL & ~$TXF_ACT ))
	sleep 0.5
	state=$(( $mask & $(cat /proc/TN_MMI/TN_COMint) ))
	datum=$(date "+%F-%T")
	sleep 0.1
	state2=$(( $mask & $(cat /proc/TN_MMI/TN_COMint) ))
	echo "-----------------------------"
	echo "TXF_error_last:$TXF_error_last TXF_error:$TXF_error"
	echo "TXF_FULL:$TXF_FULL TXF_FULL_last:$TXF_FULL_last"
	echo "TXF_ACT:$TXF_ACT TXF_ACT_last:$TXF_ACT_last"
	if [ "$state" -gt "0" ] || [ "$(( $TXF_error & $TXF_error_last))" -gt "0" ] || [ "$(( $PKT_BUF_full & $PKT_BUF_full_last))" -gt "0" ]; then
		#reconfigure FPGA
		echo "$datum FPGA fail interrupt:$state TXF_err:$TXF_error PKT_BUF:$PKT_BUF_full" >> /var/log/TN_FPGA_fail.log
		
		if [ "$state" -eq "4" ] && [ "$state2" -eq "0" ] ; then
			echo ""
			#just shortly full buffer, keep cool....
		else
			if [ "$logall" -gt "0" ]; then
				TN_fpga_status.sh 2 >>/var/log/TN_FPGA_fail_info_$datum.log
				 TN_tm.sh 0 >> /var/log/TN_FPGA_fail_info_$datum.log
				 TN_statistics.sh 2 >> /var/log/TN_FPGA_fail_info_$datum.log
				 TN_statistics.sh 5 >> /var/log/TN_FPGA_fail_info_$datum.log
			fi
			TNchangemod 8
			#reset flowcache and TAS shadow mem
			rm /tmp/INR_* 
			/etc/init.d/TN_flowdump stop &
			/etc/init.d/TN_flowdump start &
			/usr/share/InnoRoute/FlowCache_persistent.sh
		fi	
		
	fi	

done
