#!/bin/bash

source /usr/share/InnoRoute/tn_env.sh
source /usr/share/InnoRoute/tn_func_ll.sh

if [[ $# == 0 ]]; then
  echo "$0 <ex> is used to scan the PCIe device list for the FPGA endpoint"
  echo "The parameter <ex> can have any value. Without it, this help is displayed"
else
  echo "Enable FPGA BARs"
  setpci -s `lspci -n -d 10ee:* | cut -d " " -f 1` COMMAND=0x02
  
  echo "Rescanning PCIe (this might hang the system, if the FPGA is improperly configured)"
  echo 1 > /sys/bus/pci/rescan
  
  echo "Filtered PCIe status"
  lspci -nn -vvv -d 10ee:*

  echo "Done"
fi
