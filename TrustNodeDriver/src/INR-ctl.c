/**
*@file INR-ctl.c
*@brief Functions for Proc-filesystem
*@author M.Ulbricht 2015
**/
#include <linux/kernel.h>
#include <linux/export.h>

#include <linux/version.h>
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
#include "INR-TIME.h"

#define PROCFS_MAX_SIZE		1024
static char procfs_buffer[PROCFS_MAX_SIZE];
static size_t procfs_buffer_size = 0;
static struct proc_dir_entry *reg1, *reg2,*reg3,*reg4, *INR_proc_dir, *INR_proc_dir2,*reg5,*reg6,*reg7,*reg8,*reg9;

//*****************************************************************************************************************
/**
*  proc write function
*
*/
int
set_TSN_USE_ctrl_bridge_clock_offset_write (struct file *file, const char *buffer, size_t count, void *data)
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
    INR_LOG_debug (loglevel_info"write %d to set_TSN_queue\n", tmp);
    INR_TIME_set_USE_ctrl_bridge_clock_offset(tmp);
    return procfs_buffer_size;
}

//*****************************************************************************************************************
/**
*  proc print function
*
*/
static int
set_TSN_USE_ctrl_bridge_clock_offset_proc_show (struct seq_file *m, void *v)
{
    seq_printf (m, "not implemented\n");
    return 0;
}

//*****************************************************************************************************************
/**
*  proc open function
*
*/
static int
set_TSN_USE_ctrl_bridge_clock_offset_proc_open (struct inode *inode, struct file *file)
{
    return single_open (file, set_TSN_USE_ctrl_bridge_clock_offset_proc_show, NULL);
}
//*****************************************************************************************************************
/**
*  proc write function
*
*/
int
set_TSN_debug_write (struct file *file, const char *buffer, size_t count, void *data)
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
    INR_LOG_debug (loglevel_info"write %d to set_TSN_queue\n", tmp);
    INR_TIME_set_debug(tmp);
    return procfs_buffer_size;
}

//*****************************************************************************************************************
/**
*  proc print function
*
*/
static int
set_TSN_debug_proc_show (struct seq_file *m, void *v)
{
    seq_printf (m, "not implemented\n");
    return 0;
}

//*****************************************************************************************************************
/**
*  proc open function
*
*/
static int
set_TSN_debug_proc_open (struct inode *inode, struct file *file)
{
    return single_open (file, set_TSN_debug_proc_show, NULL);
}

//*****************************************************************************************************************
/**
*  proc write function
*
*/
int
set_TSN_queue_write (struct file *file, const char *buffer, size_t count, void *data)
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
    INR_LOG_debug (loglevel_info"write %d to set_TSN_queue\n", tmp);
    set_TSN_queue(tmp);
    return procfs_buffer_size;
}

//*****************************************************************************************************************
/**
*  proc print function
*
*/
static int
set_TSN_queue_proc_show (struct seq_file *m, void *v)
{
    seq_printf (m, "not implemented\n");
    return 0;
}

//*****************************************************************************************************************
/**
*  proc open function
*
*/
static int
set_TSN_queue_proc_open (struct inode *inode, struct file *file)
{
    return single_open (file, set_TSN_queue_proc_show, NULL);
}
//*****************************************************************************************************************
/**
*  proc write function
*
*/
int
set_TSN_ts_write (struct file *file, const char *buffer, size_t count, void *data)
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
    INR_LOG_debug (loglevel_info"write %d to set_TSN_ts\n", tmp);
    set_TSN_ts(tmp);
    return procfs_buffer_size;
}

//*****************************************************************************************************************
/**
*  proc print function
*
*/
static int
set_TSN_ts_proc_show (struct seq_file *m, void *v)
{
    seq_printf (m, "not implemented\n");
    return 0;
}

//*****************************************************************************************************************
/**
*  proc open function
*
*/
static int
set_TSN_ts_proc_open (struct inode *inode, struct file *file)
{
    return single_open (file, set_TSN_ts_proc_show, NULL);
}
//*****************************************************************************************************************
/**
*  proc write function
*
*/
int
set_TSN_sock_opt_write (struct file *file, const char *buffer, size_t count, void *data)
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
    INR_LOG_debug (loglevel_info"write %d to set_TSN_sock_opt\n", tmp);
    set_TSN_sock_opt(tmp);
    return procfs_buffer_size;
}

//*****************************************************************************************************************
/**
*  proc print function
*
*/
static int
set_TSN_sock_opt_proc_show (struct seq_file *m, void *v)
{
    seq_printf (m, "not implemented\n");
    return 0;
}

