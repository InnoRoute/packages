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
				printf("-  RX%i (buffered, towards port %i):%i", inport, outport, storeval);
			if (storeval_tx)
				printf("-> TX%i (sent, RX from port %i):%i", outport, inport, storeval_tx);
			printf("\n");
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
				printf("-  RX%i (buffered, towards port %i):%i", inport, outport, storeval);
			if (storeval_tx)
				printf("-> TX%i (sent, RX from port %i):%i", outport, inport, storeval_tx);
			printf("\n");
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
