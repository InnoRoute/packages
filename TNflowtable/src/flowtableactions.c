#ifndef __KERNEL__
	#include <inttypes.h>
	#include <stddef.h>
	#include <stdio.h>
#else
	#include <linux/kernel.h>
	#include <linux/export.h>
	#include <linux/module.h>
	#include <linux/printk.h>
	#define printf printk
#endif
#include "tnlibflowtable.h"
#include "flowtableactions.h"

//********************************************************************************************************************
/**
*set default values to arguments structure
@param arguments argumentes from userinterface
*/
void
clear_arguments (struct arguments *arguments)
{
  arguments->args[0] = "";
//  arguments.args[1] = "";
  arguments->verbose = 0;
  arguments->MAC_SRC = 0;
  arguments->MAC_DST = 0;
  arguments->VLAN_ID = 0;
  arguments->VLAN_PRIO = 0;
  arguments->INPORT = 0;
  arguments->IPv4_SRC = 0;
  arguments->IPv4_DST = 0;
  arguments->PROTOCOL = 0;
  arguments->PORT_SRC = 0;
  arguments->PORT_DST = 0;
  arguments->TYPE_ID = 0;
  arguments->ETHERTYPE = 0;
  arguments->PRIORITY = 1;	//zero seems not a good default value..
  arguments->TOS = 0;
  arguments->ACTION_ID = 0;
//arguments.NEXT_BIT=0;  
  arguments->RULEPOINTER = 0;
  arguments->ID = 0;
  arguments->COUNT = 0;
  arguments->HASH.EMH = 0;
  arguments->HASH.gauto = 0;
  arguments->MASK_MAC_SRC = 0xffffffffffffffff;
  arguments->MASK_MAC_DST = 0xffffffffffffffff;
  arguments->MASK_VLAN_ID = 0xffffffffffffffff;
  arguments->MASK_VLAN_PRIO = 0xffffffffffffffff;
  arguments->MASK_IPv4_SRC = 0xffffffffffffffff;
  arguments->MASK_IPv4_DST = 0xffffffffffffffff;
  arguments->MASK_PROTOCOL = 0xffffffffffffffff;
  arguments->MASK_PORT_SRC = 0xffffffffffffffff;
  arguments->MASK_PORT_DST = 0xffffffffffffffff;
  arguments->MASK_ETHERTYPE = 0xffffffffffffffff;
  arguments->MASK_TOS = 0xffffffffffffffff;
  arguments->MASK_INPORT = 0xffffffffffffffff;
  arguments->OutPort_enable = 0;
  arguments->OutPort = 0;
  arguments->Bad_enable = 0;
  arguments->BadValue = 0;
  arguments->BadReason = 0;
  arguments->Cut_enable = 0;
  arguments->CutValue = 0;
  arguments->autoaction = 0;
  arguments->TableID.EMH_RT = 0;
  arguments->TableID.EMH_HT = 0;
  arguments->TableID.EMH_CT = 0;
  arguments->TableID.EMA_RT = 0;
  arguments->TableID.EMA_HT = 0;
  arguments->TableID.ActT = 0;
  arguments->used = 0;

  arguments->dohave_MAC_SRC = 0;
  arguments->dohave_MAC_DST = 0;
  arguments->dohave_VLAN_ID = 0;
  arguments->dohave_VLAN_PRIO = 0;
  arguments->dohave_INPORT = 0;
  arguments->dohave_IPv4_SRC = 0;
  arguments->dohave_IPv4_DST = 0;
  arguments->dohave_PROTOCOL = 0;
  arguments->dohave_PORT_SRC = 0;
  arguments->dohave_PORT_DST = 0;
  arguments->dohave_ETHERTYPE = 0;
  arguments->dohave_TOS = 0;
  arguments->dohave_MASK_MAC_SRC = 0;
  arguments->dohave_MASK_MAC_DST = 0;
  arguments->dohave_MASK_VLAN_ID = 0;
  arguments->dohave_MASK_VLAN_PRIO = 0;
  arguments->dohave_MASK_IPv4_SRC = 0;
  arguments->dohave_MASK_IPv4_DST = 0;
  arguments->dohave_MASK_PROTOCOL = 0;
  arguments->dohave_MASK_PORT_SRC = 0;
  arguments->dohave_MASK_PORT_DST = 0;
  arguments->dohave_MASK_ETHERTYPE = 0;
  arguments->dohave_MASK_TOS = 0;
  arguments->dohave_MASK_INPORT = 0;
  arguments->dohave_OutPort_enable = 0;
  arguments->dohave_OutPort = 0;
  arguments->dohave_Bad_enable = 0;
  arguments->dohave_BadValue = 0;
  arguments->dohave_BadReason = 0;
  arguments->dohave_Cut_enable = 0;
  arguments->dohave_CutValue = 0;

}

//********************************************************************************************************************
/**
*calculate all hash values for the given dataset
@param arguments argumentes from userinterface
*/
void
get_HASH (struct arguments *arguments)
{
  switch (arguments->TYPE_ID) {
  case 1:
    arguments->HASH.EMH = (0x3fff & (arguments->MAC_SRC >> 0)) ^
      (0x3fff & (arguments->MAC_SRC >> 14)) ^
      (0x3fff & (arguments->MAC_SRC >> 28)) ^
      (0x3fff & (arguments->MAC_SRC >> 42)) ^
      (0x3fff & (arguments->MAC_DST >> 0)) ^
      (0x3fff & (arguments->MAC_DST >> 14)) ^
      (0x3fff & (arguments->MAC_DST >> 28)) ^
      (0x3fff & (arguments->MAC_DST >> 42)) ^
      (0x3fff & (arguments->ETHERTYPE >> 0)) ^ (0x3fff & (arguments->ETHERTYPE >> 14)) ^ (0x3fff & (arguments->VLAN_ID << 0));
    break;
  case 2:
    arguments->HASH.EMH = (0x3fff & (arguments->IPv4_SRC >> 0)) ^
      (0x3fff & (arguments->IPv4_SRC >> 14)) ^
      (0x3fff & (arguments->IPv4_SRC >> 28)) ^
      (0x3fff & (arguments->IPv4_DST >> 0)) ^
      (0x3fff & (arguments->IPv4_DST >> 14)) ^
      (0x3fff & (arguments->IPv4_DST >> 28)) ^
      (0x3fff & (arguments->PROTOCOL << 0)) ^ (0x3fff & (arguments->PORT_SRC >> 0)) ^ (0x3fff & (arguments->PORT_DST >> 0));
    break;
  default:
    verblog    printf ("Type not valid\n");
    break;
  }

  arguments->HASH.EMA.reserved = 0;
  arguments->HASH.EMA.INGRESS_PORT = arguments->INPORT;
  arguments->HASH.EMA.MAC_SRC =
    (0xff & (arguments->MAC_SRC >> 0)) ^
    (0xff & (arguments->MAC_SRC >> 8)) ^
    (0xff & (arguments->MAC_SRC >> 16)) ^
    (0xff & (arguments->MAC_SRC >> 24)) ^ (0xff & (arguments->MAC_SRC >> 32)) ^ (0xff & (arguments->MAC_SRC >> 40));
  arguments->HASH.EMA.MAC_DST =
    (0xff & (arguments->MAC_DST >> 0)) ^
    (0xff & (arguments->MAC_DST >> 8)) ^
    (0xff & (arguments->MAC_DST >> 16)) ^
    (0xff & (arguments->MAC_DST >> 24)) ^ (0xff & (arguments->MAC_DST >> 32)) ^ (0xff & (arguments->MAC_DST >> 40));
  arguments->HASH.EMA.ETHERTYPE =
    (0xf & (arguments->ETHERTYPE >> 0)) ^
    (0xf & (arguments->ETHERTYPE >> 4)) ^ (0xf & (arguments->ETHERTYPE >> 8)) ^ (0xf & (arguments->ETHERTYPE >> 12));
  arguments->HASH.EMA.VLAN_ID = (0xf & (arguments->VLAN_ID << 0)) ^ (0xf & (arguments->VLAN_ID << 4)) ^ (0xf & (arguments->VLAN_ID << 8));
  arguments->HASH.EMA.VLAN_PRIO = 0x7 & (arguments->VLAN_PRIO);
  arguments->HASH.EMA.IPv4_SRC = (0xff & (arguments->IPv4_SRC >> 0)) ^
    (0xff & (arguments->IPv4_SRC >> 8)) ^ (0xff & (arguments->IPv4_SRC >> 16)) ^ (0xff & (arguments->IPv4_SRC >> 24));
  arguments->HASH.EMA.IPv4_DST =
    (0xff & (arguments->IPv4_DST >> 0)) ^
    (0xff & (arguments->IPv4_DST >> 8)) ^ (0xff & (arguments->IPv4_DST >> 16)) ^ (0xff & (arguments->IPv4_DST >> 24));
  arguments->HASH.EMA.PROTOCOL = (0xf & (arguments->PROTOCOL >> 0)) ^ (0xf & (arguments->PROTOCOL >> 4));
  arguments->HASH.EMA.TOS = (0x7 & (arguments->TOS >> 0)) ^ (0x7 & (arguments->TOS >> 3));
  arguments->HASH.EMA.PORT_SRC = (0xf & (arguments->PORT_SRC >> 0)) ^
    (0xf & (arguments->PORT_SRC >> 4)) ^ (0xf & (arguments->PORT_SRC >> 8)) ^ (0xf & (arguments->PORT_SRC >> 12));
  arguments->HASH.EMA.PORT_DST = (0xf & (arguments->PORT_DST >> 0)) ^
    (0xf & (arguments->PORT_DST >> 4)) ^ (0xf & (arguments->PORT_DST >> 8)) ^ (0xf & (arguments->PORT_DST >> 12));

  uint64_t *ema;
  ema = &arguments->HASH.EMA;
  verblog  printf ("EMH hash value:0x%llx\n", arguments->HASH.EMH);
  verblog  printf ("EMA hash value:0x%llx\n", *ema);
}

