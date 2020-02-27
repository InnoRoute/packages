#!/bin/bash

source /usr/share/InnoRoute/tn_env.sh
source /usr/share/InnoRoute/tn_func_ll.sh

  ovs-vsctl del-controller TNbr 
  ovs-vsctl set-fail-mode TNbr standalone
