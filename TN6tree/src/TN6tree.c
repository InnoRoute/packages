/**
*@file TN6tree.c
*@brief userspace 6tree configuration tool
*Bill Farrow, Jan-Derk Bakker, M.Ulbricht 2017
**/

//*****************************************************************************************************************
/*
 * pcimem.c: Simple program to read/write from/to a pci device from userspace.
 *
 *  Copyright (C) 2010, Bill Farrow (bfarrow@beyondelectronics.us)
 *
 *  Based on the devmem2.c code
 *  Copyright (C) 2000, Jan-Derk Bakker (J.D.Bakker@its.tudelft.nl)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *changed ulbricht@innoroute.de 2017
 *providing mmi interface to TrustNode
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <getopt.h>
#include <signal.h>
#include <fcntl.h>
#include <ctype.h>
#include <termios.h>
#include <sys/types.h>
#include <sys/mman.h>
#include "TN6tree.h"
#include "tn_env.h"

uint64_t map_base; /**<global mapped base address of mmi*/

//********************************************************************************************************************
/**
*read word value from mmi
@param address
*/
uint32_t
MMIread (uint64_t target)
{  uint32_t *read_result;
  read_result=(uint64_t *)(map_base + target);
  return *read_result;

}
//********************************************************************************************************************
/**
*return mmi base addres of specific network interface
@param netfork interface id
*/
uint32_t get_PHY_base(uint8_t index){
switch(index){
		case 0:return PHY0_base;break;
		case 1:return PHY1_base;break; 
		case 2:return PHY2_base;break; 
		case 3:return PHY3_base;break; 
		case 4:return PHY4_base;break; 
		case 5:return PHY5_base;break; 
		case 6:return PHY6_base;break; 
		case 7:return PHY7_base;break; 
		case 8:return PHY8_base;break; 
		case 9:return PHY9_base;break; 
		case 10:return PHY10_base;break; 
		case 11:return PHY11_base;break; 
		default:return PHY0_base;break; 
	}


}
//********************************************************************************************************************
/**
*write word value to mmi address
@param address
@param write value
*/
uint32_t
MMIwrite (uint64_t target, uint32_t writeval)
{ uint32_t *read_result;
  read_result=(uint64_t *)(map_base + target);
  *read_result=writeval;
  return *read_result;

}
//********************************************************************************************************************
/**
*prints all 6tree configuration parameters for all interfaces
*/
uint8_t
printentry ()
{uint8_t i=0;
uint64_t baseaddress=0;
printf("compiled: %s %s\n", __DATE__, __TIME__);
#ifdef C_MMI_ADDR_MAP_REVISION 
printconst(C_MMI_ADDR_MAP_REVISION);
#endif
#ifdef C_SUB_ADDR_COMMON_TN_MAJOR_REV
printconst(C_SUB_ADDR_COMMON_TN_MAJOR_REV);
#endif
#ifdef C_SUB_ADDR_COMMON_TN_MINOR_REV
printconst(C_SUB_ADDR_COMMON_TN_MINOR_REV);
#endif
#ifdef C_SUB_ADDR_COMMON_USER_REV
printconst(C_SUB_ADDR_COMMON_USER_REV);
#endif
#ifdef C_SUB_ADDR_COMMON_ADDR_MAP_REV
printconst(C_SUB_ADDR_COMMON_ADDR_MAP_REV);
#endif
#ifdef C_SUB_ADDR_MAC_6T_ADDR_L
printconst(C_SUB_ADDR_MAC_6T_ADDR_L);
#endif
#ifdef C_SUB_ADDR_MAC_6T_ADDR_H
printconst(C_SUB_ADDR_MAC_6T_ADDR_H);
#endif
#ifdef C_SUB_ADDR_MAC_6T_SRC_PTR
printconst(C_SUB_ADDR_MAC_6T_SRC_PTR);
#endif
printconst(PHY0_base);
for(i=0;i<12;i++){
  baseaddress=get_PHY_base(i);
  printf ("Interface:%i 6Treeprefix:AF%06lx%08lx bytecount_src:%i bytecount_dst:%i\n",i, MMIread (baseaddress+INR_6T_ADDR_H), MMIread (baseaddress+INR_6T_ADDR_L),MMIread(baseaddress+INR_6T_SRC_PTR),MMIread(baseaddress+INR_6T_DST_PTR));
}
}
//********************************************************************************************************************
/**
*main function
*/
int
main (int argc, char **argv)
{
  int fd;

  uint32_t read_result, writeval;
  char *filename;
  off_t target;
  int access_type = 'w';
  int i = 0;
  int64_t listindex = -1;

  char *error = 0;
  uint8_t writeentry = 0;
  uint8_t printall = 0;
  uint8_t writeall = 1;
  uint64_t prefix = 0;
  uint8_t id=0; 
  uint8_t srcbitcount=0,dstbitcount=0;


  if ((fd = open ("/sys/bus/pci/devices/0000:01:00.0/resource1", O_RDWR | O_SYNC)) == -1)
    PRINT_ERROR;

  map_base  = mmap (0, MAP_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);//map files and mmi to memory
  if (map_base == (void *) -1)
    PRINT_ERROR;
    if(ENABLE==0){
    printf("6tree addresses not provided in tn_env.h\n");
    goto done;
    }
if(argc==1) goto printhelp;//no parameters :/
  while ((i = getopt (argc, argv, "B:p:i:b:P")) != -1) {
    switch (i) {
    case 'p':
      writeentry = 1;
      if (optarg)
//	sscanf (optarg, "%I64x", &prefix);
	prefix=strtoul(optarg,0,0);
      break;
    case 'i':
      if (optarg)
	//sscanf (optarg, "%i", &id);
	id=atoi(optarg);
	writeall=0;//i set, dont write to all
      break;
    case 'b':
      if (optarg)
	srcbitcount=atoi(optarg);
      break;
    case 'B':
      if (optarg)
	dstbitcount=atoi(optarg);
      break;
    case 'P':
    	printall=1;
    	break;
    default:
printhelp:
      fprintf (stderr, "usage: %s [options]\n", *argv);
      fprintf (stderr, "\t-p prefix in hex\n");
      fprintf (stderr, "\t-i index of network interface\n");
      fprintf (stderr, "\t-b bytecount for srcprefix\n");
      fprintf (stderr, "\t-B bytecount for dstprefix\n");
      fprintf (stderr, "\t-P print all settings\n");

      goto done;
    }
  }
uint64_t baseaddress=0;
  if (writeentry) {
  printf("prefix: 0x%lx index:%i srcbitcount:%i dstbitcount:%i\n",prefix,id,srcbitcount,dstbitcount);
  /*
  printf("Write 0x%lx to 0x%x\n",MMIwrite (baseaddress + INR_6T_ADDR_H, (prefix&0xffffffff00000000) >> 32),baseaddress + INR_6T_ADDR_H);
  printf("Write 0x%lx to 0x%x\n",MMIwrite (baseaddress+ INR_6T_ADDR_L, prefix & 0xffffffff),baseaddress+ INR_6T_ADDR_L);
  printf("Write 0x%lx to 0x%x\n",MMIwrite (baseaddress+ INR_6T_SRC_PTR, bitcount&0xf),baseaddress+ INR_6T_SRC_PTR);
  printf("Write 0x%lx to 0x%x\n",MMIwrite (baseaddress+ INR_6T_DST_PTR, bitcount&0xf),baseaddress+ INR_6T_DST_PTR);
  */
  if (writeall)for(uint8_t i=0;i<12;i++){
  	baseaddress=get_PHY_base(i);
  	MMIwrite (baseaddress + INR_6T_ADDR_H, (prefix&0xffffffff00000000) >> 32);
 	MMIwrite (baseaddress+ INR_6T_ADDR_L, prefix & 0xffffffff);
  	MMIwrite (baseaddress+ INR_6T_SRC_PTR, srcbitcount&0xf);
  	MMIwrite (baseaddress+ INR_6T_DST_PTR, dstbitcount&0xf);
  }else {
  	baseaddress=get_PHY_base(id);
  	MMIwrite (baseaddress + INR_6T_ADDR_H, (prefix&0xffffffff00000000) >> 32);
  	MMIwrite (baseaddress+ INR_6T_ADDR_L, prefix & 0xffffffff);
  	MMIwrite (baseaddress+ INR_6T_SRC_PTR, srcbitcount&0xf);
  	MMIwrite (baseaddress+ INR_6T_DST_PTR, dstbitcount&0xf);
  }}

  if(printall)printentry ();





done:

  if (munmap (map_base, MAP_SIZE) == -1)
    PRINT_ERROR;
  close (fd);

  return 0;
}
