#!/bin/bash

source /usr/share/InnoRoute/tn_env.sh
source /usr/share/InnoRoute/tn_func_ll.sh

if [[ $# == 0 ]]; then
  echo "$0 <ex> [<stat>] is used to enable bridging between all virtual TrustNode ports"
  echo "The parameter <ex> can have any value. Without it, this help is displayed"
  echo "If the parameter <stat> is given, traffic statistics are shown after enabling"
  echo "the bridge. <stat> can have any value."
else

  # Name of the bridge port
  export bridge=TNbr

  # TODO: Move from ifconfig to ip. Initially remove all TN* and $bridge
  # TODO: Check, if interfaces already exist
  # TODO: Make bwm-ng call conditional
  
  echo "Adding $bridge to loop connect all virtual TN ports ..."
  
  #insmod trustnode.ko
  ifconfig TN0 up
  ifconfig TN1 up
  ifconfig TN2 up
  ifconfig TN3 up
  ifconfig TN4 up
  ifconfig TN5 up
  ifconfig TN6 up
  ifconfig TN7 up
  ifconfig TN8 up
  ifconfig TN9 up
  ifconfig TN10 up
  ifconfig TN11 up
  ifconfig TN12 up
  ifconfig TN13 up
  ifconfig TN14 up
  ifconfig TN15 up
  
  brctl addbr $bridge
  
  brctl addif $bridge TN0
  brctl addif $bridge TN1
  brctl addif $bridge TN2
  brctl addif $bridge TN3
  brctl addif $bridge TN4
  brctl addif $bridge TN5
  brctl addif $bridge TN6
  brctl addif $bridge TN7
  brctl addif $bridge TN8
  brctl addif $bridge TN9
  brctl addif $bridge TN10
  brctl addif $bridge TN11
  brctl addif $bridge TN12
  brctl addif $bridge TN13
  brctl addif $bridge TN14
  brctl addif $bridge TN15
  
  ifconfig $bridge up
  
  #ifconfig -a
  #tcpdump -i TN0
  #tcpdump -i TN1
  
  echo "Done"

  if [[ $# > 1 ]]; then
    echo "Starting bwm-ng: press enter"
    read ack
    bwm-ng -T sum -u packets
  fi
fi
