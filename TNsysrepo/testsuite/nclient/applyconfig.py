from ncclient import manager
import logging
import sys
host="192.168.1.100"
#host="127.0.0.1"

# create a main() method

def rolout_nc_config(xml_config,ipaddress):
	
	dev=manager.connect(
			host=ipaddress, 
			port=830,
			username="root",
			password="innoroot", 
			hostkey_verify=False,
			device_params={'name':'default'},
			)
	
	#print dev.get()
	print "moep"
	ret = dev.edit_config(target='running', config=xml_config.format("merge", 9, 99),default_operation='replace')
	dev.close_session()
	return ret

def main():
	global host
	with open(sys.argv[1], 'r') as myfile:
		netconf_data = myfile.read()
		myfile.close()
		rolout_nc_config(netconf_data,host)
	
if __name__ == '__main__':
    sys.exit(main())