//*****************************************************************************************************************
/**
*  proc open function
*
*/
static int
set_TSN_sock_opt_proc_open (struct inode *inode, struct file *file)
{
    return single_open (file, set_TSN_sock_opt_proc_show, NULL);
}

//*****************************************************************************************************************
/**
*  proc write function
*
*/
int
TNrussian_write (struct file *file, const char *buffer, size_t count, void *data)
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
    INR_LOG_debug (loglevel_info"write %d to russian\n", tmp);
    set_russian(tmp);
    return procfs_buffer_size;
}

//*****************************************************************************************************************
/**
*  proc print function
*
*/
static int
TNrussian_proc_show (struct seq_file *m, void *v)
{
    seq_printf (m, "%i\n", get_russian());
    return 0;
}

//*****************************************************************************************************************
/**
*  proc open function
*
*/
static int
TNrussian_proc_open (struct inode *inode, struct file *file)
{
    return single_open (file, TNrussian_proc_show, NULL);
}
//*****************************************************************************************************************
/**
*  proc write function
*
*/
int
TNsend2cpu_write (struct file *file, const char *buffer, size_t count, void *data)
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
TXdbg_write (struct file *file, const char *buffer, size_t count, void *data)
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
RXdbg_write (struct file *file, const char *buffer, size_t count, void *data)
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
    static const struct file_operations TNrussian = {
        .owner = THIS_MODULE,
        .open = TNrussian_proc_open,
        .write = TNrussian_write,
        .read = seq_read,
        .llseek = seq_lseek,
        .release = single_release,
    };
    static const struct file_operations set_TSN_sock_opt = {
        .owner = THIS_MODULE,
        .open = set_TSN_sock_opt_proc_open,
        .write = set_TSN_sock_opt_write,
        .read = seq_read,
        .llseek = seq_lseek,
        .release = single_release,
    };
    static const struct file_operations set_TSN_ts = {
        .owner = THIS_MODULE,
        .open = set_TSN_ts_proc_open,
        .write = set_TSN_ts_write,
        .read = seq_read,
        .llseek = seq_lseek,
        .release = single_release,
    };
    static const struct file_operations set_TSN_queue = {
        .owner = THIS_MODULE,
        .open = set_TSN_queue_proc_open,
        .write = set_TSN_queue_write,
        .read = seq_read,
        .llseek = seq_lseek,
        .release = single_release,
    };
    static const struct file_operations set_TSN_debug = {
        .owner = THIS_MODULE,
        .open = set_TSN_debug_proc_open,
        .write = set_TSN_debug_write,
        .read = seq_read,
        .llseek = seq_lseek,
        .release = single_release,
    };
    static const struct file_operations set_TSN_USE_ctrl_bridge_clock_offset = {
        .owner = THIS_MODULE,
        .open = set_TSN_USE_ctrl_bridge_clock_offset_proc_open,
        .write = set_TSN_USE_ctrl_bridge_clock_offset_write,
        .read = seq_read,
        .llseek = seq_lseek,
        .release = single_release,
    };
    INR_proc_dir = proc_mkdir("TrustNode",NULL);
    if(!INR_proc_dir)
    {
        printk(KERN_ALERT "Error creating proc entry");
        return -ENOMEM;
    }
    INR_proc_dir2 = proc_mkdir("TSN",INR_proc_dir);
    if(!INR_proc_dir2)
    {
        printk(KERN_ALERT "Error creating proc entry");
        return -ENOMEM;
    }
    reg1 = proc_create ("TN_send2cpu", 0644, INR_proc_dir, &TNsend2cpu);
    if (reg1 == NULL) {
        remove_proc_entry ("TNsend2cpu", INR_proc_dir);
        printk (KERN_ALERT "Error: Could not initialize /proc/%s\n", "TN_send2cpu");
        return -ENOMEM;
    }
    printk (KERN_INFO "/proc/%s created\n", "TNsend2cpu");

    reg2 = proc_create ("TN_RXdbg", 0644, INR_proc_dir, &RXdbg);
    if (reg2 == NULL) {
        remove_proc_entry ("TN_RXdbg", INR_proc_dir);
        printk (KERN_ALERT "Error: Could not initialize /proc/%s\n", "TN_RXdbg");
        return -ENOMEM;
    }
    printk (KERN_INFO "/proc/%s created\n", "TN_RXdbg");

    reg3 = proc_create ("TN_TXdbg", 0644, INR_proc_dir, &TXdbg);
    if (reg3 == NULL) {
        remove_proc_entry ("TN_TXdbg", INR_proc_dir);
        printk (KERN_ALERT "Error: Could not initialize /proc/%s\n", "TN_TXdbg");
        return -ENOMEM;
    }
    reg4 = proc_create ("TN_russian", 0644, INR_proc_dir, &TNrussian);
    if (reg4 == NULL) {
        remove_proc_entry ("TN_russian", INR_proc_dir);
        printk (KERN_ALERT "Error: Could not initialize /proc/%s\n", "TN_russian");
        return -ENOMEM;
    }
    reg5 = proc_create ("set_TSN_sock_opt", 0644, INR_proc_dir2, &set_TSN_sock_opt);
    if (reg5 == NULL) {
        remove_proc_entry ("set_TSN_sock_opt", INR_proc_dir2);
        printk (KERN_ALERT "Error: Could not initialize /proc/TrustNode/TSN/%s\n", "set_TSN_sock_opt");
        return -ENOMEM;
    }
    reg6 = proc_create ("set_TSN_ts", 0644, INR_proc_dir2, &set_TSN_ts);
    if (reg6 == NULL) {
        remove_proc_entry ("set_TSN_ts", INR_proc_dir2);
        printk (KERN_ALERT "Error: Could not initialize /proc/TrustNode/TSN/%s\n", "set_TSN_ts");
        return -ENOMEM;
    }
    reg7 = proc_create ("set_TSN_queue", 0644, INR_proc_dir2, &set_TSN_queue);
    if (reg7 == NULL) {
        remove_proc_entry ("set_TSN_queue", INR_proc_dir2);
        printk (KERN_ALERT "Error: Could not initialize /proc/TrustNode/TSN/%s\n", "set_TSN_queue");
        return -ENOMEM;
    }
    reg8 = proc_create ("set_TSN_debug", 0644, INR_proc_dir2, &set_TSN_debug);
    if (reg8 == NULL) {
        remove_proc_entry ("set_TSN_debug", INR_proc_dir2);
        printk (KERN_ALERT "Error: Could not initialize /proc/TrustNode/TSN/%s\n", "set_TSN_debug");
        return -ENOMEM;
    }
    reg9 = proc_create ("set_TSN_USE_ctrl_bridge_clock_offset", 0644, INR_proc_dir2, &set_TSN_USE_ctrl_bridge_clock_offset);
    if (reg9 == NULL) {
        remove_proc_entry ("set_TSN_USE_ctrl_bridge_clock_offset", INR_proc_dir2);
        printk (KERN_ALERT "Error: Could not initialize /proc/TrustNode/TSN/%s\n", "set_TSN_USE_ctrl_bridge_clock_offset");
        return -ENOMEM;
    }
    printk (KERN_INFO "/proc/%s created\n", "TN_russian");

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
    remove_proc_entry ("TN_send2cpu", INR_proc_dir);
    printk (KERN_INFO "/proc/TrustNode/%s removed\n", "TN_send2cpu");
    remove_proc_entry ("TN_TXdbg", INR_proc_dir);
    printk (KERN_INFO "/proc/TrustNode/%s removed\n", "TN_TXdbg");
    remove_proc_entry ("TN_RXdbg", INR_proc_dir);
    printk (KERN_INFO "/proc/TrustNode/%s removed\n", "TN_RXdbg");
    remove_proc_entry ("TN_russian", INR_proc_dir);
    printk (KERN_INFO "/proc/TrustNode/%s removed\n", "TN_russian");
    remove_proc_entry ("set_TSN_sock_opt", INR_proc_dir2);
    printk (KERN_INFO "/proc/TrustNode/TSN/%s removed\n", "set_TSN_sock_opt");
    remove_proc_entry ("set_TSN_ts", INR_proc_dir2);
    printk (KERN_INFO "/proc/TrustNode/TSN/%s removed\n", "set_TSN_ts");
    remove_proc_entry ("set_TSN_queue", INR_proc_dir2);
    printk (KERN_INFO "/proc/TrustNode/TSN/%s removed\n", "set_TSN_queue");
    remove_proc_entry ("set_TSN_debug", INR_proc_dir2);
    printk (KERN_INFO "/proc/TrustNode/TSN/%s removed\n", "set_TSN_debug");
    remove_proc_entry ("set_TSN_USE_ctrl_bridge_clock_offset", INR_proc_dir2);
    printk (KERN_INFO "/proc/TrustNode/TSN/%s removed\n", "set_TSN_USE_ctrl_bridge_clock_offset");
    remove_proc_entry ("TSN", INR_proc_dir);
    printk (KERN_INFO "/proc/TrustNode/%s removed\n", "TSN");
    remove_proc_entry ("TrustNode", NULL);
    printk (KERN_INFO "/proc/%s removed\n", "TrustNode");

}
