/**
*@file TNflowtable_main.c
*@brief main file of TNflowtable kernel module (for OVS interaction)
*M.Ulbricht 2017
**/
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include "tnlibflowtable.h"
#include "flowtableactions.h"
export clear_arguments ();
export HT_EMH_add ();
export HT_EMH_del ();
export HT_EMH_clear ();
export HT_EMH_print ();
export RT_EMH_add ();
export RT_EMH_del ();
export RT_EMH_clear ();
export RT_EMH_print ();
export get_HASH ();
export HT_EMA_add ();
export HT_EMA_del ();
export HT_EMA_clear ();
export HT_EMA_print ();
export RT_EMA_add ();
export RT_EMA_del ();
export RT_EMA_clear ();
export RT_EMA_print ();
export CT_EMH_add ();
export CT_EMH_del ();
export CT_EMH_clear ();
export CT_EMH_print ();
export AT_add ();
export AT_del ();
export AT_clear ();
export AT_print ();
export RT_EMA_TYPE_override ();
//*****************************************************************************************************************
/**
*init driver function
*/
static int
probe (struct pci_dev *dev, const struct pci_device_id *id)
{
  printk ("TNflowtable moep!\n");
  return 0;
}

//*****************************************************************************************************************
/**
*driver-end function, called by kernel
*@param *dev PCI-device
*/
static void
remove (struct pci_dev *dev)
{
}

//*****************************************************************************************************************
static int __init
init (void)
{
  printk ("TNflowtable moep!\n");
  return 0;
}

//*****************************************************************************************************************
static void __exit
exit (void)
{
  return 0;
}

//*****************************************************************************************************************
static uint8_t
INR_FC_net_flow (void)
{
  printk ("__FUNCTION__ = %s\n", __FUNCTION__);
}

//*****************************************************************************************************************
EXPORT_SYMBOL (INR_FC_net_flow);
module_init (init);
module_exit (exit);
MODULE_LICENSE ("Dual BSD/GPL");
MODULE_AUTHOR ("M.Ulbricht");
MODULE_VERSION ("0.1");
MODULE_DESCRIPTION ("InnoRoute Trustnode FlowTable driver");
