/**
*@file mastertable.c
*@brief functions to handle flowtable mastertable
*M.Ulbricht 2015
**/
#ifndef __KERNEL__		//find out, if we build a kernel module or not
#include <inttypes.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#else
#include <linux/kernel.h>
#include <linux/export.h>
#include <linux/module.h>
#include <linux/printk.h>
#define printf printk
#endif

#include "tnlibflowtable.h"
#include "flowtableactions.h"
#include "mastertable.h"


uint64_t MAbase = 0;		//base addres of master table
const uint64_t MASTERTABLE_ENTRY_length = sizeof (struct arguments);

//************************************************************************************************************************************
/**
*init Mastertable
*@param baseaddr_master pointer to Mastertable-root
*/
void
FCinit_MasterTable (uint64_t * baseaddr_master)
{
  MAbase = (uint64_t) baseaddr_master;
  verblog printf ("MasterTable_base_addr:0x%lx\n", baseaddr_master);
}

//************************************************************************************************************************************
/**
*Action entry in MasterTable serach function
*@param arguments struct with request
*/
uint64_t
FC_MT_have_action (struct arguments *arguments)
{
  verblog printf ("__FUNCTION__ = %s\n", __FUNCTION__);
  uint64_t i = 0;
  uint8_t match = 0;
  struct arguments *entry = NULL;
  switch (arguments->ACTION_ID) {	//hande id exeptions
  case 0x155:
    return 0x155;
    break;			//pass to cpu
  default:
    break;
  }
  do {
    match = 1;
    entry = (struct arguments *) INR_MasterT_get_addr (i++);
    if (entry->TableID.ActT == 0 || entry->used == 0) {
      match = 0;
    }
    if (entry->OutPort_enable != arguments->OutPort_enable) {
      match = 0;
    }
    if (entry->OutPort != arguments->OutPort) {
      match = 0;
    }
    if (entry->Bad_enable != arguments->Bad_enable) {
      match = 0;
    }
    if (entry->BadValue != arguments->BadValue) {
      match = 0;
    }
    if (entry->BadReason != arguments->BadReason) {
      match = 0;
    }
    
    if (entry->MAC_DST_OVERWRITE != arguments->MAC_DST_OVERWRITE) {
      match = 0;
    }
    if (entry->MAC_SRC_OVERWRITE != arguments->MAC_SRC_OVERWRITE) {
      match = 0;
    }
    if (entry->IPv4_DST_OVERWRITE != arguments->IPv4_DST_OVERWRITE) {
      match = 0;
    }
    if (entry->IPv4_SRC_OVERWRITE != arguments->IPv4_SRC_OVERWRITE) {
      match = 0;
    }
    if (entry->PORT_DST_OVERWRITE != arguments->PORT_DST_OVERWRITE) {
      match = 0;
    }
    if (entry->PORT_SRC_OVERWRITE != arguments->PORT_SRC_OVERWRITE) {
      match = 0;
    }
    
    
    if (entry->Cut_enable != arguments->Cut_enable) {
      match = 0;
    }
    if (entry->CutValue != arguments->CutValue) {
      match = 0;
    }
    if (entry->PQUEUE != arguments->PQUEUE) {
      match = 0;
    }
    if (match) {
      arguments->TableID.ActT_OVERWITE=entry->TableID.ActT_OVERWITE; // if match store also match for overwrite table
      return entry->TableID.ActT;	//match found, return entry in action table
    }
  } while ((i < MASTERTABLE_length));
  return 0;
}

//************************************************************************************************************************************
/**
*return how often an actiontable entry is used
*@param arguments struct with request
*/
uint64_t
FC_MT_count_action (uint64_t actid)
{
  verblog printf ("__FUNCTION__ = %s\n", __FUNCTION__);
  uint64_t i = 0;
  uint8_t count = 0;
  struct arguments *entry = NULL;
  do {
    entry = (struct arguments *) INR_MasterT_get_addr (i++);
    if (entry->TableID.ActT == actid && entry->used == 1) {
      count++;
    }
  } while ((i < MASTERTABLE_length));
  return count;
}