//********************************************************************************************************************
/**
*adds entry in hashtable
@param arguments argumentes from userinterface
*/
void
HT_EMH_add (struct arguments *arguments)
{
  printf ("__FUNCTION__ = %s\n", __FUNCTION__);
  if (arguments->HASH.gauto) {
    verblog    printf ("autogenhash...\n");
    get_HASH (arguments);
  }
  verblog  printf ("Hash:0x%x Rulepointer:0x%x\n", arguments->HASH.EMH, arguments->RULEPOINTER);
  struct INR_FC_EMH_HashTable_entry *entry = (struct INR_FC_EMH_HashTable_entry *) INR_HashTable_EMH_get_addr (0x3FFF & arguments->HASH.EMH);
  verblog  printf ("write to address:0x%lx\n", entry);
  arguments->TableID.EMH_HT = 0x3FFF & arguments->HASH.EMH;	//store id of entry in arguments structure
/*  if (entry != NULL) {
    entry->RULEPOINTER = 0x1ff & arguments.RULEPOINTER;
    entry->unused = 0xffffffffffffffff;		//trigger hardware
  }
  else
    printf ("Hash not valid\n");
*/
  struct INR_FC_EMH_HashTable_entry *entry_shadow =
    (struct INR_FC_EMH_HashTable_entry *) INR_HashTable_EMH_shadow_get_addr (0x3FFF & arguments->HASH.EMH);
  if (entry != NULL) {
    entry_shadow->RULEPOINTER = 0x1ff & arguments->RULEPOINTER;
    entry_shadow->unused = 0xffffffffffffffff;	//trigger hardware
  }else{
    verblog    printf ("Hash not valid\n");
    FCmemcpy (entry, entry_shadow, INR_FC_EMH_HashTable_entry_length_memcpy);	//copy shadow to mmi (wordwise)
  }
}

//********************************************************************************************************************
/**
*remove one entry from hashtable
@param arguments argumentes from userinterface
*/
void
HT_EMH_del (struct arguments arguments)
{
  verblog  printf ("__FUNCTION__ = %s\n", __FUNCTION__);
  verblog  printf ("Hash:0x%x\n", arguments.HASH.EMH);
  if (INR_HashTable_EMH_clear_entry (0x3fff & arguments.HASH.EMH)){
    verblog    printf ("Hash not valid\n");
  }
}

//********************************************************************************************************************
/**
*clear hashtable
@param arguments argumentes from userinterface
*/
void
HT_EMH_clear (struct arguments arguments)
{
  verblog  printf ("__FUNCTION__ = %s\n", __FUNCTION__);
  uint32_t i = 0;
  for (i = 0; i < INR_FC_EMH_HashTable_length; i++) {
    if (INR_HashTable_EMH_clear_entry (i)){
      verblog      printf ("Hash not valid\n");
    }
  }
}

//********************************************************************************************************************
/**
*print hashtable
@param arguments argumentes from userinterface
*/
void
HT_EMH_print (struct arguments arguments)
{
  verblog  printf ("__FUNCTION__ = %s\n", __FUNCTION__);
  uint32_t i = 0;
  if (arguments.COUNT == 0){
    arguments.COUNT = INR_FC_EMH_HashTable_length;
  }
  printf ("Print Hashtable from %i to %i.\n", arguments.ID, arguments.ID + arguments.COUNT);
  printf ("Hash\t|\tRulepointer\n");
  for (i = arguments.ID; ((i < INR_FC_EMH_HashTable_length) && (i < arguments.ID + arguments.COUNT)); i++) {
    struct INR_FC_EMH_HashTable_entry *entry = (struct INR_FC_EMH_HashTable_entry *) INR_HashTable_EMH_shadow_get_addr (i);
    if (entry != NULL){
      printf ("0x%x\t|\t0x%x\n", i, entry->RULEPOINTER);
    }else{
      verblog      printf ("ID not valid\n");
    }
  }
}

//********************************************************************************************************************
/**
*add entry to ruletable
@param arguments argumentes from userinterface
*/
void
RT_EMH_add (struct arguments *arguments)
{
  verblog  printf ("__FUNCTION__ = %s\n", __FUNCTION__);
  verblog  printf ("Adding rule to next free space up from ID:%i\n", arguments->ID);
  if(INR_RuleTable_EMH_get_next_free_entry (0xffff & arguments->ID)){
    struct INR_FC_EMH_RULE *entry =
      (struct INR_FC_EMH_RULE *) INR_RuleTable_EMH_get_addr (INR_RuleTable_EMH_get_next_free_entry (0xffff & arguments->ID));
    arguments->TableID.EMH_RT = INR_RuleTable_EMH_get_next_free_entry (0xffff & arguments->ID);	//store id fo entry in arguments structure
    //uint32_t *wordentry=INR_RuleTable_EMH_get_addr (INR_RuleTable_EMH_get_next_free_entry (0xffff & arguments->ID));
    struct INR_FC_EMH_RULE *entry_shadow =
      (struct INR_FC_EMH_RULE *) INR_RuleTable_EMH_shadow_get_addr (INR_RuleTable_EMH_get_next_free_entry (0xffff & arguments->ID));
    if (entry != NULL) {
      arguments->RULEPOINTER = INR_RuleTable_EMH_get_next_free_entry (0xffff & arguments->ID);	//save where rule is stored
      if (arguments->autoaction) {
      arguments->ACTION_ID = INR_ActT_get_next_free_entry (arguments->ID);
      AT_add (arguments);
      }
      /*entry->VALID_BIT = 1;
         entry->TYPE_ID = 0x3f & arguments->TYPE_ID;
         entry->PRIORITY = 0xff & arguments->PRIORITY;
         entry->ACTION_ID = 0xffff & arguments->ACTION_ID; */
      entry_shadow->VALID_BIT = 1;
      entry_shadow->TYPE_ID = 0x3f & arguments->TYPE_ID;	//& (1<<(arguments->TYPE_ID-1));
      entry_shadow->PRIORITY = 0xff & arguments->PRIORITY;
      entry_shadow->ACTION_ID = 0xffff & arguments->ACTION_ID;
      //entry->NEXT_BIT = 0x1 & arguments.NEXT_BIT;
      uint8_t j = 0;
      switch (arguments->TYPE_ID) {
      case 1:
        /*entry->COMP_FIELDS.TYPE1.MAC_SRC = 0xffffffffffff & arguments->MAC_SRC;
           entry->COMP_FIELDS.TYPE1.MAC_DST = 0xffffffffffff & arguments->MAC_DST;
           entry->COMP_FIELDS.TYPE1.ETHERTYPE=0xffff&arguments->ETHERTYPE;
           entry->COMP_FIELDS.TYPE1.VLAN_ID = 0xfff & arguments->VLAN_ID;
           for(uint8_t j=0;j<16;j++)entry->COMP_FIELDS.TYPE1.filling[j]=0xff;
           entry->COMP_FIELDS.TYPE1.unused = 0xffffffffffffffff;  //write MSB to trigger hardware
         */
        entry_shadow->COMP_FIELDS.TYPE1.MAC_SRC = arguments->MAC_SRC;
        entry_shadow->COMP_FIELDS.TYPE1.MAC_DST = arguments->MAC_DST;
        entry_shadow->COMP_FIELDS.TYPE1.ETHERTYPE = 0xffff & arguments->ETHERTYPE;
        entry_shadow->COMP_FIELDS.TYPE1.VLAN_ID = 0xfff & arguments->VLAN_ID;
        for (j=0; j < 16; j++){
          entry_shadow->COMP_FIELDS.TYPE1.filling[j] = 0xff;
        }
        entry_shadow->COMP_FIELDS.TYPE1.unused = 0xffffffffffffffff;	//write MSB to trigger hardware
        verblog      printf ("Type %i action written to 0x%lx id:%i\n", arguments->TYPE_ID, entry, arguments->RULEPOINTER);
        break;
      case 2:
        /*entry->COMP_FIELDS.TYPE2.IPv4_SRC = 0xffffffff & arguments->IPv4_SRC;
           entry->COMP_FIELDS.TYPE2.IPv4_DST = 0xffffffff & arguments->IPv4_DST;
           entry->COMP_FIELDS.TYPE2.PROTOCOL = 0xff & arguments->PROTOCOL;
           entry->COMP_FIELDS.TYPE2.PORT_SRC = 0xffff & arguments->PORT_SRC;
           entry->COMP_FIELDS.TYPE2.PORT_DST = 0xffff & arguments->PORT_DST;
           for(uint8_t j=0;j<19;j++)entry->COMP_FIELDS.TYPE2.filling[j]=0xff;
           entry->COMP_FIELDS.TYPE2.unused = 0xffffffffffffffff;  //write MSB to trigger hardware
         */
        entry_shadow->COMP_FIELDS.TYPE2.IPv4_SRC = 0xffffffff & arguments->IPv4_SRC;
        entry_shadow->COMP_FIELDS.TYPE2.IPv4_DST = 0xffffffff & arguments->IPv4_DST;
        entry_shadow->COMP_FIELDS.TYPE2.PROTOCOL = 0xff & arguments->PROTOCOL;
        entry_shadow->COMP_FIELDS.TYPE2.PORT_SRC = 0xffff & arguments->PORT_SRC;
        entry_shadow->COMP_FIELDS.TYPE2.PORT_DST = 0xffff & arguments->PORT_DST;
        for (j = 0; j < 19; j++){
          entry_shadow->COMP_FIELDS.TYPE2.filling[j] = 0xff;
        }
        entry_shadow->COMP_FIELDS.TYPE2.unused = 0xffffffffffffffff;	//write MSB to trigger hardware
        verblog      printf ("Type %i action written to 0x%lx id:%i\n", arguments->TYPE_ID, entry, arguments->RULEPOINTER);
        break;
      default:
        verblog      printf ("action type not valid\n");
        break;
      }
      FCmemcpy (entry, entry_shadow, INR_FC_EMH_RuleTable_entry_length_memcpy);	//copy shadow to mmi (wordwise)

      if (arguments->HASH.gauto) {
        verblog      printf ("automatic generating hashtable entry...\n");
        HT_EMH_add (arguments);
      }
    }
  }else{
    printf ("RuleTable EMH full\n");
  }
}

