/**
*@file
*@brief Functions for Timestamping
*@author M.Ulbricht 2018
**/

#include <linux/ptp_clock_kernel.h>
#include"tn_env.h"

struct INR_TIME_timestamps
{
	uint64_t bridge;
	uint64_t controlled;
};
uint16_t INR_TIME_TX_add(struct sk_buff *skb);
struct ptp_clock* INR_TIME_get_ptp_clock(void);
void INR_TIME_init_ptp_clock(struct pci_dev *dev);
static int INR_TIME_ptp_adjfreq(struct ptp_clock_info *ptp, s32 ppb);
static int INR_TIME_ptp_adjtime(struct ptp_clock_info *ptp, s64 delta);
static int INR_TIME_ptp_gettime(struct ptp_clock_info *ptp, struct timespec64 *ts);
static int INR_TIME_ptp_settime(struct ptp_clock_info *ptp, const struct timespec64 *ts);
static int INR_TIME_ptp_enable(struct ptp_clock_info *ptp,struct ptp_clock_request *rq, int on);
void INR_TIME_remove_ptp_clock(void);
void INR_TIME_TX_transmit_interrupt(void);
void INR_TIME_set_bar1_base(uint64_t *base);
void INR_TIME_correct_HW_timestamp(uint32_t hw_value,struct INR_TIME_timestamps *ts);
void INR_TIME_set_enable(uint8_t enable);
void INR_TIME_set_debug(uint8_t enable);
void INR_TIME_set_USE_ctrl_bridge_clock_offset(uint8_t enable);
void INR_TIME_reset(void);
uint8_t get_clockjump(void);

#define INR_TIME_vortex_length 65536
#define INR_TIME_base 1
#define INR_TIME_MAX_pollcount 50
#define MAX_TIME_TX_vortex_queue 2
extern void INR_MMI_common_interrupt_handler(uint32_t status);
void set_timedif(uint64_t diff);

struct INR_TIME_TX_entry
{
    struct sk_buff *skb; /**<store skb*/
    uint8_t used;      /**<valid flag*/
    uint32_t timestamp;
};


