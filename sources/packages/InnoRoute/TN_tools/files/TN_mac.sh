#!/bin/bash

source /usr/share/InnoRoute/tn_env.sh
source /usr/share/InnoRoute/tn_func_ll.sh

if [[ $# -lt 2 || $# -gt 4 ]]; then
#  echo "$0 <MAC> [<enable>] [<speed>] [<pause>] [<duplex>] [<addr>] is used to change"
  echo "$0 <MAC> [<enable>] [<speed>] [<pause>] is used to change"
  echo "the settings of the Ethernet MACs. The parameters are:"
  echo "  <MAC>    is used to select the MAC {0..11, all}, with 'all' for all ports"
  echo "  <enable> can be 0 (disabled reception) or 1 (enabled reception)"
  echo "  <speed>  can be 0 (10 Mbps), 1 (100 Mbps), or 2 (1000 Mbps)"
  echo "  <pause>  can be 0 (RX PAUSE frames not processed) or 1 (RX PAUSE enabled)"
#  echo "  <duplex>  can be 0 (Half Duplex) or 1 (Full Duplex)"
#  echo "  <addr>   provides the MAC address of the device as a hexadecimal string,"
#  echo "           e.g., in case of 00:11:22:33:44:55 <addr> is 001122334455"
#  echo "  <6tree>  TBD"
else
  if [ -z "$(lspci -mm -d 10ee:0000)" ]; then echo "No PCIe connection to FPGA. Exiting."; exit 1; fi

  let   port=$1
  let enable=$2
  if [[ $# -gt 2 ]]; then
    let speed=$3
  fi
  if [[ $# -gt 3 ]]; then
    let pause=$4
  fi
  #if [[ $# -gt 4 ]]; then
  #  let address=$5
  #fi

  if [[ $port -eq all ]]; then
    if [[ $# -gt 2 ]]; then
      echo "Disabling all MACs"
      tn_ll_mmi_write $C_BASE_ADDR_NET_LOWER $C_SUB_ADDR_NET_ENABLE 0x00000000

      case $speed in
        0)
          echo "Setting Speed 10 Mbps for all ports"
          tn_ll_mmi_write $C_BASE_ADDR_NET_LOWER $C_SUB_ADDR_NET_SPEED 0x00000000
          tn_ll_mmi_read $C_BASE_ADDR_NET_LOWER $C_SUB_ADDR_NET_SPEED
    			let physpeed=$read_data
    			if [[ $physpeed -ne 0x00000000 ]]; then
    				echo "error phy spped mismatch set:0x00000000 read:$physpeed" >> /var/log/TN_FPGA_fail.log
    			fi
          ;;
        1)
          echo "Setting Speed 100 Mbps for all ports"
          tn_ll_mmi_write $C_BASE_ADDR_NET_LOWER $C_SUB_ADDR_NET_SPEED 0x55555555
          tn_ll_mmi_read $C_BASE_ADDR_NET_LOWER $C_SUB_ADDR_NET_SPEED
    			let physpeed=$read_data
    			if [[ $physpeed -ne 0x55555555 ]]; then
    				echo "error phy spped mismatch set:0x55555555 read:$physpeed" >> /var/log/TN_FPGA_fail.log
    			fi
          ;;
        2)
          echo "Setting Speed 1000 Mbps for all ports"
          tn_ll_mmi_write $C_BASE_ADDR_NET_LOWER $C_SUB_ADDR_NET_SPEED 0xAAAAAAAA
          tn_ll_mmi_read $C_BASE_ADDR_NET_LOWER $C_SUB_ADDR_NET_SPEED
    			let physpeed=$read_data
    			if [[ $physpeed -ne 0xAAAAAAAA ]]; then
    				echo "error phy spped mismatch set:0xAAAAAAAA read:$physpeed" >> /var/log/TN_FPGA_fail.log
    			fi
          ;;
        *)
          echo "$0: Parameter error"
          exit
          ;;
      esac
    fi

    if [[ $# -gt 3 ]]; then
      if [[ $pause -eq 1 ]]; then
        echo "Enabling Pause for all ports"
        tn_ll_mmi_write $C_BASE_ADDR_NET_LOWER $C_SUB_ADDR_NET_PAUSE 0x0000FFFF
      else
        echo "Disabling Pause for all ports"
        tn_ll_mmi_write $C_BASE_ADDR_NET_LOWER $C_SUB_ADDR_NET_PAUSE 0x00000000
      fi
    fi

#    if [[ $# -gt 4 ]]; then
#      echo "Setting MAC Duplex"
#      tn_ll_mmi_write $C_BASE_ADDR_NET_LOWER $C_SUB_ADDR_NET_DUPLEX XXXXXX
#    fi

#    if [[ $# -gt 5 ]]; then
#      echo "Setting MAC Address"
#      tn_ll_mmi_write $C_BASE_ADDR_NET_LOWER $C_SUB_ADDR_NET_OWN_MAC_ADDR_L XXXXXX
#      tn_ll_mmi_write $C_BASE_ADDR_NET_LOWER $C_SUB_ADDR_NET_OWN_MAC_ADDR_H XXXXXX
#    fi

    if [[ $enable -eq 1 ]]; then
      echo "Enabling all MACs"
      tn_ll_mmi_write $C_BASE_ADDR_NET_LOWER $C_SUB_ADDR_NET_ENABLE 0x0000FFFF
    else
      echo "Disabling all MACs"
      tn_ll_mmi_write $C_BASE_ADDR_NET_LOWER $C_SUB_ADDR_NET_ENABLE 0x00000000
    fi

    echo "Done"
  elif [[ $port -ge 0 && $port -lt 16 ]]; then
    if [[ $# -gt 2 ]]; then
      echo "Disabling MAC"
      tn_ll_mmi_read $C_BASE_ADDR_NET_LOWER $C_SUB_ADDR_NET_ENABLE
      let bitmap=$(( $read_data ^ (1 << $port) ))
      tn_ll_mmi_write $C_BASE_ADDR_NET_LOWER $C_SUB_ADDR_NET_ENABLE $bitmap

      echo "Setting MAC SPEED"
      tn_ll_mmi_read $C_BASE_ADDR_NET_LOWER $C_SUB_ADDR_NET_SPEED
      case $speed in
        0)
          echo "Setting Speed 10 Mbps for all ports - individual speed setting not yet supported"
          tn_ll_mmi_write $C_BASE_ADDR_NET_LOWER $C_SUB_ADDR_NET_SPEED 0x00000000
          ;;
        1)
          echo "Setting Speed 100 Mbps for all ports - individual speed setting not yet supported"
          tn_ll_mmi_write $C_BASE_ADDR_NET_LOWER $C_SUB_ADDR_NET_SPEED 0x55555555
          ;;
        2)
          echo "Setting Speed 1000 Mbps for all ports - individual speed setting not yet supported"
          tn_ll_mmi_write $C_BASE_ADDR_NET_LOWER $C_SUB_ADDR_NET_SPEED 0xAAAAAAAA
          ;;
        *)
          echo "$0: Parameter error"
          exit
          ;;
      esac
      tn_ll_mmi_write $C_BASE_ADDR_NET_LOWER $C_SUB_ADDR_NET_SPEED $bitmap
    fi

    if [[ $# -gt 3 ]]; then
      echo "Setting MAC PAUSE"
      tn_ll_mmi_read $C_BASE_ADDR_NET_LOWER $C_SUB_ADDR_NET_PAUSE
      if [[ $pause -eq 1 ]]; then
        let bitmap=$(( $read_data | (1 << $port) ))
      else
        let bitmap=$(( $read_data ^ (1 << $port) ))
      fi
      tn_ll_mmi_write $C_BASE_ADDR_NET_LOWER $C_SUB_ADDR_NET_PAUSE $bitmap
    fi

#    if [[ $# -gt 4 ]]; then
#      echo "Setting MAC Duplex"
#      tn_ll_mmi_write $C_BASE_ADDR_NET_LOWER $C_SUB_ADDR_NET_DUPLEX XXXXXX
#    fi

#    if [[ $# -gt 5 ]]; then
#      echo "Setting MAC Address"
#      tn_ll_mmi_write $C_BASE_ADDR_NET_LOWER $C_SUB_ADDR_NET_OWN_MAC_ADDR_L XXXXXX
#      tn_ll_mmi_write $C_BASE_ADDR_NET_LOWER $C_SUB_ADDR_NET_OWN_MAC_ADDR_H XXXXXX
#    fi

    echo "Setting MAC Enable"
    tn_ll_mmi_read $C_BASE_ADDR_NET_LOWER $C_SUB_ADDR_NET_ENABLE
    if [[ $enable -eq 1 ]]; then
      let bitmap=$(( $read_data | (1 << $port) ))
    else
      let bitmap=$(( $read_data ^ (1 << $port) ))
    fi
    tn_ll_mmi_write $C_BASE_ADDR_NET_LOWER $C_SUB_ADDR_NET_ENABLE $bitmap

    echo "Done"
  else
    echo "$0: Parameter error"
    exit
  fi
fi
