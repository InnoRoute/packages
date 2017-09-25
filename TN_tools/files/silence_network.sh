#!/bin/bash

# TODO: Move from ifconfig to ip. Initially remove all TN* and br0

echo "Disabling all outgoing ARP requests for all currently active network interfaces"
for i in `ifconfig -a | grep Link | cut -d " " -f 1`; do
  ifconfig $i -arp;
done

#echo "Stopping DHCP deamon"
#/etc/init.d/dhcpcd stop
