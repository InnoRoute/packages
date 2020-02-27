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
#include <uapi/linux/net_tstamp.h>
#include "INR-TIME.h"
#include "INR-NW.h"
#include "INR-PCI.h"
//#include "INR-PCI.h"
#include "INR.h"

struct ptp_clock *ptp_clock;
struct ptp_clock_info ptp_caps;
//struct timecounter tc;
//struct cyclecounter cc;
struct INR_TIME_TX_entry INR_TIME_vortex[INR_TIME_vortex_length]; //no, its not bigger on the inside :D
uint16_t INR_TIME_TX_vortex_current=1;

volatile uint16_t INR_TIME_TX_vortex_lastread=1;
uint64_t CTRLD_rate=0x5000000;
uint64_t CTRLD_offset=24;
uint8_t TIME_DBG_mod=0;
uint8_t INR_TIME_enable=0;
uint8_t pollcount=0;
DEFINE_SPINLOCK(hardwareLock);
unsigned long flags;
volatile uint8_t clock_registred=0;
uint8_t USE_ctrl_bridge_clock_offset=1;
void (*MMI_common_interrupt_handler)(uint32_t status);
uint64_t timediff=1;
uint8_t clockjump=0;
DEFINE_SEMAPHORE (INR_TIME_TX_transmit_interrupt_sem);

//*****************************************************************************************************************
/**
*clear on read function to get information about clock inconsistency
*
*/
uint8_t get_clockjump(){
	if(clockjump){
		clockjump=0;
		return 1;
	} else return 0;


}
void set_timedif(uint64_t diff){// currently not used

timediff=diff;
}
//void *gBaseVirt1 = NULL;

//*****************************************************************************************************************
/**
*set bar1 pointer in time context
*
*/
//void INR_TIME_set_bar1_base(uint64_t *base){
//gBaseVirt1=base;

//}
void INR_TIME_set_USE_ctrl_bridge_clock_offset(uint8_t enable) {
    USE_ctrl_bridge_clock_offset=enable;
}
void INR_TIME_set_enable(uint8_t enable) {
    INR_TIME_enable=enable;
}
void INR_TIME_set_debug(uint8_t enable) {
    TIME_DBG_mod=enable;
}
//*****************************************************************************************************************
/**
*resets all variables in case of driver reload
*@brief prevents TX_conf_error interrupt to be calles instantly after driver reload
*/
void INR_TIME_reset(){
	INR_TIME_TX_vortex_current=1;
	INR_TIME_TX_vortex_lastread=1;


}
//*****************************************************************************************************************
/**
*reads hardwareclock value from hardware and reports to cycle counter
*
*/
//static cycle_t INR_TIME_cyclecounter_read(const struct cyclecounter *cc)
//{
    /*    uint64_t ns,ns_h,ns_l;
    #ifdef C_BASE_ADDR_RTC
        INR_PCI_BAR1_read_ext((C_BASE_ADDR_RTC<<8)+C_SUB_ADDR_RTC_BRIDGE_LOW);//latch clock value
        ns_l=INR_PCI_BAR1_read_ext((C_BASE_ADDR_RTC<<8)+C_SUB_ADDR_RTC_CTRLD_LOW);
        ns_h=INR_PCI_BAR1_read_ext((C_BASE_ADDR_RTC<<8)+C_SUB_ADDR_RTC_CTRLD_HIGH);
        ns=(ns_h|(ns_h<<32))*INR_TIME_base;
    #endif
    */
    /*
    int64_t offset=0;
    uint64_t BRIDGE_clock_value=0,CTRLD_clock_value=0;
    uint32_t BRIDGE_clock_value_L=0,CTRLD_clock_value_L=0,BRIDGE_clock_value_H=0,CTRLD_clock_value_H=0;
#ifdef C_BASE_ADDR_RTC
    spin_lock_irqsave(&hardwareLock, flags);
    BRIDGE_clock_value_L=INR_PCI_BAR1_read_ext((C_BASE_ADDR_RTC<<8)+C_SUB_ADDR_RTC_BRIDGE_LOW);
    BRIDGE_clock_value_H=INR_PCI_BAR1_read_ext((C_BASE_ADDR_RTC<<8)+C_SUB_ADDR_RTC_BRIDGE_HIGH);
    CTRLD_clock_value_L=INR_PCI_BAR1_read_ext((C_BASE_ADDR_RTC<<8)+C_SUB_ADDR_RTC_CTRLD_LOW);
    CTRLD_clock_value_H=INR_PCI_BAR1_read_ext((C_BASE_ADDR_RTC<<8)+C_SUB_ADDR_RTC_CTRLD_HIGH);
    spin_unlock_irqrestore(&hardwareLock, flags);
#endif
    BRIDGE_clock_value=BRIDGE_clock_value_L|((uint64_t)BRIDGE_clock_value_H<<32);

    return BRIDGE_clock_value;
}*/
//*****************************************************************************************************************
/**
*adds an TX entry to the wait for timestamp queue
*@brief need to keep the skb until transmit timestamp is reported
*/

