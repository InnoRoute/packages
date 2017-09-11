#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

//*****************************************************************************************************************
/**
*init driver function
*/
static int
probe (struct pci_dev *dev, const struct pci_device_id *id)
{
	printk("TNflowtable moep!\n");
	return 0;
}

//*****************************************************************************************************************
/**
*driver-end function, called by kernel
*@param *dev PCI-device
*/
static void
remove (struct pci_dev *dev) {}

//*****************************************************************************************************************
static int __init
init (void)
{
	printk("TNflowtable moep!\n");
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
INR_FC_net_flow(void)
{
	printk ("__FUNCTION__ = %s\n", __FUNCTION__);
}

//*****************************************************************************************************************
EXPORT_SYMBOL(INR_FC_net_flow);
module_init (init);
module_exit (exit);
MODULE_LICENSE ("Dual BSD/GPL");
MODULE_AUTHOR ("M.Ulbricht");
MODULE_VERSION ("0.1");
MODULE_DESCRIPTION ("InnoRoute Trustnode FlowTable driver");