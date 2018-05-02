#include <linux/kernel.h>
#include <linux/version.h>
#include <linux/module.h>
#include "INR-MMI.h"
#include <TN_MMI.h>
void (*MMI_handler)(void);
void (*MMI_init)(uint64_t bar1);
//*****************************************************************************************************************
/**
*init mmid address
*
*/
void INR_MMI_interrupt_start(uint64_t bar1){

MMI_handler= symbol_get(INR_MMI_interrupt_handler);
MMI_init= symbol_get(INR_MMI_init);
if(MMI_init)MMI_init(bar1);
}
//*****************************************************************************************************************
/**
*ISR for MMI interrupt
*
*/
void INR_MMI_interrupt(){
	if (MMI_handler) {
		MMI_handler();
	}
}

void INR_MMI_interrupt_stop(){
symbol_put(INR_MMI_interrupt_handler);
symbol_put(INR_MMI_init);
}