//************************************************************************************************************************************
/**
*EMH Hash in MasterTable serach function
*@param arguments struct with request
*/
uint8_t
FC_MT_have_EMH_hash (uint16_t hash)
{
  verblog printf ("__FUNCTION__ = %s\n", __FUNCTION__);
  //return 1; // hotfix until problem with EMH_ruletable is solved
  uint64_t i = 0;
  struct arguments *entry = NULL;
  do {
    entry = (struct arguments *) INR_MasterT_get_addr (i++);
    if (entry->used && entry->TableID.EMH_RT && entry->HASH.EMH == hash) {
      return 1;
    }
  } while ((i < MASTERTABLE_length));
  return 0;
}

//************************************************************************************************************************************
/**
*Find entry in MasterTable
*@param arguments struct with request
*/
uint64_t
FC_MT_FIND_entry (struct arguments * arguments)
{
  verblog printf ("__FUNCTION__ = %s\n", __FUNCTION__);
  if (arguments->ID) {
    return arguments->ID;	//if provided, don't search
  }
  uint64_t i = 0;
  uint8_t match = 1;
  struct arguments *entry = NULL;
  do {
    entry = (struct arguments *) INR_MasterT_get_addr (i++);
    match = entry->used;
    MT_matchfiled (entry, arguments, match, MAC_SRC);
    MT_matchfiled (entry, arguments, match, dohave_MAC_SRC);
    MT_matchfiled (entry, arguments, match, MAC_DST);
    MT_matchfiled (entry, arguments, match, dohave_MAC_DST);
    MT_matchfiled (entry, arguments, match, VLAN_ID);
    MT_matchfiled (entry, arguments, match, dohave_VLAN_ID);
    MT_matchfiled (entry, arguments, match, VLAN_PRIO);
    MT_matchfiled (entry, arguments, match, dohave_VLAN_PRIO);
    MT_matchfiled (entry, arguments, match, IPv4_SRC);
    MT_matchfiled (entry, arguments, match, dohave_IPv4_SRC);
    MT_matchfiled (entry, arguments, match, IPv4_DST);
    MT_matchfiled (entry, arguments, match, dohave_IPv4_DST);
    MT_matchfiled (entry, arguments, match, PROTOCOL);
    MT_matchfiled (entry, arguments, match, dohave_PROTOCOL);
    MT_matchfiled (entry, arguments, match, PORT_SRC);
    MT_matchfiled (entry, arguments, match, dohave_PORT_SRC);
    MT_matchfiled (entry, arguments, match, PORT_DST);
    MT_matchfiled (entry, arguments, match, dohave_PORT_DST);
    MT_matchfiled (entry, arguments, match, ETHERTYPE);
    MT_matchfiled (entry, arguments, match, dohave_ETHERTYPE);
    MT_matchfiled (entry, arguments, match, TOS);
    MT_matchfiled (entry, arguments, match, dohave_TOS);
    MT_matchfiled (entry, arguments, match, INPORT);
    MT_matchfiled (entry, arguments, match, dohave_INPORT);
    MT_matchfiled (entry, arguments, match, MASK_MAC_SRC);
    MT_matchfiled (entry, arguments, match, dohave_MASK_MAC_SRC);
    MT_matchfiled (entry, arguments, match, MASK_MAC_DST);
    MT_matchfiled (entry, arguments, match, dohave_MASK_MAC_DST);
    MT_matchfiled (entry, arguments, match, MASK_VLAN_ID);
    MT_matchfiled (entry, arguments, match, dohave_MASK_VLAN_ID);
    MT_matchfiled (entry, arguments, match, MASK_VLAN_PRIO);
    MT_matchfiled (entry, arguments, match, dohave_MASK_VLAN_PRIO);
    MT_matchfiled (entry, arguments, match, MASK_IPv4_SRC);
    MT_matchfiled (entry, arguments, match, dohave_MASK_IPv4_SRC);
    MT_matchfiled (entry, arguments, match, MASK_IPv4_DST);
    MT_matchfiled (entry, arguments, match, dohave_MASK_IPv4_DST);
    MT_matchfiled (entry, arguments, match, MASK_PROTOCOL);
    MT_matchfiled (entry, arguments, match, dohave_MASK_PROTOCOL);
    MT_matchfiled (entry, arguments, match, MASK_PORT_SRC);
    MT_matchfiled (entry, arguments, match, dohave_MASK_PORT_SRC);
    MT_matchfiled (entry, arguments, match, MASK_PORT_DST);
    MT_matchfiled (entry, arguments, match, dohave_MASK_PORT_DST);
    MT_matchfiled (entry, arguments, match, MASK_ETHERTYPE);
    MT_matchfiled (entry, arguments, match, dohave_MASK_ETHERTYPE);
    MT_matchfiled (entry, arguments, match, MASK_TOS);
    MT_matchfiled (entry, arguments, match, dohave_MASK_TOS);
    MT_matchfiled (entry, arguments, match, MASK_INPORT);
    MT_matchfiled (entry, arguments, match, dohave_MASK_INPORT);
    MT_matchfiled (entry, arguments, match, PRIORITY);
    MT_matchfiled (entry, arguments, match, OutPort_enable);
    MT_matchfiled (entry, arguments, match, dohave_OutPort_enable);
    MT_matchfiled (entry, arguments, match, OutPort);
    MT_matchfiled (entry, arguments, match, dohave_OutPort);
    MT_matchfiled (entry, arguments, match, Bad_enable);
    MT_matchfiled (entry, arguments, match, dohave_Bad_enable);
    MT_matchfiled (entry, arguments, match, BadValue);
    MT_matchfiled (entry, arguments, match, dohave_BadValue);
    MT_matchfiled (entry, arguments, match, BadReason);
    MT_matchfiled (entry, arguments, match, dohave_BadReason);
    MT_matchfiled (entry, arguments, match, Cut_enable);
    MT_matchfiled (entry, arguments, match, dohave_Cut_enable);
    MT_matchfiled (entry, arguments, match, CutValue);
    MT_matchfiled (entry, arguments, match, dohave_CutValue);

    verblog if (match)
    {
      printf ("Entry found, ID:%li\n");
    }
    if (match) {
      return i - 1;
    }
  } while ((i < MASTERTABLE_length));
  return 0;
}