//********************************************************************************************************************
/**
*update entry in ruletable
@param arguments argumentes from userinterface
*/
void
RT_EMH_update (struct arguments arguments)
{
verblog  printf ("__FUNCTION__ = %s\n", __FUNCTION__);
verblog  printf ("updating rule on ID:%i\n", arguments.ID);
  struct INR_FC_EMH_RULE *entry = (struct INR_FC_EMH_RULE *) INR_RuleTable_EMH_get_addr (0xffff & arguments.ID);
  struct INR_FC_EMH_RULE *entry_shadow = (struct INR_FC_EMH_RULE *) INR_RuleTable_EMH_shadow_get_addr (0xffff & arguments.ID);
uint8_t j = 0;
  if (entry != NULL) {
    /*  entry->VALID_BIT = 1;
       entry->TYPE_ID = 0x3f & arguments.TYPE_ID;
       entry->PRIORITY = 0xff & arguments.PRIORITY;
       entry->ACTION_ID = 0xffff & arguments.ACTION_ID; */
    entry_shadow->VALID_BIT = 1;
    entry_shadow->TYPE_ID = 0x3f & arguments.TYPE_ID;
    entry_shadow->PRIORITY = 0xff & arguments.PRIORITY;
    entry_shadow->ACTION_ID = 0xffff & arguments.ACTION_ID;
//    entry->NEXT_BIT = 0x1 & arguments.NEXT_BIT;
    switch (arguments.TYPE_ID) {
    case 1:
      /* entry->COMP_FIELDS.TYPE1.MAC_SRC = 0xffffffffffff & arguments.MAC_SRC;
         entry->COMP_FIELDS.TYPE1.MAC_DST = 0xffffffffffff & arguments.MAC_DST;
         entry->COMP_FIELDS.TYPE1.ETHERTYPE=0xffff&arguments.ETHERTYPE;
         entry->COMP_FIELDS.TYPE1.VLAN_ID = 0xfff & arguments.VLAN_ID;
         for(uint8_t j=0;j<16;j++)entry->COMP_FIELDS.TYPE1.filling[j]=0xff;
         entry->COMP_FIELDS.TYPE1.unused = 0xffffffffffffffff;   //write MSB to trigger hardware
       */
      entry_shadow->COMP_FIELDS.TYPE1.MAC_SRC = arguments.MAC_SRC;
      entry_shadow->COMP_FIELDS.TYPE1.MAC_DST = arguments.MAC_DST;
      entry_shadow->COMP_FIELDS.TYPE1.ETHERTYPE = 0xffff & arguments.ETHERTYPE;
      entry_shadow->COMP_FIELDS.TYPE1.VLAN_ID = 0xfff & arguments.VLAN_ID;

      for (j = 0; j < 16; j++)
	entry_shadow->COMP_FIELDS.TYPE1.filling[j] = 0xff;
      entry_shadow->COMP_FIELDS.TYPE1.unused = 0xffffffffffffffff;	//write MSB to trigger hardware
verblog      printf ("Type %i action written to 0x%x\n", arguments.TYPE_ID, entry);
      break;
    case 2:
      /* entry->COMP_FIELDS.TYPE2.IPv4_SRC = 0xffffffff & arguments.IPv4_SRC;
         entry->COMP_FIELDS.TYPE2.IPv4_DST = 0xffffffff & arguments.IPv4_DST;
         entry->COMP_FIELDS.TYPE2.PROTOCOL = 0xff & arguments.PROTOCOL;
         entry->COMP_FIELDS.TYPE2.PORT_SRC = 0xffff & arguments.PORT_SRC;
         entry->COMP_FIELDS.TYPE2.PORT_DST = 0xffff & arguments.PORT_DST;
       */
      entry_shadow->COMP_FIELDS.TYPE2.IPv4_SRC = 0xffffffff & arguments.IPv4_SRC;
      entry_shadow->COMP_FIELDS.TYPE2.IPv4_DST = 0xffffffff & arguments.IPv4_DST;
      entry_shadow->COMP_FIELDS.TYPE2.PROTOCOL = 0xff & arguments.PROTOCOL;
      entry_shadow->COMP_FIELDS.TYPE2.PORT_SRC = 0xffff & arguments.PORT_SRC;
      entry_shadow->COMP_FIELDS.TYPE2.PORT_DST = 0xffff & arguments.PORT_DST;
      entry_shadow->COMP_FIELDS.TYPE2.unused = 0xffffffffffffffff;	//write MSB to trigger hardware
 verblog     printf ("Type %i action written to 0x%x\n", arguments.TYPE_ID, entry);
      break;
    default:
verblog      printf ("action type not valid\n");
      break;


    }
    FCmemcpy (entry, entry_shadow, INR_FC_EMH_RuleTable_entry_length_memcpy);	//copy shadow to mmi (wordwise)
  }
  else
    printf ("no free space\n");
}

//********************************************************************************************************************
/**
*del entry from ruletable
@param arguments argumentes from userinterface
*/
void
RT_EMH_del (struct arguments arguments)
{
verblog  printf ("__FUNCTION__ = %s\n", __FUNCTION__);
verblog  printf ("ID:%i\n", arguments.ID);
  if (INR_RuleTable_EMH_clear_entry (0xffff & arguments.ID))
verblog    printf ("id not valid\n");

}

//********************************************************************************************************************
/**
*clear ruletable
@param arguments argumentes from userinterface
*/
void
RT_EMH_clear (struct arguments arguments)
{
verblog  printf ("__FUNCTION__ = %s\n", __FUNCTION__);
  uint16_t i = 0;
  for (i = 0; i < INR_FC_EMH_RuleTable_length; i++) {
    if (INR_RuleTable_EMH_clear_entry (i))
verblog      printf ("ID not valid\n");
  }

}

