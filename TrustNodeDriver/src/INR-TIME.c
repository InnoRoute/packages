/**
*@file
*@brief Functions for Timestamping
*@author M.Ulbricht 2018
**/

#include <linux/kernel.h>
#include <linux/version.h>
#include <linux/module.h>
#include <linux/pci.h>
#include <linux/msi.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/delay.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/netdev_features.h>
#include <linux/kthread.h>
#include <linux/skbuff.h>
#include <linux/unistd.h>
#include <linux/ptp_classify.h>
#include <linux/timecounter.h>
#include "INR-TIME.h"
#include "INR-NW.h"
#include "INR-PCI.h"
//#include "INR-PCI.h"
#include "INR.h"
struct ptp_clock *ptp_clock;
struct ptp_clock_info ptp_caps;
struct timecounter tc;
struct cyclecounter cc;
struct INR_TIME_TX_entry INR_TIME_vortex[INR_TIME_vortex_length]; //no, its not bigger on the inside :D
uint16_t INR_TIME_TX_vortex_current=1;
uint32_t CTRLD_rate=0x5000000;
uint64_t CTRLD_offset=0;
uint8_t TIME_DBG_mod=1;
uint8_t INR_TIME_enable=0;
//void *gBaseVirt1 = NULL;

//*****************************************************************************************************************
/**
*set bar1 pointer in time context
*
*/
//void INR_TIME_set_bar1_base(uint64_t *base){
//gBaseVirt1=base;

//}
void INR_TIME_set_enable(uint8_t enable){
INR_TIME_enable=enable;
}
//*****************************************************************************************************************
/**
*reads hardwareclock value from hardware and reports to cycle counter
*
*/
static cycle_t INR_TIME_cyclecounter_read(const struct cyclecounter *cc)
{
	uint64_t ns,ns_h,ns_l;
	#ifdef C_BASE_ADDR_RTC
	INR_PCI_BAR1_read_ext((C_BASE_ADDR_RTC<<8)+C_SUB_ADDR_RTC_BRIDGE_LOW);//latch clock value
	ns_l=INR_PCI_BAR1_read_ext((C_BASE_ADDR_RTC<<8)+C_SUB_ADDR_RTC_CTRLD_LOW);
	ns_h=INR_PCI_BAR1_read_ext((C_BASE_ADDR_RTC<<8)+C_SUB_ADDR_RTC_CTRLD_HIGH);
	ns=(ns_h|(ns_h<<32))*INR_TIME_base;
	#endif
	

	return ns;
}
//*****************************************************************************************************************
/**
*adds an TX entry to the wait for timestamp queue
*@brief need to keep the skb until transmit timestamp is reported
*/

uint16_t INR_TIME_TX_add(struct sk_buff *skb) {
if(INR_TIME_enable){
	INR_TIME_TX_vortex_current++;
    if(INR_TIME_TX_vortex_current==0) INR_TIME_TX_vortex_current=1;
    if(INR_TIME_vortex[INR_TIME_TX_vortex_current].used) { //overrun detected, clear entry
        skb_tx_timestamp(INR_TIME_vortex[INR_TIME_TX_vortex_current].skb);
    }
    INR_TIME_vortex[INR_TIME_TX_vortex_current].skb=skb;
    INR_TIME_vortex[INR_TIME_TX_vortex_current].used=1;
    
    if (TIME_DBG_mod)INR_LOG_debug(loglevel_warn"Net TX skb stored in timevortex at position %i\n",INR_TIME_TX_vortex_current);
    return INR_TIME_TX_vortex_current;
}}

//*****************************************************************************************************************
/**
*interrupt from hardware for new TX timestamp available
*@brief
*/

