/**
*@file tnlibstatistics.c
*@brief userspace statistics access tool (enormous speed-up compared to multiple TNbar1 calls)
*M.Ulbricht 2018
**/

#include <inttypes.h>
#include <stddef.h>
#include <stdio.h>
#include <endian.h>
#include <time.h>
#include <unistd.h>

#include "tnlibstatistics.h"
uint8_t verbose = 0;
uint64_t BASE = 0;
uint64_t BASE_shadow = 0;
uint8_t MEMDUMP = 0;
int delay=100000;

void set_sleeptime(uint64_t zeit)
{
	delay=zeit;
}
//*****************************************************************************************************************
/**
*read from mmi addr
*
*/
uint32_t INR_PCI_MMI_read(uint64_t addr) {
		uint32_t *value;
		value=BASE+addr;
    return *value;
}

//*****************************************************************************************************************
/**
*read from mmi addr
*
*/
void INR_PCI_MMI_write(uint32_t val, uint64_t addr) {
    uint32_t *value;
		value=BASE+addr;
    *value=val;
}

//*****************************************************************************************************************
/**
*write data to mdio (baremetal)
*@param PHY_addr address of ethernet PHY
*@param REG_addr address of register
*@param data data to write
*/
void INR_MDIO_write_b(uint8_t write, uint8_t PHY_addr, uint8_t REG_addr, uint16_t data) {
    uint32_t value=0,i=0;
    value=((1&write)<<26)|((PHY_addr&0x1f)<<21)|((REG_addr&0x1f)<<16)|(data);
    INR_PCI_MMI_write(value,INR_MDIO_write_addr);
    usleep(50);
    while(INR_MDIO_read_b(NULL)) { //wait until ready
        i++;
        if(i>INR_MDIO_timeout)break;


    }
}
//*****************************************************************************************************************
/**
*write data to mdio (baremetal)
*@param PHY_addr address of ethernet PHY
*@param REG_addr address of register
*@param data data to write
*/
void INR_MDIO_write(uint8_t PHY_addr, uint8_t REG_addr, uint16_t data) {
    INR_MDIO_write_b(1,PHY_addr,REG_addr,data);
}

//*****************************************************************************************************************
/**
*read data from mdio (baremtal))
*@param PHY_addr address of ethernet PHY
*@param REG_addr address of register
*@param *data address of data to write
*@return busy bit
*/
uint8_t INR_MDIO_read_b(uint16_t *data) {
    uint8_t busy=0;
    uint32_t value=0;
    //uint16_t *rd_data=data;
    value=INR_PCI_MMI_read(INR_MDIO_read_addr);
    usleep(50);
    if (value==0xeeeeeeee){printf("MMI read timeout\n");return 0;}
    busy=1&(value>>31);
    if(data)*data=value&0xffff;//check if nullpointer
    //printk("value=0x%x\n",value);
    return busy;
}

//*****************************************************************************************************************
/**
*write data to mdio
*@param PHY_addr address of ethernet PHY
*@param REG_addr address of register
*@param data data to write
*/
uint16_t INR_MDIO_read(uint8_t PHY_addr, uint8_t REG_addr) {
    uint16_t data;
    INR_MDIO_write_b(0,PHY_addr,REG_addr,0);//send read request
    INR_MDIO_read_b(&data);//read value
    //printk("value=0x%x\n",data);
    return data;
}
//********************************************************************************************************************
/**
*set default values to arguments structure
@param arguments argumentes from userinterface
*/
void
clear_arguments (struct arguments *arguments)
{
	arguments->args[0] = "";
	arguments->verbose = 0;
	arguments->used = 0;
	arguments->numberout = 0;
	arguments->machinereadable = 0;
	arguments->ID = 0;
	arguments->COUNT = 0;
	arguments->dohave_ID = 0;
	arguments->dohave_COUNT = 0;
}

