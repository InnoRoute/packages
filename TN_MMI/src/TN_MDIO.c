#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/export.h>
#include <linux/init.h>
#include <linux/delay.h>
#include "TN_MMI.h"
#include "TN_MDIO.h"

//*****************************************************************************************************************
/**
*init mmid address for MDIO
*@param bar1base mmi address of bar1
*/
void INR_MDIO_init(uint64_t *bar1base) {
    INR_MMI_PHY_hard_reset();
    INR_MMI_GPHY_soft_reset();

}
//*****************************************************************************************************************
/**
*get mac multispeed capabilities
*
*/
uint8_t INR_MAC_multispeed() {
//return 0; //disable
#ifdef C_SUB_ADDR_COMMON_FEATURES_NET
    return INR_PCI_MMI_read((C_BASE_ADDR_COMMON_LOWER<<8)+C_SUB_ADDR_COMMON_FEATURES_NET)&0x2;
#else
    return 0;
#endif

}


//*****************************************************************************************************************
/**
*write data to mdio (baremetal)
*@param PHY_addr address of ethernet PHY
*@param REG_addr address of register
*@param data data to write
*/
void INR_MDIO_write_b(uint8_t write, uint8_t PHY_addr, uint8_t REG_addr, uint16_t data) {
    uint32_t value=0,i=0;
    value=((1&write)<<26)|((PHY_addr&0x1f)<<21)|((REG_addr&0x1f)<<16)|(data);
    INR_PCI_MMI_write(value,INR_MDIO_write_addr);
    udelay(50);
    while(INR_MDIO_read_b(NULL)) { //wait until ready
        i++;
        if(i>INR_MDIO_timeout)break;


    }
}
//*****************************************************************************************************************
/**
*write data to mdio (baremetal)
*@param PHY_addr address of ethernet PHY
*@param REG_addr address of register
*@param data data to write
*/
void INR_MDIO_write(uint8_t PHY_addr, uint8_t REG_addr, uint16_t data) {
    INR_MDIO_write_b(1,PHY_addr,REG_addr,data);
}

//*****************************************************************************************************************
/**
*read data from mdio (baremtal))
*@param PHY_addr address of ethernet PHY
*@param REG_addr address of register
*@param *data address of data to write
*@return busy bit
*/
uint8_t INR_MDIO_read_b(uint16_t *data) {
    uint8_t busy=0;
    uint32_t value=0;
    //uint16_t *rd_data=data;
    value=INR_PCI_MMI_read(INR_MDIO_read_addr);
    udelay(50);
    if (value==0xeeeeeeee) {
        printk("MMI read timeout\n");
        return 0;
    }
    busy=1&(value>>31);
    if(data)*data=value&0xffff;//check if nullpointer
    //printk("value=0x%x\n",value);
    return busy;
}

//*****************************************************************************************************************
/**
*write data to mdio
*@param PHY_addr address of ethernet PHY
*@param REG_addr address of register
*@param data data to write
*/
uint16_t INR_MDIO_read(uint8_t PHY_addr, uint8_t REG_addr) {
    uint16_t data;
    INR_MDIO_write_b(0,PHY_addr,REG_addr,0);//send read request
    INR_MDIO_read_b(&data);//read value
    //printk("value=0x%x\n",data);
    return data;
}
//*****************************************************************************************************************
/**
*init Gphy
*@param id id of gphy (0..10)
*/
void INR_MDIO_GPHY_init(uint8_t id) {
    printk("%s %i\n", __func__,id);
    id+=16; //adjust MDIO address GPHY beginning from 16
    INR_MDIO_write(id,0x0,(1<<12)|(1<<9));//Autoneg_enable|autoneg_restart
    INR_MDIO_write(id,0x9,(1<<9)|(1<<8));//1000FullDuplex|1000HalfDuplex
    INR_MDIO_write(id,0x19,(1<<1)|(1<<0));//link_speed_change_int_enable|link_state_change_int_enable
}
//*****************************************************************************************************************
/**
*init Gphy
*@param id id of gphy (0..10)
*/
void INR_MDIO_ALASKA_init(uint8_t id) {
    printk("%s %i\n", __func__,id);
    INR_MDIO_write(id,0,(1<<13)|(1<<6)|(1<<12)|(1<<9)|(1<<8));//1000Base|autoneg_enable|autoneg_restart|fullduplex
    INR_MDIO_write(id,9,(1<<9)|(1<<8));//1000fulldoplex_adv|1000halfduplex_adv
    INR_MDIO_write(id,18,(1<<14)|(1<<10));//link_speed_change_int_enable|link_state_change_int_enable
}
//*****************************************************************************************************************
/**
*get speed of gphy
*@param id id of gphy (0..10)
*/
uint8_t INR_MDIO_GPHY_getspeed(uint8_t id) {
    return INR_MDIO_read(id,INR_MDIO_GPHY_REG_MIISTAT)&0x7;
}