void INR_TIME_TX_transmit_interrupt() {
    uint32_t entry_current=1;
    uint32_t portmap=0;
    uint32_t timestamp=0;
    uint8_t i=0;
    if(INR_TIME_enable){
    //if (TIME_DBG_mod)INR_LOG_debug("TX transmit interrupt\n");
#ifdef C_SUB_ADDR_NET_TX_CONF_VLD
    
    portmap=0xfff&INR_PCI_BAR1_read_ext((C_BASE_ADDR_NET_LOWER<<8)+C_SUB_ADDR_NET_TX_CONF_VLD);
    portmap=0xfff;
    if (TIME_DBG_mod)INR_LOG_debug(loglevel_warn"TX transmit interrupt handle portmap:0x%lx\n",portmap);
    for(i=0;i<32;i++)if(portmap&(1<<i)){
    	while(entry_current!=0){
    	    entry_current=0xffff&INR_PCI_BAR1_read_ext((C_BASE_ADDR_NET_LOWER<<8)+C_SUB_ADDR_NET_TX_CONF_L+(i*2*4));
	    timestamp=INR_PCI_BAR1_read_ext((C_BASE_ADDR_NET_LOWER<<8)+C_SUB_ADDR_NET_TX_CONF_L+((i*2)+1)*4);
	    if(entry_current){
	    if (TIME_DBG_mod)INR_LOG_debug(loglevel_warn"Port: %i TX entry id: %i timestamp:0x%lx\n",i,entry_current,timestamp);
	    if(INR_TIME_vortex[entry_current].used){
		    struct skb_shared_hwtstamps *skbtimestamp = skb_hwtstamps(INR_TIME_vortex[entry_current].skb);
		    skbtimestamp->hwtstamp=ns_to_ktime(timestamp); //0: insert timestamp here...
		    if (TIME_DBG_mod)INR_LOG_debug(loglevel_warn"Write timestamp to skb ns:%lli, ktime:%lli\n",timestamp,skbtimestamp->hwtstamp);
		    skb_tstamp_tx(INR_TIME_vortex[entry_current].skb, &skbtimestamp);
		    dev_kfree_skb_any(INR_TIME_vortex[entry_current].skb);
		    INR_TIME_vortex[entry_current].used=0;
    		}else INR_LOG_debug("error: TXtime got point to empty timevortex.\n");
    }}}
   
#endif
   



}}

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
void INR_TIME_init_ptp_clock(struct pci_dev *dev) {
    snprintf(ptp_caps.name, 16, "%s", "TrustNode");
    ptp_caps.owner = THIS_MODULE;
    ptp_caps.max_adj = 250000000;
    ptp_caps.n_alarm = 0;
    ptp_caps.n_ext_ts = 0;
    ptp_caps.n_per_out = 0;
    ptp_caps.pps = 1;
    ptp_caps.adjfreq = INR_TIME_ptp_adjfreq;
    ptp_caps.adjtime = INR_TIME_ptp_adjtime;
    ptp_caps.gettime64 = INR_TIME_ptp_gettime;
    ptp_caps.settime64 = INR_TIME_ptp_settime;
    ptp_caps.enable = INR_TIME_ptp_enable;

    if (get_nwdev(0)) ptp_clock = ptp_clock_register(&ptp_caps,&dev->dev); //register always on TN0
    if (IS_ERR(ptp_clock)) {
        ptp_clock = NULL;
        INR_LOG_debug (loglevel_err"ptp_clock_register failed\n");
    } else
        INR_LOG_debug (loglevel_info"registered PHC device on %s\n", get_nwdev(0)->name);
	memset(&cc, 0, sizeof(cc));
	
	cc.read = INR_TIME_cyclecounter_read;
	cc.mask = CYCLECOUNTER_MASK(64);
	cc.shift = 0;//innoroute counter runns in ns
	cc.mult = 1;
	timecounter_init(&tc,&cc,0);
}
static int INR_TIME_ptp_adjfreq(struct ptp_clock_info *ptp, s32 ppb) {
#ifdef C_BASE_ADDR_RTC
CTRLD_rate+=ppb;
INR_PCI_BAR1_write_ext(CTRLD_rate,(C_BASE_ADDR_RTC<<8)+C_SUB_ADDR_RTC_CTRLD_RATE);
if (TIME_DBG_mod)INR_LOG_debug("PTP adjfreq called new rate:%lli ppb:%lli\n",CTRLD_rate, ppb);
return 0;
#endif
    return 1;
}
static int INR_TIME_ptp_adjtime(struct ptp_clock_info *ptp, s64 delta) {
//this is maybe not neccesarry...
#ifdef C_BASE_ADDR_RTC
CTRLD_offset+=delta;
INR_PCI_BAR1_write_ext(CTRLD_offset&0xffffffff,(C_BASE_ADDR_RTC<<8)+C_SUB_ADDR_RTC_CTRLD_OFFSET_LOW);
INR_PCI_BAR1_write_ext((CTRLD_offset>>32)&0xffffffff,(C_BASE_ADDR_RTC<<8)+C_SUB_ADDR_RTC_CTRLD_OFFSET_HIGH);
if (TIME_DBG_mod)INR_LOG_debug("PTP adjtime called new offset:%lli delta:%lli\n",CTRLD_offset, delta);
#endif
//endo of maybe not neccesarry


timecounter_adjtime(&tc,delta);
    return 0;
}
static int INR_TIME_ptp_gettime(struct ptp_clock_info *ptp,struct timespec64 *ts) {
	uint64_t ns=0,ns_l=0,ns_h=0;
	ns=timecounter_read(&tc);
	
	*ts = ns_to_timespec64(ns);
	if (TIME_DBG_mod)INR_LOG_debug("PTP get time called value:%lli\n",ns);
    return 0;
}
static int INR_TIME_ptp_settime(struct ptp_clock_info *ptp, const struct timespec64 *ts) {
	if (TIME_DBG_mod)INR_LOG_debug("PTP set time called vaule:%lli\n",timespec64_to_ns(ts));

    timecounter_init(&tc,&cc,timespec64_to_ns(ts));
    return 0;
}
static int INR_TIME_ptp_enable(struct ptp_clock_info *ptp,struct ptp_clock_request *rq, int on) {

   return 0;
}