uint16_t INR_TIME_TX_add(struct sk_buff *skb) {
uint16_t waiting_queue_length=0;
if(MMI_common_interrupt_handler){
  
	}else{
		MMI_common_interrupt_handler= symbol_get(INR_MMI_common_interrupt_handler);
    
}
    if(INR_TIME_enable) {
        INR_TIME_TX_vortex_current++;
        if(INR_TIME_TX_vortex_current==0) INR_TIME_TX_vortex_current=1;
        if(INR_TIME_vortex[INR_TIME_TX_vortex_current].used) { //overrun detected, clear entry
            if(INR_TIME_vortex[INR_TIME_TX_vortex_current].skb)skb_tx_timestamp(INR_TIME_vortex[INR_TIME_TX_vortex_current].skb);
        }
        INR_TIME_vortex[INR_TIME_TX_vortex_current].skb=skb;
        INR_TIME_vortex[INR_TIME_TX_vortex_current].used=1;
				if(INR_TIME_TX_vortex_current<INR_TIME_TX_vortex_lastread){
					waiting_queue_length=65536-INR_TIME_TX_vortex_lastread-INR_TIME_TX_vortex_current;
					}else{
					waiting_queue_length=INR_TIME_TX_vortex_current-INR_TIME_TX_vortex_lastread;
					}
				if(waiting_queue_length>MAX_TIME_TX_vortex_queue)if(MMI_common_interrupt_handler)MMI_common_interrupt_handler(0xffff);// if waiting for to much interrupts, call common fpga fail interrupt
        if (TIME_DBG_mod)INR_LOG_debug(loglevel_warn"Net TX skb stored in timevortex at position %i waiting for %i\n",INR_TIME_TX_vortex_current,waiting_queue_length);
        
        return INR_TIME_TX_vortex_current;
    }
}

//*****************************************************************************************************************
/**
*interrupt from hardware for new TX timestamp available
*@brief
*/