//************************************************************************************************************************************
/**
*ultimative masterbrainfunction to store entry automatically in the right table :p
*@param arguments struct with request
*/
uint8_t
FC_MT_autotable (struct arguments * arguments)
{
  uint8_t at_added = 0;
  verblog printf ("__FUNCTION__ = %s\n", __FUNCTION__);
  arguments->ID = 1;		//override automatic switches
  arguments->autoaction = 0;
  //arguments->ACTION_ID = 0; removed to pass actionselection to mastertable
  arguments->HASH.gauto = 0;
  arguments->TYPE_ID = 0;	//reset type for self finding
  verblog printf("matchcount=%i\n",arguments->matchcount);
  if (INR_EMH_check) {
    if (entry_is_EMH1 (arguments)) {
      arguments->TYPE_ID = 1;	//set type
    }
    if (entry_is_EMH2 (arguments)) {
      arguments->TYPE_ID = 2;	// set type //type2 actually deactivated  
    }
    if (entry_is_EMH3 (arguments)) {
      arguments->TYPE_ID = 3;	//set type
    }
    if (entry_is_EMH4 (arguments)) {
      arguments->TYPE_ID = 4;	//set type
    }
  }

  //if (arguments->dohave_PQUEUE == 0) {

  arguments->ACTION_ID = FC_MT_have_action (arguments);	//try to find fitting action
  arguments->TableID.ActT = arguments->ACTION_ID;
  if (arguments->ACTION_ID == 0) {
    AT_add (arguments);		//add new action if not found
    at_added = 1;
  }
  
  /*} else {
     arguments->ID = arguments->PQUEUE;
     arguments->ACTION_ID = arguments->PQUEUE;
     arguments->TableID.ActT = arguments->ACTION_ID;
     AT_update (*arguments);

     } */
  get_HASH (arguments);		//calculate hashes
  if (arguments->TYPE_ID) {	//EMH
    if (FC_MT_have_EMH_hash (arguments->HASH.EMH)) {
      if (INR_collT_check)
	CT_EMH_add (arguments);
      else
	printf ("hash collision, this cule cant be apllied, because the collisiontable is not available in this bitstream\n");
    }
    else {
      RT_EMH_add (arguments);	//add to table
      HT_EMH_add (arguments);	//add to hashtable
    }
    return 0;
  }
  else {			//EMA
    if (INR_EMA_check ()) {
      
      RT_EMA_add (arguments);	//add to table
      HT_EMA_add (arguments);	//add to hashtable
      return 0;
    }
    else {
      printf ("Rule can't be applied the loaded bitstream doesn't provide a ruleset which can used for this rule.\n");
      if (at_added)
	AT_del (*arguments);
	
    }
  }
return 1;
}

