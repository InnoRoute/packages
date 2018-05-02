/**
*@file tntsnctl.c
*@brief userspace TSN acces tool
*M.Ulbricht 2018
**/
//*****************************************************************************************************************

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <fcntl.h>
#include <ctype.h>
#include <termios.h>
#include <sys/types.h>
#include <sys/mman.h>
#include "tnlibtsnctl.h"
#include <argp.h>
#include <arpa/inet.h>
#include <fcntl.h>

#define MAP_SIZE 16384000UL
//#define MAP_MASK (MAP_SIZE - 1)
const char *argp_program_version = "tsnctl 0.1";
const char filename[] = "/sys/bus/pci/devices/0000:01:00.0/resource1";
const char *argp_program_bug_address = "<ulbricht@innoroute.de>";
struct flock lock;

/*
   OPTIONS.  Field 1 in ARGP.
   Order of fields: {NAME, KEY, ARG, FLAGS, DOC}.
*/
static struct argp_option options[] = {	//user interface
  {0, 0, 0, 0, "General options:", 0},
  {"verbose", 'v', 0, 0, "Produce verbose output"},
  {"machine", 'm', 0, 0, "Output in machine readable notation."},
  {"ID", 'i', "", 0, "ID of entry"},
  {"COUNT", 'c', "", 0, "number of entrys to do something"},
  {0, 0, 0, 0, "General TSN config:", 1},
  {"PORT", 'P', "", 0, "TrustNode port to configure"},

  {"ADMIN_GCL_LEN", 'a', "", 0, "The length of the gate list to be programmed."},
  {"ADMIN_BASE_TIME", 'b', "", 0, "The base time for the gate list to be programmed."},
  {"ADMIN_CYCLE_TIME", 'C', "", 0, "The admin cycle time constant for the gate list to be programmed."},
  {"ADMIN_CYCLE_TIME_EXT", 'd', "", 0, "The admin cycle time extension constant for the gate list to be programmed."},
  {"CONFIG_CHANGE_TIME", 'e', "", 0, "The calculated config change time. Assumption is that calculations done in software."},
  {"CYCLE_START_TIME", 'f', "", 0, "The cycle start time for the control list. When the list pointer is set to zero."},
  {"GATE_ENABLE", 'g', "", 0, "The gate enable bit. The gate control list is only active if enabled, otherwise all traffic goes through."},
  {"CONFIG_CHANGE", 'h', "", 0, "The config change command. Issued after configuring the gate control list and admin registers."},
  {"ADMIN_GATE_STATES", 'A', "", 0, "Sets the state of the gate output when the gate is disabled."},
/*  {"CONFIG_CHANGE_PENDING", 'j', "", 0, "If 1 indicates a config is pending. Read only"}, //readonly
  {"CONFIG_CHANGE_ACK", 'k', "", 0, "Acknowledgment that the config change is successful."},//readonly
  {"OPER_GCL_LEN", 'l', "", 0, "The gate control list length for the active config."},//readonly
  {"OPER_BASE_TIME", 'r', "", 0, "The oper base time for the active config"},//readonly
  {"OPER_CYCLE_TIME", 'o', "", 0, "The oper cycle time for the active config."},//readonly
  {"OPER_CYCLE_TIME_EXT", 'q', "", 0, "The oper cycle time extension for the active config."},*/

  {0, 0, 0, 0, "List options", 2},
  {"INTERVAL", 'I', "", 0, "Interval entry of list"},
  {"GATE_STATE_VECTOR", 'S', "", 0, "Gate state of entry"},
  {"QUEUE_PRIO", 'p', "", 0, "Gate state of entry"},
  {0}
};

