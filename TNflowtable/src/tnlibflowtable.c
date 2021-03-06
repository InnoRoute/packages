/**
*@file tnlibflowtable.c
*@brief common flowtable access functions
*M.Ulbricht 2017
**/
#ifndef __KERNEL__
#include <inttypes.h>
#include <stddef.h>
#include <stdio.h>
#include <endian.h>
#include <sys/time.h>
#include <time.h>
#else
#include <linux/kernel.h>
#include <linux/export.h>
#include <linux/module.h>
#include <linux/printk.h>
#define printf printk
#endif


#include "tnlibflowtable.h"
#include "tn_env.h"
#include "tnlibaccdp.h"
#include "accdpactions.h"
#include "mastertable.h"
#include "flowtableactions.h"

uint64_t FCbase_EMH = 0;
uint64_t FCbase_EMH_shadow = 0;
uint64_t FCbase_EMA = 0;
uint64_t FCbase_EMA_shadow = 0;
uint8_t verbose = 0;
uint8_t MEMDUMP = 0;
uint8_t touch_HW = 1;		//this switch allows to disable hardware writng.


//************************************************************************************************************************************
/**
*set hardware write bit
*@param value to set
*/
void
INR_FC_set_HW_write (uint8_t value)
{
  touch_HW = value;
}

//************************************************************************************************************************************
/**
*set hardware write bit
*@param value to set
*/
uint8_t
INR_FC_get_HW_write ()
{
  return touch_HW;
}

//************************************************************************************************************************************
/**
*init Flowcache
*@param baseaddr pointer to Flowcache-root
*/
void
FCinit_EMH (uint64_t * baseaddr, uint64_t * baseaddr_shadow)
{
  FCbase_EMA = FCbase_EMH = (uint64_t) baseaddr;
  FCbase_EMA_shadow = FCbase_EMH_shadow = (uint64_t) baseaddr_shadow;
  verblog printf ("Base_addr:0x%lx shadowbase:0x%lx\n", baseaddr, baseaddr_shadow);
}

//************************************************************************************************************************************
/**
*replacement of memcopy to guarantee wordwise memory access
*@param id if of requested entry
*/
void
FCmemcpy (void *dst, const void *src, size_t len)
{
  size_t i;
  uint32_t *d = dst;
  uint32_t *s = src;
  THW {
    for (i = 0; i < len / sizeof (uint32_t); i++) {
      d[i] = s[i];
#ifndef __KERNEL__
      if (MEMDUMP) {
	struct timeval tv;
	gettimeofday (&tv, NULL);
	printf ("TNbar1 0x%08llx w 0x%08llx\n", dst - FCbase_EMA + i * sizeof (uint32_t), s[i]);
      }
#endif
    }
  }
  return dst;
}

//************************************************************************************************************************************
/**
*returns address of HashTable entry
*@param id if of requested entry
*/
uint64_t
INR_HashTable_EMH_get_addr (uint64_t id)
{
  if (id >= INR_FC_EMH_HashTable_length) {
    verblog printf ("error: id not valid\n");
    return NULL;
  }
  if (!FCbase_EMH) {
    verblog printf ("error: id not valid (not initalized)\n");
    return NULL;
  }
  uint64_t addr = INR_FC_EMH_HashTable_base + id * INR_FC_EMH_HashTable_entry_length;
  verblog printf ("HT addr request for id:%i addr:0x%lx\n", id, addr);
  return FCbase_EMH + (addr);
}

//************************************************************************************************************************************
/**
*returns address of HashTable entry for shadow memory
*@param id if of requested entry
*/
uint64_t
INR_HashTable_EMH_shadow_get_addr (uint64_t id)
{
  if (id >= INR_FC_EMH_HashTable_length) {
    verblog printf ("error: id not valid\n");
    return NULL;
  }
  if (!FCbase_EMH_shadow) {
    verblog printf ("error: id not valid (not initalized)\n");
    return NULL;
  }
  uint64_t addr = INR_FC_EMH_HashTable_base + id * INR_FC_EMH_HashTable_entry_length;
  return FCbase_EMH_shadow + (addr);
}

