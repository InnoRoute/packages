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
#define FD_TO_CLOCKID(fd) ((~(clockid_t) (fd) <<3) | CLOCKFD)
#define CLOCKFD 3
#define MAP_SIZE 16384000UL
#define XPATH_MAX_LEN 100
int clkid;
struct flock lock;
int fd, fd_shadow, fd_master;
const char filename_fc[] = "/sys/bus/pci/devices/0000:01:00.0/resource1";
uint64_t *map_base, *map_base_shadow, *map_base_master;
typedef struct changes_s
{
  pthread_mutex_t mutex;
  pthread_cond_t cond;
  int count;
} changes_t;

//************************************************************************************************************************************
/**
*init ptp0
*/
void
phc_init ()
{
  int fd = open ("/dev/ptp0", O_RDWR);

  clkid = FD_TO_CLOCKID (fd);


}

//************************************************************************************************************************************
/**
*init function
*/
void
TN_sysrepo_flowcache_init ()
{
  phc_init ();
  if ((fd = open (filename_fc, O_RDWR | O_SYNC)) == -1) {
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

}

//************************************************************************************************************************************
/**
*exit function
*/
void
TN_sysrepo_flowcache_exit ()
{

  munmap (map_base, MAP_SIZE);	//unmap files and mmi from memory
  munmap (map_base_shadow, MAP_SIZE);
  munmap (map_base_master, MASTERTABLE_length * sizeof (struct arguments));
  close (fd_shadow);		//close shadow files
  close (fd_master);
  lock.l_type = F_UNLCK;	//free semaphor
  fcntl (fd, F_SETLK, &lock);	//release filelock
  close (fd);

}

//************************************************************************************************************************************
/**
*caluclate waittime using ptp0 device
*
*/
uint64_t
get_wait_time (uint64_t exec_time)
{
  struct timespec ts;
  uint64_t time = 0;
  memset (&ts, 0, sizeof (ts));
  clock_gettime (clkid, &ts);
  time = ts.tv_nsec + ts.tv_sec * 1000000000UL;
  if (time < exec_time)
    return exec_time - time;
  else
    return 0;

}

//************************************************************************************************************************************
/**
*transfer xpath informations into flowtable
*
*/
void
TN_sysrepo_flowtable_apply (sr_session_ctx_t * session, uint32_t id, int64_t * changeid)
{
  verblog printf ("__FUNCTION__ = %s\n", __FUNCTION__);
  char xpath[XPATH_MAX_LEN] = { 0 };
  verblog printf ("%s:\n", xpath);
  sr_val_t *values = NULL;
  verblog printf("id:%i\n",id);
  char tmp_strg[200] = "moep";
  size_t count_items = 0;
  int rc = SR_ERR_OK;
  struct arguments arguments;	//create flowtable structure
  memset (&arguments, 0, sizeof (arguments));	// clear structure
  verblog printf ("last change was on id=%i\n", *changeid);
  if (*changeid != id) {
    FC_MasterT_del (id + 1);
    arguments.ID = id + 1;	//count from 1 not from 0
    snprintf (xpath, XPATH_MAX_LEN, "/TNsysrepo:TNflowtable/mastertable[id='%i']/*//.", id);


    rc = sr_get_items (session, xpath, &values, &count_items);
    if (SR_ERR_OK != rc) {
      printf ("Error by sr_get_items: %s", sr_strerror (rc));
      return;
    }
    verblog printf("processing flow values\n");
    for (size_t i = 0; i < count_items; i++) {
      if ((values[i].type != SR_CONTAINER_T) && (values[i].type != SR_CONTAINER_PRESENCE_T) && (values[i].type != SR_LIST_T)) {
		 printf("argument:%i\n",i);



	  TN_sysrepo_v2a_string (arguments, values[i], "/match/in-port", INPORT, string_val)
	  TN_sysrepo_v2a_MAC (arguments, values[i], "match/ethernet-match/ethernet-source/address", MAC_SRC, string_val)
	  TN_sysrepo_v2a_MAC (arguments, values[i], "match/ethernet-match/ethernet-destination/address", MAC_DST, string_val)
	  TN_sysrepo_v2a_number (arguments, values[i], "match/vlan-match/vlan-id/vlan-id", VLAN_ID, uint16_val)
	  TN_sysrepo_v2a_number (arguments, values[i], "match/vlan-match/vlan-pcp", VLAN_PRIO, uint8_val)
	  TN_sysrepo_v2a_IP (arguments, values[i], "match/ip-match/ipv4-source", IPv4_SRC, string_val)
	  TN_sysrepo_v2a_IP (arguments, values[i], "match/ip-match/ipv4-destination", IPv4_DST, string_val)
	  TN_sysrepo_v2a_number (arguments, values[i], "match/ip-match/ip-proto", PROTOCOL, uint8_val)
	  TN_sysrepo_v2a_number (arguments, values[i], "match/udp-source-port", PORT_SRC, uint16_val)
	  TN_sysrepo_v2a_number (arguments, values[i], "match/tcp-source-port", PORT_SRC, uint16_val)
	  TN_sysrepo_v2a_number (arguments, values[i], "match/udp-destination-port", PORT_DST, uint16_val)
	  TN_sysrepo_v2a_number (arguments, values[i], "match/tcp-destination-port", PORT_DST, uint16_val)
	  TN_sysrepo_v2a_number (arguments, values[i], "match/ethernet-match/ethernet-type/type", ETHERTYPE, uint32_val)
	  TN_sysrepo_v2a_number (arguments, values[i], "match/ip-match/ip-dscp", TOS, uint8_val)
	  TN_sysrepo_v2a_outport (arguments, values[i], "output-action/output-port", OutPort, uint8_val)
	  TN_sysrepo_v2a_number (arguments, values[i], "queue-action/queue", PQUEUE, uint8_val);

	if (arguments.PQUEUE)
	  arguments.dohave_PQUEUE = 1;
	else
	  arguments.dohave_PQUEUE = 0;
	TN_sysrepo_v2a_number (arguments, values[i], "drop-action/drop-reason", BadValue, uint8_val);
	TN_sysrepo_v2a_number (arguments, values[i], "drop-action/drop-reason", BadReason, uint8_val);
	if (arguments.BadReason) {
	  arguments.Bad_enable = 1;
	  arguments.dohave_Bad_enable = 1;
	}
      }
    }
    verblog printf ("apply config\n");
    if (count_items)
      FC_MasterT_add (&arguments);
    sr_free_values (values, count_items);

  }
  else {
    verblog printf ("ignoring double change\n");
  }
  *changeid = id;
}

static void
TN_sysrepo_flowcache_handle_change (sr_session_ctx_t * session, sr_change_oper_t op, sr_val_t * old_val, sr_val_t * new_val, int64_t * changeid)
{
  verblog printf ("__FUNCTION__ = %s\n", __FUNCTION__);
  sr_xpath_ctx_t xp_ctx = { 0 };
  int rc = SR_ERR_OK;
  sr_val_iter_t *iter = NULL;
  sr_val_t *value = NULL;

  switch (op) {
  case SR_OP_CREATED:
    if (NULL != new_val) {
      verblog printf ("CREATED: ");
      verblog sr_print_val (new_val);
      verblog printf ("new value id:%s\n", sr_xpath_key_value (new_val->xpath, "mastertable", "id", &xp_ctx));
      if ((new_val->type != SR_CONTAINER_T) && (new_val->type != SR_CONTAINER_PRESENCE_T) && (new_val->type != SR_LIST_T))TN_sysrepo_flowtable_apply (session, atoi (sr_xpath_key_value (new_val->xpath, "mastertable", "id", &xp_ctx)), changeid);
    }
    break;
  case SR_OP_DELETED:
    if (NULL != old_val) {
      verblog printf ("DELETED: ");
      verblog sr_print_val (old_val);
      verblog printf ("del value id:%s\n", sr_xpath_key_value (old_val->xpath, "mastertable", "id", &xp_ctx));
      if ((old_val->type != SR_CONTAINER_T) && (old_val->type != SR_CONTAINER_PRESENCE_T) && (old_val->type != SR_LIST_T))TN_sysrepo_flowtable_apply (session, atoi (sr_xpath_key_value (old_val->xpath, "mastertable", "id", &xp_ctx)), changeid);
    }
    break;
  case SR_OP_MODIFIED:
    if (NULL != old_val && NULL != new_val) {
      verblog printf ("MODIFIED: ");
      verblog printf ("old value");
      verblog sr_print_val (old_val);
      verblog printf ("new value");
      verblog sr_print_val (new_val);
      verblog printf ("new value id:%s\n", sr_xpath_key_value (new_val->xpath, "mastertable", "id", &xp_ctx));
      if ((old_val->type != SR_CONTAINER_T) && (old_val->type != SR_CONTAINER_PRESENCE_T) && (old_val->type != SR_LIST_T))TN_sysrepo_flowtable_apply (session, atoi (sr_xpath_key_value (old_val->xpath, "mastertable", "id", &xp_ctx)), changeid);
      if ((new_val->type != SR_CONTAINER_T) && (new_val->type != SR_CONTAINER_PRESENCE_T) && (new_val->type != SR_LIST_T))TN_sysrepo_flowtable_apply (session, atoi (sr_xpath_key_value (new_val->xpath, "mastertable", "id", &xp_ctx)), changeid);
    }
    break;
  case SR_OP_MOVED:
    if (NULL != new_val) {
      verblog printf ("MOVED: %s after %s", new_val->xpath, NULL != old_val ? old_val->xpath : NULL);
      if ((old_val->type != SR_CONTAINER_T) && (old_val->type != SR_CONTAINER_PRESENCE_T) && (old_val->type != SR_LIST_T))TN_sysrepo_flowtable_apply (session, atoi (sr_xpath_key_value (old_val->xpath, "mastertable", "id", &xp_ctx)), changeid);
      if ((new_val->type != SR_CONTAINER_T) && (new_val->type != SR_CONTAINER_PRESENCE_T) && (new_val->type != SR_LIST_T))TN_sysrepo_flowtable_apply (session, atoi (sr_xpath_key_value (new_val->xpath, "mastertable", "id", &xp_ctx)), changeid);
    }
    break;
  }


}

int
TN_sysrepo_fc_change_cb (sr_session_ctx_t * session, const char *module_name, sr_notif_event_t ev, void *private_ctx)
{
  verblog printf ("__FUNCTION__ = %s\n", __FUNCTION__);
  uint32_t *gpio;
  gpio=((uint64_t)map_base)|(C_BASE_ADDR_PERIPH<<8)|C_SUB_ADDR_PERIPH_GPIOS;
  changes_t *ch = (changes_t *) private_ctx;
  sr_change_iter_t *it = NULL;
  int rc = SR_ERR_OK;
  int64_t changeid = -1;
  sr_change_oper_t op;
  sr_val_t *new_val = NULL;
  sr_val_t *old_val = NULL;



  rc = sr_get_item (session, "/TNsysrepo:TNflowtable/config_time", &new_val);
  if (SR_ERR_OK != rc) {
    puts ("get config time failed");
    goto cleanup;
  }
  sleep (get_wait_time (new_val->data.uint64_val) / 1000000000);	//sleep seconds
  usleep (get_wait_time (new_val->data.uint64_val) / 1000);	//sleep us

  if (SR_EV_VERIFY != ev) {
    pthread_mutex_lock (&ch->mutex);
  }
  rc = sr_get_changes_iter (session, "/TNsysrepo:TNflowtable/mastertable", &it);
  if (SR_ERR_OK != rc) {
    puts ("sr get changes iter failed");
    goto cleanup;
  }
  ch->count = 0;
  //pinheader
  
  *gpio=0x400; // pin0 on
  while (SR_ERR_OK == (rc = sr_get_change_next (session, it, &op, &old_val, &new_val))) {
    TN_sysrepo_flowcache_handle_change (session, op, old_val, new_val, &changeid);
    sr_free_val (old_val);
    sr_free_val (new_val);
    ch->count++;
  }
  *gpio=0x000; // pin0 off
cleanup:
  sr_free_change_iter (it);
  if (SR_EV_VERIFY != ev) {
    pthread_cond_signal (&ch->cond);
    pthread_mutex_unlock (&ch->mutex);
  }
  return SR_ERR_OK;

}
