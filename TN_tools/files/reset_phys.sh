#!/bin/bash

echo "Hard-resetting the Ethernet PHYs"

i2cset -y 0 4 0x19 3
sleep 1
i2cset -y 0 4 0x19 0
