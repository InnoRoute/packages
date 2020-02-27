/**
*@file tnlibqcictl.h
*@brief userspace TSN acces tool
*M.Ulbricht 2019
**/

#include "tn_env.h"





#ifndef C_BASE_ADDR_QCI0_LOWER    //backward compatibiltiy
	
	#define	 C_BASE_ADDR_QCI_LOWER 0
	#define C_BASE_ADDR_QCI_UPPER 0
  #define C_BASE_ADDR_QCI0_LOWER 0
  #define C_BASE_ADDR_QCI0_UPPER 0
  #define C_SUB_ADDR_QCI0_SGCL_LOWER 0                             
  #define C_SUB_ADDR_QCI0_SGCL_UPPER 0
  #define C_SUB_ADDR_QCI0_SGCL_TIME_LOWER 0        
  #define C_SUB_ADDR_QCI0_SGCL_TIME_UPPER 0
  #define C_SUB_ADDR_QCI0_CONFIG_REG_LOWER 0
  #define C_SUB_ADDR_QCI0_CONFIG_REG_UPPER 0
  #define C_SUB_ADDR_QCI0_ADMIN_SGCL_LENGTH 0
  #define C_SUB_ADDR_QCI0_ADMIN_BASE_TIME 0
  #define C_SUB_ADDR_QCI0_ADMIN_CYCLE_TIME 0
  #define C_SUB_ADDR_QCI0_ADMIN_CYCLE_TIME_EXTENSION 0
  #define C_SUB_ADDR_QCI0_ADMIN_STREAM_GATE_STATES 0
  #define C_SUB_ADDR_QCI0_CONFIG_CHANGE 0
  #define C_SUB_ADDR_QCI0_CONFIG_PENDING 0
  #define C_SUB_ADDR_QCI0_CONFIG_CHANGE_TIME 0 
  #define C_SUB_ADDR_QCI0_CONFIG_CHANGE_ERROR 0 
  #define C_SUB_ADDR_QCI0_GATE_ENABLE 0 
  #define C_SUB_ADDR_QCI0_OPER_BASE_TIME  0 
  #define C_SUB_ADDR_QCI0_OPER_CYCLE_TIME  0 
  #define C_SUB_ADDR_QCI0_OPER_CYCLE_TIME_EXTENSION 0
  #define C_SUB_ADDR_QCI0_OPER_GATE_STATES 0
  #define C_SUB_ADDR_QCI0_OPER_SGCL_LENGTH 0
  #define C_SUB_ADDR_QCI0_CYCLE_START_TIME 0
  #define C_BASE_ADDR_QCI1_LOWER_0 0
  #define C_BASE_ADDR_QCI1_LOWER_1 0
  #define C_BASE_ADDR_QCI1_UPPER_0 0
  #define C_BASE_ADDR_QCI1_UPPER_1 0
  #define C_SUB_ADDR_QCI1_IPV_LOWER 0
  #define C_SUB_ADDR_QCI1_IPV_UPPER 0
  #define C_BASE_ADDR_QCI1_LOWER 0


#endif


#ifndef C_BLOCK_SIZE_ADDR_TM_SCHED
	#define C_BLOCK_SIZE_ADDR_TM_SCHED 0x1000
	#define C_SUB_ADDR_TM_SCHED_TAS_ADMIN_GATE_STATES 0x438
	#define C_SUB_ADDR_TM_SCHED_TAS_OPER_GATE_STATES 0x43C
#endif


uint8_t TN_qci_get_verbose (void);
void TN_qci_set_verbose (uint8_t i);
void TN_qci_printallconst (void);
#define verblog if(TN_qci_get_verbose())	//macro for checking verbose bit


//#define PORT_count get_QCI_PORT_count() 

