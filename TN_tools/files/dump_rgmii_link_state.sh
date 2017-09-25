#!/bin/bash

source /usr/share/InnoRoute/tn_env.sh

echo "Reading link state from all RGMIIs"

# Result: Bit0=LinkUp, Bit4=FullDuplex, Bits8-9=Speed (0=10Mbps, 1=100Mbps, 2=1Gbps) -> Expected 0x211 (1Gbps, full duplex, LinkUp)
# Unconnected PHYs deliver default values: GPHY->0x301 (illegal speed value, half duplex, LinkUp) and Alaska->0x200 (1Gbps, half duplex (illegal for 1Gbps), LinkUp)
# -> The speed-duplex-combination shows, if a cable is connected

TNbar1 $(($TN_BASE_ADDR_RGMII+ 0*0x100+4))
TNbar1 $(($TN_BASE_ADDR_RGMII+ 1*0x100+4))
TNbar1 $(($TN_BASE_ADDR_RGMII+ 2*0x100+4))
TNbar1 $(($TN_BASE_ADDR_RGMII+ 3*0x100+4))
TNbar1 $(($TN_BASE_ADDR_RGMII+ 4*0x100+4))
TNbar1 $(($TN_BASE_ADDR_RGMII+ 5*0x100+4))
TNbar1 $(($TN_BASE_ADDR_RGMII+ 6*0x100+4))
TNbar1 $(($TN_BASE_ADDR_RGMII+ 7*0x100+4))
TNbar1 $(($TN_BASE_ADDR_RGMII+ 8*0x100+4))
TNbar1 $(($TN_BASE_ADDR_RGMII+ 9*0x100+4))

TNbar1 $(($TN_BASE_ADDR_RGMII+10*0x100+4))
TNbar1 $(($TN_BASE_ADDR_RGMII+11*0x100+4))
