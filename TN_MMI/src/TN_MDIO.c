#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/export.h>
#include <linux/init.h>
#include "TN_MMI.h"
#include "TN_MDIO.h"

//*****************************************************************************************************************
/**
*init mmid address for MDIO
*@param bar1base mmi address of bar1
*/
void INR_MDIO_init(uint64_t bar1base) {
    INR_MMI_PHY_hard_reset();
    INR_MMI_GPHY_soft_reset();

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
    uint16_t *rd_data=data;
    value=INR_PCI_MMI_read(INR_MDIO_read_addr);
    busy=1&(value>>31);
    if(data)rd_data=value&0xffff;//check if nullpointer
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

