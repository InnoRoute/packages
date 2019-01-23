/**
*@file tnlibstatistics.h
*@brief userspace STATISTICS acces tool
*M.Ulbricht 2018
**/

#include "tn_env.h"
#define STAT_portcount 16

uint8_t get_verbose (void);
void set_verbose (uint8_t i);
void printallconst (void);
#define verblog if(get_verbose())	//macro for checking verbose bit

#define MACHINEREADABLE if(arguments->machinereadable)
#define not_MACHINEREADABLE if(arguments->machinereadable==0)
#define C_BASE_ADDR_HC(id) (C_BASE_ADDR_HC_ ## id )
#define C_BASE_ADDR_HC_PCIE(id) (C_BASE_ADDR_HC_PCIE_ ## id )

struct arguments
{
  char *args[1];		/* filename */
  uint8_t used;		/**<entry used */
  int verbose;			/* The -v flag */
  uint8_t numberout;
  uint8_t machinereadable;
  uint64_t ID;
  uint64_t COUNT;
  

  uint8_t dohave_ID;
  uint8_t dohave_COUNT;
} __attribute__ ((__packed__));


void clear_arguments (struct arguments *arguments);

void TNmemcpy (void *dst, const void *src, size_t len);

void STAT_init (uint64_t * base, uint64_t * shadow_base);