//************************************************************************************************************************************
/**
*clear Hashtable entry
*@param id if of entry
*/
uint8_t
INR_HashTable_EMH_clear_entry (uint64_t id)
{
  verblog printf ("__FUNCTION__ = %s\n", __FUNCTION__);
  struct INR_FC_EMH_HashTable_entry *entry = (struct INR_FC_EMH_HashTable_entry *) INR_HashTable_EMH_get_addr (id);
  if (entry == NULL) {
    return 1;
  }
  else {			//can't get address
    entry->RULEPOINTER = 0;
    entry->unused = 0;		//clear entry
  }
  struct INR_FC_EMH_HashTable_entry *entry_shadow = (struct INR_FC_EMH_HashTable_entry *) INR_HashTable_EMH_shadow_get_addr (id);
  if (entry == NULL) {
    return 1;
  }
  else {			//can't get address
    entry_shadow->RULEPOINTER = 0;
    entry_shadow->unused = 0;	//clear entry
  }
  return 0;
}
//************************************************************************************************************************************
/**
*read Hashtable entry
*@param id if of entry
*/
uint64_t
INR_HashTable_EMA_read_entry (uint16_t ID)
{
  uint64_t addr = FCbase_EMA_shadow + INR_FC_EMA_TCAM_base + ID * INR_FC_EMA_TCAM_entry_length;
  return addr;
}
uint64_t INR_ActT_OVERWRITE_get_base_addr(uint8_t dp){

if(dp==0)return INR_FC_ActT_OVERWRITE_base_0;
if(dp==1)return INR_FC_ActT_OVERWRITE_base_1;
}
//************************************************************************************************************************************
/**
*returns address of ActT entry
*@param id if of requested entry
*/
uint64_t
INR_ActT_OVERWRITE_get_addr (uint64_t id,uint8_t dp)
{
  if (id >= INR_FC_ActT_OVERWRITE_length) {
    verblog printf ("index out of range\n");
    return NULL;
  }
  if (!FCbase_EMH) {
    return NULL;		//not initalized
  }
  
  uint64_t addr = (INR_ActT_OVERWRITE_get_base_addr(dp) + id * INR_FC_ActT_OVERWRITE_entry_length);
  verblog printf ("AT overwrite addr request for id:%i addr:0x%lx, dp:%i\n", id, addr,dp);
  return FCbase_EMA + (addr);
}

//************************************************************************************************************************************
/**
*returns address of ActT entry
*@param id if of requested entry
*/
uint64_t
INR_ActT_OVERWRITE_shadow_get_addr (uint64_t id,uint8_t dp)
{
  if (id >= INR_FC_ActT_OVERWRITE_length) {
    verblog printf ("index out of range\n");
    return NULL;
  }
  if (!FCbase_EMA_shadow) {
    return NULL;		//not initalized
  }
  uint64_t addr = INR_ActT_OVERWRITE_get_base_addr(dp) + id * INR_FC_ActT_OVERWRITE_entry_length;
  return FCbase_EMA_shadow + (addr);
}
//************************************************************************************************************************************
/**
*returns address of RuleTable entry
*@param id if of requested entry
*/
uint64_t
INR_RuleTable_EMH_get_addr (uint64_t id)
{
  if (id >= INR_FC_EMH_RuleTable_length) {
    verblog printf ("index out of range\n");
    return NULL;
  }
  if (!FCbase_EMH) {
    return NULL;		//not initalized
  }
  uint64_t addr = (INR_FC_EMH_RuleTable_base + id * INR_FC_EMH_RuleTable_entry_length);
  verblog printf ("RT addr request for id:%i addr:0x%lx\n", id, addr);
  return FCbase_EMH + (addr);
}

//************************************************************************************************************************************
/**
*returns address of RuleTable entry
*@param id if of requested entry
*/
uint64_t
INR_RuleTable_EMH_shadow_get_addr (uint64_t id)
{
  if (id >= INR_FC_EMH_RuleTable_length) {
    verblog printf ("index out of range\n");
    return NULL;
  }
  if (!FCbase_EMH_shadow) {
    return NULL;		//not initalized
  }
  uint64_t addr = INR_FC_EMH_RuleTable_base + id * INR_FC_EMH_RuleTable_entry_length;
  return FCbase_EMH_shadow + (addr);
}

//************************************************************************************************************************************
/**
*clear Ruletable entry
*@param id if of entry
*/
uint8_t
INR_RuleTable_EMH_clear_entry (uint64_t id)
{
  verblog printf ("__FUNCTION__ = %s\n", __FUNCTION__);
  uint64_t entry_addr = (uint64_t *) INR_RuleTable_EMH_get_addr (id);
  struct INR_FC_EMH_RULE *entry = entry_addr;
  entry_addr = (uint64_t *) INR_RuleTable_EMH_shadow_get_addr (id);
  struct INR_FC_EMH_RULE *entry_shadow = entry_addr;
  if (entry_addr == NULL) {
    return 1;
  }
  else {			//can't get address
    entry_shadow->VALID_BIT = 0;	//clear entry
  }
  THW {
    FCmemcpy (entry, entry_shadow, INR_FC_EMH_RuleTable_entry_length_memcpy);
  }				//copy shadow to mmi (wordwise)
  return 0;
}

