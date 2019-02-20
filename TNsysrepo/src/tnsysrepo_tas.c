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
#include "TAS/tnlibtsnctl.h"
#include "tnsysrepo_tas.h"
#define XPATH_MAX_LEN 100
#define MAP_SIZE 16384000UL
int fd, fd_shadow;
uint8_t status;
uint64_t *map_base, *map_base_shadow;
volatile int8_t last_port_change=-1;
const char filename[] = "/sys/bus/pci/devices/0000:01:00.0/resource1";
typedef struct changes_s
{
  pthread_mutex_t mutex;
  pthread_cond_t cond;
  int count;
} changes_t;
void TN_sysrepo_tas_exit(){
		munmap(map_base, MAP_SIZE);

		close(fd_shadow);

		close(fd);
		}
void TN_sysrepo_tas_init(){
fd = open (filename, O_RDWR | O_SYNC);
	if(-1 == fd){
		status = SR_ERR_IO;
		
	}

	fd_shadow = open ("/tmp/INR_TSN_shadow.mem", O_CREAT | O_RDWR | O_SYNC, 0600);
	if(-1 == fd_shadow){
		status = SR_ERR_IO;
		
	}

	ftruncate (fd_shadow, MAP_SIZE);

	map_base = mmap (0, MAP_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if(!map_base){
		status = SR_ERR_IO;
		
	}

	map_base_shadow = mmap (0, MAP_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd_shadow, 0);
	if(!map_base_shadow){
		status = SR_ERR_IO;
		
	}

	TSN_init(map_base, map_base_shadow);



}

TN_sysrepo_tas_disable (sr_session_ctx_t * session, uint8_t id)
{
	printf ("__FUNCTION__ = %s\n", __FUNCTION__);
	struct arguments arguments;	//create structure for passing comandlinearguments and settings
	memset(&arguments,0,sizeof(arguments));// clear structure
	arguments.GATE_ENABLE=1;
	arguments.dohave_GATE_ENABLE=1;
	TN_tsn_config_change(&arguments);
}
static void
TN_sysrepo_tas_update (sr_session_ctx_t * session, uint8_t id)
{
	//printf ("__FUNCTION__ = %s\n", __FUNCTION__);
	char xpath[XPATH_MAX_LEN] = { 0 };
	int rc = SR_ERR_OK;
	sr_xpath_ctx_t xp_ctx = { 0 };
	sr_val_iter_t *iter = NULL;
	sr_val_t *value = NULL;
	sr_val_t *gcl_value = NULL;
	int16_t last_gcl_id=-1;
	uint8_t gcl_id=0;
	struct arguments arguments;	//create structure for passing comandlinearguments and settings
  	memset(&arguments,0,sizeof(arguments));// clear structure
	arguments.verbose=1;
	TN_tsn_set_verbose(1);
	snprintf (xpath, XPATH_MAX_LEN, "/TNsysrepo:TNtas/ports[id='%i']/gate_enable", id);
	rc = sr_get_item (session, xpath, &value);
	if (SR_ERR_OK != rc) {
    puts ("get gate_enable failed");
      }{    
	printf("gate_enable:%i\n",value->data.bool_val);
	if(value->data.bool_val){
	//arguments.GATE_ENABLE=1;
	//arguments.dohave_GATE_ENABLE=1;
	arguments.PORT=id;
	if(TSN_get_config (C_TM_SCHED_TAS_CONFIG_CHANGE_PENDING, id, 0)==0xeeeeeeee) return;//port cant be configured
	arguments.dohave_PORT=1;
	
		snprintf (xpath, XPATH_MAX_LEN, "/TNsysrepo:TNtas/ports[id='%i']/GCL//*", id);

		rc=sr_get_items_iter(session, xpath,&iter);
		if (SR_ERR_OK != rc) {
			puts ("sr get changes GCL iter failed");
		  }
		while(SR_ERR_OK== sr_get_item_next(session,iter,&value)){
			//sr_print_val(value);
			gcl_id=atoi(sr_xpath_key_value (value->xpath, "GCL", "id", &xp_ctx));
			
			if(last_gcl_id != gcl_id){//new entry
				arguments.ADMIN_GCL_LEN++;
				arguments.dohave_ADMIN_GCL_LEN=1;
				arguments.ID=gcl_id;
				arguments.dohave_ID=1;
				printf("GCL-id:%i\n",gcl_id);
				snprintf (xpath, XPATH_MAX_LEN, "/TNsysrepo:TNtas/ports[id='%i']/GCL[id='%i']/timeperiod", id,gcl_id);
				rc = sr_get_item (session, xpath, &gcl_value);
		
	  			if (SR_ERR_OK != rc) {
					puts ("get timeperiod failed");
		  		}else{
				printf("\ttime:%i\n",gcl_value->data.uint32_val);
				arguments.INTERVAL=gcl_value->data.uint32_val;
				arguments.dohave_INTERVAL=1;
				}
				snprintf (xpath, XPATH_MAX_LEN, "/TNsysrepo:TNtas/ports[id='%i']/GCL[id='%i']/gatestates", id,gcl_id);
				rc = sr_get_item (session, xpath, &gcl_value);
		
	  			if (SR_ERR_OK != rc) {
					puts ("get gatestates failed");
		  		}else{
				printf("\tgatestates:%i\n",gcl_value->data.uint8_val);
				arguments.GATE_STATE_VECTOR=gcl_value->data.uint8_val;
				arguments.dohave_GATE_STATE_VECTOR=1;
				}
			printf("apply gatestate\n");
			TN_tsn_GateControl_list_change(&arguments);
			printf("apply time\n");
    		TN_tsn_GateControl_TIME_list_change(&arguments);
    		printf("done\n");
			}
			last_gcl_id=gcl_id;
			
			sr_free_val(value);

		}
		
		sr_free_val_iter(iter);
		snprintf (xpath, XPATH_MAX_LEN, "/TNsysrepo:TNtas/ports[id='%i']/admin_base_time", id);
		rc = sr_get_item (session, xpath, &value);
		if (SR_ERR_OK != rc) {
		puts ("get admin base time failed");
		  }{    
		printf("ABT:%i\n",value->data.uint64_val);
		arguments.ADMIN_BASE_TIME=value->data.uint64_val;
		arguments.dohave_ADMIN_BASE_TIME=1;
		}
		snprintf (xpath, XPATH_MAX_LEN, "/TNsysrepo:TNtas/ports[id='%i']/admin_cycle_time_ext", id);
		rc = sr_get_item (session, xpath, &value);
		if (SR_ERR_OK != rc) {
		puts ("get admin_cycle_time_ext failed");
		  }{    
		printf("CT_ext:%i\n",value->data.uint32_val);
		arguments.ADMIN_CYCLE_TIME_EXT=value->data.uint32_val;
		arguments.dohave_ADMIN_CYCLE_TIME_EXT=1;
		arguments.hilscher_mode2=1;
		new_TSN_apply(&arguments);
		}
	}else{
	TN_sysrepo_tas_disable(session,id);
	
	}}

}

static void
TN_sysrepo_tas_handle_change (sr_session_ctx_t * session, sr_change_oper_t op, sr_val_t * old_val, sr_val_t * new_val, int64_t * changeid){
//printf ("__FUNCTION__ = %s\n", __FUNCTION__);

sr_xpath_ctx_t xp_ctx = { 0 };
	uint8_t change_id=0;
  int rc = SR_ERR_OK;
  sr_val_iter_t *iter = NULL;
  sr_val_t *value = NULL;

  switch (op) {
  case SR_OP_CREATED:
    if (NULL != new_val) {
      //printf ("CREATED: ");
      //sr_print_val (new_val);
      change_id=atoi(sr_xpath_key_value (new_val->xpath, "ports", "id", &xp_ctx));
      if(change_id!=last_port_change){printf ("new value id:%i\n", change_id);
      TN_sysrepo_tas_update(session,change_id);
      }
      last_port_change=change_id;
    }
    break;
  case SR_OP_DELETED:
    if (NULL != old_val) {
      //printf ("DELETED: ");
      //sr_print_val (old_val);
      change_id=atoi(sr_xpath_key_value (old_val->xpath, "ports", "id", &xp_ctx));
      if(change_id!=last_port_change){printf ("del value id:%i\n", change_id);
      TN_sysrepo_tas_disable(session,change_id);}
      last_port_change=change_id;
    }
    break;
  case SR_OP_MODIFIED:
    if (NULL != old_val && NULL != new_val) {
      //printf ("MODIFIED: ");
      //printf ("old value");
      //sr_print_val (old_val);
      //printf ("new value");
      //sr_print_val (new_val);
      change_id=atoi(sr_xpath_key_value (new_val->xpath, "ports", "id", &xp_ctx));
      if(change_id!=last_port_change){printf ("mod value id:%i\n", change_id);
      TN_sysrepo_tas_update(session,change_id);}
      last_port_change=change_id;
      

    }
    break;
  case SR_OP_MOVED:
    if (NULL != new_val) {
      printf ("MOVED: %s after %s", new_val->xpath, NULL != old_val ? old_val->xpath : NULL);
      
    }
    break;
  }


}

int TN_sysrepo_tas_change_cb (sr_session_ctx_t * session, const char *module_name, sr_notif_event_t ev, void *private_ctx){
printf ("__FUNCTION__ = %s\n", __FUNCTION__);
  changes_t *ch = (changes_t *) private_ctx;
  sr_change_iter_t *it = NULL;
  int rc = SR_ERR_OK;
  int64_t changeid = -1;
  sr_change_oper_t op;
  sr_val_t *new_val = NULL;
  sr_val_t *old_val = NULL;
  last_port_change=-1;
if (SR_EV_VERIFY != ev) {
    pthread_mutex_lock (&ch->mutex);
  }
  rc = sr_get_changes_iter (session, "/TNsysrepo:TNtas/ports", &it);
  if (SR_ERR_OK != rc) {
    puts ("sr get changes iter failed");
    goto cleanup;
  }
  ch->count = 0;
  while (SR_ERR_OK == (rc = sr_get_change_next (session, it, &op, &old_val, &new_val))) {
    TN_sysrepo_tas_handle_change (session, op, old_val, new_val, &changeid);
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
