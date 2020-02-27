#include <inttypes.h>
#include <stdio.h>
#include "INR_sysrepo.h"
#include <sysrepo.h>
#include <sysrepo/values.h>
#include <sysrepo/plugins.h>
#include <sysrepo/xpath.h>
#include <pthread.h>
#include <linux/ptp_clock.h>
#include <linux/pps.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#define maxportstorage 10
#define XPATH_MAX_LEN 100
sr_subscription_ctx_t *subscription = NULL;
sr_conn_ctx_t *connection = NULL;
sr_session_ctx_t *session = NULL;
int clkid;
typedef struct changes_s
{
  pthread_mutex_t mutex;
  pthread_cond_t cond;
  int count;
} changes_t;
int64_t pathdelay[maxportstorage]={0};
//************************************************************************************************************************************
/**
*init phc device
*
*/
void
phc_init ()
{
  int fd = open ("/dev/ptp0", O_RDWR);

  clkid = FD_TO_CLOCKID (fd);


}
//************************************************************************************************************************************

void INR_new_pathdelay(int port, int64_t pdelay){
verblog printf ("__FUNCTION__ = %s, port:%i, pdelay:%lli\n", __FUNCTION__,port,pdelay);
if(port<maxportstorage)
	{pathdelay[port]+=pdelay;
	pathdelay[port]/=2;
	}


}

//************************************************************************************************************************************
/**
*print current config
*
*/
static void
print_current_config (sr_session_ctx_t * session, const char *module_name)
{
  verblog printf ("__FUNCTION__ = %s\n", __FUNCTION__);
  sr_val_t *values = NULL;
  size_t count = 0;
  int rc = SR_ERR_OK;
  char xpath[XPATH_MAX_LEN] = { 0 };
  snprintf (xpath, XPATH_MAX_LEN, "/%s:*//.", module_name);

  rc = sr_get_items (session, xpath, &values, &count);
  if (SR_ERR_OK != rc) {
    printf ("Error by sr_get_items: %s", sr_strerror (rc));
    return;
  }
  for (size_t i = 0; i < count; i++) {
    sr_print_val (&values[i]);
  }
  sr_free_values (values, count);
  verblog printf ("done\n");
}
//********************************************************************************************
static int pathdelay_cb(const char *xpath, sr_val_t **values, size_t *values_ctn, uint64_t request_id, const char* original_xpath, void *private_ctx){
sr_val_t *vals;
int rc;
verblog printf ("__FUNCTION__ = %s\n", __FUNCTION__);
rc=sr_new_values(1,&vals);
if(SR_ERR_OK!=rc){
return rc;
}
sr_val_set_xpath(&vals[0],"/TNptp4l:TNptp4l/pathdelay");
vals[0].type=SR_INT64_T;
vals[0].data.int64_val=pathdelay[0];
*values=vals;
*values_ctn=1;
return SR_ERR_OK;	

}
//********************************************************************************************
static int currenttime_cb(const char *xpath, sr_val_t **values, size_t *values_ctn, uint64_t request_id, const char* original_xpath, void *private_ctx){
sr_val_t *vals;
int rc;
verblog printf ("__FUNCTION__ = %s\n", __FUNCTION__);
rc=sr_new_values(1,&vals);
if(SR_ERR_OK!=rc){
return rc;
}
struct timespec ts;
memset (&ts, 0, sizeof (ts));
clock_gettime (clkid, &ts);
sr_val_set_xpath(&vals[0],"/TNptp4l:TNptp4l/currenttime");
vals[0].type=SR_INT64_T;
vals[0].data.int64_val=ts.tv_nsec + ts.tv_sec * 1000000000UL;
*values=vals;
*values_ctn=1;
return SR_ERR_OK;	

}



//************************************************************************************************************************************
/**
*sysrepo init function
*
*/
void
INR_sysrepo_init()
{
  verblog printf ("__FUNCTION__ = %s\n", __FUNCTION__);
  changes_t sync = {.mutex = PTHREAD_MUTEX_INITIALIZER,.cond = PTHREAD_COND_INITIALIZER,.count = 0 };
  
  int rc = SR_ERR_OK;
  phc_init();


  /* connect to sysrepo */
  rc = sr_connect ("ptp4l", SR_CONN_DEFAULT, &connection);
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
   rc = sr_dp_get_items_subscribe(session,"/TNptp4l:TNptp4l/pathdelay",pathdelay_cb,NULL,SR_SUBSCR_CTX_REUSE,&subscription);
  if (SR_ERR_OK != rc) {
    fprintf (stderr, "Error by sr_session_start: %s\n", sr_strerror (rc));
    goto cleanup;
  }
  rc = sr_dp_get_items_subscribe(session,"/TNptp4l:TNptp4l/currenttime",currenttime_cb,NULL,SR_SUBSCR_CTX_REUSE,&subscription);
  if (SR_ERR_OK != rc) {
    fprintf (stderr, "Error by sr_session_start: %s\n", sr_strerror (rc));
    goto cleanup;
  }
  //for (int i = 1; i < argc; ++i) {
  /* read startup config */
  //printf ("\n\n ========== READING STARTUP CONFIG %s: ==========\n\n", "TNflowtable");
  //print_current_config (session, "TNflowtable");


  


  /* loop until ctrl-c is pressed / SIGINT is received */
  
return 0;
cleanup:
	INR_sysrepo_exit();
}
void
INR_sysrepo_exit(){
verblog printf ("__FUNCTION__ = %s\n", __FUNCTION__);
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
