#!/bin/bash

source /usr/share/InnoRoute/tn_env.sh
source /usr/share/InnoRoute/tn_func_ll.sh

if [[ $# == 0 ]]; then
  echo "$0 <parameters> is used to display bitmaps on the front panel 7-segment displays,"
  echo "with one segment per bit. Each display consists of 1 byte (8 bits),"
  echo "with the following bit coding:"
  echo " "
  echo "   00000"
  echo "  5     1"
  echo "  5     1"
  echo "  5     1"
  echo "   66666 "
  echo "  4     2"
  echo "  4     2"
  echo "  4     2"
  echo "   33333  77"
  echo " "
  echo "The displays are counted per window, from left to right, with displays"
  echo "within the windows also from left to right:"
  echo " -> 1-2-3-4-5-6-7-8-9-10-11-12-13-14-15-16 or"
  echo " -> 1-2-3-4 5-6-7-8 9-10-11-12 13-14-15-16"
  echo " "
  echo "Either a single, 4, 5, 16, or 17 parameters can be given. The last parameter"
  echo "in case of odd parameter counts is the brightness, which is"
  echo "between 0 (off) and 3 (max)"
  echo "   1 parameter: enable all segments, <parameter> sets the brightness."
  echo "                example: $0 3 -> all segments at max. brightness"
  #echo "   4 parameter: each parameter is a 32 bit word setting one 'window',"
  #echo "                with the least significant byte on the left of each window."
  #echo "                $0 0 0 0 0x01000000 would illuminate just the leftmost"
  #echo "                display's top segment."
  #echo "                example: $0 0x04030201 0x08070605 0x12111009 0x16151413"
  #echo "   5 parameter: same as 4 parameters, with brightness as last parameter."
  #echo "                example: $0 0x04030201 0x08070605 0x12111009 0x16151413 2"
  echo "  16 parameter: each parameter is a 8 bit word setting one display."
  echo "                example: $0 0x01 0x02 0x03 0x04 0x05 0x06 0x07 0x08 0x09 0x10 0x11 0x12 0x13 0x14 0x15 0x16"
  echo "  17 parameter: same as 16 parameters, with brightness as last parameter."
  echo "                example: $0 0x01 0x02 0x03 0x04 0x05 0x06 0x07 0x08 0x09 0x10 0x11 0x12 0x13 0x14 0x15 0x16 1"
  # Special gimmic: if all segments are on and brightness is 0 (off), then the rightmost dot is shown - at 100%
else
  if [ -z "$(lspci -mm -d 10ee:0000)" ]; then echo "No PCIe connection to FPGA. Exiting."; exit 1; fi

  # Set default brightness (max.)
  let brightness=3
  let display1=0xFFFFFFFF
  let display2=0xFFFFFFFF
  let display3=0xFFFFFFFF
  let display4=0xFFFFFFFF
  
  case $# in
    1)
      # Set all segments to on
      let brightness=$1
      ;;
    #4)
    #  # If parameters are given, it is assumed that they are the display values
    #  let display1=$1
    #  let display2=$2
    #  let display3=$3
    #  let display4=$4
    #  ;;
    #5)
    #  # If parameters are given, it is assumed that they are the display values
    #  let display1=$1
    #  let display2=$2
    #  let display3=$3
    #  let display4=$4
    #  let brightness=$5
    #  ;;
    16)
      let digit1=${1}
      let digit2=${2}
      let digit3=${3}
      let digit4=${4}
      let digit5=${5}
      let digit6=${6}
      let digit7=${7}
      let digit8=${8}
      let digit9=${9}
      let digit10=${10}
      let digit11=${11}
      let digit12=${12}
      let digit13=${13}
      let digit14=${14}
      let digit15=${15}
      let digit16=${16}
  
      # If parameters are given, it is assumed that they are the display values
      # Convert to 4-parameter mode
      let display1=$digit4*0x1000000+$digit3*0x10000+$digit2*0x100+$digit1
      let display2=$digit8*0x1000000+$digit7*0x10000+$digit6*0x100+$digit5
      let display3=$digit12*0x1000000+$digit11*0x10000+$digit10*0x100+$digit9
      let display4=$digit16*0x1000000+$digit15*0x10000+$digit14*0x100+$digit13
      #let display1=$digit4+$digit3*0x100+$digit2*0x10000+$digit1*0x1000000
      #let display2=$digit8+$digit7*0x100+$digit6*0x10000+$digit5*0x1000000
      #let display3=$digit12+$digit11*0x100+$digit10*0x10000+$digit9*0x1000000
      #let display4=$digit16+$digit15*0x100+$digit14*0x10000+$digit13*0x1000000
      ;;
    17)
      let digit1=${1}
      let digit2=${2}
      let digit3=${3}
      let digit4=${4}
      let digit5=${5}
      let digit6=${6}
      let digit7=${7}
      let digit8=${8}
      let digit9=${9}
      let digit10=${10}
      let digit11=${11}
      let digit12=${12}
      let digit13=${13}
      let digit14=${14}
      let digit15=${15}
      let digit16=${16}
      let brightness=${17}
  
      # If parameters are given, it is assumed that they are the display values
      # Convert to 4-parameter mode
      let display1=$digit4*0x1000000+$digit3*0x10000+$digit2*0x100+$digit1
      let display2=$digit8*0x1000000+$digit7*0x10000+$digit6*0x100+$digit5
      let display3=$digit12*0x1000000+$digit11*0x10000+$digit10*0x100+$digit9
      let display4=$digit16*0x1000000+$digit15*0x10000+$digit14*0x100+$digit13
      #let display1=$digit1+$digit2*0x100+$digit3*0x10000+$digit4*0x1000000
      #let display2=$digit8+$digit6*0x100+$digit7*0x10000+$digit8*0x1000000
      #let display3=$digit9+$digit10*0x100+$digit11*0x10000+$digit12*0x1000000
      #let display4=$digit13+$digit14*0x100+$digit15*0x10000+$digit16*0x1000000
      ;;
    *)
      echo "$0: Wrong number of parameters ($#)"
      ;;
  esac
  printf "Setting display to 0x%04x, 0x%04x, 0x%04x, and 0x%04x (left to right)\n" $display1 $display2 $display3 $display4
  
  # writes to display window left
  tn_ll_mmi_write $C_BASE_ADDR_PERIPH $C_SUB_ADDR_PERIPH_DISPLAY0 $display1
  # writes to display window middle left
  tn_ll_mmi_write $C_BASE_ADDR_PERIPH $C_SUB_ADDR_PERIPH_DISPLAY1 $display2
  # writes to display window middle right
  tn_ll_mmi_write $C_BASE_ADDR_PERIPH $C_SUB_ADDR_PERIPH_DISPLAY2 $display3
  # writes to display window right
  tn_ll_mmi_write $C_BASE_ADDR_PERIPH $C_SUB_ADDR_PERIPH_DISPLAY3 $display4
  
  case $brightness in
    0)
      echo "Setting brightness to 0%"
      ;;
    1)
      echo "Setting brightness to 25%"
      ;;
    2)
      echo "Setting brightness to 50%"
      ;;
    3)
      echo "Setting brightness to 100%"
      ;;
    *)
      echo "$0: Wrong brightness value given ($brightness)."
      ;;
  esac

  # set brightness
  tn_ll_mmi_write $C_BASE_ADDR_PERIPH $C_SUB_ADDR_PERIPH_BRIGHTNESS $brightness

  echo "Done"
fi
