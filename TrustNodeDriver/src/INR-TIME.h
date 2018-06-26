/**
*@file
*@brief Functions for Timestamping
*@author M.Ulbricht 2018
**/
#include <linux/ptp_clock_kernel.h>
#include<tn_env.h>
uint16_t INR_TIME_TX_add(struct sk_buff *skb);
struct ptp_clock* INR_TIME_get_ptp_clock(void);
void INR_TIME_init_ptp_clock(struct pci_dev *dev);
static int INR_TIME_ptp_adjfreq(struct ptp_clock_info *ptp, s32 ppb);
static int INR_TIME_ptp_adjtime(struct ptp_clock_info *ptp, s64 delta);
static int INR_TIME_ptp_gettime(struct ptp_clock_info *ptp, struct timespec64 *ts);
static int INR_TIME_ptp_settime(struct ptp_clock_info *ptp, const struct timespec64 *ts);
static int INR_TIME_ptp_enable(struct ptp_clock_info *ptp,struct ptp_clock_request *rq, int on);
void INR_TIME_TX_transmit_interrupt(void);
void INR_TIME_set_bar1_base(uint64_t *base);
uint32_t INR_TIME_correct_HW_timestamp(uint32_t hw_value);
void INR_TIME_set_enable(uint8_t enable);

#define INR_TIME_vortex_length 65536
#define INR_TIME_base 1
#define INR_TIME_MAX_pollcount 50

struct INR_TIME_TX_entry
{
    struct sk_buff *skb; /**<store skb*/
    uint8_t used;      /**<valid flag*/
    uint32_t timestamp;
};
