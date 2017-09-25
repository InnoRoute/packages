#!/bin/bash

source /usr/share/InnoRoute/tn_env.sh

echo "Setting front panel display, expecting none, one, or four, or 16 parameters"

case $# in
  0|1|2|3)
    echo "Four-parameter mode: "
    echo " "
    echo "Segment Coding (one byte per display)"
    echo "            "
    echo "   00000    "
    echo "  5     1   "
    echo "  5     1   "
    echo "  5     1   "
    echo "   66666    "
    echo "  4     2   "
    echo "  4     2   "
    echo "  4     2   "
    echo "   33333  77"
    echo " "
    echo "The displays are counted per window, from left to right, with displays"
    echo "within the windows also from left to right:"
    echo " -> 1-2-3-4  5-6-7-8  9-10-11-12  13-14-15-16"
    echo "and the parameters given are all 32 bit words, with the least significant byte"
    echo "on the left of each window and the most significant byte on the right of each"
    echo "window, i.e., set_display.sh 0 0 0 0x01000000 would illuminate just the"
    echo "leftmost display's top segment:"
    echo " -> set_display.sh 0x04030201 0x08070605 0x12111009 0x16151413"
    echo " "

    # Set all segments to on, if no parameter or an insufficient number of parameters is given
    let display1=0xFFFFFFFF
    let display2=0xFFFFFFFF
    let display3=0xFFFFFFFF
    let display4=0xFFFFFFFF
    ;;
  4)
    # If parameters are given, it is assumed that they are the display values
    let display1=$1
    let display2=$2
    let display3=$3
    let display4=$4
    ;;
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
    let display1=$digit4+$digit3*0x100+$digit2*0x10000+$digit1*0x1000000
    let display2=$digit8+$digit7*0x100+$digit6*0x10000+$digit5*0x1000000
    let display3=$digit12+$digit11*0x100+$digit10*0x10000+$digit9*0x1000000
    let display4=$digit16+$digit15*0x100+$digit14*0x10000+$digit13*0x1000000
    ;;
  *)
    echo "Sixteen-parameter mode: "
    echo " "
    echo "Segment Coding (one byte per display)"
    echo "            "
    echo "   00000    "
    echo "  5     1   "
    echo "  5     1   "
    echo "  5     1   "
    echo "   66666    "
    echo "  4     2   "
    echo "  4     2   "
    echo "  4     2   "
    echo "   33333  77"
    echo " "
    echo "The displays are left to right, from first to sixteenth parameter:"
    echo " -> 1-2-3-4  5-6-7-8  9-10-11-12  13-14-15-16"
    echo " -> set_display.sh 0x01 0x02 0x03 0x04 0x05 0x06 0x07 0x08 0x09 0x10 0x11 0x12 0x13 0x14 0x15 0x16"
    echo " "

    # Set all segments to on, if no parameter or an insufficient number of parameters is given
    let display1=0xFFFFFFFF
    let display2=0xFFFFFFFF
    let display3=0xFFFFFFFF
    let display4=0xFFFFFFFF
    ;;
esac
printf "Setting display to 0x%04x, 0x%04x, 0x%04x, and 0x%04x (left to right)\n" $display1 $display2 $display3 $display4

# writes to display window left
TNbar1 $(($TN_BASE_ADDR_PERIPHERIALS+0x10)) w $display1

# writes to display window middle left
TNbar1 $(($TN_BASE_ADDR_PERIPHERIALS+0x14)) w $display2

# writes to display window middle right
TNbar1 $(($TN_BASE_ADDR_PERIPHERIALS+0x18)) w $display3

# writes to display window right
TNbar1 $(($TN_BASE_ADDR_PERIPHERIALS+0x1C)) w $display4
