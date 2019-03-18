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
uint8_t TN_tsn_MEMDUMP = 0;

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
  arguments->hilscher_mode = 0;
  arguments->hilscher_mode2 = 0;
  arguments->COUNT = 0;
  arguments->ADMIN_GCL_LEN = 0;
  arguments->ADMIN_BASE_TIME = 0;
  arguments->ADMIN_CYCLE_TIME = 0;
  arguments->ADMIN_CYCLE_TIME_EXT = 0;
  arguments->CONFIG_CHANGE_TIME = 0;
  arguments->bulk = 0;
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
TN_tsn_printallconst ()
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

  printconst (C_TM_SCHED_TAS_OPER_CYCLE_TIME);
  printconst (C_TM_SCHED_TAS_OPER_CYCLE_TIME_EXT);
  printconst (C_TM_SCHED_TAS_OPER_BASE_TIME);

  printconst (C_TM_SCHED_TAS_ADMIN_CYCLE_TIME);
  printconst (C_TM_SCHED_TAS_ADMIN_CYCLE_TIME_EXT);
  printconst (C_TM_SCHED_TAS_ADMIN_BASE_TIME);




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
  while (TSN_get_config (C_TM_SCHED_TAS_CONFIG_CHANGE_PENDING, arguments->PORT, 0)) {
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
TN_tsn_config_print (struct arguments *arguments)
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
TN_tsn_GCL_entry (struct arguments *arguments)
{
  if (TN_tsn_verbose)
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
    if (arguments->dohave_PORT == 0) {
      printf ("please provide a Port number!\n");
      return;
    }
    if (arguments->ID >= GateControl_list_length) {
      printf ("ID out of range!\n");
      return;
    }
    if (arguments->dohave_INTERVAL == 0) {
      printf ("please provide a valid entry INTERVAL!\n");
      return;
    }
    TN_tsn_config_change (arguments);	//write config


    if (TSN_get_config (C_TM_SCHED_TAS_ADMIN_GCL_LEN, arguments->PORT, 0) <= arguments->ID) {
      printf ("Provided ID(%li) is beyond the GateControlList length (%li)\n", arguments->ID,
	      TSN_get_config (C_TM_SCHED_TAS_ADMIN_GCL_LEN, arguments->PORT, 0));
      return;
    }
    TN_tsn_GateControl_list_change (arguments);
    TN_tsn_GateControl_TIME_list_change (arguments);

  }
}

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
    if (TN_tsn_verbose)
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
    TN_tsn_config_change (arguments);
  }
}

