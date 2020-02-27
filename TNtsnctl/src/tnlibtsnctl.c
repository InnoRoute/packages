/**
*@file tnlibtsnctl.c
*@brief userspace TSN acces tool
*M.Ulbricht 2018
**/

#include <inttypes.h>
#include <stddef.h>
#include <stdio.h>
#include <endian.h>
#include <time.h>
#include <unistd.h>

#include "tnlibtsnctl.h"
uint8_t TN_tsn_verbose = 0;
uint64_t BASE = 0;
uint64_t BASE_shadow = 0;
uint64_t BASE_shadow2 = 0;
uint8_t TN_tsn_MEMDUMP = 0;
uint8_t TSN_enable=1;
uint8_t TSN_PORT_count=0;
uint8_t PORT_count=0;

uint32_t	TASvar_TSN_config_space_lower=0;
uint32_t	TASvar_PORT_TSN_width=0;
uint32_t	TASvar_GateControl_list_base=0;
uint32_t	TASvar_GateControl_list_entry_length=0;
uint32_t	TASvar_GateControl_list_length=0;
uint32_t	TASvar_GateControl_TIME_list_base=0;
uint32_t	TASvar_GateControl_TIME_list_entry_length=0;
uint32_t	TASvar_GateControl_TIME_list_length=0;
uint32_t	TASvar_QUEUE_PRIO_list_base=0;
uint32_t	TASvar_QUEUE_PRIO_list_entry_length=0;
uint32_t	TASvar_QUEUE_PRIO_list_length=0;
uint32_t	TASvar_C_SUB_ADDR_TM_SCHED_TAS_ADMIN_GCL_LEN=0;
uint32_t  TASvar_C_SUB_ADDR_TM_SCHED_TAS_ADMIN_BASE_TIME=0;
uint32_t  TASvar_C_SUB_ADDR_TM_SCHED_TAS_ADMIN_CYCLE_TIME=0;
uint32_t  TASvar_C_SUB_ADDR_TM_SCHED_TAS_ADMIN_CYCLE_TIME_EXT=0;
uint32_t  TASvar_C_SUB_ADDR_TM_SCHED_TAS_CONFIG_CHANGE_TIME=0;
uint32_t  TASvar_C_SUB_ADDR_TM_SCHED_TAS_CYCLE_START_TIME=0;
uint32_t  TASvar_C_SUB_ADDR_TM_SCHED_TAS_GATE_ENABLE=0;
uint32_t  TASvar_C_SUB_ADDR_TM_SCHED_TAS_CONFIG_CHANGE=0;
uint32_t  TASvar_C_SUB_ADDR_TM_SCHED_TAS_CONFIG_CHANGE_PENDING=0;

uint32_t  TASvar_C_SUB_ADDR_TM_SCHED_TAS_OPER_GCL_LEN=0;
uint32_t  TASvar_C_SUB_ADDR_TM_SCHED_TAS_OPER_BASE_TIME=0;
uint32_t  TASvar_C_SUB_ADDR_TM_SCHED_TAS_OPER_CYCLE_TIME=0;
uint32_t  TASvar_C_SUB_ADDR_TM_SCHED_TAS_OPER_CYCLE_TIME_EXT=0;
uint32_t  TASvar_C_SUB_ADDR_TM_SCHED_TAS_OPER_GATE_STATES=0;
uint32_t 	TASvar_C_SUB_ADDR_TM_SCHED_TAS_CUR_TIME=0;
uint32_t 	TASvar_C_SUB_ADDR_TM_SCHED_TAS_ADMIN_GATE_STATES=0;
uint8_t H_tas=0, INR_TAS=0;


//********************************************************************************************************************
/**
*get TSN port count
*/
uint8_t get_TSN_PORT_count(){

return TSN_PORT_count;
}

//********************************************************************************************************************
/**
*set default values to arguments structure
@param arguments argumentes from userinterface
*/
void
TN_tsn_clear_arguments (struct arguments *arguments)
{
  arguments->args[0] = "";
  arguments->verbose = 0;
  arguments->used = 0;
  arguments->numberout = 0;
  arguments->machinereadable = 0;
  arguments->ID = 0;
  arguments->hilscher_mode=0;
  arguments->hilscher_mode2=0;
  arguments->COUNT = 0;
  arguments->ADMIN_GCL_LEN = 0;
  arguments->ADMIN_BASE_TIME = 0;
  arguments->ADMIN_CYCLE_TIME = 0;
  arguments->ADMIN_CYCLE_TIME_EXT = 0;
  arguments->CONFIG_CHANGE_TIME = 0;
  arguments->bulk=0;
  arguments->CYCLE_START_TIME = 0;
  arguments->GATE_ENABLE = 0;
  arguments->CONFIG_CHANGE = 0;
  arguments->CONFIG_CHANGE_PENDING = 0;
  arguments->CONFIG_CHANGE_ACK = 0;
  arguments->OPER_GCL_LEN = 0;
  arguments->OPER_BASE_TIME = 0;
  arguments->OPER_CYCLE_TIME = 0;
  arguments->OPER_CYCLE_TIME_EXT = 0;
  arguments->INTERVAL = 0;
  arguments->PORT = 0;
  arguments->GATE_STATE_VECTOR = 0;
  arguments->QUEUE_PRIO = 0;
  arguments->ADMIN_GATE_STATES = 0;

  arguments->dohave_ID = 0;
  arguments->dohave_COUNT = 0;
  arguments->dohave_ADMIN_GCL_LEN = 0;
  arguments->dohave_ADMIN_BASE_TIME = 0;
  arguments->dohave_ADMIN_CYCLE_TIME = 0;
  arguments->dohave_ADMIN_CYCLE_TIME_EXT = 0;
  arguments->dohave_CONFIG_CHANGE_TIME = 0;
  arguments->dohave_CYCLE_START_TIME = 0;
  arguments->dohave_GATE_ENABLE = 0;
  arguments->dohave_CONFIG_CHANGE = 0;
  arguments->dohave_CONFIG_CHANGE_PENDING = 0;
  arguments->dohave_CONFIG_CHANGE_ACK = 0;
  arguments->dohave_OPER_GCL_LEN = 0;
  arguments->dohave_OPER_BASE_TIME = 0;
  arguments->dohave_OPER_CYCLE_TIME = 0;
  arguments->dohave_OPER_CYCLE_TIME_EXT = 0;
  arguments->dohave_INTERVAL = 0;
  arguments->dohave_PORT = 0;
  arguments->dohave_GATE_STATE_VECTOR = 0;
  arguments->dohave_QUEUE_PRIO = 0;
  arguments->dohave_ADMIN_GATE_STATES = 0;

}

//************************************************************************************************************************************
/**
*get TSN enable
*@param verbose value
*/

