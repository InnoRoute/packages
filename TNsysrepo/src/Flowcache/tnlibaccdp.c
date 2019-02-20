/**
*@file tnlibaccdp.c
*@brief functions to handle accaleration datapath
*M.Ulbricht 2018
**/
#ifndef __KERNEL__
#include <inttypes.h>
#include <stddef.h>
#include <stdio.h>
#include <endian.h>
#else
#include <linux/kernel.h>
#include <linux/export.h>
#include <linux/module.h>
#include <linux/printk.h>
#define printf printk
#endif
#include "tnlibaccdp.h"
#include "tnlibflowtable.h"
#include "flowtableactions.h"
#include "accdpactions.h"
uint64_t ACCDPbase = 0, ACCDPbase_shadow = 0;
uint8_t ACCDP_ID = 0;

//************************************************************************************************************************************
/**
*check if Acceleration DP is available in current bitstream
*/
uint8_t
INR_accdp_check ()
{
  uint32_t *available = (uint32_t *) (ACCDPbase + INR_ACCDP_available);
  verblog printf ("Acceleration datapath available:0x%lx\n", *available);
  return *available;
}

//************************************************************************************************************************************
/**
*init Acceleration Datapath
*@param baseaddr pointer to mmi-root
*/
void
INR_ACCDP_init (uint64_t * baseaddr, uint64_t * baseaddr_shadow)
{
  verblog printf ("__FUNCTION__ = %s\n", __FUNCTION__);
  ACCDPbase = (uint64_t) baseaddr;
  ACCDPbase_shadow = (uint64_t) baseaddr_shadow;
  verblog printf ("Base_addr:0x%lx shadowbase:0x%lx\n", baseaddr, baseaddr_shadow);
}

//************************************************************************************************************************************
/**
*returns base address of ACCDP
*@param id ACCDP
*/
uint64_t
INR_ACCDP_getbase (uint8_t i)
{
  if (i == 0) {
    return INR_ACC_FIELD_TABLE_0;
  }
  else {
    return INR_ACC_FIELD_TABLE_1;
  }



}

//************************************************************************************************************************************
/**
*returns address of Acceleration Datapath entry
*@param id if of requested entry
*/
uint64_t
INR_ACCDP_get_addr (uint64_t id)
{
  verblog printf ("__FUNCTION__ = %s\n", __FUNCTION__);
  if (id >= INR_ACCDP_length) {
    verblog printf ("index out of range\n");
    return NULL;
  }
  if (!ACCDPbase) {
    return NULL;		//not initalized
  }
  uint64_t addr = (INR_ACCDP_getbase (ACCDP_ID) + id * INR_ACCDP_entry_length);
  verblog printf ("ACCDP addr request for id:%i addr:0x%lx\n", id, addr);
  return ACCDPbase + (addr);
}

//************************************************************************************************************************************
/**
*returns address of Acceleration Datapath entry
*@param id if of requested entry
*/
uint64_t
INR_ACCDP_shadow_get_addr (uint64_t id)
{
  verblog printf ("__FUNCTION__ = %s\n", __FUNCTION__);
  if (id >= INR_ACCDP_length) {
    verblog printf ("index out of range\n");
    return NULL;
  }
  if (!ACCDPbase_shadow) {
    return NULL;		//not initalized
  }
  uint64_t addr = (INR_ACCDP_getbase (ACCDP_ID) + id * INR_ACCDP_entry_length);
  verblog printf ("ACCDP_shadow addr request for id:%i addr:0x%lx\n", id, addr);
  return ACCDPbase_shadow + (addr);
}

//************************************************************************************************************************************
/**
*clear Acceleration Datapath entry
*@param id if of entry
*/
uint8_t
INR_ACCDP_clear_entry (uint64_t id)
{
  verblog printf ("__FUNCTION__ = %s\n", __FUNCTION__);
  verblog printf ("__FUNCTION__ = %s\n", __FUNCTION__);
  uint64_t entry_addr = (uint64_t) INR_ACCDP_get_addr (id);
  struct INR_ACCDP_RULE *entry = entry_addr;
  entry_addr = (uint64_t) INR_ACCDP_shadow_get_addr (id);
  struct INR_ACCDP_RULE *entry_shadow = entry_addr;
  if (entry_addr == NULL) {
    return 1;
  }
  else {			//can't get address
    entry_shadow->enable = 0;	//clear entry
  }
  //FCmemcpy (entry, entry_shadow, INR_ACCDP_entry_length); //copy shadow to mmi (wordwise)
  FCmemcpy (INR_ACCDP_get_addr (0), INR_ACCDP_shadow_get_addr (0), INR_ACCDP_entry_length_memcpy * INR_ACCDP_length);
  return 0;
}

//************************************************************************************************************************************
/**
*returns address of next free Acceleration Datapath entry
*@param start of search
*/
uint64_t
INR_ACCDP_get_next_free_entry (uint64_t id)
{
  verblog printf ("__FUNCTION__ = %s\n", __FUNCTION__);
  uint16_t i = id;
/*  if (i == 0) {
    i++;
  }
*/
  i = 4;
  struct INR_ACCDP_RULE *entry = NULL;
  if (id >= INR_ACCDP_length) {
    return NULL;		//error id not valid
  }
  do {
    entry = (struct INR_ACCDP_RULE *) INR_ACCDP_shadow_get_addr (i);
    i++;
  } while ((i < INR_ACCDP_length) && (entry->enable));
  if (i > 5) {			//== INR_ACCDP_length
    return 0;
  }
  return i - 1;
}