//********************************************************************************************************************
/**
*print ruletable
@param arguments argumentes from userinterface
*/
void
RT_EMH_print (struct arguments arguments)
{
verblog  printf ("__FUNCTION__ = %s\n", __FUNCTION__);
  uint32_t i = 0;
  if (arguments.COUNT == 0)
    arguments.COUNT = INR_FC_EMH_RuleTable_length;
  printf ("Print Ruletable from %i to %i.\n", arguments.ID, arguments.ID + arguments.COUNT - 1);
  for (i = arguments.ID; ((i < INR_FC_EMH_RuleTable_length) && (i < arguments.ID + arguments.COUNT)); i++) {
    struct INR_FC_EMH_RULE *entry = (struct INR_FC_EMH_RULE *) INR_RuleTable_EMH_shadow_get_addr (0xffff & i);
    if (entry != NULL) {
      printf ("ID:%i  ", i);
      printf ("VALID_BIT:0x%x  ", entry->VALID_BIT);
      printf ("TYPE_ID:0x%x  ", entry->TYPE_ID);
      printf ("PRIORITY:0x%x  ", entry->PRIORITY);
      printf ("ACTION_ID:0x%lx  ", entry->ACTION_ID);
//      printf ("NEXT_BIT:0x%x  ", entry->NEXT_BIT);
      printf ("\n\t");
      switch (entry->TYPE_ID) {
      case 1:
	printf ("MAC_SRC:0x%lx  ", entry->COMP_FIELDS.TYPE1.MAC_SRC);
	printf ("MAC_DST:0x%lx  ", entry->COMP_FIELDS.TYPE1.MAC_DST);
	printf ("VLAN_ID:0x%lx  ", entry->COMP_FIELDS.TYPE1.VLAN_ID);
	printf ("Ethertype:0x%lx  ", entry->COMP_FIELDS.TYPE1.ETHERTYPE);
	break;
      case 2:
	printf ("IPv4_SRC:0x%lx  ", entry->COMP_FIELDS.TYPE2.IPv4_SRC);
	printf ("IPv4_DST:0x%lx  ", entry->COMP_FIELDS.TYPE2.IPv4_DST);
	printf ("PROTOCOL:0x%lx  ", entry->COMP_FIELDS.TYPE2.PROTOCOL);
	printf ("PORT_SRC:0x%lx  ", entry->COMP_FIELDS.TYPE2.PORT_SRC);
	printf ("PORT_DST:0x%lx  ", entry->COMP_FIELDS.TYPE2.PORT_DST);
	break;
      default:
	printf ("action type not valid\n");
	break;
      }
      printf ("\n");

    }
    else
      printf ("ID not valid\n");
  }
}

//********************************************************************************************************************
/**
*add entry to collisiontable
@param arguments argumentes from userinterface
*/
void
CT_EMH_add (struct arguments *arguments)
{
verblog  printf ("__FUNCTION__ = %s\n", __FUNCTION__);
verblog  printf ("Adding rule to next free space up from ID:%i\n", arguments->ID);
if(INR_CTable_EMH_get_next_free_entry (0xffff & arguments->ID)){
  struct INR_FC_EMH_RULE *entry = (struct INR_FC_EMH_RULE *) INR_CTable_EMH_get_addr (INR_CTable_EMH_get_next_free_entry (0xffff & arguments->ID));
  struct INR_FC_EMH_RULE *entry_shadow =
    (struct INR_FC_EMH_RULE *) INR_CTable_EMH_shadow_get_addr (INR_CTable_EMH_get_next_free_entry (0xffff & arguments->ID));
  if (entry != NULL) {
    if (arguments->autoaction) {
      arguments->ACTION_ID = INR_ActT_get_next_free_entry (arguments->ID);
      AT_add (arguments);
    }
    arguments->RULEPOINTER = INR_CTable_EMH_get_next_free_entry (0xffff & arguments->ID);	//save where rule is stored
    arguments->TableID.EMH_CT = arguments->RULEPOINTER;	//store table position in arguments structure
    /*   entry->VALID_BIT = 1;
       entry->TYPE_ID = 0x3f & arguments->TYPE_ID;
       entry->PRIORITY = 0xff & arguments->PRIORITY;
       entry->ACTION_ID = 0xffff & arguments->ACTION_ID; */
    entry_shadow->VALID_BIT = 1;
    entry_shadow->TYPE_ID = 0x3f & arguments->TYPE_ID;
    entry_shadow->PRIORITY = 0xff & arguments->PRIORITY;
    entry_shadow->ACTION_ID = 0xffff & arguments->ACTION_ID;
//    entry->NEXT_BIT = 0x1 & arguments.NEXT_BIT;
uint8_t j = 0;
    switch (arguments->TYPE_ID) {
    case 1:
      /*entry->COMP_FIELDS.TYPE1.MAC_SRC = 0xffffffffffff & arguments->MAC_SRC;
         entry->COMP_FIELDS.TYPE1.MAC_DST = 0xffffffffffff & arguments->MAC_DST;
         entry->COMP_FIELDS.TYPE1.ETHERTYPE=0xffff&arguments->ETHERTYPE;
         entry->COMP_FIELDS.TYPE1.VLAN_ID = 0xfff & arguments->VLAN_ID;
         for(uint8_t j=0;j<16;j++)entry->COMP_FIELDS.TYPE1.filling[j]=0xff;
         entry->COMP_FIELDS.TYPE1.unused = 0xffffffffffffffff;  //write MSB to trigger hardware
       */
      entry_shadow->COMP_FIELDS.TYPE1.MAC_SRC = arguments->MAC_SRC;
      entry_shadow->COMP_FIELDS.TYPE1.MAC_DST = arguments->MAC_DST;
      entry_shadow->COMP_FIELDS.TYPE1.ETHERTYPE = 0xffff & arguments->ETHERTYPE;
      entry_shadow->COMP_FIELDS.TYPE1.VLAN_ID = 0xfff & arguments->VLAN_ID;
      for (j = 0; j < 16; j++)
	entry_shadow->COMP_FIELDS.TYPE1.filling[j] = 0xff;
      entry_shadow->COMP_FIELDS.TYPE1.unused = 0xffffffffffffffff;	//write MSB to trigger hardware
verblog      printf ("Type %i action written to 0x%lx id:%i\n", arguments->TYPE_ID, entry, arguments->RULEPOINTER);
      break;
    case 2:
      /*entry->COMP_FIELDS.TYPE2.IPv4_SRC = 0xffffffff & arguments->IPv4_SRC;
         entry->COMP_FIELDS.TYPE2.IPv4_DST = 0xffffffff & arguments->IPv4_DST;
         entry->COMP_FIELDS.TYPE2.PROTOCOL = 0xff & arguments->PROTOCOL;
         entry->COMP_FIELDS.TYPE2.PORT_SRC = 0xffff & arguments->PORT_SRC;
         entry->COMP_FIELDS.TYPE2.PORT_DST = 0xffff & arguments->PORT_DST;
         for(uint8_t j=0;j<19;j++)entry->COMP_FIELDS.TYPE2.filling[j]=0xff;
         entry->COMP_FIELDS.TYPE2.unused = 0xffffffffffffffff;  //write MSB to trigger hardware
       */
      entry_shadow->COMP_FIELDS.TYPE2.IPv4_SRC = 0xffffffff & arguments->IPv4_SRC;
      entry_shadow->COMP_FIELDS.TYPE2.IPv4_DST = 0xffffffff & arguments->IPv4_DST;
      entry_shadow->COMP_FIELDS.TYPE2.PROTOCOL = 0xff & arguments->PROTOCOL;
      entry_shadow->COMP_FIELDS.TYPE2.PORT_SRC = 0xffff & arguments->PORT_SRC;
      entry_shadow->COMP_FIELDS.TYPE2.PORT_DST = 0xffff & arguments->PORT_DST;
      for (j = 0; j < 19; j++)
	entry_shadow->COMP_FIELDS.TYPE2.filling[j] = 0xff;
      entry_shadow->COMP_FIELDS.TYPE2.unused = 0xffffffffffffffff;	//write MSB to trigger hardware
verblog      printf ("Type %i action written to 0x%lx id:%i\n", arguments->TYPE_ID, entry, arguments->RULEPOINTER);
      break;
    default:
verblog      printf ("action type not valid\n");
      break;


    }
    FCmemcpy (entry, entry_shadow, INR_FC_EMH_CTable_entry_length_memcpy);	//copy shadow to mmi (wordwise)

  }}
  else
    printf ("Colliontable EMH full\n");
}