uint8_t get_TSN_enable(){
	return TSN_enable;

}
//************************************************************************************************************************************
/**
*refresh tas settings to hardware
*
*/
void TSN_refresh (){
if (TSN_enable) {
	//TN_tsn_set_verbose (1);
	if (TN_tsn_verbose)
    printf ("info: %s:%d:%s \n", __FILE__, __LINE__, __func__,get_TSN_PORT_count());
	uint8_t port=0;
	uint16_t gcl_entry=0;
	for(port=0;port<get_TSN_PORT_count();port++){
		if(TSN_get_config(TASvar_C_SUB_ADDR_TM_SCHED_TAS_GATE_ENABLE,port,0)){
		if (TN_tsn_verbose)printf("refresh GCL %i %i\n",port,gcl_entry);
		TSN_set_config(TASvar_C_SUB_ADDR_TM_SCHED_TAS_ADMIN_GCL_LEN,port,TSN_get_config(TASvar_C_SUB_ADDR_TM_SCHED_TAS_ADMIN_GCL_LEN,port,0));
		for(gcl_entry=0;gcl_entry<TSN_get_config(TASvar_C_SUB_ADDR_TM_SCHED_TAS_ADMIN_GCL_LEN,port,0);gcl_entry++){
					if (TN_tsn_verbose)printf("refresh GCL %i %i\n",port,gcl_entry);
			    struct GateControl_list_entry *entry =
     		 		BASE + port * TASvar_PORT_TSN_width + TASvar_GateControl_list_base + TASvar_GateControl_list_entry_length * gcl_entry;
			    struct GateControl_list_entry *entry_shadow =
 				     BASE_shadow + port * TASvar_PORT_TSN_width + TASvar_GateControl_list_base + TASvar_GateControl_list_entry_length * gcl_entry;
			    TSNmemcpy (entry, entry_shadow, TASvar_GateControl_list_entry_length);
			    
					struct GateControl_TIME_list_entry *Tentry =
     			 BASE + port * TASvar_PORT_TSN_width + TASvar_GateControl_TIME_list_base + TASvar_GateControl_TIME_list_entry_length * gcl_entry;
    			struct GateControl_TIME_list_entry *Tentry_shadow =
     			 BASE_shadow + port * TASvar_PORT_TSN_width + TASvar_GateControl_TIME_list_base + TASvar_GateControl_TIME_list_entry_length * gcl_entry;
     			 
     			struct granularitytime *correcttime =
      		BASE_shadow2 + port * TASvar_PORT_TSN_width * 2 + sizeof(struct granularitytime) + sizeof(struct granularitytime) * gcl_entry;
   				 if ((entry_shadow != NULL)&&(correcttime != NULL)) {
    				*correcttime=TSN_time_ganularity(Tentry_shadow->time);
    			}
    			TSNmemcpy (Tentry, Tentry_shadow, TASvar_GateControl_TIME_list_entry_length);
		
		
		}
		struct arguments arguments;
		TN_tsn_clear_arguments (&arguments);
		arguments.PORT=port;
		arguments.hilscher_mode2=1;
		arguments.ADMIN_BASE_TIME=TSN_get_config(TASvar_C_SUB_ADDR_TM_SCHED_TAS_ADMIN_BASE_TIME,port,1);
		arguments.dohave_ADMIN_BASE_TIME=1;
		arguments.ADMIN_CYCLE_TIME=TSN_get_config(TASvar_C_SUB_ADDR_TM_SCHED_TAS_ADMIN_CYCLE_TIME,port,0);
		arguments.dohave_ADMIN_CYCLE_TIME=1;
		arguments.ADMIN_CYCLE_TIME_EXT=TSN_get_config(TASvar_C_SUB_ADDR_TM_SCHED_TAS_ADMIN_CYCLE_TIME_EXT,port,0);
		arguments.dohave_ADMIN_CYCLE_TIME_EXT=1;
		arguments.GATE_ENABLE=TSN_get_config(TASvar_C_SUB_ADDR_TM_SCHED_TAS_GATE_ENABLE,port,0);
		arguments.dohave_GATE_ENABLE=1;
		arguments.dohave_PORT=1;
		new_TSN_apply (&arguments);


	}else {
	if (TN_tsn_verbose)printf("port %i disabled\n",port);
	}
	
	}
}
  

}
//************************************************************************************************************************************
/**
*set verbode
*@param verbose value
*/
void
TN_tsn_printallconst ()
{
#ifndef __KERNEL__		//not allowed for kmods
  printf ("Compiled %s %s\n", __DATE__, __TIME__);

#endif

  printconst (TASvar_GateControl_list_length);
  printconst (TASvar_GateControl_list_entry_length);
  printconst (TASvar_GateControl_list_base);
  printconst (TASvar_GateControl_TIME_list_length);
  printconst (TASvar_GateControl_TIME_list_entry_length);
  printconst (TASvar_GateControl_TIME_list_base);
  printconst (TASvar_QUEUE_PRIO_list_length);
  printconst (TASvar_QUEUE_PRIO_list_entry_length);
  printconst (TASvar_QUEUE_PRIO_list_base);
  printconst (TASvar_TSN_config_space_lower);
  printconst (get_TSN_PORT_count());
  printconst (TASvar_PORT_TSN_width);
  
  printconst (TASvar_C_SUB_ADDR_TM_SCHED_TAS_OPER_CYCLE_TIME);
  printconst (TASvar_C_SUB_ADDR_TM_SCHED_TAS_OPER_CYCLE_TIME_EXT);
  printconst (TASvar_C_SUB_ADDR_TM_SCHED_TAS_OPER_BASE_TIME);
  
  printconst (TASvar_C_SUB_ADDR_TM_SCHED_TAS_ADMIN_CYCLE_TIME);
  printconst (TASvar_C_SUB_ADDR_TM_SCHED_TAS_ADMIN_CYCLE_TIME_EXT);
  printconst (TASvar_C_SUB_ADDR_TM_SCHED_TAS_ADMIN_BASE_TIME);




};

