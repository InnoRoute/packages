/*#include<linux/init.h>
#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/ioport.h>
#include<linux/types.h>

struct resource *p;
#define addr 0x02f8
static int start(void)
{
 //   printk("module registered\n");
    p=request_region(addr, 1, "my_port");
    if(p==NULL)
    {
  //          printk(KERN_ALERT "port allocation failed\n");
            return 0;
    }
   // outb(12, addr);
    return 0;
}

static void stop(void)    
{
//    printk("module unregistered\n");
    uint8_t a;
    a=inb(addr);
    a=inb(addr+1);
    a=inb(addr+2);
    a=inb(addr+3);
    outw(0x666, addr);
//    printk("%d\n", a);
//    release_region(addr, 1);
}

module_init(start);
module_exit(stop);
MODULE_LICENSE("GPL");*/

//ad9558 i2c pll driver for Intel E3800
//Marian Ulbricht Innoroute GmbH 2016
//ulbricht@innoroute.de

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/ioport.h>
#include <linux/types.h>
#include <linux/export.h>
#include <linux/proc_fs.h>
#include <linux/pci.h>
//#include <linux/smp_lock.h>
#include <linux/uaccess.h>
#include <linux/delay.h>

#define PROCFS_MAX_SIZE		1024

static char procfs_buffer[PROCFS_MAX_SIZE];
static unsigned long procfs_buffer_size = 0;
static struct proc_dir_entry *reg1, *reg2,*reg3;// *INR_proc_dir;
uint64_t addr=0x2f8;
struct resource *p;

//-------------------------------------------data
int
data_proc_write (struct file *file, const char *buffer, unsigned long count, void *data)
{
  /* get buffer size */
  procfs_buffer_size = count;
  if (procfs_buffer_size > PROCFS_MAX_SIZE) {
    procfs_buffer_size = PROCFS_MAX_SIZE;
  }

 if (copy_from_user (procfs_buffer, buffer, procfs_buffer_size)) {
    return -EFAULT;
  }
	uint16_t i=0;
	for(i=0;i<procfs_buffer_size;i++)outb(procfs_buffer[i], addr+1);
  return procfs_buffer_size;
}

static int
data_proc_show (struct seq_file *m, void *v)
{
  seq_printf (m, "%i\n", "moep");
  return 0;
}

static int
data_proc_open (struct inode *inode, struct file *file)
{
  return single_open (file, data_proc_show, NULL);
}
//-------------------------------------------ctl
int
ctl_proc_write (struct file *file, const char *buffer, unsigned long count, void *data)
{
  /* get buffer size */
  procfs_buffer_size = count;
  if (procfs_buffer_size > PROCFS_MAX_SIZE) {
    procfs_buffer_size = PROCFS_MAX_SIZE;
  }

 if (copy_from_user (procfs_buffer, buffer, procfs_buffer_size)) {
    return -EFAULT;
  }
	uint8_t tmp=0;
	sscanf (procfs_buffer, "%d", &tmp);
  outb(tmp, addr);
  return procfs_buffer_size;
}

static int
ctl_proc_show (struct seq_file *m, void *v)
{
  seq_printf (m, "%i\n", "moep");
  return 0;
}

static int
ctl_proc_open (struct inode *inode, struct file *file)
{
  return single_open (file, ctl_proc_show, NULL);
}
//-------------------------------------------addr
int
addr_proc_write (struct file *file, const char *buffer, unsigned long count, void *data)
{
  /* get buffer size */
  procfs_buffer_size = count;
  if (procfs_buffer_size > PROCFS_MAX_SIZE) {
    procfs_buffer_size = PROCFS_MAX_SIZE;
  }

  if (copy_from_user (procfs_buffer, buffer, procfs_buffer_size)) {
    return -EFAULT;
  }
//  release_region(addr, 1);
  sscanf (procfs_buffer, "%d", &addr);
/*	p=request_region(addr, 1, "my_port");
  if(p==NULL)
    {
            printk(KERN_ALERT "port allocation failed\n");
            return 0;
    }*/
	printk("set baseaddress to:%x",addr);

  return procfs_buffer_size;
}

static int
addr_proc_show (struct seq_file *m, void *v)
{
  seq_printf (m, "%i\n", "moep");
  return 0;
}

static int
addr_proc_open (struct inode *inode, struct file *file)
{
  return single_open (file, addr_proc_show, NULL);
}


static int start(void)
{
printk("loading LPCtool...\n");
  static const struct file_operations data_proc = {
    .owner = THIS_MODULE,
    .open = data_proc_open,
    .write = data_proc_write,
    .read = seq_read,
    .llseek = seq_lseek,
    .release = single_release,
  };
  static const struct file_operations ctl_proc = {
    .owner = THIS_MODULE,
    .open = ctl_proc_open,
    .write = ctl_proc_write,
    .read = seq_read,
    .llseek = seq_lseek,
    .release = single_release,
  };
  static const struct file_operations addr_proc = {
    .owner = THIS_MODULE,
    .open = addr_proc_open,
    .write = addr_proc_write,
    .read = seq_read,
    .llseek = seq_lseek,
    .release = single_release,
  };

p=request_region(addr, 1, "my_port");



  reg1 = proc_create ("TN_LPC_data", 0644, NULL, &data_proc);

  if (reg1 == NULL) {
    remove_proc_entry ("TN_LPC_data", NULL);
    printk (KERN_ALERT "Error: Could not initialize /proc/%s\n", "TN_LPC_data");
    return -ENOMEM;
  }
reg2 = proc_create ("TN_LPC_ctl", 0644, NULL, &ctl_proc);

  if (reg2 == NULL) {
    remove_proc_entry ("TN_LPC_ctl", NULL);
    printk (KERN_ALERT "Error: Could not initialize /proc/%s\n", "TN_LPC_ctl");
    return -ENOMEM;
  }
reg3 = proc_create ("TN_LPC_addr", 0644, NULL, &addr_proc);

  if (reg3 == NULL) {
    remove_proc_entry ("TN_LPC_addr", NULL);
    printk (KERN_ALERT "Error: Could not initialize /proc/%s\n", "TN_LPC_addr");
    return -ENOMEM;
  }
 

if(p==NULL)
    {
            printk(KERN_ALERT "port allocation failed\n");
            return 0;
    }
 

    return 0;
  }

  static void stop(void)    
  {
//release_region(addr, 1);
    remove_proc_entry ("TN_LPC_addr", NULL);
    printk (KERN_INFO "/proc/%s removed\n", "TN_LPC_addr");
		remove_proc_entry ("TN_LPC_ctl", NULL);
    printk (KERN_INFO "/proc/%s removed\n", "TN_LPC_ctl");
 		remove_proc_entry ("TN_LPC_data", NULL);
    printk (KERN_INFO "/proc/%s removed\n", "TN_LPC_data");
    

  }

  module_init(start);
  module_exit(stop);
  MODULE_LICENSE("GPL");
