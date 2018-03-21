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

#include "tnlibflowtable.h"
#include "flowtableactions.h"
#include "tnlibaccdp.h"
#include "accdpactions.h"

//********************************************************************************************************************
/**
*add entry to AccelerationDatapath
@param arguments argumentes from userinterface
*/
void
ACCDP_add (struct arguments *arguments)
{
	verblog printf ("__FUNCTION__ = %s\n", __FUNCTION__);
	verblog printf ("Adding rule to next free space up from ID:%i\n", arguments->ID);
	if (INR_ACCDP_get_next_free_entry (0xffff & arguments->ID)) {
		struct INR_ACCDP_RULE *entry = (struct INR_ACCDP_RULE *) INR_ACCDP_get_addr (INR_ACCDP_get_next_free_entry (0xffff & arguments->ID));
		struct INR_ACCDP_RULE *entry_shadow =
		  (struct INR_ACCDP_RULE *) INR_ACCDP_shadow_get_addr (INR_ACCDP_get_next_free_entry (0xffff & arguments->ID));
		if (entry != NULL) {
			arguments->RULEPOINTER = INR_ACCDP_get_next_free_entry (0xffff & arguments->ID);	//save where rule is stored
			arguments->TableID.ACCDP = arguments->RULEPOINTER;	//store table position in arguments structure
			if(arguments->numberout)printf("DP_ID:%i\n",arguments->TableID.ACCDP);
			entry_shadow->enable = 1;	//Outputport enable 
			entry_shadow->reserved=0;
			entry_shadow->IPv4_12lsb=0xfff&arguments->IPv4_DST;
			entry_shadow->UDP_DST=0xffff&arguments->PORT_DST;
			entry_shadow->NAL_ID=0x3f&arguments->NAL_ID;
			entry_shadow->pad1=0;
			entry_shadow->pad2=0;
			entry_shadow->pad3=0;
			entry_shadow->pad4=0;
			FCmemcpy (INR_ACCDP_get_addr(0), INR_ACCDP_shadow_get_addr(0), INR_ACCDP_entry_length_memcpy*INR_ACCDP_length);	//copy shadow to mmi (wordwise)
		}
	}	else {
		if(arguments->numberout)printf("DP_ID:%i\n",0);
	}
}



//********************************************************************************************************************
/**
*update entry in ruletable
@param arguments argumentes from userinterface
*/
void
ACCDP_update (struct arguments arguments)
{
	verblog printf ("__FUNCTION__ = %s\n", __FUNCTION__);
	verblog printf ("updating rule on ID:%i\n", arguments.ID);
}

//********************************************************************************************************************
/**
*del entry from AccelerationDatapath
@param arguments argumentes from userinterface
*/
void
ACCDP_del (struct arguments arguments)
{
	verblog printf ("__FUNCTION__ = %s\n", __FUNCTION__);
	verblog printf ("ID:%i\n", arguments.ID);
	if (INR_ACCDP_clear_entry (0xffff & arguments.ID)) {
		verblog printf ("id not valid\n");
	}
}

//********************************************************************************************************************
/**
*clear AccelerationDatapath
@param arguments argumentes from userinterface
*/
void
ACCDP_clear ()
{
	verblog printf ("__FUNCTION__ = %s\n", __FUNCTION__);
	uint16_t i = 0;
	for (i = 0; i < INR_ACCDP_length; i++) {
		if (INR_ACCDP_clear_entry (i)) {
			verblog printf ("ID not valid\n");
		}
	}
}

//********************************************************************************************************************
/**
*print AccelerationDatapath
@param arguments argumentes from userinterface
*/
void
ACCDP_print (struct arguments arguments)
{
	verblog  printf ("__FUNCTION__ = %s\n", __FUNCTION__);
	uint32_t i = 0;
	if (arguments.COUNT == 0) {
		arguments.COUNT = INR_ACCDP_length;
	}
	printf ("Print ACCDP from %i to %i.\n", arguments.ID, arguments.ID + arguments.COUNT - 1);
	for (i = arguments.ID; ((i < INR_ACCDP_length) && (i < arguments.ID + arguments.COUNT)); i++) {
		struct INR_ACCDP_RULE *entry = (struct INR_ACCDP_RULE *) INR_ACCDP_shadow_get_addr (0xffff & i);
		if (entry != NULL) {
			printf ("ID:%i  ", i);
			printf ("enable:%i  ", entry->enable);
			printf ("IPv4_12lsb:0x%x  ", entry->IPv4_12lsb);
			printf ("UDP_DST:0x%x  ", entry->UDP_DST);
			printf ("NAL_ID:0x%x  ", entry->NAL_ID);
			printf ("\n");
		} else {
			printf ("ID not valid\n");
		}
	}

}