//********************************************************************************************************************
/**
*update entry in collisiontable
@param arguments argumentes from userinterface
*/
void
CT_EMH_update (struct arguments arguments)
{
verblog  printf ("__FUNCTION__ = %s\n", __FUNCTION__);
verblog  printf ("updating collision-rule on ID:%i\n", arguments.ID);
  struct INR_FC_EMH_RULE *entry = (struct INR_FC_EMH_RULE *) INR_CTable_EMH_get_addr (0xffff & arguments.ID);
  struct INR_FC_EMH_RULE *entry_shadow = (struct INR_FC_EMH_RULE *) INR_CTable_EMH_shadow_get_addr (0xffff & arguments.ID);
  if (entry != NULL) {
    /*   entry->VALID_BIT = 1;
       entry->TYPE_ID = 0x3f & arguments.TYPE_ID;
       entry->PRIORITY = 0xff & arguments.PRIORITY;
       entry->ACTION_ID = 0xffff & arguments.ACTION_ID; */
    entry_shadow->VALID_BIT = 1;
    entry_shadow->TYPE_ID = 0x3f & arguments.TYPE_ID;
    entry_shadow->PRIORITY = 0xff & arguments.PRIORITY;
    entry_shadow->ACTION_ID = 0xffff & arguments.ACTION_ID;
//    entry->NEXT_BIT = 0x1 & arguments.NEXT_BIT;
uint8_t j = 0;
    switch (arguments.TYPE_ID) {
    case 1:
      /*entry->COMP_FIELDS.TYPE1.MAC_SRC = 0xffffffffffff & arguments->MAC_SRC;
         entry->COMP_FIELDS.TYPE1.MAC_DST = 0xffffffffffff & arguments->MAC_DST;
         entry->COMP_FIELDS.TYPE1.ETHERTYPE=0xffff&arguments->ETHERTYPE;
         entry->COMP_FIELDS.TYPE1.VLAN_ID = 0xfff & arguments->VLAN_ID;
         for(uint8_t j=0;j<16;j++)entry->COMP_FIELDS.TYPE1.filling[j]=0xff;
         entry->COMP_FIELDS.TYPE1.unused = 0xffffffffffffffff;  //write MSB to trigger hardware
       */
      entry_shadow->COMP_FIELDS.TYPE1.MAC_SRC = arguments.MAC_SRC;
      entry_shadow->COMP_FIELDS.TYPE1.MAC_DST = arguments.MAC_DST;
      entry_shadow->COMP_FIELDS.TYPE1.ETHERTYPE = 0xffff & arguments.ETHERTYPE;
      entry_shadow->COMP_FIELDS.TYPE1.VLAN_ID = 0xfff & arguments.VLAN_ID;
      for (j = 0; j < 16; j++)
	entry_shadow->COMP_FIELDS.TYPE1.filling[j] = 0xff;
      entry_shadow->COMP_FIELDS.TYPE1.unused = 0xffffffffffffffff;	//write MSB to trigger hardware
verblog      printf ("Type %i action written to 0x%lx id:%i\n", arguments.TYPE_ID, entry, arguments.RULEPOINTER);
      break;
    case 2:
      /*entry->COMP_FIELDS.TYPE2.IPv4_SRC = 0xffffffff & arguments->IPv4_SRC;
         entry->COMP_FIELDS.TYPE2.IPv4_DST = 0xffffffff & arguments->IPv4_DST;
         entry->COMP_FIELDS.TYPE2.PROTOCOL = 0xff & arguments->PROTOCOL;
         entry->COMP_FIELDS.TYPE2.PORT_SRC = 0xffff & arguments->PORT_SRC;
         entry->COMP_FIELDS.TYPE2.PORT_DST = 0xffff & arguments->PORT_DST;
         for(uint8_t j=0;j<19;j++)entry->COMP_FIELDS.TYPE2.filling[j]=0xff;
         entry->COMP_FIELDS.TYPE2.unused = 0xffffffffffffffff;  //write MSB to trigger hardware
       */
      entry_shadow->COMP_FIELDS.TYPE2.IPv4_SRC = 0xffffffff & arguments.IPv4_SRC;
      entry_shadow->COMP_FIELDS.TYPE2.IPv4_DST = 0xffffffff & arguments.IPv4_DST;
      entry_shadow->COMP_FIELDS.TYPE2.PROTOCOL = 0xff & arguments.PROTOCOL;
      entry_shadow->COMP_FIELDS.TYPE2.PORT_SRC = 0xffff & arguments.PORT_SRC;
      entry_shadow->COMP_FIELDS.TYPE2.PORT_DST = 0xffff & arguments.PORT_DST;
      for (j = 0; j < 19; j++)
	entry_shadow->COMP_FIELDS.TYPE2.filling[j] = 0xff;
      entry_shadow->COMP_FIELDS.TYPE2.unused = 0xffffffffffffffff;	//write MSB to trigger hardware
verblog      printf ("Type %i action written to 0x%lx id:%i\n", arguments.TYPE_ID, entry, arguments.RULEPOINTER);
      break;
    default:
verblog      printf ("action type not valid\n");
      break;


    }
    FCmemcpy (entry, entry_shadow, INR_FC_EMH_CTable_entry_length_memcpy);	//copy shadow to mmi (wordwise)
  }
  else
verblog    printf ("no free space\n");
}

//********************************************************************************************************************
/**
*del entry from collisiontable
@param arguments argumentes from userinterface
*/
void
CT_EMH_del (struct arguments arguments)
{
verblog  printf ("__FUNCTION__ = %s\n", __FUNCTION__);
verblog  printf ("ID:%i\n", arguments.ID);
  if (INR_CTable_EMH_clear_entry (0xffff & arguments.ID))
verblog    printf ("id not valid\n");

}

//********************************************************************************************************************
/**
*clear collisiontable
@param arguments argumentes from userinterface
*/
void
CT_EMH_clear (struct arguments arguments)
{
verblog  printf ("__FUNCTION__ = %s\n", __FUNCTION__);
  uint16_t i = 0;
  for (i = 0; i < INR_FC_EMH_CTable_length; i++) {
    if (INR_CTable_EMH_clear_entry (i))
verblog      printf ("ID not valid\n");
  }

}

//********************************************************************************************************************
/**
*print collisiontable
@param arguments argumentes from userinterface
*/
void
CT_EMH_print (struct arguments arguments)
{
verblog  printf ("__FUNCTION__ = %s\n", __FUNCTION__);
  uint32_t i = 0;
  if (arguments.COUNT == 0)
    arguments.COUNT = INR_FC_EMH_CTable_length;
  printf ("Print Collisiontable from %i to %i.\n", arguments.ID, arguments.ID + arguments.COUNT - 1);
  for (i = arguments.ID; ((i < INR_FC_EMH_CTable_length) && (i < arguments.ID + arguments.COUNT)); i++) {
    struct INR_FC_EMH_RULE *entry = (struct INR_FC_EMH_RULE *) INR_CTable_EMH_shadow_get_addr (0xffff & i);
    if (entry != NULL) {
      printf ("ID:%i  ", i);
      printf ("VALID_BIT:0x%x  ", entry->VALID_BIT);
      printf ("TYPE_ID:0x%x  ", entry->TYPE_ID);
      printf ("PRIORITY:0x%x  ", entry->PRIORITY);
      printf ("ACTION_ID:0x%x  ", entry->ACTION_ID);
//      printf ("NEXT_BIT:0x%x  ", entry->NEXT_BIT);
      printf ("\n\t");
      switch (entry->TYPE_ID) {
      case 1:
	printf ("MAC_SRC:0x%x  ", entry->COMP_FIELDS.TYPE1.MAC_SRC);
	printf ("MAC_DST:0x%x  ", entry->COMP_FIELDS.TYPE1.MAC_DST);
	printf ("VLAN_ID:0x%x  ", entry->COMP_FIELDS.TYPE1.VLAN_ID);
	break;
      case 2:
	printf ("IPv4_SRC:0x%x  ", entry->COMP_FIELDS.TYPE2.IPv4_SRC);
	printf ("IPv4_DST:0x%x  ", entry->COMP_FIELDS.TYPE2.IPv4_DST);
	printf ("PROTOCOL:0x%x  ", entry->COMP_FIELDS.TYPE2.PROTOCOL);
	printf ("PORT_SRC:0x%x  ", entry->COMP_FIELDS.TYPE2.PORT_SRC);
	printf ("PORT_DST:0x%x  ", entry->COMP_FIELDS.TYPE2.PORT_DST);
	break;
      default:
	printf ("action type not valid\n");
	break;
      }
      printf ("\n");

    }
    else
      printf ("ID not valid\n");
  }
}

