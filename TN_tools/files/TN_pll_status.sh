#!/bin/bash

source /usr/share/InnoRoute/tn_env.sh
source /usr/share/InnoRoute/tn_func_ll.sh

if [[ $# == 0 ]]; then
  echo "$0 <ex> is used to read the current AD9558 PLL status"
  echo "The parameter <ex> can have any value. Without it, this help is displayed"
else
  let plladdr=0x69
  
  echo "PLL Status:"
  
  let sypll=`pllctl 0x69 0x0d01 | tr -d '\n' | sed -e 's/0x69 0x0d01read-->//g'`
  if [[ $(( $sypll & 1 )) -eq 1 ]]; then
    echo "* Raw SysClk Locked";
  else
    echo "* Raw SysClk NOT Locked";
  fi
  if [[ $(( $sypll & 2 )) -eq 2 ]]; then
    echo "* SysClk Stable";
  else
    echo "* SysClk NOT Stable";
  fi
  if [[ $(( $sypll & 4 )) -eq 4 ]]; then
    echo "* APLL Locked";
  else
    echo "* APLL NOT Locked";
  fi
  if [[ $(( $sypll & 8 )) -eq 8 ]]; then
    echo "* APLL Calibration in Progress";
  fi
  if [[ $(( $sypll & 16 )) -eq 16 ]]; then
    echo "* APLL VCO Status is high";
  else
    echo "* APLL VCO Status is low";
  fi
  if [[ $(( $sypll & 32 )) -eq 32 ]]; then
    echo "* All PLLs Locked";
  else
    echo "* NOT all PLLs Locked";
  fi
  if [[ $(( $sypll & 64 )) -eq 64 ]]; then
    echo "* DPLL APLL Locked";
  else
    echo "* DPLL APLL NOT Locked";
  fi
  if [[ $(( $sypll & 128 )) -eq 128 ]]; then
    echo "* SysClk Locked";
  else
    echo "* SysClk NOT Locked";
  fi
  
  let dpll1=`pllctl 0x69 0x0d08 | tr -d '\n' | sed -e 's/0x69 0x0d08read-->//g'`
  if [[ $(( $dpll1 & 1 )) -eq 1 ]]; then
    echo "* DPLL Free Run";
  fi
  if [[ $(( $dpll1 & 2 )) -eq 2 ]]; then
    echo "* DPLL Active";
  fi
  if [[ $(( $dpll1 & 4 )) -eq 4 ]]; then
    echo "* DPLL Holdover";
  fi
  if [[ $(( $dpll1 & 8 )) -eq 8 ]]; then
    echo "* DPLL Loop Switching";
  fi
  if [[ $(( $dpll1 & 16 )) -eq 16 ]]; then
    echo "* DPLL Phase Locked";
  else
    echo "* DPLL NOT Phase Locked";
  fi
  if [[ $(( $dpll1 & 32 )) -eq 32 ]]; then
    echo "* DPLL Frequency Locked";
  else
    echo "* DPLL NOT Frequency Locked";
  fi
  if [[ $(( $dpll1 & 64 )) -eq 64 ]]; then
    echo "* DPLL Offset Slew Limiting";
  fi
  
  let dpll2=`pllctl 0x69 0x0d09 | tr -d '\n' | sed -e 's/0x69 0x0d09read-->//g'`
  if [[ $(( $dpll2 & 3 )) -eq 0 ]]; then
    echo "* Active Reference is REFA (SyncE Port A)";
  elif [[ $(( $dpll2 & 3 )) -eq 1 ]]; then
    echo "* Active Reference is REFB (SyncE Port B)";
  elif [[ $(( $dpll2 & 3 )) -eq 2 ]]; then
    echo "* Active Reference is REFC (BNC)";
  else
    echo "* Active Reference is REFD (Artix FPGA)";
  fi
  if [[ $(( $dpll2 & 4 )) -eq 4 ]]; then
    echo "* DPLL Active Reference Priority is high";
  else
    echo "* DPLL Active Reference Priority is low";
  fi
  if [[ $(( $dpll2 & 16 )) -eq 16 ]]; then
    echo "* DPLL History Available";
  else
    echo "* DPLL History NOT Available";
  fi
  if [[ $(( $dpll2 & 32 )) -eq 32 ]]; then
    echo "* DPLL Frequency Clamped";
  else
    echo "* DPLL NOT Frequency Clamped";
  fi
  
  let refab=`pllctl 0x69 0x0d0b | tr -d '\n' | sed -e 's/0x69 0x0d0bread-->//g'`
  echo "REFA = SyncE Port A"
  if [[ $(( $refab & 1 )) -eq 1 ]]; then
    echo "* REFA Slow";
  fi
  if [[ $(( $refab & 2 )) -eq 2 ]]; then
    echo "* REFA Fast";
  fi
  if [[ $(( $refab & 4 )) -eq 4 ]]; then
    echo "* REFA Fault";
  fi
  if [[ $(( $refab & 8 )) -eq 8 ]]; then
    echo "* REFA Valid";
  fi
  echo "REFB = SyncE Port B"
  if [[ $(( $refab & 16 )) -eq 16 ]]; then
    echo "* REFB Slow";
  fi
  if [[ $(( $refab & 32 )) -eq 32 ]]; then
    echo "* REFB Fast";
  fi
  if [[ $(( $refab & 64 )) -eq 64 ]]; then
    echo "* REFB Fault";
  fi
  if [[ $(( $refab & 128 )) -eq 128 ]]; then
    echo "* REFB Valid";
  fi
  
  echo "REFC = BNC In"
  let refcd=`pllctl 0x69 0x0d0c | tr -d '\n' | sed -e 's/0x69 0x0d0cread-->//g'`
  if [[ $(( $refcd & 1 )) -eq 1 ]]; then
    echo "* REFC Slow";
  fi
  if [[ $(( $refcd & 2 )) -eq 2 ]]; then
    echo "* REFC Fast";
  fi
  if [[ $(( $refcd & 4 )) -eq 4 ]]; then
    echo "* REFC Fault";
  fi
  if [[ $(( $refcd & 8 )) -eq 8 ]]; then
    echo "* REFC Valid";
  fi
  echo "REFD = Artix-provided Clock (V1.2 and later)"
  if [[ $(( $refcd & 16 )) -eq 16 ]]; then
    echo "* REFD Slow";
  fi
  if [[ $(( $refcd & 32 )) -eq 32 ]]; then
    echo "* REFD Fast";
  fi
  if [[ $(( $refcd & 64 )) -eq 64 ]]; then
    echo "* REFD Fault";
  fi
  if [[ $(( $refcd & 128 )) -eq 128 ]]; then
    echo "* REFD Valid";
  fi
  echo "Done"
fi
