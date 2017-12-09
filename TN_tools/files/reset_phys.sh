#!/bin/bash

echo "Hard-resetting the Ethernet PHYs"

# Bit 0 for GPHYs, bit 1 for Alaskas -> combined value: 3
i2cset -y 0 4 0x19 3
sleep 1
i2cset -y 0 4 0x19 0