//************************************************************************************************************************************
/**
*set verbode
*@param verbose value
*/
void
printallconst ()
{
#ifndef __KERNEL__		//not allowed for kmods
	printf ("Compiled %s %s\n", __DATE__, __TIME__);
#endif
	//printconst (GateControl_list_length);
};

//************************************************************************************************************************************
/**
*replacement of memcopy to guarantee wordwise memory access
*
*/
void
TNmemcpy (void *dst, const void *src, size_t len)
{
	size_t i;
	uint32_t *d = dst;
	uint32_t *s = src;

	for (i = 0; i < len / sizeof (uint32_t); i++)
	{
		d[i] = s[i];
#ifndef __KERNEL__
		if (MEMDUMP)
		{
			printf ("TNbar1 0x%llx w 0x%llx\n", dst - BASE + i * sizeof (uint32_t), s[i]);
		}
#endif
    }
	return dst;
}

//************************************************************************************************************************************
/**
*get verbode
*/
uint8_t
get_verbose ()
{
	return verbose;
};

//************************************************************************************************************************************
/**
*get verbode
*/
void
STAT_init (uint64_t * base, uint64_t * shadow_base)
{
	BASE = (uint64_t) base;
	BASE_shadow = (uint64_t) shadow_base;
	verblog printf ("Base address bar1:0x%llx, base address_shadow:0x%llx\n", BASE, BASE_shadow);
};

//************************************************************************************************************************************
/**
*set verbode
*@param verbose value
*/
void
set_verbose (uint8_t i)
{
	verbose = i;
};

//************************************************************************************************************************************

