#!/bin/bash
echo "#!/bin/bash" > /usr/share/openvswitch/scripts/ovs-kmod-ctl
chmod +x /usr/share/openvswitch/scripts/ovs-kmod-ctl
ovs-ctl start
#rmmod ip_gre
#rmmod gre
#modprobe openvswitch
#/etc/init.d/openvswitch stop
#/usr/share/openvswitch/scripts/ovs-ctl stop
#/etc/init.d/openvswitch start
#/usr/share/openvswitch/scripts/ovs-ctl start


