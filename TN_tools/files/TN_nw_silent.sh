#!/bin/bash

source /usr/share/InnoRoute/tn_env.sh
source /usr/share/InnoRoute/tn_func_ll.sh

if [[ $# == 0 ]]; then
  echo "$0 <ex> is used to silence the Linux, so it doesn't send out,"
  echo "e.g., discovey packets on its own"
  echo "The parameter <ex> can have any value. Without it, this help is displayed"
else

  # TODO: Move from ifconfig to ip. Initially remove all TN* and br0
  
  echo "Disabling all outgoing ARP requests for all currently active network interfaces"
  for i in `ifconfig -a | grep Link | cut -d " " -f 1`; do
    ifconfig $i -arp;
  done
  
  #echo "Stopping DHCP deamon"
  #/etc/init.d/dhcpcd stop

  echo "Done"
fi
