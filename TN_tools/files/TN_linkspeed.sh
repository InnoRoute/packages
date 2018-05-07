#!/bin/bash

source /usr/share/InnoRoute/tn_env.sh

echo -e "set FPGA linkspeed, parameter:linkspeed [0,1,2]\n\t0=10Mbit/s\n\t1=100MBit/s\n\t2=1000Mbit/s"
if [ -z "${C_SUB_ADDR_MAC_SPEED+xxx}" ]; then 
	echo "Setting linkspeed is not supported with this tn_env-setting." 
	exit 0
fi
if [[ $# != 1 ]]; then
  echo "Port 0:"
  TNbar1 $(($C_BASE_ADDR_MAC_0*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_MAC_SPEED)) w
  echo "Port 1:"
  TNbar1 $(($C_BASE_ADDR_MAC_1*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_MAC_SPEED)) w
  echo "Port 2:"
  TNbar1 $(($C_BASE_ADDR_MAC_2*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_MAC_SPEED)) w
  echo "Port 3:"
  TNbar1 $(($C_BASE_ADDR_MAC_3*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_MAC_SPEED)) w
  echo "Port 4:"
  TNbar1 $(($C_BASE_ADDR_MAC_4*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_MAC_SPEED)) w
  echo "Port 5:"
  TNbar1 $(($C_BASE_ADDR_MAC_5*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_MAC_SPEED)) w
  echo "Port 6:"
  TNbar1 $(($C_BASE_ADDR_MAC_6*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_MAC_SPEED)) w
  echo "Port 7:"
  TNbar1 $(($C_BASE_ADDR_MAC_7*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_MAC_SPEED)) w
  echo "Port 8:"
  TNbar1 $(($C_BASE_ADDR_MAC_8*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_MAC_SPEED)) w
  echo "Port 9:"
  TNbar1 $(($C_BASE_ADDR_MAC_9*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_MAC_SPEED)) w
  echo "Port 10:"
  TNbar1 $(($C_BASE_ADDR_MAC_10*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_MAC_SPEED)) w
  echo "Port 11:"
  TNbar1 $(($C_BASE_ADDR_MAC_11*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_MAC_SPEED)) w
  
else
  
  TNbar1 $(($C_BASE_ADDR_MAC_0*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_MAC_SPEED)) w $1 > /dev/null 
  TNbar1 $(($C_BASE_ADDR_MAC_1*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_MAC_SPEED)) w $1 > /dev/null
  TNbar1 $(($C_BASE_ADDR_MAC_2*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_MAC_SPEED)) w $1 > /dev/null
  TNbar1 $(($C_BASE_ADDR_MAC_3*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_MAC_SPEED)) w $1 > /dev/null
  TNbar1 $(($C_BASE_ADDR_MAC_4*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_MAC_SPEED)) w $1 > /dev/null
  TNbar1 $(($C_BASE_ADDR_MAC_5*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_MAC_SPEED)) w $1 > /dev/null
  TNbar1 $(($C_BASE_ADDR_MAC_6*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_MAC_SPEED)) w $1 > /dev/null
  TNbar1 $(($C_BASE_ADDR_MAC_7*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_MAC_SPEED)) w $1 > /dev/null
  TNbar1 $(($C_BASE_ADDR_MAC_8*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_MAC_SPEED)) w $1 > /dev/null
  TNbar1 $(($C_BASE_ADDR_MAC_9*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_MAC_SPEED)) w $1 > /dev/null
  TNbar1 $(($C_BASE_ADDR_MAC_10*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_MAC_SPEED)) w $1 > /dev/null
  TNbar1 $(($C_BASE_ADDR_MAC_11*$C_BASE_ADDR_FACTOR+$C_SUB_ADDR_MAC_SPEED)) w $1 > /dev/null
fi



