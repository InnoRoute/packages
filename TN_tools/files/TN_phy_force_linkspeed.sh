#!/bin/bash

source /usr/share/InnoRoute/tn_env.sh
source /usr/share/InnoRoute/tn_func_ll.sh

if [[ $# -ne 1 ]]; then
  echo "$0 <speed> is used to set a specific link speed in the PHYs and MACs."
  echo "The parameter <speed> can be 0 (10 Mbps), 1 (100 Mbps), or 2 (1000 Mbps)"
  echo "The setting applies to all ports"
else
  if [ -z "$(lspci -mm -d 10ee:0000)" ]; then echo "No PCIe connection to FPGA. Exiting."; exit 1; fi

  echo "Hard resetting PHYs"
  /usr/share/InnoRoute/TN_phy_reset.sh 1
  echo "Disabling RX MACs ..."
  /usr/share/InnoRoute/TN_mac.sh all 0
  
  case $1 in
    0) echo "Setting 10 Mbps"
       # 10 FD only
       let write_data_4=0x0C41
       # not 1000
       let write_data_9=0x0000
      ;;
    1) echo "Setting 100 Mbps"
       # 100 FD only
       let write_data_4=0x0D01
       # not 1000
       let write_data_9=0x0000
      ;;
    2) echo "Setting 1000 Mbps"
       # not 100, not 10
       let write_data_4=0x0C01
       # 1000 FD only
       let write_data_9=0x0200
      ;;
    *) echo "$0: Parameter error"
       exit
      ;;
  esac
  
  echo "Writing to PHYs and restarting AN ..."
  for phy in `seq 0 11`; do 
    tn_ll_write_phy $phy 0 4 $write_data_4
    tn_ll_write_phy $phy 0 9 $write_data_9
    tn_ll_write_phy $phy 0 0 0x1200
  done

  echo "Changing MAC settings ..."
  /usr/share/InnoRoute/TN_mac.sh all 1 $1
  
  echo "Done"
fi
