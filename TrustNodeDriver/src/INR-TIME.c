/**
*@file
*@brief Functions for Timestamping
*@author M.Ulbricht 2018
**/

#include <linux/skbuff.h>
#include <linux/ptp_clock_kernel.h>
#include "INR-TIME.h"
#include "INR-NW.h"
#include "INR.h"
struct ptp_clock *ptp_clock;
struct ptp_clock_info ptp_caps;
struct INR_TIME_TX_entry INR_TIME_vortex[INR_TIME_vortex_length]; //no, its not bigger on the inside :D
uint16_t INR_TIME_TX_vortex_current=1;

//*****************************************************************************************************************
/**
*adds an TX entry to the wait for timestamp queue
*@brief need to keep the skb until transmit timestamp is reported
*/

uint16_t INR_TIME_TX_add(struct sk_buff *skb) {
    if(INR_TIME_vortex[INR_TIME_TX_vortex_current].used) { //overrun detected, clear entry
        kfree_skb(INR_TIME_vortex[INR_TIME_TX_vortex_current].skb);
    }

    INR_TIME_TX_vortex_current++;
    if(INR_TIME_TX_vortex_current==0) INR_TIME_TX_vortex_current=1;
    return INR_TIME_TX_vortex_current;
}

//*****************************************************************************************************************
/**
*interrupt from hardware for new TX timestamp available
*@brief
*/

void INR_TIME_TX_transmit_interrupt() {
    uint16_t entry_current=0;

    //skb_hwtstamp_tx(data_tx[INR_PCI_tx_descriptor_current].skb,ns_to_ktime(0),INR_PCI_tx_descriptor_current].nwdev);//0: insert timestamp here
    struct skb_shared_hwtstamps *skbtimestamp = skb_hwtstamps(INR_TIME_vortex[entry_current].skb);
    skbtimestamp->hwtstamp=ns_to_ktime(0); //0: insert timestamp here...



}

//*****************************************************************************************************************
/**
*return ptp clock device information
*@brief
*/
struct ptp_clock* INR_TIME_get_ptp_clock() {
    return ptp_clock;
}
//*****************************************************************************************************************
/**
*init ptp clock device
*@brief
*/
void INR_TIME_init_ptp_clock() {

    snprintf(ptp_caps.name, 16, "%s", "TrustNode");
    ptp_caps.owner = THIS_MODULE;
    ptp_caps.max_adj = 250000000;
    ptp_caps.n_alarm = 0;
    ptp_caps.n_ext_ts = 0;
    ptp_caps.n_per_out = 0;
    ptp_caps.pps = 1;
    ptp_caps.adjfreq = INR_TIME_ptp_adjfreq;
    ptp_caps.adjtime = INR_TIME_ptp_adjtime;
    //ptp_caps.gettime = INR_TIME_ptp_gettime;
    //ptp_caps.settime = INR_TIME_ptp_settime;
    ptp_caps.enable = INR_TIME_ptp_enable;

    ptp_clock = ptp_clock_register(&ptp_caps,get_nwdev(0)); //register always on TN0
    if (IS_ERR(ptp_clock)) {
        ptp_clock = NULL;
        INR_LOG_debug (loglevel_err"ptp_clock_register failed\n");
    } else
        INR_LOG_debug (loglevel_info"registered PHC device on %s\n", get_nwdev(0)->name);

}
static int INR_TIME_ptp_adjfreq(struct ptp_clock_info *ptp, s32 ppb) {

    return 0;
}
static int INR_TIME_ptp_adjtime(struct ptp_clock_info *ptp, s64 delta) {

    return 0;
}
static int INR_TIME_ptp_gettime(struct ptp_clock_info *ptp, struct timespec *ts) {

    return 0;
}
static int INR_TIME_ptp_settime(struct ptp_clock_info *ptp,const struct timespec *ts) {

    return 0;
}
static int INR_TIME_ptp_enable(struct ptp_clock_info *ptp,struct ptp_clock_request *rq, int on) {

    return 0;
}

