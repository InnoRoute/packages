#!/bin/bash

source /usr/share/InnoRoute/tn_env.sh
source /usr/share/InnoRoute/tn_func_ll.sh

if [[ $# == 0 ]]; then
  echo "$0 <enable> is used to enable or disable FPGA-level physical port default loopback"
  echo "Valid values of <enable> are 0 (by default all traffic is sent to the processor),"
  echo "and 1 (by default all traffic is sent back to the incomming port). Both settings"
  echo "are the default values that can be overridden by FlowCache+ActionUnit or by EthSwitch"
else
  if [ -z "$(lspci -mm -d 10ee:0000)" ]; then echo "No PCIe connection to FPGA. Exiting."; exit 1; fi

  if [[ $1 -eq 1 ]]; then
    echo "Default forwarding from physical ports is now looping back on FPGA level"
    tn_ll_mmi_write $C_BASE_ADDR_COMMON_LOWER $C_SUB_ADDR_COMMON_FEATURES_FLOWCACHE 0x00
  else
    echo "Default forwarding from physical ports is now forwarding to processor"
    tn_ll_mmi_write $C_BASE_ADDR_COMMON_LOWER $C_SUB_ADDR_COMMON_FEATURES_FLOWCACHE 0x10
  fi
  echo ".. except, if overridden by 6Tree, processor, FlowCache,"
  echo "Ethernet Switch (if DST MAC isn't own MAC), or FlowID 0 entry in ActionUnit"

  echo "Done"
fi
