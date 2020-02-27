/**
*@mainpage  TrustNode Ethernetdriver
*@author M.Ulbricht, G.Eschemann
*
*This is the documentation for the TrustNode Ethernet driver. The code is mainly structured in PCI and NetWork communication. You should start browsing the <a href="files.html">filelist</a> first.<br>Questions and bugs please report to <a href="mailto:ulbricht@innoroute.de">ulbricht@innoroute.de</a>
**/
/**
*@file
*@brief main driver Function
*M.Ulbricht 2015
**/

#include <linux/kernel.h>
#include <linux/export.h>
#include <linux/module.h>
#include <linux/version.h>
#include <linux/pci.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/delay.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include "INR.h"
#include "INR-PCI.h"
#include "INR-NW.h"
#include "INR-ctl.h"
#include "INR-MMI.h"
#include "INR-TIME.h"



EXPORT_SYMBOL(INR_TIME_TX_transmit_interrupt);
EXPORT_SYMBOL(INR_NW_carrier_update);

static struct uio_info *can_userspace_interrupt;
volatile uint8_t probed=0;
static void remove (struct pci_dev *dev);

/**
*@brief PCI-Device definition
*
*/
static struct pci_device_id ids[] = {
    {PCI_DEVICE (0xAAAA, 0x7024),},
    {PCI_DEVICE (0x10EE, 0x0000),},
    {PCI_DEVICE (0xAAAA, 0x7121),},
    {PCI_DEVICE (0xAAAA, 0x7021),},
    {0,}
};

struct net_device *INR_NW;

/**
*@brief init nw-device
*
*/
int
INR_NWDEV_init (void)
{
    uint8_t i = 0;
    for (i = 0; i < INR_NW_devcount; i++) {
        INR_NW = alloc_netdev_mq (sizeof (struct INR_NW_priv), "TN%d", NET_NAME_UNKNOWN, INR_NW_init,INR_NW_queue_count);
        if (INR_NW == NULL) {
            INR_LOG_debug (loglevel_err"Cant alloc NWDEV %i !\n", i);
            return 1;
        } else {

            if (0 == register_netdev (INR_NW))
                INR_LOG_debug (loglevel_info"NWDev %i registerd, flags:%llx ", i, INR_NW->hw_features);
            INR_LOG_debug (loglevel_info"flags2:%llx\n", INR_NW->hw_features);
            set_nwdev (i, INR_NW);
        }
      INR_NW_carrier_update (i,0);//set carrier off (will be disabled via MDIO module later)  
    }
    return 0;
}

//*****************************************************************************************************************
/**
*destroy nw-device
*/
void
INR_NWDEV_destroi (void)
{
    uint8_t i = 0;
    for (i = 0; i < INR_NW_devcount; i++) {
        INR_LOG_debug (loglevel_info"destroy INRNWDEV %i\n",i);
        unregister_netdev (get_nwdev(i));
        free_netdev (get_nwdev(i));
    }
}

//*****************************************************************************************************************
/**
*init driver function
*/
static int
probe (struct pci_dev *dev, const struct pci_device_id *id)
{
    printk (loglevel_info "moep!\n"); //this is one of the verry important things needed to run the driver :D
    if (probed) {
        INR_LOG_debug (loglevel_err"driver already loaded... exit\n");
        return -1;
    }
    probed++;   //prevent driver from loaded twice
    INR_NW_zerovars();  //reset all global vars
    INR_PCI_zerovars();
    INR_zerovars();
    int result;
    INR_LOG_timelog_init ();	//safe timestamp
    INR_LOG_debug (loglevel_info"Start load module\n");
    if ((result = pci_enable_device (dev)) < 0) {
        INR_LOG_debug (loglevel_err"device enable fail...\n");
        return result;
    } else {
        INR_LOG_debug (loglevel_info"device enabled\n");
        INR_STATUS_set (INR_STATUS_DEVICE);
    }
    if(get_RING0_dummy_loop()==0)if (0 == INR_NWDEV_init ()) {
        INR_STATUS_set (INR_STATUS_NW_enabled);
    }
    //mdelay(1000);
    if(get_RING0_dummy_loop()==0){INR_init_drv (dev);		//INIT pci and network
    }else{
    	INR_init_drv_dummy (dev);
    }
    INR_CTL_init_proc (dev);	//init proc fs
    if (get_HW_user_feature(HW_feature_RTC))INR_TIME_init_ptp_clock(dev);//init ptp
    INR_STATUS_set (INR_STATUS_DRV_INIT_done);
    //INR_NW_set_PTP_prio(INIT_ptp_prio);
    INR_MMI_set_interrupt_allowed(1);
    return 0;
}

//*****************************************************************************************************************
static struct pci_driver pci_driver = {
    .name = "INRTrustnode",
    .id_table = ids,
    .probe = probe,
    .remove = remove,
};

//*****************************************************************************************************************
/**
*driver-end function, called by kernel
*@param *dev PCI-device
*/
static void
remove (struct pci_dev *dev)
{
    INR_MMI_set_interrupt_allowed(0);
    INR_NWDEV_destroi();
    INR_CTL_remove_proc (dev);
    INR_LOG_debug (loglevel_info"remove Module\n");
    INR_LOG_debug (loglevel_info"Reset Logic\n");
    INR_remove_drv (dev);
    INR_TIME_remove_ptp_clock();
    if(probed) {
        probed--;
    }
}

//*****************************************************************************************************************
static int __init
pci_skel_init (void)
{
    return pci_register_driver (&pci_driver);
}

//*****************************************************************************************************************
static void __exit
pci_skel_exit (void)
{
    pci_unregister_driver (&pci_driver);
}

//*****************************************************************************************************************
module_init (pci_skel_init);
module_exit (pci_skel_exit);
MODULE_LICENSE ("Dual BSD/GPL");
MODULE_AUTHOR ("M.Ulbricht");
MODULE_VERSION ("1.0");
MODULE_DESCRIPTION ("InnoRoute Trustnode driver");
MODULE_DEVICE_TABLE (pci, ids);