void INR_TIME_TX_transmit_interrupt() {

		while(down_trylock(&INR_TIME_TX_transmit_interrupt_sem)); // prevent raceconditions
    uint32_t entry_current=1, tmp=0;
    uint32_t portmap=0;
    u64 timestamp=0;
    u64 timestamp2=0;
    uint8_t i=0;
    if(INR_TIME_enable) {
        //if (TIME_DBG_mod)INR_LOG_debug("TX transmit interrupt\n");
#ifdef C_SUB_ADDR_NET_TX_CONF_VLD
        portmap=0xfff&INR_PCI_BAR1_read_ext((C_BASE_ADDR_NET_LOWER<<8)+C_SUB_ADDR_NET_TX_CONF_VLD);
        uint8_t pollcount2=0;
        while(portmap) {
            pollcount2++;
            if(pollcount2>=INR_TIME_MAX_pollcount) {
                //INR_TIME_enable=0;
                INR_LOG_debug(loglevel_warn"time interrupt max pollcount reached, C_SUB_ADDR_NET_TX_CONF_VLD:%x\n",portmap);
                break;
            }
            //portmap=0xfff;
            if (TIME_DBG_mod)INR_LOG_debug(loglevel_warn"TX transmit interrupt handle portmap:0x%lx\n",portmap);
            for(i=0; i<32; i++)if(portmap&(1<<i)) {

                    pollcount=0;
                    while(entry_current!=0) {
                    		INR_TIME_TX_vortex_lastread=entry_current;
                        pollcount++;
                        if(pollcount>=INR_TIME_MAX_pollcount) {
                            //INR_TIME_enable=0;
                            INR_LOG_debug(loglevel_warn"time interrupt max pollcount reached, entry_current:%x\n",entry_current);
                            break;
                        }
                        tmp=INR_PCI_BAR1_read_ext((C_BASE_ADDR_NET_LOWER<<8)+C_SUB_ADDR_NET_TX_CONF_L+(i*2*4));
                        if(tmp!=0xeeeeeeee)entry_current=0xffff&tmp;else entry_current=0; 
                        timestamp=INR_PCI_BAR1_read_ext((C_BASE_ADDR_NET_LOWER<<8)+C_SUB_ADDR_NET_TX_CONF_L+((i*2)+1)*4);
                        if(entry_current) {
                        
                            if (TIME_DBG_mod)INR_LOG_debug(loglevel_warn"Port: %i TX entry id: %i timestamp:0x%lx\n",i,entry_current,timestamp);
                            if(INR_TIME_vortex[entry_current].used) {
                                struct skb_shared_hwtstamps *skbtimestamp = skb_hwtstamps(INR_TIME_vortex[entry_current].skb);
                                memset(skbtimestamp, 0, sizeof(struct skb_shared_hwtstamps));
                                struct INR_TIME_timestamps ts;
                                INR_TIME_correct_HW_timestamp(timestamp,&ts);
                                timestamp=ts.controlled;
                                timestamp2=ts.bridge;
                                skbtimestamp->hwtstamp=ns_to_ktime(timestamp); //0: insert timestamp here...
#if LINUX_VERSION_CODE < KERNEL_VERSION(5,0,0) 
                                skbtimestamp->hwtstamp2=ns_to_ktime(timestamp2);
#endif
                                if (TIME_DBG_mod)INR_LOG_debug(loglevel_warn"Write timestamp to skb ns:%lli, ktime:%lli\n",timestamp,skbtimestamp->hwtstamp);
                                skb_tstamp_tx(INR_TIME_vortex[entry_current].skb, skbtimestamp);
                                dev_kfree_skb_any(INR_TIME_vortex[entry_current].skb);
                                INR_TIME_vortex[entry_current].used=0;
                            } else INR_LOG_debug("error: TXtime got point to empty timevortex.\n");
                        }
                    }
                }
            portmap=0xfff&INR_PCI_BAR1_read_ext((C_BASE_ADDR_NET_LOWER<<8)+C_SUB_ADDR_NET_TX_CONF_VLD);
        }

#endif




    }

up(&INR_TIME_TX_transmit_interrupt_sem);
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
*del ptp clock device
*@brief
*/
void INR_TIME_remove_ptp_clock(){

if(clock_registred)ptp_clock_unregister(ptp_clock);
clock_registred=0;


}
//*****************************************************************************************************************
/**
*init ptp clock device
*@brief
*/
void INR_TIME_init_ptp_clock(struct pci_dev *dev) {
if(clock_registred==0){
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
        return ;
    } else
        INR_LOG_debug (loglevel_info"registered PHC device on %s\n", get_nwdev(0)->name);
        clock_registred=1;
    //memset(&cc, 0, sizeof(cc));
    uint64_t BRIDGE_clock_value=0,CTRLD_clock_value=0;
    uint32_t BRIDGE_clock_value_L=0,CTRLD_clock_value_L=0,BRIDGE_clock_value_H=0,CTRLD_clock_value_H=0;
#ifdef C_BASE_ADDR_RTC
    spin_lock_irqsave(&hardwareLock, flags);
    BRIDGE_clock_value_L=INR_PCI_BAR1_read_ext((C_BASE_ADDR_RTC<<8)+C_SUB_ADDR_RTC_BRIDGE_LOW);
    BRIDGE_clock_value_H=INR_PCI_BAR1_read_ext((C_BASE_ADDR_RTC<<8)+C_SUB_ADDR_RTC_BRIDGE_HIGH);
    CTRLD_clock_value_L=INR_PCI_BAR1_read_ext((C_BASE_ADDR_RTC<<8)+C_SUB_ADDR_RTC_CTRLD_LOW);
    CTRLD_clock_value_H=INR_PCI_BAR1_read_ext((C_BASE_ADDR_RTC<<8)+C_SUB_ADDR_RTC_CTRLD_HIGH);
    spin_unlock_irqrestore(&hardwareLock, flags);
#endif
    BRIDGE_clock_value=BRIDGE_clock_value_L|((uint64_t)BRIDGE_clock_value_H<<32);
    CTRLD_clock_value=CTRLD_clock_value_L|((uint64_t)CTRLD_clock_value_H<<32);
    //cc.read = INR_TIME_cyclecounter_read;
    //cc.mask = CYCLECOUNTER_MASK(64);//trustnode count ns in 32 bit
    //cc.shift = 0;//innoroute counter runns in ns
    //cc.mult = 1;
    //timecounter_init(&tc,&cc,CTRLD_clock_value);
    INR_TIME_ptp_adjtime(NULL,0);//synchonize controlled and freeruning clock
}

uint64_t time_granularity=5;
#ifdef C_SUB_ADDR_TM_SCHED_TAS_TICK_GRANULARITY
	time_granularity=INR_PCI_BAR1_read_ext((C_BASE_ADDR_TM_SCHED_LOWER<<8)+C_SUB_ADDR_TM_SCHED_TAS_TICK_GRANULARITY);
#endif
	if(time_granularity>100){
		INR_LOG_debug("error: Strange value %i for TAS time granularity, will be reset to 5ns",time_granularity);
		time_granularity=5; //plausicheck		
	}
CTRLD_rate=(time_granularity<<24);
INR_LOG_debug("Set Clock CTRL rate to 0x%llx",CTRLD_rate);

}
static int INR_TIME_ptp_adjfreq(struct ptp_clock_info *ptp, s32 ppb) {
//ppb is from base frequency
    uint64_t freq=1;

    uint32_t incval=CTRLD_rate;
    uint32_t diff=0;
    int neg_adj = 0;

    if (ppb < 0) {
        neg_adj = 1;
        ppb = -ppb;
    }
#ifdef C_BASE_ADDR_RTC
    //CTRLD_rate+=ppb;
    freq*=ppb;
    freq=freq<<1;
    diff=(div_u64(freq, 1000000000ULL)<<24)|((freq%1000000000ULL)>>3);       
    incval = neg_adj ? (incval - diff) : (incval + diff);
    //incval+=3656;
    INR_PCI_BAR1_write_ext(incval,(C_BASE_ADDR_RTC<<8)+C_SUB_ADDR_RTC_CTRLD_RATE);
    if (TIME_DBG_mod)
        INR_LOG_debug("PTP adjfreq called new rate:0x%llx, diff:%lli ppb:%lli neg:%i\n",incval,diff, ppb,neg_adj);
    return 0;
#endif
    return 1;
}
static int INR_TIME_ptp_adjtime(struct ptp_clock_info *ptp, s64 delta) {
//this is maybe not neccesarry...
#ifdef C_BASE_ADDR_RTC
    CTRLD_offset+=delta;
    clockjump=1;
    spin_lock_irqsave(&hardwareLock, flags);
    INR_PCI_BAR1_write_ext(CTRLD_offset&0xffffffff,(C_BASE_ADDR_RTC<<8)+C_SUB_ADDR_RTC_CTRLD_OFFSET_LOW);
    INR_PCI_BAR1_write_ext((CTRLD_offset>>32)&0xffffffff,(C_BASE_ADDR_RTC<<8)+C_SUB_ADDR_RTC_CTRLD_OFFSET_HIGH);
    INR_PCI_BAR1_write_ext(1,(C_BASE_ADDR_RTC<<8)+  C_SUB_ADDR_RTC_CLKSEL);
    spin_unlock_irqrestore(&hardwareLock, flags);
    if (TIME_DBG_mod)
        INR_LOG_debug("PTP adjtime called new offset:%lli delta:%lli\n",CTRLD_offset, delta);
#endif
//endo of maybe not neccesarry


    //timecounter_adjtime(&tc,delta);
    return 0;
}
static int INR_TIME_ptp_gettime(struct ptp_clock_info *ptp,struct timespec64 *ts) {
    uint64_t ns=0,ns_l=0,ns_h=0;

    int64_t offset=0;
    uint64_t BRIDGE_clock_value=0,CTRLD_clock_value=0;
    uint32_t BRIDGE_clock_value_L=0,CTRLD_clock_value_L=0,BRIDGE_clock_value_H=0,CTRLD_clock_value_H=0;
#ifdef C_BASE_ADDR_RTC
    spin_lock_irqsave(&hardwareLock, flags);
    BRIDGE_clock_value_L=INR_PCI_BAR1_read_ext((C_BASE_ADDR_RTC<<8)+C_SUB_ADDR_RTC_BRIDGE_LOW);
    BRIDGE_clock_value_H=INR_PCI_BAR1_read_ext((C_BASE_ADDR_RTC<<8)+C_SUB_ADDR_RTC_BRIDGE_HIGH);
    CTRLD_clock_value_L=INR_PCI_BAR1_read_ext((C_BASE_ADDR_RTC<<8)+C_SUB_ADDR_RTC_CTRLD_LOW);
    CTRLD_clock_value_H=INR_PCI_BAR1_read_ext((C_BASE_ADDR_RTC<<8)+C_SUB_ADDR_RTC_CTRLD_HIGH);
    spin_unlock_irqrestore(&hardwareLock, flags);
#endif
    BRIDGE_clock_value=BRIDGE_clock_value_L|((uint64_t)BRIDGE_clock_value_H<<32);
    CTRLD_clock_value=CTRLD_clock_value_L|((uint64_t)CTRLD_clock_value_H<<32);

    //ns=timecounter_read(&tc);
    ns=CTRLD_clock_value;
    *ts = ns_to_timespec64(ns);
    //if (TIME_DBG_mod)
    INR_LOG_debug("PTP get time called value:%lli\n",ns);
    return 0;
}
static int INR_TIME_ptp_settime(struct ptp_clock_info *ptp, const struct timespec64 *ts) {
    //if (TIME_DBG_mod)
    INR_LOG_debug("PTP set time called vaule:%lli\n",timespec64_to_ns(ts));
		uint64_t newvalue;
    #ifdef C_BASE_ADDR_RTC
     
    
    newvalue= timespec64_to_ns(ts);
    spin_lock_irqsave(&hardwareLock, flags);
    INR_PCI_BAR1_write_ext(newvalue&0xffffffff,(C_BASE_ADDR_RTC<<8)+C_SUB_ADDR_RTC_CTRLD_OFFSET_LOW);
    INR_PCI_BAR1_write_ext((newvalue>>32)&0xffffffff,(C_BASE_ADDR_RTC<<8)+C_SUB_ADDR_RTC_CTRLD_OFFSET_HIGH);
    INR_PCI_BAR1_write_ext(1,(C_BASE_ADDR_RTC<<8)+  C_SUB_ADDR_RTC_CLKSEL);
    spin_unlock_irqrestore(&hardwareLock, flags);
    if (TIME_DBG_mod)
        INR_LOG_debug("PTP adjtime called new value:%lli\n",newvalue);
#endif
    return 0;
}
static int INR_TIME_ptp_enable(struct ptp_clock_info *ptp,struct ptp_clock_request *rq, int on) {

    return 0;
}

