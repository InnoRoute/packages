#include <linux/kernel.h>
#include <linux/version.h>
#include <linux/module.h>
#include "INR-MMI.h"
#include "TN_MMI.h"
#include <asm/signal.h>
#include <linux/kthread.h>
#include <linux/interrupt.h>
#include <linux/delay.h>
#include <asm/signal.h>
#include <linux/semaphore.h>
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,14,50)
	#include <linux/sched/signal.h>
#endif
DEFINE_SEMAPHORE (MMI_int_sem);
static DECLARE_WAIT_QUEUE_HEAD (INR_MMI_int_watchd_waittingqueu);		/**<waiting lock for mmi int watchd*/
volatile uint8_t interrupt_allowed=0;
void (*MMI_handler)(void);
void (*MMI_init)(uint64_t bar1);

//*****************************************************************************************************************
/**
*allow global interrupts
*
*/
void INR_MMI_set_interrupt_allowed(uint8_t val) {
interrupt_allowed=val;
if(val)INR_MMI_interrupt();//trigger interrupt if activated
}
//*****************************************************************************************************************
/**
*init mmid address
*
*/
void INR_MMI_interrupt_start(uint64_t bar1) {

    MMI_handler= symbol_get(INR_MMI_interrupt_handler);
    MMI_init= symbol_get(INR_MMI_init);
    if(MMI_init)MMI_init(bar1);
    kthread_run (&INR_MMI_interrupt_watchd, NULL, "INR_MMI_int_watchd");
    wake_up_interruptible (&INR_MMI_int_watchd_waittingqueu);	//wakeup mmi int watchd
}

//*****************************************************************************************************************
/**
*ISR for MMI interrupt
*
*/
void INR_MMI_interrupt() {
if(interrupt_allowed){
    if (MMI_handler) {
    		while(down_trylock(&MMI_int_sem));//block until semaphore get
        MMI_handler();
    		up(&MMI_int_sem);
    }}
}

void INR_MMI_interrupt_stop() {
    symbol_put(INR_MMI_interrupt_handler);
    symbol_put(INR_MMI_init);
}

//*****************************************************************************************************************
/**
*tread function which automatically poll MMI interrupt
*/
void
INR_MMI_interrupt_watchd_wakeup()// contains empirical thesholds..but it works ;)
{
	wake_up_interruptible (&INR_MMI_int_watchd_waittingqueu);	//wakeup mmi int watchd

}
//*****************************************************************************************************************
/**
*tread function which automatically poll MMI interrupt
*/
int
INR_MMI_interrupt_watchd (void *nix)// contains empirical thesholds..but it works ;)
{
    DECLARE_WAITQUEUE (wait3, current);
    allow_signal (SIGKILL);
    add_wait_queue (&INR_MMI_int_watchd_waittingqueu, &wait3);
    while (1) {
        set_current_state (TASK_INTERRUPTIBLE);
        schedule ();
        if (signal_pending (current))
            break;			//exit on thermination
        INR_MMI_interrupt();// trigger isr
        
        
            }
    set_current_state (TASK_RUNNING);
    remove_wait_queue (&INR_MMI_int_watchd_waittingqueu, &wait3);
    return 0;
}
