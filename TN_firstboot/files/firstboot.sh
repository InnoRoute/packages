#!/bin/bash
echo "Setting up Trustnode"
ls /root/.ssh/id_rsa && echo "ssh key already generated, skipping this... " || ssh-keygen -t rsa -b 4096 -f /root/.ssh/id_rsa -N '' &
/usr/share/InnoRoute/scripts/start_ovs.sh
ovs-vsctl del-br TNbr
ovs-vsctl add-br TNbr
#ovs counts from 1 nor from 0
ovs-vsctl add-port TNbr TN0 -- set Interface TN0 ofport_request=1
ovs-vsctl add-port TNbr TN1 -- set Interface TN1 ofport_request=2
ovs-vsctl add-port TNbr TN2 -- set Interface TN2 ofport_request=3
ovs-vsctl add-port TNbr TN3 -- set Interface TN3 ofport_request=4
ovs-vsctl add-port TNbr TN4 -- set Interface TN4 ofport_request=5
ovs-vsctl add-port TNbr TN5 -- set Interface TN5 ofport_request=6
ovs-vsctl add-port TNbr TN6 -- set Interface TN6 ofport_request=7
ovs-vsctl add-port TNbr TN7 -- set Interface TN7 ofport_request=8
ovs-vsctl add-port TNbr TN8 -- set Interface TN8 ofport_request=9
ovs-vsctl add-port TNbr TN9 -- set Interface TN9 ofport_request=10
ovs-vsctl add-port TNbr TN10 -- set Interface TN10 ofport_request=11
ovs-vsctl add-port TNbr TN11 -- set Interface TN11 ofport_request=12
ovs-vsctl add-port TNbr TN12 -- set Interface TN12 ofport_request=13
ovs-vsctl add-port TNbr TN13 -- set Interface TN13 ofport_request=14
ovs-vsctl add-port TNbr TN14 -- set Interface TN14 ofport_request=15
ovs-vsctl add-port TNbr TN15 -- set Interface TN15 ofport_request=16
INRpllload /usr/share/InnoRoute/TN-PLL-V1.2_synce_7.stp E
sensors-detect --auto
INR_ftdi_eeprom -p 0x6010 -v 0x0403 -M
chown -R TNuser /home/TNuser/
for file in $(ls /usr/share/InnoRoute/TN_*.sh)
do
ln -s $file /usr/sbin/
done
/usr/share/InnoRoute/TN_phy_init.sh
/usr/share/InnoRoute/TN_beep.sh
sleep 1
/usr/share/InnoRoute/TN_beep.sh
sleep 1
/usr/share/InnoRoute/TN_beep.sh
wait
echo "Setup on firstboot done"
echo "Have fun with your TrustNode"
