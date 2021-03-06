/**
*@file
*@brief Functions for the Network-stack
*@author M.Ulbricht 2015
**/

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/version.h>
#include <linux/pci.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/delay.h>
#include <linux/netdevice.h>
#include <linux/if_vlan.h>
#include <linux/netdev_features.h>
#include <linux/etherdevice.h>
#include <linux/dma-mapping.h>
#include <linux/skbuff.h>
#include <linux/net_tstamp.h>
#include <linux/ethtool.h>
#include <linux/ptp_clock_kernel.h>
#include <linux/time.h>
#include "INR-NW.h"
#include "INR.h"
#include "INR-PCI.h"
#include "INR-TIME.h"

volatile uint8_t nwdev_counter = 0;
volatile uint8_t send2cpu = 0;
volatile uint8_t EN_TSN_sock_opt = 0;
volatile uint32_t TSN_TX_ts =0;
volatile uint8_t TSN_TX_queue=0;
struct net_device *globnwdev[INR_NW_devcount];
struct hwtstamp_config INR_tstamp_config;
volatile uint8_t INR_force_HW_ts[INR_NW_devcount]= {0};
volatile uint8_t NO_TX=0;//disable packet TX completely
uint8_t PTP_prio=7;

/**
*set PTP priority
*@param prio
*/
void
INR_NW_set_PTP_prio (uint8_t prio)
{
    PTP_prio=prio;
    INR_PCI_BAR1_write_ext(0x7&prio,(C_BASE_ADDR_FLOW_CACHE<<8)|C_SUB_ADDR_FLOW_CACHE_PTP_QUEUE);
    
}

/**
*get PTP priority
*
*/
uint8_t
INR_NW_get_PTP_prio ()
{
    return PTP_prio;
}


/**
*updates the interface carrier status
*@param index of TrustNode interface
*@param status of interface carrier
*/
void
INR_NW_carrier_update (uint8_t index,uint16_t status)
{
		uint64_t BRIDGE_clock_value=0,CTRLD_clock_value=0;
    uint32_t BRIDGE_clock_value_L=0,CTRLD_clock_value_L=0,BRIDGE_clock_value_H=0,CTRLD_clock_value_H=0;
    INR_LOG_debug("TN%i link state changed to %i\n",index,status);
    BRIDGE_clock_value_L=INR_PCI_BAR1_read_ext((C_BASE_ADDR_RTC<<8)+C_SUB_ADDR_RTC_BRIDGE_LOW);
    BRIDGE_clock_value_H=INR_PCI_BAR1_read_ext((C_BASE_ADDR_RTC<<8)+C_SUB_ADDR_RTC_BRIDGE_HIGH);
    CTRLD_clock_value_L=INR_PCI_BAR1_read_ext((C_BASE_ADDR_RTC<<8)+C_SUB_ADDR_RTC_CTRLD_LOW);
    CTRLD_clock_value_H=INR_PCI_BAR1_read_ext((C_BASE_ADDR_RTC<<8)+C_SUB_ADDR_RTC_CTRLD_HIGH);
    CTRLD_clock_value=CTRLD_clock_value_L|((uint64_t)CTRLD_clock_value_H<<32);
    INR_LOG_debug("CTL_CLK: %lli\n",CTRLD_clock_value);
    if(status) {
    #ifdef C_SUB_ADDR_COMMON_PARAM_PRT_CNT
    if(index<INR_PCI_BAR1_read_ext(C_BASE_ADDR_COMMON_LOWER*256+C_SUB_ADDR_COMMON_PARAM_PRT_CNT))
    #endif
        if(get_nwdev(index)) netif_carrier_on(get_nwdev(index));
    }
    else {
       if(get_nwdev(index)) netif_carrier_off(get_nwdev(index));
    }
}

int INR_NW_set_mac(struct net_device *nwdev, void *p){



	struct sockaddr *addr = p;

	if (!is_valid_ether_addr(addr->sa_data)){
		return -1;
		INR_LOG_debug("error changing mac address");
		}
	INR_LOG_debug("Change MAC to %x",addr->sa_data);
	memcpy(nwdev->dev_addr, addr->sa_data, nwdev->addr_len);
//	memcpy(hw->mac.addr, addr->sa_data, netdev->addr_len);
return 0;
}
/**
*return pointer to net-dev
*@param index index of NW-device
*/
struct net_device *
get_nwdev (uint8_t index)
{
    return globnwdev[index];
}

