/**
*@file tnstatistics.c
*@brief userspace statistics acces tool
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
#include "tnlibstatistics.h"
#include <argp.h>
#include <arpa/inet.h>
#include <fcntl.h>

#define MAP_SIZE 16384000UL
//#define MAP_MASK (MAP_SIZE - 1)
const char *argp_program_version = "tsnctl 0.1";
const char filename[] = "/sys/bus/pci/devices/0000:01:00.0/resource1";
const char *argp_program_bug_address = "<ulbricht@innoroute.de>";
struct flock lock;
uint8_t MEMDUMP2=0;

/*
   OPTIONS.  Field 1 in ARGP.
   Order of fields: {NAME, KEY, ARG, FLAGS, DOC}.
*/
static struct argp_option options[] = {	//user interface
  {0, 0, 0, 0, "General options:", 0},
  {"verbose", 'v', 0, 0, "Produce verbose output"},
  {"machine", 'M', 0, 0, "Output in machine readable notation."},
  {"ID", 'i', "", 0, "ID of entry"},
  {"COUNT", 'c', "", 0, "number of entrys to do something"},
  {0, 0, 0, 0, "General TSN config:", 1},
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
  case 'M':
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
      //if(MEMDUMP2==0)printf("Using proprietary TSN implementation of Hilscher Gesellschaft fuer Systemautomation mbH, licensed for research projects and for use on the TrustNode only.\n");
    }
    break;
  default:
  	//if(MEMDUMP2==0)printf("Using proprietary TSN implementation of Hilscher Gesellschaft fuer Systemautomation mbH, licensed for research projects and for use on the TrustNode only.\n");
 
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
static char args_doc[] = "[all|io_counters|port_counters|bad_counters]";

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
  STAT_init (map_base, map_base_shadow);

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
    STAT_print_inout_counters(&arguments);
    break;
  case 'i':
    STAT_print_inout_counters(&arguments);
    break;
  case 'p':
    STAT_print_port_counters(&arguments);
    break;
  case 'b':
    STAT_print_bad_counters(&arguments);
    break;
  
  default:
    printf ("unknown action\n");
    break;
  }
  
  munmap (map_base, MAP_SIZE);	//unmap files and mmi from memory
  munmap (map_base_shadow, MAP_SIZE);

  close (fd_shadow);		//close shadow files

  lock.l_type = F_UNLCK;	//free semaphor
  fcntl (fd, F_SETLK, &lock);	//release filelock
  close (fd);
  return 0;
}
