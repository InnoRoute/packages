#!/bin/bash

source /usr/share/InnoRoute/tn_env.sh
source /usr/share/InnoRoute/tn_func_ll.sh

if [[ $# -ne 3 ]]; then
  echo "$0 <enEMH> <enLS> <enEMA> is used to enable/disable the FlowCache parts"
  echo "The parameters each can be 0 (disabled) or 1 (enabled)"
  echo " <enEMH>: Enable Hash-based FlowCache"
  echo " <enLS>:  Enable Linear Search-based FlowCache"
  echo " <enEMA>: Enable TCAM-based FlowCache"
else
  echo "Working on FlowCache"
  if [[ $1 -eq 1 ]]; then
    echo "Enabling EMH"
    let bitmap=1
  else
    echo "Disabling EMH"
    let bitmap=0
  fi
  if [[ $2 -eq 1 ]]; then
    echo "Enabling LS"
    let bitmap=$(($bitmap | 2))
  else
    echo "Disabling LS"
  fi
  if [[ $3 -eq 1 ]]; then
    echo "Enabling EMA"
    let bitmap=$(($bitmap | 4))
  else
    echo "Disabling EMA"
  fi
  tn_ll_mmi_write $C_BASE_ADDR_FLOW_CACHE $C_SUB_ADDR_FLOW_CACHE_ENABLE $bitmap

  echo "Done"
fi
