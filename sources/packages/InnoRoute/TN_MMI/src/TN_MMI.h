#include "tn_env.h"
//#include <linux/inttypes.h>

void INR_MMI_interrupt_handler(void);
void INR_MMI_init(uint64_t bar1);
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
#define INR_MMI_common_interrupt_mask ((C_BASE_ADDR_COMMON_LOWER<<8)|C_SUB_ADDR_COMMON_INTERRUPT_EN)
#define INR_MMI_common_interrupt_status ((C_BASE_ADDR_COMMON_LOWER<<8)|C_SUB_ADDR_COMMON_INTERRUPT)
extern void INR_TIME_TX_transmit_interrupt(void);
extern void INR_NW_carrier_update(uint8_t id, uint16_t status);
int INR_MMI_phy_state_watch(void *nix);
void INR_MMI_phy_state_watch_start(void);
void INR_MMI_phy_state_watch_wakeup(void);
void INR_MMI_common_interrupt_handler(uint32_t status);
void INR_MMI_common_interrupt_reset(void);
uint32_t INR_MMI_common_interrupt_get(void);
void INR_MMI_can_interrupt_handler(uint32_t mask);
static int can_int_init(void);
static void  can_int_exit(void);
static void can_int_release(struct device *dev);


#define INR_MAX_POLL 200