/**
*reset all global variables
*/
void
INR_NW_zerovars()
{
    nwdev_counter = 0;
    send2cpu = 0;
    PTP_prio=7;
}

/**
*store net-dev point for later use
*@param index index of NW-device
*@param dev NW-dev
*/
void
set_nwdev (uint8_t index, struct net_device *dev)
{
    globnwdev[index] = dev;
    INR_NW_STATUS_set (1 << index);
}

/**
*set send2cpu-flag
*@param flag
*/
void
set_send2cpu (uint8_t flag)
{
    send2cpu = flag;
}
/**
*set TSN value socket option
*@param flag
*/
void
set_TSN_sock_opt (uint8_t flag)
{
    EN_TSN_sock_opt=flag;
}
/**
*set TSN ts value
*@param flag
*/
void
set_TSN_ts (uint32_t flag)
{
    TSN_TX_ts=flag;
}
/**
*set TSN ts value
*@param flag
*/
void
set_TSN_queue (uint8_t flag)
{
    TSN_TX_queue=flag;
}
/**
*get send2cpu-flag
*/
uint8_t
get_send2cpu ()
{
    return send2cpu;
}

/**
*NW-rx
*@param nwdev
*@param pkt Ethernetpacket
*/
void
INR_NW_rx (struct net_device *nwdev, struct INR_NW_packet *pkt)
{
    INR_LOG_debug (loglevel_info"rx called\n");
    struct sk_buff *skb;
    struct INR_NW_priv *priv = netdev_priv (nwdev);
    skb = dev_alloc_skb (pkt->datalen + 2);
    if (!skb) {
        priv->stats.rx_dropped++;
        return;
    }
    skb_reserve (skb, 2);
    memcpy (skb_put (skb, pkt->datalen), pkt->data, pkt->datalen);
    skb->dev = nwdev;
    skb->protocol = eth_type_trans (skb, nwdev);
    skb->ip_summed = CHECKSUM_UNNECESSARY;
    priv->stats.rx_packets++;
    priv->stats.rx_bytes += pkt->datalen;
    netif_rx (skb);
    return;
}

//*****************************************************************************************************************
/**
*NW-config
*@param nwdev network device
*@param config change-map
*/
int
INR_NW_config (struct net_device *nwdev, struct ifmap *map)
{
    INR_LOG_debug (loglevel_info"NW-config called\n");
    return 0;
}

//*****************************************************************************************************************
/**
*NW-dev open
*@param nwdev network device
*/
int
INR_NW_open (struct net_device *nwdev)
{
    INR_LOG_debug (loglevel_info "NWDev open\n");
    
    struct INR_NW_priv *priv = netdev_priv (nwdev);
    nwdev->max_mtu=INR_NWDEV_MAX_MTU;
    INR_LOG_debug (loglevel_info"HW-addr:%x Broadcast-addr:%x\n", nwdev->dev_addr, nwdev->broadcast);
    netif_start_queue (nwdev);
    INR_PCI_FPGA_PORT_status(priv->port,1);
    netif_tx_start_all_queues(nwdev);
    return 0;
}

