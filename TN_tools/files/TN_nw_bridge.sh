#!/bin/bash

# TODO: Move from ifconfig to ip. Initially remove all TN* and br0
# TODO: Check, if interfaces already exist
# TODO: Make bwm-ng call conditional

echo "Adding br0 to loop connect all virtual TN ports ..."

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

brctl addbr br0

brctl addif br0 TN0
brctl addif br0 TN1
brctl addif br0 TN2
brctl addif br0 TN3
brctl addif br0 TN4
brctl addif br0 TN5
brctl addif br0 TN6
brctl addif br0 TN7
brctl addif br0 TN8
brctl addif br0 TN9
brctl addif br0 TN10
brctl addif br0 TN11
brctl addif br0 TN12
brctl addif br0 TN13
brctl addif br0 TN14
brctl addif br0 TN15

ifconfig br0 up

#ifconfig -a
#tcpdump -i TN0
#tcpdump -i TN1

if [[ $# == 0 ]]; then
  bwm-ng -T sum -u packets
fi