//************************************************************************************************************************************
/**
*sort RT/HT_EMA by priority
*/
void
FC_MT_apply_priority ()
{
  verblog printf ("__FUNCTION__ = %s\n", __FUNCTION__);
  INR_FC_set_HW_write (0);	//don't touch hardware
  HT_EMA_clear ();
  RT_EMA_clear ();		//clear all, and rearange from MT information
  INR_FC_set_HW_write (1);	//dont touch hardware
  uint32_t i = 0;
  uint16_t priority = MAXprio;
  uint8_t match = 0;
  uint32_t lastmatch = 0;
  while (priority) {		//do for all priorities
    lastmatch = 0;
    //     do{     
    match = 0;
    for (i = lastmatch; i < MASTERTABLE_length; i++) {
      struct arguments *entry = (struct arguments *) INR_MasterT_get_addr (0xffff & i);
      if (entry != NULL)
	if (entry->used && entry->TableID.EMA_RT)
	  if (entry->PRIORITY == priority) {
	    verblog printf ("sorting match %i\n", i);
//                              match=1;
//                              lastmatch=i+1;//nextrun start on next entry
	    entry->ID = 1;	//search next free space from start of table
	    RT_EMA_add (entry);	//add to table
	    HT_EMA_add (entry);	//add to hashtable
	  }
    }

//              }while(match);//do until no matching entry found  
    priority--;
  }
}


//************************************************************************************************************************************
/**
*saves entry in mastetable
*@param arguments struct with request
*/
void
FC_MasterT_add (struct arguments *arguments)
{
  verblog printf ("__FUNCTION__ = %s\n", __FUNCTION__);
  uint16_t i;
  for (i = 0; i < arguments->bulk + 1; i++) {
    if (arguments->bulk) {
      arguments->MAC_SRC++;
      arguments->MAC_DST++;
      arguments->VLAN_ID++;
      arguments->IPv4_SRC++;
      arguments->IPv4_DST++;	//make some random values to don't have the same hash'

    }
    if (INR_MasterT_get_next_free_entry (arguments->ID)) {
      struct arguments *entry = (struct arguments *) INR_MasterT_get_addr (INR_MasterT_get_next_free_entry (arguments->ID));
      arguments->used = 1;
      if(FC_MT_autotable (arguments))return 0;
      memcpy (entry, arguments, sizeof (struct arguments));

      if ((arguments->bulk && (i == arguments->bulk)) || arguments->bulk == 0)
	if (arguments->TableID.EMA_RT)
	  FC_MT_apply_priority ();
    }
    else {
      printf ("error: MasterTabele full");
    }
  }
  return 0;
}