STAT_print_phy_counters(struct arguments *arguments)
{
	uint16_t write_data;
	uint8_t REG_addr=0x15;
	uint8_t data;
	int total=0;
	not_MACHINEREADABLE printf("\n### GPHY: RX Packet Counters (saturating at 255, non-zero only)");
	MACHINEREADABLE printf("{");	
	write_data=0x0100;
	for (int phy=16; phy<26; phy++)
	{
		INR_MDIO_write(phy, REG_addr, write_data);
		//usleep(1000);
	}
	usleep(delay);
	for (int phy=16; phy<26; phy++)
	{	
		data=INR_MDIO_read(phy, REG_addr);
		//usleep(1000);
		if (data != 0)
		{
			not_MACHINEREADABLE printf("PHY %i: %i packets received\n", phy, data);
			total=total+data;
		}
		MACHINEREADABLE printf("\"rxpacketsphy%i\":%i,", phy, data);
	}
	not_MACHINEREADABLE printf("\nTotal RX: %i packets\n\n", total);
	MACHINEREADABLE printf("\"totalrx\":%i,", total);
	
	not_MACHINEREADABLE printf("\n### GPHY: RX Error Counters (saturating at 255, non-zero only)");
	write_data=0x000;
	total=0;
	for (int phy=16; phy<26; phy++)
	{
		INR_MDIO_write(phy, REG_addr, write_data);
		//usleep(1000);
	}
	usleep(delay);
	for (int phy=16; phy<26; phy++)
	{
		data=INR_MDIO_read(phy, REG_addr);
		//usleep(1000);
		if (data != 0)
		{
			not_MACHINEREADABLE printf ("PHY %i: %i errors received\n", phy, data);
			total=total+data;
		}
		MACHINEREADABLE printf("\"rxerrorphy%i\":%i,", phy, data);
	}
	not_MACHINEREADABLE printf("\nTotal RX Errors: %i packets\n\n", total);
	MACHINEREADABLE printf("\"totalrxerror\":%i,", total);
	
	not_MACHINEREADABLE printf("\n### GPHY: RX ESD Error Counters (saturating at 255, non-zero only)");
	write_data=0x0200;
	total=0;
	for (int phy=16; phy<26; phy++)
	{
		INR_MDIO_write(phy, REG_addr, write_data);
		//usleep(1000);
		
	}
	usleep(delay);
	for (int phy=16; phy<26; phy++)
	{
		data=INR_MDIO_read(phy, REG_addr);
		//usleep(1000);
		if (data != 0)
		{
			not_MACHINEREADABLE printf ("PHY %i: %i ESD errors received\n", phy, data);
			total=total+data;
		}
		MACHINEREADABLE printf("\"rxerroresdphy%i\":%i,", phy, data);
	}
	not_MACHINEREADABLE printf("\nTotal RX ESD Errors: %i packets\n\n", total);
	MACHINEREADABLE printf("\"totalrxerroresd\":%i,", total);
	
	not_MACHINEREADABLE printf("\n### GPHY: RX SSD Error Counters (saturating at 255, non-zero only)");
	write_data=0x0300;
	total=0;
	for (int phy=16; phy<26; phy++)
	{
		INR_MDIO_write(phy, REG_addr, write_data);
		//usleep(1000);
	}
	usleep(delay);
	for (int phy=16; phy<26; phy++)
	{
		data=INR_MDIO_read(phy, REG_addr);
		//usleep(1000);
		if (data != 0)
		{
			not_MACHINEREADABLE printf ("PHY %i: %i SSD errors received\n", phy, data);
			total=total+data;
		}
		MACHINEREADABLE printf("\"rxerrorssdphy%i\":%i,", phy, data);
	}
	not_MACHINEREADABLE printf("\nTotal RX SSD Errors: %i packets\n\n", total);
	MACHINEREADABLE printf("\"totalrxerrorssd\":%i,", total);
	
	not_MACHINEREADABLE printf("\n### GPHY: TX Packet Counters (saturating at 255, non-zero only)");
	write_data=0x0500;
	total=0;
	for (int phy=16; phy<26; phy++)
	{
		INR_MDIO_write(phy, REG_addr, write_data);
		//usleep(1000);
	}
	usleep(delay);
	for (int phy=16; phy<26; phy++)
	{
		data=INR_MDIO_read(phy, REG_addr);
		//usleep(1000);
		if (data != 0)
		{
			not_MACHINEREADABLE printf ("PHY %i: %i packets transmitted\n", phy, data);
			total=total+data;
		}
		MACHINEREADABLE printf("\"txpacketsphy%i\":%i,", phy, data);
	}
	not_MACHINEREADABLE printf("\nTotal TX: %i packets\n\n", total);
	MACHINEREADABLE printf("\"totaltx\":%i,", total);	
	
	not_MACHINEREADABLE printf("\n### GPHY: TX Error Counters (saturating at 255, non-zero only)");
	
	write_data=0x0400;
	for (int phy=16; phy <26; phy++)
	{;
		INR_MDIO_write(phy, REG_addr, write_data);
		//usleep(1000);
	}
	usleep(delay);
	total=0;
	for (int phy=16; phy<26; phy++)
	{
		data=INR_MDIO_read(phy, REG_addr);
		//usleep(1000);
		if (data != 0)
		{
			not_MACHINEREADABLE printf("PHY %i: %i packets with errors transmitted\n", phy, data);
			total=total+data;
		}
		MACHINEREADABLE printf("\"txerrorphy%i\":%i,", phy, data);
	}
	not_MACHINEREADABLE printf("\nTotal TX Errors: %i packets\n\n", total);
	MACHINEREADABLE printf("\"totaltxerror\":%i,", total);
	
	not_MACHINEREADABLE printf("\n### GPHY: TX Collision Counters (saturating at 255, non-zero only)");
	write_data=0x0600;
	for (int phy=16; phy <26; phy++)
	{
		INR_MDIO_write(phy, REG_addr, write_data);
		//usleep(1000);
	}
	usleep(delay);
	total=0;
	for (int phy=16; phy<26; phy++)
	{
		data=INR_MDIO_read(phy, REG_addr);
		//usleep(1000);
		if (data != 0)
		{
			not_MACHINEREADABLE printf("PHY %i: %i collisions\n", phy, data);
			total=total+data;
		}
		MACHINEREADABLE printf("\"txcollisions%i\":%i,", phy, data);
	}
	not_MACHINEREADABLE printf("\nTotal Collisions: %i packets\n\n", total);
	MACHINEREADABLE printf("\"totaltxcollision\":%i", total);
	MACHINEREADABLE printf("}");
}
//************************************************************************************************************************************
/**
*print flow counters
*@param arguments settings
*/

