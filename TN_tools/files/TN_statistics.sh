#!/bin/bash

# Beware: Alaska PHY documentation uses this notation: REG_PAGE.High:Low

source /usr/share/InnoRoute/tn_env.sh
source /usr/share/InnoRoute/tn_func_ll.sh

if [[ $# == 0 ]]; then
  echo "$0 <verbosity> is used to read current system statistics."
  echo "The parameter <verbosity> can be 0 (few outputs) or 1 (many outputs) or"
  echo " 2 (many outputs, with waiting times)"
#  echo "$0 <verbosity> [<port>] is used to read current system statistics."
#TODO  echo "The parameter <port>, if given, limits the statistics to this specific port"
else

  echo "### PHY Autonegotiation States: please check against manually set MAC speed/duplex"
  let page=0x0
  let reg=0x18
  #echo "GPHY PAGE:$page, REG:$reg"
  echo " - GPHY ports:"
  for phy in `seq 0 9`; do
    let reg=0x18
    tn_ll_read_phy $phy $page $reg;
    ## check $read_data -> still busy?
    let speed=$(($read_data & 3))
    let duplex=$((($read_data >> 3) & 1))
    let reg=0x01
    tn_ll_read_phy $phy $page $reg;
    ## check $read_data -> still busy?
    # Speed=3 also indicates that the GPHY is down!
    if [[ $speed -eq 3 ]]; then
      let link=0
    else
      let link=$((($read_data >> 2) & 1))
    fi
    echo "Port=$phy, LinkUp=$link, Speed=$speed, Duplex=$duplex"
  done
  
  let page=0x0
  let reg=0x11
  #echo "Alaska PAGE:$page, REG:$reg"
  echo " - Alaska ports:"
  for phy in `seq 10 11`; do
    tn_ll_read_phy $phy $page $reg;
    ## check $read_data -> still busy?
    let valid=$((($read_data >> 11) & 1))
    let speed=$((($read_data >> 14) & 3))
    let duplex=$((($read_data >> 13) & 1))
    # Speed=3 also indicates the Alaska is down!
    if [[ $speed -eq 3 ]]; then
      let link=0
    else
    let link=$((($read_data >> 10) & 1))
    fi
    echo "Port=$phy, Valid=$valid, LinkUp=$link, Speed=$speed, Duplex=$duplex"
  done
  
  echo " - MACs:"
  tn_ll_mmi_read $C_BASE_ADDR_NET_LOWER $C_SUB_ADDR_NET_ENABLE
  let enable=$read_data
  tn_ll_mmi_read $C_BASE_ADDR_NET_LOWER $C_SUB_ADDR_NET_SPEED
  let speed=$read_data
  tn_ll_mmi_read $C_BASE_ADDR_NET_LOWER $C_SUB_ADDR_NET_DUPLEX
  let duplex=$read_data
  for mac in `seq 0 11`; do
    let mac_enable=$((($enable >> $mac) & 1))
    let mac_speed=$((($speed   >> 2*$mac) & 3))
    let mac_duplex=$((($duplex >> $mac) & 1))
    echo "Port=$mac, Enabled=$mac_enable, Speed=$mac_speed, Duplex=$mac_duplex"
  done

  if [[ $1 > 0 ]]; then
    if [[ $1 == 2 ]]; then
      echo -e "\n### Please start the traffic NOW"
	fi
  
    echo -e "\n### GPHY: RX Packet Counters (saturating at 255, non-zero only)"
    let page=0
    let reg=0x15
    ## Select RX error counter:
    ##let write_data=0x0000
    # Select RX frame counter:
    let write_data=0x0100
    ## Select ESD error counter:
    ##let write_data=0x0200
    ## Select SSD error counter:
    ##let write_data=0x0300
    ## Select TX error counter:
    ##let write_data=0x0400
    ## Select TX frame counter:
    ##let write_data=0x0500
    ## Select collision counter:
    ##let write_data=0x0600
    #echo "GPHY PAGE:$page, REG:$reg"
    for phy in `seq 0 9`; do
      tn_ll_write_phy $phy $page $reg $write_data;
      ## check $read_data -> still busy?
    done
    if [[ $1 == 2 ]]; then
      echo "Waiting for 10 seconds to aggregate statistics"
      sleep 10
    fi
    #echo "GPHY PAGE:$page, REG:$reg"
    let total=0
    for phy in `seq 0 9`; do
      tn_ll_read_phy $phy $page $reg;
      ## check $read_data -> still busy? + read data
      if [ $(($read_data & 0xFF)) -ne 0 ]; then
        printf "PHY $phy: %d packets received\n" $(($read_data & 0xFF))
        let total+=$(($read_data & 0xFF))
      fi
    done
    echo "Total RX: $total packets"
  
    echo -e "\n### GPHY: RX Error Counters (saturating at 255, non-zero only)"
    let page=0
    let reg=0x15
    # Select RX error counter:
    let write_data=0x0000
    ## Select RX frame counter:
    ##let write_data=0x0100
    ## Select ESD error counter:
    ##let write_data=0x0200
    ## Select SSD error counter:
    ##let write_data=0x0300
    ## Select TX error counter:
    ##let write_data=0x0400
    ## Select TX frame counter:
    ##let write_data=0x0500
    ## Select collision counter:
    ##let write_data=0x0600
    #echo "GPHY PAGE:$page, REG:$reg"
    for phy in `seq 0 9`; do
      tn_ll_write_phy $phy $page $reg $write_data;
      ## check $read_data -> still busy?
    done
    if [[ $1 == 2 ]]; then
      echo "Waiting for 10 seconds to aggregate statistics"
      sleep 10
    fi
    #echo "GPHY PAGE:$page, REG:$reg"
    let total=0
    for phy in `seq 0 9`; do
      tn_ll_read_phy $phy $page $reg;
      ## check $read_data -> still busy? + read data
      if [ $(($read_data & 0xFF)) -ne 0 ]; then
        printf "PHY $phy: %d errors received\n" $(($read_data & 0xFF))
        let total+=$(($read_data & 0xFF))
      fi
    done
    echo "Total RX Errors: $total packets"
  
    echo -e "\n### GPHY: RX ESD Error Counters (saturating at 255, non-zero only)"
    let page=0
    let reg=0x15
    ## Select RX error counter:
    ##let write_data=0x0000
    ## Select RX frame counter:
    ##let write_data=0x0100
    # Select ESD error counter:
    let write_data=0x0200
    ## Select SSD error counter:
    ##let write_data=0x0300
    ## Select TX error counter:
    ##let write_data=0x0400
    ## Select TX frame counter:
    ##let write_data=0x0500
    ## Select collision counter:
    ##let write_data=0x0600
    #echo "GPHY PAGE:$page, REG:$reg"
    for phy in `seq 0 9`; do
      tn_ll_write_phy $phy $page $reg $write_data;
      ## check $read_data -> still busy?
    done
    if [[ $1 == 2 ]]; then
      echo "Waiting for 10 seconds to aggregate statistics"
      sleep 10
    fi
    #echo "GPHY PAGE:$page, REG:$reg"
    let total=0
    for phy in `seq 0 9`; do
      tn_ll_read_phy $phy $page $reg;
      ## check $read_data -> still busy? + read data
      if [ $(($read_data & 0xFF)) -ne 0 ]; then
        printf "PHY $phy: %d ESD errors received\n" $(($read_data & 0xFF))
        let total+=$(($read_data & 0xFF))
	  fi
    done
    echo "Total RX ESD Errors: $total packets"
  
    echo -e "\n### GPHY: RX SSD Error Counters (saturating at 255, non-zero only)"
    let page=0
    let reg=0x15
    ## Select RX error counter:
    ##let write_data=0x0000
    ## Select RX frame counter:
    ##let write_data=0x0100
    ## Select ESD error counter:
    ##let write_data=0x0200
    # Select SSD error counter:
    let write_data=0x0300
    ## Select TX error counter:
    ##let write_data=0x0400
    ## Select TX frame counter:
    ##let write_data=0x0500
    ## Select collision counter:
    ##let write_data=0x0600
    #echo "GPHY PAGE:$page, REG:$reg"
    for phy in `seq 0 9`; do
      tn_ll_write_phy $phy $page $reg $write_data;
      ## check $read_data -> still busy?
    done
    if [[ $1 == 2 ]]; then
      echo "Waiting for 10 seconds to aggregate statistics"
      sleep 10
    fi
    #echo "GPHY PAGE:$page, REG:$reg"
    let total=0
    for phy in `seq 0 9`; do
      tn_ll_read_phy $phy $page $reg;
      ## check $read_data -> still busy? + read data
      if [ $(($read_data & 0xFF)) -ne 0 ]; then
        printf "PHY $phy: %d SSD errors received\n" $(($read_data & 0xFF))
        let total+=$(($read_data & 0xFF))
	  fi
    done
    echo "Total RX SSD Errors: $total packets"
  
    ## IDLE Error Count in the lower octet -> let reg=0x0a
    ## Dev 0x03 -> let reg=0x16:    EEE wake time faults
    ## Dev 0x1F -> let reg=0x1EA/B: EEE Link-Fail Counter
  
    echo -e "\n### Alaska: Copper-specific Receive Error Counters: false carrier or symbol error (saturating at 65535, non-zero only)"
	echo "Saturated at maximum, if not connected"
    let page=0
    let reg=0x15
    #echo "Alaska PAGE:$page, REG:$reg"
    let total=0
    for phy in `seq 10 11`; do
      tn_ll_read_phy $phy $page $reg;
      ## check $read_data -> still busy? + read data
      if [ $(($read_data & 0xFFFF)) -ne 0 ]; then
        printf "PHY $phy: %d errors received\n" $(($read_data & 0xFFFF))
        let total+=$(($read_data & 0xFFFF))
	  fi
    done
    echo "Total RX Errors: $total packets"
  
    echo -e "\n### Alaska: CRC Counters (saturating at 255, non-zero only)"
	echo "Saturated at maximum, if not connected"
    let page=0x12
    let reg=0x12
    # Select Copper Counters:
    let write_data=0x0002
    ## Clear Copper Counters:
    ##let write_data=0x0012
    ## Select RGMII Counters:
    ##let write_data=0x0006
    ## Clear RGMII Counters:
    ##let write_data=0x0016
    #echo "Alaska PAGE:$page, REG:$reg"
    for phy in `seq 10 11`; do
      tn_ll_write_phy $phy $page $reg $write_data;
      ## check $read_data -> still busy?
    done
    if [[ $1 == 2 ]]; then
      echo "Waiting for 10 seconds to aggregate statistics"
      sleep 10
    fi
    let page=0x12
    let reg=0x11
    let write_data=0x0012
    #echo "Alaska PAGE:$page, REG:$reg"
    let total=0
    let total_err=0
    for phy in `seq 10 11`; do
      # Read Counters
      let reg=0x11
      tn_ll_read_phy $phy $page $reg;
      ## check $read_data -> still busy? + read data
      if [ $(($read_data & 0xFFFF)) -ne 0 ]; then
        printf "PHY $phy: %d packets received (%d with CRC errors)\n" $((($read_data >> 8) & 0xFF)) $(($read_data & 0xFF))
        let total+=$(($read_data & 0xFF))
        let total_err+=$((($read_data >> 8) & 0xFF))
	  fi
      # Clear Counters
      let reg=0x12
      tn_ll_write_phy $phy $page $reg $write_data;
      ## check $read_data -> still busy?
    done
    echo "Total RX:        $total packets"
    echo "Total RX Errors: $total_err packets"

    #echo -e "\n### Alaska: Copper Port CRC Counters (saturating at 255, non-zero only)"
    ## Enable CRC Checking
    #let page=0x6
    #let reg=0x10
    #let write_data=0x0010
    ##echo "Alaska PAGE:$page, REG:$reg"
    #for phy in `seq 10 11`; do
    #  tn_ll_write_phy $phy $page $reg $write_data;
    #  ## check $read_data -> still busy?
    #done
    #if [[ $1 == 2 ]]; then
    #  echo "Waiting for 10 seconds to aggregate statistics"
    #  sleep 10
    #fi
    #let reg=0x11
    #let write_data=0x0010
    ##echo "Alaska PAGE:$page, REG:$reg"
    #let total=0
    #let total_err=0
    #for phy in `seq 10 11`; do
    #  # Read Counters
    #  let reg=0x11
    #  tn_ll_read_phy $phy $page $reg;
    #  ## check $read_data -> still busy? + read data
    #  if [ $(($read_data & 0xFFFF)) -ne 0 ]; then
    #    printf "PHY $phy (Copper): %d packets received (%d with CRC errors)\n" $((($read_data >> 8) & 0xFF)) $(($read_data & 0xFF))
    #    let total+=$(($read_data & 0xFF))
    #    let total_err+=$((($read_data >> 8) & 0xFF))
	#  fi
    #  # Clear Counters
    #  let reg=0x12
    #  tn_ll_write_phy $phy $page $reg $write_data;
    #  ## check $read_data -> still busy?
    #done
    #echo "Total RX:        $total packets"
    #echo "Total RX Errors: $total_err packets"
    ## IDLE Error Count in the lower octet -> let page=0; let reg=0x0a
    ## Dev 0x03 -> let reg=0x16: EEE wake time faults
    ## HD Copper Late Collision counters -> let page=0x06; let reg=0x17/0x18
    ## Link disconnect counter -> let page=0x12; let reg=0x19
    ## PTP HW-TS Discard Counter -> let page=0x09; let reg=0x05
  
    # Just to make sure that everything is fine: enable all MACs
    echo -e "\n### Enabling all MACs"
    tn_ll_mmi_write $C_BASE_ADDR_NET_LOWER $C_SUB_ADDR_NET_ENABLE 0xFFFFFFFF
  
    #echo "###############################################################"
    #echo "## Not displaying RGMII Tap values                           ##"
    #echo "###############################################################"
  
    #echo "###############################################################"
    #echo "## Missing: RX Activity Counter                              ##"
    #echo "###############################################################"
  fi

  echo -e "\n### Displaying non-zero RX Statistics (good packets only)"
  let total=0
  for i in `seq 0 15`; do
    tn_ll_mmi_read $C_BASE_ADDR_STATISTIC_GOOD_INPRT_LOWER_0 $(($i*4))
    if [ $read_data -ne 0 ]; then
      let total+=$read_data
      echo "Input Port $i (PHY): $read_data"
    fi
  done
  for i in `seq 0 15`; do
    tn_ll_mmi_read $C_BASE_ADDR_STATISTIC_GOOD_INPRT_LOWER_1 $(($i*4))
    if [ $read_data -ne 0 ]; then
      let total+=$read_data
      echo "Input Port $i (PHY): $read_data"
    fi
  done
  for i in `seq 16 31`; do
    tn_ll_mmi_read $C_BASE_ADDR_STATISTIC_GOOD_INPRT_LOWER_0 $(($i*4))
    if [ $read_data -ne 0 ]; then
      let total+=$read_data
      echo "Input Port $i (CPU): $read_data"
    fi
  done
  for i in `seq 16 31`; do
    tn_ll_mmi_read $C_BASE_ADDR_STATISTIC_GOOD_INPRT_LOWER_1 $(($i*4))
    if [ $read_data -ne 0 ]; then
      let total+=$read_data
      echo "Input Port $i (CPU): $read_data"
    fi
  done
  echo "Total Good Input Port: $total packets"

  echo -e "\n### Displaying non-zero fragment dropping counts at HC modules (saturating at 65535)"
  let total=0
  for i in `seq 0 11`; do
    let hcbase=C_BASE_ADDR_HC_$i
    tn_ll_mmi_read $hcbase $C_SUB_ADDR_HC_CNT_BAD_FRAMES
    if [[ $read_data -gt 0 ]]; then
      let total+=$read_data
      printf "* Port $i: %d\n" $read_data
    fi
  done
  for i in `seq 0 1`; do
    let hcbase=C_BASE_ADDR_HC_PCIE_$i
    tn_ll_mmi_read $hcbase $C_SUB_ADDR_HC_CNT_BAD_FRAMES
    if [[ $read_data -gt 0 ]]; then
      let total+=$read_data
      printf "* PCIe Port $i: %d\n" $read_data
    fi
  done
  echo "Total Fragments: $total packets"

  echo -e "\n### Displaying non-zero frame drop reason counts at RX Statistics"
  let total=0
  for prt in `seq 0 31`; do
    for badrsn in `seq 0 31`; do
      tn_ll_mmi_read $C_BASE_ADDR_STATISTIC_BADRSN_LOWER_0 $((($prt*32+$badrsn)*4))
      if [[ $read_data -gt 0 ]]; then
        let total+=$read_data
        printf "DP0 Port %d - BadReason %d: %d\n" $prt $badrsn $read_data
      fi
    done
  done
  for prt in `seq 0 31`; do
    for badrsn in `seq 0 31`; do
      tn_ll_mmi_read $C_BASE_ADDR_STATISTIC_BADRSN_LOWER_1 $((($prt*32+$badrsn)*4))
      if [[ $read_data -gt 0 ]]; then
        let total+=$read_data
        printf "DP1 Port %d - BadReason %d: %d\n" $prt $badrsn $read_data
      fi
    done
  done
  echo "Total Bad: $total packets"

  if [[ $1 > 0 ]]; then
    echo -e "\n### Displaying non-zero frame count matrix (input-output) at RX Statistics"
    let total=0
    for i in `seq 0 1023`; do
      tn_ll_mmi_read $C_BASE_ADDR_STATISTIC_PKT_CNT_LOWER_0 $(($i*4))
      if [ $read_data -ne 0 ]; then
        let inprt=$i%32
        let outprt=$i/32
        let total+=$read_data
		if [ $inprt -eq $outprt ]; then
          echo "DP0 counter $i ($inprt loopback): $read_data"
		elif [ $inprt -eq $(($outprt+16)) ]; then
          echo "DP0 counter $i (CPU->$outprt):     $read_data"
		elif [ $(($inprt+16)) -eq $outprt ]; then
          echo "DP0 counter $i ($inprt->CPU):     $read_data"
		else
          echo "DP0 counter $i ($inprt->$outprt):       $read_data"
		fi
      fi
    done
    for i in `seq 0 1023`; do
      tn_ll_mmi_read $C_BASE_ADDR_STATISTIC_PKT_CNT_LOWER_1 $(($i*4))
      if [ $read_data -ne 0 ]; then
        let inprt=$i%32
        let outprt=$i/32
        let total+=$read_data
		if [ $inprt -eq $outprt ]; then
          echo "DP1 counter $i ($inprt loopback): $read_data"
		elif [ $inprt -eq $(($outprt+16)) ]; then
          echo "DP1 counter $i (CPU->$outprt):     $read_data"
		elif [ $(($inprt+16)) -eq $outprt ]; then
          echo "DP1 counter $i ($inprt->CPU):     $read_data"
		else
          echo "DP1 counter $i ($inprt->$outprt):       $read_data"
		fi
      fi
    done
    echo "Total Matrix: $total packets"
  
    echo -e "\n### Displaying non-zero FlowID Match Statistics (good packets only)"
    let total=0
    for i in `seq 0 1023`; do
      tn_ll_mmi_read $C_BASE_ADDR_STATISTIC_GOOD_FLOWID_LOWER_0 $(($i*4))
      if [ $read_data -ne 0 ]; then
        let total+=$read_data
		if [ $i -eq 0 ]; then
          echo "DP0 FlowID $i (default): $read_data"
		else
          echo "DP0 FlowID $i:           $read_data"
		fi
      fi
    done
    for i in `seq 0 1023`; do
      tn_ll_mmi_read $C_BASE_ADDR_STATISTIC_GOOD_FLOWID_LOWER_1 $(($i*4))
      if [ $read_data -ne 0 ]; then
        let total+=$read_data
		if [ $i -eq 0 ]; then
          echo "DP1 FlowID $i (default): $read_data"
		else
          echo "DP1 FlowID $i:           $read_data"
		fi
      fi
    done
    echo "Total Good FlowID: $total packets"

	echo " "

    #echo "###############################################################"
    #echo "## Missing: Flow Counters (packets, volume)                  ##"
    #echo "###############################################################"
  
    # 6tree, bridging/routing, flow-overrides, uc/mc/bc, volumes, packet rates, data rates, ...
    #echo "###############################################################"
    #echo "## Missing: ETH Switch Counters (Matches, Flooding, ...)     ##"
    #echo "###############################################################"
    echo "DP0 Ethernet Switch:"
    tn_ll_mmi_read $C_BASE_ADDR_ETH_SW_0 $C_SUB_ADDR_ETH_SW_NUM_SOF          
    echo " - Number of SoF segments:                    $read_data"
    tn_ll_mmi_read $C_BASE_ADDR_ETH_SW_0 $C_SUB_ADDR_ETH_SW_NUM_LEARNED      
    echo " - Number of learned addresses:               $read_data"
    tn_ll_mmi_read $C_BASE_ADDR_ETH_SW_0 $C_SUB_ADDR_ETH_SW_NUM_AGED_OUT     
    echo " - Number of aged out addresses:              $read_data"
    tn_ll_mmi_read $C_BASE_ADDR_ETH_SW_0 $C_SUB_ADDR_ETH_SW_NUM_FLOOD_UNKNOWN
    echo " - Number of floods due to unknown addresses: $read_data"
    tn_ll_mmi_read $C_BASE_ADDR_ETH_SW_0 $C_SUB_ADDR_ETH_SW_NUM_FLOOD_FULL   
    echo " - Number of floods due to saturated CAM:     $read_data"
    tn_ll_mmi_read $C_BASE_ADDR_ETH_SW_0 $C_SUB_ADDR_ETH_SW_NUM_PORT_ERROR   
    echo " - Number of port mismatches:                 $read_data"
    #tn_ll_mmi_read $C_BASE_ADDR_ETH_SW_0 $C_SUB_ADDR_ETH_SW_NUM_MATCH_SRC <- needs selection of address ahead in time   
    #tn_ll_mmi_read $C_BASE_ADDR_ETH_SW_0 $C_SUB_ADDR_ETH_SW_NUM_MATCH_DST <- needs selection of address ahead in time   

    echo "DP1 Ethernet Switch:"
    tn_ll_mmi_read $C_BASE_ADDR_ETH_SW_1 $C_SUB_ADDR_ETH_SW_NUM_SOF          
    echo " - Number of SoF segments:                    $read_data"
    tn_ll_mmi_read $C_BASE_ADDR_ETH_SW_1 $C_SUB_ADDR_ETH_SW_NUM_LEARNED      
    echo " - Number of learned addresses:               $read_data"
    tn_ll_mmi_read $C_BASE_ADDR_ETH_SW_1 $C_SUB_ADDR_ETH_SW_NUM_AGED_OUT     
    echo " - Number of aged out addresses:              $read_data"
    tn_ll_mmi_read $C_BASE_ADDR_ETH_SW_1 $C_SUB_ADDR_ETH_SW_NUM_FLOOD_UNKNOWN
    echo " - Number of floods due to unknown addresses: $read_data"
    tn_ll_mmi_read $C_BASE_ADDR_ETH_SW_1 $C_SUB_ADDR_ETH_SW_NUM_FLOOD_FULL   
    echo " - Number of floods due to saturated CAM:     $read_data"
    tn_ll_mmi_read $C_BASE_ADDR_ETH_SW_1 $C_SUB_ADDR_ETH_SW_NUM_PORT_ERROR   
    echo " - Number of port mismatches:                 $read_data"
    #tn_ll_mmi_read $C_BASE_ADDR_ETH_SW_1 $C_SUB_ADDR_ETH_SW_NUM_MATCH_SRC <- needs selection of address ahead in time   
    #tn_ll_mmi_read $C_BASE_ADDR_ETH_SW_1 $C_SUB_ADDR_ETH_SW_NUM_MATCH_DST <- needs selection of address ahead in time   
  
    # Free length, Queue valid
    #echo "###############################################################"
    #echo "## Missing: TM Drop Counters (Buffer Full, Queue Threshold)  ##"
    #echo "###############################################################"
  
    # Total vs. bad per port, packets vs. volume
    #echo "###############################################################"
    #echo "## Missing: HT Counters                                      ##"
    #echo "###############################################################"
  
    #echo "###############################################################"
    #echo "## Missing: TX Activity Counter                              ##"
    #echo "###############################################################"
  
    echo -e "\n### GPHY: TX Packet Counters (saturating at 255, non-zero only)"
    let page=0
    let reg=0x15
    ## Select RX error counter:
    ##let write_data=0x0000
    ## Select RX frame counter:
    ##let write_data=0x0100
    ## Select ESD error counter:
    ##let write_data=0x0200
    ## Select SSD error counter:
    ##let write_data=0x0300
    ## Select TX error counter:
    ##let write_data=0x0400
    # Select TX frame counter:
    let write_data=0x0500
    ## Select collision counter:
    ##let write_data=0x0600
    #echo "GPHY PAGE:$page, REG:$reg"
    for phy in `seq 0 9`; do
      tn_ll_write_phy $phy $page $reg $write_data;
      ## check $read_data -> still busy?
    done
    if [[ $1 == 2 ]]; then
      echo "Waiting for 10 seconds to aggregate statistics"
      sleep 10
    fi
    #echo "GPHY PAGE:$page, REG:$reg"
    let total=0
    for phy in `seq 0 9`; do
      tn_ll_read_phy $phy $page $reg;
      ## check $read_data -> still busy? + read data
      if [ $(($read_data & 0xFF)) -ne 0 ]; then
        printf "PHY $phy: %d packets transmitted\n" $(($read_data & 0xFF))
        let total+=$(($read_data & 0xFF))
	  fi
    done
    echo "Total TX: $total packets"
    
    echo -e "\n### GPHY: TX Error Counters (saturating at 255, non-zero only)"
    let page=0
    let reg=0x15
    ## Select RX error counter:
    ##let write_data=0x0000
    ## Select RX frame counter:
    ##let write_data=0x0100
    ## Select ESD error counter:
    ##let write_data=0x0200
    ## Select SSD error counter:
    ##let write_data=0x0300
    # Select TX error counter:
    let write_data=0x0400
    ## Select TX frame counter:
    ##let write_data=0x0500
    ## Select collision counter:
    ##let write_data=0x0600
    #echo "GPHY PAGE:$page, REG:$reg"
    for phy in `seq 0 9`; do
      tn_ll_write_phy $phy $page $reg $write_data;
      ## check $read_data -> still busy?
    done
    if [[ $1 == 2 ]]; then
      echo "Waiting for 10 seconds to aggregate statistics"
      sleep 10
    fi
    #echo "GPHY PAGE:$page, REG:$reg"
    let total=0
    for phy in `seq 0 9`; do
      tn_ll_read_phy $phy $page $reg;
      ## check $read_data -> still busy? + read data
      if [ $(($read_data & 0xFF)) -ne 0 ]; then
        printf "PHY $phy: %d packets with errors transmitted\n" $(($read_data & 0xFF))
        let total+=$(($read_data & 0xFF))
	  fi
    done
    echo "Total TX Errors: $total packets"
    
    echo -e "\n### GPHY: TX Collision Counters (saturating at 255, non-zero only)"
    let page=0
    let reg=0x15
    ## Select RX error counter:
    ##let write_data=0x0000
    ## Select RX frame counter:
    ##let write_data=0x0100
    ## Select ESD error counter:
    ##let write_data=0x0200
    ## Select SSD error counter:
    ##let write_data=0x0300
    ## Select TX error counter:
    ##let write_data=0x0400
    ## Select TX frame counter:
    ##let write_data=0x0500
    # Select collision counter:
    let write_data=0x0600
    #echo "GPHY PAGE:$page, REG:$reg"
    for phy in `seq 0 9`; do
      tn_ll_write_phy $phy $page $reg $write_data;
      ## check $read_data -> still busy?
    done
    if [[ $1 == 2 ]]; then
      echo "Waiting for 10 seconds to aggregate statistics"
      sleep 10
    fi
    #echo "GPHY PAGE:$page, REG:$reg"
    let total=0
    for phy in `seq 0 9`; do
      tn_ll_read_phy $phy $page $reg;
      ## check $read_data -> still busy? + read data
      if [ $(($read_data & 0xFF)) -ne 0 ]; then
        printf "PHY $phy: %d collisions\n" $(($read_data & 0xFF))
        let total+=$(($read_data & 0xFF))
	  fi
    done
    echo "Total Collisions: $total packets"
    #echo "### No transmit counters for Alaskas"
  fi
  echo "Done"
fi  
