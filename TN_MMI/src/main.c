/**
*@mainpage  TrustNode MMI functions
*@author M.Ulbricht
*
**/
/**
*@file
*@brief main driver Function
*M.Ulbricht 2017
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
#include "tn_env.h"
#include "TN_MMI.h"
#include "TN_MDIO.h"
#define PROCFS_MAX_SIZE		1024
static char procfs_buffer[PROCFS_MAX_SIZE];
static size_t procfs_buffer_size = 0;
static struct proc_dir_entry *reg1, *INR_proc_dir;
EXPORT_SYMBOL(INR_MMI_interrupt_handler);
EXPORT_SYMBOL(INR_MMI_init);


//*****************************************************************************************************************
/**
*  proc write function
*
*/
int
INR_proc_PHYspeed_write (struct file *file, const char *buffer, size_t count, void *data)
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
    printk("Phy %i speed: 0x%x\n",tmp,INR_MDIO_GPHY_getspeed(tmp+16));
    return procfs_buffer_size;
}

//*****************************************************************************************************************
/**
*  proc print function
*
*/
static int
INR_proc_PHYspeed_show (struct seq_file *m, void *v)
{
    seq_printf (m, "%i\n", 0);
    INR_MMI_phy_state_watch_wakeup();
    return 0;
}

//*****************************************************************************************************************
/**
*  proc open function
*
*/
static int
INR_proc_PHYspeed_open (struct inode *inode, struct file *file)
{
    return single_open (file, INR_proc_PHYspeed_show, NULL);
}
//*****************************************************************************************************************
/**init MMI proc functions
*
*/
void INR_MMI_remove_procfs(void){
    remove_proc_entry ("TN_PHYspeed", INR_proc_dir);
    printk (KERN_INFO "/proc/TrustNode/%s removed\n", "TN_PHYspeed");
    remove_proc_entry ("TN_MMI", NULL);
    printk (KERN_INFO "/proc/%s removed\n", "TN_MMI");

}

//*****************************************************************************************************************
/**init MMI proc functions
*
*/
void INR_MMI_init_procfs(void){
 static const struct file_operations PHYspeed = {
        .owner = THIS_MODULE,
        .open = INR_proc_PHYspeed_open,
        .write = INR_proc_PHYspeed_write,
        .read = seq_read,
        .llseek = seq_lseek,
        .release = single_release,
    };
INR_proc_dir = proc_mkdir("TN_MMI",NULL);
    if(!INR_proc_dir)
    {
        printk(KERN_ALERT "Error creating proc entry");
        return -ENOMEM;
    }
reg1 = proc_create ("TN_PHYspeed", 0644, INR_proc_dir, &PHYspeed);
    if (reg1 == NULL) {
        remove_proc_entry ("TN_PHYspeed", INR_proc_dir);
        printk (KERN_ALERT "Error: Could not initialize /proc/%s\n", "TN_PHYspeed");
        return -ENOMEM;
    }
}
//*****************************************************************************************************************
static int __init
skel_init (void)
{
    INR_MMI_init_procfs();
    INR_MMI_phy_state_watch_start();
    return 0;
}

//*****************************************************************************************************************
static void __exit
skel_exit (void)
{
    INR_MMI_exit();
    return 0;
}


//*****************************************************************************************************************
module_init (skel_init);
module_exit (skel_exit);
MODULE_LICENSE ("Dual BSD/GPL");
MODULE_AUTHOR ("M.Ulbricht");
MODULE_VERSION ("1.0");
MODULE_DESCRIPTION ("InnoRoute Trustnode MMI module");

