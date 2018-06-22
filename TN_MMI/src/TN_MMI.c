#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/export.h>
#include <linux/init.h>
#include "TN_MMI.h"
#include "TN_MDIO.h"
#include "tn_env.h"
uint16_t INTERRRUPT_MASK=0x13ff;

void *gBaseVirt1_MMI = NULL;
//*****************************************************************************************************************
/**
*read from mmi addr
*
*/
uint32_t INR_PCI_MMI_read(uint64_t addr) {
    uint32_t val=INR_PCI_BAR1_read(addr);
    //printk("read 0x%lx from 0x%lx, base:0x%lx\n",val,addr,gBaseVirt1_MMI);
    return val;
}

//*****************************************************************************************************************
/**
*read from mmi addr
*
*/
void INR_PCI_MMI_write(uint32_t value, uint64_t addr) {
    //printk("write 0x%lx to 0x%lx, base:0x%lx\n",value,addr,gBaseVirt1_MMI);
    return INR_PCI_BAR1_write(value,addr);
}
//*****************************************************************************************************************
/**
*init mmid address
*
*/
void INR_MMI_init(uint64_t *bar1) {
    gBaseVirt1_MMI=bar1;//get baseaddr from
    if(ENABLE)if(gBaseVirt1_MMI){
    
    INR_PCI_BAR1_write(INTERRRUPT_MASK,INR_MMI_interrupt_mask);//settin interrupt mask
    INR_MMI_PHY_interrupt(INTERRRUPT_MASK);//read all phy registers
    INR_PCI_BAR1_read(INR_MDIO_interrupt);
    INR_PCI_BAR1_read(INR_MMI_interrupt_status);
    
    INR_PCI_BAR1_write(0xffffffff,INR_HC_INTERRUPT_EN(0));
    INR_PCI_BAR1_write(0xffffffff,INR_HC_INTERRUPT_EN(1));
    INR_PCI_BAR1_write(0xffffffff,INR_HC_INTERRUPT_EN(2));
    INR_PCI_BAR1_write(0xffffffff,INR_HC_INTERRUPT_EN(3));
    INR_PCI_BAR1_write(0xffffffff,INR_HC_INTERRUPT_EN(4));
    INR_PCI_BAR1_write(0xffffffff,INR_HC_INTERRUPT_EN(5));
    INR_PCI_BAR1_write(0xffffffff,INR_HC_INTERRUPT_EN(6));
    INR_PCI_BAR1_write(0xffffffff,INR_HC_INTERRUPT_EN(7));
    INR_PCI_BAR1_write(0xffffffff,INR_HC_INTERRUPT_EN(8));
    INR_PCI_BAR1_write(0xffffffff,INR_HC_INTERRUPT_EN(9));
    INR_PCI_BAR1_write(0xffffffff,INR_HC_INTERRUPT_EN(10));
    INR_PCI_BAR1_write(0xffffffff,INR_HC_INTERRUPT_EN(11));
#ifdef C_BASE_ADDR_RTC    
    INR_PCI_BAR1_write(0xffffffff,(C_BASE_ADDR_RTC<<8)+C_SUB_ADDR_RTC_INTERRUPT_EN);
#endif    
    }
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
    /*INR_PCI_BAR1_write(0x0,0xc20010);
    INR_PCI_BAR1_write(0x0,0xc20014);
    INR_PCI_BAR1_write(0x0,0xc20018);
    INR_PCI_BAR1_write(0x0,0xc2001c);
    */
    uint32_t MDIO_int=INR_PCI_BAR1_read(INR_MDIO_interrupt);
    

    //printk("Flowcache EMA has entry 1 cleared\n");
if(ENABLE){
    INR_MMI_PHY_interrupt(MDIO_int&INTERRRUPT_MASK);
    INR_PCI_BAR1_read(INR_MDIO_interrupt);
    while(INTERRRUPT_MASK&INR_PCI_BAR1_read(INR_MDIO_interrupt))INR_MMI_PHY_interrupt(INTERRRUPT_MASK);//if not reset poll all
    //if(MDIO_int&(3<<16))INR_MMI_ALASKA_PHY_PTP_interrupt((MDIO_int>>16)&3);
    //if(MDIO_int&(1<<30))INR_MMI_PHY_hard_reset();
    //if(MDIO_int&(1<<31))INR_MMI_PHY_hard_reset();
    INR_PCI_BAR1_read(INR_MDIO_interrupt);//read again to reset
    INR_PCI_BAR1_read(INR_MMI_interrupt_status);//read to reset
    
    /*printk("HC interrupt 0:0x%x\n",INR_PCI_BAR1_read(INR_HC_INTERRUPT(0)));
    printk("HC interrupt 1:0x%x\n",INR_PCI_BAR1_read(INR_HC_INTERRUPT(1)));
    printk("HC interrupt 2:0x%x\n",INR_PCI_BAR1_read(INR_HC_INTERRUPT(2)));
    printk("HC interrupt 3:0x%x\n",INR_PCI_BAR1_read(INR_HC_INTERRUPT(3)));
    printk("HC interrupt 4:0x%x\n",INR_PCI_BAR1_read(INR_HC_INTERRUPT(4)));
    printk("HC interrupt 5:0x%x\n",INR_PCI_BAR1_read(INR_HC_INTERRUPT(5)));
    printk("HC interrupt 6:0x%x\n",INR_PCI_BAR1_read(INR_HC_INTERRUPT(6)));
    printk("HC interrupt 7:0x%x\n",INR_PCI_BAR1_read(INR_HC_INTERRUPT(7)));
    printk("HC interrupt 8:0x%x\n",INR_PCI_BAR1_read(INR_HC_INTERRUPT(8)));
    printk("HC interrupt 9:0x%x\n",INR_PCI_BAR1_read(INR_HC_INTERRUPT(9)));
    printk("HC interrupt 10:0x%x\n",INR_PCI_BAR1_read(INR_HC_INTERRUPT(10)));
    printk("HC interrupt 11:0x%x\n",INR_PCI_BAR1_read(INR_HC_INTERRUPT(11)));
    */
    INR_PCI_BAR1_read(INR_HC_INTERRUPT(0));
    INR_PCI_BAR1_read(INR_HC_INTERRUPT(1));
    INR_PCI_BAR1_read(INR_HC_INTERRUPT(2));
    INR_PCI_BAR1_read(INR_HC_INTERRUPT(3));
    INR_PCI_BAR1_read(INR_HC_INTERRUPT(4));
    INR_PCI_BAR1_read(INR_HC_INTERRUPT(5));
    INR_PCI_BAR1_read(INR_HC_INTERRUPT(6));
    INR_PCI_BAR1_read(INR_HC_INTERRUPT(7));
    INR_PCI_BAR1_read(INR_HC_INTERRUPT(8));
    INR_PCI_BAR1_read(INR_HC_INTERRUPT(9));
    INR_PCI_BAR1_read(INR_HC_INTERRUPT(10));
    INR_PCI_BAR1_read(INR_HC_INTERRUPT(11));
}
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
	printk("MMI_PHY_INTERRUPT: 0x%lx\n",id);
    uint8_t phyid=0,i=0,j=0;
    uint16_t read_val_gphy,read_val_alaska;
    	for (i=0;i<12;i++)if(id&(1<<i))if(i<10){
    		phyid=i+16;
    		read_val_gphy=INR_MDIO_read(phyid,INR_MDIO_GPHY_REG_IMASK)&INR_MDIO_read(phyid,INR_MDIO_GPHY_REG_ISTAT);
    		for(j=0; j<16; j++)switch(read_val_gphy&(1<<j)){
			case (1<<15):
			    printk("Port %i: wake on lan status\n",i);
			    break;
			case (1<<14):
			    printk("Port %i: master/slave resulution error\n",i);
			    break;
			case (1<<13):
			    printk("Port %i: next page received interrupt status\n",i);
			    break;
			case (1<<12):
			    printk("Port %i: next page transmittwed interrupt status\n",i);
			    break;
			case (1<<11):
			    printk("Port %i: auto-negotiation error interrupt status\n",i);
			    break;
			case (1<<10):
			    printk("Port %i: auto-negotiation complete interrupt status\n",i);
			    break;
			case (1<<9):
			    printk("Port %i: unknown 9\n",i);
			    break;
			case (1<<8):
			    printk("Port %i: unknown 8\n",i);
			    break;
			case (1<<7):
			    printk("Port %i: unknown 7\n",i);
			    break;
			case (1<<6):
			    printk("Port %i: unknown 6\n",i);
			    break;
			case (1<<5):
			    printk("Port %i: Link speed auto-downspeed detect interrupt status\n",i);
			    break;
			case (1<<4):
			    printk("Port %i: MDI polarity change detect interrupt status\n",i);
			    break;
			case (1<<3):
			    printk("Port %i: MDIX change detect interrupt status\n",i);
			    break;
			case (1<<2):
			    printk("Port %i: duplex mode change detect interrupt status\n",i);
			    break;
			case (1<<1):
			    printk("Port %i: link speed change interrupt status\n",i);
			    INR_GPHY_adapt_speed(i+16);
			    break;
			case (1<<0):
			    printk("Port %i: link state change interrupt status\n",i);
			    break;//link state change
			default:
			    break;
			}
    	
    	
    	
    	
    	
    	
    	}else{
    		if((i=10))phyid=0;
        	if((i==11))phyid=1;
        	
    		read_val_alaska=INR_MDIO_read(phyid,INR_MDIO_ALASKA_PHY_REG_IMASK)&INR_MDIO_read(phyid,INR_MDIO_ALASKA_PHY_REG_ISTAT);
    		for(j=0; j<16; j++)switch(read_val_alaska&(1<<j)){
			case (1<<15):
			    printk("Port %i: copper autonegotioation error\n",i);
			    break;
			case (1<<14):
			    printk("Port %i: copper speed change\n",i);
			    INR_ALASKA_adapt_speed(phyid);
			    break;
			case (1<<13):
			    printk("Port %i: copper duplex change\n",i);
			    break;
			case (1<<12):
			    printk("Port %i: copper page received\n",i);
			    break;
			case (1<<11):
			    printk("Port %i: copper autonegotiation complete\n",i);
			    break;
			case (1<<10):
			    printk("Port %i: copper link status changed\n",i);
			    break;
			case (1<<9):
			    printk("Port %i: copper symbol error\n",i);
			    break;
			case (1<<8):
			    printk("Port %i: copper false carrier\n",i);
			    break;
			case (1<<7):
			    printk("Port %i: WOL event happened\n",i);
			    break;
			case (1<<6):
			    printk("Port %i: MDI crossover changed\n",i);
			    break;
			case (1<<5):
			    printk("Port %i: downshift interrupt\n",i);
			    break;
			case (1<<4):
			    printk("Port %i: copper energy detect changed\n",i);
			    break;
			case (1<<3):
			    printk("Port %i: FLP exchange complete but no link\n",i);
			    break;
			case (1<<2):
			    printk("Port %i: DTE power status changed interrupt\n",i);
			    break;
			case (1<<1):
			    printk("Port %i: polarity changed\n",i);
			    break;
			case (1<<0):
			    printk("Port %i: jabber\n",i);
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