/*
#define QCI_config_space_lower (C_BASE_ADDR_TM_SCHED_LOWER<<8)

#define PORT_QCI_width C_BLOCK_SIZE_ADDR_TM_SCHED

#define GateControl_list_base (C_SUB_ADDR_TM_SCHED_GCL_LOWER<<0)+(QCI_config_space_lower)
#define GateControl_list_entry_length 4
#define GateControl_list_length ((((C_SUB_ADDR_TM_SCHED_GCL_UPPER+1)<<0)-(C_SUB_ADDR_TM_SCHED_GCL_LOWER<<0))/GateControl_list_entry_length)

#define GateControl_TIME_list_base (C_SUB_ADDR_TM_SCHED_GCL_TIME_LOWER<<0)+(QCI_config_space_lower)
#define GateControl_TIME_list_entry_length 4
#define GateControl_TIME_list_length ((((C_SUB_ADDR_TM_SCHED_GCL_TIME_UPPER+1)<<0)-(C_SUB_ADDR_TM_SCHED_GCL_TIME_LOWER<<0))/GateControl_TIME_list_entry_length)

#define QUEUE_PRIO_list_base (C_SUB_ADDR_TM_SCHED_PROC_QUEUE_PRIO_LOWER<<0)+(QCI_config_space_lower)
#define QUEUE_PRIO_list_entry_length 4
#define QUEUE_PRIO_list_length ((((C_SUB_ADDR_TM_SCHED_PROC_QUEUE_PRIO_UPPER+1)<<0)-(C_SUB_ADDR_TM_SCHED_PROC_QUEUE_PRIO_LOWER<<0))/QUEUE_PRIO_list_entry_length)
*/


#define QCI_config_print(type, shadow) {MACHINEREADABLE printf("{'"#type"':0x%lx},", QCI_get_config(TASvar_C_SUB_ADDR_TM_SCHED_TAS_ ## type , arguments->PORT, shadow )); else printf(#type":0x%lx \n", QCI_get_config(TASvar_C_SUB_ADDR_TM_SCHED_TAS_ ## type , arguments->PORT, shadow ));}
#define QCI_config_dohavewrite(entry, type) {if( entry ->dohave_ ## type )QCI_set_config(TASvar_C_SUB_ADDR_TM_SCHED_TAS_ ## type , entry -> PORT, entry -> type );}
#define QCI_config_dohavewrite_time(entry, type) {if( entry ->dohave_ ## type ){struct granularitytime tmp_gran=QCI_time_ganularity( entry -> type );QCI_set_config(TASvar_C_SUB_ADDR_TM_SCHED_TAS_ ## type , entry -> PORT,tmp_gran.correcttime);}}
#define POLLSLEEP usleep(500);
#define MACHINEREADABLE if(arguments->machinereadable)
#define not_MACHINEREADABLE if(arguments->machinereadable==0)

#define HW_TIMEBASE 1		// one HW tick are X nanoseconds

struct granularitytime
{
	uint32_t correcttime;
	uint32_t error_ns;

} __attribute__ ((__packed__));
struct arguments
{
  char *args[1];		/* filename */
  uint8_t used;		/**<entry used */
  int verbose;			/* The -v flag */
  uint8_t numberout;
  uint8_t machinereadable;
  uint8_t hilscher_mode;
  uint8_t hilscher_mode2;
  uint64_t ID;
  uint64_t COUNT;
  uint32_t ADMIN_GCL_LEN;
  uint64_t ADMIN_BASE_TIME;
  uint64_t ADMIN_CYCLE_TIME;
  uint64_t ADMIN_CYCLE_TIME_EXT;
  uint64_t CONFIG_CHANGE_TIME;
  uint64_t CYCLE_START_TIME;
  uint32_t GATE_ENABLE;
  uint16_t bulk;
  uint32_t CONFIG_CHANGE;
  uint32_t CONFIG_CHANGE_PENDING;
  uint32_t CONFIG_CHANGE_ACK;
  uint64_t OPER_GCL_LEN;
  uint64_t OPER_BASE_TIME;
  uint64_t OPER_CYCLE_TIME;
  uint8_t	 IPV_value;
  uint8_t  IPV_gate;
  uint8_t  IPV_gclenty;
  uint64_t OPER_CYCLE_TIME_EXT;
  uint64_t INTERVAL;
  uint8_t PORT;
  uint16_t GATE_STATE_VECTOR;
  uint8_t QUEUE_PRIO;
  uint32_t ADMIN_GATE_STATES;

