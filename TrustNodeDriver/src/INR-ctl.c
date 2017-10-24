/**
*@file INR-ctl.c
*@brief Functions for Proc-filesystem
*@author M.Ulbricht 2015
**/
#include <linux/kernel.h>
#include <linux/export.h>
#include <linux/module.h>
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
static unsigned long procfs_buffer_size = 0;
static struct proc_dir_entry *reg1, *reg2,*reg3, *INR_proc_dir;

//*****************************************************************************************************************
/**
*  proc write function
*
*/
int
TNsend2cpu_write (struct file *file, const char *buffer, unsigned long count, void *data)
{
    procfs_buffer_size = count;
    if (procfs_buffer_size > PROCFS_MAX_SIZE) {
        procfs_buffer_size = PROCFS_MAX_SIZE;
    }
    if (copy_from_user (procfs_buffer, buffer, procfs_buffer_size)) {
        return -EFAULT;
    }
    uint32_t tmp = 0;
    sscanf (procfs_buffer, "%d", &tmp);
    INR_LOG_debug (loglevel_info"write %d to send2cpu\n", tmp);
    set_send2cpu(tmp);
    return procfs_buffer_size;
}

//*****************************************************************************************************************
/**
*  proc print function
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
*  proc open function
*
*/
static int
TNsend2cpu_proc_open (struct inode *inode, struct file *file)
{
    return single_open (file, TNsend2cpu_proc_show, NULL);
}

//*****************************************************************************************************************
/**
*  proc write function
*
*/
int
TXdbg_write (struct file *file, const char *buffer, unsigned long count, void *data)
{
    procfs_buffer_size = count;
    if (procfs_buffer_size > PROCFS_MAX_SIZE) {
        procfs_buffer_size = PROCFS_MAX_SIZE;
    }
    if (copy_from_user (procfs_buffer, buffer, procfs_buffer_size)) {
        return -EFAULT;
    }
    uint32_t tmp = 0;
    sscanf (procfs_buffer, "%d", &tmp);
    INR_LOG_debug (loglevel_info"write %d to TXdbg\n", tmp);
    set_tx_dbg(tmp);
    return procfs_buffer_size;
}

//*****************************************************************************************************************
/**
*  proc print function
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
*  proc open function
*
*/
static int
TXdbg_proc_open (struct inode *inode, struct file *file)
{
    return single_open (file, TXdbg_proc_show, NULL);
}
//*****************************************************************************************************************
/**
*  proc write function
*
*/
int
RXdbg_write (struct file *file, const char *buffer, unsigned long count, void *data)
{
    procfs_buffer_size = count;
    if (procfs_buffer_size > PROCFS_MAX_SIZE) {
        procfs_buffer_size = PROCFS_MAX_SIZE;
    }
    if (copy_from_user (procfs_buffer, buffer, procfs_buffer_size)) {
        return -EFAULT;
    }
    uint32_t tmp = 0;
    sscanf (procfs_buffer, "%d", &tmp);
    INR_LOG_debug (loglevel_info"write %d to RXdbg\n", tmp);
    set_rx_dbg(tmp);
    return procfs_buffer_size;
}

//*****************************************************************************************************************
/**
*  proc print function
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
*  proc open function
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

    return 0;
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
}
