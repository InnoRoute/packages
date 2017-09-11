/**
*@file tnflowtable.c
*@brief userspace flowtable acces
*M.Ulbricht 2016
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
#include "tnlibflowtable.h"
#include "flowtableactions.h"
#include "mastertable.h"
#include <argp.h>
#include <arpa/inet.h>
#include <fcntl.h>
#define MAP_SIZE 1024000UL
//#define MAP_MASK (MAP_SIZE - 1)
const char *argp_program_version = "tnflowcache 0.8";
const char filename[] = "/sys/bus/pci/devices/0000:01:00.0/resource1";
const char *argp_program_bug_address = "<ulbricht@innoroute.de>";
struct flock lock;


/*
   OPTIONS.  Field 1 in ARGP.
   Order of fields: {NAME, KEY, ARG, FLAGS, DOC}.
*/
static struct argp_option options[] = {
  {0, 0, 0, 0, "General options:", 0},
  {"verbose", 'v', 0, 0, "Produce verbose output"},
  {"ID", 'i', "", 0, "ID of entry"},
  {"COUNT", 'c', "", 0, "number of entrys to do something"},
  {0, 0, 0, 0, "Rule options:", 1},
  {"AUTOHASH", 'a', 0, 0, "automatic generate and write hash to hashtable"},
  {"AUTOACT", 'z', 0, 0, "automatic write action to Actiontable"},
  {"RULEPOINTER", 'r', "", 0, "Pointer to rule"},
  {"RULETYPE", 't', "", 0, "Type of Rule"},
  {"HASH_EMH", 'H', "", 0, "Hash value for EMH table"},
  {"RULEPRIO", 'p', "", 0, "Priority of Rule"},
  {"RULEACTION", 'Y', "", 0, "Action which is performed by rule"},
  //{"RULENEXT", 'n', "", 0, "Has rule an extension?"},
  {0, 0, 0, 0, "Rule Type 1 options:", 2},
  {"RC_MAC_SRC", 'S', "", 0, "source Mac address (hex Format)"},
  {"RC_MAC_DST", 'D', "", 0, "destination Mac address (hex Format)"},
  {"RC_VLAN", 'V', "", 0, "ID of VLAN"},
  {"RC_ETHERTYPE", 'E', "", 0, "ETHERTYPE"},
  {0, 0, 0, 0, "Rule Type 2 options:", 3},
  {"RC_IPv4_SRC", 'C', "", 0, "IPv4 source address"},
  {"RC_IPv4_DST", 'T', "", 0, "IPv4 destination address"},
  {"RC_PROTO", 'P', "", 0, "Protocol type of packet"},
  {"RC_PORT_SRC", 'R', "", 0, "UDP/TCP source port"},
  {"RC_PORT_DST", 'A', "", 0, "UDP/TCP destination port"},
  {0, 0, 0, 0, "Rule Type 3 options:", 4},
  {0, 0, 0, 0, "Rule Type 4 options:", 5},
  {0, 0, 0, 0, "EMA_Rule Type options:", 6},
  {"RC_EMA_VLAN_PRIO", 'U', "", 0, "Priority of VLAN"},
  {"RC_EMA_TOS", 'W', "", 0, "ToS field"},
  {"RC_EMA_INPORT", 'I', "", 0, "Ingressport of packet"},
  {0, 0, 0, 0, "ActionTable_Rule Type options:", 7},
  {"AT_OutPort", 'O', "", 0, "Output Port"},
  {"AT_Bad", 'b', "", 0, "Bad assignment"},
  {"AT_Bad_reason", 'B', "", 0, "Bad reason"},
  {"AT_Cut", 'X', "", 0, "Cut through assingment"},
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
    set_verbose(1);
    break;
  case 'a':
    arguments->HASH.gauto = 1;
    break;
  case 'z':
    arguments->autoaction = 1;
    break;
  case 'i':
    arguments->ID = strtoull (arg, 0, 0);
    break;
  case 'r':
    arguments->RULEPOINTER = strtoul (arg, 0, 0);
    break;
  case 't':
    arguments->TYPE_ID = strtoul (arg, 0, 0);
    break;
  case 'p':
    arguments->PRIORITY = strtoul (arg, 0, 0);
    break;
  case 'O':
    arguments->OutPort = strtoul (arg, 0, 0);
    arguments->OutPort_enable = 1;
    arguments->dohave_OutPort = 1;
    arguments->dohave_OutPort_enable = 1;
    break;
  case 'b':
    arguments->Bad_enable = 1;
    arguments->BadValue = strtoul (arg, 0, 0);
    arguments->dohave_Bad_enable = 1;
    arguments->dohave_BadValue = 1;
    break;
  case 'B':
    arguments->BadReason = strtoul (arg, 0, 0);
    arguments->Bad_enable = 1;
    arguments->dohave_BadReason = 1;
    arguments->dohave_Bad_enable = 1;
    break;
  case 'X':
    arguments->CutValue = strtoul (arg, 0, 0);
    arguments->Cut_enable = 1;
    arguments->dohave_CutValue = 1;
    arguments->dohave_Cut_enable = 1;
    break;
  case 'Y':
    arguments->ACTION_ID = strtoul (arg, 0, 0);
    break;
/*  case 'n':
    arguments->NEXT_BIT = strtoul (arg, 0, 0);
    break; */
  case 'c':
    arguments->COUNT = strtoull (arg, 0, 0);
    break;
  case 'S':
    arguments->MAC_SRC = strtoull (arg, 0, 0);
    hwaddr_aton2(arg,&arguments->MAC_SRC);
    arguments->MAC_SRC=htobe64(arguments->MAC_SRC)>>16;
    arguments->MASK_MAC_SRC = 0;
    arguments->dohave_MAC_SRC = 1;
    arguments->dohave_MASK_MAC_SRC = 1;
    break;
  case 'D':
    arguments->MAC_DST = strtoull (arg, 0, 0);
    hwaddr_aton2(arg,&arguments->MAC_DST);
    arguments->MAC_DST=htobe64(arguments->MAC_DST)>>16;
    arguments->MASK_MAC_DST = 0;
    arguments->dohave_MAC_DST = 1;
    arguments->dohave_MASK_MAC_DST = 1;
    break;
  case 'H':
    arguments->HASH.EMH = strtoull (arg, 0, 0);
    break;
  case 'E':
    arguments->ETHERTYPE = strtoull (arg, 0, 0);
    arguments->MASK_ETHERTYPE = 0;
    arguments->dohave_ETHERTYPE = 1;
    arguments->dohave_MASK_ETHERTYPE = 1;
    break;
  case 'V':
    arguments->VLAN_ID = strtoul (arg, 0, 0);
    arguments->MASK_VLAN_ID = 0;
    arguments->dohave_VLAN_ID = 1;
    arguments->dohave_MASK_VLAN_ID = 1;
    break;
  case 'W':
    arguments->TOS = strtoul (arg, 0, 0);
    arguments->MASK_TOS = 0;
    arguments->dohave_TOS = 1;
    arguments->dohave_MASK_TOS = 1;
    break;
  case 'U':
    arguments->VLAN_PRIO = strtoul (arg, 0, 0);
    arguments->MASK_VLAN_PRIO = 0;
    arguments->dohave_VLAN_PRIO = 1;
    arguments->dohave_MASK_VLAN_PRIO = 1;
    break;
  case 'I':
    arguments->INPORT = strtoul (arg, 0, 0);
    arguments->MASK_INPORT = 0;
    arguments->dohave_INPORT = 1;
    arguments->dohave_MASK_INPORT = 1;
    break;
  case 'C':
    arguments->IPv4_SRC = htobe32(inet_addr(arg));//strtoul (arg, 0, 0);
    arguments->MASK_IPv4_SRC = 0;
    arguments->dohave_IPv4_SRC = 1;
    arguments->dohave_MASK_IPv4_SRC = 1;
    break;
  case 'T':
    arguments->IPv4_DST =htobe32(inet_addr(arg));
//    arguments->IPv4_DST =parseIPV4string(arg);// strtoul (arg, 0, 0);
    arguments->MASK_IPv4_DST = 0;
    arguments->dohave_IPv4_DST = 1;
    arguments->dohave_MASK_IPv4_DST = 1;
    break;
  case 'P':
    arguments->PROTOCOL = strtoul (arg, 0, 0);
    arguments->MASK_PROTOCOL = 0;
    arguments->dohave_PROTOCOL = 1;
    arguments->dohave_MASK_PROTOCOL = 1;
    break;
  case 'R':
    arguments->PORT_SRC = strtoul (arg, 0, 0);
    arguments->MASK_PORT_SRC = 0;
    arguments->dohave_PORT_SRC = 1;
    arguments->dohave_MASK_PORT_SRC = 1;
    break;
  case 'A':
    arguments->PORT_DST = strtoul (arg, 0, 0);
    arguments->MASK_PORT_DST = 0;
    arguments->dohave_PORT_DST = 1;
    arguments->dohave_MASK_PORT_DST = 1;
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
static char args_doc[] = "action[[[HashT_[EMH|EMA]_|ActT_|RuleT_[EMH|EMA]_|CollT_EMH_][add|del|clearall|print]]|[[RuleT_[EMH|EMA]|CollT_EMH]_update]";

/*
  DOC.  Field 4 in ARGP.
  Program documentation.
*/
static char doc[] = "Trustnode flowtable userspace interface";

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
  struct arguments arguments;
  clear_arguments (&arguments);
  argp_parse (&argp, argc, argv, 0, 0, &arguments);