//*****************************************************************************************************************
/**
*NW-dev stop
*@param nwdev network device
*/
int
INR_NW_stop (struct net_device *nwdev)
{
    struct INR_NW_priv *priv = netdev_priv (nwdev);
    netif_tx_stop_all_queues(nwdev);
    INR_PCI_FPGA_PORT_status(priv->port,0);
    INR_LOG_debug (loglevel_info"NWDev stop\n");
    netif_stop_queue (nwdev);
}
//*****************************************************************************************************************
/**
*set NO-TX
*@param TX state
*/
void 
INR_NW_set_NO_TX (uint8_t tx_state)
{
    NO_TX=tx_state;
}
//*****************************************************************************************************************
/**
*Software transmit function, called by kernel
*@param skb socket buffer
*@param nwdev network device
*/
netdev_tx_t
INR_NW_tx (struct sk_buff *skb, struct net_device *nwdev)
{
    if (INR_STATUS_get (INR_STATUS_HW_RUNNING)) {
        struct INR_NW_priv *priv = netdev_priv (nwdev);
        uint8_t toport = priv->port;
        uint8_t error = 0;
        unsigned int from;
        unsigned int to = skb->len;
        struct skb_seq_state st;
        const u8 *data;
        unsigned int len;
        uint8_t time_queue=0;
        uint32_t TXtimestamp=0;
        unsigned int consumed = 0;
        uint16_t TX_confirmastion_id=0;
        uint64_t no0cpdata_consumed=0;		
				
				if(NO_TX){if (skb)
                    kfree_skb (skb);    //free skb if dropped (handled by nw_stack if returned busy)
                return NETDEV_TX_OK;
        }

        if (EN_TSN_sock_opt) {
#if LINUX_VERSION_CODE < KERNEL_VERSION(5,0,0)        
            time_queue=0x3f&skb->TN_TX_queue;
            if(get_tx_dbg())printk("TX queue:%i   (%i)\n",skb->TN_TX_queue,time_queue);
            TXtimestamp=skb->TN_TX_timestamp;
#else
//todo
#endif            
        } else {
            time_queue=0x3f&TSN_TX_queue;
            TXtimestamp=TSN_TX_ts;
        }
        uint16_t ethertype=((uint16_t )skb->data[13] << 8) + skb->data[12];
				if(get_tx_dbg())printk("Ethertype:%x\n",ethertype);
				if (ethertype==0xf788){//is ptp packet
					time_queue=PTP_prio;
				}
        // if(zerocopy_tx)skb_shinfo(skb)->tx_flags |= SKBTX_DEV_ZEROCOPY; //maybe this fix the memory drain
        //######Timestamping
        if ((skb_shinfo(skb)->tx_flags & SKBTX_HW_TSTAMP)||INR_force_HW_ts[toport]) { //check if timestamp is requested or forced
            if(get_INR_PCI_HW_timestamp()&&get_HW_user_feature(HW_feature_TXconf)) { //hw timestamping
                skb_shinfo(skb)->tx_flags |= SKBTX_IN_PROGRESS; //announce HW will do timestamping
                TX_confirmastion_id=INR_TIME_TX_add(skb);// store skb for later
            } else { //no hw timestamping
                skb_tx_timestamp(skb);
            }
        }
        if (skb_shinfo(skb)->tx_flags & SKBTX_SW_TSTAMP)skb_tx_timestamp(skb);



        //#################
        from = 0;
        if (skb_shinfo (skb)->nr_frags) {
             	
							uint8_t *no0cpdata = kmalloc (skb->len, GFP_DMA | GFP_ATOMIC);				
												
							
            if(get_tx_dbg())printk("Fragmented SKB, zerocopy:%i\n",zerocopy_tx);
                int i = 0;
                if(get_INR_PCI_HW_timestamp()) {
                    uint64_t *timestamp = kmalloc (8, GFP_DMA | GFP_ATOMIC);
                    //ktime_t time=ktime_get(); //real
                    *timestamp=((uint64_t)TX_confirmastion_id<<32)|(0xffffffff&TXtimestamp);
                    error = INR_TX_push (nwdev,NULL, (uint8_t*)timestamp, 8, 0, toport, get_send2cpu(), 0, 1,time_queue,1);
                    if (error) {
                        goto errorhandling;
                    }
                }
                if (zerocopy_tx) {error = INR_TX_push (nwdev,skb, skb->data, skb_headlen (skb), 0, toport, get_send2cpu(), 1, skb_shinfo (skb)->nr_frags,time_queue,0);
                if (error) {
                    goto errorhandling;
                }} else { //nozerocopy
                memcpy(no0cpdata+no0cpdata_consumed,skb->data,skb_headlen (skb));             
                no0cpdata_consumed+=skb_headlen (skb);
                }
                for (i = 0; i < skb_shinfo (skb)->nr_frags - 1; i++) {
                if (zerocopy_tx){
                    error = INR_TX_push (nwdev,skb, page_address (skb_shinfo (skb)->frags[i].page.p) + skb_shinfo (skb)->frags[i].page_offset, skb_shinfo (skb)->frags[i].size, 0, toport, get_send2cpu(), 1, skb_shinfo (skb)->nr_frags - i,time_queue,0);    //add fragments
                    if (error) {
                        goto errorhandling;
                    }}else{//nozerocopy
                    memcpy(no0cpdata+no0cpdata_consumed,page_address (skb_shinfo (skb)->frags[i].page.p) + skb_shinfo (skb)->frags[i].page_offset,skb_shinfo (skb)->frags[i].size);             
                		no0cpdata_consumed+=skb_shinfo (skb)->frags[i].size;
                    
                    
                    
                    }
                }
                i = skb_shinfo (skb)->nr_frags - 1;
                if (zerocopy_tx){error = INR_TX_push (nwdev,skb, page_address (skb_shinfo (skb)->frags[i].page.p) + skb_shinfo (skb)->frags[i].page_offset, skb_shinfo (skb)->frags[i].size, 1, toport, get_send2cpu(), 1, 1,time_queue,0); //add last fragment
                if (error) {
                    goto errorhandling;
                }}else{//nozerocopy
                memcpy(no0cpdata+no0cpdata_consumed,page_address (skb_shinfo (skb)->frags[i].page.p) + skb_shinfo (skb)->frags[i].page_offset,skb_shinfo (skb)->frags[i].size);             
                no0cpdata_consumed+=skb_shinfo (skb)->frags[i].size;
                
                }
							if (zerocopy_tx==0){
							error = INR_TX_push (nwdev,skb, no0cpdata, no0cpdata_consumed, 1, toport, get_send2cpu(), 1, 1,time_queue,0); //add last fragment
                if (error) {
                    goto errorhandling;
                }			
							
							}else kfree(no0cpdata);
        } else { if(get_tx_dbg())printk("non Fragmented SKB\n");
            if (skb->len > INR_PCI_FPGA_max_tx_length) {
                uint16_t rest = skb->len;
                uint16_t offset = 0, nextfrag = 0;
                uint8_t last = 0;
                uint64_t countfrag = 0;
                if(get_INR_PCI_HW_timestamp()) {
                    uint64_t *timestamp = kmalloc (8, GFP_DMA | GFP_ATOMIC);
                    //ktime_t time=ktime_get();
                    *timestamp=((uint64_t)TX_confirmastion_id<<32)|(0xffffffff&TXtimestamp);
                    error = INR_TX_push (nwdev,NULL, (uint8_t*)timestamp, 8, 0, toport, get_send2cpu(), 0, 1,time_queue,1);
                    if (error) {
                        goto errorhandling;
                    }
                }
                while (rest) {
                if(get_tx_dbg())printk("bigskbhandling rest:%i, offset:%i\n",rest,offset);
                    if (rest > INR_PCI_FPGA_max_tx_length) {
                        nextfrag = INR_PCI_FPGA_max_tx_length;
                    }
                    else {
                        nextfrag = rest;
                        last = 1;
                    }
                    uint8_t *skb_data = kmalloc (nextfrag, GFP_DMA | GFP_ATOMIC);
                    if (!skb_data) {
                        INR_LOG_debug (loglevel_err"Cant alloc tx data\n");
                    } else {
                        memcpy (skb_data+ offset, skb->data + offset, nextfrag);
                        error = INR_TX_push (nwdev,skb, skb_data, nextfrag, last, toport, get_send2cpu(), 0, (skb->len / INR_PCI_FPGA_max_tx_length) + 1 - countfrag,time_queue,0);
                        countfrag++;
                        if (error) {
                            goto errorhandling;
                        }
                    }
                    offset += nextfrag;
                    rest -= nextfrag;
                }

            } else {//
                uint8_t *skb_data;
                if (0) {
                    INR_LOG_debug (loglevel_err"Cant alloc tx data\n");
                } else {
                    skb_data = kmemdup (skb->data, skb->len, GFP_DMA);
                    if (!skb_data) {
                        INR_LOG_debug (loglevel_err"Cant alloc tx data\n");
                        goto errorhandling;
                    }
                    if(get_INR_PCI_HW_timestamp()) {
                        uint64_t *timestamp = kmalloc (8, GFP_DMA | GFP_ATOMIC);
                        //ktime_t time=ktime_get();
                        *timestamp=((uint64_t)TX_confirmastion_id<<32)|(0xffffffff&TXtimestamp);
                        error = INR_TX_push (nwdev,NULL, (uint8_t*)timestamp, 8, 0, toport, get_send2cpu(), 0, 1,time_queue,1);
                        if (error) {
                            goto errorhandling;
                        }
                    }
                    error = INR_TX_push (nwdev,skb, skb_data, skb->len, 1, toport, get_send2cpu(), 0, 1,time_queue,0);
                    if (error) {
                        if (skb_data) {
                            kfree (skb_data);
                        }
                        goto errorhandling;
                    }
                }
            }
        }
        priv->stats.tx_packets++;
        priv->stats.tx_bytes += skb->len;

errorhandling:
        if (error) {
            priv->stats.tx_dropped++;   //fpga descriptorrin full pkt dropped
            if (INR_NW_repeatonbusy) {
                return NETDEV_TX_BUSY;  //dont drop, just tell nw stack to repeat
            } else {
                if (skb)
                    kfree_skb (skb);    //free skb if dropped (handled by nw_stack if returned busy)
                return NET_XMIT_DROP;
            }
        }
        
        return NETDEV_TX_OK;
    }
    return NETDEV_TX_BUSY;
}

