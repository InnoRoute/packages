#!/bin/bash

source /usr/share/InnoRoute/tn_env.sh

echo "Setting board-specific RGMII input delays, expecting none or 12 parameters"

if [[ $# < 12 ]]; then
  let idelay1=11
  let idelay2=11
  let idelay3=11
  let idelay4=11
  let idelay5=11
  let idelay6=11
  let idelay7=11
  let idelay8=11
  let idelay9=11
  let idelay10=11
  let idelay11=11
  let idelay12=11
  echo "None or not enough parameters given: setting all IDELAY values to 11"
else
  let idelay1=${1}
  let idelay2=${2}
  let idelay3=${3}
  let idelay4=${4}
  let idelay5=${5}
  let idelay6=${6}
  let idelay7=${7}
  let idelay8=${8}
  let idelay9=${9}
  let idelay10=${10}
  let idelay11=${11}
  let idelay12=${12}
  printf "Setting the following IDELAYs: Port0=%d, Port1=%d, Port2=%d, Port3=%d, Port4=%d, Port5=%d, Port6=%d, Port7=%d, Port8=%d, Port9=%d, Port10=%d, Port11=%d\n" $idelay1 $idelay2 $idelay3 $idelay4 $idelay5 $idelay6 $idelay7 $idelay8 $idelay9 $idelay10 $idelay11 $idelay12
fi

# write data range: 0-31
TNbar1 $((C_BASE_ADDR_RGMII*256+ 0*0x100)) w $idelay1
TNbar1 $((C_BASE_ADDR_RGMII*256+ 1*0x100)) w $idelay2
TNbar1 $((C_BASE_ADDR_RGMII*256+ 2*0x100)) w $idelay3
TNbar1 $((C_BASE_ADDR_RGMII*256+ 3*0x100)) w $idelay4
TNbar1 $((C_BASE_ADDR_RGMII*256+ 4*0x100)) w $idelay5
TNbar1 $((C_BASE_ADDR_RGMII*256+ 5*0x100)) w $idelay6
TNbar1 $((C_BASE_ADDR_RGMII*256+ 6*0x100)) w $idelay7
TNbar1 $((C_BASE_ADDR_RGMII*256+ 7*0x100)) w $idelay8
TNbar1 $((C_BASE_ADDR_RGMII*256+ 8*0x100)) w $idelay9
TNbar1 $((C_BASE_ADDR_RGMII*256+ 9*0x100)) w $idelay10
TNbar1 $((C_BASE_ADDR_RGMII*256+10*0x100)) w $idelay11
TNbar1 $((C_BASE_ADDR_RGMII*256+11*0x100)) w $idelay12
