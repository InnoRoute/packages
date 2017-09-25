#!/bin/bash
echo "Setting up Trustnode"
ls /root/.ssh/id_rsa && echo "ssh key already generated, skipping this... " || ssh-keygen -t rsa -b 4096 -f /root/.ssh/id_rsa -N '' &
INRpllload /usr/share/InnoRoute/TN-PLL.stp E
sensors-detect --auto
INR_ftdi_eeprom -p 0x6010 -v 0x0403 -M
wait
echo "Setup on firstboot done"
echo "Have fun with your TrustNode"
