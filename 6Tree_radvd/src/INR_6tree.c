#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include <linux/ptp_clock.h>
#include <linux/pps.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <unistd.h>
#include <sys/mman.h>
#include <inttypes.h>
#include "INR_6tree.h"

int fd, fd_shadow, fd_master;
const char filename_fc[] = "/sys/bus/pci/devices/0000:01:00.0/resource1";
#define MAP_SIZE 16384000UL
uint64_t *map_base;


void INR_6tree_init(){
	if (INR_MMI_verbose) printf ("__FUNCTION__ = %s\n", __FUNCTION__);
	if ((fd = open (filename_fc, O_RDWR | O_SYNC)) == -1) {
    printf ("error opening file\n");
  	}
  	map_base = mmap (0, MAP_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);	//map files and mmi to memory
  	if (!map_base) {
    printf ("error mapping memory\n");
  	}


}

void INR_6tree_exit(){
	if (INR_MMI_verbose) printf ("__FUNCTION__ = %s\n", __FUNCTION__);
	munmap (map_base, MAP_SIZE);	//unmap files and mmi from memory
  	close (fd);

}

void
INR_MMI_set (uint64_t reg, uint32_t value)
{
    if (INR_MMI_verbose)
      printf ("writing to address:0x%llx value:0x%llx\n", reg, value);
    if (INR_MMI_MEMDUMP)
      printf ("TNbar1 0x%lx w 0x%lx\n", reg, value);
    uint32_t *val = (uint64_t)map_base + reg;
    *val = value;
 
}

uint32_t
INR_MMI_get (uint64_t reg)
{
    if (INR_MMI_verbose)
      printf ("reading from address:0x%llx \n", reg);
    if (INR_MMI_MEMDUMP)
      printf ("TNbar1 0x%lx \n", reg);
    uint32_t *val = (uint64_t)map_base + reg;
    return *val;
 
}