void
HT_EMA_add (struct arguments *arguments)
{
verblog  printf ("__FUNCTION__ = %s\n", __FUNCTION__);
  if (arguments->HASH.gauto) {
verblog    printf ("autogenhash...\n");
    get_HASH (arguments);
  }
  union INR_FC_EMA_HashTable_entry entry;
  entry.fields.din = arguments->HASH.EMA;
  entry.fields.mask.reserved = 1;
  entry.fields.mask.INGRESS_PORT = arguments->MASK_INPORT;
  entry.fields.mask.MAC_SRC = arguments->MASK_MAC_SRC;
  entry.fields.mask.MAC_DST = arguments->MASK_MAC_DST;
  entry.fields.mask.ETHERTYPE = arguments->MASK_ETHERTYPE;
  entry.fields.mask.VLAN_ID = arguments->MASK_VLAN_ID;
  entry.fields.mask.VLAN_PRIO = arguments->MASK_VLAN_PRIO;
  entry.fields.mask.IPv4_SRC = arguments->MASK_IPv4_SRC;
  entry.fields.mask.IPv4_DST = arguments->MASK_IPv4_DST;
  entry.fields.mask.PROTOCOL = arguments->MASK_PROTOCOL;
  entry.fields.mask.TOS = arguments->MASK_TOS;
  entry.fields.mask.PORT_SRC = arguments->MASK_PORT_SRC;
  entry.fields.mask.PORT_DST = arguments->MASK_PORT_DST;
  arguments->TableID.EMA_HT = INR_HashTable_EMA_get_next_free_entry (arguments->ID);	//store id of entry in arguments structure
  INR_HashT_EMA_write (entry, INR_HashTable_EMA_get_next_free_entry (arguments->ID));
}

void
HT_EMA_del (struct arguments arguments)
{
verblog  printf ("__FUNCTION__ = %s\n", __FUNCTION__);
  INR_HashTable_EMA_clear_entry (arguments.ID);
}

void
HT_EMA_clear (struct arguments arguments)
{

verblog  printf ("__FUNCTION__ = %s\n", __FUNCTION__);
  uint32_t i = 0;
  for (i = 0; i < INR_FC_EMA_TCAM_length; i++) {
    if (INR_HashTable_EMA_clear_entry (i))
verblog      printf ("Hash not valid\n");
  }
}

void
HT_EMA_print (struct arguments arguments)
{

verblog  printf ("__FUNCTION__ = %s\n", __FUNCTION__);
  uint32_t i = 0;
  if (arguments.COUNT == 0)
    arguments.COUNT = INR_FC_EMA_TCAM_length;
  printf ("Print EMA Hashtable from %i to %i.\n", arguments.ID, arguments.ID + arguments.COUNT - 1);
  union INR_FC_EMA_HashTable_entry *entry;
  for (i = arguments.ID; ((i < INR_FC_EMA_TCAM_length) && (i < arguments.ID + arguments.COUNT)); i++) {
    entry = INR_HashTable_EMA_read_entry (i);
    printf ("id:%i din:0x%lx%lx mask:0x%lx%lx\n", i, entry->words.dinH, entry->words.dinL, entry->words.maskH, entry->words.maskL);



  }
}

//********************************************************************************************************************
/**
*add entry to ruletable
@param arguments argumentes from userinterface
*/
void
RT_EMA_add (struct arguments *arguments)
{
verblog  printf ("__FUNCTION__ = %s\n", __FUNCTION__);
verblog  printf ("Adding rule to next free space up from ID:%i\n", arguments->ID);
if(INR_RuleTable_EMA_get_next_free_entry (0xffff & arguments->ID)){
  if (arguments->TYPE_ID == 0)
    RT_EMA_TYPE_override (arguments);
  struct INR_FC_EMA_RULE *entry =
    (struct INR_FC_EMA_RULE *) INR_RuleTable_EMA_get_addr (INR_RuleTable_EMA_get_next_free_entry (0xffff & arguments->ID));
  arguments->TableID.EMA_RT = INR_RuleTable_EMA_get_next_free_entry (0xffff & arguments->ID);	//store id fo entry in arguments structure
  struct INR_FC_EMA_RULE *entry_shadow =
    (struct INR_FC_EMA_RULE *) INR_RuleTable_EMA_shadow_get_addr (INR_RuleTable_EMA_get_next_free_entry (0xffff & arguments->ID));
  if (entry != NULL) {
    if (arguments->autoaction) {
      arguments->ACTION_ID = INR_ActT_get_next_free_entry (arguments->ID);
      AT_add (arguments);
    }
    arguments->RULEPOINTER = INR_RuleTable_EMA_get_next_free_entry (0xffff & arguments->ID);	//save where rule is stored
//    entry->VALID_BIT = 1;         /**<use this rule or not*/
//    entry->reserved = 0;
//    entry->TYPE_ID = 0xfff & arguments->TYPE_ID;        /**<type of rule*/
//    entry->PRIORITY = 0xff & arguments->PRIORITY;
//    entry->ACTION_ID = 0xffff & arguments->ACTION_ID;    /**<pointer to action table*/
//    entry->INGRESS_PORT = 0x1f & arguments->INPORT;
//    entry->MAC_SRC = 0xffffffffff & arguments->MAC_SRC;           /**<source MAC*/
//    entry->MAC_DST = 0xffffffffff & arguments->MAC_DST;           /**<destination MAC*/
//    entry->ETHERTYPE = 0xffff & arguments->ETHERTYPE;     /**<ETHERTYPE*/
//    entry->VLAN_ID = 0xfff & arguments->VLAN_ID;          /**<VLAN_ID*/
//    entry->VLAN_PRIO = 7 & arguments->VLAN_PRIO;     /**<VLAN_Priority*/
//    entry->IPv4_SRC = 0xffffffff & arguments->IPv4_SRC;         /**<IPv4 source address*/
//    entry->IPv4_DST = 0xffffffff & arguments->IPv4_DST;         /**<IPv4 destination address*/
//    entry->PROTOCOL = 0xff & arguments->PROTOCOL;       /**<layer 3 protocol*/
//    entry->TOS = 0x3f & arguments->TOS;
//    entry->PORT_SRC = 0xffff & arguments->PORT_SRC;     /**<TCP/UDP source port*/
//    entry->PORT_DST = 0xffff & arguments->PORT_DST;     /**<TCP/UDP destination port*/

    entry_shadow->VALID_BIT = 1;	    /**<use this rule or not*/
    entry_shadow->reserved = 0;
    entry_shadow->TYPE_ID = 0xfff & arguments->TYPE_ID;	  /**<type of rule*/
    entry_shadow->PRIORITY = 0xff & arguments->PRIORITY;
    entry_shadow->ACTION_ID = 0xffff & arguments->ACTION_ID;	   /**<pointer to action table*/
    entry_shadow->INGRESS_PORT = 0x1f & arguments->INPORT;
    entry_shadow->MAC_SRC = arguments->MAC_SRC;		    /**<source MAC*/
    entry_shadow->MAC_DST = arguments->MAC_DST;		    /**<destination MAC*/
    entry_shadow->ETHERTYPE = 0xffff & arguments->ETHERTYPE;	    /**<ETHERTYPE*/
    entry_shadow->VLAN_ID = 0xfff & arguments->VLAN_ID;	    /**<VLAN_ID*/
    entry_shadow->VLAN_PRIO = 7 & arguments->VLAN_PRIO;	    /**<VLAN_Priority*/
    entry_shadow->IPv4_SRC = 0xffffffff & arguments->IPv4_SRC;	  /**<IPv4 source address*/
    entry_shadow->IPv4_DST = 0xffffffff & arguments->IPv4_DST;	  /**<IPv4 destination address*/
    entry_shadow->PROTOCOL = 0xff & arguments->PROTOCOL;	  /**<layer 3 protocol*/
    entry_shadow->TOS = 0x3f & arguments->TOS;
    entry_shadow->PORT_SRC = 0xffff & arguments->PORT_SRC;	  /**<TCP/UDP source port*/
    entry_shadow->PORT_DST = 0xffff & arguments->PORT_DST;	  /**<TCP/UDP destination port*/

    FCmemcpy (entry, entry_shadow, INR_FC_EMA_RuleTable_entry_length_memcpy);	//copy shadow to mmi (wordwise)

    if (arguments->HASH.gauto) {
 verblog     printf ("automatic generating hashtable entry...\n");
      HT_EMA_add (arguments);
    }
  }}
  else
verblog    printf ("RuleTable EMA full\n");
}

