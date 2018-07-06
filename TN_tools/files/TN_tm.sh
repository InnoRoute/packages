#!/bin/bash

source /usr/share/InnoRoute/tn_env.sh
source /usr/share/InnoRoute/tn_func_ll.sh

if [[ $# -ne 1 ]]; then
  echo "$0 <ex> is used to output information on the Traffic Manager"
  echo "The parameter <ex> can have any value. Without it, this help is displayed"
else
  tn_ll_mmi_read $C_BASE_ADDR_TM $C_SUB_ADDR_TM_QUEUE_THRES_FULL
  printf "Packet Buffer Queue Threshold: %d\n" $read_data

  let buf_in_use=0
  for buf in `seq 0 2999`; do
    tn_ll_mmi_read $C_BASE_TM_DEBUG_LOWER $(($buf * 4))
    let dbg_queue=$(( $read_data & 0x01F ))
    let dbg_oport=$(( $read_data & 0x3E0 ))
    let dbg_valid=$(( $read_data & 0x400 ))
    if [[ $dbg_valid -ne 0 ]]; then
      echo "State of buffer $buf is p$dbg_oport->q$dbg_queue"
	  let buf_in_use+=1;
    fi
  done;
  echo "$buf_in_use buffers are in use (individual buffer state counts)"

  tn_ll_mmi_read $C_BASE_ADDR_TM $C_SUB_ADDR_TM_PTR_CNT
  echo   "Packet Buffers in use: $read_data (free list state)"
  if [[ $read_data -gt 3000 ]]; then
    echo " - ### Packet Buffer Underrun - System crashed ###"
  fi;
  if [[ $read_data -gt 2700 && $read_data -lt 3001 ]]; then
    echo " - Packet buffer full - possibly tail dropping newly received packets"
  fi;

  tn_ll_mmi_read $C_BASE_ADDR_COMMON_LOWER $C_SUB_ADDR_COMMON_BUFFER_FULL
  printf "Packet Buffer Full: %d\n" $(($read_data & 1))

  echo "Done"
fi
