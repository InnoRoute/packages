#!/bin/bash

source /usr/share/InnoRoute/tn_env.sh

if [[ $# == 0 ]]; then
  echo "Muting TrustNode"
  i2cset -y 0 0x04 0x03 0x10
else
  if [[ $1 == 1 ]]; then
    echo "Muting TrustNode"
    i2cset -y 0 0x04 0x03 0x10
  else
    echo "Unmuting TrustNode"
    i2cset -y 0 0x04 0x03 0x00
  fi
fi
