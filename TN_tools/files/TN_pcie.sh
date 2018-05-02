#!/bin/bash

# Enable FPGA BARs
setpci -s `lspci -n -d 10ee:* | cut -d " " -f 1` COMMAND=0x02

echo "Rescanning PCIe"
echo 1 > /sys/bus/pci/rescan #This might break the Linux driver

echo "PCIe status"
lspci -nn -vvv -d 10ee:*