//************************************************************************************************************************************
/**
*returns address of next free RuleTable entry
*@param start of search
*/
uint64_t
INR_RuleTable_EMH_get_next_free_entry (uint64_t id)
{
  uint16_t i = id;
  if (i == 0) {
    i++;
  }
  struct INR_FC_EMH_RULE *entry = NULL;
  if (id >= INR_FC_EMH_RuleTable_length) {
    return NULL;		//error id not valid
  }
  do {
    entry = (struct INR_FC_EMH_RULE *) INR_RuleTable_EMH_shadow_get_addr (i);
    i++;
  } while ((i < INR_FC_EMH_RuleTable_length) && (entry->VALID_BIT));
  if (i == INR_FC_EMH_RuleTable_length) {
    return 0;
  }
  return i - 1;
}

//************************************************************************************************************************************
/**
*returns number of used entrys
*
*/
uint32_t
INR_RuleTable_EMH_get_used ()
{
  uint16_t i = 0;
  uint16_t c = 0;
  if (i == 0) {
    i++;
  }
  struct INR_FC_EMH_RULE *entry = NULL;
  for (i = 0; i < INR_FC_EMH_RuleTable_length; i++) {
    entry = (struct INR_FC_EMH_RULE *) INR_RuleTable_EMH_shadow_get_addr (i);
    if (entry->VALID_BIT)
      c++;
  }

  return c;
}

//************************************************************************************************************************************
/**
*returns address of CollisionTabletable entry
*@param id if of requested entry
*/
uint64_t
INR_CTable_EMH_get_addr (uint64_t id)
{
  if (id >= INR_FC_EMH_CTable_length) {
    verblog printf ("index out of range\n");
    return NULL;
  }
  if (!FCbase_EMH) {
    return NULL;		//not initalized
  }
  uint64_t addr = (INR_FC_EMH_CTable_base + id * INR_FC_EMH_CTable_entry_length);
  verblog printf ("CT addr request for id:%i addr:0x%lx\n", id, addr);
  return FCbase_EMH + (addr);
}

//************************************************************************************************************************************
/**
*returns address of CollisionTabletable entry
*@param id if of requested entry
*/
uint64_t
INR_CTable_EMH_shadow_get_addr (uint64_t id)
{
  if (id >= INR_FC_EMH_CTable_length) {
    verblog printf ("index out of range\n");
    return NULL;
  }
  if (!FCbase_EMH_shadow) {
    return NULL;		//not initalized
  }
  uint64_t addr = INR_FC_EMH_CTable_base + id * INR_FC_EMH_CTable_entry_length;
  return FCbase_EMH_shadow + (addr);
}

//************************************************************************************************************************************
/**
*clear CollisionTabletable entry
*@param id if of entry
*/
uint8_t
INR_CTable_EMH_clear_entry (uint64_t id)
{
  verblog printf ("__FUNCTION__ = %s\n", __FUNCTION__);
  uint64_t entry_addr = (uint64_t *) INR_CTable_EMH_get_addr (id);
  struct INR_FC_EMH_RULE *entry = entry_addr;
  entry_addr = (uint64_t *) INR_CTable_EMH_shadow_get_addr (id);
  struct INR_FC_EMH_RULE *entry_shadow = entry_addr;
  if (entry_addr == NULL) {
    return 1;
  }
  else {			//can't get address
    entry_shadow->VALID_BIT = 0;	//clear entry
  }
  THW {
    FCmemcpy (entry, entry_shadow, INR_FC_EMH_CTable_entry_length_memcpy);
  }				//copy shadow to mmi (wordwise)
  return 0;
}

//************************************************************************************************************************************
/**
*returns address of next free CollisionTabletable entry
*@param start of search
*/
uint64_t
INR_CTable_EMH_get_next_free_entry (uint64_t id)
{
  uint16_t i = id;
  struct INR_FC_EMH_RULE *entry = NULL;
  if (id >= INR_FC_EMH_CTable_length) {
    return NULL;		//error id not valid
  }
  do {
    entry = (struct INR_FC_EMH_RULE *) INR_CTable_EMH_shadow_get_addr (i);
    i++;
  } while ((i < INR_FC_EMH_CTable_length) && (entry->VALID_BIT));
  if (i == INR_FC_EMH_CTable_length) {
    return 0;
  }
  return i - 1;
}

//************************************************************************************************************************************
/**
*returns number of used entrys
*
*/
uint16_t
INR_CTable_EMH_get_used ()
{
  uint16_t i = 0;
  uint16_t c = 0;
  if (i == 0) {
    i++;
  }
  struct INR_FC_EMH_RULE *entry = NULL;
  for (i = 0; i < INR_FC_EMH_CTable_length; i++) {
    entry = (struct INR_FC_EMH_RULE *) INR_CTable_EMH_shadow_get_addr (i);
    if (entry->VALID_BIT)
      c++;
  }

  return c;
}

