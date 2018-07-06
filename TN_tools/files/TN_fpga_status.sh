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

  ###>>> Param=0 or 1 or 2
  if [[ $1 -ne 3 ]]; then
    echo -e "\n### Reading system status from common_mmi module:"
    
    tn_ll_mmi_read $C_BASE_ADDR_COMMON_LOWER $C_SUB_ADDR_COMMON_TN_MAJOR_REV
    let    major=$read_data
    tn_ll_mmi_read $C_BASE_ADDR_COMMON_LOWER $C_SUB_ADDR_COMMON_TN_MINOR_REV
    let    minor=$read_data
    tn_ll_mmi_read $C_BASE_ADDR_COMMON_LOWER $C_SUB_ADDR_COMMON_USER_REV
    let     user=$read_data
    echo   "TrustNode FPGA design revision is $major.$minor, with a user sub-version of $user"
    
    tn_ll_mmi_read $C_BASE_ADDR_COMMON_LOWER $C_SUB_ADDR_COMMON_ADDR_MAP_REV
    let    addrmap=$(($read_data & 0x7FFFFFFF))
    echo   "MMI address map revision is $addrmap"
    if [[ $(($read_data & 0x80000000)) -gt 0 ]]; then
      echo "Processor provides and expects second half of the NoC header"
    else
      echo "Processor does not provide or expect NoC header information"
    fi
    
    tn_ll_mmi_read $C_BASE_ADDR_COMMON_LOWER $C_SUB_ADDR_COMMON_FPGA_ID0
    let    fpgaid0=$read_data
    tn_ll_mmi_read $C_BASE_ADDR_COMMON_LOWER $C_SUB_ADDR_COMMON_FPGA_ID1
    let    fpgaid1=$read_data
    printf "Device DNA is 0x%07x%08x\n" $fpgaid1 $fpgaid0
    
    tn_ll_mmi_read $C_BASE_ADDR_COMMON_LOWER $C_SUB_ADDR_COMMON_EFUSE
    let    efuse=$read_data
    echo   "eFUSE key is $efuse"
    
    tn_ll_mmi_read $C_BASE_ADDR_COMMON_LOWER $C_SUB_ADDR_COMMON_BITGEN_TIME
    let       day=$((($read_data & 0xF8000000) >> 27))
    let     month=$((($read_data & 0x07800000) >> 23))
    let      year=$((($read_data & 0x007E0000) >> 17))
    let      hour=$((($read_data & 0x0001F000) >> 12))
    let    minute=$((($read_data & 0x00000FC0) >> 6))
    let    second=$((($read_data & 0x0000003F) >> 0))
    printf   "Bitstream generation start was at %02d.%02d.20%02d %02d:%02d:%02d\n" $day $month $year $hour $minute $second
    
    tn_ll_mmi_read $C_BASE_ADDR_COMMON_LOWER $C_SUB_ADDR_COMMON_FPGA_TEMP
    let    temp=$read_data*504/4096-273
    echo   "FPGA temperature is $temp degrees celsius"
  fi

  ####
  # Stop here, if a parameter was given -> used on logging in
  if [[ $1 -eq 1 ]]; then
    ###>>> Param=1
    exit 0

  elif [[ $1 -ne 3 ]]; then
    ###>>> Param=0 or 2
  
    ####
    tn_ll_mmi_read $C_BASE_ADDR_COMMON_LOWER $C_SUB_ADDR_COMMON_FEATURES_USER
    printf "User Feature Bitmap: 0x%08x\n" $read_data
    
    tn_ll_mmi_read $C_BASE_ADDR_COMMON_LOWER $C_SUB_ADDR_COMMON_FEATURES_6TREE
    let    feature_bitmap=$read_data
    let               st_en=$((($feature_bitmap &  1) >> 0))
    let              st_mmi=$((($feature_bitmap &  2) >> 1))
    let    st_uplink_bitmap=$((($feature_bitmap     ) >> 16))
    if [[ $st_en -eq 0 ]]; then
      echo   "6Tree Enabled: 0"
    else
      printf "6Tree Enabled: $st_en (Display(0) or MMI(1):$st_mmi; Uplink Port Bitmap: 0x%04x)\n" $st_uplink_bitmap
    fi
    #tn_ll_mmi_read $C_BASE_ADDR_NET_LOWER $C_SUB_ADDR_NET_6T_ADDR_L
    #tn_ll_mmi_read $C_BASE_ADDR_NET_LOWER $C_SUB_ADDR_NET_6T_ADDR_H
    #tn_ll_mmi_read $C_BASE_ADDR_NET_LOWER $C_SUB_ADDR_NET_6T_DST_PTR
    #tn_ll_mmi_read $C_BASE_ADDR_NET_LOWER $C_SUB_ADDR_NET_6T_SRC_PTR
    
    tn_ll_mmi_read $C_BASE_ADDR_COMMON_LOWER $C_SUB_ADDR_COMMON_FEATURES_ETH_SW
    let    feature=$read_data
    echo   "Ethernet Switch Enabled: $feature"
    tn_ll_mmi_read $C_BASE_ADDR_COMMON_LOWER $C_SUB_ADDR_COMMON_FEATURES_FLOWCACHE
    let    feature_bitmap=$read_data
    let             fc_en=$((($feature_bitmap &  1) >> 0))
    let         fc_emh_en=$((($feature_bitmap &  2) >> 1))
    let          fc_ls_en=$((($feature_bitmap &  4) >> 2))
    let         fc_ema_en=$((($feature_bitmap &  8) >> 3))
    let   default_is_proc=$((($feature_bitmap & 16) >> 4))
    if [[ $fc_en -eq 0 ]]; then
      echo   "Flow Cache Enabled: 0"
    else
      echo   "Flow Cache Enabled: $fc_en (EMH:$fc_emh_en, LinSearch:$fc_ls_en, EMA:$fc_ema_en, DefaultIsProc:$default_is_proc)"
    fi
    tn_ll_mmi_read $C_BASE_ADDR_COMMON_LOWER $C_SUB_ADDR_COMMON_FEATURES_ACC_DP
    let    feature_bitmap=$read_data
    let          accdp_en=$((($feature_bitmap &          1)))
    let         accdp_cnt=$((($feature_bitmap &     0xFF00) >>  8))
    let     accdp_entries=$((($feature_bitmap & 0xFFFF0000) >> 16))
    if [[ $accdp_en -eq 0 ]]; then
      echo   "Acceleration Datapath Enabled: 0"
    else
      echo   "Acceleration Datapath Enabled: $accdp_en (AccDP Count:$accdp_cnt, AccDP Entry Count:$accdp_entries)"
    fi
    tn_ll_mmi_read $C_BASE_ADDR_COMMON_LOWER $C_SUB_ADDR_COMMON_FEATURES_TX_ROUTER
    let    feature=$read_data
    echo   "TX Route Enabled: $feature"
    tn_ll_mmi_read $C_BASE_ADDR_COMMON_LOWER $C_SUB_ADDR_COMMON_FEATURES_SFP
    let    feature_bitmap=$read_data
    let            sfp_en=$((($feature_bitmap &          1)))
    let           sfp_cnt=$((($feature_bitmap &     0xFF00) >>  8))
    if [[ $sfp_en -eq 0 ]]; then
      echo   "SFP Interface Enabled: 0"
    else
      echo   "SFP Interface Enabled: $sfp_en ($sfp_cnt ports)"
    fi
    tn_ll_mmi_read $C_BASE_ADDR_COMMON_LOWER $C_SUB_ADDR_COMMON_FEATURES_SODIMM
    let    feature=$read_data
    echo   "SODIMM Interface Enabled: $feature"
    
    tn_ll_mmi_read $C_BASE_ADDR_COMMON_LOWER $C_SUB_ADDR_COMMON_PARAM_PRT_CNT
    let    param=$read_data
    echo   "Physical Port Count:       $param"
    tn_ll_mmi_read $C_BASE_ADDR_COMMON_LOWER $C_SUB_ADDR_COMMON_PARAM_DP_CNT
    let    param=$read_data
    echo   "Datapath Count (RX/TX):     $param"
    tn_ll_mmi_read $C_BASE_ADDR_COMMON_LOWER $C_SUB_ADDR_COMMON_PARAM_BUF_CNT
    let    param=$read_data
    echo   "Segment buffer count:     $param"
    tn_ll_mmi_read $C_BASE_ADDR_COMMON_LOWER $C_SUB_ADDR_COMMON_PARAM_RX_MIN
    let    param=$read_data
    echo   "Minimum Frame Size (RX/TX): $param"
    tn_ll_mmi_read $C_BASE_ADDR_COMMON_LOWER $C_SUB_ADDR_COMMON_PARAM_RX_MAX
    let    param=$read_data
    echo   "Maximum Frame Size (RX):  $param"
    tn_ll_mmi_read $C_BASE_ADDR_COMMON_LOWER $C_SUB_ADDR_COMMON_PARAM_TX_PRE
    let    param=$read_data
    echo   "TX Preamble Length:         $param"
    tn_ll_mmi_read $C_BASE_ADDR_COMMON_LOWER $C_SUB_ADDR_COMMON_PARAM_TX_IFG
    let    param=$read_data
    echo   "TX Minimum Inter-Frame Gap: $param"
    
    echo "System connection codings are 0=Unused, 1=Slave Serial Interface, 2=MMI, 3=6Tree Application, 4=User"
    
    tn_ll_mmi_read $C_BASE_ADDR_COMMON_LOWER $C_SUB_ADDR_COMMON_PARAM_DISPLAY
    let    param=$read_data
    echo   "Display Connection:                $param"
    tn_ll_mmi_read $C_BASE_ADDR_COMMON_LOWER $C_SUB_ADDR_COMMON_PARAM_LED_SYNCE
    let    param=$read_data
    echo   "SyncE LED Connection:              $param"
    tn_ll_mmi_read $C_BASE_ADDR_COMMON_LOWER $C_SUB_ADDR_COMMON_PARAM_LED_INTERNAL
    let    param=$read_data
    echo   "Internal LED Connection:           $param"
    tn_ll_mmi_read $C_BASE_ADDR_COMMON_LOWER $C_SUB_ADDR_COMMON_PARAM_BEEP
    let    param=$read_data
    echo   "Beeper Connection:                 $param"
    tn_ll_mmi_read $C_BASE_ADDR_COMMON_LOWER $C_SUB_ADDR_COMMON_PARAM_PHY_RESET
    let    param=$read_data
    echo   "PHY Reset Connection:              $param"
    tn_ll_mmi_read $C_BASE_ADDR_COMMON_LOWER $C_SUB_ADDR_COMMON_PARAM_PINHEADER
    let    param=$read_data
    echo   "Internal Big Pinheader Connection: $param"
    tn_ll_mmi_read $C_BASE_ADDR_COMMON_LOWER $C_SUB_ADDR_COMMON_PARAM_PMOD
    let    param=$read_data
    echo   "Internal PMod Connection:          $param"
    
    tn_ll_mmi_read $C_BASE_ADDR_COMMON_LOWER $C_SUB_ADDR_COMMON_PARAM_EXT_BYPASS
    let    param=$read_data
    echo   "External Bypass active: $param"
    
    tn_ll_mmi_read $C_BASE_ADDR_COMMON_LOWER $C_SUB_ADDR_COMMON_WIDTH_HC_BAD
    let    width=$read_data
    echo   "MMI Header Creation Fragment Counter Width: $width"
    tn_ll_mmi_read $C_BASE_ADDR_COMMON_LOWER $C_SUB_ADDR_COMMON_WIDTH_ETH_SW
    let    width=$((2**$read_data))
    echo   "Number of Ethernet Switch Entries per Datapath: $width"
    tn_ll_mmi_read $C_BASE_ADDR_COMMON_LOWER $C_SUB_ADDR_COMMON_WIDTH_RX_STAT_BAD
    let    width=$read_data
    echo   "RX Statistics Counter Width: $width"
      
    tn_ll_mmi_read $C_BASE_ADDR_COMMON_LOWER $C_SUB_ADDR_COMMON_BUFFER_FULL
    printf "Packet Buffer Full: %d\n" $(($read_data & 1))
  
    tn_ll_mmi_read $C_BASE_ADDR_TM $C_SUB_ADDR_TM_QUEUE_THRES_FULL
    printf "Packet Buffer Queue Threshold: %d\n" $read_data
  
    if [[ $((($read_data & 0x00FF0000) >> 16)) -eq 3 ]]; then
      echo   "RX Datapaths ready.";
    else
      printf "RX Datapaths experience backpressure: 0x%x\n" $((($read_data & 0x00FF0000) >> 24))
    fi
    if [[ $((($read_data & 0xFF000000) >> 24)) -eq 3 ]]; then
      echo   "TX Datapaths ready.";
    else
      printf "TX Datapaths experience backpressure: 0x%x\n" $((($read_data & 0xFF000000) >> 24))
    fi
    
    tn_ll_mmi_read $C_BASE_ADDR_COMMON_LOWER $C_SUB_ADDR_COMMON_TXF_FULL
    if [[ $read_data -eq 0 ]]; then
      echo   "TX FIFO Full Port Bitmap (current): 0";
    else
      printf "TX FIFO Full Port Bitmap (current): 0x%08x\n" $read_data
    fi
    
    tn_ll_mmi_read $C_BASE_ADDR_COMMON_LOWER $C_SUB_ADDR_COMMON_PCIE_TIMEOUT_CNT
    let    pcie_timeouts=$read_data
    echo   "Number of MMI Timeouts (Read/Write, saturating counter): $pcie_timeouts"
    
    echo -e "\n### Current MMI status:"
    let    status=`TNbar1 $(($C_BASE_ADDR_BM*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_BM_MMI_STATUS))   | cut -d " " -f 6`             
    let     err_acc_rd=$((($status & 1) >> 0))                                                                   
    let     err_acc_wr=$((($status & 2) >> 1))                                                                  
    let    err_time_rd=$((($status & 4) >> 2))                                                                  
    let    err_time_wr=$((($status & 8) >> 3))
    let        err_cnt=$((($status & 0xFFF0) >> 4))
    let        acc_cnt=$((($status & 0xFFFF0000) >> 16))
    printf "MMI Statistics (COR): 0x%04x accesses, thereof 0x%03x failing (Errors: AccessErrRd=%d,AccessErrWr=%d,TimeoutRd=%d,TimeoutWr=%d)\n" $acc_cnt $err_cnt $err_acc_rd $err_acc_wr $err_time_rd $err_time_wr
    if [[ $status -eq 0xEEEEEEEE ]]; then
      echo " ** MMI Read Timeout **"
    fi
    
    tn_ll_mmi_read $C_BASE_ADDR_BM $C_SUB_ADDR_BM_MMI_INTERRUPT
    let interrupt=$read_data
    #tn_ll_mmi_read $C_BASE_ADDR_BM $C_SUB_ADDR_BM_MMI_INTERRUPT_EN
    #let interrupt_en=$read_data
    printf "MMI Interrupt register is 0x%08x\n" $interrupt # (enable: 0x%08x) $interrupt_en
  
    ###>>> Param=2
    if [[ $1 -eq 2 ]]; then
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
      #        printf "* SysIfClk:  %d (~1.5625 MHz)\n" $clk_freq
      #        ;;
      #      1)
      #        printf "* Pinheader: %d\n" $clk_freq
      #        ;;
      #      2)
      #        printf "* PMod:      %d\n" $clk_freq
      #        ;;
      #      3)
      #        printf "* FC_CAM:    %d (~100 MHz)\n" $clk_freq
      #        ;;
      #      4)
      #        printf "* SYS_RTC:   %d (~125 MHz)\n" $clk_freq
      #        ;;
      #      5)
      #        printf "* TX_CLK90:  %d (125 MHz, +/- 6250)\n" $clk_freq
      #        ;;
      #      6)
      #        printf "* SGMII:     %d (125 MHz, +/- 6250)\n" $clk_freq
      #        ;;
      #      7)
      #        printf "* TX_CLK:    %d (125 MHz, +/- 6250)\n" $clk_freq
      #        ;;
      #      8)
      #        printf "* ACCDP:     %d (~200 MHz)\n" $clk_freq
      #        ;;
      #      9)
      #        printf "* 200MHzREF: %d (~200 MHz)\n" $clk_freq
      #        ;;
      #     10)
      #        printf "* GPHY0:     %d (2.5/25/125 MHz, +/- 125/1250/6250)\n" $clk_freq
      #        ;;
      #     11)
      #        printf "* GPHY1:     %d (2.5/25/125 MHz, +/- 125/1250/6250)\n" $clk_freq
      #        ;;
      #     12)
      #        printf "* GPHY2:     %d (2.5/25/125 MHz, +/- 125/1250/6250)\n" $clk_freq
      #        ;;
      #     13)
      #        printf "* GPHY3:     %d (2.5/25/125 MHz, +/- 125/1250/6250)\n" $clk_freq
      #        ;;
      #     14)
      #        printf "* GPHY4:     %d (2.5/25/125 MHz, +/- 125/1250/6250)\n" $clk_freq
      #        ;;
      #     15)
      #        printf "* GPHY5:     %d (2.5/25/125 MHz, +/- 125/1250/6250)\n" $clk_freq
      #        ;;
      #     16)
      #        printf "* GPHY6:     %d (2.5/25/125 MHz, +/- 125/1250/6250)\n" $clk_freq
      #        ;;
      #     17)
      #        printf "* GPHY7:     %d (2.5/25/125 MHz, +/- 125/1250/6250)\n" $clk_freq
      #        ;;
      #     18)
      #        printf "* GPHY8:     %d (2.5/25/125 MHz, +/- 125/1250/6250)\n" $clk_freq
      #        ;;
      #     19)
      #        printf "* GPHY9:     %d (2.5/25/125 MHz, +/- 125/1250/6250)\n" $clk_freq
      #        ;;
      #     20)
      #        printf "* AlaskaA:   %d (2.5/25/125 MHz, +/- 125/1250/6250)\n" $clk_freq
      #        ;;
      #     21)
      #        printf "* AlaskaB:   %d (2.5/25/125 MHz, +/- 125/1250/6250)\n" $clk_freq
      #        ;;
      #     22)
      #        printf "* PLL GTP0:  %d (e.g., 125 MHz, +/- 12500)\n" $clk_freq
      #        ;;
      #     23)
      #        printf "* PLL SFP1:  %d (e.g., 125 MHz, +/- 12500)\n" $clk_freq
      #        ;;
      #     24)
      #        printf "* AXI_DP:    %d (~200 MHz)\n" $clk_freq
      #        ;;
      #      *)
      #        printf "* TBD(%d):   %d\n" $clk $clk_freq
      #        ;;
      #    esac
      #  done
      #fi
    
      echo -e "\n### Reading system status from peripherals module"
    
      tn_ll_mmi_read $C_BASE_ADDR_PERIPH $C_SUB_ADDR_PERIPH_BUTTONS
      let peripherals_button_bitmap=$read_data
      let    button_front=$((($peripherals_button_bitmap & 0x0000FFFF)))
      let     button_user=$((($peripherals_button_bitmap & 0x00010000) >> 16))
      if [[ $button_front -eq 0 ]]; then
        echo   "Front Button Bitmap: 0";
      else
        printf "Front Button Bitmap: 0x%04x\n" $button_front
      fi
      echo   "User Button: $button_user"
    
      tn_ll_mmi_read $C_BASE_ADDR_PERIPH $C_SUB_ADDR_PERIPH_GPIOS
      let peripherals_gpio_bitmap=$read_data
      let      pinheader1_gpio_in=$((($peripherals_gpio_bitmap & 0x000003FF)))
      let           pmod0_gpio_in=$((($peripherals_gpio_bitmap & 0x00003C00) >> 10))
      let     pinheader1_gpio_tri=$((($peripherals_gpio_bitmap & 0x03FF0000) >> 16))
      let          pmod0_gpio_tri=$((($peripherals_gpio_bitmap & 0x3C000000) >> 26))
      if [[ $pinheader1_gpio_in -eq 0 && $pinheader1_gpio_tri -eq 0 ]]; then
        echo   "Pinheader1 GPIO Bitmap: 0";
      else
        printf "Pinheader1 GPIO Bitmap: 0x%03x (Read Data Valid: 0x%03x)\n" $pinheader1_gpio_in $pinheader1_gpio_tri
      fi
      if [[ $pmod0_gpio_in -eq 0 && $pmod0_gpio_tri -eq 0 ]]; then
        echo   "PMOD0 GPIO Bitmap: 0";
      else
        printf "PMOD0 GPIO Bitmap: 0x%x (Read Data Valid: 0x%x)\n" $pmod0_gpio_in $pmod0_gpio_tri
      fi
    
      echo -e "\n### Reading interrupt status from MDIO module"
    
      tn_ll_mmi_read $C_BASE_ADDR_MDIO $C_SUB_ADDR_MDIO_INTERRUPT
      let peripherals_interrupt_bitmap=$read_data
      let                 gphy_md_ints=$((($peripherals_interrupt_bitmap & 0x000003FF)))
      let              alaska_ptp_ints=$((($peripherals_interrupt_bitmap & 0x00000C00) >> 10))
      let    alaska_ptp_event_triggers=$((($peripherals_interrupt_bitmap & 0x00030000) >> 16))
      if [[ $gphy_md_ints -eq 0 ]]; then
        echo   "GPHY MDIO Interrupt Bitmap: 0";
      else
        printf "GPHY MDIO Interrupt Bitmap: 0x%03x\n" $gphy_md_ints
      fi
      if [[ $alaska_ptp_ints -eq 0 ]]; then
        echo   "Alaska PTP Interrupt Bitmap: 0";
      else
        printf "Alaska PTP Interrupt Bitmap: 0x%x\n" $alaska_ptp_ints
      fi
      if [[ $alaska_ptp_event_triggers -eq 0 ]]; then
        echo   "Alaska PTP Event Trigger Bitmap: 0";
      else
        printf "Alaska PTP Event Trigger Bitmap: 0x%x\n" $alaska_ptp_event_triggers
      fi
    
      echo -e "\n### Displaying active interrupts on HC modules"
    
      for prt in `seq 0 11`; do
        let hcbase=C_BASE_ADDR_HC_$prt
        tn_ll_mmi_read $hcbase $C_SUB_ADDR_HC_INTERRUPT_EN
        let interrupt_en=$read_data
        tn_ll_mmi_read $hcbase $C_SUB_ADDR_HC_INTERRUPT
        if [[ $read_data -gt 0 && $read_data -ne 0xEEEEEEEE ]]; then
          printf "* Port $prt: %d (enable: 0x%08x)\n" $read_data $interrupt_en
        fi
      done
      for prt in `seq 0 1`; do
        let hcbase=C_BASE_ADDR_HC_PCIE_$prt
        tn_ll_mmi_read $hcbase $C_SUB_ADDR_HC_INTERRUPT_EN
        let interrupt_en=$read_data
        tn_ll_mmi_read $hcbase $C_SUB_ADDR_HC_INTERRUPT
        if [[ $read_data -gt 0 && $read_data -ne 0xEEEEEEEE ]]; then
          printf "* PCIe Port $prt: %d (enable: 0x%08x)\n" $read_data $interrupt_en
        fi
      done
    fi
  fi
	
  ###>>> Param=0 or 2 or 3
	
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
    echo   "FPGA Alarm: 0";
  else
    echo   "FPGA Alarm: $alarm (Overtemperature:$over_temp, User Temperature:$user_temp, AUX Voltage:$vccaux_alarm, Logic Voltage:$vccint_alarm, BRAM Voltage:$vbram_alarm)";
  fi

  ####
  tn_ll_mmi_read $C_BASE_ADDR_COMMON_LOWER $C_SUB_ADDR_COMMON_CONFIG_CHECK
  let      config_check_bitmap=$read_data
  let           crc_error=$((($config_check_bitmap & 1) >> 0))
  let           ecc_error=$((($config_check_bitmap & 2) >> 1))
  let    single_ecc_error=$((($config_check_bitmap & 4) >> 2))
  echo   "FPGA State: CRC Error=$crc_error, ECC Error=$ecc_error (Single=$single_ecc_error)"
  
  tn_ll_mmi_read $C_BASE_ADDR_COMMON_LOWER $C_SUB_ADDR_COMMON_FIFO_OVERFLOW0
  let    overflow_bitmap0=$read_data
  tn_ll_mmi_read $C_BASE_ADDR_COMMON_LOWER $C_SUB_ADDR_COMMON_FIFO_OVERFLOW1
  let    overflow_bitmap1=$read_data
  tn_ll_mmi_read $C_BASE_ADDR_COMMON_LOWER $C_SUB_ADDR_COMMON_FIFO_OVERFLOW2
  let    overflow_bitmap2=$read_data
  tn_ll_mmi_read $C_BASE_ADDR_COMMON_LOWER $C_SUB_ADDR_COMMON_FIFO_OVERFLOW3
  let    overflow_bitmap3=$read_data
  if [[ $overflow_bitmap0 -eq 0 && $overflow_bitmap1 -eq 0 && $overflow_bitmap2 -eq 0 && $overflow_bitmap3 -eq 0 ]]; then
    echo "FIFO Overflow Bitmap: 0"
  else
    printf "FIFO Overflow Bitmap: 0x%08x%08x%08x%08x\n" $overflow_bitmap3 $overflow_bitmap2 $overflow_bitmap1 $overflow_bitmap0
  fi
  
  tn_ll_mmi_read $C_BASE_ADDR_COMMON_LOWER $C_SUB_ADDR_COMMON_FIFO_UNDERRUN0
  let    underrun_bitmap0=$read_data
  tn_ll_mmi_read $C_BASE_ADDR_COMMON_LOWER $C_SUB_ADDR_COMMON_FIFO_UNDERRUN1
  let    underrun_bitmap1=$read_data
  tn_ll_mmi_read $C_BASE_ADDR_COMMON_LOWER $C_SUB_ADDR_COMMON_FIFO_UNDERRUN2
  let    underrun_bitmap2=$read_data
  tn_ll_mmi_read $C_BASE_ADDR_COMMON_LOWER $C_SUB_ADDR_COMMON_FIFO_UNDERRUN3
  let    underrun_bitmap3=$read_data
  if [[ $underrun_bitmap0 -eq 0 && $underrun_bitmap1 -eq 0 && $underrun_bitmap2 -eq 0 && $underrun_bitmap3 -eq 0 ]]; then
    echo "FIFO Underrun Bitmap: 0"
  else
    printf "FIFO Underrun Bitmap: 0x%08x%08x%08x%08x\n" $underrun_bitmap3 $underrun_bitmap2 $underrun_bitmap1 $underrun_bitmap0
  fi
  
  tn_ll_mmi_read $C_BASE_ADDR_COMMON_LOWER $C_SUB_ADDR_COMMON_PCIE_ERROR0
  let    pcie_error_bitmap0=$read_data
  tn_ll_mmi_read $C_BASE_ADDR_COMMON_LOWER $C_SUB_ADDR_COMMON_PCIE_ERROR1
  let    pcie_error_bitmap1=$read_data
  if [[ $pcie_error_bitmap1 -eq 0 && $pcie_error_bitmap0 -eq 0 ]]; then
    echo "PCIe Error Bitmap: 0"
  else
    printf "PCIe Error Bitmap: 0x%08x%08x\n" $pcie_error_bitmap1 $pcie_error_bitmap0
  fi
  
  tn_ll_mmi_read $C_BASE_ADDR_TM $C_SUB_ADDR_TM_PTR_CNT
  echo   "Packet Buffers in use: $read_data"
  if [[ $read_data -gt 3000 ]]; then
    echo " - ### Packet Buffer Underrun - System crashed ###"
  fi;
  if [[ $read_data -gt 2700 && $read_data -lt 3001 ]]; then
    echo " - Packet buffer full - possibly tail dropping newly received packets"
  fi;

  echo -e "\n### Displaying NoC Monitor Results"

  for prt in `seq 0 11; seq 14 15`; do
    let hcbase=C_BASE_ADDR_NOC_MON_HC_$prt
    tn_ll_mmi_read $hcbase $C_SUB_ADDR_NOC_MON_FIRST_ERR
    let first_err=$read_data
    tn_ll_mmi_read $hcbase $C_SUB_ADDR_NOC_MON_STATUS
    if [[ $read_data -gt 0 && $read_data -ne 0xEEEEEEEE ]]; then
      printf "* Error Bitmap HC $prt: 0x%08x (first: 0x%08x)\n" $read_data $first_err
    fi
  done
  for dp in `seq 0 1`; do
    let hcbase=C_BASE_ADDR_NOC_MON_RX_IN_$prt
    tn_ll_mmi_read $hcbase $C_SUB_ADDR_NOC_MON_FIRST_ERR
    let first_err=$read_data
    tn_ll_mmi_read $hcbase $C_SUB_ADDR_NOC_MON_STATUS
    if [[ $read_data -gt 0 && $read_data -ne 0xEEEEEEEE ]]; then
      printf "* Error Bitmap RX_IN $dp: 0x%08x (first: 0x%08x)\n" $read_data $first_err
    fi
  done
  for dp in `seq 0 1`; do
    let hcbase=C_BASE_ADDR_NOC_MON_RX_OUT_$prt
    tn_ll_mmi_read $hcbase $C_SUB_ADDR_NOC_MON_FIRST_ERR
    let first_err=$read_data
    tn_ll_mmi_read $hcbase $C_SUB_ADDR_NOC_MON_STATUS
    if [[ $read_data -gt 0 && $read_data -ne 0xEEEEEEEE ]]; then
      printf "* Error Bitmap RX_OUT $dp: 0x%08x (first: 0x%08x)\n" $read_data $first_err
    fi
  done
  for prt in `seq 0 11; seq 14 15`; do
	  # HT address range is currently unused, so NoC Monitor is connected there
    let hcbase=C_BASE_ADDR_HT_$prt
    tn_ll_mmi_read $hcbase $C_SUB_ADDR_NOC_MON_FIRST_ERR
    let first_err=$read_data
    tn_ll_mmi_read $hcbase $C_SUB_ADDR_NOC_MON_STATUS
    if [[ $read_data -gt 0 && $read_data -ne 0xEEEEEEEE ]]; then
      printf "* Error Bitmap HT $prt: 0x%08x (first: 0x%08x)\n" $read_data $first_err
    fi
  done

  echo "Done"
fi