//*****************************************************************************************************************
/**
*change device options
*@param nwdev network device
*@param rq
*@param cmd
*/
int
INR_NW_ioctl (struct net_device *nwdev, struct ifreq *rq, int cmd)
{
    //INR_LOG_debug (loglevel_info"INR_NW_ioctl called, cmd=0x%lx\n",cmd);
    struct INR_NW_priv *priv = netdev_priv(nwdev);
    struct hwtstamp_config config;
    if (copy_from_user(&config, rq->ifr_data, sizeof(config)))
        return -EFAULT;
    if (config.flags)
        return -EINVAL;

    switch (cmd) {
    case SOF_TIMESTAMPING_TX_SOFTWARE:
        break;
    case SIOCGHWTSTAMP:
        copy_to_user(rq->ifr_data, &INR_tstamp_config,sizeof(INR_tstamp_config));
        break;
    case SIOCSHWTSTAMP:
        switch (config.tx_type) {
        case HWTSTAMP_TX_OFF:
            INR_tstamp_config.tx_type=config.tx_type;
            INR_force_HW_ts[priv->port]=0;
            break;

        case HWTSTAMP_TX_ON:
            if((get_INR_PCI_HW_timestamp()==0)||(get_HW_user_feature(HW_feature_TXconf)==0)) {
                return -EOPNOTSUPP;
            }
            else {
                INR_tstamp_config.tx_type=config.tx_type;
                INR_force_HW_ts[priv->port]=1;
            }
            break;

        default:
            return -ERANGE;
        }
        switch (config.rx_filter) {
        case HWTSTAMP_FILTER_NONE:
            INR_tstamp_config.rx_filter=config.rx_filter;
            break;

        case HWTSTAMP_FILTER_ALL:
        case HWTSTAMP_FILTER_PTP_V2_L2_EVENT:
        case HWTSTAMP_FILTER_PTP_V2_L4_EVENT:
        case HWTSTAMP_FILTER_PTP_V1_L4_SYNC:

            if((get_INR_PCI_HW_timestamp()==0)) {
                return -EOPNOTSUPP;
            }
            else {
                INR_tstamp_config.rx_filter=config.rx_filter;
            }
            break;
        case HWTSTAMP_FILTER_SOME:
        case HWTSTAMP_FILTER_PTP_V1_L4_EVENT:
        case HWTSTAMP_FILTER_PTP_V1_L4_DELAY_REQ:
        case HWTSTAMP_FILTER_PTP_V2_L4_SYNC:
        case HWTSTAMP_FILTER_PTP_V2_L4_DELAY_REQ:
        case HWTSTAMP_FILTER_PTP_V2_L2_SYNC:
        case HWTSTAMP_FILTER_PTP_V2_L2_DELAY_REQ:
        case HWTSTAMP_FILTER_PTP_V2_EVENT:
        case HWTSTAMP_FILTER_PTP_V2_SYNC:
        case HWTSTAMP_FILTER_PTP_V2_DELAY_REQ:
            return -EOPNOTSUPP;
            break;
        default:
            return -ERANGE;
        }

        memcpy(&INR_tstamp_config, &config, sizeof(config));
        break;
    default:
        return -EOPNOTSUPP;
    }
    return 0;
}

