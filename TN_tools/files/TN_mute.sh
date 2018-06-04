#!/bin/bash

source /usr/share/InnoRoute/tn_env.sh
source /usr/share/InnoRoute/tn_func_ll.sh

if [[ $# == 0 ]]; then
  echo "$0 <mute> is used to mute or unmute the TrustNode"
  echo "Muting suppresses all sounds, even warnings, so handle with care"
  echo "Parameter <mute> can be 0 or 1"
  echo "  0: unmute (default value)"
  echo "  1: mute"
else
  if [[ $1 == 1 ]]; then
    echo "Muting TrustNode"
    i2cset -y 0 0x04 0x03 0x10
  else
    echo "Unmuting TrustNode"
    i2cset -y 0 0x04 0x03 0x00
  fi

  echo "Done"
fi