STAT_print_flow_counters(struct arguments *arguments)
{
	uint64_t total=0;
	uint32_t *value,storeval;
	
	//RX
	not_MACHINEREADABLE printf("Displaying non-zero FlowID Match Statistics (RX, good packets only)");
	MACHINEREADABLE printf("{");
	for (int i=0; i <= 1023; i++)
	{
		value=BASE+(C_BASE_ADDR_STATISTICS_RX_GOOD_FLOWID_LOWER_0*256)+(i * 4);
		storeval=*value;// copy because clear on read
		if (storeval != 0)
		{
			total=total+storeval;
			if (i==0)
			{
				not_MACHINEREADABLE printf("\nDP0 FlowID %i (default):%lli \n",i,storeval);
			}
			else
			{
				not_MACHINEREADABLE printf("\nDP0 FlowID %i:%lli \n",i,storeval);
			}
		}
		MACHINEREADABLE printf("\"rxdp0#flowid#%i\":%i,",i,storeval);
	}
	for (int i=0; i <= 1023; i++)
	{
		value=BASE+(C_BASE_ADDR_STATISTICS_RX_GOOD_FLOWID_LOWER_1*256)+(i * 4);
		storeval=*value;// copy because clear on read
		if (storeval != 0)
		{
			total=total+storeval;
			if (i==0)
			{
				not_MACHINEREADABLE printf("\nDP1 FlowID %i (default):%lli \n",i,storeval);
			}
			else
			{
				not_MACHINEREADABLE printf("\nDP1 FlowID %i:%lli \n",i,storeval);
			}
			MACHINEREADABLE printf("\"rxdp1#flowid#%i\":%i,",i,storeval);
		}
	}
	not_MACHINEREADABLE printf("Total Good FlowID:%lli\n",total);
	MACHINEREADABLE printf("\"rxtotal\":%i,",total);

	//TX
	total=0;
	not_MACHINEREADABLE printf("\nDisplaying non-zero FlowID Match Statistics (TX, good packets only)");
	for (int i=0; i <= 1023; i++)
	{
		value=BASE+(C_BASE_ADDR_STATISTICS_TX_GOOD_FLOWID_LOWER_0*256)+(i * 4);
		storeval=*value;// copy because clear on read
		if (storeval != 0)
		{
			total=total+storeval;
			if (i==0)
			{
				not_MACHINEREADABLE printf("\nDP0 FlowID %i (default):%lli \n",i,storeval);
			}
			else
			{
				not_MACHINEREADABLE printf("\nDP0 FlowID %i:%lli \n",i,storeval);
			}
			MACHINEREADABLE printf("\"txdp0#flowid#%i\":%i,",i,storeval);
		}
	}
	for (int i=0; i <= 1023; i++)
	{
		value=BASE+(C_BASE_ADDR_STATISTICS_TX_GOOD_FLOWID_LOWER_1*256)+(i * 4);
		storeval=*value;// copy because clear on read
		if (storeval != 0)
		{
			total=total+storeval;
			if (i==0)
			{
				not_MACHINEREADABLE printf("\nDP1 FlowID %i (default):%lli \n",i,storeval);
			}
			else
			{
				not_MACHINEREADABLE printf("\nDP1 FlowID %i:%lli \n",i,storeval);
			}
		}
		MACHINEREADABLE printf("\"txdp1#flowid#%i\":%i,",i,storeval);
	}
	not_MACHINEREADABLE printf("Total Good FlowID:%lli\n",total);
	MACHINEREADABLE printf("\"txtotal\":%i",total);
	MACHINEREADABLE printf("}");
}