//************************************************************************************************************************************
/**
*returns address of RuleTable entry
*@param id if of requested entry
*/
uint64_t
INR_RuleTable_EMA_get_addr (uint64_t id)
{
  if (id >= INR_FC_EMA_RuleTable_length) {
    verblog printf ("index out of range\n");
    return NULL;
  }
  if (!FCbase_EMH) {
    return NULL;		//not initalized
  }
  uint64_t addr = (INR_FC_EMA_RuleTable_base + id * INR_FC_EMA_RuleTable_entry_length);
  verblog printf ("RT addr request for id:%i addr:0x%lx\n", id, addr);
  return FCbase_EMA + (addr);
}

//************************************************************************************************************************************
/**
*returns address of RuleTable entry
*@param id if of requested entry
*/
uint64_t
INR_RuleTable_EMA_shadow_get_addr (uint64_t id)
{
  if (id >= INR_FC_EMA_RuleTable_length) {
    verblog printf ("index out of range\n");
    return NULL;
  }

  if (!FCbase_EMA_shadow) {
    return NULL;		//not initalized
  }
  uint64_t addr = INR_FC_EMA_RuleTable_base + id * INR_FC_EMA_RuleTable_entry_length;
  return FCbase_EMA_shadow + (addr);
}

//************************************************************************************************************************************
/**
*clear Ruletable entry
*@param id if of entry
*/
uint8_t
INR_RuleTable_EMA_clear_entry (uint64_t id)
{
  verblog printf ("__FUNCTION__ = %s\n", __FUNCTION__);
  uint64_t entry_addr = (uint64_t) INR_RuleTable_EMA_get_addr (id);
  struct INR_FC_EMA_RULE *entry = entry_addr;
  entry_addr = (uint64_t) INR_RuleTable_EMA_shadow_get_addr (id);
  struct INR_FC_EMA_RULE *entry_shadow = entry_addr;
  if (entry_addr == NULL) {
    return 1;
  }
  else {			//can't get address
    entry_shadow->VALID_BIT = 0;	//clear entry
  }
  THW FCmemcpy (entry, entry_shadow, INR_FC_EMA_RuleTable_entry_length_memcpy);	//copy shadow to mmi (wordwise)
  return 0;
}

//************************************************************************************************************************************
/**
*returns address of next free RuleTable entry
*@param start of search
*/
uint64_t
INR_RuleTable_EMA_get_next_free_entry (uint64_t id)
{
  uint16_t i = id;
  if (i == 0) {
    i++;
  }
  struct INR_FC_EMA_RULE *entry = NULL;
  if (id >= INR_FC_EMA_RuleTable_length) {
    return NULL;		//error id not valid
  }
  do {
    entry = (struct INR_FC_EMA_RULE *) INR_RuleTable_EMA_shadow_get_addr (i);
    i++;
  } while ((i < INR_FC_EMA_RuleTable_length) && (entry->VALID_BIT));
  if (i == INR_FC_EMA_RuleTable_length) {
    return 0;
  }
  return i - 1;
}

//************************************************************************************************************************************
/**
*returns number of used entrys
*
*/
uint16_t
INR_RuleTable_EMA_get_used ()
{
  uint16_t i = 0;
  uint16_t c = 0;
  if (i == 0) {
    i++;
  }
  struct INR_FC_EMA_RULE *entry = NULL;
  for (i = 0; i < INR_FC_EMA_RuleTable_length; i++) {
    entry = (struct INR_FC_EMA_RULE *) INR_RuleTable_EMA_shadow_get_addr (i);
    if (entry->VALID_BIT)
      c++;
  }

  return c;
}

//************************************************************************************************************************************
/**
*returns address of next free HashTable entry
*@param start of search
*/
uint64_t
INR_HashTable_EMA_get_next_free_entry (uint64_t id)
{
  verblog printf ("search next free entry from %i\n", id);
  uint16_t i = id;
  if (i == 0) {
    i++;
  }
  union INR_FC_EMA_HashTable_entry *entry = NULL;
  if (id >= INR_FC_EMA_TCAM_length) {
    return NULL;		//error id not valid
  }
  do {
    entry = (union INR_FC_EMA_HashTable_entry *) (FCbase_EMA_shadow + INR_FC_EMA_TCAM_base + i * INR_FC_EMA_TCAM_entry_length);
    i++;
  } while ((i < INR_FC_EMA_TCAM_length) && (entry->fields.mask.reserved));
  if (i == INR_FC_EMA_TCAM_length) {
    return 0;
  }
  return i - 1;
}

