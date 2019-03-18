/**
*@file tnsysrepo.c
*@brief functions to handle sysrepo connection
*M.Ulbricht 2019
* using example code from https://github.com/sysrepo/sysrepo/tree/master/examples
**/
/* 
 * 
 * This program is free software: you can redistribute it and/or modify  
 * it under the terms of the GNU General Public License as published by  
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License 
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sysrepo.h>
#include <sysrepo/values.h>
#include <sysrepo/plugins.h>
#include <sysrepo/xpath.h>
#include <signal.h>
#include <pthread.h>
#include <linux/ptp_clock.h>
#include <linux/pps.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <unistd.h>
#include "tnsysrepo.h"
#include "tnsysrepo_flowcache.h"
#include "tnsysrepo_tas.h"

uint8_t TN_sr_verbose = 0;

volatile int exit_application = 0;
int clkid;
#define XPATH_MAX_LEN 100
typedef struct changes_s
{
  pthread_mutex_t mutex;
  pthread_cond_t cond;
  int count;
} changes_t;

static void
sigint_handler (int signum)
{
  exit_application = 1;
}

//************************************************************************************************************************************
/**
*sysrepo init function
*
*/

void
TN_sysrepo_init (uint8_t verb)
{
  TN_sr_verbose = verb;
  TN_sysrepo_tas_init ();
  TN_sysrepo_flowcache_init ();
  verblog printf ("__FUNCTION__ = %s\n", __FUNCTION__);

  changes_t sync = {.mutex = PTHREAD_MUTEX_INITIALIZER,.cond = PTHREAD_COND_INITIALIZER,.count = 0 };
  sr_subscription_ctx_t *subscription = NULL;
  int rc = SR_ERR_OK;
  sr_conn_ctx_t *connection = NULL;
  sr_session_ctx_t *session = NULL;

  /* connect to sysrepo */
  rc = sr_connect ("example_application", SR_CONN_DEFAULT, &connection);
  if (SR_ERR_OK != rc) {
    fprintf (stderr, "Error by sr_connect: %s\n", sr_strerror (rc));
    goto cleanup;
  }

  /* start session */
  rc = sr_session_start (connection, SR_DS_STARTUP, SR_SESS_DEFAULT, &session);
  if (SR_ERR_OK != rc) {
    fprintf (stderr, "Error by sr_session_start: %s\n", sr_strerror (rc));
    goto cleanup;
  }
  // register callbacks here:
  rc = sr_subtree_change_subscribe (session, "/TNsysrepo:TNflowtable", TN_sysrepo_fc_change_cb, &sync,
				    0, SR_SUBSCR_DEFAULT | SR_SUBSCR_APPLY_ONLY, &subscription);
  if (SR_ERR_OK != rc) {
    fprintf (stderr, "Error by sr_module_change_subscribe TNflowtable: %s\n", sr_strerror (rc));
    goto cleanup;
  }
  rc = sr_subtree_change_subscribe (session, "/TNsysrepo:TNtas", TN_sysrepo_tas_change_cb, &sync,
				    0, SR_SUBSCR_DEFAULT | SR_SUBSCR_APPLY_ONLY, &subscription);
  if (SR_ERR_OK != rc) {
    fprintf (stderr, "Error by sr_module_change_subscribe TNtas: %s\n", sr_strerror (rc));
    goto cleanup;
  }

  /* loop until ctrl-c is pressed / SIGINT is received */
  signal (SIGINT, sigint_handler);
  signal (SIGPIPE, SIG_IGN);
  while (!exit_application) {
    sleep (1000);		/* or do some more useful work... */
  }
  printf ("Application exit requested, exiting.\n");

cleanup:
  TN_sysrepo_tas_exit ();
  TN_sysrepo_flowcache_exit ();
  if (NULL != subscription) {
    sr_unsubscribe (session, subscription);
  }
  if (NULL != session) {
    sr_session_stop (session);
  }
  if (NULL != connection) {
    sr_disconnect (connection);
  }
}