//************************************************************************************************************************************
/**
*clear entry in mastetable
*@param arguments struct with request
*/
uint8_t
FC_MasterT_del (uint64_t ID)
{
  verblog printf ("__FUNCTION__ = %s\n", __FUNCTION__);
  verblog printf ("going to del entry %li\n", ID);
  if (ID > 0 && ID < MASTERTABLE_length) {
    struct arguments *entry = (struct arguments *) INR_MasterT_get_addr (ID);
    if (entry->used) {
      verblog printf ("Entry stored in ");
      verblog printf ("EMH_RT:%li ", entry->TableID.EMH_RT);
      verblog printf ("EMH_HT:%li ", entry->TableID.EMH_HT);
      verblog printf ("EMH_CT:%li ", entry->TableID.EMH_CT);
      verblog printf ("EMA_RT:%li ", entry->TableID.EMA_RT);
      verblog printf ("EMA_HT:%li ", entry->TableID.EMA_HT);
      verblog printf ("ActT:%li\n ", entry->TableID.ActT);
      verblog printf ("ActT_OVERWITE:%li\n ", entry->TableID.ActT_OVERWITE);
      
      //remove entry from tables where stored
      if (entry->TableID.EMH_RT) {
	INR_RuleTable_EMH_clear_entry (entry->TableID.EMH_RT);
      }
      if (entry->TableID.EMH_HT) {
	INR_HashTable_EMH_clear_entry (0x3fff & entry->HASH.EMH);
      }
      if (entry->TableID.EMH_CT) {
	INR_CTable_EMH_clear_entry (entry->TableID.EMH_CT);
      }
      if (entry->TableID.EMA_RT) {
	INR_RuleTable_EMA_clear_entry (entry->TableID.EMA_RT);
      }
      if (entry->TableID.EMA_HT) {
	INR_HashTable_EMA_clear_entry (entry->TableID.EMA_HT);
      }
      if ((entry->TableID.ActT) && (FC_MT_count_action (entry->TableID.ActT) == 1)) {
	INR_ActT_clear_entry (entry->TableID.ActT);	//can't remove because multible entrys could have the same action, first check
       AT_OVERWRITE_del(entry);  
      }
    }
    entry->used = 0;
  }
  //FC_MT_apply_priority ();
  return ID;
}

//************************************************************************************************************************************
/**
*clear entry in mastetable by arguments
*@param arguments struct with request
*/
uint8_t
FC_MasterT_del_entry (struct arguments * arguments)
{
  verblog printf ("__FUNCTION__ = %s\n", __FUNCTION__);
  return FC_MasterT_del (FC_MT_FIND_entry (arguments));
}

//************************************************************************************************************************************
/**
*clear whole mastetable
*@param arguments struct with request
*/
void
FC_MasterT_clear ()
{
  verblog printf ("__FUNCTION__ = %s\n", __FUNCTION__);
  uint64_t i = 0;
  for (i = 0; i < MASTERTABLE_length; i++) {
    FC_MasterT_del (i);
  }
  return 0;
}

//************************************************************************************************************************************
/**
*returns address of MasterTable entry
*@param id if of requested entry
*/
uint64_t
INR_MasterT_get_addr (uint64_t id)
{
  if (id >= MASTERTABLE_length) {
    verblog printf ("index out of range\n");
    return NULL;
  }
  if (!MAbase) {
    return NULL;		//not initalized
  }
  uint64_t addr = (id * sizeof (struct arguments));
  return MAbase + (addr);
}

//************************************************************************************************************************************
/**
*returns address of next free MasterTable entry
*@param start of search
*/
uint64_t
INR_MasterT_get_next_free_entry (uint64_t id)
{
  verblog printf ("__FUNCTION__ = %s\n", __FUNCTION__);
  uint16_t i = id;
  if (i == 0) {
    i++;
  }
  struct arguments *entry = NULL;
  if (id >= MASTERTABLE_length) {
    return NULL;		//error, id not valid
  }
  do {
    entry = (struct arguments *) INR_MasterT_get_addr (i);
    i++;
  } while ((i < MASTERTABLE_length) && (entry->used));
  if (i == MASTERTABLE_length) {
    return 0;
  }
  return i - 1;
}

//************************************************************************************************************************************
/**
*returns umber of used entrys
*@param *optioncount variable to store amount of used options in the mastertable
*/
uint32_t
INR_MasterT_get_used (uint64_t * optioncount)
{
  uint32_t i = 0;
  uint32_t c = 0;
  if (i == 0) {
    i++;
  }
  struct arguments *entry = NULL;
  for (i = 0; i < MASTERTABLE_length; i++) {
    entry = (struct arguments *) INR_MasterT_get_addr (i);
    if ((entry->used))
      c++;
    *optioncount += entry->OPTION_COUNT;
  }
  //printf("mtcount:%lli\n",c);
  return c;
}

//************************************************************************************************************************************
/**
*get mastetable MAX
*/
uint32_t
FC_MasterT_get_MAX ()
{
  return MASTERTABLE_length;
}