//************************************************************************************************************************************
/**
*returns number of used entrys
*
*/
uint16_t
INR_HashTable_EMA_get_used ()
{
  uint16_t i = 0;
  uint16_t c = 0;
  if (i == 0) {
    i++;
  }
  union INR_FC_EMA_HashTable_entry *entry = NULL;
  for (i = 0; i < INR_FC_EMA_TCAM_length; i++) {
    entry = (union INR_FC_EMA_HashTable_entry *) (FCbase_EMA_shadow + INR_FC_EMA_TCAM_base + i * INR_FC_EMA_TCAM_entry_length);
    if (entry->fields.mask.reserved)
      c++;
  }

  return c;
}

//************************************************************************************************************************************
/**
*writes dataset to the TCAm hashtable of EMA
*@param entry hash and mask
*@param id if of entry
*/
uint8_t
INR_HashT_EMA_write (union INR_FC_EMA_HashTable_entry entry, uint16_t ID)
{
  verblog printf ("__FUNCTION__ = %s\n", __FUNCTION__);
  if (ID >= INR_FC_EMA_TCAM_length) {
    verblog printf ("index out of range\n");
    return 0;
  }
  uint32_t dinL = entry.words.dinL;
  uint32_t dinH = entry.words.dinH;
  uint32_t maskL = entry.words.maskL;	//invert mask, FPGA is active low
  uint32_t maskH = entry.words.maskH;
  verblog printf ("HashT_EMA id:%i base:0x%lx\n", ID, FCbase_EMA);
  uint64_t addr = NULL;
  if (INR_HashT_EMA_impl == 0) {
    uint64_t addr = FCbase_EMA + INR_FC_EMA_TCAM_base | (0x7f0 & (ID << 4));
    uint32_t *TCAM;
    THW {
      TCAM = addr;
      verblog printf ("writing dinL:0x%x to 0x%lx\n", dinL, addr);
      *TCAM = dinL;

      addr = FCbase_EMA + INR_FC_EMA_TCAM_base + 0x4;
      TCAM = addr;
      verblog printf ("writing dinH:0x%x to 0x%lx\n", dinH, addr);
      *TCAM = dinH;

      addr = FCbase_EMA + INR_FC_EMA_TCAM_base + 0x8;
      TCAM = addr;
      verblog printf ("writing maskL:0x%x to 0x%lx\n", maskL, addr);
      *TCAM = maskL;

      addr = FCbase_EMA + INR_FC_EMA_TCAM_base + 0xC;
      TCAM = addr;
      verblog printf ("writing maskH:0x%x to 0x%lx\n", maskH, addr);
      *TCAM = maskH;
    }
  }
  union INR_FC_EMA_HashTable_entry *entry_fc;
  if (INR_HashT_EMA_impl) {
    addr = FCbase_EMA + INR_FC_EMA_TCAM_base + ID * INR_FC_EMA_TCAM_entry_length;
    entry_fc = addr;
  }
  addr = FCbase_EMA_shadow + INR_FC_EMA_TCAM_base + ID * INR_FC_EMA_TCAM_entry_length;
  union INR_FC_EMA_HashTable_entry *entry2;
  entry2 = addr;
  verblog printf ("writing in shado memory...\n");
  entry2->words.dinL = dinL;
  entry2->words.dinH = dinH;
  entry2->words.maskL = maskL;
  entry2->words.maskH = maskH;
  if (INR_HashT_EMA_impl) { {
      THW FCmemcpy (entry_fc, entry2, INR_FC_EMA_TCAM_entry_length);
  }
  }				//copy shadow to mmi (wordwise)
  return 0;
}

//************************************************************************************************************************************
/**
*clear Hashtable entry
*@param id if of entry
*/
uint8_t
INR_HashTable_EMA_clear_entry (uint64_t id)
{
  verblog printf ("__FUNCTION__ = %s\n", __FUNCTION__);
  union INR_FC_EMA_HashTable_entry entry;
  entry.words.dinL = 0;
  entry.words.dinH = 0;
  entry.words.maskL = 0;
  entry.words.maskH = 0;
  return INR_HashT_EMA_write (entry, id);
}

//************************************************************************************************************************************
/**
*read Hashtable entry
*@param id if of entry
*/
/*uint64_t*/
/*INR_HashTable_EMA_read_entry (uint16_t ID)*/
/*{*/
/*  uint64_t addr = FCbase_EMA_shadow + INR_FC_EMA_TCAM_base + ID * INR_FC_EMA_TCAM_entry_length;*/
/*  return addr;*/
/*}*/


//************************************************************************************************************************************
/**
*returns address of ActT entry
*@param id if of requested entry
*/
uint64_t
INR_ActT_get_addr (uint64_t id)
{
  if (id >= INR_FC_ActT_length) {
    verblog printf ("index out of range\n");
    return NULL;
  }
  if (!FCbase_EMH) {
    return NULL;		//not initalized
  }
  uint64_t addr = (INR_FC_ActT_base + id * INR_FC_ActT_entry_length);
  verblog printf ("AT addr request for id:%i addr:0x%lx\n", id, addr);
  return FCbase_EMA + (addr);
}

