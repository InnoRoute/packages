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
#include <sys/mman.h>
#include <fcntl.h>
#include <time.h>
#include <unistd.h>
#include "tnsysrepo_config.h"
#include "tn_env.h"
#include "TAS/tnlibtsnctl.h"
#define XPATH_MAX_LEN 100
#define MAP_SIZE 16384000UL


typedef struct changes_s
{
  pthread_mutex_t mutex;
  pthread_cond_t cond;
  int count;
} changes_t;
int
TN_sysrepo_config_change_cb (sr_session_ctx_t * session, const char *module_name, sr_notif_event_t ev, void *private_ctx)
{
  printf ("__FUNCTION__ = %s\n", __FUNCTION__);
  changes_t *ch = (changes_t *) private_ctx;
  sr_change_iter_t *it = NULL;
  int rc = SR_ERR_OK;
  int64_t changeid = -1;
  sr_change_oper_t op;
  sr_val_t *new_val = NULL;
  sr_val_t *old_val = NULL;

  if (SR_EV_VERIFY != ev) {
    pthread_mutex_lock (&ch->mutex);
  }
  rc = sr_get_changes_iter (session, "/TNsysrepo:TNconfig", &it);
  if (SR_ERR_OK != rc) {
    puts ("sr get changes iter failed");
    goto cleanup;
  }
  ch->count = 0;
  while (SR_ERR_OK == (rc = sr_get_change_next (session, it, &op, &old_val, &new_val))) {
    TN_sysrepo_config_handle_change (session, op, old_val, new_val, &changeid);
    sr_free_val (old_val);
    sr_free_val (new_val);
    ch->count++;
  }
cleanup:
  sr_free_change_iter (it);
  if (SR_EV_VERIFY != ev) {
    pthread_cond_signal (&ch->cond);
    pthread_mutex_unlock (&ch->mutex);
  }
  return SR_ERR_OK;




}

static void
TN_sysrepo_config_handle_change (sr_session_ctx_t * session, sr_change_oper_t op, sr_val_t * old_val, sr_val_t * new_val, int64_t * changeid)
{
//printf ("__FUNCTION__ = %s\n", __FUNCTION__);

  sr_xpath_ctx_t xp_ctx = { 0 };
  uint8_t change_id = 0;
  int rc = SR_ERR_OK;
  sr_val_iter_t *iter = NULL;
  sr_val_t *value = NULL;

  switch (op) {
  case SR_OP_CREATED:
    if (NULL != new_val) {
      printf ("CREATED: ");
      sr_print_val (new_val);
	if ((new_val->type != SR_CONTAINER_T) && (new_val->type != SR_CONTAINER_PRESENCE_T) && (new_val->type != SR_LIST_T))TN_sysrepo_config_apply (session,new_val);
    }
    break;
  case SR_OP_DELETED:
    if (NULL != old_val) {
      printf ("DELETED: ");
      sr_print_val (old_val);

    }
    break;
  case SR_OP_MODIFIED:
    if (NULL != old_val && NULL != new_val) {
      printf ("MODIFIED: ");
      printf ("old value");
      sr_print_val (old_val);
      printf ("new value");
      sr_print_val (new_val);
if ((new_val->type != SR_CONTAINER_T) && (new_val->type != SR_CONTAINER_PRESENCE_T) && (new_val->type != SR_LIST_T))TN_sysrepo_config_apply (session,new_val);

    }
    break;
  case SR_OP_MOVED:
    if (NULL != new_val) {
      printf ("MOVED: %s after %s", new_val->xpath, NULL != old_val ? old_val->xpath : NULL);

    }
    break;
  }


}

void
TN_sysrepo_config_apply (sr_session_ctx_t * session,sr_val_t *value){
	if(sr_str_ends_with( value->xpath , "Ethernet_Switch_enable" )){
		if(value->data.bool_val){
			     
   	INR_PCI_BAR1_write(1,(C_BASE_ADDR_ETH_SW_0<<8)|C_SUB_ADDR_ETH_SW_COMMON);
   	INR_PCI_BAR1_write(1,(C_BASE_ADDR_ETH_SW_1<<8)|C_SUB_ADDR_ETH_SW_COMMON);
   	sleep(1);
   	INR_PCI_BAR1_write(10,(C_BASE_ADDR_ETH_SW_0<<8)|C_SUB_ADDR_ETH_SW_AGEING);
   	INR_PCI_BAR1_write(10,(C_BASE_ADDR_ETH_SW_1<<8)|C_SUB_ADDR_ETH_SW_AGEING);
            
			}else{
		INR_PCI_BAR1_write(0,(C_BASE_ADDR_ETH_SW_0<<8)|C_SUB_ADDR_ETH_SW_COMMON);
   	INR_PCI_BAR1_write(0,(C_BASE_ADDR_ETH_SW_1<<8)|C_SUB_ADDR_ETH_SW_COMMON);
				}
	}
	if(sr_str_ends_with( value->xpath , "Ethernet_Switch_aging" )){
		
   	INR_PCI_BAR1_write(value->data.uint8_val,(C_BASE_ADDR_ETH_SW_0<<8)|C_SUB_ADDR_ETH_SW_AGEING);
   	INR_PCI_BAR1_write(value->data.uint8_val,(C_BASE_ADDR_ETH_SW_1<<8)|C_SUB_ADDR_ETH_SW_AGEING);
            
			
	}





}