//*****************************************************************************************************************
/**
*get speed of gphys linkpartner
*@param id id of gphy (0..10)
*/
uint8_t INR_MDIO_GPHY_getpartnerspeed(uint8_t id) {
    uint8_t maxspeed=2;//if phy is off don't care
    if(INR_MDIO_read(id,INR_MDIO_GPHY_REG_LPA)&0x40)maxspeed=0; //support 10full
    if(INR_MDIO_read(id,INR_MDIO_GPHY_REG_LPA)&0x100)maxspeed=1; //support 100full
    if(INR_MDIO_read(id,0xa)&0x800)maxspeed=2; //support 1000full

    return maxspeed;
}
//*****************************************************************************************************************
/**
*get speed of alaska phy
*@param id id of alaska (0,1)
*/
uint8_t INR_MDIO_ALASKA_getspeed(uint8_t id) {
    return ((INR_MDIO_read(id,INR_MDIO_ALASKA_REG_MIISTAT)&0xC000)>>14);
}
//*****************************************************************************************************************
/**
*adapt gphy speed
*@param id id of gphy (0..10)
*/
void INR_GPHY_adapt_speed(uint8_t id) {
    uint16_t physpeed=INR_MDIO_GPHY_getspeed(id);
    uint32_t macspeed_addr=0;
    /*switch(id-16){
    	case 0:macspeed_addr=INR_MAC_SPEED(0);break;
    	case 1:macspeed_addr=INR_MAC_SPEED(1);break;
    	case 2:macspeed_addr=INR_MAC_SPEED(2);break;
    	case 3:macspeed_addr=INR_MAC_SPEED(3);break;
    	case 4:macspeed_addr=INR_MAC_SPEED(4);break;
    	case 5:macspeed_addr=INR_MAC_SPEED(5);break;
    	case 6:macspeed_addr=INR_MAC_SPEED(6);break;
    	case 7:macspeed_addr=INR_MAC_SPEED(7);break;
    	case 8:macspeed_addr=INR_MAC_SPEED(8);break;
    	case 9:macspeed_addr=INR_MAC_SPEED(9);break;
    	default:return;break;
    }*/
    if(INR_MDIO_set_FPGA_speed_to_PHY && INR_MAC_multispeed()) {
        printk("set speed of MAC %i to %i\n",id-16,physpeed);
        /*switch(physpeed){
        	case 0:break;
        	case 1:INR_PCI_MMI_write(1,macspeed_addr);break;
        	case 2:INR_PCI_MMI_write(2,macspeed_addr);break;

        	default:break;
        }*/
#ifdef C_SUB_ADDR_COMMON_FEATURES_NET
        INR_PCI_MMI_write((INR_PCI_MMI_read((C_BASE_ADDR_NET_LOWER<<8)+C_SUB_ADDR_NET_SPEED)&~(0x3<<((id-16)*2)))|(physpeed<<((id-16)*2)),(C_BASE_ADDR_NET_LOWER<<8)+C_SUB_ADDR_NET_SPEED);
#endif
        printk("new MAC speed mask:0x%llx",INR_PCI_MMI_read((C_BASE_ADDR_NET_LOWER<<8)+C_SUB_ADDR_NET_SPEED));
    } else {
        printk("MAC doesn't support multispeed\n");

    }

}
//*****************************************************************************************************************
/**
*searches maximum possible speed for system
*@brief The FPGA currently suports just one speed rate at all. This function polls all PHYs and searches for the maximum possible speed. Afterwards all faster PHYs are forced down.
*/
void INR_collective_max_speed() {
    uint8_t i=0;
    uint8_t maxspeed=2;
    INR_MDIO_write(i,0x4,0x0C01|0x0C41|0x0D01);
    INR_MDIO_write(i,0x9,0x0200);
    INR_MDIO_write(i,0x0,0x1200);
    for(i=16; i<26; i++)if(INR_MDIO_GPHY_getpartnerspeed(i)<maxspeed)maxspeed=INR_MDIO_GPHY_getpartnerspeed(i);
    printk("set maximum linkspeed for all phys to %i\n",maxspeed);
    for(i=16; i<26; i++)
    {   switch(maxspeed) {
        case 0:
            INR_MDIO_write(i,0x4,0x0C41);
            INR_MDIO_write(i,0x9,0x0);
            INR_PCI_MMI_write(0x00000000,((C_BASE_ADDR_NET_LOWER)<<8)|C_SUB_ADDR_NET_SPEED);
            break;
        case 1:
            INR_MDIO_write(i,0x4,0x0D01);
            INR_MDIO_write(i,0x9,0x0);
            INR_PCI_MMI_write(0x55555555,((C_BASE_ADDR_NET_LOWER)<<8)|C_SUB_ADDR_NET_SPEED);
            break;
        case 2:
            INR_MDIO_write(i,0x4,0x0C01);
            INR_MDIO_write(i,0x9,0x0200);
            INR_PCI_MMI_write(0xAAAAAAAA,((C_BASE_ADDR_NET_LOWER)<<8)|C_SUB_ADDR_NET_SPEED);
            break;
        }
        INR_MDIO_write(i,0x0,0x1200);//trigger reautoneg
    }
}
//*****************************************************************************************************************
/**
*adapt alaska speed
*@param id id of gphy (0..10)
*/
void INR_ALASKA_adapt_speed(uint8_t id) {
    uint16_t physpeed=INR_MDIO_ALASKA_getspeed(id);
    uint32_t macspeed_addr=0;
    /*
    switch(id){
    	case 0:macspeed_addr=INR_MAC_SPEED(10);break;
    	case 1:macspeed_addr=INR_MAC_SPEED(11);break;
    	default:return;break;
    }*/
    if(INR_MDIO_set_FPGA_speed_to_PHY  && INR_MAC_multispeed()) {
        printk("set speed of MAC %i to %i\n",id+10,physpeed);
        /*switch(physpeed){
        	case 0:INR_PCI_MMI_write(0,macspeed_addr);break;
        	case 1:INR_PCI_MMI_write(1,macspeed_addr);break;
        	case 2:INR_PCI_MMI_write(2,macspeed_addr);break;

        	default:break;
        }*/
#ifdef C_SUB_ADDR_COMMON_FEATURES_NET
        INR_PCI_MMI_write((INR_PCI_MMI_read((C_BASE_ADDR_NET_LOWER<<8)+C_SUB_ADDR_NET_SPEED)&~(0x3<<(id+10)))|(physpeed<<((id+10)*2)),(C_BASE_ADDR_NET_LOWER<<8)+C_SUB_ADDR_NET_SPEED);
#endif
        printk("new MAC speed mask:0x%llx",INR_PCI_MMI_read((C_BASE_ADDR_NET_LOWER<<8)+C_SUB_ADDR_NET_SPEED));
    }
}
void INR_MDIO_update_port_states(){
uint16_t result=0;
uint8_t i;
for (i=0; i<12; i++)if((1<<2)&INR_MDIO_read(i+16,0x1)) result|=(1<<i);
#if C_MMI_ADDR_MAP_REVISION > 8
INR_PCI_MMI_write(result,(C_BASE_ADDR_NET_LOWER<<8)+C_SUB_ADDR_NET_LINK);
printk("set NET_LINK to 0x%x",result);
#endif
}
