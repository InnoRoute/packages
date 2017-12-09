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

