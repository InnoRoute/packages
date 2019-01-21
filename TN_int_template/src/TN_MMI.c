#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/kthread.h>
#include "TN_MMI.h"

void INR_MMI_interrupt_handler(){
	allow_signal (SIGKILL);
	printk("MMI_interrupt handler_test\n");
}
