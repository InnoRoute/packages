#!/bin/bash
time=`date +%Y%m%d%H%M%S-%s`
echo "Dumping relevant system information to diag_$time.tgz on the first/Windows-accessible partition"
folder=$(mktemp -d)
cp /usr/share/InnoRoute/*.log $folder
cp /usr/share/InnoRoute/*.conf $folder
cp /usr/share/InnoRoute/*.info $folder			
cp ~/.ash_history $folder
cp /var/log/* $folder
cp /usr/share/InnoRoute/TNsysrepostorage.xml $folder
dmesg > $folder/dmesg
TN_fpga_status.sh 2 > $folder/fpga_status.log
TN_statistics.sh 1 > $folder/fpga_statistics.log
#To be fixed: TN_mmi_hist.sh 0 > $folder/mmi_hist.log
TN_ovs_rstp_info.sh > $folder/ovs_rstp.log       
TN_ovs_stp_info.sh > $folder/ovs_stp.log
TN_pll_dump.sh 0; cp pll_dump.txt $folder
TN_pll_status.sh 0 > $folder/pll_status.log
TNethswitchdump > $folder/eth_shadow_table.txt
TN_pcie.sh 0 > $folder/pcie.log
TN_usb.sh 0 > $folder/usb.log
TN_phy_dump.sh 0; cp /tmp/gphy* $folder; cp /tmp/alaska* $folder
TN_tm.sh 0 > $folder/tm.log
TN_sys_ctrl_status.sh 0 > $folder/sys_ctrl_status.log
TN_phy_interrupts.sh 0 > $folder/phy_int.log
TN_version.sh 0 > $folder/versions.log
lsmod > $folder/modules.log
ps -aef > $folder/processes.log
cat /proc/meminfo > $folder/memory.log
uptime > $folder/uptime.log
tar cvfz /boot/diag_$time.tgz $folder > /dev/null
echo "Done"
