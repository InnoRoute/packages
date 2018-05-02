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
#include "INR-NW.h"
#include "INR.h"
#include "INR-PCI.h"
#include "INR-TIME.h"
volatile uint8_t nwdev_counter = 0;
volatile uint8_t send2cpu = 0;
struct net_device *globnwdev[INR_NW_devcount];
struct hwtstamp_config INR_tstamp_config;

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
    memcpy (nwdev->dev_addr, "\0SNUL1", ETH_ALEN);
    memcpy (nwdev->broadcast, "\0\0\0\0\0\0", ETH_ALEN);
    struct INR_NW_priv *priv = netdev_priv (nwdev);
    nwdev->dev_addr[ETH_ALEN - 1] = priv->port;
    uint8_t i = 0;
    for (i = 0; i < ETH_ALEN; i++) {
        nwdev->broadcast[i] = (uint8_t) 0xff;
    }
    INR_LOG_debug (loglevel_info"HW-addr:%x Broadcast-addr:%x\n", nwdev->dev_addr, nwdev->broadcast);
    netif_start_queue (nwdev);
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
    INR_LOG_debug (loglevel_info"NWDev stop\n");
    netif_stop_queue (nwdev);
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
        unsigned int consumed = 0;
       // if(zerocopy_tx)skb_shinfo(skb)->tx_flags |= SKBTX_DEV_ZEROCOPY; //maybe this fix the memory drain
	//######Timestamping
	if (skb_shinfo(skb)->tx_flags & SKBTX_HW_TSTAMP){//check if timestamp is requested
	if(INR_PCI_HW_timestamp){//hw timestamping
		skb_shinfo(skb)->tx_flags |= SKBTX_IN_PROGRESS; //announce HW will do timestamping
	}else{//no hw timestamping
	skb_tx_timestamp(skb);
	}
	}
	if (skb_shinfo(skb)->tx_flags & SKBTX_SW_TSTAMP)skb_tx_timestamp(skb); 
	
	
	
	//#################
        from = 0;
        if (skb_shinfo (skb)->nr_frags) {
            if (zerocopy_tx) {
                int i = 0;
                error = INR_TX_push (nwdev,skb, skb->data, skb_headlen (skb), 0, toport, get_send2cpu(), 1, skb_shinfo (skb)->nr_frags);
                if (error){
                    goto errorhandling;
                }
                for (i = 0; i < skb_shinfo (skb)->nr_frags - 1; i++) {
                    error = INR_TX_push (nwdev,skb, page_address (skb_shinfo (skb)->frags[i].page.p) + skb_shinfo (skb)->frags[i].page_offset, skb_shinfo (skb)->frags[i].size, 0, toport, get_send2cpu(), 1, skb_shinfo (skb)->nr_frags - i);    //add fragments
                    if (error) {
                        goto errorhandling;
                    }
                }
                i = skb_shinfo (skb)->nr_frags - 1;
                error = INR_TX_push (nwdev,skb, page_address (skb_shinfo (skb)->frags[i].page.p) + skb_shinfo (skb)->frags[i].page_offset, skb_shinfo (skb)->frags[i].size, 1, toport, get_send2cpu(), 1, 1); //add last fragment
                if (error) {
                    goto errorhandling;
                }
            } else {
                skb_prepare_seq_read (skb, from, to, &st);
                while ((len = skb_seq_read (consumed, &data, &st)) != 0) {
                    if (consumed + len == to) {
                        error = INR_TX_push (nwdev,skb, data, len, 1, toport, get_send2cpu(), 0, 1);
                    }
                    if (error) {
                        goto errorhandling;
                    } else {
                        error = INR_TX_push (nwdev,skb, data, len, 0, toport, get_send2cpu(), 0, 30); //30 free fragments needed
                    }
                    if (error) {
                        goto errorhandling;
                    }
                    consumed += len;
                }
            }
        } else {
            if (skb->len > INR_PCI_FPGA_max_tx_length) {
                uint16_t rest = skb->len;
                uint16_t offset = 0, nextfrag = 0;
                uint8_t last = 0;
                uint64_t countfrag = 0;

                while (rest) {
                    if (rest > INR_PCI_FPGA_max_tx_length) {
                        nextfrag = INR_PCI_FPGA_max_tx_length;
                    }
                    else {
                        nextfrag = rest;
                        last = 1;
                    }
                    uint8_t *skb_data = kmalloc (nextfrag, GFP_DMA | GFP_ATOMIC);
                    if (!skb_data){
                        INR_LOG_debug (loglevel_err"Cant alloc tx data\n");
                    } else {
                        memcpy (skb_data, skb->data + offset, nextfrag);
                        error = INR_TX_push (nwdev,skb, skb_data, nextfrag, last, toport, get_send2cpu(), 0, (skb->len / INR_PCI_FPGA_max_tx_length) + 1 - countfrag);
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
                    error = INR_TX_push (nwdev,skb, skb_data, skb->len, 1, toport, get_send2cpu(), 0, 1);
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
                return NETDEV_TX_OK;
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
	INR_LOG_debug (loglevel_info"INR_NW_ioctl called\n");
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
				break;

			case HWTSTAMP_TX_ON:
				if(INR_PCI_HW_timestamp==0){return -EOPNOTSUPP;}else{INR_tstamp_config.tx_type=config.tx_type;}
				break;

			default:
				return -ERANGE;
		}
		switch (config.rx_filter) {
			case HWTSTAMP_FILTER_NONE:
				INR_tstamp_config.rx_filter=config.rx_filter;
				break;

			case HWTSTAMP_FILTER_ALL:
				if(INR_PCI_HW_timestamp==0){return -EOPNOTSUPP;}else{INR_tstamp_config.rx_filter=config.rx_filter;}
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
    spin_lock_init (&priv->lock);
    priv->dev = nwdev;
    priv->port = nwdev_counter++;
    nwdev->netdev_ops = &INR_NW_netdev_ops;
    nwdev->features |= INR_NWDEV_features;
    nwdev->hw_features |= INR_NWDEV_features_HW;
    nwdev->ethtool_ops = &INR_NW_ethtool_ops;
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
{ INR_LOG_debug (loglevel_info"INR_NW_get_ts_info called\n");
	struct INR_NW_priv *priv = netdev_priv(nwdev);

	info->so_timestamping =
			SOF_TIMESTAMPING_TX_SOFTWARE |
			SOF_TIMESTAMPING_RX_SOFTWARE |
			SOF_TIMESTAMPING_SOFTWARE;// |
	if(INR_PCI_HW_timestamp){
	info->so_timestamping|=		
			SOF_TIMESTAMPING_TX_HARDWARE |
			SOF_TIMESTAMPING_RX_HARDWARE |
			SOF_TIMESTAMPING_RAW_HARDWARE;}

		if(INR_PCI_HW_timestamp){
			info->phc_index = ptp_clock_index(INR_TIME_get_ptp_clock());
		}else{
			info->phc_index = -1;
			}

	info->tx_types = (1 << HWTSTAMP_TX_OFF);
	if(INR_PCI_HW_timestamp)info->tx_types |=(1 << HWTSTAMP_TX_ON);

	info->rx_filters =(1 << HWTSTAMP_FILTER_NONE);
	if(INR_PCI_HW_timestamp)info->rx_filters |=(1 << HWTSTAMP_FILTER_ALL);
		
	return 0;
}

int
INR_NW_set_features (struct net_device *net, netdev_features_t features)
{
    u8 tmp;
    struct usbnet *dev = netdev_priv (net);
    netdev_features_t changed = net->features ^ features;

    if (changed & NETIF_F_TSO){
        net->features ^= NETIF_F_TSO;
    }
    if (changed & NETIF_F_SG) {
        net->features ^= NETIF_F_SG;
    }
}