uint32_t INR_TIME_correct_HW_timestamp(uint32_t hw_value){
int64_t offset=0;
uint64_t BRIDGE_clock_value=0,CTRLD_clock_value=0;
uint32_t BRIDGE_clock_value_L=0,CTRLD_clock_value_L=0,BRIDGE_clock_value_H=0,CTRLD_clock_value_H=0;
#ifdef C_BASE_ADDR_RTC
BRIDGE_clock_value_L=INR_PCI_BAR1_read_ext((C_BASE_ADDR_RTC<<8)+C_SUB_ADDR_RTC_BRIDGE_LOW);
BRIDGE_clock_value_H=INR_PCI_BAR1_read_ext((C_BASE_ADDR_RTC<<8)+C_SUB_ADDR_RTC_BRIDGE_HIGH);
CTRLD_clock_value_L=INR_PCI_BAR1_read_ext((C_BASE_ADDR_RTC<<8)+C_SUB_ADDR_RTC_CTRLD_LOW);
CTRLD_clock_value_H=INR_PCI_BAR1_read_ext((C_BASE_ADDR_RTC<<8)+C_SUB_ADDR_RTC_CTRLD_HIGH);
#endif
BRIDGE_clock_value=BRIDGE_clock_value_L|((uint64_t)BRIDGE_clock_value_H<<32);
CTRLD_clock_value=CTRLD_clock_value_L|((uint64_t)CTRLD_clock_value_H<<32);
offset=BRIDGE_clock_value-CTRLD_clock_value;
if (TIME_DBG_mod)INR_LOG_debug("TIME adjust value..CTRLD_clock:%lli BRIDGE_clock:%lli offset:%lli pkt_value:%lli\n",CTRLD_clock_value,BRIDGE_clock_value,offset,hw_value);
return hw_value+abs(offset);
}

