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
#include <linux/module.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/delay.h>
#include "tn_env.h"
#include "TN_MMI.h"

EXPORT_SYMBOL(INR_MMI_interrupt_handler);
EXPORT_SYMBOL(INR_MMI_init);

//*****************************************************************************************************************
static int __init
skel_init (void)
{

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

