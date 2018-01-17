#!/bin/bash

echo "Rescanning PCIe"
echo 1 > /sys/bus/pci/rescan #This might break the Linux driver

echo "PCIe status"
lspci -nn -vvv
