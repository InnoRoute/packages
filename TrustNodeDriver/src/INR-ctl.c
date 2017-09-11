/**
*@file INR-ctl.c
*@brief Functions for Proc-filesystem
*@author M.Ulbricht 2015
**/
#include <linux/kernel.h>
#include <linux/export.h>
#include <linux/module.h>
//#include <stdint.h>
//#include <openvswitch/dpif.h>
#include <linux/pci.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/delay.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/proc_fs.h>
#include <linux/openvswitch.h>
#include <linux/kthread.h>
#include <asm/signal.h>
#include <linux/semaphore.h>
#include "INR.h"
#include "INR-PCI.h"
#include "INR-NW.h"
#include "INR-ctl.h"

#define PROCFS_MAX_SIZE		1024
static char procfs_buffer[PROCFS_MAX_SIZE];
//extern uint8_t INR_TN_ovscon_comand = 0;
				      /**<Command from ovs datapath*/
//extern struct INR_TN_ovscon_data INR_TN_ovscon_flow;/**<flow from ovs datapath*/
//extern struct semaphore INR_TN_ovscon_sema;/**<Semaphore for ovs datapath*/
/**
 * The size of the buffer
 *
 */
static unsigned long procfs_buffer_size = 0;
static struct proc_dir_entry *reg1, *reg2,*reg3, *INR_proc_dir;
uint8_t globalvar;/**<comment for globalvar*/
void *gBaseVirt1 = NULL;/**< pointer for start-address of bar0*/
unsigned long gBaseHdwr1,/**< start-address of bar0*/
  gBaseLen1; /**<legth of bar0*/

//*****************************************************************************************************************
/**
*  proc write funkction
*
*/
int
TNsend2cpu_write (struct file *file, const char *buffer, unsigned long count, void *data)
{
  /* get buffer size */
  procfs_buffer_size = count;
  if (procfs_buffer_size > PROCFS_MAX_SIZE) {
    procfs_buffer_size = PROCFS_MAX_SIZE;
  }

  /* write data to the buffer */
  if (copy_from_user (procfs_buffer, buffer, procfs_buffer_size)) {
    return -EFAULT;
  }
  uint32_t tmp = 0;
  sscanf (procfs_buffer, "%d", &tmp);
  INR_LOG_debug ("write %d to send2cpu\n", tmp);
  set_send2cpu(tmp);
  return procfs_buffer_size;
}

//*****************************************************************************************************************
/**
*  proc print funkction
*
*/
static int
TNsend2cpu_proc_show (struct seq_file *m, void *v)
{
  seq_printf (m, "%i\n", get_send2cpu());
  return 0;
}

//*****************************************************************************************************************
/**
*  proc open funkction
*
*/
static int
TNsend2cpu_proc_open (struct inode *inode, struct file *file)
{
  return single_open (file, TNsend2cpu_proc_show, NULL);
}

//*****************************************************************************************************************
/**
*  proc write funkction
*
*/
int
TXdbg_write (struct file *file, const char *buffer, unsigned long count, void *data)
{
  /* get buffer size */
  procfs_buffer_size = count;
  if (procfs_buffer_size > PROCFS_MAX_SIZE) {
    procfs_buffer_size = PROCFS_MAX_SIZE;
  }

  /* write data to the buffer */
  if (copy_from_user (procfs_buffer, buffer, procfs_buffer_size)) {
    return -EFAULT;
  }
  uint32_t tmp = 0;
  sscanf (procfs_buffer, "%d", &tmp);
  INR_LOG_debug ("write %d to TXdbg\n", tmp);
  set_tx_dbg(tmp);
  return procfs_buffer_size;
}

//*****************************************************************************************************************
/**
*  proc print funkction
*
*/
static int
TXdbg_proc_show (struct seq_file *m, void *v)
{
  seq_printf (m, "%i\n", get_tx_dbg());
  return 0;
}

//*****************************************************************************************************************
/**
*  proc open funkction
*
*/
static int
TXdbg_proc_open (struct inode *inode, struct file *file)
{
  return single_open (file, TXdbg_proc_show, NULL);
}
//*****************************************************************************************************************
/**
*  proc write funkction
*
*/
int
RXdbg_write (struct file *file, const char *buffer, unsigned long count, void *data)
{
  /* get buffer size */
  procfs_buffer_size = count;
  if (procfs_buffer_size > PROCFS_MAX_SIZE) {
    procfs_buffer_size = PROCFS_MAX_SIZE;
  }

  /* write data to the buffer */
  if (copy_from_user (procfs_buffer, buffer, procfs_buffer_size)) {
    return -EFAULT;
  }
  uint32_t tmp = 0;
  sscanf (procfs_buffer, "%d", &tmp);
  INR_LOG_debug ("write %d to RXdbg\n", tmp);
  set_rx_dbg(tmp);
  return procfs_buffer_size;
}

//*****************************************************************************************************************
/**
*  proc print funkction
*
*/
static int
RXdbg_proc_show (struct seq_file *m, void *v)
{
  seq_printf (m, "%i\n", get_rx_dbg());
  return 0;
}