//************************************************************************************************************************************
/**
*returns address of ActT entry
*@param id if of requested entry
*/
uint64_t
INR_ActT_shadow_get_addr (uint64_t id)
{
  if (id >= INR_FC_ActT_length) {
    verblog printf ("index out of range\n");
    return NULL;
  }
  if (!FCbase_EMA_shadow) {
    return NULL;		//not initalized
  }
  uint64_t addr = INR_FC_ActT_base + id * INR_FC_ActT_entry_length;
  return FCbase_EMA_shadow + (addr);
}

//************************************************************************************************************************************
/**
*clear ActT entry
*@param id if of entry
*/
uint8_t
INR_ActT_clear_entry (uint64_t id)
{
  verblog printf ("__FUNCTION__ = %s\n", __FUNCTION__);
  uint64_t entry_addr = (uint64_t) INR_ActT_get_addr (id);
  struct INR_FC_ActT_RULE *entry = entry_addr;
  entry_addr = (uint64_t) INR_ActT_shadow_get_addr (id);
  struct INR_FC_ActT_RULE *entry_shadow = entry_addr;
  if (entry_addr == NULL) {
    return 1;
  }
  else {			//can't get address
    entry_shadow->OutPort_enable = 0;	//clear entry
  }
  entry_shadow->Bad_enable = 0;	//clear entry
  entry_shadow->Cut_enable = 0;	//clear entry
  THW {
    FCmemcpy (entry, entry_shadow, INR_FC_ActT_entry_length);
  }				//copy shadow to mmi (wordwise)
  return 0;
}
//************************************************************************************************************************************
/**
*returns address of next free ActT entry
*@param start of search
*/
uint64_t
INR_ActT_OVERWRITE_get_next_free_entry (uint64_t id){
uint16_t i = id;
if(i==0)i++;
struct INR_FC_ActT_OVERWRITE_RULE *entry = NULL;
    if (id >= INR_FC_ActT_OVERWRITE_length) {
      return NULL;		//error id not valid
    }
    do {
      entry = (struct INR_FC_ActT_OVERWRITE_RULE *) INR_ActT_OVERWRITE_shadow_get_addr (i,0);
      i++;
    } while ((i < INR_FC_ActT_OVERWRITE_length)&&(entry->valid));
    if (i == INR_FC_ActT_OVERWRITE_length) {
      return 0;
    }
    return i - 1;

}


//************************************************************************************************************************************
/**
*returns address of next free ActT entry
*@param start of search
*/
uint64_t
INR_ActT_get_next_free_entry (uint64_t id, uint8_t have_PQUEUE, uint8_t PQUEUE, uint8_t overwrite_action,uint8_t QCI)
{
  uint16_t i = id;
  uint64_t overwrite_offset=0;
  if (i == 0) {
    i++;
  }
 if(overwrite_action){overwrite_offset=INR_ActT_OVERWRITE_get_next_free_entry(0);}
  
  if (have_PQUEUE == 0) {

    struct INR_FC_ActT_RULE *entry = NULL;
    if (id >= INR_FC_ActT_length) {
      return NULL;		//error id not valid
    }
    do {
      entry = (struct INR_FC_ActT_RULE *) INR_ActT_shadow_get_addr (i);
      i++;
    } while ((i < INR_FC_ActT_length)
	     && (entry->OutPort_enable || entry->Bad_enable || entry->Cut_enable || (i == ActT_default_flow) || (i & ActT_queue_entrys_queue_mask)));
    if (i == INR_FC_ActT_length) {
      return 0;
    }
    verblog printf ("Found free ActT entry %i  overwrite_offset:0x%x QCI:0x%x\n", i - 1 +((overwrite_offset&0x7f)<<16)|(((uint64_t)QCI&0x3F)<<12),overwrite_offset,QCI);
    return (i - 1)|((overwrite_offset&0x7f)<<16)|(((uint64_t)QCI&0x3F)<<12);
  }
  else {
    PQUEUE &= 0x1f;		//limit length

    uint16_t maxentry = ~(ActT_queue_entrys_queue_mask - 1);
    struct INR_FC_ActT_RULE *entry = NULL;
    if (id >= INR_FC_ActT_length) {
      return NULL;		//error id not valid
    }
    do {
      entry = (struct INR_FC_ActT_RULE *) INR_ActT_shadow_get_addr (i + (((uint64_t) PQUEUE << 7)));
      i++;
    } while ((i < maxentry) && (entry->OutPort_enable || entry->Bad_enable || entry->Cut_enable || (((uint64_t) PQUEUE << 7) == ActT_default_flow)));
    if (i == maxentry) {
      return 0;


    }
    verblog printf ("Found free ActT entry %i pqueue:0x%x, overwrite_offset:0x%x QCI:0x%x\n", i - 1 + ((uint64_t) PQUEUE << 7)+((overwrite_offset&0x7f)<<16)|(((uint64_t)QCI&0x3F)<<12), ((uint64_t) PQUEUE << 7),overwrite_offset,QCI);

    return ((i - 1) + ((uint64_t) PQUEUE << 7))+((overwrite_offset&0x7f)<<16)|(((uint64_t)QCI&0x3F)<<12); //returns the flow_id
  }
}

