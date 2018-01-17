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
TNbar1 $((($C_BASE_ADDR_RGMII+ 0)*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_RGMII_TAP)) w $idelay1 > /dev/null
TNbar1 $((($C_BASE_ADDR_RGMII+ 1)*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_RGMII_TAP)) w $idelay2 > /dev/null
TNbar1 $((($C_BASE_ADDR_RGMII+ 2)*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_RGMII_TAP)) w $idelay3 > /dev/null
TNbar1 $((($C_BASE_ADDR_RGMII+ 3)*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_RGMII_TAP)) w $idelay4 > /dev/null
TNbar1 $((($C_BASE_ADDR_RGMII+ 4)*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_RGMII_TAP)) w $idelay5 > /dev/null
TNbar1 $((($C_BASE_ADDR_RGMII+ 5)*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_RGMII_TAP)) w $idelay6 > /dev/null
TNbar1 $((($C_BASE_ADDR_RGMII+ 6)*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_RGMII_TAP)) w $idelay7 > /dev/null
TNbar1 $((($C_BASE_ADDR_RGMII+ 7)*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_RGMII_TAP)) w $idelay8 > /dev/null
TNbar1 $((($C_BASE_ADDR_RGMII+ 8)*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_RGMII_TAP)) w $idelay9 > /dev/null
TNbar1 $((($C_BASE_ADDR_RGMII+ 9)*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_RGMII_TAP)) w $idelay10 > /dev/null
TNbar1 $((($C_BASE_ADDR_RGMII+10)*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_RGMII_TAP)) w $idelay11 > /dev/null
TNbar1 $((($C_BASE_ADDR_RGMII+11)*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_RGMII_TAP)) w $idelay12 > /dev/null
