#!/bin/bash

source /usr/share/InnoRoute/tn_env.sh
source /usr/share/InnoRoute/tn_func_ll.sh

if [[ $# -ne 1 ]]; then
  echo "$0 <ex> is used to output information on the Traffic Manager"
  echo "The parameter <ex> can have any value. Without it, this help is displayed"
else
  if [ -z "$(lspci -mm -d 10ee:0000)" ]; then echo "No PCIe connection to FPGA. Exiting."; exit 1; fi


  tn_ll_mmi_read $C_BASE_ADDR_TM_LOWER $C_SUB_ADDR_TM_QUEUE_THRES_FULL
  printf "Packet Buffer Queue Threshold: %d\n" $read_data

  tn_ll_mmi_read $C_BASE_ADDR_COMMON_LOWER $C_SUB_ADDR_COMMON_PARAM_BUF_CNT
  let buf_total=$read_data
  let buf_max=$buf_total-1
  let buf_thresh=$buf_total-300
  let buf_in_use=0
  for buf in `seq 0 $buf_max`; do
    echo -n "."
	tn_ll_mmi_read $C_BASE_ADDR_TM_DEBUG_LOWER $(($buf * 4))
    # 5 bits dbg_pkt_buf_qm.flow_id_rx_s(0) - from pkt_buf_in.qm_msg_s
    let dbg_queue=$(( $read_data & 0x01F ))
    # 5 bits dbg_pkt_buf_qm.oport_id_rx_s(0) - from pkt_buf_in.qm_msg_s
    let dbg_oport=$(( ($read_data >> 5) & 0x1F ))
    # valid, i.e., used at least once
    let dbg_valid=$(( ($read_data >> 10) & 0x1 ))
    if [[ $dbg_valid -ne 0 ]]; then
      #echo "State of buffer $buf is p$dbg_oport->q$dbg_queue"
      let buf_in_use+=1;
      case $dbg_queue in
        0)
          let prio0[$dbg_oport]=${prio0[$dbg_oport]}+1
          ;;
        1)
          let prio1[$dbg_oport]=${prio1[$dbg_oport]}+1
          ;;
        2)
          let prio2[$dbg_oport]=${prio2[$dbg_oport]}+1
          ;;
        3)
          let prio3[$dbg_oport]=${prio3[$dbg_oport]}+1
          ;;
        4)
          let prio4[$dbg_oport]=${prio4[$dbg_oport]}+1
          ;;
        5)
          let prio5[$dbg_oport]=${prio5[$dbg_oport]}+1
          ;;
        6)
          let prio6[$dbg_oport]=${prio6[$dbg_oport]}+1
          ;;
        7)
          let prio7[$dbg_oport]=${prio7[$dbg_oport]}+1
          ;;
        *)
          let priox[$dbg_oport]=${priox[$dbg_oport]}+1
          ;;
      esac
    #else
    #  echo "State of buffer $buf is unused"
    fi
  done

  echo -e "\n$buf_in_use buffers are in use (individual buffer state counts)"

  for oport in `seq 0 31`; do
    if [[ -n ${prio0[$oport]} || -n ${prio1[$oport]} || -n ${prio2[$oport]} || -n ${prio3[$oport]} || -n ${prio4[$oport]} || -n ${prio5[$oport]} || -n ${prio6[$oport]} || -n ${prio7[$oport]} || -n ${priox[$oport]} ]]; then
	  echo " - Port $oport:"
      if [[ -n ${prio0[$oport]} ]]; then
        printf "   Queue 0:  %d\n" ${prio0[$oport]}
	  fi
      if [[ -n ${prio1[$oport]} ]]; then
        printf "   Queue 1:  %d\n" ${prio1[$oport]}
	  fi
      if [[ -n ${prio2[$oport]} ]]; then
        printf "   Queue 2:  %d\n" ${prio2[$oport]}
	  fi
      if [[ -n ${prio3[$oport]} ]]; then
        printf "   Queue 3:  %d\n" ${prio3[$oport]}
	  fi
      if [[ -n ${prio4[$oport]} ]]; then
        printf "   Queue 4:  %d\n" ${prio4[$oport]}
	  fi
      if [[ -n ${prio5[$oport]} ]]; then
        printf "   Queue 5:  %d\n" ${prio5[$oport]}
	  fi
      if [[ -n ${prio6[$oport]} ]]; then
        printf "   Queue 6:  %d\n" ${prio6[$oport]}
	  fi
      if [[ -n ${prio7[$oport]} ]]; then
        printf "   Queue 7:  %d\n" ${prio7[$oport]}
	  fi
      if [[ -n ${priox[$oport]} ]]; then
        printf "   Queue 8+: %d\n" ${priox[$oport]}
	  fi
	fi
  done
 
  tn_ll_mmi_read $C_BASE_ADDR_TM_LOWER $C_SUB_ADDR_TM_PTR_CNT
  echo   "Packet Buffers in use: $read_data (free list state)"
  if [[ $read_data -gt $buf_total ]]; then
    echo " - ### Packet Buffer Underrun - System crashed ###"
  fi;
  if [[ $read_data -gt $buf_thresh && $read_data -le $buf_total ]]; then
    echo " - Packet buffer full - possibly tail dropping newly received packets"
  fi;

  tn_ll_mmi_read $C_BASE_ADDR_COMMON_LOWER $C_SUB_ADDR_COMMON_BUFFER_FULL
  printf "Packet Buffer Full: %d\n" $(($read_data & 1))

#  tn_ll_mmi_read $C_BASE_ADDR_TM_LOWER $C_SUB_ADDR_TM_DEBUG_CNT_BUF0
#  if [[ $status -eq 0 ]]; then
#    echo   "TM Drop Count (Buffer Full, DP0): $read_data"
#  fi;
#  tn_ll_mmi_read $C_BASE_ADDR_TM_LOWER $C_SUB_ADDR_TM_DEBUG_CNT_BUF1
#  if [[ $status -eq 0 ]]; then
#    echo   "TM Drop Count (Buffer Full, DP1): $read_data"
#  fi;
#  tn_ll_mmi_read $C_BASE_ADDR_TM_LOWER $C_SUB_ADDR_TM_DEBUG_CNT_BAD0
#  if [[ $status -eq 0 ]]; then
#    echo   "TM Drop Count (Bad Frame, DP0):   $read_data"
#  fi;
#  tn_ll_mmi_read $C_BASE_ADDR_TM_LOWER $C_SUB_ADDR_TM_DEBUG_CNT_BAD1
#  if [[ $status -eq 0 ]]; then
#    echo   "TM Drop Count (Bad Frame, DP1):   $read_data"
#  fi;
#  tn_ll_mmi_read $C_BASE_ADDR_TM_LOWER $C_SUB_ADDR_TM_DEBUG_CNT_QUE0
#  if [[ $status -eq 0 ]]; then
#    echo   "TM Drop Count (Queue Full, DP0):  $read_data"
#  fi;
#  tn_ll_mmi_read $C_BASE_ADDR_TM_LOWER $C_SUB_ADDR_TM_DEBUG_CNT_QUE1
#  if [[ $status -eq 0 ]]; then
#    echo   "TM Drop Count (Queue Full, DP1):  $read_data"
#  fi;
  
  echo "Done"
fi