/*
   PARSER. Field 2 in ARGP.
   Order of parameters: KEY, ARG, STATE.
*/
static error_t
parse_opt (int key, char *arg, struct argp_state *state)
{
  struct arguments *arguments = state->input;
  switch (key) {
  case 'v':
    arguments->verbose = 1;
    set_verbose (1);
    printallconst ();
    break;
  case 'm':
    arguments->machinereadable = 1;
    break;
  case 'n':
    arguments->numberout = 1;
    break;
  case 'i':
    arguments->ID = strtoull (arg, 0, 0);
    arguments->dohave_ID = 1;
    break;
  case 'c':
    arguments->COUNT = strtoull (arg, 0, 0);
    arguments->dohave_COUNT = 1;
    break;

  case 'a':
    arguments->ADMIN_GCL_LEN = strtoull (arg, 0, 0);
    arguments->dohave_ADMIN_GCL_LEN = 1;
    break;
  case 'A':
    arguments->ADMIN_GATE_STATES = strtoull (arg, 0, 0);
    arguments->dohave_ADMIN_GATE_STATES = 1;
    break;

  case 'b':
    arguments->ADMIN_BASE_TIME = strtoull (arg, 0, 0);
    arguments->dohave_ADMIN_BASE_TIME = 1;
    break;
  case 'C':
    arguments->ADMIN_CYCLE_TIME = strtoull (arg, 0, 0);
    arguments->dohave_ADMIN_CYCLE_TIME = 1;
    break;
  case 'd':
    arguments->ADMIN_CYCLE_TIME_EXT = strtoull (arg, 0, 0);
    arguments->dohave_ADMIN_CYCLE_TIME_EXT = 1;
    break;
  case 'e':
    arguments->CONFIG_CHANGE_TIME = strtoull (arg, 0, 0);
    arguments->dohave_CONFIG_CHANGE_TIME = 1;
    break;
  case 'f':
    arguments->CYCLE_START_TIME = strtoull (arg, 0, 0);
    arguments->dohave_CYCLE_START_TIME = 1;
    break;
  case 'g':
    arguments->GATE_ENABLE = strtoull (arg, 0, 0);
    arguments->dohave_GATE_ENABLE = 1;
    break;
  case 'h':
    arguments->CONFIG_CHANGE = strtoull (arg, 0, 0);
    arguments->dohave_CONFIG_CHANGE = 1;
    break;
  case 'j':
    arguments->CONFIG_CHANGE_PENDING = strtoull (arg, 0, 0);
    arguments->dohave_CONFIG_CHANGE_PENDING = 1;
    break;
  case 'k':
    arguments->CONFIG_CHANGE_ACK = strtoull (arg, 0, 0);
    arguments->dohave_CONFIG_CHANGE_ACK = 1;
    break;
  case 'l':
    arguments->OPER_GCL_LEN = strtoull (arg, 0, 0);
    arguments->dohave_OPER_GCL_LEN = 1;
    break;
  case 'o':
    arguments->OPER_CYCLE_TIME = strtoull (arg, 0, 0);
    arguments->dohave_OPER_CYCLE_TIME = 1;
    break;
  case 'q':
    arguments->OPER_CYCLE_TIME_EXT = strtoull (arg, 0, 0);
    arguments->dohave_OPER_CYCLE_TIME_EXT = 1;
    break;
  case 'r':
    arguments->OPER_BASE_TIME = strtoull (arg, 0, 0);
    arguments->dohave_OPER_BASE_TIME = 1;
    break;

  case 'I':
    arguments->INTERVAL = strtoull (arg, 0, 0);
    arguments->dohave_INTERVAL = 1;
    break;
  case 'S':
    arguments->GATE_STATE_VECTOR = strtoull (arg, 0, 0);
    arguments->dohave_GATE_STATE_VECTOR = 1;
    break;
  case 'p':
    arguments->QUEUE_PRIO = strtoull (arg, 0, 0);
    arguments->dohave_QUEUE_PRIO = 1;
    break;
  case 'P':
    arguments->PORT = strtoull (arg, 0, 0);
    arguments->dohave_PORT = 1;
    if (arguments->PORT >= PORT_count) {
      printf ("Invalid port number!\n");
      return ARGP_ERR_UNKNOWN;
    }
    break;

  case ARGP_KEY_ARG:
    if (state->arg_num >= 2) {
      argp_usage (state);
      break;
    }
    arguments->args[state->arg_num] = arg;
    break;
  case ARGP_KEY_END:
    if (state->arg_num < 1) {
      argp_usage (state);
    }
    break;
  default:
    return ARGP_ERR_UNKNOWN;
  }
  return 0;
}

/*
   ARGS_DOC. Field 3 in ARGP.
   A description of the non-option command-line arguments
     that we accept.
*/
//static char args_doc[] = "[AdminCTL_list_|OperCTL_list_|config_][add|del|print|change]"; //old doku
static char args_doc[] = "[apply|QueuePrio_list_|GateControl_list_|TGateControl_list_|config_][print|change]";