//************************************************************************************************************************************
/**
*get mastetable entry
*@param arguments struct with request
*/
uint8_t
FC_MasterT_get_entry (struct arguments * entry, uint32_t id)
{

  if (id >= MASTERTABLE_length)
    return 0;
  entry = (struct arguments *) INR_MasterT_get_addr (0xffff & id);

  if (entry->used == 0) {
    return 0;
  }
  verblog printf ("__FUNCTION__ = %s\n", __FUNCTION__);
  if (entry)
    return 1;
  else
    return 0;
}

//************************************************************************************************************************************
/**
*print mastetable
*@param arguments struct with request
*/
void
FC_MasterT_print (struct arguments *arguments)
{
  if(!arguments->machinereadable)printf ("__FUNCTION__ = %s\n", __FUNCTION__);
  uint32_t i = 0;
  if (arguments->COUNT == 0) {
    arguments->COUNT = MASTERTABLE_length;
  }
  
      	if(arguments->machinereadable){
    	printf("{\"FT\":[");
    	}else{
    	printf ("Print MasterTable from %i to %i.\n", arguments->ID, arguments->ID + arguments->COUNT - 1); 	
  	
    	}
  for (i = arguments->ID; ((i < MASTERTABLE_length) && (i < arguments->ID + arguments->COUNT)); i++) {
    struct arguments *entry = (struct arguments *) INR_MasterT_get_addr (0xffff & i);
    if (entry != NULL) {
    	if(arguments->machinereadable){
    	printf("{\"ID\":%li,\"VALID_BIT\":%lli",i,entry->used);
    	}    	
    	else{
		    printf ("ID:%li  ", i);
		    printf ("VALID_BIT:0x%x  ", entry->used);
      }
      if (entry->used) {
      if(arguments->machinereadable)printf(",");
	MT_dohaveprint (entry, MAC_SRC);	//print if filed is available
	if(arguments->machinereadable)printf(",");
	MT_dohaveprint (entry, MAC_DST);
	if(arguments->machinereadable)printf(",");
	MT_dohaveprint (entry, VLAN_ID);
	if(arguments->machinereadable)printf(",");
	MT_dohaveprint (entry, VLAN_PRIO);
	if(arguments->machinereadable)printf(",");
	MT_dohaveprint (entry, IPv4_SRC);
	if(arguments->machinereadable)printf(",");
	MT_dohaveprint (entry, IPv4_DST);
	if(arguments->machinereadable)printf(",");
	MT_dohaveprint (entry, PROTOCOL);
	if(arguments->machinereadable)printf(",");
	MT_dohaveprint (entry, PORT_SRC);
	if(arguments->machinereadable)printf(",");
	MT_dohaveprint (entry, PORT_DST);
	if(arguments->machinereadable)printf(",");
	MT_dohaveprint (entry, ETHERTYPE);
	if(arguments->machinereadable)printf(",");
	MT_dohaveprint (entry, TOS);
	if(arguments->machinereadable)printf(",");
	MT_dohaveprint (entry, INPORT);
	if(arguments->machinereadable)printf(",");
	MT_dohaveprint (entry, MASK_MAC_SRC);
	if(arguments->machinereadable)printf(",");
	MT_dohaveprint (entry, MASK_MAC_DST);
	if(arguments->machinereadable)printf(",");
	MT_dohaveprint (entry, MASK_VLAN_ID);
	if(arguments->machinereadable)printf(",");
	MT_dohaveprint (entry, MASK_VLAN_PRIO);
	if(arguments->machinereadable)printf(",");
	MT_dohaveprint (entry, MASK_IPv4_SRC);
	if(arguments->machinereadable)printf(",");
	MT_dohaveprint (entry, MASK_IPv4_DST);
	if(arguments->machinereadable)printf(",");
	MT_dohaveprint (entry, MASK_PROTOCOL);
	if(arguments->machinereadable)printf(",");
	MT_dohaveprint (entry, MASK_PORT_SRC);
	if(arguments->machinereadable)printf(",");
	MT_dohaveprint (entry, MASK_PORT_DST);
	if(arguments->machinereadable)printf(",");
	MT_dohaveprint (entry, MASK_ETHERTYPE);
	if(arguments->machinereadable)printf(",");
	MT_dohaveprint (entry, MASK_TOS);
	if(arguments->machinereadable)printf(",");
	MT_dohaveprint (entry, MASK_INPORT);
	if(arguments->machinereadable)printf(",");
	MT_dohaveprint (entry, PQUEUE);
	if(arguments->machinereadable)printf(",");
	MT_dohaveprint (entry, OutPort_enable);
	if(arguments->machinereadable)printf(",");
	MT_dohaveprint (entry, OutPort);
	if(arguments->machinereadable)printf(",");
	MT_dohaveprint (entry, Bad_enable);
	if(arguments->machinereadable)printf(",");
	MT_dohaveprint (entry, BadValue);
	if(arguments->machinereadable)printf(",");
	MT_dohaveprint (entry, BadReason);
	if(arguments->machinereadable)printf(",");
	MT_dohaveprint (entry, Cut_enable);
	if(arguments->machinereadable)printf(",");
	MT_dohaveprint (entry, CutValue);
	if(arguments->machinereadable)printf(",");
	
		MT_dohaveprint (entry, MAC_DST_OVERWRITE);
		if(arguments->machinereadable)printf(",");
	MT_dohaveprint (entry, MAC_SRC_OVERWRITE);
	if(arguments->machinereadable)printf(",");
	MT_dohaveprint (entry, IPv4_DST_OVERWRITE);
	if(arguments->machinereadable)printf(",");
	MT_dohaveprint (entry, IPv4_SRC_OVERWRITE);
	if(arguments->machinereadable)printf(",");
	MT_dohaveprint (entry, PORT_DST_OVERWRITE);
	if(arguments->machinereadable)printf(",");
	MT_dohaveprint (entry, PORT_SRC_OVERWRITE);
if(arguments->machinereadable)printf(",");
	if(arguments->machinereadable){
	printf ("\"prio\":%li,", entry->PRIORITY);
			printf ("\"EMH_RT\":%li,", entry->TableID.EMH_RT);
			printf ("\"EMH_HT\":%li,", entry->TableID.EMH_HT);
			printf ("\"EMH_CT\":%li,", entry->TableID.EMH_CT);
			printf ("\"EMA_RT\":%li,", entry->TableID.EMA_RT);
			printf ("\"EMA_HT\":%li,", entry->TableID.EMA_HT);
			printf ("\"ActT\":%li ", entry->TableID.ActT);

	
	}else{
		printf ("\n");
		printf ("prio:%li ", entry->PRIORITY);
		if (entry->TableID.EMH_RT)
			printf ("EMH_RT:%li ", entry->TableID.EMH_RT);
		if (entry->TableID.EMH_HT)
			printf ("EMH_HT:%li ", entry->TableID.EMH_HT);
		if (entry->TableID.EMH_CT)
			printf ("EMH_CT:%li ", entry->TableID.EMH_CT);
		if (entry->TableID.EMA_RT)
			printf ("EMA_RT:%li ", entry->TableID.EMA_RT);
		if (entry->TableID.EMA_HT)
			printf ("EMA_HT:%li ", entry->TableID.EMA_HT);
		if (entry->TableID.ActT) {
			printf ("ActT:%li ", entry->TableID.ActT);
		}
		if (entry->TableID.ActT_OVERWITE) {
	  printf ("ActT_OVERWITE:%li ", entry->TableID.ActT_OVERWITE);
	}
	}

      }
      if(arguments->machinereadable){
    	if(i<(arguments->ID + arguments->COUNT-1)){printf("},");}else{printf("}");}
    	}else
      printf ("\n");
      
      
    }
    else {
      printf ("ID not valid\n");
    }
  }
  if(arguments->machinereadable){
    	printf("]}");
    	}
}

//************************************************************************************************************************************
/**
*update mastetable entry
*@param arguments struct with request
*/
void
FC_MasterT_update (struct arguments *arguments)
{
  printf ("__FUNCTION__ = %s\n", __FUNCTION__);
  return 0;
}
