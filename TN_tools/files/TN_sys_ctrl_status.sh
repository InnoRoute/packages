#!/bin/bash

source /usr/share/InnoRoute/tn_env.sh
source /usr/share/InnoRoute/tn_func_ll.sh

if [[ $# == 0 ]]; then
  echo "$0 <ex> is used to display the system controller status."
  echo "The parameter <ex> can have any value. Without it, this help is displayed"
  echo "Some values are latched since the last time they were read,"
  echo "so it might be necessary to execute it twice to get the most recent values"
else

  # System Controller has SMBus address 0x04
  
  echo "SysCtrl values since last read:"
  
  let firmware=`i2cget -y 0 4 0x00`
  printf  "*  0: Firmware Version (RO)                      %d\n" $firmware
  case $firmware in
  [1-3])
    echo "*     -> FW0.9";;
  [4-5])
    echo "*     -> FW1.0";;
  6)
    echo "*     -> FW1.1";;
  7)
    echo "*     -> FW1.2";;
  8)
    echo "*     -> FW1.3";;
  9)
    echo "*     -> FW1.4";;
  10)
    echo "*     -> FW1.5";;
  11)
    echo "*     -> FW1.6";;
  12)
    echo "*     -> FW1.7";;
  13)
    echo "*     -> FW1.8";;
  [14-15])
    echo "*     -> FW1.9";;
  *)
    echo "*     -> TBD";;
  esac
  
  let clkbuf=`i2cget -y 0 4 0x01`
  printf  "*  1: Clock Buffer (LOS & MUX) (RW)              0x%02x\n" $clkbuf
  if [[ $(( $clkbuf & 1 )) -eq 0 ]]; then
    echo "*     -> Currently TCXO clock selected";
  fi
  if [[ $(( $clkbuf & 1 )) -eq 1 ]]; then
    echo "*     -> Currently PLL clock selected";
  fi
  if [[ $(( $clkbuf & 16 )) -eq 16 ]]; then
    echo "*     -> Currently TCXO Loss-of-Signal -> System down";
  fi
  if [[ $(( $clkbuf & 32 )) -eq 32 ]]; then
    echo "*     -> Currently PLL Loss-of-Signal -> TCXO clock active";
  fi
  
  let biterr=`i2cget -y 0 4 0x02`
  printf  "*  2: Bitstream Error (ID-ERR+ID & CONF-ERR (R); OPTO (W)) 0x%02x\n" $biterr
  if [[ $(( $biterr & 140 )) -eq 0 ]]; then
    printf "*     -> Currently no error\n";
  fi
  if [[ $(( $biterr & 128 )) -eq 128 ]]; then
    printf "*     -> Currently ID Error with ID %d\n" $(( ($biterr & 112) / 16));
  fi
  if [[ $(( $biterr & 3 )) -eq 1 ]]; then
    echo "*     -> Currently Flash Error";
  fi
  if [[ $(( $biterr & 3 )) -eq 2 ]]; then
    echo "*     -> Currently LPC Error";
  fi
  if [[ $(( $biterr & 3 )) -eq 3 ]]; then
    echo "*     -> Currently OPTO Error";
  fi
  if [[ $(( $biterr & 4 )) -gt 0 ]]; then
    echo "*     -> INIT_B went low";
  fi
  if [[ $(( $biterr & 8 )) -gt 0 ]]; then
    echo "*     -> DONE timeout";
  fi
  
  let mutestuckfan=`i2cget -y 0 4 0x03`
  printf  "*  3: Mute & Stuck Fan (RW)                      0x%02x\n" $mutestuckfan
  if [[ $(( $mutestuckfan & 1 )) -eq 1 ]]; then
    echo "*     -> Fan is currently stuck";
  fi
  if [[ $(( $mutestuckfan & 16 )) -eq 16 ]]; then
    echo "*     -> All sounds currently muted";
  fi
  
  let pll=`i2cget -y 0 4 0x04`
  printf  "*  4: PLL (Current M Pins value; RO)             0x%02x\n" $pll
  #if [[ $(( $pll & 1 )) -eq 1 ]]; then
  #  echo "*     -> M0 is high";
  #fi
  #if [[ $(( $pll & 2 )) -eq 2 ]]; then
  #  echo "*     -> M1 is high";
  #fi
  #if [[ $(( $pll & 4 )) -eq 4 ]]; then
  #  echo "*     -> M2 is high";
  #fi
  #if [[ $(( $pll & 8 )) -eq 8 ]]; then
  #  echo "*     -> M3 is high";
  #fi
  #if [[ $(( $pll & 16 )) -eq 16 ]]; then
  #  echo "*     -> M4 is high"; # "Possible encoding: currently Ref A valid (SyncE Port A, if default config)";
  #fi
  #if [[ $(( $pll & 32 )) -eq 32 ]]; then
  #  echo "*     -> M5 is high"; # "Possible encoding: currently Ref B valid (SyncE Port B, if default config)";
  #fi
  #if [[ $(( $pll & 64 )) -eq 64 ]]; then
  #  echo "*     -> M6 is high"; # "Possible encoding: currently Ref C valid (BNC In, if default config)";
  #fi
  #if [[ $(( $pll & 128 )) -eq 128 ]]; then
  #  echo "*     -> M7 is high"; # "Possible encoding: currently Ref D valid (Artix FPGA, if default config)";
  #fi
  
  let dutycycle=`i2cget -y 0 4 0x05`
  let dutycyclepercent=$dutycycle*25/16
  printf  "*  5: Fan Duty Cycle {20..64} (RW)               %d (%d percent)\n" $dutycycle $dutycyclepercent
  
  let fanspeed=32*`i2cget -y 0 4 0x06`
  printf  "*  6: Fan Speed (RPM; RO)                        %d\n" $fanspeed
  
  printf  "*  7: Board Pinout ID (RO)                       %d\n" `i2cget -y 0 4 0x07`
  
  printf  "*  8: Successful Artix Program Count (RO)        %d\n" `i2cget -y 0 4 0x08`
  echo    "*     (incl. successful flash loads after failed OPTO/LPC configuration)"
  
  if [[ $firmware > 11 ]]; then
    printf  "*     Failed Artix Program Count (DONE/INITB;RO) %d\n" `i2cget -y 0 4 0x1b`;
    echo    "*     (OPTO/LPC/flash, but not JTAG)"
  fi
  
  printf  "*  9: Front buttons (RO)                         0x%02x%02x\n" `i2cget -y 0 4 0x0a` `i2cget -y 0 4 0x09`
  
  let event0=`i2cget -y 0 4 0x0b`
  let event1=`i2cget -y 0 4 0x0c`
  let event2=`i2cget -y 0 4 0x0d`
  let event3=`i2cget -y 0 4 0x0e`
  let event4=`i2cget -y 0 4 0x0f`
  printf  "* 10: Event latch (RO)                           0x%02x%02x%02x%02x%02x\n" $event4 $event3 $event2 $event1 $event0
  if [[ $(( $event0 & 1 )) -eq 1 ]]; then
    echo "*     -> FPGA or environmental overtemperature";
  fi
  if [[ $(( $event0 & 2 )) -eq 2 ]]; then
    echo "*     -> Atom shutdown due to overtemperature";
  fi
  if [[ $(( $event0 & 4 )) -eq 4 ]]; then
    echo "*     -> SODIMM overtemperature";
  fi
  if [[ $(( $event0 & 8 )) -eq 8 ]]; then
    echo "*     -> Fan does not rotate";
  fi
  if [[ $(( $event0 & 16 )) -eq 16 ]]; then
    echo "*     -> (Re)configuration";
  fi
  if [[ $(( $event0 & 32 )) -eq 32 ]]; then
    echo "*     -> CRC error or other bitstream error";
  fi
  if [[ $(( $event0 & 64 )) -eq 64 ]]; then
    echo "*     -> DONE timeout at (re)configuration";
  fi
  if [[ $(( $event0 & 128 )) -eq 128 ]]; then
    echo "*     -> Bad bitstream identifier";
  fi
  
  if [[ $(( $event1 & 1 )) -eq 1 ]]; then
    echo "*     -> No Atom module detected";
  fi
  if [[ $(( $event1 & 2 )) -eq 2 ]]; then
    echo "*     -> USB3.0 overcurrent";
  fi
  if [[ $(( $event1 & 4 )) -eq 4 ]]; then
    echo "*     -> USB2.0 overcurrent";
  fi
  if [[ $(( $event1 & 8 )) -eq 8 ]]; then
    echo "*     -> Reserved";
  fi
  if [[ $(( $event1 & 16 )) -eq 16 ]]; then
    echo "*     -> PCIe reset";
  fi
  if [[ $(( $event1 & 32 )) -eq 32 ]]; then
    echo "*     -> Atom watchdog timeout";
  fi
  if [[ $(( $event1 & 64 )) -eq 64 ]]; then
    echo "*     -> Atom sleep/reset button pressed";
  fi
  if [[ $(( $event1 & 128 )) -eq 128 ]]; then
    echo "*     -> FPGA reset/reconfiguration button pressed";
  fi
  
  if [[ $(( $event2 & 1 )) -eq 1 ]]; then
    echo "*     -> User-requested beep";
  fi
  if [[ $(( $event2 & 2 )) -eq 2 ]]; then
    echo "*     -> Display switched off";
  fi
  if [[ $(( $event2 & 4 )) -eq 4 ]]; then
    echo "*     -> Bad serial interfaces frames";
  fi
  if [[ $(( $event2 & 8 )) -eq 8 ]]; then
    echo "*     -> Mute all sounds"; # REDUNDANT
  fi
  if [[ $(( $event2 & 16 )) -eq 16 ]]; then
    echo "*     -> TXCO does not run -> system halted";
  fi
  if [[ $(( $event2 & 32 )) -eq 32 ]]; then
    echo "*     -> PLL does not run -> no SyncE/BNC/SFP support";
  fi
  if [[ $(( $event2 & 64 )) -eq 64 ]]; then
    echo "*     -> Interrupt request from PLL: high";
  fi
  if [[ $(( $event2 & 128 )) -eq 128 ]]; then
    echo "*     -> Interrupt request from PLL: low";
  fi
  
  if [[ $(( $event3 & 1 )) -eq 1 ]]; then
    echo "*     -> PLL-M0 high";
  fi
  if [[ $(( $event4 & 1 )) -eq 1 ]]; then
    echo "*     -> PLL-M0 low";
  fi
  
  if [[ $(( $event3 & 2 )) -eq 2 ]]; then
    echo "*     -> PLL-M1 high";
  fi
  if [[ $(( $event4 & 2 )) -eq 2 ]]; then
    echo "*     -> PLL-M1 low";
  fi
  
  if [[ $(( $event3 & 4 )) -eq 4 ]]; then
    echo "*     -> PLL-M2 high";
  fi
  if [[ $(( $event4 & 4 )) -eq 4 ]]; then
    echo "*     -> PLL-M2 low";
  fi
  
  if [[ $(( $event3 & 8 )) -eq 8 ]]; then
    echo "*     -> PLL-M3 high";
  fi
  if [[ $(( $event4 & 8 )) -eq 8 ]]; then
    echo "*     -> PLL-M3 low";
  fi
  
  if [[ $(( $event3 & 16 )) -eq 16 ]]; then
    echo "*     -> PLL-M4 high";
  fi
  if [[ $(( $event4 & 16 )) -eq 16 ]]; then
    echo "*     -> PLL-M4 low";
  fi
  
  if [[ $(( $event3 & 32 )) -eq 32 ]]; then
    echo "*     -> PLL-M5 high";
  fi
  if [[ $(( $event4 & 32 )) -eq 32 ]]; then
    echo "*     -> PLL-M5 low";
  fi
  
  if [[ $(( $event3 & 64 )) -eq 64 ]]; then
    echo "*     -> PLL-M6 high";
  fi
  if [[ $(( $event4 & 64 )) -eq 64 ]]; then
    echo "*     -> PLL-M6 low";
  fi
  
  if [[ $(( $event3 & 128 )) -eq 128 ]]; then
    echo "*     -> PLL-M7 high";
  fi
  if [[ $(( $event4 & 128 )) -eq 128 ]]; then
    echo "*     -> PLL-M7 low";
  fi
  
  printf  "* 11: Uptime (RO;serially read)                  0x%02x%02x%02x%02x%02x%02x%02x%02x\n" `i2cget -y 0 4 0x17` `i2cget -y 0 4 0x16` `i2cget -y 0 4 0x15` `i2cget -y 0 4 0x14` `i2cget -y 0 4 0x13` `i2cget -y 0 4 0x12` `i2cget -y 0 4 0x11` `i2cget -y 0 4 0x10`
  let rtc_sec0=`i2cget -y 0 4 0x14`
  let rtc_sec1=`i2cget -y 0 4 0x15`
  let rtc_sec2=`i2cget -y 0 4 0x16`
  let rtc_sec3=`i2cget -y 0 4 0x17`
  let rtc_sec=`printf "0x%02x%02x%02x%02x" $rtc_sec3 $rtc_sec2 $rtc_sec1 $rtc_sec0`
  printf "*     -> Currently number of seconds:    %d\n" $rtc_sec;
  let rtc_days=$rtc_sec/86400;
  let rtc_hours=$rtc_sec/3600-$rtc_days*24;
  let rtc_mins=$rtc_sec/60-$rtc_days*1440-$rtc_hours*60;
  let rtc_sec_rem=$rtc_sec-$rtc_days*86400-$rtc_hours*3600-$rtc_mins*60;
  printf "*           -> %03dd, %02dh:%02dm:%02ds\n" $rtc_days $rtc_hours $rtc_mins $rtc_sec_rem;
  echo    "*     (serial reading leads to inconsistent values at counter lapse)"
  
  # The following does not make sense, because the serial SMBus accesses take too long:
  #let rtc_qns0=`i2cget -y 0 4 0x10`
  #let rtc_qns1=`i2cget -y 0 4 0x11`
  #let rtc_qns2=`i2cget -y 0 4 0x12`
  #let rtc_qns3=`i2cget -y 0 4 0x13`
  #let rtc_qns=`printf "0x%02x%02x%02x%02x" $rtc_qns3 $rtc_qns2 $rtc_qns1 $rtc_qns0`
  #printf "*     -> Currently number of quarter ns: %d\n" $rtc_qns;
  #let rtc_ms=$rtc_qns/4000000;
  #let rtc_us=$rtc_qns/4000-$rtc_ms*1000;
  #let rtc_ns=$rtc_qns/4-$rtc_ms*1000000-$rtc_us*1000;
  #let rtc_ps_rem=$rtc_qns*250-$rtc_ms*4000000*250-$rtc_us*4000*250-$rtc_ns*4*250;
  #printf "*           -> %03dms:%03dus:%03dns:%03dps\n" $rtc_ms $rtc_us $rtc_ns $rtc_ps_rem;
  
  printf  "* 12: Status register (RO)                       0x%02x\n" `i2cget -y 0 4 0x18`
  
  if [[ $firmware > 11 ]]; then
    printf  "* 13: PHY Resets (RW)                            0x%02x\n" `i2cget -y 0 4 0x19`;
    let sysmon=`i2cget -y 0 4 0x1a`;
    printf  "* 14: System Monitor (RO)                        0x%02x\n" $sysmon;  
    if [[ $(( $sysmon & 1 )) -eq 1 ]]; then
      echo "*     -> Internal FSM Reset";
    fi
    if [[ $(( $sysmon & 2 )) -eq 2 ]]; then
      echo "*     -> Artix PROGRAM_B";
    fi
    if [[ $(( $sysmon & 4 )) -eq 4 ]]; then
      echo "*     -> FPGA-ID PROGRAM_B";
    fi
    if [[ $(( $sysmon & 8 )) -eq 8 ]]; then
      echo "*     -> Atom WDT";
    fi
    if [[ $(( $sysmon & 16 )) -eq 16 ]]; then
      echo "*     -> Atom Reset Button";
    fi
    if [[ $(( $sysmon & 32 )) -eq 32 ]]; then
      echo "*     -> FPGA Restart";
    fi
    if [[ $(( $sysmon & 64 )) -eq 64 ]]; then
      echo "*     -> Unknown Flag";
    fi
    if [[ $(( $sysmon & 128 )) -eq 128 ]]; then
      echo "*     -> Unknown Flag";
    fi
    let pmod=`i2cget -y 0 4 0x1c`;
    printf  "* 15: System Controller PMod function (RW)       0x%02x\n" $pmod;
    if [[ $pmod == 0 ]]; then
      echo "*     -> Clock Output";
    elif [[ $pmod == 1 ]]; then
      echo "*     -> FPGA UART";
    elif [[ $pmod == 2 ]]; then
      echo "*     -> Second Atom UART";
    else
      echo "*     -> High-Z";
    fi 
  fi

  echo "Done"
fi