//********************************************************************************************************************
/**
*calculates EMA rule type
@param arguments argumentes from userinterface
*/
void
RT_EMA_TYPE_override (struct arguments *arguments)
{
verblog  printf ("__FUNCTION__ = %s\n", __FUNCTION__);
  if (arguments->MASK_INPORT == 0)
    arguments->TYPE_ID |= (1 << ID_INPORT);
  if (arguments->MASK_MAC_SRC == 0)
    arguments->TYPE_ID |= (1 << ID_MAC_SRC);
  if (arguments->MASK_MAC_DST == 0)
    arguments->TYPE_ID |= (1 << ID_MAC_DST);
  if (arguments->MASK_ETHERTYPE == 0)
    arguments->TYPE_ID |= (1 << ID_ETHERTYPE);
  if (arguments->MASK_VLAN_ID == 0)
    arguments->TYPE_ID |= (1 << ID_VLAN_ID);
  if (arguments->MASK_VLAN_PRIO == 0)
    arguments->TYPE_ID |= (1 << ID_VLAN_PRIO);
  if (arguments->MASK_IPv4_SRC == 0)
    arguments->TYPE_ID |= (1 << ID_IPv4_SRC);
  if (arguments->MASK_IPv4_DST == 0)
    arguments->TYPE_ID |= (1 << ID_IPv4_DST);
  if (arguments->MASK_PROTOCOL == 0)
    arguments->TYPE_ID |= (1 << ID_PROTOCOL);
  if (arguments->MASK_TOS == 0)
    arguments->TYPE_ID |= (1 << ID_TOS);
  if (arguments->MASK_PORT_SRC == 0)
    arguments->TYPE_ID |= (1 << ID_PORT_SRC);
  if (arguments->MASK_PORT_DST == 0)
    arguments->TYPE_ID |= (1 << ID_PORT_DST);
verblog  printf ("new type:%x\n", arguments->TYPE_ID);
}

//********************************************************************************************************************
/**
*update entry in ruletable
@param arguments argumentes from userinterface
*/
void
RT_EMA_update (struct arguments arguments)
{
verblog  printf ("__FUNCTION__ = %s\n", __FUNCTION__);
verblog  printf ("updating rule on ID:%i\n", arguments.ID);
  if (arguments.TYPE_ID == 0)
    RT_EMA_TYPE_override (&arguments);
  struct INR_FC_EMA_RULE *entry = (struct INR_FC_EMA_RULE *) INR_RuleTable_EMA_get_addr (0xffff & arguments.ID);
  struct INR_FC_EMA_RULE *entry_shadow = (struct INR_FC_EMA_RULE *) INR_RuleTable_EMA_shadow_get_addr (0xffff & arguments.ID);
  if (entry != NULL) {
//    arguments.RULEPOINTER = INR_RuleTable_EMA_get_next_free_entry (0xffff & arguments.ID);    //save where rule is stored
//    entry->VALID_BIT = 1;         /**<use this rule or not*/
//    entry->reserved = 0;
//    entry->TYPE_ID = 0xfff & arguments.TYPE_ID;                 /**<type of rule*/
//    entry->PRIORITY = 0xff & arguments.PRIORITY;
//    entry->ACTION_ID = 0xffff & arguments.ACTION_ID;     /**<pointer to action table*/
//    entry->INGRESS_PORT = 0x1f & arguments.INPORT;
//    entry->MAC_SRC = 0xffffffffff & arguments.MAC_SRC;           /**<source MAC*/
//    entry->MAC_DST = 0xffffffffff & arguments.MAC_DST;           /**<destination MAC*/
//    entry->ETHERTYPE = 0xffff & arguments.ETHERTYPE;      /**<ETHERTYPE*/
//    entry->VLAN_ID = 0xfff & arguments.VLAN_ID;                   /**<VLAN_ID*/
//    entry->VLAN_PRIO = 7 & arguments.VLAN_PRIO;           /**<VLAN_Priority*/
//    entry->IPv4_SRC = 0xffffffff & arguments.IPv4_SRC;         /**<IPv4 source address*/
//    entry->IPv4_DST = 0xffffffff & arguments.IPv4_DST;         /**<IPv4 destination address*/
//    entry->PROTOCOL = 0xff & arguments.PROTOCOL;        /**<layer 3 protocol*/
//    entry->TOS = 0x3f & arguments.TOS;
//    entry->PORT_SRC = 0xffff & arguments.PORT_SRC;      /**<TCP/UDP source port*/
//    entry->PORT_DST = 0xffff & arguments.PORT_DST;      /**<TCP/UDP destination port*/

    entry_shadow->VALID_BIT = 1;	    /**<use this rule or not*/
    entry_shadow->reserved = 0;
    entry_shadow->TYPE_ID = 0xfff & arguments.TYPE_ID;	 /**<type of rule*/
    entry_shadow->PRIORITY = 0xff & arguments.PRIORITY;
    entry_shadow->ACTION_ID = 0xffff & arguments.ACTION_ID;	   /**<pointer to action table*/
    entry_shadow->INGRESS_PORT = 0x1f & arguments.INPORT;
    entry_shadow->MAC_SRC = arguments.MAC_SRC;	   /**<source MAC*/
    entry_shadow->MAC_DST = arguments.MAC_DST;	   /**<destination MAC*/
    entry_shadow->ETHERTYPE = 0xffff & arguments.ETHERTYPE;	    /**<ETHERTYPE*/
    entry_shadow->VLAN_ID = 0xfff & arguments.VLAN_ID;	   /**<VLAN_ID*/
    entry_shadow->VLAN_PRIO = 7 & arguments.VLAN_PRIO;	   /**<VLAN_Priority*/
    entry_shadow->IPv4_SRC = 0xffffffff & arguments.IPv4_SRC;	 /**<IPv4 source address*/
    entry_shadow->IPv4_DST = 0xffffffff & arguments.IPv4_DST;	 /**<IPv4 destination address*/
    entry_shadow->PROTOCOL = 0xff & arguments.PROTOCOL;		  /**<layer 3 protocol*/
    entry_shadow->TOS = 0x3f & arguments.TOS;
    entry_shadow->PORT_SRC = 0xffff & arguments.PORT_SRC;	  /**<TCP/UDP source port*/
    entry_shadow->PORT_DST = 0xffff & arguments.PORT_DST;	  /**<TCP/UDP destination port*/
    FCmemcpy (entry, entry_shadow, INR_FC_EMA_RuleTable_entry_length_memcpy);	//copy shadow to mmi (wordwise)
  }
  else
verblog    printf ("no free space\n");
}

//********************************************************************************************************************
/**
*del entry from ruletable
@param arguments argumentes from userinterface
*/
void
RT_EMA_del (struct arguments arguments)
{
verblog  printf ("__FUNCTION__ = %s\n", __FUNCTION__);
verblog  printf ("ID:%i\n", arguments.ID);
  if (INR_RuleTable_EMA_clear_entry (0xffff & arguments.ID))
verblog    printf ("id not valid\n");

}

//********************************************************************************************************************
/**
*clear ruletable
@param arguments argumentes from userinterface
*/
void
RT_EMA_clear (struct arguments arguments)
{
verblog  printf ("__FUNCTION__ = %s\n", __FUNCTION__);
  uint16_t i = 0;
  for (i = 0; i < INR_FC_EMA_RuleTable_length; i++) {
    if (INR_RuleTable_EMA_clear_entry (i))
verblog      printf ("ID not valid\n");
  }

}

//********************************************************************************************************************
/**
*print ruletable
@param arguments argumentes from userinterface
*/
void
RT_EMA_print (struct arguments arguments)
{
verblog  printf ("__FUNCTION__ = %s\n", __FUNCTION__);
  uint32_t i = 0;
  if (arguments.COUNT == 0)
    arguments.COUNT = INR_FC_EMA_RuleTable_length;
  printf ("Print Ruletable from %i to %i.\n", arguments.ID, arguments.ID + arguments.COUNT - 1);
  for (i = arguments.ID; ((i < INR_FC_EMA_RuleTable_length) && (i < arguments.ID + arguments.COUNT)); i++) {
    struct INR_FC_EMA_RULE *entry = (struct INR_FC_EMA_RULE *) INR_RuleTable_EMA_shadow_get_addr (0xffff & i);
    if (entry != NULL) {
      printf ("ID:%i  ", i);
      printf ("VALID_BIT:0x%x  ", entry->VALID_BIT);
      printf ("TYPE_ID:0x%x  ", entry->TYPE_ID);
      printf ("PRIORITY:0x%x  ", entry->PRIORITY);
      printf ("ACTION_ID:0x%x  ", entry->ACTION_ID);
      if (entry->TYPE_ID & (1 << ID_INPORT))
	printf (":0x%x  ", entry->INGRESS_PORT);
      if (entry->TYPE_ID & (1 << ID_MAC_SRC))
	printf (":0x%x  ", entry->MAC_SRC);
      if (entry->TYPE_ID & (1 << ID_MAC_DST))
	printf (":0x%x  ", entry->MAC_DST);
      if (entry->TYPE_ID & (1 << ID_ETHERTYPE))
	printf (":0x%x  ", entry->ETHERTYPE);
      if (entry->TYPE_ID & (1 << ID_VLAN_ID))
	printf (":0x%x  ", entry->VLAN_ID);
      if (entry->TYPE_ID & (1 << ID_VLAN_PRIO))
	printf (":0x%x  ", entry->VLAN_PRIO);
      if (entry->TYPE_ID & (1 << ID_IPv4_SRC))
	printf (":0x%x  ", entry->IPv4_SRC);
      if (entry->TYPE_ID & (1 << ID_IPv4_DST))
	printf (":0x%x  ", entry->IPv4_DST);
      if (entry->TYPE_ID & (1 << ID_PROTOCOL))
	printf (":0x%x  ", entry->PROTOCOL);
      if (entry->TYPE_ID & (1 << ID_TOS))
	printf (":0x%x  ", entry->TOS);
      if (entry->TYPE_ID & (1 << ID_PORT_SRC))
	printf (":0x%x  ", entry->PORT_SRC);
      if (entry->TYPE_ID & (1 << ID_PORT_DST))
	printf (":0x%x  ", entry->PORT_DST);
      printf ("\n");
    }
    else
      printf ("ID not valid\n");
  }
}

