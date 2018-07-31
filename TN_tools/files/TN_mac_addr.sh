#!/bin/bash

source /usr/share/InnoRoute/tn_env.sh
source /usr/share/InnoRoute/tn_func_ll.sh

if [[ $# -lt 1 || $# -gt 1 ]]; then
#  echo "$0 <MAC> [<enable>] [<speed>] [<pause>] [<duplex>] [<addr>] is used to change"
  echo "$0 <own_mac_addr> is used to change"
  echo "Just one own mac address is supported actually"
  
else
	tn_ll_mmi_write $C_BASE_ADDR_NET_LOWER $C_SUB_ADDR_NET_OWN_MAC_ADDR_L $(( $1 & 0xffffffff ))
	tn_ll_mmi_write $C_BASE_ADDR_NET_LOWER $C_SUB_ADDR_NET_OWN_MAC_ADDR_H $((( $1 & 0xffff00000000 ) >> 32))
fi
