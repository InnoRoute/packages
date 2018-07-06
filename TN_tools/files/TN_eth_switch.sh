#!/bin/bash

source /usr/share/InnoRoute/tn_env.sh
source /usr/share/InnoRoute/tn_func_ll.sh

if [[ $# -ne 1 ]]; then
  echo "$0 <enable> is used to enable or disable the Ethernet switch"
  echo "The parameter <enable> can be 0 (disable) or 1 (enable)"
else
  # Set ageing time: Parmeter is half the time [s] minus 1, e.g. 149 for 300 seconds
  #tn_ll_mmi_write $C_BASE_ADDR_ETH_SW_0 $C_SUB_ADDR_ETH_SW_AGEING 149
  #tn_ll_mmi_write $C_BASE_ADDR_ETH_SW_1 $C_SUB_ADDR_ETH_SW_AGEING 149
  case $1 in
    0)
      echo "Disabling Ethernet Switch"
      tn_ll_mmi_write $C_BASE_ADDR_ETH_SW_0 $C_SUB_ADDR_ETH_SW_COMMON $1
      tn_ll_mmi_write $C_BASE_ADDR_ETH_SW_1 $C_SUB_ADDR_ETH_SW_COMMON $1
      echo "Done"
      ;;
    1)
      echo "Enabling Ethernet Switch"
      tn_ll_mmi_write $C_BASE_ADDR_ETH_SW_0 $C_SUB_ADDR_ETH_SW_COMMON $1
      tn_ll_mmi_write $C_BASE_ADDR_ETH_SW_1 $C_SUB_ADDR_ETH_SW_COMMON $1
      echo "Done"
      ;;
    *)
      echo "$0: Parameter error"
      ;;
  esac
fi