  if ((fd = open (filename, O_RDWR | O_SYNC)) == -1)
    printf ("error opening file\n");

  lock.l_start=0;
  lock.l_whence=SEEK_SET;
  lock.l_len=0;
  lock.l_type=F_RDLCK;
  fcntl(fd, F_SETLKW, &lock);//block until file is free

  if ((fd_shadow = open ("/tmp/INR_FC_shadow.mem", O_CREAT | O_RDWR | O_SYNC, 0600)) == -1)
    printf ("error opening shadowmem file\n");
//fd=fd_shadow; //workaround for missing memory
  if ((fd_master = open ("/tmp/INR_FC_masterTable.mem", O_CREAT | O_RDWR | O_SYNC, 0600)) == -1)
    printf ("error opening msterTable file\n");

  ftruncate (fd_shadow, MAP_SIZE);
  ftruncate (fd_master, MASTERTABLE_length * sizeof (struct arguments));

  map_base = mmap (0, MAP_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  map_base_shadow = mmap (0, MAP_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd_shadow, 0);
  map_base_master = mmap (0, MASTERTABLE_length * sizeof (struct arguments), PROT_READ | PROT_WRITE, MAP_SHARED, fd_master, 0);

  if (!map_base)
    printf ("error mapping memory\n");
  if (!map_base_shadow)
    printf ("error mapping memory\n");
  if (!map_base_master)
    printf ("error mapping memory\n");
  FCinit_EMH (map_base, map_base_shadow);
  FCinit_MasterTable (map_base_master);

  switch (arguments.args[0][0]) {
  case 'A':
    switch (arguments.args[0][5]) {	//actiontable
    case 'a':
      AT_add (&arguments);
      break;			//add
    case 'd':
      AT_del (arguments);
      break;			//del
    case 'c':
      AT_clear (arguments);
      break;			//clear all
    case 'p':
      AT_print (arguments);
      break;			//print
    default:
      printf ("unknown subaction\n");
      break;
    }
    break;
  case 'H':
    if (arguments.args[0][8] == 'H')
      switch (arguments.args[0][10]) {	//hashtable
      case 'a':
	HT_EMH_add (&arguments);
	break;			//add
      case 'd':
	HT_EMH_del (arguments);
	break;			//del
      case 'c':
	HT_EMH_clear (arguments);
	break;			//clear all
      case 'p':
	HT_EMH_print (arguments);
	break;			//print
      default:
	printf ("unknown subaction\n");
	break;
      }
    else
      switch (arguments.args[0][10]) {	//hashtable
      case 'a':
	HT_EMA_add (&arguments);
	break;			//add
      case 'd':
	HT_EMA_del (arguments);
	break;			//del
      case 'c':
	HT_EMA_clear (arguments);
	break;			//clear all
      case 'p':
	HT_EMA_print (arguments);
	break;			//print
      default:
	printf ("unknown subaction\n");
	break;
      }
    break;
  case 'C':
    if (arguments.args[0][8] == 'H')
      switch (arguments.args[0][10]) {	//ruletable
      case 'a':
	CT_EMH_add (&arguments);
	break;			//add
      case 'd':
	CT_EMH_del (arguments);
	break;			//del
      case 'u':
	CT_EMH_update (arguments);
	break;			//del
      case 'c':
	CT_EMH_clear (arguments);
	break;			//clear all
      case 'p':
	CT_EMH_print (arguments);
	break;			//print
      default:
	printf ("unknown subaction\n");
	break;
      }
    break;
  case 'R':
    if (arguments.args[0][8] == 'H')
      switch (arguments.args[0][10]) {	//ruletable
      case 'a':
	RT_EMH_add (&arguments);
	break;			//add
      case 'd':
	RT_EMH_del (arguments);
	break;			//del
      case 'u':
	RT_EMH_update (arguments);
	break;			//del
      case 'c':
	RT_EMH_clear (arguments);
	break;			//clear all
      case 'p':
	RT_EMH_print (arguments);
	break;			//print
      default:
	printf ("unknown subaction\n");
	break;
      }
    else
      switch (arguments.args[0][10]) {	//ruletable
      case 'a':
	RT_EMA_add (&arguments);
	break;			//add
      case 'd':
	RT_EMA_del (arguments);
	break;			//del
      case 'u':
	RT_EMA_update (arguments);
	break;			//del
      case 'c':
	RT_EMA_clear (arguments);
	break;			//clear all
      case 'p':
	RT_EMA_print (arguments);
	break;			//print
      default:
	printf ("unknown subaction\n");
	break;
      }
    break;
  default:			//mastertable
    switch (arguments.args[0][0]) {	//ruletable
    case 'a':
      FC_MasterT_add (&arguments);
      break;			//add
    case 'd':
      FC_MasterT_del_entry (&arguments);
      break;			//del
    case 'u':
      FC_MasterT_update (arguments.ID);
      break;			//del
    case 'c':
      FC_MasterT_clear ();
      break;			//clear all
    case 'p':
      FC_MasterT_print (&arguments);
      break;			//print
    default:
      printf ("unknown subaction\n");
      break;
    }
    break;
  }
munmap(map_base,MAP_SIZE);
munmap(map_base_shadow,MAP_SIZE);
munmap(map_base_master,MASTERTABLE_length * sizeof (struct arguments));
close(fd_shadow);
close(fd_master);
lock.l_type=F_UNLCK;
fcntl(fd, F_SETLK, &lock);//release filelock
close(fd);
  return 0;
}