//********************************************************************************************************************
/**
*
@param arguments argumentes from userinterface
*/
void
TN_tsn_GateControl_list_print (struct arguments *arguments)
{
  if (TN_tsn_verbose)
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
      if (TN_tsn_verbose)
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
TN_tsn_GateControl_TIME_list_change (struct arguments *arguments)
{
  if (TN_tsn_verbose)
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
      printf ("please provide a valid entry INTERVAL!\n");
      return;
    }
    if (TN_tsn_verbose)
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
TN_tsn_GateControl_TIME_list_print (struct arguments *arguments)
{
  if (TN_tsn_verbose)
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
      if (TN_tsn_verbose)
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
TN_tsn_QUEUE_PRIO_list_change (struct arguments *arguments)
{
  if (TN_tsn_verbose)
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
    if (TN_tsn_verbose)
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
TN_tsn_QUEUE_PRIO_list_print (struct arguments *arguments)
{
  if (TN_tsn_verbose)
    printf ("info: %s:%d:%s \n", __FILE__, __LINE__, __func__);
  if (TSN_enable) {
    uint32_t i = 0;
    if (arguments->COUNT == 0) {
      arguments->COUNT = QUEUE_PRIO_list_length;
    }
    MACHINEREADABLE printf ("{");
    not_MACHINEREADABLE printf ("QUEUE_PRIO for port %i list from %i to %i.\n", arguments->PORT, arguments->ID, arguments->ID + arguments->COUNT - 1);
    for (i = arguments->ID; ((i < QUEUE_PRIO_list_length) && (i < arguments->ID + arguments->COUNT)); i++) {
      if (TN_tsn_verbose)
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
    if (TN_tsn_verbose)
      printf ("reading address:0x%llx value:0x%llx\n", port * PORT_TSN_width + TSN_config_space_lower + reg, *val);
    if (TN_tsn_MEMDUMP)
      printf ("TNbar1 0x%lx w \n", port * PORT_TSN_width + TSN_config_space_lower + reg);

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
    if (TN_tsn_verbose)
      printf ("writing to address:0x%llx value:0x%llx\n", port * PORT_TSN_width + TSN_config_space_lower + reg, value);
    if (TN_tsn_MEMDUMP)
      printf ("TNbar1 0x%lx w 0x%lx\n", port * PORT_TSN_width + TSN_config_space_lower + reg, value);
    uint32_t *val = BASE + port * PORT_TSN_width + TSN_config_space_lower + reg;
    *val = value;
    uint32_t *val_shadow = BASE_shadow + port * PORT_TSN_width + TSN_config_space_lower + reg;
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
TN_tsn_INR_PCI_BAR1_read (uint64_t reg)
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
TN_tsn_INR_PCI_BAR1_write (uint32_t value, uint64_t reg)
{

  uint32_t *val = BASE + reg;
  *val = value;
}

  //********************************************************************************************************************
/**
*makes sure that time values have the correct granularity, see  TSN_TIME_GRANULARITY
*/
uint32_t
TSN_time_ganularity (uint32_t inputtime)
{
  uint32_t correcttime = inputtime;
  while (correcttime % TSN_TIME_GRANULARITY)
    correcttime--;		//decrement until it fits
  if (correcttime != inputtime)
    printf ("warning: timestamp %llins dosn't meet the time constrains of %llins and was corrected to %llins\n", inputtime, TSN_TIME_GRANULARITY,
	    correcttime);
  return correcttime;
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
  uint64_t TAS_OPER_BASE_TIME, TAS_OPER_CYCLE_TIME, config_change_time, cycle_start_time, N, TAS_ADMIN_BASE_TIME, TAS_ADMIN_CYCLE_TIME, curr_TIME,
    TAS_ADMIN_CYCLE_TIME_EXT;
  struct timespec currenttime;
  uint32_t BRIDGE_clock_value_L = 0, CTRLD_clock_value_L = 0, BRIDGE_clock_value_H = 0, CTRLD_clock_value_H = 0;
  TN_tsn_INR_PCI_BAR1_write (1, (C_BASE_ADDR_RTC << 8) + C_SUB_ADDR_RTC_CLKSEL);
  for (port = arguments->PORT; port < PORT_count; port++) {
    verblog printf ("Configuring TSN for port %i\n", port);
    if (1 & TSN_get_config (C_TM_SCHED_TAS_GATE_ENABLE, port, 1)) {
      firstconfig = 0;
      verblog printf ("TSN already runing\n");
    }
    else {
      TSN_set_config (C_TM_SCHED_TAS_GATE_ENABLE, port, 1);
      firstconfig = 1;
      verblog printf ("TSN was disabled, enabling\n");
    }				//enable TSN
    verblog printf ("Wait until pending config is done\n");
    if (1 & TSN_get_config (C_TM_SCHED_TAS_CONFIG_CHANGE_PENDING, port, 0))
      while ((1 & TSN_get_config (C_TM_SCHED_TAS_CONFIG_CHANGE_ACK, port, 0)) == 0) {
      POLLSLEEP}		//wait if config is pending
    verblog printf ("read config values and time\n");
    TAS_OPER_BASE_TIME = TN_tsn_tick2ns (TSN_get_config (C_TM_SCHED_TAS_OPER_BASE_TIME, port, 0));
    TAS_OPER_CYCLE_TIME = TN_tsn_tick2ns (TSN_get_config (C_TM_SCHED_TAS_OPER_CYCLE_TIME, port, 0));
    TAS_ADMIN_CYCLE_TIME_EXT = TN_tsn_tick2ns (TSN_get_config (C_TM_SCHED_TAS_ADMIN_CYCLE_TIME_EXT, port, 1));
    clock_gettime (CLOCK_REALTIME, &currenttime);
    curr_TIME = currenttime.tv_nsec;



#ifdef C_BASE_ADDR_RTC

    BRIDGE_clock_value_L = TN_tsn_INR_PCI_BAR1_read ((C_BASE_ADDR_RTC << 8) + C_SUB_ADDR_RTC_BRIDGE_LOW);
    BRIDGE_clock_value_H = TN_tsn_INR_PCI_BAR1_read ((C_BASE_ADDR_RTC << 8) + C_SUB_ADDR_RTC_BRIDGE_HIGH);
    CTRLD_clock_value_L = TN_tsn_INR_PCI_BAR1_read ((C_BASE_ADDR_RTC << 8) + C_SUB_ADDR_RTC_CTRLD_LOW);
    CTRLD_clock_value_H = TN_tsn_INR_PCI_BAR1_read ((C_BASE_ADDR_RTC << 8) + C_SUB_ADDR_RTC_CTRLD_HIGH);

#endif
    curr_TIME = CTRLD_clock_value_L | ((uint64_t) CTRLD_clock_value_H << 32);


    //Implementation according to IEEE802.1Qbv 2015 
    verblog printf ("calculate cycle_start_time...\n");
    if (TAS_OPER_BASE_TIME >= curr_TIME) {

      cycle_start_time = TAS_OPER_BASE_TIME;
    }
    else {

      N = 0;
      cycle_start_time = TAS_OPER_BASE_TIME + N * TAS_OPER_CYCLE_TIME + TAS_ADMIN_CYCLE_TIME_EXT;
      while (cycle_start_time < curr_TIME) {
	N++;
	cycle_start_time = TAS_OPER_BASE_TIME + N * TAS_OPER_CYCLE_TIME + TAS_ADMIN_CYCLE_TIME_EXT;
	if (N > 65000)
	  break;		//value not important just to prevent endless loops
      }
      //N--;
      cycle_start_time = TAS_OPER_BASE_TIME + N * TAS_OPER_CYCLE_TIME;
    }
    //we scip case c and d because there is no pending config here
    verblog printf ("read config values and time\n");
    TAS_ADMIN_BASE_TIME = TN_tsn_tick2ns (TSN_get_config (C_TM_SCHED_TAS_ADMIN_BASE_TIME, port, 0));
    TAS_ADMIN_CYCLE_TIME = TN_tsn_tick2ns (TSN_get_config (C_TM_SCHED_TAS_ADMIN_CYCLE_TIME, port, 0));
    TAS_ADMIN_CYCLE_TIME_EXT = TN_tsn_tick2ns (TSN_get_config (C_TM_SCHED_TAS_ADMIN_CYCLE_TIME_EXT, port, 1));
    clock_gettime (CLOCK_REALTIME, &currenttime);
    curr_TIME = currenttime.tv_nsec;
#ifdef C_BASE_ADDR_RTC

    BRIDGE_clock_value_L = TN_tsn_INR_PCI_BAR1_read ((C_BASE_ADDR_RTC << 8) + C_SUB_ADDR_RTC_BRIDGE_LOW);
    BRIDGE_clock_value_H = TN_tsn_INR_PCI_BAR1_read ((C_BASE_ADDR_RTC << 8) + C_SUB_ADDR_RTC_BRIDGE_HIGH);
    CTRLD_clock_value_L = TN_tsn_INR_PCI_BAR1_read ((C_BASE_ADDR_RTC << 8) + C_SUB_ADDR_RTC_CTRLD_LOW);
    CTRLD_clock_value_H = TN_tsn_INR_PCI_BAR1_read ((C_BASE_ADDR_RTC << 8) + C_SUB_ADDR_RTC_CTRLD_HIGH);

#endif
    curr_TIME = CTRLD_clock_value_L | ((uint64_t) CTRLD_clock_value_H << 32);
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
      //N--;
      config_change_time = TAS_ADMIN_BASE_TIME + N * TAS_ADMIN_CYCLE_TIME;
      if (firstconfig == 0) {	//errorconter++
      }

    }
    //store values
    verblog printf ("write values and trigger update\n");
    TSN_set_config (C_TM_SCHED_TAS_CYCLE_START_TIME, port, TN_tsn_ns2ticks (cycle_start_time));
    TSN_set_config (C_TM_SCHED_TAS_CONFIG_CHANGE_TIME, port, TN_tsn_ns2ticks (config_change_time));

    TSN_set_config (C_TM_SCHED_TAS_OPER_BASE_TIME, port, TN_tsn_ns2ticks (TSN_get_config (C_TM_SCHED_TAS_ADMIN_BASE_TIME, port, 1)));
    TSN_set_config (C_TM_SCHED_TAS_OPER_CYCLE_TIME, port, TN_tsn_ns2ticks (TSN_get_config (C_TM_SCHED_TAS_ADMIN_CYCLE_TIME, port, 1)));

    TSN_set_config (C_TM_SCHED_TAS_CONFIG_CHANGE, port, 1);	//trigger config change
    TSN_set_config (C_TM_SCHED_TAS_CONFIG_CHANGE, port, 0);

    /*while ((TSN_get_config (C_TM_SCHED_TAS_CONFIG_CHANGE_ACK, port, 0) & 1) == 0) {
       POLLSLEEP} */
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
TN_tsn_tick2ns (uint32_t ticks)
{
  return ticks * HW_TIMEBASE;
}

//********************************************************************************************************************
/**
*transform hardware ticks to ns (timebase 5ns)
*/
uint32_t
TN_tsn_ns2ticks (uint32_t ns)
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
TN_tsn_check_gcl (uint8_t port)
{
  uint8_t i = 0, gcl_length = 0;
  uint8_t queue_vector = 0;
  gcl_length = TSN_get_config (C_TM_SCHED_TAS_ADMIN_GCL_LEN, port, 0);	// get gcl length
  for (i = 0; i < gcl_length; i++) {
    struct GateControl_list_entry *entry_shadow = BASE_shadow + port * PORT_TSN_width + GateControl_list_base + GateControl_list_entry_length * i;
    if (entry_shadow != NULL) {
      queue_vector |= entry_shadow->GCL;
    }
  }
  if (queue_vector != 255) {
    printf ("WARNING! Queue(s) ");
    for (i = 0; i < 8; i++)
      if ((~queue_vector) & (1 << i))
	printf ("%i, ", i);
    printf ("never served. Scheduler will crash if packes are forwarded to this queues!\n");

  }
}

//********************************************************************************************************************
/**
*checks time_gcl for sum of timeslots
*/
uint64_t
TN_tsn_check_tgcl (uint8_t port)
{
  uint8_t i = 0, gcl_length = 0;
  uint64_t sum = 0;
  gcl_length = TSN_get_config (C_TM_SCHED_TAS_ADMIN_GCL_LEN, port, 0);	// get gcl length
  for (i = 0; i < gcl_length; i++) {
    struct GateControl_TIME_list_entry *entry =
      BASE_shadow + port * PORT_TSN_width + GateControl_TIME_list_base + GateControl_TIME_list_entry_length * i;
    if (entry != NULL) {
      sum += (uint64_t) entry->time;
    }
  }
  return sum;
}

//********************************************************************************************************************
/**
*apply setting to tas (this is the new, cool version)
*/
void
new_TSN_apply (struct arguments *arguments)
{
  uint8_t firstconfig = 0;
  uint64_t sleepbuffer = 300000000;
  uint64_t timewindow = 2000000000, N = 0, tmp_act = 0;;
  uint32_t BRIDGE_clock_value_L = 0, CTRLD_clock_value_L = 0, BRIDGE_clock_value_H = 0, CTRLD_clock_value_H = 0;
  uint64_t currenttime = 0, sleeptime_ns = 0, sleeptime_s = 0;
  uint64_t cycle_start_time, TAS_OPER_BASE_TIME, TAS_OPER_CYCLE_TIME, TAS_OPER_CYCLE_TIME_EXT, TAS_ADMIN_CYCLE_TIME_EXT;


  if (arguments->dohave_ADMIN_BASE_TIME == 0) {
    printf ("Please provide ADMIN_BASE_TIME parameter!\n");
    return;
  }
/*    if (arguments->dohave_ADMIN_CYCLE_TIME==0) {*/
/*      printf ("Please provide ADMIN_CYCLE_TIME parameter!\n");*/
/*      return;*/
/*    }*/

  tmp_act = TN_tsn_check_tgcl (arguments->PORT);
  if (arguments->ADMIN_CYCLE_TIME != tmp_act) {
    arguments->ADMIN_CYCLE_TIME = (uint32_t) tmp_act;
    printf ("autocorrect ADMIN_CYCLE_TIME to sum of GCL:%lli\n", arguments->ADMIN_CYCLE_TIME);
    arguments->dohave_ADMIN_CYCLE_TIME = 1;

  }
  if (tmp_act & 0xffffffff00000000) {
    printf ("Sum of GCL timeslots to big, cancel operation!\n");
    return;
  }
  if (arguments->dohave_ADMIN_CYCLE_TIME_EXT == 0) {
    printf ("Please provide ADMIN_CYCLE_TIME_EXT parameter!\n");
    return;
  }
  if (arguments->hilscher_mode2)
    if (arguments->ADMIN_CYCLE_TIME >= 0xfffffff7) {
      printf ("ADMIN_CYCLE_TIME out of range!\n");
      return;
    }


  if (1 & TSN_get_config (C_TM_SCHED_TAS_GATE_ENABLE, arguments->PORT, 1)) {
    firstconfig = 0;
    verblog printf ("TSN already runing\n");
  }
  else {
    TSN_set_config (C_TM_SCHED_TAS_GATE_ENABLE, arguments->PORT, 1);
    firstconfig = 1;
    verblog printf ("TSN was off, enabling\n");
  }				//enable TSN


  verblog printf ("GCL length:%i\n", TSN_get_config (C_TM_SCHED_TAS_ADMIN_GCL_LEN, arguments->PORT, 0));
  verblog printf ("Wait until pending config is done\n");
  while (TSN_get_config (C_TM_SCHED_TAS_CONFIG_CHANGE_PENDING, arguments->PORT, 0)) {
  POLLSLEEP}
#ifdef C_BASE_ADDR_RTC
  BRIDGE_clock_value_L = TN_tsn_INR_PCI_BAR1_read ((C_BASE_ADDR_RTC << 8) + C_SUB_ADDR_RTC_BRIDGE_LOW);
  BRIDGE_clock_value_H = TN_tsn_INR_PCI_BAR1_read ((C_BASE_ADDR_RTC << 8) + C_SUB_ADDR_RTC_BRIDGE_HIGH);
  CTRLD_clock_value_L = TN_tsn_INR_PCI_BAR1_read ((C_BASE_ADDR_RTC << 8) + C_SUB_ADDR_RTC_CTRLD_LOW);
  CTRLD_clock_value_H = TN_tsn_INR_PCI_BAR1_read ((C_BASE_ADDR_RTC << 8) + C_SUB_ADDR_RTC_CTRLD_HIGH);
#endif
  currenttime = CTRLD_clock_value_L | ((uint64_t) CTRLD_clock_value_H << 32);
  verblog printf ("Current time: %lli\n", currenttime);
start:
  if (arguments->ADMIN_BASE_TIME < currenttime) {	//error timoint in past
    printf ("Oper time in the past, using time machine!\n Basetime:%lli, Now:%lli, Increment:%lli\n", arguments->ADMIN_BASE_TIME, currenttime,
	    ((currenttime - arguments->ADMIN_BASE_TIME) / arguments->ADMIN_CYCLE_TIME) * arguments->ADMIN_CYCLE_TIME + arguments->ADMIN_CYCLE_TIME);

  }
  while (arguments->ADMIN_BASE_TIME < currenttime) {

    if (currenttime - arguments->ADMIN_BASE_TIME < arguments->ADMIN_CYCLE_TIME * 10) {	//if we are near the end, we check time again
#ifdef C_BASE_ADDR_RTC
      BRIDGE_clock_value_L = TN_tsn_INR_PCI_BAR1_read ((C_BASE_ADDR_RTC << 8) + C_SUB_ADDR_RTC_BRIDGE_LOW);
      BRIDGE_clock_value_H = TN_tsn_INR_PCI_BAR1_read ((C_BASE_ADDR_RTC << 8) + C_SUB_ADDR_RTC_BRIDGE_HIGH);
      CTRLD_clock_value_L = TN_tsn_INR_PCI_BAR1_read ((C_BASE_ADDR_RTC << 8) + C_SUB_ADDR_RTC_CTRLD_LOW);
      CTRLD_clock_value_H = TN_tsn_INR_PCI_BAR1_read ((C_BASE_ADDR_RTC << 8) + C_SUB_ADDR_RTC_CTRLD_HIGH);
#endif
      currenttime = CTRLD_clock_value_L | ((uint64_t) CTRLD_clock_value_H << 32);
      arguments->ADMIN_BASE_TIME += arguments->ADMIN_CYCLE_TIME;
    }
    else {
      arguments->ADMIN_BASE_TIME += (((currenttime - arguments->ADMIN_BASE_TIME) / arguments->ADMIN_CYCLE_TIME) - 5) * arguments->ADMIN_CYCLE_TIME;
    }
    //printf("basetime:%lli currenttime:%lli diff:%lli\n",arguments->ADMIN_BASE_TIME,currenttime,currenttime-arguments->ADMIN_BASE_TIME);
  }
  printf ("New Basetime:%li!\n", arguments->ADMIN_BASE_TIME);

  if (arguments->hilscher_mode) {	// wait until config change time is insede next running cycle
    TAS_OPER_BASE_TIME = 0xffffffff & TN_tsn_tick2ns (TSN_get_config (C_TM_SCHED_TAS_OPER_BASE_TIME, arguments->PORT, 0));
    TAS_OPER_CYCLE_TIME = 0xffffffff & TN_tsn_tick2ns (TSN_get_config (C_TM_SCHED_TAS_OPER_CYCLE_TIME, arguments->PORT, 0));
    TAS_ADMIN_CYCLE_TIME_EXT = arguments->ADMIN_CYCLE_TIME_EXT;
    verblog printf ("Hilscher compatibility mode...\n");
    if (TAS_OPER_CYCLE_TIME) {	//prevent endless loops
      if ((currenttime / (TAS_OPER_CYCLE_TIME + TAS_ADMIN_CYCLE_TIME_EXT)) > 10) {
	N = (currenttime / (TAS_OPER_CYCLE_TIME + TAS_ADMIN_CYCLE_TIME_EXT)) - 10;
      }
      else
	N = 0;
      N = 0;
      cycle_start_time = TAS_OPER_BASE_TIME + N * TAS_OPER_CYCLE_TIME + TAS_ADMIN_CYCLE_TIME_EXT;

      while (cycle_start_time < currenttime) {
	N++;
	cycle_start_time = TAS_OPER_BASE_TIME + N * TAS_OPER_CYCLE_TIME + TAS_ADMIN_CYCLE_TIME_EXT;
	if (N > 0xfffffffffffffff0)
	  break;		//value not important just to prevent endless loops
      }
      cycle_start_time = TAS_OPER_BASE_TIME + N * TAS_OPER_CYCLE_TIME + TAS_ADMIN_CYCLE_TIME_EXT;
      //timewindow=cycle_start_time+TAS_OPER_CYCLE_TIME+TAS_ADMIN_CYCLE_TIME_EXT;
      timewindow = TAS_OPER_CYCLE_TIME + TAS_ADMIN_CYCLE_TIME_EXT;
      verblog printf ("cycle_start_time is %lli new timewindow is %llins\n", cycle_start_time, timewindow);
    }
    else {
      printf ("cannot calculate N because OPER_CYCLE_TIME=%lli, leaving Hilscher mode...\n", TAS_OPER_CYCLE_TIME);

    }
  }

startsleep:


  while (arguments->ADMIN_BASE_TIME > (timewindow + currenttime)) {	//time to long, have to sleep
    //if(arguments->ADMIN_BASE_TIME<=currenttime+sleepbuffer)break;
    sleeptime_ns = arguments->ADMIN_BASE_TIME - currenttime;
    if (sleeptime_ns > sleepbuffer)
      sleeptime_ns -= sleepbuffer;
    else
      sleeptime_ns /= 2;
    verblog printf ("basetime:%lli, currenttime:%lli, sleepns:%lli\n", arguments->ADMIN_BASE_TIME, currenttime, sleeptime_ns);
    if (sleeptime_ns >= timewindow) {
      sleeptime_s = (sleeptime_ns / 1000000000) - (timewindow / 1000000000);
      sleeptime_ns = (sleeptime_ns % 1000000000) - (timewindow % 1000000000);
    }
    else {
      sleeptime_s = (sleeptime_ns / 1000000000);
      sleeptime_ns = (sleeptime_ns % 1000000000);
    }
    verblog printf ("Going to sleep for %llis and %llins\n", sleeptime_s, sleeptime_ns);
    sleep (sleeptime_s);
    usleep ((sleeptime_ns / 1000));
#ifdef C_BASE_ADDR_RTC
    BRIDGE_clock_value_L = TN_tsn_INR_PCI_BAR1_read ((C_BASE_ADDR_RTC << 8) + C_SUB_ADDR_RTC_BRIDGE_LOW);
    BRIDGE_clock_value_H = TN_tsn_INR_PCI_BAR1_read ((C_BASE_ADDR_RTC << 8) + C_SUB_ADDR_RTC_BRIDGE_HIGH);
    CTRLD_clock_value_L = TN_tsn_INR_PCI_BAR1_read ((C_BASE_ADDR_RTC << 8) + C_SUB_ADDR_RTC_CTRLD_LOW);
    CTRLD_clock_value_H = TN_tsn_INR_PCI_BAR1_read ((C_BASE_ADDR_RTC << 8) + C_SUB_ADDR_RTC_CTRLD_HIGH);
#endif
    currenttime = CTRLD_clock_value_L | ((uint64_t) CTRLD_clock_value_H << 32);

  }
  if (arguments->ADMIN_BASE_TIME < currenttime) {
    printf ("sleeped %llins too long, restarting...\n", currenttime - arguments->ADMIN_BASE_TIME);
    goto start;
  }
  if (arguments->ADMIN_BASE_TIME > (timewindow + currenttime)) {
    goto startsleep;		//doublechck
  }
  arguments->ADMIN_BASE_TIME &= 0xffffffff;
  if (arguments->hilscher_mode) {	//in this case we waited for ABS=CCT=CST
    arguments->CONFIG_CHANGE_TIME = arguments->ADMIN_BASE_TIME;
    arguments->dohave_CONFIG_CHANGE_TIME = 1;
    arguments->CYCLE_START_TIME = arguments->ADMIN_BASE_TIME;
    arguments->dohave_CYCLE_START_TIME = 1;
  }
  TN_tsn_config_change (arguments);	//set values to hardware;

  TSN_set_config (C_TM_SCHED_TAS_CONFIG_CHANGE, arguments->PORT, 1);	//trigger config change
  //usleep(1);//be sure the TAS gets the falling edge
  TSN_set_config (C_TM_SCHED_TAS_CONFIG_CHANGE, arguments->PORT, 0);
/*    if(arguments->hilscher_mode){*/
/*    	printf ("writing oper config...\n");*/
/*		TSN_set_config (C_TM_SCHED_TAS_OPER_BASE_TIME, arguments->PORT, ns2ticks (arguments->ADMIN_BASE_TIME));*/
/*		TSN_set_config (C_TM_SCHED_TAS_OPER_CYCLE_TIME, arguments->PORT, ns2ticks (arguments->ADMIN_CYCLE_TIME));*/
/*    }*/
  TN_tsn_check_gcl (arguments->PORT);	// check if cosed queues are configured!
  verblog printf ("Basetime:%lli\nCycletime:%lli\nCycletime_ext:%lli\n", arguments->ADMIN_BASE_TIME, arguments->ADMIN_CYCLE_TIME,
		  arguments->ADMIN_CYCLE_TIME_EXT);
}