//*****************************************************************************************************************
/**
*return device statistics
*@param nwdev network device
*/
struct net_device_stats *
INR_NW_stats (struct net_device *nwdev)
{
    struct INR_NW_priv *priv = netdev_priv (nwdev);
    return &priv->stats;
}

//*****************************************************************************************************************
/**
*change mtu
*@param cahnge mtu
*@param nwdev network device
*/
int
INR_NW_change_mtu (struct net_device *nwdev, int new_mtu)
{
    INR_LOG_debug (loglevel_info"change_mtu called\n");
    return 0;
}

//*****************************************************************************************************************
/**
*tx-timeut function
*@param nwdev network device
*/
void
INR_NW_tx_timeout (struct net_device *nwdev)
{
    INR_LOG_debug (loglevel_info"tx_timeour called\n");
    return;
}


//*****************************************************************************************************************
/**
*init nw-device structure
*@param nwdev network device
*/
//*****************************************************************************************************************
void
INR_NW_init (struct net_device *nwdev)
{
    struct INR_NW_priv *priv = netdev_priv (nwdev);
    memset (priv, 0, sizeof (struct INR_NW_priv));
    memcpy (nwdev->dev_addr, "\0SNUL1", ETH_ALEN);
    memcpy (nwdev->broadcast, "\0\0\0\0\0\0", ETH_ALEN);
    spin_lock_init (&priv->lock);
    priv->dev = nwdev;
    priv->port = nwdev_counter++;
    nwdev->netdev_ops = &INR_NW_netdev_ops;
    nwdev->features |= INR_NWDEV_features;
    nwdev->hw_features |= INR_NWDEV_features_HW;
    nwdev->ethtool_ops = &INR_NW_ethtool_ops;
    nwdev->real_num_tx_queues=INR_NW_queue_count;
    nwdev->real_num_rx_queues=0;
    nwdev->max_mtu=INR_NWDEV_MAX_MTU;
    nwdev->mtu=1400;
    nwdev->dev_addr[ETH_ALEN - 1] = priv->port;
    uint8_t i = 0;
    for (i = 0; i < ETH_ALEN; i++) {
        nwdev->broadcast[i] = (uint8_t) 0xff;
    }
    //SET_ETHTOOL_OPS(nwdev, &INR_NW_ethtool_ops);
    ether_setup (nwdev);
    INR_LOG_debug (loglevel_info"Init NWDev %i done\n", priv->port);
}

