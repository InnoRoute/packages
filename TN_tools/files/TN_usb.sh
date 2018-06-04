#!/bin/bash

if [[ $# == 0 ]]; then
  echo "$0 <ex> is used to return information on all connected USB devices"
  echo "The parameter <ex> can have any value. Without it, this help is displayed"
else
  echo "USB host ports:"
  lsusb -s 1:2
  lsusb -s 2:2
  lsusb -s 1:4
  echo "Done"
fi