//************************************************************************************************************************************
/**
*returns zumber of used entrys
*
*/
uint16_t
INR_ActT_get_used ()
{
  uint16_t i = 0;
  uint16_t c = 0;
  if (i == 0) {
    i++;
  }
  struct INR_FC_ActT_RULE *entry = NULL;
  for (i = 0; i < INR_FC_ActT_length; i++) {
    entry = (struct INR_FC_ActT_RULE *) INR_ActT_shadow_get_addr (i);
    if (entry->OutPort_enable || entry->Bad_enable || entry->Cut_enable)
      c++;
  }

  return c;
}

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
*set verbode
*@param verbose value
*/
void
printallconst ()
{
#ifndef __KERNEL__		//not allowed for kmods
  printf ("Compiled %s %s\n", __DATE__, __TIME__);
#endif
  printconst (EMH_hash_revision);
  printconst (INR_HashT_EMA_impl);
  printconst (C_MMI_ADDR_MAP_REVISION);
  printconst (C_SUB_ADDR_COMMON_TN_MAJOR_REV);
  printconst (C_SUB_ADDR_COMMON_TN_MINOR_REV);
  printconst (C_SUB_ADDR_COMMON_USER_REV);
  printconst (C_SUB_ADDR_COMMON_ADDR_MAP_REV);

  printconst (INR_FC_ActT_entry_length);
  printconst (INR_FC_ActT_entry_length);
  printconst (INR_FC_ActT_entry_length_memcpy);
  printconst (INR_FC_ActT_base);
  printconst (INR_FC_ActT_length);

  printconst (INR_FC_EMH_HashTable_entry_length);
  printconst (INR_FC_EMH_HashTable_entry_length_memcpy);
  printconst (INR_FC_EMH_HashTable_base);
  printconst (INR_FC_EMH_HashTable_length);

  printconst (INR_FC_EMH_RuleTable_entry_length);
  printconst (INR_FC_EMH_RuleTable_entry_length_memcpy);
  printconst (INR_FC_EMH_RuleTable_base);
  printconst (INR_FC_EMH_RuleTable_length);

  printconst (INR_FC_EMH_CTable_entry_length_memcpy);
  printconst (INR_FC_EMH_CTable_entry_length);
  printconst (INR_FC_EMH_CTable_base);
  printconst (INR_FC_EMH_CTable_length);

  printconst (INR_FC_EMA_TCAM_base);
  printconst (INR_FC_EMA_TCAM_length);
  printconst (INR_FC_EMA_TCAM_entry_length);

  printconst (INR_FC_EMA_RuleTable_entry_length);
  printconst (INR_FC_EMA_RuleTable_entry_length_memcpy);
  printconst (INR_FC_EMA_RuleTable_base);
  printconst (INR_FC_EMA_RuleTable_length);

  printconst (INR_ACCDP_available);
  printconst (INR_ACC_FIELD_TABLE_0);
  printconst (INR_ACC_FIELD_TABLE_1);

  printconst (INR_ACCDP_entry_length);
  printconst (INR_ACCDP_length);



};


//************************************************************************************************************************************
/**
*get verbode
*/
uint8_t
get_verbose ()
{
  return verbose;
};

/**
 * hwaddr_aton2 - Convert ASCII string to MAC address (in any known format)
 * wpa_supplicant/hostapd / common helper functions, etc.
 * Copyright (c) 2002-2007, Jouni Malinen <j@w1.fi>
 * @txt: MAC address as a string (e.g., 00:11:22:33:44:55 or 0011.2233.4455)
 * @addr: Buffer for the MAC address (ETH_ALEN = 6 bytes)
 * Returns: Characters used (> 0) on success, -1 on failure
 */