//********************************************************************************************************************
/**
*add entry to ruletable
@param arguments argumentes from userinterface
*/
void
AT_add (struct arguments *arguments)
{
verblog  printf ("__FUNCTION__ = %s\n", __FUNCTION__);
verblog  printf ("Adding rule to next free space up from ID:%i\n", arguments->ID);
if(INR_ActT_get_next_free_entry (0xffff & arguments->ID)){
  struct INR_FC_ActT_RULE *entry = (struct INR_FC_ActT_RULE *) INR_ActT_get_addr (INR_ActT_get_next_free_entry (0xffff & arguments->ID));
  struct INR_FC_ActT_RULE *entry_shadow =
    (struct INR_FC_ActT_RULE *) INR_ActT_shadow_get_addr (INR_ActT_get_next_free_entry (0xffff & arguments->ID));
  if (entry != NULL) {
    arguments->RULEPOINTER = INR_ActT_get_next_free_entry (0xffff & arguments->ID);	//save where rule is stored
    arguments->TableID.ActT = arguments->RULEPOINTER;	//store table position in arguments structure
    arguments->ACTION_ID=arguments->RULEPOINTER;
//entry->OutPort_enable=1 & arguments->OutPort_enable;/**<Outputport enable  */
//entry->OutPort=0x1f & arguments->OutPort;/**<outputport assignment  */
//entry->Bad_enable=1 & arguments->Bad_enable;/**<bad enable  */
//entry->BadValue=1 & arguments->BadValue;/**<bad assignment  */
//entry->BadReason=0x1f & arguments->BadReason;/**<bad reason  */
//entry->Cut_enable=1 & arguments->Cut_enable;/**<cut through enable  */
//entry->CutValue=1 & arguments->CutValue;/**<cut through assignment  */
//entry->unused=0xffffffffffffffff;
    entry_shadow->OutPort_enable = 1 & arguments->OutPort_enable;
							   /**<Outputport enable  */
    entry_shadow->OutPort = 0x1f & arguments->OutPort;
						/**<outputport assignment  */
    entry_shadow->Bad_enable = 1 & arguments->Bad_enable;
						   /**<bad enable  */
    entry_shadow->BadValue = 1 & arguments->BadValue;
					       /**<bad assignment  */
    entry_shadow->BadReason = 0x1f & arguments->BadReason;
						    /**<bad reason  */
    entry_shadow->Cut_enable = 1 & arguments->Cut_enable;
						   /**<cut through enable  */
    entry_shadow->CutValue = 1 & arguments->CutValue;
					       /**<cut through assignment  */
    entry_shadow->unused = 0xffffffffffffffff;
    FCmemcpy (entry, entry_shadow, INR_FC_ActT_entry_length_memcpy);	//copy shadow to mmi (wordwise)
  }}
  else printf ("ActionTable full\n");
}

//********************************************************************************************************************
/**
*update entry in ruletable
@param arguments argumentes from userinterface
*/
void
AT_update (struct arguments arguments)
{
verblog  printf ("__FUNCTION__ = %s\n", __FUNCTION__);
verblog  printf ("updating rule on ID:%i\n", arguments.ID);
  struct INR_FC_ActT_RULE *entry = (struct INR_FC_ActT_RULE *) INR_ActT_get_addr (0xffff & arguments.ID);
  struct INR_FC_ActT_RULE *entry_shadow = (struct INR_FC_ActT_RULE *) INR_ActT_shadow_get_addr (0xffff & arguments.ID);
  if (entry != NULL) {

//      entry->OutPort_enable=1 & arguments.OutPort_enable;/**<Outputport enable  */
//      entry->OutPort=0x1f & arguments.OutPort;/**<outputport assignment  */
//      entry->Bad_enable=1 & arguments.Bad_enable;/**<bad enable  */
//      entry->BadValue=1 & arguments.BadValue;/**<bad assignment  */
//      entry->BadReason=0x1f & arguments.BadReason;/**<bad reason  */
//      entry->Cut_enable=1 & arguments.Cut_enable;/**<cut through enable  */
//      entry->CutValue=1 & arguments.CutValue;/**<cut through assignment  */
//      entry->unused=0xffffffffffffffff;;
    entry_shadow->OutPort_enable = 1 & arguments.OutPort_enable;  /**<Outputport enable  */
    entry_shadow->OutPort = 0x1f & arguments.OutPort;  /**<outputport assignment  */
    entry_shadow->Bad_enable = 1 & arguments.Bad_enable;  /**<bad enable  */
    entry_shadow->BadValue = 1 & arguments.BadValue;  /**<bad assignment  */
    entry_shadow->BadReason = 0x1f & arguments.BadReason;  /**<bad reason  */
    entry_shadow->Cut_enable = 1 & arguments.Cut_enable;  /**<cut through enable  */
    entry_shadow->CutValue = 1 & arguments.CutValue;  /**<cut through assignment  */
    FCmemcpy (entry, entry_shadow, INR_FC_ActT_entry_length_memcpy);	//copy shadow to mmi (wordwise)
  }
  else
verblog    printf ("no free space\n");
}

//********************************************************************************************************************
/**
*del entry from ruletable
@param arguments argumentes from userinterface
*/
void
AT_del (struct arguments arguments)
{
verblog  printf ("__FUNCTION__ = %s\n", __FUNCTION__);
verblog  printf ("ID:%i\n", arguments.ID);
  if (INR_ActT_clear_entry (0xffff & arguments.ID))
verblog    printf ("id not valid\n");

}

//********************************************************************************************************************
/**
*clear ruletable
@param arguments argumentes from userinterface
*/
void
AT_clear (struct arguments arguments)
{
verblog  printf ("__FUNCTION__ = %s\n", __FUNCTION__);
  uint16_t i = 0;
  for (i = 0; i < INR_FC_ActT_length; i++) {
    if (INR_ActT_clear_entry (i))
verblog      printf ("ID not valid\n");
  }

}

//********************************************************************************************************************
/**
*print ruletable
@param arguments argumentes from userinterface
*/
void
AT_print (struct arguments arguments)
{
verblog  printf ("__FUNCTION__ = %s\n", __FUNCTION__);
  uint32_t i = 0;
  if (arguments.COUNT == 0)
    arguments.COUNT = INR_FC_ActT_length;
  printf ("Print Ruletable from %i to %i.\n", arguments.ID, arguments.ID + arguments.COUNT - 1);
  for (i = arguments.ID; ((i < INR_FC_ActT_length) && (i < arguments.ID + arguments.COUNT)); i++) {
    struct INR_FC_ActT_RULE *entry = (struct INR_FC_ActT_RULE *) INR_ActT_shadow_get_addr (0xffff & i);
    if (entry != NULL) {
      printf ("ID:%i  ", i);
      printf ("OutPort_enable:%i  ", entry->OutPort_enable);
      printf ("OutPort:0x%x  ", entry->OutPort);
      printf ("Bad_enable:%i  ", entry->Bad_enable);
      printf ("BadValue:%i  ", entry->BadValue);
      printf ("BadReason:0x%x  ", entry->BadReason);
      printf ("CutTrough_enable:%i  ", entry->Cut_enable);
      printf ("CutTroughValue:%i  ", entry->CutValue);
      printf ("\n");
    }
    else
      printf ("ID not valid\n");
  }
}
