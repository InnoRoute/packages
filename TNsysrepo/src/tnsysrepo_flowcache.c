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
#include <sys/mman.h>
#include "Flowcache/mastertable.h"
#include "Flowcache/tnlibflowtable.h"
#include "Flowcache/tnlibaccdp.h"
#include "Flowcache/flowtableactions.h"
#include "Flowcache/accdpactions.h"
#include "tnsysrepo_flowcache.h"

typedef struct changes_s
{
  pthread_mutex_t mutex;
  pthread_cond_t cond;
  int count;
} changes_t;
void TN_sysrepo_flowcache_init(){



}
void TN_sysrepo_flowcache_exit(){



}

static void
TN_sysrepo_flowcache_handle_change (sr_session_ctx_t * session, sr_change_oper_t op, sr_val_t * old_val, sr_val_t * new_val, int64_t * changeid){
printf ("__FUNCTION__ = %s\n", __FUNCTION__);

sr_xpath_ctx_t xp_ctx = { 0 };

  int rc = SR_ERR_OK;
  sr_val_iter_t *iter = NULL;
  sr_val_t *value = NULL;

  switch (op) {
  case SR_OP_CREATED:
    if (NULL != new_val) {
      printf ("CREATED: ");
      sr_print_val (new_val);
      printf ("new value id:%s\n", sr_xpath_key_value (new_val->xpath, "flows", "id", &xp_ctx));
      
    }
    break;
  case SR_OP_DELETED:
    if (NULL != old_val) {
      printf ("DELETED: ");
      sr_print_val (old_val);
      printf ("del value id:%s\n", sr_xpath_key_value (new_val->xpath, "flows", "id", &xp_ctx));
    }
    break;
  case SR_OP_MODIFIED:
    if (NULL != old_val && NULL != new_val) {
      printf ("MODIFIED: ");
      printf ("old value");
      sr_print_val (old_val);
      printf ("new value");
      sr_print_val (new_val);
      printf ("new value id:%s\n", sr_xpath_key_value (new_val->xpath, "flows", "id", &xp_ctx));
      //printf ("full changed subtree:\n");
      //sprintf(xpath,"/TNflowtable:TNflowtable/flows[id='%s']",sr_xpath_key_value(new_val->xpath,"flows","id",&xp_ctx));
      

    }
    break;
  case SR_OP_MOVED:
    if (NULL != new_val) {
      printf ("MOVED: %s after %s", new_val->xpath, NULL != old_val ? old_val->xpath : NULL);
      
    }
    break;
  }


}
int
TN_sysrepo_fc_change_cb (sr_session_ctx_t * session, const char *module_name, sr_notif_event_t ev, void *private_ctx)
{printf ("__FUNCTION__ = %s\n", __FUNCTION__);

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
  rc = sr_get_changes_iter (session, "/TNsysrepo:TNtnflowtable", &it);
  if (SR_ERR_OK != rc) {
    puts ("sr get changes iter failed");
    goto cleanup;
  }
  ch->count = 0;
  while (SR_ERR_OK == (rc = sr_get_change_next (session, it, &op, &old_val, &new_val))) {
    TN_sysrepo_flowcache_handle_change (session, op, old_val, new_val, &changeid);
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


