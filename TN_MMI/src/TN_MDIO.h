#include "tn_env.h"
void INR_MDIO_init(uint64_t *bar1base);
void INR_MDIO_write(uint8_t PHY_addr, uint8_t REG_addr, uint16_t data);
void INR_MDIO_write_b(uint8_t write, uint8_t PHY_addr, uint8_t REG_addr, uint16_t data);
uint16_t INR_MDIO_read(uint8_t PHY_addr, uint8_t REG_addr);
uint8_t INR_MDIO_read_b(uint16_t *data);
void INR_MDIO_GPHY_init(uint8_t id);
void INR_MDIO_ALASKA_init(uint8_t id);
uint8_t INR_MDIO_GPHY_getspeed(uint8_t id);
void INR_GPHY_adapt_speed(uint8_t id);
uint8_t INR_MDIO_ALASKA_getspeed(uint8_t id);
void INR_ALASKA_adapt_speed(uint8_t id);

#ifndef C_BASE_ADDR_BM
	#define C_BASE_ADDR_BM 0
	#define ENABLE 0
#endif

#ifndef C_SUB_ADDR_BM_MMI_INTERRUPT
		#define C_SUB_ADDR_BM_MMI_INTERRUPT 0
		#define ENABLE 0
#endif

#ifndef C_SUB_ADDR_MAC_SPEED
		#define C_SUB_ADDR_MAC_SPEED 0
		#define ENABLE 0
#endif

#ifndef C_SUB_ADDR_MDIO_INTERRUPT_EN
		#define C_SUB_ADDR_MDIO_INTERRUPT_EN 0
		#define ENABLE 0
#endif



#ifndef ENABLE
	#define ENABLE 1
#endif


#define INR_MDIO_read_addr ((C_SUB_ADDR_MDIO_READ<<0)|(C_BASE_ADDR_MDIO<<8))
#define INR_MDIO_write_addr ((C_SUB_ADDR_MDIO_WRITE<<0)|(C_BASE_ADDR_MDIO<<8))
#ifdef C_SUB_ADDR_MDIO_INTERRUPTS //backwardcompatibility
#define INR_MDIO_interrupt ((C_SUB_ADDR_MDIO_INTERRUPTS<<0)|(C_BASE_ADDR_MDIO<<8))
#else
#define INR_MDIO_interrupt ((C_SUB_ADDR_MDIO_INTERRUPT<<0)|(C_BASE_ADDR_MDIO<<8))
#endif

#define INR_MDIO_GPHY_REG_ISTAT 0x1A
#define INR_MDIO_GPHY_REG_IMASK 0x19
#define INR_MDIO_ALASKA_PHY_REG_ISTAT 19
#define INR_MDIO_ALASKA_PHY_REG_IMASK 18
#define INR_MDIO_timeout 10
#define INR_MDIO_GPHY_REG_MIISTAT 0x18
#define INR_MDIO_ALASKA_REG_MIISTAT 17

#ifdef C_BASE_ADDR_HC_0
	#define INR_HC_BASE(id) (C_BASE_ADDR_HC_ ## id )
	#define INR_HC_INTERRUPT(id) (INR_HC_BASE(id)<<8)+C_SUB_ADDR_HC_INTERRUPT
	#define INR_HC_INTERRUPT_EN(id) (INR_HC_BASE(id)<<8)+C_SUB_ADDR_HC_INTERRUPT_EN

#endif
#ifndef C_BASE_ADDR_NET_LOWER
	#define INR_MAC_BASE(id) (C_BASE_ADDR_MAC_ ## id )
	#define INR_MAC_SPEED(id) (INR_MAC_BASE(id)<<8)+C_SUB_ADDR_MAC_SPEED
#else
	#define INR_MAC_BASE(id) C_BASE_ADDR_NET_LOWER
	#define INR_MAC_SPEED(id) (INR_MAC_BASE(id)<<8)+C_SUB_ADDR_NET_SPEED
#endif

#define INR_MDIO_set_FPGA_speed_to_PHY 0
