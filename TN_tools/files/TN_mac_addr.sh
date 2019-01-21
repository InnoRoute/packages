#!/bin/bash

source /usr/share/InnoRoute/tn_env.sh
source /usr/share/InnoRoute/tn_func_ll.sh

if [[ $# -ne 1 ]]; then
  echo "$0 <own_mac_addr> is used to change"
  echo "Just one mac (start) address is supported"
  
else
  tn_ll_mmi_write $C_BASE_ADDR_NET_LOWER $C_SUB_ADDR_NET_OWN_MAC_ADDR_L $(( $1 & 0xffffffff ))
  tn_ll_mmi_write $C_BASE_ADDR_NET_LOWER $C_SUB_ADDR_NET_OWN_MAC_ADDR_H $((( $1 & 0xffff00000000 ) >> 32))
fi
