#!/bin/bash
source /usr/share/InnoRoute/tn_env.sh
source /usr/share/InnoRoute/tn_func_ll.sh

if [[ $# == 0 ]]; then
  echo "$0 <verbosity> is used to read current FPGA status,"
  echo "provided by the MMI_COMMON unit and others"
  echo "The parameter <verbosity> can between 0-3"
  echo "  0: Standard Output"
  echo "  1: Minimum Output"
  echo "  2: Maximum Output"
  echo "  3: Error status only"
else

if [ -z "$(lspci -mm -d 10ee:0000)" ]; then
  echo "No PCIe connection to FPGA. Exiting."
  exit 1
fi

if [[ $2 == 1 ]]; then
	let __print_machinereadable=1
	echo -n "{"
else
	let __print_machinereadable=0
fi

  ###>>> Param=0 or 1 or 2
  if [[ $1 -ne 3 ]]; then
  	if [[ __print_machinereadable -eq 0 ]]; then
   	 echo -e  "\n### COMMON_MMI status:"
   	fi

    
    tn_ll_mmi_read $C_BASE_ADDR_COMMON_LOWER $C_SUB_ADDR_COMMON_TN_MAJOR_REV
    let    major=$read_data
    tn_ll_mmi_read $C_BASE_ADDR_COMMON_LOWER $C_SUB_ADDR_COMMON_TN_MINOR_REV
    let    minor=$read_data
    tn_ll_mmi_read $C_BASE_ADDR_COMMON_LOWER $C_SUB_ADDR_COMMON_USER_REV
    let     user=$read_data
    if [[ __print_machinereadable -eq 0 ]]; then
    	echo     "TrustNode FPGA design revision is $major.$minor, with a user sub-version of $user"
    else
    	echo -n '"majorrevison":'$major',"minorrevision":'$minor',"subversion":'$user','
    fi

    tn_ll_mmi_read $C_BASE_ADDR_COMMON_LOWER $C_SUB_ADDR_COMMON_BITGEN_TIME
    let       day=$((($read_data & 0xF8000000) >> 27))
    let     month=$((($read_data & 0x07800000) >> 23))
    let      year=$((($read_data & 0x007E0000) >> 17))
    let      hour=$((($read_data & 0x0001F000) >> 12))
    let    minute=$((($read_data & 0x00000FC0) >> 6))
    let    second=$((($read_data & 0x0000003F) >> 0))
    if [[ __print_machinereadable -eq 0 ]]; then
    	printf   "Bitstream generation start was at %02d.%02d.20%02d %02d:%02d:%02d\n" $day $month $year $hour $minute $second
    else
    	echo -n '"bitstreamday":'$day',"bitstreammonth":'$month',"bitstreamyear":'$year',"bitstreamhour":'$hour',"bitstreamminute":'$minute',"bitstreamsecond":'$second','
    fi
    
    if [[ __print_machinereadable -eq 1 ]]; then
    	printf '"imagename":"'$(cat /usr/share/InnoRoute/unstablebitstream.info)'","bitstreamname":"'$(cat /usr/share/InnoRoute/image.info)'",'
    fi	

    tn_ll_mmi_read $C_BASE_ADDR_COMMON_LOWER $C_SUB_ADDR_COMMON_FPGA_ID0
    let    fpgaid0=$read_data
    tn_ll_mmi_read $C_BASE_ADDR_COMMON_LOWER $C_SUB_ADDR_COMMON_FPGA_ID1
    let    fpgaid1=$read_data
    if [[ __print_machinereadable -eq 0 ]]; then
    	printf   "Device DNA is 0x%07x%08x\n" $fpgaid1 $fpgaid0
	else
		printf	'"fpgaid":"0x%07x%08x",' $fpgaid1 $fpgaid0
	fi

    tn_ll_mmi_read $C_BASE_ADDR_COMMON_LOWER $C_SUB_ADDR_COMMON_EFUSE
    let    efuse=$read_data
    if [[ __print_machinereadable -eq 0 ]]; then
    	printf   "eFUSE key is  0x%08x\n" $efuse
    else
    	printf '"efuse":"0x%08x",' $efuse
	fi
    tn_ll_mmi_read $C_BASE_ADDR_COMMON_LOWER $C_SUB_ADDR_COMMON_FPGA_TEMP
    let    temp=$read_data*504/4096-273
    if [[ __print_machinereadable -eq 0 ]]; then
    	echo     "FPGA temperature is $temp degrees celsius"
    else
    	echo -n '"temperature":'$temp','
    fi
  fi

  ####
  # Stop here, if a parameter was given -> used on logging in
  if [[ $1 -eq 1 ]]; then
    ###>>> Param=1
    exit 0

  elif [[ $1 -ne 3 ]]; then
    ###>>> Param=0 or 2
	if [[ __print_machinereadable -eq 0 ]]; then
    	echo -e  "\n### Features & Parameters"
	fi
	
    ####
    tn_ll_mmi_read $C_BASE_ADDR_COMMON_LOWER $C_SUB_ADDR_COMMON_ADDR_MAP_REV
    let    addrmap=$(($read_data & 0x7FFFFFFF))
    if [[ __print_machinereadable -eq 0 ]]; then
    	echo     "MMI address map revision:    $addrmap"
    else
    	echo -n '"mmiaddress":'$addrmap','
    fi
    if [[ $(($read_data & 0x80000000)) -gt 0 ]]; then
    	if [[ __print_machinereadable -eq 0 ]]; then
      		echo   "PCIe NoC1 exchange:          provided and expected"
      	else
      		echo -n '"pcienoc1":"provided and expected",'
      	fi
    else
    	if [[ __print_machinereadable -eq 0 ]]; then
      		echo   "PCIe NoC1 exchange:          No"
      	else
      		echo -n '"pchienoc1":"No",'
      	fi
    fi

    tn_ll_mmi_read $C_BASE_ADDR_COMMON_LOWER $C_SUB_ADDR_COMMON_FEATURES_USER
    if [[ __print_machinereadable -eq 0 ]]; then
    	printf   "User Feature/bit:            0x%08x\n" $read_data
	else
		printf '"userfeaturebit":"0x%08x",' $read_data
	fi

    tn_ll_mmi_read $C_BASE_ADDR_COMMON_LOWER $C_SUB_ADDR_COMMON_FEATURES_6TREE
    let    feature_bitmap=$read_data
    let               st_en=$((($feature_bitmap &  1) >> 0))
    let              st_mmi=$((($feature_bitmap &  2) >> 1))
    let    st_uplink_bitmap=$((($feature_bitmap     ) >> 16))
    if [[ $st_en -eq 0 ]]; then
    	if [[ __print_machinereadable -eq 0 ]]; then
      		echo   "6Tree Enabled:               No"
      	else
      		echo -n '"treeenable":"No",'
      	fi
    else
    	if [[ __print_machinereadable -eq 0 ]]; then
     		echo   "6Tree Enabled:               Yes"
     	else
     		echo -n '"treeenable":"Yes",'
     	fi
      if [[ __print_machinereadable -eq 0 ]]; then
     	printf " - Display(0) or MMI(1):$st_mmi\n"
     	printf " - Uplink Port Bitmap: 0x%04x)\n" $st_uplink_bitmap
      else
      	printf '"displayormmi":'$st_mmi',"uplinkportbitmap":"0x%04x",' $st_uplink_bitmap
      fi
    fi
    #tn_ll_mmi_read $C_BASE_ADDR_NET_LOWER $C_SUB_ADDR_NET_6T_ADDR_L
    #tn_ll_mmi_read $C_BASE_ADDR_NET_LOWER $C_SUB_ADDR_NET_6T_ADDR_H
    #tn_ll_mmi_read $C_BASE_ADDR_NET_LOWER $C_SUB_ADDR_NET_6T_DST_PTR
    #tn_ll_mmi_read $C_BASE_ADDR_NET_LOWER $C_SUB_ADDR_NET_6T_SRC_PTR

    tn_ll_mmi_read $C_BASE_ADDR_COMMON_LOWER $C_SUB_ADDR_COMMON_FEATURES_ETH_SW
    if [[ $read_data -eq 0 ]]; then
    	if [[ __print_machinereadable -eq 0 ]]; then
      		echo   "Ethernet Switch Enabled:     No"
      	else
      		echo -n '"ethernetswitchenable":"No",'
      	fi
    else
    	if [[ __print_machinereadable -eq 0 ]]; then
      		echo   "Ethernet Switch Enabled:     Yes"
      	else
      		echo -n '"ethernetswitchenable":"Yes",'
      	fi
    fi
    tn_ll_mmi_read $C_BASE_ADDR_COMMON_LOWER $C_SUB_ADDR_COMMON_FEATURES_FLOWCACHE
    let    feature_bitmap=$read_data
    let             fc_en=$((($feature_bitmap &  1) >> 0))
    let         fc_emh_en=$((($feature_bitmap &  2) >> 1))
    let          fc_ls_en=$((($feature_bitmap &  4) >> 2))
    let         fc_ema_en=$((($feature_bitmap &  8) >> 3))
    let   default_is_proc=$((($feature_bitmap & 16) >> 4))
    if [[ $fc_en -eq 0 ]]; then
    	if [[ __print_machinereadable -eq 0 ]]; then
      		echo   "Flow Cache Enabled:          No"
      	else
      		echo -n '"flowcacheenable":"No",'
      	fi
    else
    	if [[ __print_machinereadable -eq 0 ]]; then
      		echo   "Flow Cache Enabled:          Yes"
      	else
      		echo -n '"flowcacheenable":"Yes",'
      	fi
      if [[ $fc_emh_en -eq 0 ]]; then
      	if [[ __print_machinereadable -eq 0 ]]; then
        	echo " - EMH (Hash):                No"
        else
        	echo -n '"emh":"No",'
        fi
      else
      	if [[ __print_machinereadable -eq 0 ]]; then
       		echo " - EMH (Hash):                Yes"
       	else
       		echo -n '"emh":"Yes",'
       	fi
      fi
      if [[ $fc_ema_en -eq 0 ]]; then
      	if [[ __print_machinereadable -eq 0 ]]; then
        	echo " - EMA (TCAM):                No"
        else
        	echo -n '"ema":"No",'
        fi
      else
      	if [[ __print_machinereadable -eq 0 ]]; then
        	echo " - EMA (TCAM):                Yes"
        else
        	echo -n '"ema":"Yes",'
        fi
      fi
      if [[ $fc_ls_en -eq 0 ]]; then
      	if [[ __print_machinereadable -eq 0 ]]; then
        	echo " - LS  (Linear Search):       No"
        else
        	echo -n '"ls":"No",'
        fi
      else
      	if [[ __print_machinereadable -eq 0 ]]; then
        	echo " - LS  (Linear Search):       Yes"
        else
        	echo -n '"ls":"Yes",'
        fi
      fi
      if [[ $default_is_proc -eq 0 ]]; then
      	if [[ __print_machinereadable -eq 0 ]]; then
        	echo " - Default to processor:      No"
        else
        	echo -n '"defaulttoprocessor":"No",'
        fi
      else
      	if [[ __print_machinereadable -eq 0 ]]; then
        	echo " - Default to processor:      Yes"
        else
        	echo -n '"defaulttoprocessor":"Yes",'
        fi
      fi
    fi
    tn_ll_mmi_read $C_BASE_ADDR_COMMON_LOWER $C_SUB_ADDR_COMMON_FEATURES_ACC_DP
    let    feature_bitmap=$read_data
    let          accdp_en=$((($feature_bitmap &          1)))
    let         accdp_cnt=$((($feature_bitmap &     0xFF00) >>  8))
    let     accdp_entries=$((($feature_bitmap & 0xFFFF0000) >> 16))
    if [[ $accdp_en -eq 0 ]]; then
    	if [[ __print_machinereadable -eq 0 ]]; then
      		echo   "AccDP Enabled:               No"
      	else
      		echo -n '"accdpenabled":"No",'
      	fi
    else
    	if [[ __print_machinereadable -eq 0 ]]; then
      		echo   "AccDP Enabled:               Yes"
      		echo   " - AccDP Count:               $accdp_cnt"
     		echo   " - AccDP Entry Count:         $accdp_entries"
     	else
     		echo -n '"accdpenabled":"Yes","accdpcount":'$accdp_cnt',"accdpentries":'$accdp_entries','
     	fi
    fi
    tn_ll_mmi_read $C_BASE_ADDR_COMMON_LOWER $C_SUB_ADDR_COMMON_FEATURES_TX_ROUTER
    if [[ $read_data -eq 0 ]]; then
    	if [[ __print_machinereadable -eq 0 ]]; then
      		echo   "TX Router Enabled:           No"
      	else
      		echo -n '"txrouterenabled":"No",'
      	fi
    else
    	if [[ __print_machinereadable -eq 0 ]]; then
      		echo   "TX Router Enabled:           Yes"
      	else
      		echo -n '"txrouterenabled":"Yes",'
      	fi
    fi
    tn_ll_mmi_read $C_BASE_ADDR_COMMON_LOWER $C_SUB_ADDR_COMMON_FEATURES_SFP
    let    feature_bitmap=$read_data
    let            sfp_en=$((($feature_bitmap &          1)))
    let           sfp_cnt=$((($feature_bitmap &     0xFF00) >>  8))
    if [[ $sfp_en -eq 0 ]]; then
    	if [[ __print_machinereadable -eq 0 ]]; then
      		echo   "SFP Enabled:                 No"
      	else
      		echo -n '"sfpenabled":"No",'
      	fi
    else
    	if [[ __print_machinereadable -eq 0 ]]; then
      		echo   "SFP Enabled:                 Yes"
      		echo   " - $sfp_cnt ports"
      	else
      		echo -n '"sfpenabled":"Yes","sfpports":'$sfp_cnt','
      	fi
    fi
    tn_ll_mmi_read $C_BASE_ADDR_COMMON_LOWER $C_SUB_ADDR_COMMON_FEATURES_SODIMM
    if [[ $read_data -eq 0 ]]; then
    	if [[ __print_machinereadable -eq 0 ]]; then
      		echo   "SODIMM Enabled:              No"
      	else
      		echo -n '"sodimmenabled":"No",'
      	fi
    else
    	if [[ __print_machinereadable -eq 0 ]]; then
	      	echo   "SODIMM Enabled:              Yes"
	    else
	    	echo -n '"sodimmenabled":"Yes",'
	    fi
    fi

    tn_ll_mmi_read $C_BASE_ADDR_COMMON_LOWER $C_SUB_ADDR_COMMON_PARAM_PRT_CNT
    let    param=$read_data
    if [[ __print_machinereadable -eq 0 ]]; then
    	echo     "Physical Port Count:         $param"
    else
    	echo -n '"physicalportcount":'$param','
    fi
    tn_ll_mmi_read $C_BASE_ADDR_COMMON_LOWER $C_SUB_ADDR_COMMON_PARAM_DP_CNT
    let    param=$read_data
    if [[ __print_machinereadable -eq 0 ]]; then
    	echo     "DP Count (RX/TX):            $param"
    else
    	echo -n '"dpcount":'$param','
    fi
    tn_ll_mmi_read $C_BASE_ADDR_COMMON_LOWER $C_SUB_ADDR_COMMON_PARAM_BUF_CNT
    let    seg_buf_cnt=$read_data
    if [[ __print_machinereadable -eq 0 ]]; then
    	echo     "Segment buffer count:        $seg_buf_cnt"
    else
    	echo -n '"segbufcnt":'$seg_buf_cnt','
    fi
    tn_ll_mmi_read $C_BASE_ADDR_COMMON_LOWER $C_SUB_ADDR_COMMON_PARAM_RX_MIN
    let    param=$read_data
    if [[ __print_machinereadable -eq 0 ]]; then
    	echo     "Min. Frame Size:             $param"
    else
    	echo -n '"minframesize":'$param','
    fi
    tn_ll_mmi_read $C_BASE_ADDR_COMMON_LOWER $C_SUB_ADDR_COMMON_PARAM_RX_MAX
    let    param=$read_data
    if [[ __print_machinereadable -eq 0 ]]; then
    	echo     "Max. Frame Size (RX):        $param"
    else
    	echo -n '"maxframesize":'$param','
    fi
    #tn_ll_mmi_read $C_BASE_ADDR_COMMON_LOWER $C_SUB_ADDR_COMMON_PARAM_TX_PRE
    let    param=$read_data
    if [[ __print_machinereadable -eq 0 ]]; then
    	echo     "Preamble Length (TX):        $param"
    else
    	echo -n '"preamblelength":'$param','
    fi
    tn_ll_mmi_read $C_BASE_ADDR_COMMON_LOWER $C_SUB_ADDR_COMMON_PARAM_TX_IFG
    let    param=$read_data
    if [[ __print_machinereadable -eq 0 ]]; then
   		echo     "Minimum IFG (TX):            $param"
	else
		echo -n '"minimumifg":'$param','
	fi
	
    tn_ll_mmi_read $C_BASE_ADDR_COMMON_LOWER $C_SUB_ADDR_COMMON_PARAM_EXT_BYPASS
    if [[ $read_data -eq 0 ]]; then
   		if [[ __print_machinereadable -eq 0 ]]; then
      		echo   "External Bypass Enabled:     No"
      	else
      		echo -n '"externalbypassenabled":"No",'
      	fi
    else
    	if [[ __print_machinereadable -eq 0 ]]; then
      		echo   "External Bypass Enabled:     Yes"
      	else
      		echo -n '"externalbypassenabled":"Yes",'
      	fi
    fi

    tn_ll_mmi_read $C_BASE_ADDR_COMMON_LOWER $C_SUB_ADDR_COMMON_WIDTH_HC_BAD
    let    width=$read_data
    if [[ __print_machinereadable -eq 0 ]]; then
    	echo     "Fragment Counter Width:      $width"
    else
    	echo -n '"fragmencounterwidth":'$width','
    fi
    tn_ll_mmi_read $C_BASE_ADDR_COMMON_LOWER $C_SUB_ADDR_COMMON_WIDTH_ETH_SW
    let    width=$((2**$read_data))
    if [[ __print_machinereadable -eq 0 ]]; then
    	echo     "EthSW Entries/DP:            $width"
    else
    	echo -n '"ethswentries":'$width','
    fi
    tn_ll_mmi_read $C_BASE_ADDR_COMMON_LOWER $C_SUB_ADDR_COMMON_WIDTH_RX_STAT_BAD
    let    width=$read_data
    if [[ __print_machinereadable -eq 0 ]]; then
    	echo     "RX Statistics Counter Width: $width"
	else
		echo -n '"rxcounterwidth":'$width','
	fi
    tn_ll_mmi_read $C_BASE_ADDR_TM_LOWER $C_SUB_ADDR_TM_QUEUE_THRES_FULL
    if [[ __print_machinereadable -eq 0 ]]; then
    	printf   "Queue Threshold:             %d\n" $read_data
	else
		echo -n '"queuethreshold":'$read_data','
	fi
	
	if [[ __print_machinereadable -eq 0 ]]; then
    	echo -e  "\n### System connection codings: 0=Unused, 1=System Controller, 2=MMI, 3=6Tree, 4=User"
	fi
	
    tn_ll_mmi_read $C_BASE_ADDR_COMMON_LOWER $C_SUB_ADDR_COMMON_PARAM_DISPLAY
    let    param=$read_data
    if [[ __print_machinereadable -eq 0 ]]; then
    	echo     "Display:                     $param"
    else
    	echo -n '"display":'$param','
    fi
    tn_ll_mmi_read $C_BASE_ADDR_COMMON_LOWER $C_SUB_ADDR_COMMON_PARAM_LED_SYNCE
    let    param=$read_data
    if [[ __print_machinereadable -eq 0 ]]; then
    	echo     "SyncE LEDs:                  $param"
    else
    	echo -n '"synceleds":'$param','
    fi
    tn_ll_mmi_read $C_BASE_ADDR_COMMON_LOWER $C_SUB_ADDR_COMMON_PARAM_LED_INTERNAL
    let    param=$read_data
    if [[ __print_machinereadable -eq 0 ]]; then
    	echo     "Internal LEDs:               $param"
    else
    	echo -n '"internalleds":'$param','
    fi
    tn_ll_mmi_read $C_BASE_ADDR_COMMON_LOWER $C_SUB_ADDR_COMMON_PARAM_BEEP
    let    param=$read_data
    if [[ __print_machinereadable -eq 0 ]]; then
    	echo     "Beeper:                      $param"
    else
    	echo -n '"beeper":'$param','
    fi
    tn_ll_mmi_read $C_BASE_ADDR_COMMON_LOWER $C_SUB_ADDR_COMMON_PARAM_PHY_RESET
    let    param=$read_data
    if [[ __print_machinereadable -eq 0 ]]; then
    	echo     "PHY Reset:                   $param"
    else
    	echo -n '"phyreset":'$param','
    fi
    tn_ll_mmi_read $C_BASE_ADDR_COMMON_LOWER $C_SUB_ADDR_COMMON_PARAM_PINHEADER
    let    param=$read_data
    if [[ __print_machinereadable -eq 0 ]]; then
    	echo     "Internal Big Header:         $param"
    else
    	echo -n '"internalbigheader":'$param','
    fi
    tn_ll_mmi_read $C_BASE_ADDR_COMMON_LOWER $C_SUB_ADDR_COMMON_PARAM_PMOD
    let    param=$read_data
    if [[ __print_machinereadable -eq 0 ]]; then
    	echo     "Internal PMod:               $param"
	else
		echo -n '"internalpmod":'$param','
	fi

    ###>>> Param=2
    if [[ $1 -eq 2 ]]; then
    	if [[ __print_machinereadable -eq 0 ]]; then
     		echo -e  "\n### Peripherals status"
		fi

      tn_ll_mmi_read $C_BASE_ADDR_PERIPH $C_SUB_ADDR_PERIPH_BUTTONS
      let peripherals_button_bitmap=$read_data
      let    button_front=$((($peripherals_button_bitmap & 0x0000FFFF)))
      let     button_user=$((($peripherals_button_bitmap & 0x00010000) >> 16))
      if [[ $button_front -eq 0 ]]; then
        if [[ __print_machinereadable -eq 0 ]]; then
        	echo   "Front Button:                No button pressed";
      	else
      		echo -n '"frontbutton":"No button pressed",'
      	fi
      else
      	if [[ __print_machinereadable -eq 0 ]]; then
        	printf "Front Button/bit:            0x%04x\n" $button_front
     	else
     		echo -n '"frontbutton":'$button_front','
     	fi 
      fi
      if [[ $button_user -eq 0 ]]; then
        if [[ __print_machinereadable -eq 0 ]]; then
        	echo   "User Button:                 Not pressed"
		else
			echo -n '"userbutton":"Not pressed",'
		fi      
      else
      	if [[ __print_machinereadable -eq 0 ]]; then
        	echo   "User Button:                 Pressed"
      	else
      		echo -n '"userbutton":"Pressed",'
      	fi
      fi

      tn_ll_mmi_read $C_BASE_ADDR_PERIPH $C_SUB_ADDR_PERIPH_GPIOS
      let peripherals_gpio_bitmap=$read_data
      let      pinheader1_gpio_in=$((($peripherals_gpio_bitmap & 0x000003FF)))
      let           pmod0_gpio_in=$((($peripherals_gpio_bitmap & 0x00003C00) >> 10))
      let     pinheader1_gpio_tri=$((($peripherals_gpio_bitmap & 0x03FF0000) >> 16))
      let          pmod0_gpio_tri=$((($peripherals_gpio_bitmap & 0x3C000000) >> 26))
      if [[ $pinheader1_gpio_in -eq 0 && $pinheader1_gpio_tri -eq 0 ]]; then
      	if [[ __print_machinereadable -eq 0 ]]; then
      		echo   "Pinheader1 GPIO:             0";
      	else
      		echo -n '"pinheader1gpio":'0','
      	fi
      else
      	if [[ __print_machinereadable -eq 0 ]]; then
        	printf "Pinheader1 GPIO/bit:         0x%03x (Read Data Valid: 0x%03x)\n" $pinheader1_gpio_in $pinheader1_gpio_tri
      	else
      		printf '"pinheader1gpio":"0x%03x",' $pinheader1_gpio_in $pinheader1_gpio_tri
      	fi
      fi
      if [[ $pmod0_gpio_in -eq 0 && $pmod0_gpio_tri -eq 0 ]]; then
        if [[ __print_machinereadable -eq 0 ]]; then
        	echo   "PMOD0 GPIO:                  0";
      	else
      		echo -n '"pmod0gpio":'0','
      	fi
      else
        if [[ __print_machinereadable -eq 0 ]]; then
        	printf "PMOD0 GPIO/bit:              0x%x (Read Data Valid: 0x%x)\n" $pmod0_gpio_in $pmod0_gpio_tri
      	else
      		printf '"pmod0gpio":"0x%x","pmod0readdata":"0x%x",' $pmod0_gpio_in $pmod0_gpio_tri 
      	fi
      fi
	
	  if [[ __print_machinereadable -eq 0 ]]; then
      	echo -e  "\n### MDIO interrupt status"
	  fi
	  
      tn_ll_mmi_read $C_BASE_ADDR_MDIO $C_SUB_ADDR_MDIO_INTERRUPT
      let peripherals_interrupt_bitmap=$read_data
      let                 gphy_md_ints=$((($peripherals_interrupt_bitmap & 0x000003FF)))
      let              alaska_ptp_ints=$((($peripherals_interrupt_bitmap & 0x00000C00) >> 10))
      let    alaska_ptp_event_triggers=$((($peripherals_interrupt_bitmap & 0x00030000) >> 16))
      if [[ $gphy_md_ints -eq 0 ]]; then
      	if [[ __print_machinereadable -eq 0 ]]; then
        	echo   "GPHY MDIO INT:               None";
      	else
      		echo -n '"gphymdioint":"None",'
      	fi
      else
        if [[ __print_machinereadable -eq 0 ]]; then
        	printf "GPHY MDIO INT/bit:           0x%03x\n" $gphy_md_ints
	else
		printf '"gphymdioint":"0x%03x",' $gphy_md_ints
	fi	      
      fi
      if [[ $alaska_ptp_ints -eq 0 ]]; then
        if [[ __print_machinereadable -eq 0 ]]; then
        	echo   "Alaska PTP INT:              None";
      	else
      		echo -n '"alaskaptpint":"None",'
      	fi
      else
      	if [[ __print_machinereadable -eq 0 ]]; then
        	printf "Alaska PTP INT/bit:          0x%x\n" $alaska_ptp_ints
      	else
      		echo -n '"alaskaptpintbit":'$alaska_ptp_ints','
      	fi
      fi
      if [[ $alaska_ptp_event_triggers -eq 0 ]]; then
        if [[ __print_machinereadable -eq 0 ]]; then
        	echo   "Alaska PTP Triggers:         None";
      	else
      		echo -n '"alaskaptptriggers":"None",'
      	fi
      else
        if [[ __print_machinereadable -eq 0 ]]; then
        	printf "Alaska PTP Triggers:         0x%x\n" $alaska_ptp_event_triggers
      	else
      		echo -n '"alaskaptptriggers":'$alaska_ptp_event_triggers','
      	fi
      fi

	  if [[ __print_machinereadable -eq 0 ]]; then
      	echo -e  "\n### Header Creation interrupts"
	  fi

      for prt in `seq 0 11`; do
        let hcbase=C_BASE_ADDR_HC_$prt
        tn_ll_mmi_read $hcbase $C_SUB_ADDR_HC_INTERRUPT_EN
        let interrupt_en=$read_data
        tn_ll_mmi_read $hcbase $C_SUB_ADDR_HC_INTERRUPT
        if [[ $read_data -gt 0 && $status -eq 0 ]]; then
			if [[ __print_machinereadable -eq 0 ]]; then        
          		printf " - Port $prt:               %d\n" $read_data
          		printf "  (enable: 0x%08x)\n" $interrupt_en
   			else
   				printf '"portinterrupt0x%x":'0x%x',"enableinterrupt0x%x":'0x%x',' $prt $read_data $prt $interrupt_en
   			fi    
        else
        	if [[ __print_machinereadable -eq 1 ]]; then
        		printf '"portinterrupt0x%x":"none",' $prt
        	fi
        fi
        	
      done
      for prt in `seq 0 1`; do
        let hcbase=C_BASE_ADDR_HC_PCIE_$prt
        tn_ll_mmi_read $hcbase $C_SUB_ADDR_HC_INTERRUPT_EN
        let interrupt_en=$read_data
        tn_ll_mmi_read $hcbase $C_SUB_ADDR_HC_INTERRUPT
        if [[ $read_data -gt 0 && $status -eq 0 ]]; then
          if [[ __print_machinereadable -eq 0 ]]; then
          	printf " - PCIe Port $prt:          %d\n" $read_data
          	printf "  (enable: 0x%08x)\n" $interrupt_en
		  else
		  	echo -n '"pcieport'$prt'":'$read_data',"enableinterrupt":'$interrupt_en','
		  fi        
        fi
      done
    fi

	if [[ __print_machinereadable -eq 0 ]]; then
    	echo -e  "\n### Status & Events"
	fi
	
    ####
    tn_ll_mmi_read $C_BASE_ADDR_COMMON_LOWER $C_SUB_ADDR_COMMON_FPGA_ALARM
    let    fpga_alarm_bitmap=$read_data
    let            alarm=$((($fpga_alarm_bitmap &  1) >> 0))
    let        over_temp=$((($fpga_alarm_bitmap &  2) >> 1))
    let        user_temp=$((($fpga_alarm_bitmap &  4) >> 2))
    let     vccaux_alarm=$((($fpga_alarm_bitmap &  8) >> 3))
    let     vccint_alarm=$((($fpga_alarm_bitmap & 16) >> 4))
    let      vbram_alarm=$((($fpga_alarm_bitmap & 32) >> 5))
    if [[ $alarm -eq 0 ]]; then
    	if [[ __print_machinereadable -eq 0 ]]; then
      		echo     "FPGA Alarm:                  None";
    	else
    		echo -n '"fpgaalarm":"None",'
    	fi
    else
    	if [[ __print_machinereadable -eq 0 ]]; then
      		echo     "FPGA Alarm:                  $alarm";
      		echo     " - Overtemperature:           $over_temp";
      		echo     " - User Temperature:          $user_temp";
      		echo     " - AUX Voltage:               $vccaux_alarm";
      		echo     " - Logic Voltage:             $vccint_alarm";
      		echo     " - BRAM Voltage:              $vbram_alarm";
    	else
    		echo -n '"fpgaalarm":'$alarm',"overtemperature":'$over_temp',"usertemperature":'$user_temp',"auxvoltage":'$vccaux_alarm',"logicvoltage":'$vccint_alarm',"bramvoltage":'$vbram_alarm','
    	fi
    fi

    ####
    tn_ll_mmi_read $C_BASE_ADDR_COMMON_LOWER $C_SUB_ADDR_COMMON_CONFIG_CHECK
    let      config_check_bitmap=$read_data
    let           crc_error=$((($config_check_bitmap & 1) >> 0))
    let           ecc_error=$((($config_check_bitmap & 2) >> 1))
    let    single_ecc_error=$((($config_check_bitmap & 4) >> 2))
    if [[ __print_machinereadable -eq 0 ]]; then
    	echo       "FPGA State:"
    fi
    if [[ $crc_error -eq 0 ]]; then
      	if [[ __print_machinereadable -eq 0 ]]; then
	  		echo     " - CRC Error:                 No"
      	else
      		echo -n '"crcerror":"No",'
      fi
    else
    	if [[ __print_machinereadable -eq 0 ]]; then
      		echo     " - CRC Error:                 Yes"
      	else
      		echo -n '"crcerror":"Yes",'
      	fi
    fi
    if [[ $ecc_error -eq 0 ]]; then
    	if [[ __print_machinereadable -eq 0 ]]; then
      		echo     " - ECC Error:                 No"
    	else
    		echo -n '"eccerror":"No",'
    	fi
    else
      	if [[ __print_machinereadable -eq 0 ]]; then
      		echo     " - ECC Error:                 Yes"
    	else
    		echo '"eccerror":"Yes",'
    	fi
    fi
    if [[ $single_ecc_error -eq 0 ]]; then
      	if [[ __print_machinereadable -eq 0 ]]; then
      		echo     " - Single ECC Error:          No"
    	else
    		echo -n '"singleeccerror":"No",'
    	fi
    else
      	if [[ __print_machinereadable -eq 0 ]]; then
      		echo     " - Single ECC Error:          Yes"
    	else
    		echo -n '"singleeccerror":"Yes",'
    	fi
    fi

    tn_ll_mmi_read $C_BASE_ADDR_COMMON_LOWER $C_SUB_ADDR_COMMON_PCIE_ERROR0
    let    pcie_error_bitmap0=$read_data
    tn_ll_mmi_read $C_BASE_ADDR_COMMON_LOWER $C_SUB_ADDR_COMMON_PCIE_ERROR1
    let    pcie_error_bitmap1=$read_data
    if [[ $pcie_error_bitmap1 -eq 0 && $pcie_error_bitmap0 -eq 0 ]]; then
      if [[ __print_machinereadable -eq 0 ]]; then
        echo     "PCIe Errors:                 No issues"
      else
        echo -n '"pcieerrors":"None",'
      fi
    elif [[ $pcie_error_bitmap1 -eq 0x00001200 && $pcie_error_bitmap0 -eq 0 ]]; then
      if [[ __print_machinereadable -eq 0 ]]; then
        echo     "PCIe Status:                 Just MMI Timeouts"
      else
        echo -n '"pcieerrors":"MMI Timeouts",'
      fi
    else
      if [[ __print_machinereadable -eq 0 ]]; then
        printf   "PCIe Error/bit:              0x%08x%08x\n" $pcie_error_bitmap1 $pcie_error_bitmap0
      else
        printf '"pcieerrors":"0x%08x%08x",' $pcie_error_bitmap1 $pcie_error_bitmap0
      fi
    fi

    #tn_ll_mmi_read $C_BASE_ADDR_TM_LOWER $C_SUB_ADDR_TM_PTR_CNT
    #if [[ __print_machinereadable -eq 0 ]]; then
    #	echo       "Segments stored:             $read_data"
    #else
    #	echo -n '"segmentsstored":'$read_data','
    #fi

    #if [[ $read_data -ge $seg_buf_thresh && $read_data -le $seg_buf_cnt ]]; then
    #  if [[ __print_machinereadable -eq 0 ]]; then
    #      echo     " - ### Packet Buffer Underrun - System crashed ###"
    #  else
    #      printf '"packetbufferunderrun":'1','
    #  fi
    #else
    #  if [[ __print_machinereadable -eq 1 ]]; then
    #    printf '"packetbufferunderrun:"'0','
    #  fi
    #fi;
    #let seg_buf_thresh=$seg_buf_cnt-300
    #if [[ $read_data -ge $seg_buf_thresh && $read_data -le $seg_buf_cnt ]]; then
    #	if [[ __print_machinereadable -eq 0 ]]; then
    #  		echo     " - ### Packet buffer full - possibly tail dropping newly received packets ###"
    #  	else
    #  		printf '"packetbufferfull":'1','
    # fi
    #else
    #	if [[ __print_machinereadable -eq 1 ]]; then
    #		printf '"packetbufferfull":'0','
    #	fi
    #fi;

    tn_ll_mmi_read $C_BASE_ADDR_COMMON_LOWER $C_SUB_ADDR_COMMON_FIFO_OVERFLOW0
    let    overflow_bitmap0=$read_data
    tn_ll_mmi_read $C_BASE_ADDR_COMMON_LOWER $C_SUB_ADDR_COMMON_FIFO_OVERFLOW1
    let    overflow_bitmap1=$read_data
    tn_ll_mmi_read $C_BASE_ADDR_COMMON_LOWER $C_SUB_ADDR_COMMON_FIFO_OVERFLOW2
    let    overflow_bitmap2=$read_data
    tn_ll_mmi_read $C_BASE_ADDR_COMMON_LOWER $C_SUB_ADDR_COMMON_FIFO_OVERFLOW3
    let    overflow_bitmap3=$read_data
    tn_ll_mmi_read $C_BASE_ADDR_COMMON_LOWER $C_SUB_ADDR_COMMON_FIFO_OVERFLOW4
    let    overflow_bitmap4=$read_data
    if [[ $overflow_bitmap0 -eq 0 && $overflow_bitmap1 -eq 0 && $overflow_bitmap2 -eq 0 && $overflow_bitmap3 -eq 0 && $overflow_bitmap4 -eq 0 ]]; then
    	if [[ __print_machinereadable -eq 0 ]]; then	
    		echo     "FIFO Overflow:               None"
    	else
    		echo -n '"fifooverflow":"None",'
    	fi
    else
    	if [[ __print_machinereadable -eq 0 ]]; then
      		printf   "FIFO Overflow/bit:           0x%08x%08x%08x%08x%08x\n" $overflow_bitmap4 $overflow_bitmap3 $overflow_bitmap2 $overflow_bitmap1 $overflow_bitmap0
    	else
    		printf '"fifooverflow":"0x%08x%08x%08x%08x%08x",' $overflow_bitmap4 $overflow_bitmap3 $overflow_bitmap2 $overflow_bitmap1 $overflow_bitmap0
    	fi
    fi

    tn_ll_mmi_read $C_BASE_ADDR_COMMON_LOWER $C_SUB_ADDR_COMMON_FIFO_UNDERRUN0
    let    underrun_bitmap0=$read_data
    tn_ll_mmi_read $C_BASE_ADDR_COMMON_LOWER $C_SUB_ADDR_COMMON_FIFO_UNDERRUN1
    let    underrun_bitmap1=$read_data
    tn_ll_mmi_read $C_BASE_ADDR_COMMON_LOWER $C_SUB_ADDR_COMMON_FIFO_UNDERRUN2
    let    underrun_bitmap2=$read_data
    tn_ll_mmi_read $C_BASE_ADDR_COMMON_LOWER $C_SUB_ADDR_COMMON_FIFO_UNDERRUN3
    let    underrun_bitmap3=$read_data
    tn_ll_mmi_read $C_BASE_ADDR_COMMON_LOWER $C_SUB_ADDR_COMMON_FIFO_UNDERRUN4
    let    underrun_bitmap4=$read_data
    if [[ $underrun_bitmap0 -eq 0 && $underrun_bitmap1 -eq 0 && $underrun_bitmap2 -eq 0 && $underrun_bitmap3 -eq 0 && $underrun_bitmap4 -eq 0 ]]; then
    	if [[ __print_machinereadable -eq 0 ]]; then
      		echo     "FIFO Underrun:               None"
    	else
    		echo -n '"fifounderrun":"None",'
    	fi
    else
    	if [[ __print_machinereadable -eq 0 ]]; then
      		printf   "FIFO Underrun/bit:           0x%08x%08x%08x%08x%08x\n" $underrun_bitmap4 $underrun_bitmap3 $underrun_bitmap2 $underrun_bitmap1 $underrun_bitmap0
    	else
    		printf '"fifounderrun":"0x%08x%08x%08x%08x%08x",' $underrun_bitmap4 $underrun_bitmap3 $underrun_bitmap2 $underrun_bitmap1 $underrun_bitmap0
    	fi
    fi

    # Read once to clean, read a second time to get the latest values
    tn_ll_mmi_read $C_BASE_ADDR_COMMON_LOWER $C_SUB_ADDR_COMMON_TXF_FULL
    tn_ll_mmi_read $C_BASE_ADDR_COMMON_LOWER $C_SUB_ADDR_COMMON_TXF_ACTIVITY

    tn_ll_mmi_read $C_BASE_ADDR_COMMON_LOWER $C_SUB_ADDR_COMMON_TXF_FULL
    if [[ $read_data -eq 0 ]]; then
    	if [[ __print_machinereadable -eq 0 ]]; then
      		echo   "TXF Full/Port:               No";
    	else
      printf "TXF Full/Port:               0x%08x (activity: " $read_data
      sleep 0.1
      tn_ll_mmi_read $C_BASE_ADDR_COMMON_LOWER $C_SUB_ADDR_COMMON_TXF_ACTIVITY
      printf "0x%08x)\n" $read_data
    		echo -n '"txffullport":"No",'
    	fi
    else
      	if [[ __print_machinereadable -eq 0 ]]; then
      		printf "TXF Full/Port:               0x%08x (activity: " $read_data
      	else
      		printf '"txffullport":"0x%08x (activity: ",' $read_data
      	fi
      sleep 0.1
      tn_ll_mmi_read $C_BASE_ADDR_COMMON_LOWER $C_SUB_ADDR_COMMON_TXF_ACTIVITY
      if [[ __print_machinereadable -eq 0 ]]; then
      	printf "0x%08x)\n" $read_data
      else
      		printf '"txffullportactivity":"0x%08x)",' $read_data
      fi
    fi

    tn_ll_mmi_read $C_BASE_ADDR_COMMON_LOWER $C_SUB_ADDR_COMMON_BUFFER_FULL
    if [[ $(($read_data & 1)) -eq 0 ]]; then
    	if [[ __print_machinereadable -eq 0 ]]; then
      		echo   "Packet Buffer Full:          No"
    	else
    		echo -n '"packetbufferfull2":"No",'
    	fi
    else
    	if [[ __print_machinereadable -eq 0 ]]; then
      		echo   "Packet Buffer Full:          Yes"
    	else
    		echo -n '"packetbufferfull2":"Yes",'
    	fi
    fi


    if [[ $((($read_data & 0x00FF0000) >> 16)) -eq 3 ]]; then
    	if [[ __print_machinereadable -eq 0 ]]; then
      		echo   "RX DPs:                      currently ready";
    	else
    		echo -n '"rxdps":"currently ready",'
    	fi
    else
    	if [[ __print_machinereadable -eq 0 ]]; then
      		printf "RX DP backpressure:          0x%x\n" $((($read_data & 0x00FF0000) >> 24))
    	else
    		printf '"rxdps":"0x%x",' $((($read_data & 0x00FF0000) >> 24))
    	fi
    fi
    if [[ $((($read_data & 0xFF000000) >> 24)) -eq 3 ]]; then
    	if [[ __print_machinereadable -eq 0 ]]; then
     		echo   "TX DPs:                      currently ready";
    	else
    		echo -n '"txdps":"currently ready",'
    	fi
    else
    	if [[ __print_machinereadable -eq 0 ]]; then
      		printf "TX DP backpressure:          0x%x\n" $((($read_data & 0xFF000000) >> 24))
    	else
    		printf '"txdps":"0x%x",' $((($read_data & 0xFF000000) >> 24))
    	fi
    fi

	if [[ __print_machinereadable -eq 0 ]]; then
    	echo -e  "\n### Current MMI status:"
    fi

    tn_ll_mmi_read $C_BASE_ADDR_COMMON_LOWER $C_SUB_ADDR_COMMON_PCIE_TIMEOUT_CNT
    let    pcie_timeouts=$read_data
    if [[ __print_machinereadable -eq 0 ]]; then
    	echo     "MMI Timeouts (sat.):         $pcie_timeouts"
	else
		echo -n '"mmitimeouts":'$pcie_timeouts','
	fi
	
    let    status=`TNbar1 $(($C_BASE_ADDR_BM*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_BM_MMI_STATUS))   | cut -d " " -f 6`
    let     err_acc_rd=$((($status & 1) >> 0))
    let     err_acc_wr=$((($status & 2) >> 1))
    let    err_time_rd=$((($status & 4) >> 2))
    let    err_time_wr=$((($status & 8) >> 3))
    let        err_cnt=$((($status & 0xFFF0) >> 4))
    let        acc_cnt=$((($status & 0xFFFF0000) >> 16))
    if [[ __print_machinereadable -eq 0 ]]; then
    	printf   "MMI Statistics (COR):        0x%04x accesses, thereof 0x%03x failing\n" $acc_cnt $err_cnt
    else
    	printf '"mmistatistics":"0x%04x accesses, thereof 0x%03x failing",' $acc_cnt $err_cnt
    fi
    if [[ $err_acc_rd -eq 0 ]]; then
    	if [[ __print_machinereadable -eq 0 ]]; then
      		echo   " - Access Error at Read:      No"
    	else
    		echo -n '"accesserroratread":"No",'
    	fi
    else
    	if [[ __print_machinereadable -eq 0 ]]; then
      		echo   " - Access Error at Read:      Yes"
    	else
    		echo -n '"accesserroratread":"Yes",'
    	fi
    fi
    if [[ $err_acc_wr -eq 0 ]]; then
    	if [[ __print_machinereadable -eq 0 ]]; then
      		echo   " - Access Error at Write:     No"
    	else
    		echo -n '"accesserroratwrite":"No",'
    	fi
    else
    	if [[ __print_machinereadable -eq 0 ]]; then
      		echo   " - Access Error at Write:     Yes"
    	else 
    		echo -n '"accesserroratwrite":"Yes",'
    	fi
    fi
    if [[ $err_time_rd -eq 0 ]]; then
    	if [[ __print_machinereadable -eq 0 ]]; then
      		echo   " - Timeout at Read:           No"
    	else
    		echo -n '"timeoutatread":"No",'
    	fi
    else
    	if [[ __print_machinereadable -eq 0 ]]; then
      		echo   " - Timeout at Read:           Yes"
    	else
    		echo -n '"timeoutatread":"Yes",'
    	fi
    fi
    if [[ $err_time_wr -eq 0 ]]; then
    	if [[ __print_machinereadable -eq 0 ]]; then
      		echo   " - Timeout at Write:          No"
    	else
    		echo -n '"timeoutatwrite":"No",'
    	fi
    else
    	if [[ __print_machinereadable -eq 0 ]]; then
      		echo   " - Timeout at Write:          Yes"
    	else
    		echo -n '"timeoutatwrite":"Yes",'
    	fi
    fi

    if [[ $status -ne 0 ]]; then
    	if [[ __print_machinereadable -eq 0 ]]; then
      		echo   " ** MMI Read Timeout **"
      	fi
    fi
    tn_ll_mmi_read $C_BASE_ADDR_COMMON_LOWER $C_SUB_ADDR_BM_MMI_TO_ADDR
    let last_timeout_addr=$read_data
    if [[ $last_timeout_addr -ne 0 ]]; then
    	if [[ __print_machinereadable -eq 0 ]]; then
      		printf "Last MMI Timeout address:    0x%08x\n" $last_timeout_addr
    	else
    		printf '"mmitimeoutaddress":"0x%08x",' $last_timeout_addr
    	fi
    else
    	if [[ __print_machinereadable -eq 1 ]]; then
    		printf '"mmitimeoutaddress":"none",'
    	fi
    fi

    tn_ll_mmi_read $C_BASE_ADDR_BM $C_SUB_ADDR_BM_MMI_INTERRUPT
    let interrupt=$read_data
    #tn_ll_mmi_read $C_BASE_ADDR_BM $C_SUB_ADDR_BM_MMI_INTERRUPT_EN
    #let interrupt_en=$read_data
    if [[ __print_machinereadable -eq 0 ]]; then
    	printf   "MMI Interrupt register is    0x%08x\n" $interrupt # (enable: 0x%08x) $interrupt_en
    else
    	printf '"mmiinterruptregister":"0x%08x",' $interrupt
    fi
    ###>>> Param=2
    #if [[ $1 -eq 2 ]]; then
      #tn_ll_mmi_read $C_BASE_ADDR_COMMON_LOWER $C_SUB_ADDR_COMMON_CLK_MON_L
      #let sys_if_freq=$read_data
      #if [[ $sys_if_freq -ne 0 ]]; then
      #  let last_clk=$C_SUB_ADDR_COMMON_CLK_MON_H-$C_SUB_ADDR_COMMON_CLK_MON_L
      #  let last_clk=$last_clk/4-1
      #  echo "Clock frequencies in relation to 125 MHz PCIe MMI frequency:"
      #  for clk in `seq 0 $last_clk`; do
      #    tn_ll_mmi_read $C_BASE_ADDR_COMMON_LOWER $(($C_SUB_ADDR_COMMON_CLK_MON_L+4*$clk))
      #    let clk_freq=$read_data
      #    case $clk in
      #      0)
      #        printf " - SysIfClk:                  %d (~1.5625 MHz)\n" $clk_freq
      #        ;;
      #      1)
      #        printf " - Pinheader:                 %d\n" $clk_freq
      #        ;;
      #      2)
      #        printf " - PMod:                      %d\n" $clk_freq
      #        ;;
      #      3)
      #        printf " - FC_CAM:                    %d (~100 MHz)\n" $clk_freq
      #        ;;
      #      4)
      #        printf " - SYS_RTC:                   %d (~125 MHz)\n" $clk_freq
      #        ;;
      #      5)
      #        printf " - TX_CLK90:                  %d (125 MHz, +/- 6250)\n" $clk_freq
      #        ;;
      #      6)
      #        printf " - SGMII:                     %d (125 MHz, +/- 6250)\n" $clk_freq
      #        ;;
      #      7)
      #        printf " - TX_CLK:                    %d (125 MHz, +/- 6250)\n" $clk_freq
      #        ;;
      #      8)
      #        printf " - ACCDP:                     %d (~200 MHz)\n" $clk_freq
      #        ;;
      #      9)
      #        printf " - 200MHzREF:                 %d (~200 MHz)\n" $clk_freq
      #        ;;
      #     10)
      #        printf " - GPHY0:                     %d (2.5/25/125 MHz, +/- 125/1250/6250)\n" $clk_freq
      #        ;;
      #     11)
      #        printf " - GPHY1:                     %d (2.5/25/125 MHz, +/- 125/1250/6250)\n" $clk_freq
      #        ;;
      #     12)
      #        printf " - GPHY2:                     %d (2.5/25/125 MHz, +/- 125/1250/6250)\n" $clk_freq
      #        ;;
      #     13)
      #        printf " - GPHY3:                     %d (2.5/25/125 MHz, +/- 125/1250/6250)\n" $clk_freq
      #        ;;
      #     14)
      #        printf " - GPHY4:                     %d (2.5/25/125 MHz, +/- 125/1250/6250)\n" $clk_freq
      #        ;;
      #     15)
      #        printf " - GPHY5:                     %d (2.5/25/125 MHz, +/- 125/1250/6250)\n" $clk_freq
      #        ;;
      #     16)
      #        printf " - GPHY6:                     %d (2.5/25/125 MHz, +/- 125/1250/6250)\n" $clk_freq
      #        ;;
      #     17)
      #        printf " - GPHY7:                     %d (2.5/25/125 MHz, +/- 125/1250/6250)\n" $clk_freq
      #        ;;
      #     18)
      #        printf " - GPHY8:                     %d (2.5/25/125 MHz, +/- 125/1250/6250)\n" $clk_freq
      #        ;;
      #     19)
      #        printf " - GPHY9:                     %d (2.5/25/125 MHz, +/- 125/1250/6250)\n" $clk_freq
      #        ;;
      #     20)
      #        printf " - AlaskaA:                   %d (2.5/25/125 MHz, +/- 125/1250/6250)\n" $clk_freq
      #        ;;
      #     21)
      #        printf " - AlaskaB:                   %d (2.5/25/125 MHz, +/- 125/1250/6250)\n" $clk_freq
      #        ;;
      #     22)
      #        printf " - PLL GTP0:                  %d (e.g., 125 MHz, +/- 12500)\n" $clk_freq
      #        ;;
      #     23)
      #        printf " - PLL SFP1:                  %d (e.g., 125 MHz, +/- 12500)\n" $clk_freq
      #        ;;
      #     24)
      #        printf " - AXI_DP:                    %d (~200 MHz)\n" $clk_freq
      #        ;;
      #      *)
      #        printf " - TBD(%d):                   %d\n" $clk $clk_freq
      #        ;;
      #    esac
      #  done
      #fi
    #fi
  fi

  ###>>> Param=0 or 2 or 3
  if [[ __print_machinereadable -eq 0 ]]; then
  	echo -e  "\n### Displaying NoC Monitor Results (empty, if they are not instantiated)"
  fi
  for prt in `seq 0 11; seq 14 15`; do
    let hcbase=C_BASE_ADDR_NOC_MON_HC_$prt
    tn_ll_mmi_read $hcbase $C_SUB_ADDR_NOC_MON_FIRST_ERR
    let first_err=$read_data
    tn_ll_mmi_read $hcbase $C_SUB_ADDR_NOC_MON_STATUS
    if [[ $read_data -gt 0 && $status -eq 0 ]]; then
    	if [[ __print_machinereadable -eq 0 ]]; then  
      		printf " - Error HC $prt/bit:            0x%08x (first: 0x%08x)\n" $read_data $first_err
    	else
    		printf '"errorhc'$prt'":"0x%08x (first: 0x%08x)",' $read_data $first_err
    	fi
    else
    	if [[ __print_machinereadable -eq 1 ]]; then  
    		printf '"errorhc'$prt'":"none",'
    	fi
      break
    fi
  done
  for dp in `seq 0 1`; do
    let rxinbase=C_BASE_ADDR_NOC_MON_RX_IN_$prt
    tn_ll_mmi_read $rxinbase $C_SUB_ADDR_NOC_MON_FIRST_ERR
    let first_err=$read_data
    tn_ll_mmi_read $rxinbase $C_SUB_ADDR_NOC_MON_STATUS
    if [[ $read_data -gt 0 && $status -eq 0 ]]; then
    	if [[ __print_machinereadable -eq 0 ]]; then
      		printf " - Error RX_IN $dp/bit:          0x%08x (first: 0x%08x)\n" $read_data $first_err
    	else
    		printf '"errorrxin'$dp'":"0x%08x (first: 0x%08x)",' $read_data $first_err
    	fi
    else
    	if [[ __print_machinereadable -eq 1 ]]; then  
    		printf '"errorrxin'$dp'":"none",'
    	fi
      break
    fi
  done
  for dp in `seq 0 1`; do
    let rxoutbase=C_BASE_ADDR_NOC_MON_RX_OUT_$prt
    tn_ll_mmi_read $rxoutbase $C_SUB_ADDR_NOC_MON_FIRST_ERR
    let first_err=$read_data
    tn_ll_mmi_read $rxoutbase $C_SUB_ADDR_NOC_MON_STATUS
    if [[ $read_data -gt 0 && $status -eq 0 ]]; then
    	if [[ __print_machinereadable -eq 0 ]]; then
      		printf " - Error RX_OUT $dp/bit:         0x%08x (first: 0x%08x)\n" $read_data $first_err
    	else
    		printf '"errorrxout'$dp'":"0x%08x (first: 0x%08x)",' $read_data $first_err
    	fi
    else
        if [[ __print_machinereadable -eq 1 ]]; then  
    		printf '"errorrxout'$dp'":"none",'
    	fi
      break
    fi
  done
  for dp in `seq 0 1`; do
    let tmbase=C_BASE_ADDR_NO_CMON_TM_OUT_$prt
    tn_ll_mmi_read $tmbase $C_SUB_ADDR_NOC_MON_FIRST_ERR
    let first_err=$read_data
    tn_ll_mmi_read $tmbase $C_SUB_ADDR_NOC_MON_STATUS
    if [[ $read_data -gt 0 && $status -eq 0 ]]; then
    	if [[ __print_machinereadable -eq 0 ]]; then
      		printf " - Error TM_OUT $dp/bit:         0x%08x (first: 0x%08x)\n" $read_data $first_err
    	else
    		printf '"errortmout'$dp'":"0x%08x (first: 0x%08x)",' $read_data $first_err
    	fi
    else
       	if [[ __print_machinereadable -eq 1 ]]; then  
    		printf '"errortmout'$dp'":"none",'
    	fi
      break
    fi
  done
  for prt in `seq 0 11; seq 14 15`; do
    # HT address range is currently unused, so NoC Monitor is connected there
    let htbase=C_BASE_ADDR_HT_$prt
    tn_ll_mmi_read $htbase $C_SUB_ADDR_NOC_MON_FIRST_ERR
    let first_err=$read_data
    tn_ll_mmi_read $htbase $C_SUB_ADDR_NOC_MON_STATUS
    if [[ $read_data -gt 0 && $status -eq 0 ]]; then
    	if [[ __print_machinereadable -eq 0 ]]; then
      		printf " - Error HT $prt/bit:            0x%08x (first: 0x%08x)\n" $read_data $first_err
    	else
    		printf '"errorht'$prt'":"0x%08x (first: 0x%08x)",' $read_data $first_err
    	fi
    else
        if [[ __print_machinereadable -eq 1 ]]; then  
    		printf '"errorht'$prt'":"none",'
    	fi
      break
    fi
  done
  if [[ __print_machinereadable -eq 0 ]]; then
  	echo "Done"
  else
  	echo -n '"done":'1'}'
  fi
fi
