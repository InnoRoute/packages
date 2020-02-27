#!/bin/bash
# Get Bridge info
ovs-vsctl get Bridge TNbr rstp_status
# Get Per-Port info
for port in TN0 TN1 TN2 TN3 TN4 TN5 TN6 TN7 TN8 TN9 TN10 TN11 ; do
  echo "==================================="
  echo "PORT $port :"
  echo ""
  ovs-vsctl get Port $port rstp_status
  ovs-vsctl get Port $port rstp_statistics
done