/*
  DOC.  Field 4 in ARGP.
  Program documentation.
*/
static char doc[] = "Trustnode TSN userspace interface";

/*
   The ARGP structure itself.
*/
static struct argp argp = { options, parse_opt, args_doc, doc };

/*
   The main function.
   Notice how now the only function call needed to process
   all command-line options and arguments nicely
   is argp_parse.
*/
//********************************************************************************************************************
/**
*main
*/
int
main (int argc, char **argv)
{
  int fd, fd_shadow, fd_master;
  uint64_t *map_base, *map_base_shadow, *map_base_master;
  off_t target;
  struct arguments arguments;	//create structure for passing comandlinearguments and settings
  clear_arguments (&arguments);
  argp_parse (&argp, argc, argv, 0, 0, &arguments);

  if ((fd = open (filename, O_RDWR | O_SYNC)) == -1) {
    printf ("error opening file\n");
  }

  lock.l_start = 0;		//lock semaphor to prevent shadowfile acces of other instances
  lock.l_whence = SEEK_SET;
  lock.l_len = 0;
  lock.l_type = F_RDLCK;
  fcntl (fd, F_SETLKW, &lock);	//block until file is free

  if ((fd_shadow = open ("/tmp/INR_TSN_shadow.mem", O_CREAT | O_RDWR | O_SYNC, 0600)) == -1) {	//shadowmemory, because FPGA-mmi is write only
    printf ("error opening shadowmem file\n");
  }

  ftruncate (fd_shadow, MAP_SIZE);	//if new files created, expand them


  map_base = mmap (0, MAP_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);	//map files and mmi to memory
  map_base_shadow = mmap (0, MAP_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd_shadow, 0);
  TSN_init (map_base, map_base_shadow);

  if (!map_base) {
    printf ("error mapping memory\n");
  }
  if (!map_base_shadow) {
    printf ("error mapping memory\n");
  }

  switch (arguments.args[0][0]) {	//parse commandline arguments
    /*case 'A': //old doku
       switch (arguments.args[0][14]) {
       case 'a':AdminCTL_list_add(&arguments);break;
       case 'd':AdminCTL_list_del(&arguments);break;
       case 'p':AdminCTL_list_print(&arguments);break;
       default:printf("unknown subaction\n");break;
       }
       break;
       case 'O':
       switch (arguments.args[0][13]) {
       case 'a':OperCTL_list_add(&arguments);break;
       case 'd':OperCTL_list_del(&arguments);break;
       case 'p':OperCTL_list_print(&arguments);break;
       default:printf("unknown subaction\n");break;
       }
       break;
     */
  case 'a':
    TSN_apply (&arguments);
    break;
  case 'G':
    switch (arguments.args[0][17]) {
    case 'c':
      GateControl_list_change (&arguments);
      break;
    case 'p':
      GateControl_list_print (&arguments);
      break;
    default:
      printf ("unknown subaction\n");
      break;
    }
    break;
  case 'Q':
    switch (arguments.args[0][15]) {
    case 'c':
      QUEUE_PRIO_list_change (&arguments);
      break;
    case 'p':
      QUEUE_PRIO_list_print (&arguments);
      break;
    default:
      printf ("unknown subaction\n");
      break;
    }
    break;
  case 'T':
    switch (arguments.args[0][18]) {
    case 'c':
      GateControl_TIME_list_change (&arguments);
      break;
    case 'p':
      GateControl_TIME_list_print (&arguments);
      break;
    default:
      printf ("unknown subaction\n");
      break;
    }
    break;
  case 'c':
    switch (arguments.args[0][7]) {
    case 'c':
      config_change (&arguments);
      break;
    case 'p':
      config_print (&arguments);
      break;
    default:
      printf ("unknown subaction\n");
      break;
    }
    break;
  default:
    printf ("unknown action\n");
    break;
  }
  if (TSN_enable == 0)
    printf ("This program was compiled without TSN address map information, nothing will happen!\n");
  munmap (map_base, MAP_SIZE);	//unmap files and mmi from memory
  munmap (map_base_shadow, MAP_SIZE);

  close (fd_shadow);		//close shadow files

  lock.l_type = F_UNLCK;	//free semaphor
  fcntl (fd, F_SETLK, &lock);	//release filelock
  close (fd);
  return 0;
}