void INR_TIME_correct_HW_timestamp(uint32_t hw_value,struct INR_TIME_timestamps *ts) {
    uint64_t offset=0;
    uint8_t neg=0;
    uint64_t newvalue;
    uint64_t BRIDGE_clock_value=0,CTRLD_clock_value=0;
    uint32_t BRIDGE_clock_value_L=0,CTRLD_clock_value_L=0,BRIDGE_clock_value_H=0,CTRLD_clock_value_H=0;
#ifdef C_BASE_ADDR_RTC
    spin_lock_irqsave(&hardwareLock, flags);
    BRIDGE_clock_value_L=INR_PCI_BAR1_read_ext((C_BASE_ADDR_RTC<<8)+C_SUB_ADDR_RTC_BRIDGE_LOW);
    BRIDGE_clock_value_H=INR_PCI_BAR1_read_ext((C_BASE_ADDR_RTC<<8)+C_SUB_ADDR_RTC_BRIDGE_HIGH);
    CTRLD_clock_value_L=INR_PCI_BAR1_read_ext((C_BASE_ADDR_RTC<<8)+C_SUB_ADDR_RTC_CTRLD_LOW);
    CTRLD_clock_value_H=INR_PCI_BAR1_read_ext((C_BASE_ADDR_RTC<<8)+C_SUB_ADDR_RTC_CTRLD_HIGH);
    spin_unlock_irqrestore(&hardwareLock, flags);
#endif
    BRIDGE_clock_value=BRIDGE_clock_value_L|((uint64_t)BRIDGE_clock_value_H<<32);
    CTRLD_clock_value=CTRLD_clock_value_L|((uint64_t)CTRLD_clock_value_H<<32);

    if(CTRLD_clock_value<BRIDGE_clock_value)neg=1;
    if (neg) offset=(BRIDGE_clock_value-CTRLD_clock_value);
        else offset=(CTRLD_clock_value-BRIDGE_clock_value);


    
    if((BRIDGE_clock_value&0xffffffff)<(u64)hw_value)BRIDGE_clock_value-=0x100000000; //there was an overflow, i asume just one and not several times 4 sec
    //if((CTRLD_clock_value&0xffffffff)<(u64)hw_value)CTRLD_clock_value-=0x100000000; //there was an overflow, i asume just one and not several times 4 sec
    newvalue=((BRIDGE_clock_value&0xffffffff00000000)|(u64)hw_value);
    ts->bridge=newvalue;
    if (neg) newvalue-=offset;
    else newvalue+=offset;
    //if(bridgeclock)
    ts->controlled=newvalue;
    if (TIME_DBG_mod)INR_LOG_debug("TIME adjust value..CTRLD_clock:%lli BRIDGE_clock:%lli negative:%i offset:%lli pkt_value:%lli ts_bridge:%lli ts_ctrld:%lli\n",CTRLD_clock_value,BRIDGE_clock_value,neg,offset,hw_value,ts->bridge,ts->controlled);
    //else
    //  return (((CTRLD_clock_value&0xffffffff00000000)|(u64)hw_value));
    //return timecounter_cyc2time(&tc,(u64)hw_value+abs(offset));
}

