#!/usr/bin/python

import sys
import re
import subprocess
import datetime
import os
import subprocess

if len(sys.argv) != 2:
    print "bridge name needed"
    sys.exit()

br_name = sys.argv[1]

flow_list = list()
flow_list_old = list()

def read_flows():
    del flow_list[:]
    lines = subprocess.check_output(['sudo', 'ovs-ofctl', 'dump-flows', br_name])
    for line in lines.splitlines():
        if not re.findall(r" (actions=)", line):
            continue
        a,b=line.split('actions=', 1 )
        match_map = dict()
        action_map = dict()

        ### parse and make match_map ###
        #
        a=a.split(',')      
        for tmp in a:
            if re.findall("=",tmp):
                x,y=tmp.split('=')
                x = re.sub(r"\s+",'', x)
                y = re.sub(r"\s+",'', y)
            else:
                x="proto"
                y=tmp
            if not x in ['idle_age','duration','n_packets','n_bytes']:
                match_map[x]=y

        ### parse and make action_map ###
        #
        b=re.compile(r"[,](?=[a-z])").split(b)
        for tmp in b:
            if re.findall(":",tmp):
                x,y=tmp.split(':')
                action_map[x]=y
            elif re.findall(r"\(\d*\,\d*\)",tmp):
                x=re.findall(r"(.*)(\(\d*\,\d*\))",tmp)
                action_map[x[0][0]]= x[0][1]
            else:
                action_map[tmp]=1

        ### make dict for flow and appand to flow_list 
        #        
        flow = dict()
        flow['match']=match_map
        flow['action']=action_map

        ### flow_list = list of dict of dict
        #  
        flow_list.append(flow)


def print_flows():
    ### print out flow_list ###
    #
    print "\n#######################################################################################################\n"
    for tmp in flow_list:
        print_flow(tmp)

def run(command):
		command=command + ">lastlog"
		if os.system(command) == 0:
			with open('lastlog', 'r') as lastlog:
				return lastlog.read()
		else:
			with open('lastlog', 'r') as lastlog:
				return lastlog.read()        

def print_flow(tmp):
    ### print out flow ###
    #
    print "--------------------------------------------"
    print "MATCH:"
    for p in tmp['match']:
        print "     ",p,":",tmp['match'][p]
    print "\n"
    print "ACTION:"
    for p in tmp['action']:
        print "     ",p,":", tmp['action'][p]
    print "--------------------------------------------"

def TNflowcache(tmp,act):
	command=""
	for p in tmp['match']:
		if p == "dl_src" :
			command=command+" -S"+tmp['match'][p]
		if p == "dl_dst" :
			command=command+" -D"+tmp['match'][p]
		if p == "dl_type" :
			command=command+" -E"+tmp['match'][p]
		if p == "dl_vlan" :
			command=command+" -V"+tmp['match'][p]
		if p == "vlan_pcp" :
			command=command+" -U"+tmp['match'][p]
		if p == "nw_src" :
			command=command+" -C"+tmp['match'][p]
		if p == "nw_dst" :
			command=command+" -T"+tmp['match'][p]
		if p == "nw_tos" :
			command=command+" -W"+tmp['match'][p]
		if p == "in_port" :
			command=command+" -I"+str(int(tmp['match'][p])-1)
		if p == "tp_src" :
			command=command+" -R"+tmp['match'][p]
		if p == "tp_dst" :
			command=command+" -A"+tmp['match'][p]
		if p == "priority" :
			command=command+" -p"+tmp['match'][p]
#		if p == "proto" :
#			command=command+" -P"+tmp['match'][p]
#deactivated, have to fix nameconversatzion of OVS, need numbers no strings

	for p in tmp['action']:
		if p == "output" :
			command=command+" -X1 -p5 -O"+str(int(tmp['action'][p])-1) #ovs counts from 1 we from 0
			command="TNflowtable "+act+" "+command
#			command='echo "' +command +'"'  #testing mode
			log= run(command)#set entry in fpga flowtable
			return log
	return "unknown action"
		
	

def check_flows():
    ### check flow_list for added or removed flows ###
    #
    for a in flow_list:
        match=0
        for b in flow_list_old:
            if a==b:
                match=1
                break 
        if not match:
#            print "\nFLOW ADDED [", datetime.datetime.now(),"]"
#            print_flow(a)
	    print TNflowcache(a,"add")
            
    for a in flow_list_old:
        match=0
        for b in flow_list:
            if a==b:
                match=1
                break 
        if not match:
#            print "\nFLOW REMOVED [", datetime.datetime.now(),"]"
#            print_flow(a)
	    print TNflowcache(a,"del")
   

while 1:
    flow_list_old = list(flow_list)
    read_flows()
    check_flows()
    #print "=================================================="
    #raw_input()











### NOTES
'''
Building a flow

A flow is always built in two parts, the match and the actions :

 ovs-ofctl add-flow s0 "table=X,priority=X,<match part>,actions=<actions part>"
 
you can have several matches and several actions at once, comma separated. Priority starts from 0 up to 65535. Flows with Higher priority will match instead of flows with lower priority. If you don't specify a priority, the default value is 32768.

Matching a value :

 <match field>=<value>
 
adding an action :

 <action>=<value>
 
exceptions :

 resubmit(in_port,table)
 output:X,Y,Z
 
A full command would then be :

 ovs-ofctl add-flow s0 "table=2,priority=1,nw_src=192.168.200.0/24,actions=mod_nw_src=10.0.0.1,output:3"
Matching pattern

in_port
dl_vlan
dl_src
dl_dst
dl_type
nw_src
nw_dst
nw_proto
nw_ttl
tp_src
tp_dst
icmp_type
icmp_code
ip
icmp
arp
tcp
udp
arp_sha
arp_spa
arp_tha
arp_tpa
ipv6_dst
ipv6_src
ipv6
icmp6
actions

output
normal
flood
resubmit
goto_table(prefer using resubmit, as goto_table is supported only by OpenFlow 1.3, so it is rejected unless you specify OpenFlow version when calling ovs-ofctl)
all
controller
local
in_port
drop
mod_dl_src
mod_dl_dst
mod_nw_src
mod_nw_dst
mod_tp_src
mod_tp_dst
dec_ttl
load
set_field
move
learn

'''