//*****************************************************************************************************************
/**
*  proc open funkction
*
*/
static int
RXdbg_proc_open (struct inode *inode, struct file *file)
{
  return single_open (file, RXdbg_proc_show, NULL);
}


//*****************************************************************************************************************
/**
*  Init function for proc-fs
*  @brief dev PCI-device
*/
int
INR_CTL_init_proc (struct pci_dev *dev)
{
  

  static const struct file_operations TNsend2cpu = {
    .owner = THIS_MODULE,
    .open = TNsend2cpu_proc_open,
    .write = TNsend2cpu_write,
    .read = seq_read,
    .llseek = seq_lseek,
    .release = single_release,
  };
  static const struct file_operations TXdbg = {
    .owner = THIS_MODULE,
    .open = TXdbg_proc_open,
    .write = TXdbg_write,
    .read = seq_read,
    .llseek = seq_lseek,
    .release = single_release,
  };
  static const struct file_operations RXdbg = {
    .owner = THIS_MODULE,
    .open = RXdbg_proc_open,
    .write = RXdbg_write,
    .read = seq_read,
    .llseek = seq_lseek,
    .release = single_release,
  };


  reg1 = proc_create ("TNsend2cpu", 0644, NULL, &TNsend2cpu);

  if (reg1 == NULL) {
    remove_proc_entry ("TNsend2cpu", NULL);
    printk (KERN_ALERT "Error: Could not initialize /proc/%s\n", "TNsend2cpu");
    return -ENOMEM;
  }
  printk (KERN_INFO "/proc/%s created\n", "TNsend2cpu");

  reg2 = proc_create ("TN_RXdbg", 0644, NULL, &RXdbg);

  if (reg2 == NULL) {
    remove_proc_entry ("TN_RXdbg", NULL);
    printk (KERN_ALERT "Error: Could not initialize /proc/%s\n", "TN_RXdbg");
    return -ENOMEM;
  }
  printk (KERN_INFO "/proc/%s created\n", "TN_RXdbg");

  reg3 = proc_create ("TN_TXdbg", 0644, NULL, &TXdbg);

  if (reg3 == NULL) {
    remove_proc_entry ("TN_TXdbg", NULL);
    printk (KERN_ALERT "Error: Could not initialize /proc/%s\n", "TN_TXdbg");
    return -ENOMEM;
  }
  printk (KERN_INFO "/proc/%s created\n", "TN_TXdbg");



  return 0;			/* everything is ok */




}

//*****************************************************************************************************************
/**
*  Remove function for proc-fs
*  @brief dev PCI-device
*/
int
INR_CTL_remove_proc (struct pci_dev *dev)
{

  remove_proc_entry ("TNsend2cpu", NULL);
  printk (KERN_INFO "/proc/%s removed\n", "TNsend2cpu");
  remove_proc_entry ("TN_TXdbg", NULL);
  printk (KERN_INFO "/proc/%s removed\n", "TN_TXdbg");
  remove_proc_entry ("TN_RXdbg", NULL);
  printk (KERN_INFO "/proc/%s removed\n", "TN_RXdbg");
//  remove_proc_entry ("trustnode2", NULL);
//  printk (KERN_INFO "/proc/%s removed\n", "trustnode2");

//remove_proc_entry("trustnode", NULL);


}

//*****************************************************************************************************************
/**
*  Init connection to openvswitch
*  
*/
void
INR_CTL_init_ovs ()
{
  //kthread_run (&INR_CTL_ovs_d, NULL, "INR_ovs_d");



}

//*****************************************************************************************************************
/**
*  ovs link deamon
*  
*/
void
INR_CTL_ovs_d (void *nix)
{
  /*allow_signal (SIGKILL);
     while (1) {
     if (signal_pending (current))
     break;
     //down(&INR_TN_ovscon_sema);
     if (INR_TN_ovscon_comand) {
     switch (INR_TN_ovscon_comand) {
     case INR_TN_ovsctl_cmd_ADD:
     INR_CTL_ovs_flow_add ();
     break;
     case INR_TN_ovsctl_cmd_RM:
     INR_CTL_ovs_flow_rm ();
     break;
     case INR_TN_ovsctl_cmd_FLUSH:
     INR_CTL_ovs_flow_flush ();
     break;
     default:
     break;
     }
     INR_TN_ovscon_comand = 0;
     }
     //up(&INR_TN_ovscon_sema);
     msleep (100);
     } */

}

//*****************************************************************************************************************
/**
*  new flow in ovs table
*  
*/
void
INR_CTL_ovs_flow_add ()
{
  INR_LOG_debug ("add new flow\n");
}

//*****************************************************************************************************************
/**
*  remove flow in ovs table
*  
*/
void
INR_CTL_ovs_flow_rm ()
{
  INR_LOG_debug ("rm flow\n");
}

//*****************************************************************************************************************
/**
*  flush ovs table
*  
*/
void
INR_CTL_ovs_flow_flush ()
{
  INR_LOG_debug ("flosh table\n");
}