//************************************************************************************************************************************
/**
*print in-out port counters
*@param arguments settings
*/

STAT_print_inout_counters(struct arguments *arguments)
{
	uint8_t inport,outport;
	uint32_t *value,storeval,*value_tx,storeval_tx,i;
	uint64_t allrxcounters=0;
	uint64_t alltxcounters=0;
	not_MACHINEREADABLE printf("DP0:\n");else printf("{");

	for (i=0;i<STAT_portcount*4*STAT_portcount;i++)
	{
		inport=i%32;
		outport=i/32;
		value=BASE+(C_BASE_ADDR_STATISTICS_RX_PKT_CNT_LOWER_0*256)+(i * 4);
		storeval=*value;// copy because clear on read
		value_tx=BASE+(C_BASE_ADDR_STATISTICS_TX_PKT_CNT_LOWER_0*256)+(i * 4);
		storeval_tx=*value_tx;// copy because clear on read
		if (storeval==0xEEEEEEEE)
			storeval=0;
		if (storeval_tx==0xEEEEEEEE)
			storeval_tx=0;
		allrxcounters+=storeval;
		alltxcounters+=storeval_tx;
		MACHINEREADABLE{
			printf("\"dp0#rx#%i#%i\":%i,", inport, outport, storeval);
			printf("\"dp0#tx#%i#%i\":%i,", inport, outport, storeval_tx);
		} else {
			if (storeval)
				printf("-  RX%i (buffered, towards port %i):%i\n", inport, outport, storeval);
			if (storeval_tx)
				printf("-> TX%i (sent, RX from port %i):%i\n", outport, inport, storeval_tx);
			//printf("\n");
		}
	}
	not_MACHINEREADABLE printf("Total packets:%i received and buffered, %i forwarded from buffer\n\n",allrxcounters,alltxcounters);allrxcounters=0;alltxcounters=0;
	not_MACHINEREADABLE printf("DP1:\n");

	for (i=0;i<STAT_portcount*4*STAT_portcount;i++)
	{
		inport=i%32;
		outport=i/32;
		value=BASE+(C_BASE_ADDR_STATISTICS_RX_PKT_CNT_LOWER_1*256)+(i * 4);
		storeval=*value;// copy because clear on read
		value_tx=BASE+(C_BASE_ADDR_STATISTICS_TX_PKT_CNT_LOWER_0*256)+(i * 4);
		storeval_tx=*value_tx;// copy because clear on read
		if (storeval==0xEEEEEEEE)
			storeval=0;
		if (storeval_tx==0xEEEEEEEE)
			storeval_tx=0;
		allrxcounters+=storeval;
		alltxcounters+=storeval_tx;
		MACHINEREADABLE{
			printf("\"dp1#rx#%i#%i\":%i,", inport, outport, storeval);
			printf("\"dp1#tx#%i#%i\":%i,", inport, outport, storeval_tx);
		} else {
			if (storeval)
				printf("-  RX%i (buffered, towards port %i):%i\n", inport, outport, storeval);
			if (storeval_tx)
				printf("-> TX%i (sent, RX from port %i):%i\n", outport, inport, storeval_tx);
			//printf("\n");
		}
	}
	not_MACHINEREADABLE printf("Total packets:%i received and buffered, %i forwarded from buffer\n\n",allrxcounters,alltxcounters);allrxcounters=0;alltxcounters=0;
	MACHINEREADABLE printf("\"end\":0}");
}