//************************************************************************************************************************************
/**
*replacement of memcopy to guarantee wordwise memory access
*
*/
void
TSNmemcpy (void *dst, const void *src, size_t len)
{
  size_t i;
  uint32_t *d = dst;
  uint32_t *s = src;
  
    for (i = 0; i < len / sizeof (uint32_t); i++) {
      d[i] = s[i];
#ifndef __KERNEL__
      if (TN_tsn_MEMDUMP) {
	
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
TN_tsn_get_verbose ()
{
  return TN_tsn_verbose;
};


//************************************************************************************************************************************
/**
*dynamically set addresses for several TAS Implementations
*/
void
TSN_set_addresses()
{
  
  uint32_t *val = BASE + (C_BASE_ADDR_COMMON_LOWER<<8)+C_SUB_ADDR_COMMON_FEATURES_USER;
  H_tas=(*val&0x1);
#ifdef C_SUB_ADDR_COMMON_FEATURES_TM
  val=BASE + (C_BASE_ADDR_COMMON_LOWER<<8)+C_SUB_ADDR_COMMON_FEATURES_TM;
  INR_TAS=(*val&0x1);
#else
  INR_TAS=0;
#endif
  printf ("TAS version: %i.%i\n",H_tas,INR_TAS);
  if(INR_TAS && H_tas){
  	printf ("Error: two TAS versions active, disabling TAS.\n");
  	TSN_enable=0;
  }else TSN_enable=INR_TAS|H_tas;
  
if(H_tas){
	TSN_PORT_count=((0xff0000&INR_PCI_BAR1_read((C_BASE_ADDR_COMMON_LOWER<<8)+C_SUB_ADDR_COMMON_FEATURES_USER))>>16);
	TASvar_TSN_config_space_lower=(C_BASE_ADDR_TM_SCHED_LOWER<<8);
	TASvar_PORT_TSN_width=C_BLOCK_SIZE_ADDR_TM_SCHED;
	TASvar_GateControl_list_base=(C_SUB_ADDR_TM_SCHED_GCL_LOWER<<0)+(TASvar_TSN_config_space_lower);
	TASvar_GateControl_list_entry_length=4;
	TASvar_GateControl_list_length=((((C_SUB_ADDR_TM_SCHED_GCL_UPPER+1)<<0)-(C_SUB_ADDR_TM_SCHED_GCL_LOWER<<0))/TASvar_GateControl_list_entry_length);
	TASvar_GateControl_TIME_list_base=(C_SUB_ADDR_TM_SCHED_GCL_TIME_LOWER<<0)+(TASvar_TSN_config_space_lower);
	TASvar_GateControl_TIME_list_entry_length=4;
	TASvar_GateControl_TIME_list_length=((((C_SUB_ADDR_TM_SCHED_GCL_TIME_UPPER+1)<<0)-(C_SUB_ADDR_TM_SCHED_GCL_TIME_LOWER<<0))/TASvar_GateControl_TIME_list_entry_length);
	TASvar_QUEUE_PRIO_list_base=(C_SUB_ADDR_TM_SCHED_PROC_QUEUE_PRIO_LOWER<<0)+(TASvar_TSN_config_space_lower);
	TASvar_QUEUE_PRIO_list_entry_length=4;
	TASvar_QUEUE_PRIO_list_length=((((C_SUB_ADDR_TM_SCHED_PROC_QUEUE_PRIO_UPPER+1)<<0)-(C_SUB_ADDR_TM_SCHED_PROC_QUEUE_PRIO_LOWER<<0))/TASvar_QUEUE_PRIO_list_entry_length);
	TASvar_C_SUB_ADDR_TM_SCHED_TAS_ADMIN_GCL_LEN=C_SUB_ADDR_TM_SCHED_TAS_ADMIN_GCL_LEN;
  TASvar_C_SUB_ADDR_TM_SCHED_TAS_ADMIN_BASE_TIME=C_SUB_ADDR_TM_SCHED_TAS_ADMIN_BASE_TIME;
  TASvar_C_SUB_ADDR_TM_SCHED_TAS_ADMIN_CYCLE_TIME=C_SUB_ADDR_TM_SCHED_TAS_ADMIN_CYCLE_TIME;
  TASvar_C_SUB_ADDR_TM_SCHED_TAS_ADMIN_CYCLE_TIME_EXT=C_SUB_ADDR_TM_SCHED_TAS_ADMIN_CYCLE_TIME_EXT;
  TASvar_C_SUB_ADDR_TM_SCHED_TAS_CONFIG_CHANGE_TIME=C_SUB_ADDR_TM_SCHED_TAS_CONFIG_CHANGE_TIME;
  TASvar_C_SUB_ADDR_TM_SCHED_TAS_CYCLE_START_TIME=C_SUB_ADDR_TM_SCHED_TAS_CYCLE_START_TIME;
  TASvar_C_SUB_ADDR_TM_SCHED_TAS_GATE_ENABLE=C_SUB_ADDR_TM_SCHED_TAS_GATE_ENABLE;
  TASvar_C_SUB_ADDR_TM_SCHED_TAS_CONFIG_CHANGE=C_SUB_ADDR_TM_SCHED_TAS_CONFIG_CHANGE;
  TASvar_C_SUB_ADDR_TM_SCHED_TAS_CONFIG_CHANGE_PENDING=C_SUB_ADDR_TM_SCHED_TAS_CONFIG_CHANGE_PENDING;
  
  TASvar_C_SUB_ADDR_TM_SCHED_TAS_OPER_GCL_LEN=C_SUB_ADDR_TM_SCHED_TAS_OPER_GCL_LEN;
  TASvar_C_SUB_ADDR_TM_SCHED_TAS_OPER_BASE_TIME=C_SUB_ADDR_TM_SCHED_TAS_OPER_BASE_TIME;
  TASvar_C_SUB_ADDR_TM_SCHED_TAS_OPER_CYCLE_TIME=C_SUB_ADDR_TM_SCHED_TAS_OPER_CYCLE_TIME;
  TASvar_C_SUB_ADDR_TM_SCHED_TAS_OPER_CYCLE_TIME_EXT=C_SUB_ADDR_TM_SCHED_TAS_OPER_CYCLE_TIME_EXT;
  TASvar_C_SUB_ADDR_TM_SCHED_TAS_OPER_GATE_STATES=C_SUB_ADDR_TM_SCHED_TAS_OPER_GATE_STATES;
   TASvar_C_SUB_ADDR_TM_SCHED_TAS_ADMIN_GATE_STATES=C_SUB_ADDR_TM_SCHED_TAS_ADMIN_GATE_STATES;
  #ifdef C_SUB_ADDR_TM_SCHED_TAS_CUR_TIME
  	TASvar_C_SUB_ADDR_TM_SCHED_TAS_CUR_TIME=C_SUB_ADDR_TM_SCHED_TAS_CUR_TIME;
  #endif
 } 
 if(INR_TAS){
#ifdef C_SUB_ADDR_SCHED_TAS_GCL_MEM_GATES
#ifdef C_SUB_ADDR_COMMON_FEATURES_TM
	TSN_PORT_count=((0xff0000&INR_PCI_BAR1_read((C_BASE_ADDR_COMMON_LOWER<<8)+C_SUB_ADDR_COMMON_FEATURES_TM))>>16);
#endif
 	TASvar_TSN_config_space_lower=(C_BASE_ADDR_TM_SCHED_LOWER<<8);
	TASvar_PORT_TSN_width=C_BLOCK_SIZE_ADDR_TM_SCHED;
	TASvar_GateControl_list_base=(C_SUB_ADDR_SCHED_TAS_GCL_MEM_GATES<<0)+(TASvar_TSN_config_space_lower);
	TASvar_GateControl_list_entry_length=4;
	TASvar_GateControl_list_length=((((C_SUB_ADDR_TM_SCHED_GCL_UPPER+1)<<0)-(C_SUB_ADDR_TM_SCHED_GCL_LOWER<<0))/TASvar_GateControl_list_entry_length);
	TASvar_GateControl_TIME_list_base=(C_SUB_ADDR_SCHED_TAS_GCL_MEM_TIMES<<0)+(TASvar_TSN_config_space_lower);
	TASvar_GateControl_TIME_list_entry_length=4;
	TASvar_GateControl_TIME_list_length=((((C_SUB_ADDR_TM_SCHED_GCL_TIME_UPPER+1)<<0)-(C_SUB_ADDR_TM_SCHED_GCL_TIME_LOWER<<0))/TASvar_GateControl_TIME_list_entry_length);
	TASvar_QUEUE_PRIO_list_base=(C_SUB_ADDR_TM_SCHED_PROC_QUEUE_PRIO_LOWER<<0)+(TASvar_TSN_config_space_lower);
	TASvar_QUEUE_PRIO_list_entry_length=4;
	TASvar_QUEUE_PRIO_list_length=((((C_SUB_ADDR_TM_SCHED_PROC_QUEUE_PRIO_UPPER+1)<<0)-(C_SUB_ADDR_TM_SCHED_PROC_QUEUE_PRIO_LOWER<<0))/TASvar_QUEUE_PRIO_list_entry_length);
	TASvar_C_SUB_ADDR_TM_SCHED_TAS_ADMIN_GCL_LEN=C_SUB_ADDR_SCHED_TAS_ADMIN_CONTROL_LIST_LENGTH;
  TASvar_C_SUB_ADDR_TM_SCHED_TAS_ADMIN_BASE_TIME=C_SUB_ADDR_SCHED_TAS_ADMIN_BASE_TIME;
  TASvar_C_SUB_ADDR_TM_SCHED_TAS_ADMIN_CYCLE_TIME=C_SUB_ADDR_SCHED_TAS_ADMIN_CYCLE_TIME;
  TASvar_C_SUB_ADDR_TM_SCHED_TAS_ADMIN_CYCLE_TIME_EXT=C_SUB_ADDR_SCHED_TAS_ADMIN_CYCLE_TIME_EXTENSION;
  TASvar_C_SUB_ADDR_TM_SCHED_TAS_CONFIG_CHANGE_TIME=C_SUB_ADDR_SCHED_TAS_CONFIG_CHANGE;
  TASvar_C_SUB_ADDR_TM_SCHED_TAS_CYCLE_START_TIME=0;// not used for this tas
  TASvar_C_SUB_ADDR_TM_SCHED_TAS_GATE_ENABLE=C_SUB_ADDR_SCHED_TAS_GATE_ENABLED;
  TASvar_C_SUB_ADDR_TM_SCHED_TAS_CONFIG_CHANGE=C_SUB_ADDR_SCHED_TAS_CONFIG_CHANGE;
  TASvar_C_SUB_ADDR_TM_SCHED_TAS_CONFIG_CHANGE_PENDING=C_SUB_ADDR_SCHED_TAS_CONFIG_PENDING;

  TASvar_C_SUB_ADDR_TM_SCHED_TAS_OPER_GCL_LEN=0; // not used
  TASvar_C_SUB_ADDR_TM_SCHED_TAS_OPER_BASE_TIME=C_SUB_ADDR_SCHED_TAS_OPER_BASE_TIME;
  TASvar_C_SUB_ADDR_TM_SCHED_TAS_OPER_CYCLE_TIME=C_SUB_ADDR_SCHED_TAS_OPER_CYCLE_TIME;
  TASvar_C_SUB_ADDR_TM_SCHED_TAS_OPER_CYCLE_TIME_EXT=C_SUB_ADDR_SCHED_TAS_OPER_CYCLE_TIME_EXTENSION;
  TASvar_C_SUB_ADDR_TM_SCHED_TAS_OPER_GATE_STATES=C_SUB_ADDR_SCHED_TAS_OPER_GATE_STATES;
   TASvar_C_SUB_ADDR_TM_SCHED_TAS_ADMIN_GATE_STATES=C_SUB_ADDR_SCHED_TAS_ADMIN_GATE_STATES;
  #ifdef C_SUB_ADDR_TM_SCHED_TAS_CUR_TIME
  	TASvar_C_SUB_ADDR_TM_SCHED_TAS_CUR_TIME=C_SUB_ADDR_TM_SCHED_TAS_CUR_TIME;
  #endif
#endif 
 
 
 
 
 
 
 }
  
};

//************************************************************************************************************************************
/**
*get verbode
*/
void
TSN_init (uint64_t * base, uint64_t * shadow_base,uint64_t * shadow_base2)
{
  BASE = (uint64_t) base;
  BASE_shadow = (uint64_t) shadow_base;
  BASE_shadow2 = (uint64_t) shadow_base2;
  verblog printf ("Base address bar1:0x%llx, base address_shadow:0x%llx\n", BASE, BASE_shadow,BASE_shadow2);
  TSN_set_addresses();
};

//************************************************************************************************************************************
/**
*set verbode
*@param verbose value
*/
void
TN_tsn_set_verbose (uint8_t i)
{
  TN_tsn_verbose = i;
};

//********************************************************************************************************************
/**
*
@param arguments argumentes from userinterface
*/
/*
void AdminCTL_list_add(struct arguments *arguments){
printf("info: %s:%d:%s \n", __FILE__, __LINE__,__func__);

};*/
//********************************************************************************************************************
/**
*
@param arguments argumentes from userinterface
*/
/*
void AdminCTL_list_del(struct arguments *arguments){
printf("info: %s:%d:%s \n", __FILE__, __LINE__,__func__);

};*/
//********************************************************************************************************************
/**
*
@param arguments argumentes from userinterface
*/
/*
void AdminCTL_list_print(struct arguments *arguments){
printf("info: %s:%d:%s \n", __FILE__, __LINE__,__func__);

};*/
//********************************************************************************************************************
/**
*
@param arguments argumentes from userinterface
*/
/*void OperCTL_list_add(struct arguments *arguments){
printf("info: %s:%d:%s \n", __FILE__, __LINE__,__func__);

};*/
//********************************************************************************************************************
/**
*
@param arguments argumentes from userinterface
*/
/*void OperCTL_list_del(struct arguments *arguments){
printf("info: %s:%d:%s \n", __FILE__, __LINE__,__func__);

};*/
//********************************************************************************************************************
/**
*
@param arguments argumentes from userinterface
*/
/*void OperCTL_list_print(struct arguments *arguments){
printf("info: %s:%d:%s \n", __FILE__, __LINE__,__func__);

};
*/
//********************************************************************************************************************
/**
*
@param arguments argumentes from userinterface
*/
void
TN_tsn_config_change (struct arguments *arguments)
{
  if (TN_tsn_verbose)
    printf ("info: %s:%d:%s \n", __FILE__, __LINE__, __func__);
  verblog printf ("Wait until pending config is done\n");
    while (TSN_get_config (TASvar_C_SUB_ADDR_TM_SCHED_TAS_CONFIG_CHANGE_PENDING, arguments->PORT, 0)) {
      POLLSLEEP}
  if (TSN_enable) {
    TSN_config_dohavewrite (arguments, ADMIN_GCL_LEN)
      TSN_config_dohavewrite_time (arguments, ADMIN_BASE_TIME)
      TSN_config_dohavewrite_time (arguments, ADMIN_CYCLE_TIME)
      TSN_config_dohavewrite_time (arguments, ADMIN_CYCLE_TIME_EXT)
      TSN_config_dohavewrite_time (arguments, CONFIG_CHANGE_TIME)
      TSN_config_dohavewrite_time (arguments, CYCLE_START_TIME)
      TSN_config_dohavewrite (arguments, GATE_ENABLE)
      TSN_config_dohavewrite (arguments, CONFIG_CHANGE)
      TSN_config_dohavewrite (arguments, ADMIN_GATE_STATES)
//readonly!: TSN_config_dohavewrite(arguments,CONFIG_CHANGE_PENDING)
//readonly!: TSN_config_dohavewrite(arguments,CONFIG_CHANGE_ACK)
//readonly!: TSN_config_dohavewrite(arguments,OPER_GCL_LEN)
//readonly!: TSN_config_dohavewrite(arguments,OPER_BASE_TIME)
//readonly!: TSN_config_dohavewrite(arguments,OPER_CYCLE_TIME)
//readonly!: TSN_config_dohavewrite(arguments,OPER_CYCLE_TIME_EXT)
  }
};

//********************************************************************************************************************
/**
*
@param arguments argumentes from userinterface
*/
void
config_print (struct arguments *arguments)
{
  if (TN_tsn_verbose)
    printf ("info: %s:%d:%s \n", __FILE__, __LINE__, __func__);
  if (TSN_enable) {
    TSN_config_print (ADMIN_GCL_LEN, 0)
      TSN_config_print (ADMIN_BASE_TIME, 0)
      TSN_config_print (ADMIN_CYCLE_TIME, 0)
      TSN_config_print (ADMIN_CYCLE_TIME_EXT, 0)
      TSN_config_print (CONFIG_CHANGE_TIME, 0)
      TSN_config_print (CYCLE_START_TIME, 0)
      TSN_config_print (GATE_ENABLE, 0)
      TSN_config_print (CONFIG_CHANGE, 0)
      TSN_config_print (CONFIG_CHANGE_PENDING, 0)
      TSN_config_print (OPER_GCL_LEN, 0)
      TSN_config_print (OPER_BASE_TIME, 0)
      TSN_config_print (OPER_CYCLE_TIME, 0)
      TSN_config_print (OPER_CYCLE_TIME_EXT, 0)
      TSN_config_print (OPER_GATE_STATES, 0)
#ifdef C_SUB_ADDR_TM_SCHED_TAS_CUR_TIME
      TSN_config_print (CUR_TIME, 0)
#endif
      //TSN_config_print (ADMIN_GATE_STATES, 0)
  }
};
//********************************************************************************************************************
/**
*
@param arguments argumentes from userinterface
*/
void
GCL_entry (struct arguments *arguments)
{
  if (TN_tsn_verbose)
    printf ("info: %s:%d:%s \n", __FILE__, __LINE__, __func__);
    
  if(arguments->PORT>=TSN_PORT_count){
		   printf ("The bitstream doen't provide a TAS for Port %i'\n",arguments->PORT);
		   return;
   }  
  if (TSN_enable) {
    if (arguments->dohave_ID == 0 || arguments->ID >= TASvar_GateControl_list_length) {
      printf ("please provide a valid entry ID!\n");
      return;
    }
    if (arguments->dohave_GATE_STATE_VECTOR == 0) {
      printf ("please provide a valid entry GATE_STATE_VECTOR!\n");
      return;
    }
    if (arguments->dohave_PORT == 0) {
      printf ("please provide a Port number!\n");
      return;
    }
    if (arguments->ID >= TASvar_GateControl_list_length) {
      printf ("ID out of range!\n");
      return;
    }
    if (arguments->dohave_INTERVAL == 0) {
      printf ("please provide a valid entry INTERVAL!\n");
      return;
    }
    TN_tsn_config_change (arguments);//write config
    
    
    if (TSN_get_config (TASvar_C_SUB_ADDR_TM_SCHED_TAS_ADMIN_GCL_LEN, arguments->PORT, 0)<=arguments->ID) {
      printf ("Provided ID(%li) is beyond the GateControlList length (%li)\n",arguments->ID,TSN_get_config (TASvar_C_SUB_ADDR_TM_SCHED_TAS_ADMIN_GCL_LEN, arguments->PORT, 0));
      return;
    }
    TN_tsn_GateControl_list_change(arguments);
    TN_tsn_GateControl_TIME_list_change(arguments);
    
}}
//********************************************************************************************************************
/**
*
@param arguments argumentes from userinterface
*/
void
TN_tsn_GateControl_list_change (struct arguments *arguments)
{
  if (TN_tsn_verbose)
    printf ("info: %s:%d:%s \n", __FILE__, __LINE__, __func__);
  if (TSN_enable) {
    if (arguments->dohave_ID == 0 || arguments->ID >= TASvar_GateControl_list_length) {
      printf ("please provide a valid entry ID!\n");
      return;
    }
    if (arguments->dohave_GATE_STATE_VECTOR == 0) {
      printf ("please provide a valid entry GATE_STATE_VECTOR!\n");
      return;
    }
    if (arguments->ID >= TASvar_GateControl_list_length) {
      printf ("ID out of range!\n");
      return;
    }
    if (TN_tsn_verbose)
      printf ("accessing address:0x%llx\n", arguments->PORT * TASvar_PORT_TSN_width + TASvar_GateControl_list_base + TASvar_GateControl_list_entry_length * arguments->ID);
    struct GateControl_list_entry *entry =
      BASE + arguments->PORT * TASvar_PORT_TSN_width + TASvar_GateControl_list_base + TASvar_GateControl_list_entry_length * arguments->ID;
    /*if (entry != NULL) {
       entry->GCL=arguments->GATE_STATE_VECTOR;
       } */
    struct GateControl_list_entry *entry_shadow =
      BASE_shadow + arguments->PORT * TASvar_PORT_TSN_width + TASvar_GateControl_list_base + TASvar_GateControl_list_entry_length * arguments->ID;
    if (entry_shadow != NULL) {
      entry_shadow->GCL = arguments->GATE_STATE_VECTOR;
    }
    TSNmemcpy (entry, entry_shadow, TASvar_GateControl_list_entry_length);
		TN_tsn_config_change(arguments);
  }
}

//********************************************************************************************************************
/**
*
@param arguments argumentes from userinterface
*/
void
GateControl_list_print (struct arguments *arguments)
{
  if (TN_tsn_verbose)
    printf ("info: %s:%d:%s \n", __FILE__, __LINE__, __func__);
  if (TSN_enable) {
    uint32_t i = 0;
    if (arguments->COUNT == 0) {
      arguments->COUNT = TASvar_GateControl_list_length;
    }
    not_MACHINEREADABLE printf ("GateControl list for port %i from %i to %i.\n", arguments->PORT, arguments->ID,
				arguments->ID + arguments->COUNT - 1);
    MACHINEREADABLE printf ("{");
    for (i = arguments->ID; ((i < TASvar_GateControl_list_length) && (i < arguments->ID + arguments->COUNT)); i++) {
      if (TN_tsn_verbose)
	printf ("accessing address[%i]:0x%llx\n", i, arguments->PORT * TASvar_PORT_TSN_width + TASvar_GateControl_list_base + TASvar_GateControl_list_entry_length * i);
      struct GateControl_list_entry *entry =
	BASE_shadow + arguments->PORT * TASvar_PORT_TSN_width + TASvar_GateControl_list_base + TASvar_GateControl_list_entry_length * i;
      if (entry != NULL) {
	MACHINEREADABLE {
	  printf ("0x%x,", entry->GCL);
	}
	else
	printf ("%i|0x%x\n", i, entry->GCL);
      }
    }
    MACHINEREADABLE printf ("}");

  }
}


//********************************************************************************************************************
/**
*
@param arguments argumentes from userinterface
*/
struct granularitytime get_TSN_granularitytime(uint8_t port, uint32_t ID){
	struct granularitytime retval;
	struct granularitytime *correcttime = BASE_shadow2 + port * TASvar_PORT_TSN_width * 2 + sizeof(struct granularitytime) + sizeof(struct granularitytime) * ID;
	retval=*correcttime;
	verblog printf ("read time entry %i, value:%lli, error:%lli\n",ID,correcttime->correcttime,correcttime->error_ns);
  return retval;
}
//********************************************************************************************************************
/**
*
@param arguments argumentes from userinterface
*/
void
TN_tsn_GateControl_TIME_list_change (struct arguments *arguments)
{
  
  
  if (TN_tsn_verbose)
    printf ("info: %s:%d:%s \n", __FILE__, __LINE__, __func__);
  if (TSN_enable) {
    if (arguments->dohave_ID == 0) {
      printf ("please provide a valid entry ID!\n");
      return;
    }
    if (arguments->ID >= TASvar_GateControl_TIME_list_length) {
      printf ("ID out of range!\n");
      return;
    }
    if (arguments->dohave_INTERVAL == 0) {
      printf ("please provide a valid entry INTERVAL!\n");
      return;
    }
    if (TN_tsn_verbose)
      printf ("accessing address:0x%llx\n",
	      arguments->PORT * TASvar_PORT_TSN_width + TASvar_GateControl_TIME_list_base + TASvar_GateControl_TIME_list_entry_length * arguments->ID);
    struct GateControl_TIME_list_entry *entry =
      BASE + arguments->PORT * TASvar_PORT_TSN_width + TASvar_GateControl_TIME_list_base + TASvar_GateControl_TIME_list_entry_length * arguments->ID;
    /*if (entry != NULL) {
       entry->time=arguments->INTERVAL;
       } */
    struct GateControl_TIME_list_entry *entry_shadow =
      BASE_shadow + arguments->PORT * TASvar_PORT_TSN_width + TASvar_GateControl_TIME_list_base + TASvar_GateControl_TIME_list_entry_length * arguments->ID;
    struct granularitytime *correcttime =
      BASE_shadow2 + arguments->PORT * TASvar_PORT_TSN_width * 2 + sizeof(struct granularitytime) + sizeof(struct granularitytime) * arguments->ID;
    if ((entry_shadow != NULL)&&(correcttime != NULL)) {
    	*correcttime=TSN_time_ganularity(arguments->INTERVAL);
      entry_shadow->time = correcttime->correcttime;
    }
    TSNmemcpy (entry, entry_shadow, TASvar_GateControl_TIME_list_entry_length);
  }
}

//********************************************************************************************************************
/**
*
@param arguments argumentes from userinterface
*/
void
GateControl_TIME_list_print (struct arguments *arguments)
{
  if (TN_tsn_verbose)
    printf ("info: %s:%d:%s \n", __FILE__, __LINE__, __func__);
  if (TSN_enable) {
    uint32_t i = 0;
    if (arguments->COUNT == 0) {
      arguments->COUNT = TASvar_GateControl_TIME_list_length;
    }
    not_MACHINEREADABLE printf ("GateControl TIME for port %i list from %i to %i.\n", arguments->PORT, arguments->ID,
				arguments->ID + arguments->COUNT - 1);
    MACHINEREADABLE printf ("{");
    for (i = arguments->ID; ((i < TASvar_GateControl_TIME_list_length) && (i < arguments->ID + arguments->COUNT)); i++) {
      if (TN_tsn_verbose)
	printf ("accessing address[%i]:0x%llx\n", i,
		arguments->PORT * TASvar_PORT_TSN_width + TASvar_GateControl_TIME_list_base + TASvar_GateControl_TIME_list_entry_length * i);
      struct GateControl_TIME_list_entry *entry =
	BASE_shadow + arguments->PORT * TASvar_PORT_TSN_width + TASvar_GateControl_TIME_list_base + TASvar_GateControl_TIME_list_entry_length * i;
      if (entry != NULL) {
	MACHINEREADABLE printf ("0x%lx,", entry->time);
	else
	printf ("%i|0x%lx\n", i, entry->time);
      }
    }
    MACHINEREADABLE printf ("}");
  }
}

//********************************************************************************************************************
/**
*
@param arguments argumentes from userinterface
*/
void
QUEUE_PRIO_list_change (struct arguments *arguments)
{
  if (TN_tsn_verbose)
    printf ("info: %s:%d:%s \n", __FILE__, __LINE__, __func__);
  if (TSN_enable) {
    if (arguments->dohave_ID == 0) {
      printf ("please provide a valid entry ID!\n");
      return;
    }
    if (arguments->ID >= TASvar_QUEUE_PRIO_list_length) {
      printf ("ID out of range!\n");
      return;
    }
    if (arguments->dohave_QUEUE_PRIO == 0) {
      printf ("please provide a valid QUEUE_PRIO!\n");
      return;
    }
    if (TN_tsn_verbose)
      printf ("accessing address:0x%llx\n", arguments->PORT * TASvar_PORT_TSN_width + TASvar_QUEUE_PRIO_list_base + TASvar_QUEUE_PRIO_list_entry_length * arguments->ID);
    struct QUEUE_PRIO_list_entry *entry =
      BASE + arguments->PORT * TASvar_PORT_TSN_width + TASvar_QUEUE_PRIO_list_base + TASvar_QUEUE_PRIO_list_entry_length * arguments->ID;
    /*if (entry != NULL) {
       entry->prio=arguments->QUEUE_PRIO;
       } */
    struct QUEUE_PRIO_list_entry *entry_shadow =
      BASE_shadow + arguments->PORT * TASvar_PORT_TSN_width + TASvar_QUEUE_PRIO_list_base + TASvar_QUEUE_PRIO_list_entry_length * arguments->ID;
    if (entry_shadow != NULL) {
      entry_shadow->prio = arguments->QUEUE_PRIO;
    }
    TSNmemcpy (entry, entry_shadow, TASvar_QUEUE_PRIO_list_entry_length);
  }
}

//********************************************************************************************************************
/**
*
@param arguments argumentes from userinterface
*/
void
QUEUE_PRIO_list_print (struct arguments *arguments)
{
  if (TN_tsn_verbose)
    printf ("info: %s:%d:%s \n", __FILE__, __LINE__, __func__);
  if (TSN_enable) {
    uint32_t i = 0;
    if (arguments->COUNT == 0) {
      arguments->COUNT = TASvar_QUEUE_PRIO_list_length;
    }
    MACHINEREADABLE printf ("{");
    not_MACHINEREADABLE printf ("QUEUE_PRIO for port %i list from %i to %i.\n", arguments->PORT, arguments->ID, arguments->ID + arguments->COUNT - 1);
    for (i = arguments->ID; ((i < TASvar_QUEUE_PRIO_list_length) && (i < arguments->ID + arguments->COUNT)); i++) {
      if (TN_tsn_verbose)
	printf ("accessing address[%i]:0x%llx\n", i, arguments->PORT * TASvar_PORT_TSN_width + TASvar_QUEUE_PRIO_list_base + TASvar_QUEUE_PRIO_list_entry_length * i);
      struct QUEUE_PRIO_list_entry *entry = BASE_shadow + arguments->PORT * TASvar_PORT_TSN_width + TASvar_QUEUE_PRIO_list_base + TASvar_QUEUE_PRIO_list_entry_length * i;
      if (entry != NULL) {
	MACHINEREADABLE printf ("0x%lx,", entry->prio);
	else
	printf ("%i|0x%lx\n", i, entry->prio);
      }
    }
    MACHINEREADABLE printf ("}");
  }
}

//********************************************************************************************************************
/**
*
@param reg register of config in port config space
@param port portnumber
@param read from shadow memory
*/
uint32_t
TSN_get_config (uint64_t reg, uint8_t port, uint8_t shadow)
{
  if (TSN_enable) {
    
    uint64_t base = 0;
    if (shadow)
      base = BASE_shadow;
    else
      base = BASE;
    uint32_t *val = base + port * TASvar_PORT_TSN_width + TASvar_TSN_config_space_lower + reg;
	if (TN_tsn_verbose)
      printf ("reading address:0x%llx value:0x%llx\n", port * TASvar_PORT_TSN_width + TASvar_TSN_config_space_lower + reg,*val);
      if(TN_tsn_MEMDUMP)printf("TNbar1 0x%lx w \n",port * TASvar_PORT_TSN_width + TASvar_TSN_config_space_lower + reg);
    return *val;
  }
}
//********************************************************************************************************************
/**
*
@param reg register of config in port config space
@param port portnumber
@param read from shadow memory
*/
uint32_t
TSN_get_config2 (uint64_t reg, uint8_t port, uint8_t shadow)
{
  if (TSN_enable) {
    
    uint64_t base = 0;
   
      base = BASE_shadow2;
    
    uint32_t *val = base + port * TASvar_PORT_TSN_width + TASvar_TSN_config_space_lower + reg;
	if (TN_tsn_verbose)
      printf ("reading address:0x%llx value:0x%llx\n", port * TASvar_PORT_TSN_width + TASvar_TSN_config_space_lower + reg,*val);
      if(TN_tsn_MEMDUMP)printf("TNbar1 0x%lx w \n",port * TASvar_PORT_TSN_width + TASvar_TSN_config_space_lower + reg);
    return *val;
  }
}
//********************************************************************************************************************
/**
*
@param reg register of config in port config space
@param port portnumber
@param value value to write
*/
uint32_t
TSN_set_config(uint64_t reg, uint8_t port, uint32_t value)
{
  if (TSN_enable) {
    if (TN_tsn_verbose)
      printf ("writing to address:0x%llx value:0x%llx\n", port * TASvar_PORT_TSN_width + TASvar_TSN_config_space_lower + reg,value);
      if(TN_tsn_MEMDUMP)printf("TNbar1 0x%lx w 0x%lx\n",port * TASvar_PORT_TSN_width + TASvar_TSN_config_space_lower + reg,value);
    uint32_t *val = BASE + port * TASvar_PORT_TSN_width + TASvar_TSN_config_space_lower + reg;
    *val = value;
    uint32_t *val_shadow = BASE_shadow + port * TASvar_PORT_TSN_width + TASvar_TSN_config_space_lower + reg;
    *val = value;
  }
}
//********************************************************************************************************************
/**
*
@param reg register of config in port config space
@param port portnumber
@param value value to write
*/
uint32_t
TSN_set_config2(uint64_t reg, uint8_t port, uint32_t value)
{
  if (TSN_enable) {
    if (TN_tsn_verbose)
      printf ("writing to address:0x%llx value:0x%llx\n", port * TASvar_PORT_TSN_width + TASvar_TSN_config_space_lower + reg,value);
      
    uint32_t *val_shadow = BASE_shadow + port * TASvar_PORT_TSN_width + TASvar_TSN_config_space_lower + reg;
    *val_shadow = value;
  }
}
//********************************************************************************************************************
/**
*
@param reg register of config in port config space
@param port portnumber
@param value value to write
*/
uint32_t
INR_PCI_BAR1_read (uint64_t reg)
{
  
    uint32_t *val = BASE + reg;
		return *val;
  }

//********************************************************************************************************************
/**
*
@param reg register of config in port config space
@param port portnumber
@param value value to write
*/
void
INR_PCI_BAR1_write (uint32_t value,uint64_t reg)
{
  
    uint32_t *val = BASE + reg;
	 *val=value;
  }
  //********************************************************************************************************************
/**
*makes sure that time values have the correct granularity, 
*granularity of the hardware tas in ns, all time values have to be multibles of this value
*/
struct granularitytime TSN_time_ganularity(uint32_t inputtime){
  struct  granularitytime retval;
	uint32_t time_granularity=5;
#ifdef C_SUB_ADDR_TM_SCHED_TAS_TICK_GRANULARITY
	time_granularity=INR_PCI_BAR1_read((C_BASE_ADDR_TM_SCHED_LOWER<<8)+C_SUB_ADDR_TM_SCHED_TAS_TICK_GRANULARITY);
#endif
	if(time_granularity>100){
		printf("error: Strange value %i for TAS time granularity, will be reset to 5ns",time_granularity);
		time_granularity=5; //plausicheck		
	}
	uint32_t correcttime=inputtime;
	while(correcttime%time_granularity)correcttime--;//decrement until it fits
	if(correcttime!=inputtime)printf("warning: timestamp %llins dosn't meet the time granularity constrains of %llins and was corrected to %llins\n",inputtime, time_granularity,correcttime);
	retval.correcttime=correcttime;
	retval.error_ns=inputtime-correcttime;
	return retval;
}
//********************************************************************************************************************
/**
*applies the configuration which was specified before
*function theoretically unused, will be removed soon
*/
void
TSN_apply (struct arguments *arguments)
{
  
}

//********************************************************************************************************************
/**
*transform hardware ticks to ns (timebase 5ns)
*/
uint32_t
tick2ns (uint32_t ticks)
{
  return ticks * HW_TIMEBASE;
}

//********************************************************************************************************************
/**
*transform hardware ticks to ns (timebase 5ns)
*/
uint32_t
ns2ticks (uint32_t ns)
{
  return ns / HW_TIMEBASE;
}
void
TN_tsn_memdump_en ()
{
  TN_tsn_MEMDUMP = 1;

}

//********************************************************************************************************************
/**
*checks gcl for lost queues
*/
void
check_gcl(uint8_t port)
{
  uint8_t i=0,gcl_length=0;uint8_t queue_vector=0;
  gcl_length=TSN_get_config(TASvar_C_SUB_ADDR_TM_SCHED_TAS_ADMIN_GCL_LEN, port,0); // get gcl length
  for(i=0;i<gcl_length;i++){
  	struct GateControl_list_entry *entry_shadow =
      BASE_shadow + port * TASvar_PORT_TSN_width + TASvar_GateControl_list_base + TASvar_GateControl_list_entry_length * i;
    if (entry_shadow != NULL) {
      queue_vector|=entry_shadow->GCL;
    }
  }
  if((~queue_vector)&0xff){
  printf("WARNING! Queue(s) ");
  for (i=0;i<8;i++)if((~queue_vector)&(1<<i))printf("%i, ",i);
  printf("never served. Scheduler will crash if packes are forwarded to this queues!\n");
   
  }
}
//********************************************************************************************************************
/**
*return real cycle time without time granularity adjustment
*/
uint64_t 
get_real_cycletime(uint8_t port)
{
  uint8_t i=0,gcl_length=0;
  uint64_t sum=0;
  gcl_length=TSN_get_config(TASvar_C_SUB_ADDR_TM_SCHED_TAS_ADMIN_GCL_LEN, port,0); // get gcl length
  for(i=0;i<gcl_length;i++){
  
 		 struct granularitytime granularitytime=get_TSN_granularitytime(port,i);	
     sum+=(uint64_t)granularitytime.correcttime+(uint64_t)granularitytime.error_ns;
     

  }
  return sum;
}

//********************************************************************************************************************
/**
*checks time_gcl for sum of timeslots
*/
uint64_t 
check_tgcl(uint8_t port)
{
  uint8_t i=0,gcl_length=0;
  uint64_t sum=0;
  gcl_length=TSN_get_config(TASvar_C_SUB_ADDR_TM_SCHED_TAS_ADMIN_GCL_LEN, port,0); // get gcl length
  for(i=0;i<gcl_length;i++){
  	struct GateControl_TIME_list_entry *entry =
	BASE_shadow + port * TASvar_PORT_TSN_width + TASvar_GateControl_TIME_list_base + TASvar_GateControl_TIME_list_entry_length * i;
      if (entry != NULL) {
      sum+=(uint64_t)entry->time;
	}
  }
  return sum;
}
//********************************************************************************************************************
/**
*converts user GCL into GCL wich can be applied to hardware respecting time granularity
*/
void INR_TSN_correct_gcl(struct arguments *arguments){
	
	uint32_t gcl_length=TSN_get_config(TASvar_C_SUB_ADDR_TM_SCHED_TAS_ADMIN_GCL_LEN, arguments->PORT,0); // get gcl length
	uint64_t i=0,j=0;
	struct granularitytime gcl_correction_values;
	uint32_t correctionerror=0;
	uint32_t new_gcl_length=1;
	uint32_t time_granularity=5;
#ifdef C_SUB_ADDR_TM_SCHED_TAS_TICK_GRANULARITY
	time_granularity=INR_PCI_BAR1_read((C_BASE_ADDR_TM_SCHED_LOWER<<8)+C_SUB_ADDR_TM_SCHED_TAS_TICK_GRANULARITY);
#endif
	if(time_granularity>100){
		printf("error: Strange value %i for TAS time granularity, will be reset to 5ns",time_granularity);
		time_granularity=5; //plausicheck		
	}
	for(i=0;i<gcl_length;i++){ //get time error per cycle
		 gcl_correction_values=get_TSN_granularitytime(arguments->PORT, i);
		 correctionerror+=gcl_correction_values.error_ns;
	}
	while((new_gcl_length*correctionerror)%time_granularity){//is time_error inside time granulartiy
		new_gcl_length++; //if not we have to extend the GCL
		}
	verblog printf ("Time error per cycle: %lli ns, extending GCL to %i cycles\n",correctionerror,new_gcl_length);
	correctionerror*=new_gcl_length;//sum ns to correct for all cycles
	if(new_gcl_length>1){
		for(i=0;i<gcl_length;i++){

		  struct GateControl_TIME_list_entry *old_Tentry_shadow =
		    BASE_shadow + arguments->PORT * TASvar_PORT_TSN_width + TASvar_GateControl_TIME_list_base + TASvar_GateControl_TIME_list_entry_length * i;
		  struct GateControl_list_entry *old_entry_shadow =
		    BASE_shadow + arguments->PORT * TASvar_PORT_TSN_width + TASvar_GateControl_list_base + TASvar_GateControl_list_entry_length * i;
			
			for(j=1;j<new_gcl_length;j++){
			
				struct GateControl_TIME_list_entry *Tentry =
				  BASE + arguments->PORT * TASvar_PORT_TSN_width + TASvar_GateControl_TIME_list_base + TASvar_GateControl_TIME_list_entry_length * (i+j*gcl_length);
				struct GateControl_TIME_list_entry *Tentry_shadow =
				  BASE_shadow + arguments->PORT * TASvar_PORT_TSN_width + TASvar_GateControl_TIME_list_base + TASvar_GateControl_TIME_list_entry_length * (i+j*gcl_length);
				struct GateControl_list_entry *entry =
				  BASE + arguments->PORT * TASvar_PORT_TSN_width + TASvar_GateControl_list_base + TASvar_GateControl_list_entry_length * (i+j*gcl_length);
				struct GateControl_list_entry *entry_shadow =
				  BASE_shadow + arguments->PORT * TASvar_PORT_TSN_width + TASvar_GateControl_list_base + TASvar_GateControl_list_entry_length * (i+j*gcl_length);
				Tentry_shadow->time=old_Tentry_shadow->time;
				entry_shadow->GCL=old_entry_shadow->GCL;
				if(correctionerror){//still error in gcl
					Tentry_shadow->time+=time_granularity;//correct time entry
					correctionerror-=time_granularity;
				}
				TSNmemcpy (Tentry, Tentry_shadow, TASvar_GateControl_TIME_list_entry_length);
				TSNmemcpy (entry, entry_shadow, TASvar_GateControl_list_entry_length);
			
	}}}
	
	new_gcl_length*=gcl_length; //calculate number of entrys in new GCL
	TSN_set_config(TASvar_C_SUB_ADDR_TM_SCHED_TAS_ADMIN_GCL_LEN, arguments->PORT,new_gcl_length);//store new value
  arguments->ADMIN_GCL_LEN=new_gcl_length;
}

//********************************************************************************************************************
/**
*apply setting to tas (this is the new, cool version)
*/
void
new_TSN_apply (struct arguments *arguments){
	uint8_t firstconfig=0;
	uint64_t old_admin_basetime=0;
	uint64_t realcycletime=0;
	uint64_t sleepbuffer=300000000;
	uint64_t timewindow=2000000000,N=0,tmp_act=0;;
	uint32_t BRIDGE_clock_value_L=0,CTRLD_clock_value_L=0,BRIDGE_clock_value_H=0,CTRLD_clock_value_H=0;
    uint64_t currenttime=0, sleeptime_ns=0,sleeptime_s=0;
    uint64_t cycle_start_time, TAS_OPER_BASE_TIME,TAS_OPER_CYCLE_TIME,TAS_OPER_CYCLE_TIME_EXT,TAS_ADMIN_CYCLE_TIME_EXT;
    old_admin_basetime=arguments->ADMIN_BASE_TIME;
     if(arguments->PORT>=TSN_PORT_count){
		   printf ("The bitstream doen't provide a TAS for Port %i'\n",arguments->PORT);
		   return;
       }
		 if(INR_TAS)TSN_set_config (TASvar_C_SUB_ADDR_TM_SCHED_TAS_GATE_ENABLE, arguments->PORT, 1);// enable TAS in case of INR version
		  
		 if (arguments->dohave_ADMIN_BASE_TIME==0) {
		    printf ("Please provide ADMIN_BASE_TIME parameter!\n");
		    return;
		 }
/*    if (arguments->dohave_ADMIN_CYCLE_TIME==0) {*/
/*      printf ("Please provide ADMIN_CYCLE_TIME parameter!\n");*/
/*      return;*/
/*    }*/
		
		realcycletime=get_real_cycletime(arguments->PORT);
		INR_TSN_correct_gcl(arguments);
		tmp_act=check_tgcl(arguments->PORT);
		if(arguments->ADMIN_CYCLE_TIME!=tmp_act){
			printf ("autocorrect ADMIN_CYCLE_TIME to sum of GCL:%lli  realcycletime: %lli\n",tmp_act,realcycletime);
			arguments->ADMIN_CYCLE_TIME=(uint32_t)tmp_act;
			arguments->dohave_ADMIN_CYCLE_TIME=1;
		}
		if(tmp_act&0xffffffff00000000){
			printf ("Sum of GCL timeslots to big, cancel operation!\n");
			return;
		}
    if (arguments->dohave_ADMIN_CYCLE_TIME_EXT==0) {
      printf ("Please provide ADMIN_CYCLE_TIME_EXT parameter!\n");
      return;
    }
    if (arguments->hilscher_mode2)if(arguments->ADMIN_CYCLE_TIME>=0xfffffff7){
    	printf ("ADMIN_CYCLE_TIME out of range!\n");
      	return;
    }
		
		
	if (1 & TSN_get_config (TASvar_C_SUB_ADDR_TM_SCHED_TAS_GATE_ENABLE, arguments->PORT, 1)) {
      firstconfig = 0;
      verblog printf ("TSN already runing\n");
    }
    else {
      TSN_set_config (TASvar_C_SUB_ADDR_TM_SCHED_TAS_GATE_ENABLE, arguments->PORT, 1);
      firstconfig = 1;
      verblog printf ("TSN was off, enabling\n");
    }				//enable TSN
		
		
	verblog printf("GCL length:%i\n",TSN_get_config(TASvar_C_SUB_ADDR_TM_SCHED_TAS_ADMIN_GCL_LEN, arguments->PORT,0));	
	verblog printf ("Wait until pending config is done\n");
    while (TSN_get_config (TASvar_C_SUB_ADDR_TM_SCHED_TAS_CONFIG_CHANGE_PENDING, arguments->PORT, 0)) {
      POLLSLEEP}
		#ifdef C_BASE_ADDR_RTC
	BRIDGE_clock_value_L=INR_PCI_BAR1_read((C_BASE_ADDR_RTC<<8)+C_SUB_ADDR_RTC_BRIDGE_LOW);
    BRIDGE_clock_value_H=INR_PCI_BAR1_read((C_BASE_ADDR_RTC<<8)+C_SUB_ADDR_RTC_BRIDGE_HIGH);
    CTRLD_clock_value_L=INR_PCI_BAR1_read((C_BASE_ADDR_RTC<<8)+C_SUB_ADDR_RTC_CTRLD_LOW);
    CTRLD_clock_value_H=INR_PCI_BAR1_read((C_BASE_ADDR_RTC<<8)+C_SUB_ADDR_RTC_CTRLD_HIGH);
		#endif
	currenttime=CTRLD_clock_value_L|((uint64_t)CTRLD_clock_value_H<<32);
	verblog printf("Current time: %lli\n",currenttime);
start:	
	if(arguments->ADMIN_BASE_TIME<currenttime){//error timoint in past
		printf ("Oper time in the past, using time machine!\n Basetime:%lli, Now:%lli, Increment:%lli\n",arguments->ADMIN_BASE_TIME,currenttime,((currenttime-arguments->ADMIN_BASE_TIME)/realcycletime)*realcycletime+realcycletime);
		
	}
	while(arguments->ADMIN_BASE_TIME<currenttime){
		
		if(currenttime-arguments->ADMIN_BASE_TIME<realcycletime*10){//if we are near the end, we check time again
				#ifdef C_BASE_ADDR_RTC
			BRIDGE_clock_value_L=INR_PCI_BAR1_read((C_BASE_ADDR_RTC<<8)+C_SUB_ADDR_RTC_BRIDGE_LOW);
			BRIDGE_clock_value_H=INR_PCI_BAR1_read((C_BASE_ADDR_RTC<<8)+C_SUB_ADDR_RTC_BRIDGE_HIGH);
			CTRLD_clock_value_L=INR_PCI_BAR1_read((C_BASE_ADDR_RTC<<8)+C_SUB_ADDR_RTC_CTRLD_LOW);
			CTRLD_clock_value_H=INR_PCI_BAR1_read((C_BASE_ADDR_RTC<<8)+C_SUB_ADDR_RTC_CTRLD_HIGH);
				#endif
			currenttime=CTRLD_clock_value_L|((uint64_t)CTRLD_clock_value_H<<32);
			arguments->ADMIN_BASE_TIME+=realcycletime;
			}else
			{arguments->ADMIN_BASE_TIME+=(((currenttime-arguments->ADMIN_BASE_TIME)/realcycletime)-5)*realcycletime;}
			//printf("basetime:%lli currenttime:%lli diff:%lli\n",arguments->ADMIN_BASE_TIME,currenttime,currenttime-arguments->ADMIN_BASE_TIME);
	}
	printf ("New Basetime:%li!\n",arguments->ADMIN_BASE_TIME);
	
	if (arguments->hilscher_mode) {// wait until config change time is insede next running cycle
      TAS_OPER_BASE_TIME = 0xffffffff&tick2ns (TSN_get_config (TASvar_C_SUB_ADDR_TM_SCHED_TAS_OPER_BASE_TIME, arguments->PORT, 0));
      TAS_OPER_CYCLE_TIME = 0xffffffff&tick2ns (realcycletime);
      TAS_ADMIN_CYCLE_TIME_EXT = arguments->ADMIN_CYCLE_TIME_EXT;
     verblog  printf ("Hilscher compatibility mode...\n");
      if(TAS_OPER_CYCLE_TIME){//prevent endless loops
      if((currenttime/(TAS_OPER_CYCLE_TIME + TAS_ADMIN_CYCLE_TIME_EXT))>10){
      N = (currenttime/(TAS_OPER_CYCLE_TIME + TAS_ADMIN_CYCLE_TIME_EXT))-10;}else N=0;
      N=0;
      cycle_start_time = TAS_OPER_BASE_TIME + N * TAS_OPER_CYCLE_TIME + TAS_ADMIN_CYCLE_TIME_EXT;
      
		  while (cycle_start_time < currenttime) {
			N++;
			cycle_start_time = TAS_OPER_BASE_TIME + N * TAS_OPER_CYCLE_TIME + TAS_ADMIN_CYCLE_TIME_EXT;
			if (N > 0xfffffffffffffff0)
		  	break;		//value not important just to prevent endless loops
		  }
		  cycle_start_time = TAS_OPER_BASE_TIME + N * TAS_OPER_CYCLE_TIME + TAS_ADMIN_CYCLE_TIME_EXT;
		  //timewindow=cycle_start_time+TAS_OPER_CYCLE_TIME+TAS_ADMIN_CYCLE_TIME_EXT;
		  timewindow=TAS_OPER_CYCLE_TIME+TAS_ADMIN_CYCLE_TIME_EXT;
		verblog   printf("cycle_start_time is %lli new timewindow is %llins\n",cycle_start_time,timewindow);
      }else{
      	printf ("cannot calculate N because OPER_CYCLE_TIME=%lli, leaving Hilscher mode...\n",TAS_OPER_CYCLE_TIME);
           
      }
    }
    
startsleep:

	
	while(arguments->ADMIN_BASE_TIME>(timewindow+currenttime)){//time to long, have to sleep
		//if(arguments->ADMIN_BASE_TIME<=currenttime+sleepbuffer)break;
		sleeptime_ns=arguments->ADMIN_BASE_TIME-currenttime;
		if(sleeptime_ns>sleepbuffer)sleeptime_ns-=sleepbuffer;else sleeptime_ns/=2;
		verblog printf("basetime:%lli, currenttime:%lli, sleepns:%lli\n",arguments->ADMIN_BASE_TIME,currenttime,sleeptime_ns);
		if(sleeptime_ns>=timewindow){
		sleeptime_s=(sleeptime_ns/1000000000)-(timewindow/1000000000);
		sleeptime_ns=(sleeptime_ns%1000000000)-(timewindow%1000000000);}else
		{sleeptime_s=(sleeptime_ns/1000000000);
		sleeptime_ns=(sleeptime_ns%1000000000);
		}
		verblog printf ("Going to sleep for %llis and %llins\n",sleeptime_s,sleeptime_ns);
		sleep(sleeptime_s);
		usleep((sleeptime_ns/1000));
			#ifdef C_BASE_ADDR_RTC
		BRIDGE_clock_value_L=INR_PCI_BAR1_read((C_BASE_ADDR_RTC<<8)+C_SUB_ADDR_RTC_BRIDGE_LOW);
		BRIDGE_clock_value_H=INR_PCI_BAR1_read((C_BASE_ADDR_RTC<<8)+C_SUB_ADDR_RTC_BRIDGE_HIGH);
		CTRLD_clock_value_L=INR_PCI_BAR1_read((C_BASE_ADDR_RTC<<8)+C_SUB_ADDR_RTC_CTRLD_LOW);
		CTRLD_clock_value_H=INR_PCI_BAR1_read((C_BASE_ADDR_RTC<<8)+C_SUB_ADDR_RTC_CTRLD_HIGH);
			#endif
		currenttime=CTRLD_clock_value_L|((uint64_t)CTRLD_clock_value_H<<32);
		
	}
	if(arguments->ADMIN_BASE_TIME<currenttime){printf ("sleeped %llins too long, restarting...\n",currenttime-arguments->ADMIN_BASE_TIME); goto start;}
	if(arguments->ADMIN_BASE_TIME>(timewindow+currenttime)){goto startsleep;//doublechck
	}
	arguments->ADMIN_BASE_TIME&=0xffffffff;
	if(arguments->hilscher_mode){ //in this case we waited for ABS=CCT=CST
		arguments->CONFIG_CHANGE_TIME=arguments->ADMIN_BASE_TIME;
		arguments->dohave_CONFIG_CHANGE_TIME=1;
		arguments->CYCLE_START_TIME=arguments->ADMIN_BASE_TIME;
		arguments->dohave_CYCLE_START_TIME=1;
	}
	TN_tsn_config_change (arguments);//set values to hardware;
	
	TSN_set_config (TASvar_C_SUB_ADDR_TM_SCHED_TAS_CONFIG_CHANGE, arguments->PORT, 1);	//trigger config change
	//usleep(1);//be sure the TAS gets the falling edge
    if(INR_TAS==0)TSN_set_config (TASvar_C_SUB_ADDR_TM_SCHED_TAS_CONFIG_CHANGE, arguments->PORT, 0);
/*    if(arguments->hilscher_mode){*/
/*    	printf ("writing oper config...\n");*/
/*		TSN_set_config (TASvar_C_SUB_ADDR_TM_SCHED_TAS_OPER_BASE_TIME, arguments->PORT, ns2ticks (arguments->ADMIN_BASE_TIME));*/
/*		TSN_set_config (TASvar_C_SUB_ADDR_TM_SCHED_TAS_OPER_CYCLE_TIME, arguments->PORT, ns2ticks (arguments->ADMIN_CYCLE_TIME));*/
/*    }*/
	check_gcl(arguments->PORT);// check if cosed queues are configured!
	TSN_set_config2(TASvar_C_SUB_ADDR_TM_SCHED_TAS_ADMIN_BASE_TIME,arguments->PORT,old_admin_basetime);
	
   verblog  printf("Basetime:%lli\nCycletime:%lli\nCycletime_ext:%lli\n",arguments->ADMIN_BASE_TIME,arguments->ADMIN_CYCLE_TIME,arguments->ADMIN_CYCLE_TIME_EXT);
}

