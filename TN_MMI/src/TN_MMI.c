#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/export.h>
#include <linux/init.h>
#include "TN_MMI.h"
#include "TN_MDIO.h"
#include "tn_env.h"
void *gBaseVirt1_MMI = NULL;
//*****************************************************************************************************************
/**
*read from mmi addr
*
*/
uint32_t INR_PCI_MMI_read(uint64_t addr) {
    uint32_t val=INR_PCI_BAR1_read(addr);
    printk("read 0x%lx from 0x%lx, base:0x%lx\n",val,addr,gBaseVirt1_MMI);
    return val;
}

//*****************************************************************************************************************
/**
*read from mmi addr
*
*/
void INR_PCI_MMI_write(uint32_t value, uint64_t addr) {
    printk("write 0x%lx to 0x%lx, base:0x%lx\n",value,addr,gBaseVirt1_MMI);
    return INR_PCI_BAR1_write(value,addr);
}
//*****************************************************************************************************************
/**
*init mmid address
*
*/
void INR_MMI_init(uint64_t *bar1) {
    gBaseVirt1_MMI=bar1;//get baseaddr from
    printk("INR_MMI_init base:0x%lx\n",gBaseVirt1_MMI);
    //if(gBaseVirt1_MMI)INR_MDIO_init(gBaseVirt1_MMI);//init mdio

}
//*****************************************************************************************************************
/**
*ISR for MMI interrupt
*
*/
void INR_MMI_interrupt_handler() {
    if (!gBaseVirt1_MMI) {
        printk("error no connection to BAR1\n");
        return 0;//no address of bar 1, exit
    }
    INR_PCI_BAR1_write(0x0,0xc20010);
    INR_PCI_BAR1_write(0x0,0xc20014);
    INR_PCI_BAR1_write(0x0,0xc20018);
    INR_PCI_BAR1_write(0x0,0xc2001c);
    //uint32_t MDIO_int=INR_PCI_BAR1_read(INR_MDIO_interrupt);
    //printk("MMI_interrupt handler_test: 0x%lx\n",MDIO_int);

    printk("Flowcache EMA has entry 1 cleared\n");
	
    //if(MDIO_int&0xfff)INR_MMI_PHY_interrupt(MDIO_int&0xfff);
    //if(MDIO_int&(3<<16))INR_MMI_ALASKA_PHY_PTP_interrupt((MDIO_int>>16)&3);
    //if(MDIO_int&(1<<30))INR_MMI_PHY_hard_reset();
    //if(MDIO_int&(1<<31))INR_MMI_PHY_hard_reset();

}
//*****************************************************************************************************************
/**
*give mmi address back
*
*/
void INR_MMI_exit() {
    printk("%s\n", __func__);
}
//*****************************************************************************************************************
/**
*handle MMI-PHY interrupt
*@param id ID of PHY
*/
void INR_MMI_PHY_interrupt(uint16_t id) {
    uint8_t phyid=0,i=0;
    uint16_t read_val;
    if(id>=(1<<10)) { //int from Alaska phy
        if(id&(1<<10))phyid=0;
        if(id&(1<<11))phyid=1;
        read_val=INR_MDIO_read(phyid,INR_MDIO_ALASKA_PHY_REG_ISTAT);
        printk("GPHY interrupt from ALASKA PHY %i, cause:0x%x",phyid,read_val);
        switch(read_val) {
        case (1<<15):
            printk("copper autonegotioation error\n");
            break;
        case (1<<14):
            printk("copper speed change\n");
            break;
        case (1<<13):
            printk("copper duplex change\n");
            break;
        case (1<<12):
            printk("copper page received\n");
            break;
        case (1<<11):
            printk("copper autonegotiation complete\n");
            break;
        case (1<<10):
            printk("copper link status changed\n");
            break;
        case (1<<9):
            printk("copper symbol error\n");
            break;
        case (1<<8):
            printk("copper false carrier\n");
            break;
        case (1<<7):
            printk("WOL event happened\n");
            break;
        case (1<<6):
            printk("MDI crossover changed\n");
            break;
        case (1<<5):
            printk("downshift interrupt\n");
            break;
        case (1<<4):
            printk("copper energy detect changed\n");
            break;
        case (1<<3):
            printk("FLP exchange complete but no link\n");
            break;
        case (1<<2):
            printk("DTE power status changed interrupt\n");
            break;
        case (1<<1):
            printk("polarity changed\n");
            break;
        case (1<<0):
            printk("jabber\n");
            break;//link state change
        default:
            break;
        }

    } else { //int from GPHY
        for(i==0; i<10; i++)if(id&(1<<i))phyid=i+16; //determine MDIO phy ID
        read_val=INR_MDIO_read(phyid,INR_MDIO_GPHY_REG_ISTAT);
        printk("GPHY interrupt from PHY %i, cause:0x%x",phyid,read_val);
        switch(read_val) {
        case (1<<15):
            printk("wake on lan status\n");
            break;
        case (1<<14):
            printk("master/slave resulution error\n");
            break;
        case (1<<13):
            printk("next page received interrupt status\n");
            break;
        case (1<<12):
            printk("next page transmittwed interrupt status\n");
            break;
        case (1<<11):
            printk("auto-negotiation error interrupt status\n");
            break;
        case (1<<10):
            printk("auto-negotiation complete interrupt status\n");
            break;
        case (1<<9):
            printk("unknown 9\n");
            break;
        case (1<<8):
            printk("unknown 8\n");
            break;
        case (1<<7):
            printk("unknown 7\n");
            break;
        case (1<<6):
            printk("unknown 6\n");
            break;
        case (1<<5):
            printk("Link speed auto-downspeed detect interrupt status\n");
            break;
        case (1<<4):
            printk("MDI polarity change detect interrupt status\n");
            break;
        case (1<<3):
            printk("MDIX change detect interrupt status\n");
            break;
        case (1<<2):
            printk("duplex mode change detect interrupt status\n");
            break;
        case (1<<1):
            printk("link speed change interrupt status\n");
            break;
        case (1<<0):
            printk("link state change interrupt status\n");
            break;//link state change
        default:
            break;
        }


    }
}
//*****************************************************************************************************************
/**
*handle MMI-Alalska-PHY interrupt
*@param id ID of PHY
*/
void INR_MMI_ALASKA_PHY_PTP_interrupt(uint8_t id) {




}
//*****************************************************************************************************************
/**
*handle MMI-PHY hard reset
*/
void INR_MMI_PHY_hard_reset() {
    uint8_t i=0;
    printk("%s\n", __func__);
    for (i=1; i<2; i++) {
        INR_MDIO_ALASKA_init(i);
    }



}
//*****************************************************************************************************************
/**
*handle MMI-GPHY soft reset
*/
void INR_MMI_GPHY_soft_reset() {
    uint8_t i=0;
    printk("%s\n", __func__);
    for (i=1; i<10; i++) {
        INR_MDIO_GPHY_init(i);
    }



}

