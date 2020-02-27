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
#include <argp.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <time.h>
#include "tnsysrepo.h"

#define MAP_SIZE 16384000UL
//#define MAP_MASK (MAP_SIZE - 1)
const char *argp_program_version = "tnsysrepo 0.1";

const char *argp_program_bug_address = "<ulbricht@innoroute.de>";
struct flock lock;

struct arguments_main
{
  char *args[1];
  uint8_t verbose;
};
/*
   OPTIONS.  Field 1 in ARGP.
   Order of fields: {NAME, KEY, ARG, FLAGS, DOC}.
*/
static struct argp_option options[] = {	//user interface
  {0, 0, 0, 0, "General options:", 0},
  {"verbose", 'v', 0, 0, "Produce verbose output"}
};

/*
   PARSER. Field 2 in ARGP.
   Order of parameters: KEY, ARG, STATE.
*/
static error_t
parse_opt (int key, char *arg, struct argp_state *state)
{
  struct arguments_main *arguments = state->input;
  switch (key) {
  case 'v':
    arguments->verbose = 1;
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
static char args_doc[] = "start";

/*
  DOC.  Field 4 in ARGP.
  Program documentation.
*/
static char doc[] = "Trustnode sysrepo NetConf/RestConf interface";

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

  struct arguments_main arguments;	//create structure for passing comandlinearguments and settings
  memset (&arguments, 0, sizeof (struct arguments_main));	// clear structure
  argp_parse (&argp, argc, argv, 0, 0, &arguments);


  switch (arguments.args[0][0]) {	//parse commandline arguments
  case 's':
    TN_sysrepo_init (arguments.verbose);
    break;

  default:
    printf ("unknown action\n");
    break;
  }



  return 0;
}