//************************************************************************************************************************************
/**
*print  port counters
*@param arguments settings
*/
STAT_print_port_counters(struct arguments *arguments)
{

	uint8_t inport,outport;
	uint32_t *value,storeval,*value_tx,storeval_tx,i;
	uint64_t allcounters=0;
	not_MACHINEREADABLE printf("DP0:\n"); else printf("{");
	
	for (i=0;i<STAT_portcount*2;i++)
	{
		value=BASE+(C_BASE_ADDR_STATISTICS_RX_GOOD_PRT_LOWER_0*256)+(i * 4);
		storeval=*value;// copy because clear on read
		value_tx=BASE+(C_BASE_ADDR_STATISTICS_TX_GOOD_PRT_LOWER_0*256)+(i * 4);
		storeval_tx=*value_tx;// copy because clear on read
		if (storeval==0xEEEEEEEE)
			storeval=0;
		if (storeval_tx==0xEEEEEEEE)
			storeval_tx=0;
		allcounters+=storeval+storeval_tx;
		MACHINEREADABLE{
			printf("\"dp0#rx#%i\":%i,",i,storeval);
			printf("\"dp0#tx#%i\":%i,",i,storeval_tx);
		} else {
			if (storeval)
				printf("RX_Port%i:%i\n",i,storeval);
			if (storeval_tx)
				printf("TX_Port%i:%i\n",i,storeval);
		}
	}
	not_MACHINEREADABLE printf("Total packets:%i\n\n",allcounters);allcounters=0;
	not_MACHINEREADABLE printf("DP1:\n");

	for (i=0;i<STAT_portcount*2;i++)
	{
		value=BASE+(C_BASE_ADDR_STATISTICS_RX_GOOD_PRT_LOWER_1*256)+(i * 4);
		storeval=*value;// copy because clear on read
		value_tx=BASE+(C_BASE_ADDR_STATISTICS_RX_GOOD_PRT_LOWER_1*256)+(i * 4);
		storeval_tx=*value_tx;// copy because clear on read
		if (storeval==0xEEEEEEEE)
			storeval=0;
		if (storeval_tx==0xEEEEEEEE)
			storeval_tx=0;
		allcounters+=storeval+storeval_tx;
		MACHINEREADABLE{
			printf("\"dp1#rx#%i\":%i,", i, storeval);
			printf("\"dp1#tx#%i\":%i,", i, storeval_tx);
		}else{
			if (storeval)
				printf("RX_Port%i:%i\n", i, storeval);
			if (storeval_tx)
				printf("TX_Port%i:%i\n", i, storeval);
		}
	}
	not_MACHINEREADABLE printf("Total packets:%i\n\n",allcounters);allcounters=0;
	MACHINEREADABLE printf("\"end\":0}");
}