int
hwaddr_aton2 (const char *txt, uint8_t * addr)
{
  int i;
  const char *pos = txt;
  for (i = 0; i < 6; i++) {
    int a, b;
    while (*pos == ':' || *pos == '.' || *pos == '-') {
      pos++;
    }
    a = hex2num (*pos++);
    if (a < 0) {
      return -1;
    }
    b = hex2num (*pos++);
    if (b < 0) {
      return -1;
    }
    *addr++ = (a << 4) | b;
  }
  return pos - txt;
}
//************************************************************************************************************************************
/**
*swaps nibbles and position to match with FC overwrite unit data format
*/
void nibbletwist(uint8_t *data, uint8_t length){
	uint8_t i=0;
	uint8_t tmp;
	//for(i=0;i<length;i++){
	//	data[i]=(data[i]<<4)|(data[i]>>4);
	//}
	for(i=0;i<length/2;i++){
	tmp=data[i];
	data[i]=data[length-i-1];
	data[length-i-1]=tmp;
	}

}

//************************************************************************************************************************************
/**
*convert hex to integer
*@param hex calue
*/
static int
hex2num (char c)
{
  if (c >= '0' && c <= '9') {
    return c - '0';
  }
  if (c >= 'a' && c <= 'f') {
    return c - 'a' + 10;
  }
  if (c >= 'A' && c <= 'F') {
    return c - 'A' + 10;
  }
  return -1;
}

//************************************************************************************************************************************
/**
*parse ipv4 address
*@param ipAddress char
*/
uint32_t
parseIPV4string (char *ipAddress)
{
  char ipbytes[4];
  sscanf (ipAddress, "%uhh.%uhh.%uhh.%uhh", &ipbytes[3], &ipbytes[2], &ipbytes[1], &ipbytes[0]);
  return ipbytes[0] | ipbytes[1] << 8 | ipbytes[2] << 16 | ipbytes[3] << 24;
}

//************************************************************************************************************************************
/**
*print flowtable usage statistics
*
*/
void
FC_statistics_print ()
{
  uint64_t dummy;
  printf ("ActT, used:%i total:%i\n", INR_ActT_get_used (), INR_FC_ActT_length);
  printf ("HashTable_EMA, used:%i total:%i\n", INR_HashTable_EMA_get_used (), INR_FC_EMA_TCAM_length);
  printf ("RuleTable_EMA, used:%i total:%i\n", INR_RuleTable_EMA_get_used (), INR_FC_EMA_RuleTable_length);
  printf ("CTable_EMH, used:%i total:%i\n", INR_CTable_EMH_get_used (), INR_FC_EMH_CTable_length);
  printf ("RuleTable_EMH, used:%i total:%i\n", INR_RuleTable_EMH_get_used (), INR_FC_EMH_RuleTable_length);
  printf ("HashTable_EMH, used:%lli total:%lli\n", INR_RuleTable_EMH_get_used () - INR_CTable_EMH_get_used (), INR_FC_EMH_HashTable_length);
  printf ("Mastertable, used:%lli total:%lli\n", INR_MasterT_get_used (&dummy), MASTERTABLE_length);

}

void
memdump_en ()
{
  MEMDUMP = 1;

}

//************************************************************************************************************************************
/**
*check if EMH table is available in current bitstream
*/
uint8_t
INR_EMH_check ()
{
  uint32_t *available = (uint32_t *) ((C_BASE_ADDR_COMMON_LOWER << 8) | C_SUB_ADDR_COMMON_FEATURES_FLOWCACHE + FCbase_EMA);
  verblog printf ("EMH flowtable available:0x%lx\n", *available & 2);
  return *available & 2;
}

//************************************************************************************************************************************
/**
*check if EMA table is available in current bitstream
*/
uint8_t
INR_EMA_check ()
{
  uint32_t *available = (uint32_t *) ((C_BASE_ADDR_COMMON_LOWER << 8) | C_SUB_ADDR_COMMON_FEATURES_FLOWCACHE + FCbase_EMA);
  verblog printf ("EMA flowtable available:0x%lx\n", *available & 8);
  return *available & 8;
}

//************************************************************************************************************************************
/**
*check if EMA table is available in current bitstream
*/
uint8_t
INR_collT_check ()
{
  uint32_t *available = (uint32_t *) ((C_BASE_ADDR_COMMON_LOWER << 8) | C_SUB_ADDR_COMMON_FEATURES_FLOWCACHE + FCbase_EMA);
  verblog printf ("Collision flowtable available:0x%lx\n", *available & 4);
  return *available & 4;
}

//************************************************************************************************************************************
/**
*check if EMA table is available in current bitstream
*/
uint8_t
INR_FC_check ()
{
  uint32_t *available = (uint32_t *) ((C_BASE_ADDR_COMMON_LOWER << 8) | C_SUB_ADDR_COMMON_FEATURES_FLOWCACHE + FCbase_EMA);
  verblog printf ("Collision flowtable available:0x%lx\n", *available & 1);
  return *available & 1;
}
