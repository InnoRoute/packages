#include "tn_env.h"
void INR_MDIO_init(uint64_t bar1base);
void INR_MDIO_write(uint8_t PHY_addr, uint8_t REG_addr, uint16_t data);
void INR_MDIO_write_b(uint8_t write, uint8_t PHY_addr, uint8_t REG_addr, uint16_t data);
uint16_t INR_MDIO_read(uint8_t PHY_addr, uint8_t REG_addr);
uint8_t INR_MDIO_read_b(uint16_t *data);
void INR_MDIO_GPHY_init(uint8_t id);
void INR_MDIO_ALASKA_init(uint8_t id);


#define INR_MDIO_read_addr ((C_SUB_ADDR_MDIO_READ<<8)|C_BASE_ADDR_MDIO)
#define INR_MDIO_write_addr ((C_SUB_ADDR_MDIO_WRITE<<8)|C_BASE_ADDR_MDIO)
#define INR_MDIO_interrupt ((C_SUB_ADDR_MDIO_INTERRUPTS<<8)|C_BASE_ADDR_MDIO)

#define INR_MDIO_GPHY_REG_ISTAT 0x1A
#define INR_MDIO_ALASKA_PHY_REG_ISTAT 19
#define INR_MDIO_timeout 10
