#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/version.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/export.h>
#include <linux/delay.h>
#include <linux/device.h>
#include <linux/init.h>
#include "TN_MMI.h"
#include "TN_MDIO.h"
#include "tn_env.h"
#include <linux/kthread.h>
//#include <linux/uio_driver.h>

#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,14,50)
#include <linux/sched/signal.h>
#endif
//static struct uio_info *can_userspace_interrupt;
//static int irq = 1;
static struct device *dev;
uint16_t INTERRRUPT_MASK=0x13ff;
uint32_t COMMON_INTERRUPT_MASK=0xffffffff;
uint8_t pollcount=0,pollcount2=0;
void (*TIME_int_handler)(void);
volatile uint32_t COMMON_INTERRUPT_STATE=0;
void (*INR_NW_carrier_update_handler)(uint8_t id, uint16_t status);
static DECLARE_WAIT_QUEUE_HEAD (INR_MMI_phy_state_watch_wq);
uint64_t gBaseVirt1_MMI = NULL;
uint8_t portcount=0;
/*
static irqreturn_t can_int_handler(int irq, struct uio_info *dev_info)
{
	static int count = 0;
	printk(KERN_INFO "In UIO handler\n");
	return IRQ_HANDLED;
}
static int can_int_init(void)
{
	dev = kzalloc(sizeof(struct device), GFP_KERNEL);
	dev_set_name(dev, "TrustNode_CAN");
	dev->release = can_int_release;
	device_register(dev);

	can_userspace_interrupt = kzalloc(sizeof(struct uio_info), GFP_KERNEL);
	can_userspace_interrupt->name = "TrustNode_CAN_uio";
	can_userspace_interrupt->version = "0.0.1";
	can_userspace_interrupt->irq = irq;
	can_userspace_interrupt->irq_flags = IRQF_SHARED;
	can_userspace_interrupt->handler = can_int_handler;

	if (uio_register_device(dev, can_userspace_interrupt) < 0) {
		device_unregister(dev);
		kfree(dev);
		kfree(can_userspace_interrupt);
		printk(KERN_INFO "Failing to register uio device\n");
		return -1;
	}
	printk(KERN_INFO "Registered UIO handler for IRQ=%d\n", irq);
	return 0;
}

static void  can_int_exit(void)
{
	uio_unregister_device(can_userspace_interrupt);
	device_unregister(dev);
	printk(KERN_INFO "Un-Registered UIO handler for IRQ=%d\n", irq);
	kfree(can_userspace_interrupt);
	kfree(dev);
}
static void can_int_release(struct device *dev)
{
	printk(KERN_INFO "releasing my uio device\n");
}
*/

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
void INR_MMI_init(uint64_t bar1) {
    gBaseVirt1_MMI=bar1;//get baseaddr from
    if(ENABLE)if(gBaseVirt1_MMI) {
            if(!INR_NW_carrier_update_handler) {
                printk("error INR_NW_carrier_update\n");
                INR_NW_carrier_update_handler=symbol_get(INR_NW_carrier_update);
                if(!INR_NW_carrier_update_handler) printk("error INR_NW_carrier_update not registred\n");
            }
            portcount=0xff&INR_PCI_BAR1_read(C_BASE_ADDR_COMMON_LOWER*256+C_SUB_ADDR_COMMON_PARAM_PRT_CNT);
            printk("portcount:%i\n",portcount);
            if (portcount>100) INR_MMI_common_interrupt_handler(1);// reflash interrupt, because portcount is wrong
            if (portcount>100) INR_MMI_common_interrupt_handler(1);
            if (portcount>100) INR_MMI_common_interrupt_handler(1);//be sure, something happs alsi in init stage
            INR_PCI_BAR1_write(COMMON_INTERRUPT_MASK,INR_MMI_common_interrupt_mask);//settin interrupt mask
            INR_PCI_BAR1_write(INTERRRUPT_MASK,INR_MMI_interrupt_mask);//settin interrupt mask
            INR_MMI_PHY_interrupt(INTERRRUPT_MASK);//read all phy registers
            INR_PCI_BAR1_read(INR_MDIO_interrupt);
            INR_PCI_BAR1_read(INR_MMI_interrupt_status);
            wake_up_interruptible (&INR_MMI_phy_state_watch_wq);
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
            /*
                INR_MDIO_write(0,INR_MDIO_GPHY_REG_IMASK,7);
                INR_MDIO_write(1,INR_MDIO_GPHY_REG_IMASK,7);
                INR_MDIO_write(2,INR_MDIO_GPHY_REG_IMASK,7);
                INR_MDIO_write(3,INR_MDIO_GPHY_REG_IMASK,7);
                INR_MDIO_write(4,INR_MDIO_GPHY_REG_IMASK,7);
                INR_MDIO_write(5,INR_MDIO_GPHY_REG_IMASK,7);
                INR_MDIO_write(6,INR_MDIO_GPHY_REG_IMASK,7);
                INR_MDIO_write(7,INR_MDIO_GPHY_REG_IMASK,7);
                INR_MDIO_write(8,INR_MDIO_GPHY_REG_IMASK,7);
                INR_MDIO_write(9,INR_MDIO_GPHY_REG_IMASK,7);
            */
        }
    printk("INR_MMI_init base:0x%lx\n",gBaseVirt1_MMI);
    //if(gBaseVirt1_MMI)INR_MDIO_init(gBaseVirt1_MMI);//init mdio
    TIME_int_handler= symbol_get(INR_TIME_TX_transmit_interrupt);
    INR_NW_carrier_update_handler=symbol_get(INR_NW_carrier_update);
    wake_up_interruptible (&INR_MMI_phy_state_watch_wq);
    wake_up_interruptible (&INR_MMI_phy_state_watch_wq);

	//can_int_init();
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
    


    //printk("Flowcache EMA has entry 1 cleared\n");
    if(ENABLE) {
    
//#if C_MMI_ADDR_MAP_REVISION < 8
uint32_t MDIO_int=INR_PCI_BAR1_read(INR_MDIO_interrupt); // get MDIO interrupt mask
// ignoring INR_MDIO_interrupt
//##############TX_timestamp interrupt       
        if(TIME_int_handler)TIME_int_handler();//call TX_timestamp interrupt
        else {
            printk("error TIME_int_handler not registred\n");
            TIME_int_handler=symbol_get(INR_TIME_TX_transmit_interrupt);
            if(TIME_int_handler)TIME_int_handler();//call TX_timestamp interrupt
            else printk("error TIME_int_handler not registred\n");
        }
//############################################################################
//##############carrier update interrupt
        if(!INR_NW_carrier_update_handler) {
            printk("error INR_NW_carrier_update\n");
            INR_NW_carrier_update_handler=symbol_get(INR_NW_carrier_update);
            if(!INR_NW_carrier_update_handler) printk("error INR_NW_carrier_update not registred\n");
        }
        INR_MMI_PHY_interrupt(MDIO_int&INTERRRUPT_MASK); //call MDIO interrupt for carrier update
//############################################################################
/*        INR_PCI_BAR1_read(INR_MDIO_interrupt);*/
/*        pollcount=0;*/
/*        while(INTERRRUPT_MASK&INR_PCI_BAR1_read(INR_MDIO_interrupt)) {*/
/*            INR_MMI_PHY_interrupt(INTERRRUPT_MASK);//if not reset poll allu*/
/*            pollcount++;*/
/*            if(pollcount>=INR_MAX_POLL) {*/
/*                //INTERRRUPT_MASK=0;*/
/*                printk("INR int error, max pollcount reached for MDIO interrupt, forcing interrupt mask to 0\n");*/
/*                break;*/
/*            }*/
/*        }*/
/*        pollcount=0;*/
        INR_MMI_common_interrupt_handler(COMMON_INTERRUPT_MASK&INR_PCI_BAR1_read(INR_MMI_common_interrupt_status)); // call common interrupt
/*        while(COMMON_INTERRUPT_MASK&INR_PCI_BAR1_read(INR_MMI_common_interrupt_status)) {*/
/*            INR_MMI_common_interrupt_handler(COMMON_INTERRUPT_MASK&INR_PCI_BAR1_read(INR_MMI_common_interrupt_status));*/
/*            pollcount++;*/
/*            if(pollcount>=INR_MAX_POLL) {*/
/*                //COMMON_INTERRUPT_MASK=0;*/
/*                printk("INR int error, max pollcount reached for common interrupt  interrupt, forcing interrupt mask to 0\n");*/
/*                break;*/
/*            }*/
/*        }*/
        //if(MDIO_int&(3<<16))INR_MMI_ALASKA_PHY_PTP_interrupt((MDIO_int>>16)&3);
        //if(MDIO_int&(1<<30))INR_MMI_PHY_hard_reset();
        //if(MDIO_int&(1<<31))INR_MMI_PHY_hard_reset();
//        INR_PCI_BAR1_read(INR_MDIO_interrupt);//read again to reset

				
/*        pollcount=0;*/
/*        while() { //read to reset*/
/*            pollcount++;*/
/*            if(pollcount>=INR_MAX_POLL) {*/
/*                //INTERRRUPT_MASK=0;*/
/*                printk("INR int error, max pollcount reached for MMI interrupt, forcing interrupt mask to 0\n");*/
/*                break;*/
/*            }*/
/*        }*/

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
        if (portcount>0)INR_PCI_BAR1_read(INR_HC_INTERRUPT(0));  // hc interrupts handled by tx interrupt ISW so reset all
        if (portcount>1)INR_PCI_BAR1_read(INR_HC_INTERRUPT(1));
        if (portcount>2)INR_PCI_BAR1_read(INR_HC_INTERRUPT(2));
        if (portcount>3)INR_PCI_BAR1_read(INR_HC_INTERRUPT(3));
        if (portcount>4)INR_PCI_BAR1_read(INR_HC_INTERRUPT(4));
        if (portcount>5)INR_PCI_BAR1_read(INR_HC_INTERRUPT(5));
        if (portcount>6)INR_PCI_BAR1_read(INR_HC_INTERRUPT(6));
        if (portcount>7)INR_PCI_BAR1_read(INR_HC_INTERRUPT(7));
        if (portcount>8)INR_PCI_BAR1_read(INR_HC_INTERRUPT(8));
        if (portcount>9)INR_PCI_BAR1_read(INR_HC_INTERRUPT(9));
        if (portcount>10)INR_PCI_BAR1_read(INR_HC_INTERRUPT(10));
        if (portcount>11)INR_PCI_BAR1_read(INR_HC_INTERRUPT(11));
        
        
        INR_PCI_BAR1_read(INR_MMI_interrupt_status); // reset global interrupt bit
        
        
        // because sometimes reset does not work... call again...
        if(INR_PCI_BAR1_read(INR_MDIO_interrupt))INR_MMI_interrupt_handler();
        if(INR_PCI_BAR1_read(INR_MMI_interrupt_status))INR_MMI_interrupt_handler();
        if (portcount>0)if(INR_PCI_BAR1_read(INR_HC_INTERRUPT(0)))INR_MMI_interrupt_handler();  // hc interrupts handled by tx interrupt ISW so reset all
        if (portcount>1)if(INR_PCI_BAR1_read(INR_HC_INTERRUPT(1)))INR_MMI_interrupt_handler();
        if (portcount>2)if(INR_PCI_BAR1_read(INR_HC_INTERRUPT(2)))INR_MMI_interrupt_handler();
        if (portcount>3)if(INR_PCI_BAR1_read(INR_HC_INTERRUPT(3)))INR_MMI_interrupt_handler();
        if (portcount>4)if(INR_PCI_BAR1_read(INR_HC_INTERRUPT(4)))INR_MMI_interrupt_handler();
        if (portcount>5)if(INR_PCI_BAR1_read(INR_HC_INTERRUPT(5)))INR_MMI_interrupt_handler();
        if (portcount>6)if(INR_PCI_BAR1_read(INR_HC_INTERRUPT(6)))INR_MMI_interrupt_handler();
        if (portcount>7)if(INR_PCI_BAR1_read(INR_HC_INTERRUPT(7)))INR_MMI_interrupt_handler();
        if (portcount>8)if(INR_PCI_BAR1_read(INR_HC_INTERRUPT(8)))INR_MMI_interrupt_handler();
        if (portcount>9)if(INR_PCI_BAR1_read(INR_HC_INTERRUPT(9)))INR_MMI_interrupt_handler();
        if (portcount>10)if(INR_PCI_BAR1_read(INR_HC_INTERRUPT(10)))INR_MMI_interrupt_handler();
        if (portcount>11)if(INR_PCI_BAR1_read(INR_HC_INTERRUPT(11)))INR_MMI_interrupt_handler();
        
        /*
        pollcount=0;
        while(INR_PCI_BAR1_read(INR_PCI_BAR1_read(INR_HC_INTERRUPT(0)))){//read to reset
        pollcount++;
        if(pollcount>=INR_MAX_POLL){
        	INTERRRUPT_MASK=0;
        	printk("INR int error, max pollcount reached for INR_HC_INTERRUPT0, forcing interrupt mask to 0\n");
        	break;}
        	}
        pollcount=0;
        while(INR_PCI_BAR1_read(INR_PCI_BAR1_read(INR_HC_INTERRUPT(1)))){//read to reset
        pollcount++;
        if(pollcount>=INR_MAX_POLL){
        	INTERRRUPT_MASK=0;
        	printk("INR int error, max pollcount reached for INR_HC_INTERRUPT1, forcing interrupt mask to 0\n");
        	break;}
        }
        pollcount=0;
        while(INR_PCI_BAR1_read(INR_PCI_BAR1_read(INR_HC_INTERRUPT(2)))){//read to reset
        pollcount++;
        if(pollcount>=INR_MAX_POLL){
        	INTERRRUPT_MASK=0;
        	printk("INR int error, max pollcount reached for INR_HC_INTERRUPT2, forcing interrupt mask to 0\n");
        	break;}
        }
        pollcount=0;
        while(INR_PCI_BAR1_read(INR_PCI_BAR1_read(INR_HC_INTERRUPT(3)))){//read to reset
        pollcount++;
        if(pollcount>=INR_MAX_POLL){
        	INTERRRUPT_MASK=0;
        	printk("INR int error, max pollcount reached for INR_HC_INTERRUPT3, forcing interrupt mask to 0\n");
        	break;}
        }
        pollcount=0;
        while(INR_PCI_BAR1_read(INR_PCI_BAR1_read(INR_HC_INTERRUPT(4)))){//read to reset
        pollcount++;
        if(pollcount>=INR_MAX_POLL){
        	INTERRRUPT_MASK=0;
        	printk("INR int error, max pollcount reached for INR_HC_INTERRUPT4, forcing interrupt mask to 0\n");
        	break;}
        }
        pollcount=0;
        while(INR_PCI_BAR1_read(INR_PCI_BAR1_read(INR_HC_INTERRUPT(5)))){//read to reset
        pollcount++;
        if(pollcount>=INR_MAX_POLL){
        	INTERRRUPT_MASK=0;
        	printk("INR int error, max pollcount reached for INR_HC_INTERRUPT5, forcing interrupt mask to 0\n");
        	break;}
        }
        pollcount=0;
        while(INR_PCI_BAR1_read(INR_PCI_BAR1_read(INR_HC_INTERRUPT(6)))){//read to reset
        pollcount++;
        if(pollcount>=INR_MAX_POLL){
        	INTERRRUPT_MASK=0;
        	printk("INR int error, max pollcount reached for INR_HC_INTERRUPT6, forcing interrupt mask to 0\n");
        	break;}
        }
        pollcount=0;
        while(INR_PCI_BAR1_read(INR_PCI_BAR1_read(INR_HC_INTERRUPT(7)))){//read to reset
        pollcount++;
        if(pollcount>=INR_MAX_POLL){
        	INTERRRUPT_MASK=0;
        	printk("INR int error, max pollcount reached for INR_HC_INTERRUPT7, forcing interrupt mask to 0\n");
        	break;}
        }
        pollcount=0;
        while(INR_PCI_BAR1_read(INR_PCI_BAR1_read(INR_HC_INTERRUPT(8)))){//read to reset
        pollcount++;
        if(pollcount>=INR_MAX_POLL){
        	INTERRRUPT_MASK=0;
        	printk("INR int error, max pollcount reached for INR_HC_INTERRUPT8, forcing interrupt mask to 0\n");
        	break;}
        }
        pollcount=0;
        while(INR_PCI_BAR1_read(INR_PCI_BAR1_read(INR_HC_INTERRUPT(9)))){//read to reset
        pollcount++;
        if(pollcount>=INR_MAX_POLL){
        	INTERRRUPT_MASK=0;
        	printk("INR int error, max pollcount reached for INR_HC_INTERRUPT9, forcing interrupt mask to 0\n");
        	break;}
        }
        pollcount=0;
        while(INR_PCI_BAR1_read(INR_PCI_BAR1_read(INR_HC_INTERRUPT(10)))){//read to reset
        pollcount++;
        if(pollcount>=INR_MAX_POLL){
        	INTERRRUPT_MASK=0;
        	printk("INR int error, max pollcount reached for INR_HC_INTERRUPT10, forcing interrupt mask to 0\n");
        	break;}
        }
        pollcount=0;
        while(INR_PCI_BAR1_read(INR_PCI_BAR1_read(INR_HC_INTERRUPT(11)))){//read to reset
        pollcount++;
        if(pollcount>=INR_MAX_POLL){
        	INTERRRUPT_MASK=0;
        	printk("INR int error, max pollcount reached for INR_HC_INTERRUPT11, forcing interrupt mask to 0\n");
        	break;}
        }
        */
/*#else
uint32_t MMI_interrupt=INR_PCI_BAR1_read(INR_MMI_interrupt_status);
uint8_t i=0;
if(i=C_MMI_INT_HC_LOWER;i<C_MMI_INT_HC_UPPER;i++)if(MMI_interrupt&(1<<i)){
					        if (portcount>0)INR_PCI_BAR1_read(INR_HC_INTERRUPT(0));  // hc interrupts handled by tx interrupt ISW so reset all
									if (portcount>1)INR_PCI_BAR1_read(INR_HC_INTERRUPT(1));
									if (portcount>2)INR_PCI_BAR1_read(INR_HC_INTERRUPT(2));
									if (portcount>3)INR_PCI_BAR1_read(INR_HC_INTERRUPT(3));
									if (portcount>4)INR_PCI_BAR1_read(INR_HC_INTERRUPT(4));
									if (portcount>5)INR_PCI_BAR1_read(INR_HC_INTERRUPT(5));
									if (portcount>6)INR_PCI_BAR1_read(INR_HC_INTERRUPT(6));
									if (portcount>7)INR_PCI_BAR1_read(INR_HC_INTERRUPT(7));
									if (portcount>8)INR_PCI_BAR1_read(INR_HC_INTERRUPT(8));
									if (portcount>9)INR_PCI_BAR1_read(INR_HC_INTERRUPT(9));
									if (portcount>10)INR_PCI_BAR1_read(INR_HC_INTERRUPT(10));
									if (portcount>11)INR_PCI_BAR1_read(INR_HC_INTERRUPT(11));
							}
if(MMI_interrupt&(1<<C_MMI_INT_MDIO)){
									uint32_t MDIO_int=INR_PCI_BAR1_read(INR_MDIO_interrupt); // get MDIO interrupt mask
									INR_MMI_PHY_interrupt(MDIO_int&INTERRRUPT_MASK);
}
if(MMI_interrupt&(1<<C_MMI_INT_RTC)){
									INR_PCI_BAR1_read((C_BASE_ADDR_RTC<<8)|C_SUB_ADDR_RTC_INTERRUPT);
}
if(MMI_interrupt&(1<<C_MMI_INT_PERIPHERALS)){
									INR_PCI_BAR1_read((C_BASE_ADDR_PERIPH<<8)|C_SUB_ADDR_PERIPH_INTERRUPT);
}
if(MMI_interrupt&(1<<C_MMI_INT_DEBUG)){
									INR_PCI_BAR1_read((C_BASE_ADDR_DEBUG_LOWER<<8)|C_SUB_ADDR_DEBUG_INTERRUPT);
}
if(MMI_interrupt&(1<<C_MMI_INT_COMMON)){
									INR_MMI_common_interrupt_handler(COMMON_INTERRUPT_MASK&INR_PCI_BAR1_read(INR_MMI_common_interrupt_status));
}
if(MMI_interrupt&(1<<C_MMI_INT_ETH_SW)){
									//INR_PCI_BAR1_read((C_BASE_ADDR_ETH_SW<<8)|C_SUB_ADDR_DEBUG_INTERRUPT);
}
if(MMI_interrupt&(1<<C_MMI_INT_CAN )){
									INR_MMI_can_interrupt_handler(INR_PCI_BAR1_read((C_BASE_ADDR_CAN_COMMON<<8)|C_SUB_ADDR_CAN_INTERRUPT));
}
#endif */
    }
}
//*****************************************************************************************************************
/**
*give mmi address back
*
*/
void INR_MMI_exit() {
    symbol_put(INR_TIME_TX_transmit_interrupt);
    symbol_put(INR_NW_carrier_update);
    //can_int_exit();
    printk("%s\n", __func__);
}
//*****************************************************************************************************************
/**
*handle MMI-PHY interrupt
*@param id ID of PHY
*/
void INR_MMI_PHY_interrupt(uint16_t id) {
    //printk("MMI_PHY_INTERRUPT: 0x%lx\n",id);
    uint8_t phyid=0,i=0,j=0;
    uint16_t read_val_gphy,read_val_alaska;
    for (i=0; i<12; i++)if(id&(1<<i))if(i<10) {
                phyid=i+16;
                pollcount2=0;
                read_val_gphy=INR_MDIO_read(phyid,INR_MDIO_GPHY_REG_IMASK)&INR_MDIO_read(phyid,INR_MDIO_GPHY_REG_ISTAT);

                while(INR_MDIO_read(phyid,INR_MDIO_GPHY_REG_IMASK)&INR_MDIO_read(phyid,INR_MDIO_GPHY_REG_ISTAT)) {
                    udelay(10);
                    pollcount2++;
                    if(pollcount2>=INR_MAX_POLL)break;
                }

                for(j=0; j<16; j++)switch(read_val_gphy&(1<<j)) {
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
                        //udelay(1000);
                        //wake_up_interruptible (&INR_MMI_phy_state_watch_wq);
                        if(INR_NW_carrier_update_handler)INR_NW_carrier_update_handler(i,(1<<2)&INR_MDIO_read(i+16,0x1));
                        if((1<<2)&INR_MDIO_read(i+16,0x1))INR_GPHY_adapt_speed(i+16);
                        //INR_collective_max_speed();
                        //if(i==0){
                        //INR_PCI_BAR1_write(0x0,0xc00040);
                        //INR_PCI_BAR1_write(0x0,0xc00060);
                        //printk("del collT entry 0\n");
                        //}
                        break;//link state change
                    default:
                        break;
                    }







            } else {
                if((i=10))phyid=0;
                if((i==11))phyid=1;

                read_val_alaska=INR_MDIO_read(phyid,INR_MDIO_ALASKA_PHY_REG_IMASK)&INR_MDIO_read(phyid,INR_MDIO_ALASKA_PHY_REG_ISTAT);
                for(j=0; j<16; j++)switch(read_val_alaska&(1<<j)) {
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
//*****************************************************************************************************************
/**
*handle MMI-GPHY soft reset
*/
int INR_MMI_phy_state_watch(void *nix) {
    DECLARE_WAITQUEUE (wait2, current);
    allow_signal (SIGKILL);
    add_wait_queue (&INR_MMI_phy_state_watch_wq, &wait2);
    while (1) {
        set_current_state (TASK_INTERRUPTIBLE);
        schedule ();
        if (signal_pending (current))
            break;
        //mdelay(5000);
        uint8_t i=0;
        for(i=0; i<=9; i++)if(INR_NW_carrier_update_handler)INR_NW_carrier_update_handler(i,(1<<2)&INR_MDIO_read(i+16,0x1));
        //INR_collective_max_speed();//adjust speed of phys
    }
    set_current_state (TASK_RUNNING);
    remove_wait_queue (&INR_MMI_phy_state_watch_wq, &wait2);
    return 0;




}
void INR_MMI_phy_state_watch_start() {

    kthread_run (&INR_MMI_phy_state_watch, NULL, "INR_MMI_phy_state_watch");
    wake_up_interruptible (&INR_MMI_phy_state_watch_wq);
}
void INR_MMI_phy_state_watch_wakeup() {

    wake_up_interruptible (&INR_MMI_phy_state_watch_wq);
}
void INR_MMI_common_interrupt_handler(uint32_t status) {
    if(status) { //error status interrupt, will be polled externally
        printk("INR common interrupt:0x%llx\n",status);
        COMMON_INTERRUPT_STATE=status;
    }
}
uint32_t INR_MMI_common_interrupt_get() {
    return COMMON_INTERRUPT_STATE;

}

void INR_MMI_common_interrupt_reset() {
    COMMON_INTERRUPT_STATE=0;

}

void INR_MMI_can_interrupt_handler(uint32_t mask){

//uio_event_notify(can_userspace_interrupt);


}