//*****************************************************************************************************************
/**
*ETHtool callback function for timstamping and ptp parameters
*@param *dev network device
*@param *info info request
*/
//*****************************************************************************************************************
static int INR_NW_get_ts_info(struct net_device *nwdev, struct ethtool_ts_info *info)
{   INR_LOG_debug (loglevel_info"INR_NW_get_ts_info called\n");
    struct INR_NW_priv *priv = netdev_priv(nwdev);

    info->so_timestamping =
        SOF_TIMESTAMPING_TX_SOFTWARE |
        SOF_TIMESTAMPING_RX_SOFTWARE |
        SOF_TIMESTAMPING_SOFTWARE;// |
    if(get_INR_PCI_HW_timestamp()) {
        info->so_timestamping|=
            SOF_TIMESTAMPING_TX_HARDWARE|
            SOF_TIMESTAMPING_RX_HARDWARE |
            SOF_TIMESTAMPING_RAW_HARDWARE;
    }
    info->tx_types =
        (1<<HWTSTAMP_TX_OFF) |
        (1<<HWTSTAMP_TX_ON);
    if(get_HW_user_feature(HW_feature_TXconf))info->so_timestamping|=SOF_TIMESTAMPING_TX_HARDWARE;

    if(get_HW_user_feature(HW_feature_RTC)) {
        info->phc_index = ptp_clock_index(INR_TIME_get_ptp_clock());
    } else {
        info->phc_index = -1;
    }

    info->tx_types = (1 << HWTSTAMP_TX_OFF);
    if(get_INR_PCI_HW_timestamp())info->tx_types |=(1 << HWTSTAMP_TX_ON);

    info->rx_filters =(1 << HWTSTAMP_FILTER_NONE);
    if(get_INR_PCI_HW_timestamp())info->rx_filters |=(1 << HWTSTAMP_FILTER_ALL);

    return 0;
}

int
INR_NW_set_features (struct net_device *net, netdev_features_t features)
{
    u8 tmp;
    struct usbnet *dev = netdev_priv (net);
    netdev_features_t changed = net->features ^ features;

    if (changed & NETIF_F_TSO) {
        net->features ^= NETIF_F_TSO;
    }
    if (changed & NETIF_F_SG) {
        net->features ^= NETIF_F_SG;
    }
}