  uint8_t dohave_ID;
  uint8_t dohave_COUNT;

  uint8_t dohave_ADMIN_GCL_LEN;
  uint8_t dohave_ADMIN_BASE_TIME;
  uint8_t dohave_ADMIN_CYCLE_TIME;
  uint8_t dohave_ADMIN_CYCLE_TIME_EXT;
  uint8_t dohave_CONFIG_CHANGE_TIME;
  uint8_t dohave_CYCLE_START_TIME;
  uint8_t dohave_GATE_ENABLE;
  uint8_t	dohave_IPV_value;
  uint8_t dohave_IPV_gate;
  uint8_t dohave_IPV_gclenty;
  uint8_t dohave_CONFIG_CHANGE;
  uint8_t dohave_CONFIG_CHANGE_PENDING;
  uint8_t dohave_CONFIG_CHANGE_ACK;
  uint8_t dohave_OPER_GCL_LEN;
  uint8_t dohave_OPER_BASE_TIME;
  uint8_t dohave_OPER_CYCLE_TIME;
  uint8_t dohave_OPER_CYCLE_TIME_EXT;

  uint8_t dohave_INTERVAL;
  uint8_t dohave_PORT;
  uint8_t dohave_GATE_STATE_VECTOR;
  uint8_t dohave_QUEUE_PRIO;
  uint8_t dohave_ADMIN_GATE_STATES;

} __attribute__ ((__packed__));

struct GateControl_list_entry
{
  uint16_t GCL;
  uint16_t fill2;
} __attribute__ ((__packed__));

struct GateControl_TIME_list_entry
{
  uint32_t time;
} __attribute__ ((__packed__));

struct QUEUE_PRIO_list_entry
{
  uint32_t prio;
} __attribute__ ((__packed__));


void TN_qci_clear_arguments (struct arguments *arguments);
/*//got from old docu, maybe neede later
void AdminCTL_list_add(struct arguments *arguments);
void AdminCTL_list_del(struct arguments *arguments);
void AdminCTL_list_print(struct arguments *arguments);
void OperCTL_list_add(struct arguments *arguments);
void OperCTL_list_del(struct arguments *arguments);
void OperCTL_list_print(struct arguments *arguments);*/

void QUEUE_PRIO_list_change (struct arguments *arguments);
void QUEUE_PRIO_list_print (struct arguments *arguments);
void TN_qci_GateControl_list_change (struct arguments *arguments);
void GateControl_list_print (struct arguments *arguments);
void TN_qci_GateControl_TIME_list_change (struct arguments *arguments);
void GateControl_TIME_list_print (struct arguments *arguments);
void TN_qci_config_change (struct arguments *arguments);
void config_print (struct arguments *arguments);
void TSNmemcpy (void *dst, const void *src, size_t len);
uint32_t QCI_set_config (uint64_t reg, uint8_t port, uint32_t value);
uint32_t QCI_get_config (uint64_t reg, uint8_t port, uint8_t shadow);
void QCI_apply (struct arguments *arguments);
uint32_t ns2ticks (uint32_t ns);
uint32_t tick2ns (uint32_t ticks);
void memdump_en (void);
void GCL_entry (struct arguments *arguments);
void new_QCI_apply (struct arguments *arguments);
void QCI_init (uint64_t * base, uint64_t * shadow_base,uint64_t * shadow_base2);
void check_gcl(uint8_t port);
uint64_t check_tgcl(uint8_t port);
uint8_t get_QCI_enable(void);
uint32_t INR_PCI_BAR1_read (uint64_t reg);
void INR_PCI_BAR1_write (uint32_t value,uint64_t reg);
uint8_t get_QCI_PORT_count();
struct granularitytime get_QCI_granularitytime(uint8_t port, uint32_t ID);
struct granularitytime QCI_time_ganularity(uint32_t inputtime);
uint64_t get_real_cycletime(uint8_t port);
void INR_QCI_correct_gcl(struct arguments *arguments);
void QCI_refresh (void);
void TN_qci_IPV_list_change (struct arguments *arguments);
