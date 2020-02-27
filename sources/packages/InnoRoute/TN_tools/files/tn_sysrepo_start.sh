#!/bin/bash
#restarting sysrepo server if firstboot has failed
echo "restarting sysrepo"
/etc/init.d/sysrepo stop
/etc/init.d/netopeer2-server stop
killall sysrepod
killall sysrepod
killall sysrepod
killall sysrepod
sleep 10
killall sysrepod
killall sysrepod
killall sysrepod
killall sysrepod
killall sysrepod
sysrepod 
sleep 5
killall netopeer2-server
killall netopeer2-server
killall netopeer2-server
killall netopeer2-server
killall netopeer2-server
sleep 10
killall netopeer2-server
killall netopeer2-server
killall netopeer2-server
killall netopeer2-server
killall netopeer2-server
netopeer2-server 
sleep 10
echo "reinstalling yangs"
for file in $(ls /etc/sysrepo/yang/*.yang)
do
sleep 1
sysrepoctl --install --yang=$file
done
/etc/init.d/netopeer2-server enable
/etc/init.d/sysrepo enable
sleep 5
sysrepoctl --feature-enable=scheduled-traffic --module=ieee802-dot1q-sched
/etc/init.d/sysrepo start
/etc/init.d/netopeer2-server start
