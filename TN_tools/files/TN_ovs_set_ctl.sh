#!/bin/bash

source /usr/share/InnoRoute/tn_env.sh
source /usr/share/InnoRoute/tn_func_ll.sh

if [[ $# -lt 1 || $# -gt 1 ]]; then
  echo "$0 <ip of of-controller>"
else
  ovs-vsctl set-controller TNbr tcp:$1:6633
  ovs-vsctl set-fail-mode TNbr secure
fi
