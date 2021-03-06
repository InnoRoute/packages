/**
*@file tnflowtable.c
*@brief userspace flowtable acces tool
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
#include "accdpactions.h"
#include "tnlibaccdp.h"
#include <argp.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <time.h>


#define MAP_SIZE 16384000UL
//#define MAP_MASK (MAP_SIZE - 1)
const char *argp_program_version = "tnflowcache 0.8";
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
  {"bulk", 'Q', "", 0, "make n entries, justfor testing!"},
  {"numberout", 'n', 0, 0, "Output number of tableentry"},
  {"ID", 'i', "", 0, "ID of entry"},
  {"COUNT", 'c', "", 0, "number of entrys to do something"},
  {"PQUEUE", 'q', "", 0, "select a processor queue"},
  {"memdump", 'm', 0, 0, "print out TNbar1 accesses"},
  {"memdump", 'M', 0, 0, "print out TNbar1 accesses"},
  {0, 0, 0, 0, "Rule options:", 1},
  {"AUTOHASH", 'a', 0, 0, "automatic generate and write hash to hashtable"},
  {"AUTOACT", 'z', 0, 0, "automatic write action to Actiontable"},
  {"RULEPOINTER", 'r', "", 0, "Pointer to rule"},
  {"EMH_HASH_OVERLAY", 'o', "", 0, "EMH_HASH_OVERLAY"},
  {"RULETYPE", 't', "", 0, "Type of Rule"},
  {"HASH_EMH", 'H', "", 0, "Hash value for EMH table"},
  {"RULEPRIO", 'p', "", 0, "Priority of Rule"},
  {"RULEACTION", 'Y', "", 0, "Action which is performed by rule"},
  //{"RULENEXT", 'n', "", 0, "Has rule an extension?"},
  {0, 0, 0, 0, "Rule Type 1 options:", 2},
#if EMH_hash_revision == 1
  {"RC_MAC_SRC", 'S', "", 0, "source Mac address (hex Format)"},
  {"RC_MAC_DST", 'D', "", 0, "destination Mac address (hex Format)"},
  {"RC_VLAN", 'V', "", 0, "ID of VLAN"},
  {"RC_ETHERTYPE", 'E', "", 0, "ETHERTYPE"},
#endif
#if EMH_hash_revision == 2
  {"RC_PROTO", 'P', "", 0, "Protocol type of packet"},
  {"RC_IPv4_SRC", 'C', "", 0, "IPv4 source address"},
  {"RC_IPv4_DST", 'T', "", 0, "IPv4 destination address"},
  {"RC_MAC_DST", 'D', "", 0, "destination Mac address (hex Format)"},
  {"RC_ETHERTYPE", 'E', "", 0, "ETHERTYPE"},
#endif
#if EMH_hash_revision == 3
  {"RC_EMA_INPORT", 'I', "", 0, "Ingressport of packet"},
#endif
#if EMH_hash_revision == 5
  {"RC_EMA_INPORT", 'I', "", 0, "Ingressport of packet"},
#endif
#if EMH_hash_revision == 6
  {"RC_PORT_DST", 'A', "", 0, "UDP/TCP destination port"},
#endif
#if EMH_hash_revision == 7
  {"RC_PORT_DST", 'A', "", 0, "UDP/TCP destination port"},
#endif
#if EMH_hash_revision == 8
  {"RC_EMA_INPORT", 'I', "", 0, "Ingressport of packet"},
#endif
#if EMH_hash_revision == 9
  {"RC_EMA_INPORT", 'I', "", 0, "Ingressport of packet"},
#endif
#if EMH_hash_revision == 11
  {"RC_EMA_INPORT", 'I', "", 0, "Ingressport of packet"},
  {"RC_VLAN", 'V', "", 0, "ID of VLAN"},
#endif
#if EMH_hash_revision == 12
  {"RC_VLAN", 'V', "", 0, "ID of VLAN"},
#endif
  {0, 0, 0, 0, "Rule Type 2 options:", 3},
#if EMH_hash_revision == 1
  {"RC_IPv4_SRC", 'C', "", 0, "IPv4 source address"},
  {"RC_IPv4_DST", 'T', "", 0, "IPv4 destination address"},
  {"RC_PROTO", 'P', "", 0, "Protocol type of packet"},
  {"RC_PORT_SRC", 'R', "", 0, "UDP/TCP source port"},
  {"RC_PORT_DST", 'A', "", 0, "UDP/TCP destination port"},
#endif
#if EMH_hash_revision == 2
  {"RC_IPv4_DST", 'T', "", 0, "IPv4 destination address"},
  {"RC_IPv4_SRC", 'C', "", 0, "IPv4 source address"},
  {"RC_PORT_DST", 'A', "", 0, "UDP/TCP destination port"},
  {"RC_MAC_DST", 'D', "", 0, "destination Mac address (hex Format)"},
  {"RC_ETHERTYPE", 'E', "", 0, "ETHERTYPE"},
#endif
#if EMH_hash_revision == 3
  {"RC_EMA_VLAN_PRIO", 'U', "", 0, "Priority of VLAN"},
#endif
#if EMH_hash_revision == 5
  {"RC_EMA_VLAN_PRIO", 'U', "", 0, "Priority of VLAN"},
#endif
#if EMH_hash_revision == 6
  {"RC_EMA_VLAN_PRIO", 'U', "", 0, "Priority of VLAN"},
#endif
#if EMH_hash_revision == 7
  {"RC_EMA_VLAN_PRIO", 'U', "", 0, "Priority of VLAN"},
#endif
#if EMH_hash_revision == 8
  {"RC_EMA_VLAN_PRIO", 'U', "", 0, "Priority of VLAN"},
#endif
#if EMH_hash_revision == 9
  {"RC_EMA_VLAN_PRIO", 'U', "", 0, "Priority of VLAN"},
#endif
#if EMH_hash_revision == 11
  {"RC_EMA_INPORT", 'I', "", 0, "Ingressport of packet"},
  {"RC_MAC_DST", 'D', "", 0, "destination Mac address (hex Format)"},
#endif
#if EMH_hash_revision == 12
  {"RC_MAC_DST", 'D', "", 0, "destination Mac address (hex Format)"},
#endif
  {0, 0, 0, 0, "Rule Type 3 options:", 4},
#if EMH_hash_revision == 2
  {"RC_IPv4_SRC", 'C', "", 0, "IPv4 source address"},
  {"RC_IPv4_DST", 'T', "", 0, "IPv4 destination address"},
  {"RC_MAC_DST", 'D', "", 0, "destination Mac address (hex Format)"},
#endif
#if EMH_hash_revision == 3
  {"RC_VLAN", 'V', "", 0, "ID of VLAN"},
  {"RC_MAC_DST", 'D', "", 0, "destination Mac address (hex Format)"},
#endif
#if EMH_hash_revision == 5
  {"RC_VLAN", 'V', "", 0, "ID of VLAN"},
  {"RC_MAC_DST", 'D', "", 0, "destination Mac address (hex Format)"},
#endif
#if EMH_hash_revision == 6
  {"RC_VLAN", 'V', "", 0, "ID of VLAN"},
  {"RC_MAC_DST", 'D', "", 0, "destination Mac address (hex Format)"},
#endif
#if EMH_hash_revision == 7
  {"RC_VLAN", 'V', "", 0, "ID of VLAN"},
  {"RC_MAC_DST", 'D', "", 0, "destination Mac address (hex Format)"},
#endif
#if EMH_hash_revision == 8
  {"RC_VLAN", 'V', "", 0, "ID of VLAN"},
  {"RC_MAC_DST", 'D', "", 0, "destination Mac address (hex Format)"},
#endif
#if EMH_hash_revision == 9
  {"RC_VLAN", 'V', "", 0, "ID of VLAN"},
  {"RC_MAC_DST", 'D', "", 0, "destination Mac address (hex Format)"},
#endif
#if EMH_hash_revision == 10
  {"RC_MAC_DST", 'D', "", 0, "destination Mac address (hex Format)"},
#endif
#if EMH_hash_revision == 11
  {"RC_EMA_INPORT", 'I', "", 0, "Ingressport of packet"},
  {"RC_ETHERTYPE", 'E', "", 0, "ETHERTYPE"},
  {"RC_IPv4_DST", 'T', "", 0, "IPv4 destination address"},
#endif
#if EMH_hash_revision == 12
  {"RC_MAC_DST", 'D', "", 0, "destination Mac address (hex Format)"},
  {"RC_VLAN", 'V', "", 0, "ID of VLAN"},
  
#endif
  {0, 0, 0, 0, "Rule Type 4 options:", 5},
#if EMH_hash_revision == 2
  {"RC_MAC_DST", 'D', "", 0, "destination Mac address (hex Format)"},
#endif
#if EMH_hash_revision == 3
  {"RC_VLAN", 'V', "", 0, "ID of VLAN"},
  {"RC_MAC_DST", 'D', "", 0, "destination Mac address (hex Format)"},
  {"RC_EMA_VLAN_PRIO", 'U', "", 0, "Priority of VLAN"},
#endif
#if EMH_hash_revision == 5
  {"RC_EMA_INPORT", 'I', "", 0, "Ingressport of packet"},
  {"RC_ETHERTYPE", 'E', "", 0, "ETHERTYPE"},
#endif
#if EMH_hash_revision == 6
  {"RC_EMA_INPORT", 'I', "", 0, "Ingressport of packet"},
  {"RC_ETHERTYPE", 'E', "", 0, "ETHERTYPE"},
#endif
#if EMH_hash_revision == 9
  {"RC_PORT_DST", 'A', "", 0, "UDP/TCP destination port"},
  {"RC_IPv4_DST", 'T', "", 0, "IPv4 destination address"},
  {"RC_PROTO", 'P', "", 0, "Protocol type of packet"},
  {"RC_ETHERTYPE", 'E', "", 0, "ETHERTYPE"},
#endif
#if EMH_hash_revision == 11
  {"RC_EMA_INPORT", 'I', "", 0, "Ingressport of packet"},
  {"RC_ETHERTYPE", 'E', "", 0, "ETHERTYPE"},
  {"RC_IPv4_DST", 'T', "", 0, "IPv4 destination address"},
  {"RC_PORT_DST", 'A', "", 0, "UDP/TCP destination port"},
#endif
#if EMH_hash_revision == 12
  {"RC_MAC_DST", 'D', "", 0, "destination Mac address (hex Format)"},
  {"RC_VLAN", 'V', "", 0, "ID of VLAN"},
  {"RC_EMA_VLAN_PRIO", 'U', "", 0, "Priority of VLAN"},
  
#endif
  {0, 0, 0, 0, "EMA_Rule Type options:", 6},
  {"RC_IPv4_SRC", 'C', "", 0, "IPv4 source address"},
  {"RC_IPv4_DST", 'T', "", 0, "IPv4 destination address"},
  {"RC_PROTO", 'P', "", 0, "Protocol type of packet"},
  {"RC_PORT_SRC", 'R', "", 0, "UDP/TCP source port"},
  {"RC_PORT_DST", 'A', "", 0, "UDP/TCP destination port"},
  {"RC_MAC_SRC", 'S', "", 0, "source Mac address (hex Format)"},
  {"RC_MAC_DST", 'D', "", 0, "destination Mac address (hex Format)"},
  {"RC_VLAN", 'V', "", 0, "ID of VLAN"},
  {"RC_ETHERTYPE", 'E', "", 0, "ETHERTYPE"},
  {"RC_EMA_VLAN_PRIO", 'U', "", 0, "Priority of VLAN"},
  {"RC_EMA_TOS", 'W', "", 0, "ToS field"},
  {"RC_EMA_INPORT", 'I', "", 0, "Ingressport of packet"},
  {0, 0, 0, 0, "ActionTable_Rule Type options:", 7},
  {"AT_OutPort", 'O', "", 0, "Output Port"},
  {"AT_Bad", 'b', "", 0, "Bad assignment"},
  {"AT_Bad_reason", 'B', "", 0, "Bad reason"},
  {"AT_Cut", 'X', "", 0, "Cut through assingment"},
  {0, 0, 0, 0, "Acceleration Datapath specific:", 8},
  {"NAL_ID", 'N', "", 0, "H265 video layer NAL_ID"},
    {0, 0, 0, 0, "Field overwriting:", 9},
  {"OVERWRITE_IPv4_SRC", 'd', "", 0, "Overwrite IPv4 source address"},
  {"OVERWRITE_IPv4_DST", 'e', "", 0, "Overwrite IPv4 destination address"},
  {"OVERWRITE_PORT_SRC", 'f', "", 0, "Overwrite UDP/TCP source port"},
  {"OVERWRITE_PORT_DST", 'g', "", 0, "Overwrite UDP/TCP destination port"},
  {"OVERWRITE_MAC_SRC", 'j', "", 0, "Overwrite source Mac address (hex Format)"},
  {"OVERWRITE_MAC_DST", 'k', "", 0, "Overwrite destination Mac address (hex Format)"},
  
  {"IPV_wildcard", 'l', "", 0, "IPV_wildcard"},
  {"QCI_gate_ID", 'x', "", 0, "QCI_gate_ID"},

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
  case 'o':
  	arguments->EMH_HASH_OVERLAY=strtoul (arg, 0, 0);
  	break;
  case 'l':
  	arguments->QCI_IPV_wildcard=strtoul (arg, 0, 0);
  	arguments->QCI_enable=1;
  	break;
  case 'x':
  	arguments->QCI_gate_ID=strtoul (arg, 0, 0);
  	arguments->QCI_enable=1;
  	arguments->dohave_QCI_gate_ID=1;
  	break;  
  case 'a':
    arguments->HASH.gauto = 1;
    break;
  case 'd':
    arguments->IPv4_SRC_OVERWRITE = htobe32 (inet_addr (arg));
    nibbletwist(&arguments->IPv4_SRC_OVERWRITE, 4);
    arguments->dohave_IPv4_SRC_OVERWRITE = 1;
    arguments->OPTION_COUNT++;
    break;
  case 'e':
    arguments->IPv4_DST_OVERWRITE = htobe32 (inet_addr (arg));
    nibbletwist(&arguments->IPv4_DST_OVERWRITE, 4);
    arguments->dohave_IPv4_DST_OVERWRITE = 1;
    arguments->OPTION_COUNT++;
    break;
  case 'f':
    arguments->PORT_SRC_OVERWRITE = strtoul (arg, 0, 0);
    nibbletwist(&arguments->PORT_SRC_OVERWRITE, 2);
    arguments->dohave_PORT_SRC_OVERWRITE = 1;
    arguments->OPTION_COUNT++;
    
    break;
  case 'g':
    arguments->PORT_DST_OVERWRITE = strtoul (arg, 0, 0);
    nibbletwist(&arguments->PORT_DST_OVERWRITE, 2);
    arguments->dohave_PORT_DST_OVERWRITE = 1;
    arguments->OPTION_COUNT++;
    break;
  case 'j':
    arguments->MAC_SRC_OVERWRITE = strtoull (arg, 0, 0);
    hwaddr_aton2 (arg, &arguments->MAC_SRC_OVERWRITE);
    arguments->MAC_SRC_OVERWRITE = htobe64 (arguments->MAC_SRC_OVERWRITE) >> 16;
    nibbletwist(&arguments->MAC_SRC_OVERWRITE, 6);
    arguments->dohave_MAC_SRC_OVERWRITE = 1;
    arguments->OPTION_COUNT++;
    //printf("%llx\n",arguments->MAC_SRC_OVERWRITE);
    break;
  case 'k':
    arguments->MAC_DST_OVERWRITE = strtoull (arg, 0, 0);
    hwaddr_aton2 (arg, &arguments->MAC_DST_OVERWRITE);
    arguments->MAC_DST_OVERWRITE = htobe64 (arguments->MAC_DST_OVERWRITE) >> 16;
    nibbletwist(&arguments->MAC_DST_OVERWRITE, 6);
    arguments->dohave_MAC_DST_OVERWRITE = 1;
    arguments->OPTION_COUNT++;
    //printf("%llx\n",arguments->MAC_DST_OVERWRITE);
    break;
  case 'z':
    arguments->autoaction = 1;
    break;
  case 'm':
    memdump_en ();
    break;
  case 'M':
    arguments->machinereadable=1;
    break;
  case 'n':
    arguments->numberout = 1;
    arguments->dohave_numberout = 1;
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
  case 'Q':
    arguments->bulk = strtoul (arg, 0, 0);
    break;
  case 'O':
  	arguments->OutPort = strtoul (arg, 0, 0);
    arguments->OutPort_enable = 1;
    arguments->dohave_OutPort = 1;
    arguments->dohave_OutPort_enable = 1;
    arguments->OPTION_COUNT++;
    break;
  case 'b':
    arguments->Bad_enable = 1;
    arguments->BadValue = strtoul (arg, 0, 0);
    arguments->dohave_Bad_enable = 1;
    arguments->dohave_BadValue = 1;
    arguments->OPTION_COUNT++;
    break;
  case 'B':
    arguments->BadReason = strtoul (arg, 0, 0);
    arguments->Bad_enable = 1;
    arguments->dohave_BadReason = 1;
    arguments->dohave_Bad_enable = 1;
    arguments->OPTION_COUNT++;
    break;
  case 'N':
    arguments->NAL_ID = strtoul (arg, 0, 0);
    arguments->dohave_NAL_ID = 1;
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
    /*case 'n':
       arguments->NEXT_BIT = strtoul (arg, 0, 0);
       break; */
  case 'c':
    arguments->COUNT = strtoull (arg, 0, 0);
    break;
  case 'S':
  	arguments->matchcount++;
    arguments->MAC_SRC = strtoull (arg, 0, 0);
    hwaddr_aton2 (arg, &arguments->MAC_SRC);
    arguments->MAC_SRC = htobe64 (arguments->MAC_SRC) >> 16;
    arguments->MASK_MAC_SRC = 0;
    arguments->dohave_MAC_SRC = 1;
    arguments->dohave_MASK_MAC_SRC = 1;
    arguments->OPTION_COUNT++;
    break;
  case 'D':
  	arguments->matchcount++;
    arguments->MAC_DST = strtoull (arg, 0, 0);
    hwaddr_aton2 (arg, &arguments->MAC_DST);
    arguments->MAC_DST = htobe64 (arguments->MAC_DST) >> 16;
    arguments->MASK_MAC_DST = 0;
    arguments->dohave_MAC_DST = 1;
    arguments->dohave_MASK_MAC_DST = 1;
    arguments->OPTION_COUNT++;
    break;

  case 'H':
    arguments->HASH.EMH = strtoull (arg, 0, 0);
    break;
  case 'E':
  	arguments->matchcount++;
    arguments->ETHERTYPE = strtoull (arg, 0, 0);
    arguments->MASK_ETHERTYPE = 0;
    arguments->dohave_ETHERTYPE = 1;
    arguments->dohave_MASK_ETHERTYPE = 1;
    arguments->OPTION_COUNT++;
    break;
  case 'V':
  	arguments->matchcount++;
    arguments->VLAN_ID = strtoul (arg, 0, 0);
    arguments->MASK_VLAN_ID = 0;
    arguments->dohave_VLAN_ID = 1;
    arguments->dohave_MASK_VLAN_ID = 1;
    arguments->OPTION_COUNT++;
    break;
  case 'q':
    arguments->PQUEUE = strtoul (arg, 0, 0);
    if (arguments->PQUEUE)
      arguments->dohave_PQUEUE = 1;
    else
      arguments->dohave_PQUEUE = 0;	// have to be >0 or nothing have to happen
    arguments->OPTION_COUNT++;
    break;
  case 'W':
  	arguments->matchcount++;
    arguments->TOS = strtoul (arg, 0, 0);
    arguments->MASK_TOS = 0;
    arguments->dohave_TOS = 1;
    arguments->dohave_MASK_TOS = 1;
    arguments->OPTION_COUNT++;
    break;
  case 'U':
  	arguments->matchcount++;
    arguments->VLAN_PRIO = strtoul (arg, 0, 0);
    arguments->MASK_VLAN_PRIO = 0;
    arguments->dohave_VLAN_PRIO = 1;
    arguments->dohave_MASK_VLAN_PRIO = 1;
    arguments->OPTION_COUNT++;
    break;
  case 'I':
  	arguments->matchcount++;
    arguments->INPORT = strtoul (arg, 0, 0);
    arguments->MASK_INPORT = 0;
    arguments->dohave_INPORT = 1;
    arguments->dohave_MASK_INPORT = 1;
    arguments->OPTION_COUNT++;
    break;
  case 'C':
  	arguments->matchcount++;
    arguments->IPv4_SRC = htobe32 (inet_addr (arg));
    arguments->MASK_IPv4_SRC = 0;
    arguments->dohave_IPv4_SRC = 1;
    arguments->dohave_MASK_IPv4_SRC = 1;
    arguments->OPTION_COUNT++;
    break;
  case 'T':
  	arguments->matchcount++;
    arguments->IPv4_DST = htobe32 (inet_addr (arg));
    arguments->MASK_IPv4_DST = 0;
    arguments->dohave_IPv4_DST = 1;
    arguments->dohave_MASK_IPv4_DST = 1;
    arguments->OPTION_COUNT++;
    break;

  case 'P':
  	arguments->matchcount++;
    arguments->PROTOCOL = strtoul (arg, 0, 0);
    arguments->MASK_PROTOCOL = 0;
    arguments->dohave_PROTOCOL = 1;
    arguments->dohave_MASK_PROTOCOL = 1;
    arguments->OPTION_COUNT++;
    break;
  case 'R':
  	arguments->matchcount++;
    arguments->PORT_SRC = strtoul (arg, 0, 0);
    arguments->MASK_PORT_SRC = 0;
    arguments->dohave_PORT_SRC = 1;
    arguments->dohave_MASK_PORT_SRC = 1;
    arguments->OPTION_COUNT++;
    break;
  case 'A':
  	arguments->matchcount++;
    arguments->PORT_DST = strtoul (arg, 0, 0);
    arguments->MASK_PORT_DST = 0;
    arguments->dohave_PORT_DST = 1;
    arguments->dohave_MASK_PORT_DST = 1;
    arguments->OPTION_COUNT++;
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
static char args_doc[] =
  "action[[[HashT_[EMH|EMA]_|DP_|ActT_|RuleT_[EMH|EMA]_|CollT_EMH_][add|del|clearall|print|stat]]|[[RuleT_[EMH|EMA]|CollT_EMH]_update]";

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

  if ((fd_shadow = open ("/tmp/INR_FC_shadow.mem", O_CREAT | O_RDWR | O_SYNC, 0600)) == -1) {	//shadowmemory, because FPGA-mmi is write only
    printf ("error opening shadowmem file\n");
  }
  if ((fd_master = open ("/tmp/INR_FC_masterTable.mem", O_CREAT | O_RDWR | O_SYNC, 0600)) == -1) {	//memory to store mastertable
    printf ("error opening msterTable file\n");
  }

  ftruncate (fd_shadow, MAP_SIZE);	//if new files created, expand them
  ftruncate (fd_master, MASTERTABLE_length * sizeof (struct arguments));

  map_base = mmap (0, MAP_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);	//map files and mmi to memory
  map_base_shadow = mmap (0, MAP_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd_shadow, 0);
  map_base_master = mmap (0, MASTERTABLE_length * sizeof (struct arguments), PROT_READ | PROT_WRITE, MAP_SHARED, fd_master, 0);

  if (!map_base) {
    printf ("error mapping memory\n");
  }
  if (!map_base_shadow) {
    printf ("error mapping memory\n");
  }
  if (!map_base_master) {
    printf ("error mapping memory\n");
  }
  FCinit_EMH (map_base, map_base_shadow);
  FCinit_MasterTable (map_base_master);	//pass the memorypointer to the flowcache libs
  INR_ACCDP_init (map_base, map_base_shadow);
  struct timespec ts1, ts2;

  timespec_get (&ts1, TIME_UTC);
  if (INR_FC_check) {
    switch (arguments.args[0][0]) {	//parse commandline arguments

    case 'A':
      switch (arguments.args[0][5]) {	//actiontable
      case 'a':
	AT_add (&arguments);
	break;
      case 'd':
	AT_del (arguments);
	break;
      case 'c':
	AT_clear (arguments);
	break;
      case 'p':
	AT_print (arguments);
	break;
      default:
	printf ("unknown subaction\n");
	break;
      }
      break;
    case 'H':
      if (arguments.args[0][8] == 'H')
	if (INR_EMH_check ()) {
	  switch (arguments.args[0][10]) {	//hashtable
	  case 'a':
	    HT_EMH_add (&arguments);
	    break;
	  case 'd':
	    HT_EMH_del (arguments);
	    break;
	  case 'c':
	    HT_EMH_clear (arguments);
	    break;
	  case 'p':
	    HT_EMH_print (arguments);
	    break;
	  default:
	    printf ("unknown subaction\n");
	    break;
	  }
	}
	else {
	  printf ("EMH table is not available in this bitstream!\n");
	}
      else if (INR_EMA_check ()) {
	switch (arguments.args[0][10]) {	//hashtable
	case 'a':
	  HT_EMA_add (&arguments);
	  break;
	case 'd':
	  HT_EMA_del (arguments);
	  break;
	case 'c':
	  HT_EMA_clear ();
	  break;
	case 'p':
	  HT_EMA_print (arguments);
	  break;
	default:
	  printf ("unknown subaction\n");
	  break;
	}
      }
      else
	printf ("EMA table is not available in this bitstream\n");
      break;
    case 'C':
      if (arguments.args[0][8] == 'H')
	if (INR_collT_check ()) {
	  switch (arguments.args[0][10]) {	//ruletable
	  case 'a':
	    CT_EMH_add (&arguments);
	    break;
	  case 'd':
	    CT_EMH_del (arguments);
	    break;
	  case 'u':
	    CT_EMH_update (arguments);
	    break;
	  case 'c':
	    CT_EMH_clear (arguments);
	    break;
	  case 'p':
	    CT_EMH_print (arguments);
	    break;
	  default:
	    printf ("unknown subaction\n");
	    break;
	  }
	}
	else
	  printf ("Collision table not available in this bitstream!\n");
      break;
    case 'D':
      if (INR_accdp_check ()) {
	switch (arguments.args[0][3]) {	//AccDP
	case 'a':
	  ACCDP_add (&arguments);
	  break;
	case 'd':
	  ACCDP_del (arguments);
	  break;
//      case 'u':
//        ACCDP_update (arguments);
//        break;
	case 'c':
	  ACCDP_clear ();
	  break;
	case 'p':
	  ACCDP_print (arguments);
	  break;
	default:
	  printf ("unknown subaction\n");
	  break;
	}
      }
      else {
	printf ("Acceleration Datapath not available for loaded bitstream\n");
      }
      break;
    case 'R':
      if (arguments.args[0][8] == 'H')
	switch (arguments.args[0][10]) {	//ruletable
	case 'a':
	  RT_EMH_add (&arguments);
	  break;
	case 'd':
	  RT_EMH_del (arguments);
	  break;
	case 'u':
	  RT_EMH_update (arguments);
	  break;
	case 'c':
	  RT_EMH_clear (arguments);
	  break;
	case 'p':
	  RT_EMH_print (arguments);
	  break;
	default:
	  printf ("unknown subaction\n");
	  break;
	}
      else
	switch (arguments.args[0][10]) {	//ruletable
	case 'a':
	  RT_EMA_add (&arguments);
	  break;
	case 'd':
	  RT_EMA_del (arguments);
	  break;
	case 'u':
	  RT_EMA_update (arguments);
	  break;
	case 'c':
	  RT_EMA_clear ();
	  break;
	case 'p':
	  RT_EMA_print (arguments);
	  break;
	default:
	  printf ("unknown subaction\n");
	  break;
	}
      break;
    default:			//mastertable
      switch (arguments.args[0][0]) {
      case 'a':
	FC_MasterT_add (&arguments);
	break;
      case 'd':
	FC_MasterT_del_entry (&arguments);
	break;
      case 'u':
	FC_MasterT_update (arguments.ID);
	break;
      case 'c':
	FC_MasterT_clear ();
	break;
      case 'p':
	FC_MasterT_print (&arguments);
	break;
      case 's':
	FC_statistics_print ();
	break;
      default:
	printf ("unknown subaction\n");
	break;
      }
      break;
    }
  }

  else
    printf ("Flow cache not available in this bitstream!\n");

  timespec_get (&ts2, TIME_UTC);
  if (arguments.bulk)
    printf ("%lli\n", (ts2.tv_nsec - ts1.tv_nsec) + ((ts2.tv_sec - ts1.tv_sec) * 1000000000));

  munmap (map_base, MAP_SIZE);	//unmap files and mmi from memory
  munmap (map_base_shadow, MAP_SIZE);
  munmap (map_base_master, MASTERTABLE_length * sizeof (struct arguments));
  close (fd_shadow);		//close shadow files
  close (fd_master);
  lock.l_type = F_UNLCK;	//free semaphor
  fcntl (fd, F_SETLK, &lock);	//release filelock
  close (fd);
  return 0;
}
