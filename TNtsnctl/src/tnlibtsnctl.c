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
  arguments->ADMIN_GCL_LEN = 0;
  arguments->ADMIN_BASE_TIME = 0;
  arguments->ADMIN_CYCLE_TIME = 0;
  arguments->ADMIN_CYCLE_TIME_EXT = 0;
  arguments->CONFIG_CHANGE_TIME = 0;
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
*set verbode
*@param verbose value
*/
void
printallconst ()
{
#ifndef __KERNEL__		//not allowed for kmods
  printf ("Compiled %s %s\n", __DATE__, __TIME__);

#endif

  printconst (GateControl_list_length);
  printconst (GateControl_list_entry_length);
  printconst (GateControl_list_base);
  printconst (GateControl_TIME_list_length);
  printconst (GateControl_TIME_list_entry_length);
  printconst (GateControl_TIME_list_base);
  printconst (QUEUE_PRIO_list_length);
  printconst (QUEUE_PRIO_list_entry_length);
  printconst (QUEUE_PRIO_list_base);
  printconst (TSN_config_space_lower);
  printconst (PORT_count);
  printconst (PORT_TSN_width);




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
      if (MEMDUMP) {
	
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
TSN_init (uint64_t * base, uint64_t * shadow_base)
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
config_change (struct arguments *arguments)
{
  if (verbose)
    printf ("info: %s:%d:%s \n", __FILE__, __LINE__, __func__);
  if (TSN_enable) {
    TSN_config_dohavewrite (arguments, ADMIN_GCL_LEN)
      TSN_config_dohavewrite (arguments, ADMIN_BASE_TIME)
      TSN_config_dohavewrite (arguments, ADMIN_CYCLE_TIME)
      TSN_config_dohavewrite (arguments, ADMIN_CYCLE_TIME_EXT)
      TSN_config_dohavewrite (arguments, CONFIG_CHANGE_TIME)
      TSN_config_dohavewrite (arguments, CYCLE_START_TIME)
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
  if (verbose)
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
      TSN_config_print (CONFIG_CHANGE_ACK, 0)
      TSN_config_print (OPER_GCL_LEN, 0)
      TSN_config_print (OPER_BASE_TIME, 0)
      TSN_config_print (OPER_CYCLE_TIME, 0)
      TSN_config_print (OPER_CYCLE_TIME_EXT, 0)
      TSN_config_print (OPER_GATE_STATES, 0)
#ifdef C_TM_SCHED_TAS_CUR_TIME
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
GateControl_list_change (struct arguments *arguments)
{
  if (verbose)
    printf ("info: %s:%d:%s \n", __FILE__, __LINE__, __func__);
  if (TSN_enable) {
    if (arguments->dohave_ID == 0 || arguments->ID >= GateControl_list_length) {
      printf ("please provide a valid entry ID!\n");
      return;
    }
    if (arguments->dohave_GATE_STATE_VECTOR == 0) {
      printf ("please provide a valid entry GATE_STATE_VECTOR!\n");
      return;
    }
    if (arguments->ID >= GateControl_list_length) {
      printf ("ID out of range!\n");
      return;
    }
    if (verbose)
      printf ("accessing address:0x%llx\n", arguments->PORT * PORT_TSN_width + GateControl_list_base + GateControl_list_entry_length * arguments->ID);
    struct GateControl_list_entry *entry =
      BASE + arguments->PORT * PORT_TSN_width + GateControl_list_base + GateControl_list_entry_length * arguments->ID;
    /*if (entry != NULL) {
       entry->GCL=arguments->GATE_STATE_VECTOR;
       } */
    struct GateControl_list_entry *entry_shadow =
      BASE_shadow + arguments->PORT * PORT_TSN_width + GateControl_list_base + GateControl_list_entry_length * arguments->ID;
    if (entry_shadow != NULL) {
      entry_shadow->GCL = arguments->GATE_STATE_VECTOR;
    }
    TSNmemcpy (entry, entry_shadow, GateControl_list_entry_length);

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
  if (verbose)
    printf ("info: %s:%d:%s \n", __FILE__, __LINE__, __func__);
  if (TSN_enable) {
    uint32_t i = 0;
    if (arguments->COUNT == 0) {
      arguments->COUNT = GateControl_list_length;
    }
    not_MACHINEREADABLE printf ("GateControl list for port %i from %i to %i.\n", arguments->PORT, arguments->ID,
				arguments->ID + arguments->COUNT - 1);
    MACHINEREADABLE printf ("{");
    for (i = arguments->ID; ((i < GateControl_list_length) && (i < arguments->ID + arguments->COUNT)); i++) {
      if (verbose)
	printf ("accessing address[%i]:0x%llx\n", i, arguments->PORT * PORT_TSN_width + GateControl_list_base + GateControl_list_entry_length * i);
      struct GateControl_list_entry *entry =
	BASE_shadow + arguments->PORT * PORT_TSN_width + GateControl_list_base + GateControl_list_entry_length * i;
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
void
GateControl_TIME_list_change (struct arguments *arguments)
{
  if (verbose)
    printf ("info: %s:%d:%s \n", __FILE__, __LINE__, __func__);
  if (TSN_enable) {
    if (arguments->dohave_ID == 0) {
      printf ("please provide a valid entry ID!\n");
      return;
    }
    if (arguments->ID >= GateControl_TIME_list_length) {
      printf ("ID out of range!\n");
      return;
    }
    if (arguments->dohave_INTERVAL == 0) {
      printf ("please provide a valid entry GATE_STATE_VECTOR!\n");
      return;
    }
    if (verbose)
      printf ("accessing address:0x%llx\n",
	      arguments->PORT * PORT_TSN_width + GateControl_TIME_list_base + GateControl_TIME_list_entry_length * arguments->ID);
    struct GateControl_TIME_list_entry *entry =
      BASE + arguments->PORT * PORT_TSN_width + GateControl_TIME_list_base + GateControl_TIME_list_entry_length * arguments->ID;
    /*if (entry != NULL) {
       entry->time=arguments->INTERVAL;
       } */
    struct GateControl_TIME_list_entry *entry_shadow =
      BASE_shadow + arguments->PORT * PORT_TSN_width + GateControl_TIME_list_base + GateControl_TIME_list_entry_length * arguments->ID;
    if (entry_shadow != NULL) {
      entry_shadow->time = arguments->INTERVAL;
    }
    TSNmemcpy (entry, entry_shadow, GateControl_TIME_list_entry_length);
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
  if (verbose)
    printf ("info: %s:%d:%s \n", __FILE__, __LINE__, __func__);
  if (TSN_enable) {
    uint32_t i = 0;
    if (arguments->COUNT == 0) {
      arguments->COUNT = GateControl_TIME_list_length;
    }
    not_MACHINEREADABLE printf ("GateControl TIME for port %i list from %i to %i.\n", arguments->PORT, arguments->ID,
				arguments->ID + arguments->COUNT - 1);
    MACHINEREADABLE printf ("{");
    for (i = arguments->ID; ((i < GateControl_TIME_list_length) && (i < arguments->ID + arguments->COUNT)); i++) {
      if (verbose)
	printf ("accessing address[%i]:0x%llx\n", i,
		arguments->PORT * PORT_TSN_width + GateControl_TIME_list_base + GateControl_TIME_list_entry_length * i);
      struct GateControl_TIME_list_entry *entry =
	BASE_shadow + arguments->PORT * PORT_TSN_width + GateControl_TIME_list_base + GateControl_TIME_list_entry_length * i;
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
  if (verbose)
    printf ("info: %s:%d:%s \n", __FILE__, __LINE__, __func__);
  if (TSN_enable) {
    if (arguments->dohave_ID == 0) {
      printf ("please provide a valid entry ID!\n");
      return;
    }
    if (arguments->ID >= QUEUE_PRIO_list_length) {
      printf ("ID out of range!\n");
      return;
    }
    if (arguments->dohave_QUEUE_PRIO == 0) {
      printf ("please provide a valid QUEUE_PRIO!\n");
      return;
    }
    if (verbose)
      printf ("accessing address:0x%llx\n", arguments->PORT * PORT_TSN_width + QUEUE_PRIO_list_base + QUEUE_PRIO_list_entry_length * arguments->ID);
    struct QUEUE_PRIO_list_entry *entry =
      BASE + arguments->PORT * PORT_TSN_width + QUEUE_PRIO_list_base + QUEUE_PRIO_list_entry_length * arguments->ID;
    /*if (entry != NULL) {
       entry->prio=arguments->QUEUE_PRIO;
       } */
    struct QUEUE_PRIO_list_entry *entry_shadow =
      BASE_shadow + arguments->PORT * PORT_TSN_width + QUEUE_PRIO_list_base + QUEUE_PRIO_list_entry_length * arguments->ID;
    if (entry_shadow != NULL) {
      entry_shadow->prio = arguments->QUEUE_PRIO;
    }
    TSNmemcpy (entry, entry_shadow, QUEUE_PRIO_list_entry_length);
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
  if (verbose)
    printf ("info: %s:%d:%s \n", __FILE__, __LINE__, __func__);
  if (TSN_enable) {
    uint32_t i = 0;
    if (arguments->COUNT == 0) {
      arguments->COUNT = QUEUE_PRIO_list_length;
    }
    MACHINEREADABLE printf ("{");
    not_MACHINEREADABLE printf ("QUEUE_PRIO for port %i list from %i to %i.\n", arguments->PORT, arguments->ID, arguments->ID + arguments->COUNT - 1);
    for (i = arguments->ID; ((i < QUEUE_PRIO_list_length) && (i < arguments->ID + arguments->COUNT)); i++) {
      if (verbose)
	printf ("accessing address[%i]:0x%llx\n", i, arguments->PORT * PORT_TSN_width + QUEUE_PRIO_list_base + QUEUE_PRIO_list_entry_length * i);
      struct QUEUE_PRIO_list_entry *entry = BASE_shadow + arguments->PORT * PORT_TSN_width + QUEUE_PRIO_list_base + QUEUE_PRIO_list_entry_length * i;
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
    uint32_t *val = base + port * PORT_TSN_width + TSN_config_space_lower + reg;
	if (verbose)
      printf ("reding address:0x%llx value:0x%llx\n", port * PORT_TSN_width + TSN_config_space_lower + reg,*val);
      if(MEMDUMP)printf("TNbar1 0x%lx w \n",port * PORT_TSN_width + TSN_config_space_lower + reg);
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
TSN_set_config (uint64_t reg, uint8_t port, uint32_t value)
{
  if (TSN_enable) {
    if (verbose)
      printf ("writing to address:0x%llx value:0x%llx\n", port * PORT_TSN_width + TSN_config_space_lower + reg,value);
      if(MEMDUMP)printf("TNbar1 0x%lx w 0x%lx\n",port * PORT_TSN_width + TSN_config_space_lower + reg,value);
    uint32_t *val = BASE + port * PORT_TSN_width + TSN_config_space_lower + reg;
    *val = value;
    uint32_t *val_shadow = BASE_shadow + port * PORT_TSN_width + TSN_config_space_lower + reg;
    *val = value;
  }
}

//********************************************************************************************************************
/**
*applies the configuration which was specified before
*/
void
TSN_apply (struct arguments *arguments)
{
  uint8_t port = 0;
  uint8_t firstconfig = 1;
  uint64_t TAS_OPER_BASE_TIME, TAS_OPER_CYCLE_TIME, config_change_time, cycle_start_time, N, TAS_ADMIN_BASE_TIME, TAS_ADMIN_CYCLE_TIME, curr_TIME;
  struct timespec currenttime;

  for (port = arguments->PORT; port < PORT_count; port++) {
    verblog printf ("Configuring TSn for port %i\n", port);
    if (1 & TSN_get_config (C_TM_SCHED_TAS_GATE_ENABLE, port, 1)) {
      firstconfig = 0;
      verblog printf ("TSn already runing\n");
    }
    else {
      TSN_set_config (C_TM_SCHED_TAS_GATE_ENABLE, port, 1);
      firstconfig = 1;
      verblog printf ("TSN was off, enabling\n");
    }				//enable TSN
    verblog printf ("Wait until pending config is done\n");
    if (1 & TSN_get_config (C_TM_SCHED_TAS_CONFIG_CHANGE_PENDING, port, 0))
      while ((1 & TSN_get_config (C_TM_SCHED_TAS_CONFIG_CHANGE_ACK, port, 0))==0) {
      POLLSLEEP}		//wait if config is pending
    verblog printf ("read config values and time\n");
    TAS_OPER_BASE_TIME = tick2ns (TSN_get_config (C_TM_SCHED_TAS_OPER_BASE_TIME, port, 0));
    TAS_OPER_CYCLE_TIME = tick2ns (TSN_get_config (C_TM_SCHED_TAS_OPER_CYCLE_TIME, port, 0));
    clock_gettime (CLOCK_REALTIME, &currenttime);
    curr_TIME = currenttime.tv_nsec;
#ifdef C_TM_SCHED_TAS_CUR_TIME
    curr_TIME = tick2ns (TSN_get_config (C_TM_SCHED_TAS_CUR_TIME, 0, 0));	//hardware hack until PHP is available
#endif
    //Implementation according to IEEE802.1Gbv 2015 
    verblog printf ("calculate cycle_start_time...\n");
    if (TAS_OPER_BASE_TIME >= curr_TIME) {

      cycle_start_time = TAS_OPER_BASE_TIME;
    }
    else {

      N = 0;
      cycle_start_time = TAS_OPER_BASE_TIME + N * TAS_OPER_CYCLE_TIME;
      while (cycle_start_time < curr_TIME) {
	N++;
	cycle_start_time = TAS_OPER_BASE_TIME + N * TAS_OPER_CYCLE_TIME;
	if (N > 65000)
	  break;		//value not important just to prevent endless loops
      }
      N--;
      cycle_start_time = TAS_OPER_BASE_TIME + N * TAS_OPER_CYCLE_TIME;
    }
    //we scip case c and d because there is no pending config here
    verblog printf ("read config values and time\n");
    TAS_ADMIN_BASE_TIME = tick2ns (TSN_get_config (C_TM_SCHED_TAS_ADMIN_BASE_TIME, port, 0));
    TAS_ADMIN_CYCLE_TIME = tick2ns (TSN_get_config (C_TM_SCHED_TAS_ADMIN_CYCLE_TIME, port, 0));
    clock_gettime (CLOCK_REALTIME, &currenttime);
    curr_TIME = currenttime.tv_nsec;
#ifdef C_TM_SCHED_TAS_CUR_TIME
    curr_TIME = tick2ns (TSN_get_config (C_TM_SCHED_TAS_CUR_TIME, 0, 0));	//hardware hack until PHP is available
#endif
    N = 0;
    verblog printf ("calculate config_change_time...\n");
    if (TAS_ADMIN_BASE_TIME >= curr_TIME) {
      config_change_time = TAS_ADMIN_BASE_TIME;
    }
    else {

      config_change_time = TAS_ADMIN_BASE_TIME + N * TAS_ADMIN_CYCLE_TIME;
      while (config_change_time < curr_TIME) {
	N++;
	config_change_time = TAS_ADMIN_BASE_TIME + N * TAS_ADMIN_CYCLE_TIME;
	if (N > 65000)
	  break;		//value not important just to prevent endless loops
      }
      N--;
      config_change_time = TAS_ADMIN_BASE_TIME + N * TAS_ADMIN_CYCLE_TIME;
      if (firstconfig == 0) {	//errorconter++
      }

    }
    //store values
    verblog printf ("write values and trigger update\n");
    TSN_set_config (C_TM_SCHED_TAS_CYCLE_START_TIME, port, ns2ticks (cycle_start_time));
    TSN_set_config (C_TM_SCHED_TAS_CONFIG_CHANGE_TIME, port, ns2ticks (config_change_time));
    TSN_set_config (C_TM_SCHED_TAS_CONFIG_CHANGE, port, 1);	//trigger config change
    TSN_set_config (C_TM_SCHED_TAS_CONFIG_CHANGE, port, 0);
    /*while ((TSN_get_config (C_TM_SCHED_TAS_CONFIG_CHANGE_ACK, port, 0) & 1) == 0) {
    POLLSLEEP}*/
    verblog printf ("port config done\n");

    if (arguments->dohave_PORT)
      break;			//do just for one port
  }
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
memdump_en ()
{
  MEMDUMP = 1;

}

