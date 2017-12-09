/**
*@mainpage  TrustNode MMI functions
*@author M.Ulbricht
*
*This is the documentation for the TrustNode Ethernet driver. The code is mainly structured in PCI and NetWork communication. You should start browsing the <a href="files.html">filelist</a> first.<br>Questions and bugs please report to <a href="mailto:ulbricht@innoroute.de">ulbricht@innoroute.de</a>
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
#include "main.h"




//*****************************************************************************************************************
static int __init
skel_init (void)
{
    
}

//*****************************************************************************************************************
static void __exit
skel_exit (void)
{
    
}

//*****************************************************************************************************************
module_init (skel_init);
module_exit (skel_exit);
MODULE_LICENSE ("Dual BSD/GPL");
MODULE_AUTHOR ("M.Ulbricht");
MODULE_VERSION ("1.0");
MODULE_DESCRIPTION ("InnoRoute Trustnode MMI module");

