#include "tn_env.h"
//#include <linux/inttypes.h>
void INR_MMI_interrupt_handler(void);
void INR_MMI_init(uint64_t *bar1);
void INR_MMI_exit(void);
void INR_MMI_PHY_interrupt(uint16_t id);
void INR_MMI_ALASKA_PHY_PTP_interrupt(uint8_t id);
void INR_MMI_PHY_hard_reset(void);
void INR_MMI_GPHY_soft_reset(void);
void INR_PCI_MMI_write(uint32_t value, uint64_t addr);
uint32_t INR_PCI_MMI_read(uint64_t addr);
#define INR_PCI_BAR1_read(addr)		readl(gBaseVirt1_MMI+addr)	/*<read from bar1 */
#define INR_PCI_BAR1_write(content, addr)	writel(content, (gBaseVirt1_MMI + addr))	/*<write to bar1 */
#define INR_MMI_interrupt_status ((C_BASE_ADDR_BM<<8)|C_SUB_ADDR_BM_MMI_INTERRUPT)
#define INR_MMI_interrupt_mask ((C_BASE_ADDR_MDIO<<8)|C_SUB_ADDR_MDIO_INTERRUPT_EN)
extern void INR_TIME_TX_transmit_interrupt(void);

#define INR_MAX_POLL 100