//************************************************************************************************************************************
/**
*print  port counters
*@param arguments settings
*/
STAT_print_bad_counters(struct arguments *arguments)
{

	uint8_t inport,outport;
	uint32_t *value,storeval,*value_tx,storeval_tx,i,badrsn;
	uint64_t allcounters=0;

	not_MACHINEREADABLE printf("Bad reason counters:\n");
	not_MACHINEREADABLE printf("DP0:\n"); else printf("{");

	for (i=0;i<STAT_portcount*2;i++)
		for (badrsn=0;badrsn<32;badrsn++)
		{
			value=BASE+(C_BASE_ADDR_STATISTICS_RX_BADRSN_LOWER_0*256)+((i*32+badrsn) * 4);
			storeval=*value;// copy because clear on read
			value_tx=BASE+(C_BASE_ADDR_STATISTICS_TX_BADRSN_LOWER_0*256)+((i*32+badrsn) * 4);
			storeval_tx=*value_tx;// copy because clear on read
			if (storeval==0xEEEEEEEE)
				storeval=0;
			if (storeval_tx==0xEEEEEEEE)
				storeval_tx=0;
			allcounters+=storeval+storeval_tx;
			MACHINEREADABLE{
				printf("\"dp0#rx#%i#%i\":%i,", i, badrsn, storeval);
				printf("\"dp0#tx#%i#%i\":%i,", i, badrsn, storeval_tx);
			} else {
				if (storeval)
					printf("RX_Port%irsn%i:%i\n", i, badrsn, storeval);
				if (storeval_tx)
					printf("TX_Port%irsn%i:%i\n", i, badrsn, storeval);
			}
		}

	not_MACHINEREADABLE printf("Total packets:%i\n\n", allcounters); allcounters=0;
	not_MACHINEREADABLE printf("DP1:\n");

	for (i=0;i<STAT_portcount*2;i++)
		for (badrsn=0;badrsn<32;badrsn++)
		{
			value=BASE+(C_BASE_ADDR_STATISTICS_RX_BADRSN_LOWER_1*256)+((i*32+badrsn) * 4);
			storeval=*value;// copy because clear on read
			value_tx=BASE+(C_BASE_ADDR_STATISTICS_TX_BADRSN_LOWER_1*256)+((i*32+badrsn) * 4);
			storeval_tx=*value_tx;// copy because clear on read
			if (storeval==0xEEEEEEEE)
				storeval=0;
			if (storeval_tx==0xEEEEEEEE)
				storeval_tx=0;
			allcounters+=storeval+storeval_tx;
			MACHINEREADABLE{
				printf("\"dp1#rx#%i#%i\":%i,",i,badrsn,storeval);
				printf("\"dp1#tx#%i#%i\":%i,",i,badrsn,storeval_tx);
			} else {
				if (storeval)
					printf("RX_Port%irsn%i:%i\n", i, badrsn, storeval);
				if (storeval_tx)
					printf("TX_Port%irsn%i:%i\n", i, badrsn, storeval);
			}
		}

	not_MACHINEREADABLE printf("Total packets:%i\n\n", allcounters); allcounters=0;
	not_MACHINEREADABLE printf("Header creation:\n");

	uint64_t hcbase=0;
	for (i=0; i<12; i++)
	{
		switch(i)
		{
			case 0: hcbase=C_BASE_ADDR_HC(0);break;
			case 1: hcbase=C_BASE_ADDR_HC(1);break;
			case 2: hcbase=C_BASE_ADDR_HC(2);break;
			case 3: hcbase=C_BASE_ADDR_HC(3);break;
			case 4: hcbase=C_BASE_ADDR_HC(4);break;
			case 5: hcbase=C_BASE_ADDR_HC(5);break;
			case 6: hcbase=C_BASE_ADDR_HC(6);break;
			case 7: hcbase=C_BASE_ADDR_HC(7);break;
			case 8: hcbase=C_BASE_ADDR_HC(8);break;
			case 9: hcbase=C_BASE_ADDR_HC(9);break;
			case 10: hcbase=C_BASE_ADDR_HC(10);break;
			case 11: hcbase=C_BASE_ADDR_HC(11);break;
			default:hcbase=0;break;
		}
		value=BASE+(hcbase*256)+C_SUB_ADDR_HC_CNT_BAD_FRAMES;
		storeval=*value;// copy because clear on read
		if (storeval==0xEEEEEEEE)
			storeval=0;
		allcounters+=storeval;
		MACHINEREADABLE{
			printf("\"HC%i\":%i,",i,storeval);
		}else{
			if (storeval)
				printf("Port%i:%i\n",i,storeval);
		}
	}
	for (i=0; i<2; i++)
	{
		switch(i)
		{
			case 0: hcbase=C_BASE_ADDR_HC_PCIE(0);break;
			case 1: hcbase=C_BASE_ADDR_HC_PCIE(1);break;
			default:hcbase=0;break;
		}
		value=BASE+(hcbase*256)+C_SUB_ADDR_HC_CNT_BAD_FRAMES;
		storeval=*value;// copy because clear on read
		if (storeval==0xEEEEEEEE)
			storeval=0;
		allcounters+=storeval;
		MACHINEREADABLE{
			printf("\"HC_PCI%i\":%i,",i,storeval);
		}else{
			if (storeval)
				printf("PCI%i:%i\n",i,storeval);
		}
	}
	not_MACHINEREADABLE printf("Total packets:%i\n\n",allcounters);allcounters=0;
	MACHINEREADABLE printf("\"end\":0}");
}
