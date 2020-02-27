#!/bin/bash

echo "Setting up Trustnode"

# SSH key per device
if test -e /root/.ssh/id_rsa; then
	echo "ssh key already generated, skipping this... "
else
	ssh-keygen -t rsa -b 4096 -f /root/.ssh/id_rsa -N '' &
fi

# Add key for InnoRoute package repository
opkg-key add /usr/share/InnoRoute/key-build.pub

# Open vSwitch configuration - requires working FPGA configuration providing network interfaces
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
macaddr=$(dd if=/dev/urandom bs=1024 count=1 2>/dev/null|md5sum|sed 's/^\(..\)\(..\)\(..\)\(..\)\(..\)\(..\).*$/\1:\2:\3:\4:\5:\6/')
ovs-vsctl set bridge TNbr other-config:hwaddr=$macaddr
ovs-vsctl set bridge TNbr rstp_enable=false

# Configure PLL
INRpllload /usr/share/InnoRoute/tn-pll-v1.2_synce_7.stp E

# lmsensors initialization
#sensors-detect --auto #TODO To be fixed

# Configure FPGA configuration bridge
INR_ftdi_eeprom -p 0x6010 -v 0x0403 -M

# Add TN_* scripts to path
chown -R TNuser /home/TNuser/
for file in $(ls /usr/share/InnoRoute/TN_*.sh); do
	ln -s $file /usr/sbin/
done
chmod +x /usr/share/InnoRoute/FlowCache_persistent.sh


# Start NETCONF daemon and install YANG files
for file in $(ls /etc/sysrepo/yang/*.yang); do
	sysrepoctl --install --yang=$file
done

# Start PHYs
/usr/share/InnoRoute/TN_phy_init.sh 1

# Triple Beep
/usr/share/InnoRoute/TN_beep.sh 1
sleep 1
/usr/share/InnoRoute/TN_beep.sh 1
sleep 1
/usr/share/InnoRoute/TN_beep.sh 1

# Copy current bitstream to /boot (Windows partition)
mount /dev/sda1 /boot/ 2>/dev/null
sleep 2
cp /usr/share/InnoRoute/bs_unstable.bit /boot/trustnode_top.bit || true

# Copy documentation from /boot (Windows partition) to web server
mkdir -p /www/luci-static/innoroute/doku
cp /boot/*.pdf /www/luci-static/innoroute/doku/
tar -cf /www/luci-static/innoroute/doku/yang.tar /etc/sysrepo/yang
chmod o+r /www/luci-static/innoroute/doku/yangmodel.html

# Wait for state change
wait

# Regenerate SSH key
rm /etc/keystored/keys/*
ssh-keygen -m pem -t rsa -q -N "" -f /etc/keystored/keys/ssh_host_rsa_key.pem


# Enable SYSREPO TAS
#sysrepoctl --feature-enable=scheduled-traffic --module=ieee802-dot1q-sched
# Start usual start scripts, again
/etc/init.d/TN_start enable

/etc/init.d/TN_imageloader enable
/etc/init.d/TN_iloadhelp enable
/etc/init.d/TN_firstboot disable

echo "Setup on firstboot done"
echo "Have fun with your TrustNode"

#/etc/init.d/TN_imageloader start
wait
