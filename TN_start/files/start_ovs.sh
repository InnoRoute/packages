#/bin/bash
rmmod ip_gre
rmmod gre
modprobe openvswitch
/etc/init.d/openvswitch stop
/etc/init.d/openvswitch start


