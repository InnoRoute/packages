/**
*@file
*@brief Functions for PCI communication
*@author M.Ulbricht 2015
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
#include "INR.h"
#include "INR-NW.h"
#include "INR-PCI.h"
#include "INR-NAPI.h"
#include "INR-TIME.h"
#include <asm/signal.h>
#include <linux/semaphore.h>
#include <asm/cacheflush.h>
#include "INR-MMI.h"

uint8_t INR_PCI_HW_timestamp=0;
volatile uint32_t tx_head_backup = 0;	/**< Storage for tx_headpointer, to spot cluttering of the FPGA*/
volatile uint8_t TX_DBG_mod=0;		/**< verbose TX messages*/
volatile uint8_t RX_DBG_mod=0;		/**< verbose RX messages*/
volatile uint64_t gIrq; 		/**< interrupt id*/
uint8_t TNrussian=0;			/**< russian mode, allow everything without valid. check*/
struct pci_dev *globdev;		/**< global availbele PCI dev structure*/
void *gBaseVirt0 = NULL;		/**< pointer for start-address of bar0*/
void *gBaseVirt1 = NULL;		/**< pointer for start-address of bar1*/
uint64_t gBaseHdwr0,			/**< start-address of bar0*/
         gBaseLen0; 			/**<legth of bar0*/
uint64_t gBaseHdwr1,			/**< start-address of bar1*/
         gBaseLen1; 			/**<legth of bar1*/
uint64_t INR_TX_Ring;	      		/**<Address of TX-descriptor ring*/
uint64_t INR_RX_Ring[INR_PCI_rx_descriptor_ring_count];	      /**<Address of RX-descriptor ring*/
uint64_t INR_PCI_tx_descriptor_ring[INR_PCI_tx_descriptor_ring_length] = { 0 };/**<Saving the addresses of TX descriptors*/
uint64_t INR_PCI_rx_descriptor_ring[INR_PCI_rx_descriptor_ring_length]
[INR_PCI_rx_descriptor_ring_count] = {0};/**<Saving the addresses of RX descriptors*/
uint32_t FPGA_PORT_mask=0;		/**<saving status of FPGA PHY ports*/

volatile uint8_t txsemophore = 0;
uint64_t INR_PCI_tx_descriptor_base = NULL;					/**<Base address of TX ring*/
volatile uint64_t INR_PCI_tx_descriptor_current = 0;				/**<next free tx descriptor*/
uint64_t INR_PCI_tx_descriptor_unmap_current = 0;				/**<next free tx descriptor*/

uint64_t INR_PCI_rx_descriptor_base[INR_PCI_rx_descriptor_ring_count] = { 0 };	/**<Base addresses of RX rings*/
uint64_t INR_PCI_rx_descriptor_current[INR_PCI_rx_descriptor_ring_count] = { 0 };/**<next free tx descriptor*/

uint32_t page_prealloc_current = 0;
struct page *INR_PCI_rx_preallocated_page[INR_PCI_page_prealloc_count] = { NULL };

struct page *INR_PCI_rx_descriptor_pool_root[INR_PCI_rx_descriptor_ring_count];/**<root of the rx descriptor pool*/
uint64_t INR_PCI_rx_descriptor_pool_dma_root[INR_PCI_rx_descriptor_ring_count];/**<dma_root of the rx descriptor pool*/
uint64_t rx_descriptor_pool_length = 0;						/**<count of rx-descriptors in one page*/
uint64_t rx_descriptor_pool_free[INR_PCI_rx_descriptor_ring_count];		/**<count of free descriptors in rx-pool*/
struct sk_buff *rx_skb[INR_PCI_rx_descriptor_ring_count];			/**<actual rx-skb, available for each RX-ring*/
struct INR_PCI_tx_descriptor_ring_entry data_tx[INR_PCI_tx_descriptor_ring_length];/**<stores memory information aberout every fragment in tx ring*/
struct INR_PCI_tx_descriptor_ring_entry tx_descriptor_unmap_queue[INR_PCI_tx_descriptor_ring_length] = { 0 };/**<tx descript unmap queue*/

uint32_t tx_unmap_queue_write_pointer = 0;					/**<points to the next free space in tx unmap queue*/
uint32_t tx_unmap_queue_read_pointer = 1;					/**<points to the tx-fragment which will be unpapped on next call of unmapper*/
//readpointer befor write, so wee wait the whole ring until handeling
uint8_t firstpkg[INR_PCI_rx_descriptor_ring_count] = { 1 };			/**<is the curretn SKB the first of packet?*/
uint8_t dropmode[INR_PCI_rx_descriptor_ring_count] = { 0 };			/**<drop all next fragments until eop (happens if bad segemtn received)*/

uint32_t refillcount[INR_PCI_rx_descriptor_ring_count] = { 0 };			/**<count the refills of descriptor-pools  */

struct sk_buff *skb_head_rx[INR_PCI_rx_descriptor_ring_count];
struct INR_PCI_rx_descriptor_ring_entry *data_rx[INR_PCI_rx_descriptor_ring_length][INR_PCI_rx_descriptor_ring_count];	/**<Structure to store the received data*/
uint32_t pagesize = 0;
uint64_t descriptor_current_lastwritten = 0;					/**<remember last written descriptor */
uint64_t dma_rx[INR_PCI_rx_descriptor_ring_length][INR_PCI_rx_descriptor_ring_count];	/**<DMA adresses of the received data*/
static DECLARE_WAIT_QUEUE_HEAD (INR_PCI_TX_unmapd_waittingqueu);		/**<waiting lock for TX_descriptor_unmap_deamon*/
static DECLARE_WAIT_QUEUE_HEAD (INR_PCI_rx_pageallocator_waittingqueu);		/**<waiting lock for rx pageallocator*/
static DECLARE_WAIT_QUEUE_HEAD (INR_tailtest);					/**<waiting lock for rx tailtest*/
uint64_t lastbot[INR_PCI_rx_descriptor_ring_count] = { (INR_PCI_rx_descriptor_ring_length - 1) * INR_PCI_rx_descriptor_length };	/**<store last bottom value*/
DEFINE_SEMAPHORE (INR_PCI_rx_pageallocator_sem);
EXPORT_SYMBOL(gBaseVirt1); //export for use in MMI module
uint32_t HW_addr_map_revision=0;
uint32_t HW_features_user=0;
/**
*setts all global variables to zero
*/
void
INR_PCI_zerovars() {
    uint64_t j=0;
    txsemophore = 0;
    INR_PCI_tx_descriptor_current = 0;
    INR_PCI_tx_descriptor_unmap_current = 0;
    for(j=0; j<INR_PCI_rx_descriptor_ring_count; j++)INR_PCI_rx_descriptor_base[j]=0;
    for(j=0; j<INR_PCI_rx_descriptor_ring_count; j++)INR_PCI_rx_descriptor_current[j]=0;
    page_prealloc_current=0;
    for(j=0; j<INR_PCI_page_prealloc_count; j++)INR_PCI_rx_preallocated_page[j] =  NULL ;
    rx_descriptor_pool_length=0;
    tx_unmap_queue_write_pointer = 0;
    tx_unmap_queue_read_pointer = 1;
    for(j=0; j<INR_PCI_rx_descriptor_ring_count; j++)firstpkg[j] = 1;
    for(j=0; j<INR_PCI_rx_descriptor_ring_count; j++)dropmode[j] = 0;
    for(j=0; j<INR_PCI_rx_descriptor_ring_count; j++)refillcount[j] = 0 ;
    for(j=0; j<INR_PCI_rx_descriptor_ring_count; j++)lastbot[j] = (INR_PCI_rx_descriptor_ring_length - 1) * INR_PCI_rx_descriptor_length ;
    pagesize = 0;
}



//*****************************************************************************************************************
/**
*Change Status of an FPGA-PHY-port
*@brief This is nor the real phy port, just a switch inside the FPGA to control the transmission of packetes to the CPU
*/
void INR_PCI_FPGA_PORT_status(uint8_t id, uint8_t status) {

    if (status) FPGA_PORT_mask|=(1<<id);
    else FPGA_PORT_mask&=~(1<<id);
#ifdef C_SUB_ADDR_NET_ENABLE
    if(gBaseVirt1) {

        if(HW_addr_map_revision>=5)INR_PCI_BAR1_write(FPGA_PORT_mask,(C_BASE_ADDR_NET_LOWER<<8)+C_SUB_ADDR_NET_ENABLE);
    }
#endif


}

//*****************************************************************************************************************
/**
*adds an TX entry to the unmap queue.
*@brief This is needed to give the hanrdware some time to acces and don''t cause memory violations.
*/
void
INR_PCI_tx_unmapper (struct INR_PCI_tx_descriptor_ring_entry *entry)//currently unused
{

    INR_LOG_debug (loglevel_warn"Unmapper: wr:%i re:%i addr:%x\n", tx_unmap_queue_write_pointer, tx_unmap_queue_read_pointer, entry->dma);
    memcpy (&tx_descriptor_unmap_queue[tx_unmap_queue_write_pointer], entry, sizeof (entry));	//put tx-descriptor in the queue

    if (tx_descriptor_unmap_queue[tx_unmap_queue_read_pointer].dma) {	//if not empty, unmap
        dma_unmap_single (&globdev->dev, tx_descriptor_unmap_queue[tx_unmap_queue_read_pointer].dma, tx_descriptor_unmap_queue[tx_unmap_queue_read_pointer].length, DMA_TO_DEVICE);	//unmap DMA
        if (tx_descriptor_unmap_queue[tx_unmap_queue_read_pointer].eop)
            kfree_skb (tx_descriptor_unmap_queue[tx_unmap_queue_read_pointer].skb);	//if last fragment of skb->free skb
        if (!tx_descriptor_unmap_queue[tx_unmap_queue_read_pointer].paged)
            kfree (tx_descriptor_unmap_queue[tx_unmap_queue_read_pointer].data);	//if not paged free kmalloc mem
        INR_LOG_debug (loglevel_warn"Unmapper: %i freeed\n", tx_unmap_queue_read_pointer);
    }


    if (tx_unmap_queue_write_pointer < (INR_PCI_tx_descriptor_ring_length - 1)) {
        tx_unmap_queue_write_pointer++;
    }
    else {
        tx_unmap_queue_write_pointer = 0;
    }
    if (tx_unmap_queue_read_pointer < (INR_PCI_tx_descriptor_ring_length - 1)) {
        tx_unmap_queue_read_pointer++;
    }
    else {
        tx_unmap_queue_read_pointer = 0;
    }
}
//*****************************************************************************************************************
/**
*read features of Hardware
*/
uint8_t get_HW_user_feature(uint32_t featurerequest) {
    if(HW_features_user&featurerequest) return 1;
    else return 0;
}

//*****************************************************************************************************************
/**
*read from bar1
*/
uint32_t INR_PCI_BAR1_read_ext(uint32_t read_addr) {
//INR_LOG_debug("Read from bar1: addr:0x%lx\n",read_addr);
    if(INR_STATUS_get (INR_STATUS_HW_RUNNING))return INR_PCI_BAR1_read(read_addr);
    return 0;
}

//*****************************************************************************************************************
/**
*read from bar1
*/
void INR_PCI_BAR1_write_ext(uint32_t value,uint32_t read_addr) {
    if (INR_STATUS_get (INR_STATUS_HW_RUNNING))INR_PCI_BAR1_write(value,read_addr);
}
//*****************************************************************************************************************
/**
*read version form hardware
*/
uint32_t
get_pci_version (void)
{

    return INR_PCI_BAR0_read (INR_PCI_version);
}
//*****************************************************************************************************************
/**
*read version form hardware
*/
uint8_t
get_INR_PCI_HW_timestamp (void)
{

    return INR_PCI_HW_timestamp;
}
//*****************************************************************************************************************
/**
*activate TX debug messages
*/
void set_tx_dbg(uint8_t txdbg)
{

    TX_DBG_mod=txdbg;
    INR_PCI_disable_error_LED
}

//*****************************************************************************************************************
/**
*activate RX debug messages
*/
void set_rx_dbg(uint8_t rxdbg)
{

    RX_DBG_mod=rxdbg;
    INR_PCI_disable_error_LED
}

//*****************************************************************************************************************
/**
*ret TX debug messages
*/
uint8_t get_tx_dbg(void)
{

    return TX_DBG_mod;
}

//*****************************************************************************************************************
/**
*ret RX debug messages
*/
uint8_t get_rx_dbg(void)
{

    return RX_DBG_mod;
}
//*****************************************************************************************************************
/**
*activate russian mode for PCI , accept everything without valid check
*/
void set_russian(uint8_t mode)
{

    TNrussian=mode;
}

//*****************************************************************************************************************
/**
*ret russian mode for PCI
*/
uint8_t get_russian(void)
{

    return TNrussian;
}
//*****************************************************************************************************************
/**
*refill rx-descriptor pool
*@param intex of rx ring
*/
uint8_t
INR_PCI_rx_descriptor_pool_refill (uint8_t index)
{
    refillcount[index]++;
    if ((page_prealloc_current))	//if no pages preallocated, try selv-allocation
    {
        if (down_trylock (&INR_PCI_rx_pageallocator_sem)) {//ry to get a preallocated page, if not, try self
            INR_PCI_rx_descriptor_pool_root[index] = alloc_page (GFP_ATOMIC);
            if (unlikely (INR_PCI_rx_descriptor_pool_root[index] == NULL)) {
                INR_LOG_debug (loglevel_err"RX Page alloc faild\n");
                INR_PCI_enable_error_LED
                return 1;
            }
        }
        else {
            INR_PCI_rx_descriptor_pool_root[index] = INR_PCI_rx_preallocated_page[--page_prealloc_current];	//get new root page
            up (&INR_PCI_rx_pageallocator_sem);
        }


    }
    else
        INR_PCI_rx_descriptor_pool_root[index] = alloc_page (GFP_ATOMIC);
    if (!INR_PCI_rx_descriptor_pool_root[index]) {
        INR_LOG_debug (loglevel_err"RX Page alloc faild(noprealloc)\n");
        INR_PCI_enable_error_LED
        return 1;
    }
    INR_PCI_rx_descriptor_pool_dma_root[index] = dma_map_page (&globdev->dev, INR_PCI_rx_descriptor_pool_root[index], 0, pagesize, DMA_FROM_DEVICE);	//gen new root dma
    if (dma_mapping_error (&globdev->dev, INR_PCI_rx_descriptor_pool_dma_root[index])) {
        INR_PCI_enable_error_LED
        INR_LOG_debug (loglevel_err"RX page mappng error! \n");
    }
    rx_descriptor_pool_free[index] = 0;	//reset pool counter
    return 0;
}

//*****************************************************************************************************************
/**
*return pagefragment as rx_descriptior memeory
*@brief returns rx descriptors with prealloceted page fragments, ordered per RX-ring to avoid memory fragmentations
*/
struct INR_PCI_rx_descriptor_ring_entry *
INR_PCI_get_new_rx_descriptor_ring_entry (uint8_t index)
{
    if (rx_descriptor_pool_free[index] >= rx_descriptor_pool_length)
        while (INR_PCI_rx_descriptor_pool_refill (index));
    //if empty, block until refill, normally this should not happen, because refill is triggered automaticly before pagestorage is empty
    struct INR_PCI_rx_descriptor_ring_entry *tmp_rx_desc = kmalloc (sizeof (struct INR_PCI_rx_descriptor_ring_entry), GFP_ATOMIC);
    while (!tmp_rx_desc)
        tmp_rx_desc = kmalloc (sizeof (struct INR_PCI_rx_descriptor_ring_entry), GFP_ATOMIC);	//block if kmalloc fails
    tmp_rx_desc->page = INR_PCI_rx_descriptor_pool_root[index];
    tmp_rx_desc->offset = rx_descriptor_pool_free[index] * data_size_rx;
    tmp_rx_desc->data = page_address (INR_PCI_rx_descriptor_pool_root[index]) + tmp_rx_desc->offset;
    tmp_rx_desc->dma = INR_PCI_rx_descriptor_pool_dma_root[index] + tmp_rx_desc->offset;
    tmp_rx_desc->dma_root = INR_PCI_rx_descriptor_pool_dma_root[index];
    tmp_rx_desc->fragmentindex = rx_descriptor_pool_free[index];
    if (!INR_PCI_rx_descriptor_pool_root[index]) {
        INR_LOG_debug (loglevel_err"error: zero rx descriptor pool root\n");
        INR_PCI_enable_error_LED
    }
    if (!page_address (INR_PCI_rx_descriptor_pool_root[index])) {
        INR_LOG_debug (loglevel_err"error: zero page_address (INR_PCI_rx_descriptor_pool_root[index])\n");
        INR_PCI_enable_error_LED
    }
    if (!INR_PCI_rx_descriptor_pool_dma_root[index]) {
        INR_LOG_debug (loglevel_err"error: zero INR_PCI_rx_descriptor_pool_dma_root[index]\n");
        INR_PCI_enable_error_LED
    }
    rx_descriptor_pool_free[index] += 1;//incement page usage counter
    return tmp_rx_desc;
}

//*****************************************************************************************************************
/**
*print version number
*@param ver version
*/
void
print_pci_version (uint32_t ver)
{
    INR_LOG_debug (loglevel_warn"Hardware version:%i  Date:%i.%i\n", 0xff & ver, (0xff00 & ver) >> 8, (0xffff0000 & ver) >> 16);

}

//*****************************************************************************************************************
/**
*reset Hardware
*/
void
INR_PCI_reset ()
{
    writel (1 << 26, (gBaseVirt0 + (4 * 0)));	//reset device
}

//*****************************************************************************************************************


/**
*Processing rx-descriptor-ring
*@param index RX-descriptor-ring-index
*/
uint16_t
INR_PCI_process_rx_descriptor_ring (uint8_t index)
{
    uint16_t count = 0; /**<count number of packets received to report to NAPI*/
    uint64_t hw_timestamp;
    uint32_t ringhead = INR_PCI_BAR0_read (INR_PCI_rx_descriptor_head_reg + (64 * index));
    if (RX_DBG_mod)INR_LOG_debug (loglevel_warn"Ring head0x%llx, expected ring head: 0x%llx",ringhead, (INR_PCI_rx_descriptor_current[index] * INR_PCI_rx_descriptor_length));
    INR_CHECK_fpga_read_val (ringhead, "process_rx_ring:INR_PCI_rx_descriptor_head_reg", 0); //print error message if readed values is 0xffffffff and PCIe interface out of sync
    uint16_t loopcount = 0; /**<count number of fragments received stop endless loop if FPGA reports wrong HEAD/TAIL pointer*/
    while (ringhead != (INR_PCI_rx_descriptor_current[index] * INR_PCI_rx_descriptor_length))	//read all available fragments
    {
        loopcount++;
        if (loopcount > INR_PCI_rx_max_loops)
            goto exit_rxloop;		//exit strategy if FPGA crashes
        volatile struct INR_PCI_rx_descriptor *RX_descriptor = INR_PCI_rx_descriptor_ring[INR_PCI_rx_descriptor_current[index]][index];	//get address of descriptor
        dma_sync_single_range_for_cpu (&globdev->dev, data_rx[INR_PCI_rx_descriptor_current[index]][index]->dma_root, data_rx[INR_PCI_rx_descriptor_current[index]][index]->offset, data_size_rx, DMA_FROM_DEVICE);	//sync memory of descriptor to CPU
        if (RX_DBG_mod)
            INR_LOG_debug (loglevel_warn"RX-Descriptor-dump RX_Current:%i DescrBase:0x%x Status:%x Length:%i Index:%i FirstPKG:%i Buffer:%llx DMA:%llx MEM:%llx offset:0x%i page:%llx, fragmentindex:%i HW_TS:%i\n", INR_PCI_rx_descriptor_current[index], INR_PCI_rx_descriptor_ring[INR_PCI_rx_descriptor_current[index]][index], RX_descriptor->Status, RX_descriptor->length, index, firstpkg[index], RX_descriptor->buffer, data_rx[INR_PCI_rx_descriptor_current[index]][index]->dma, data_rx[INR_PCI_rx_descriptor_current[index]][index]->data, data_rx[INR_PCI_rx_descriptor_current[index]][index]->offset, page_address (data_rx[INR_PCI_rx_descriptor_current[index]][index]->page), data_rx[INR_PCI_rx_descriptor_current[index]][index]->fragmentindex,INR_PCI_HW_timestamp);	//long debug print
        IfNotRuss if ((RX_descriptor->Errors & (0x1))) {	//handle descriptor error flag
            INR_LOG_debug (loglevel_err"error received fragment with error flag\n");
            INR_PCI_enable_error_LED
            dropmode[index] = 1;
        }
        IfNotRuss if ((RX_descriptor->Status & (0x1)) == 0) {	//hand bad status value
            INR_LOG_debug (loglevel_err"error received fragment with clear dd flag\n");
            INR_PCI_enable_error_LED
            dropmode[index] = 1;
        }
//##############################FIRST FRAGMENT handling
        if (firstpkg[index]) {	//first fragment of packet
            firstpkg[index] = 0;
            IfNotRuss if (RX_descriptor->length < ETH_HLEN) {	//handle to short framents
                INR_LOG_debug (loglevel_err"error received to short first fragment\n");
                INR_PCI_enable_error_LED
                dropmode[index] = 1;
                return 0;
            }
            if (zerocopy_rx) {	//if zerocopy alloc small SKB and copy just first part
                rx_skb[index] = netdev_alloc_skb (get_nwdev (index), ETH_HLEN + 2);
                if (!rx_skb[index]) {
                    INR_LOG_debug (loglevel_err"Cant alloc RX SKB\n");
                    INR_PCI_enable_error_LED
                    dropmode[index] = 1;
                    return 0;
                }
                rx_skb[index]->dev = get_nwdev (index);
                skb_reserve (rx_skb[index], 2);
                skb_put (rx_skb[index], ETH_HLEN);
                if(INR_PCI_HW_timestamp) {
                    memcpy (&hw_timestamp,data_rx[INR_PCI_rx_descriptor_current[index]]
                            [index]->data, sizeof(hw_timestamp));// copy hardware timestamp

                    memcpy (rx_skb[index]->data, data_rx[INR_PCI_rx_descriptor_current[index]]
                            [index]->data+sizeof(hw_timestamp), ETH_HLEN);

                } else {
                    memcpy (rx_skb[index]->data, data_rx[INR_PCI_rx_descriptor_current[index]]
                            [index]->data, ETH_HLEN);
                }
            }
            else {			//if no zerocopy alloc big skb and coppy whole fragment into
                rx_skb[index] = netdev_alloc_skb (get_nwdev (index), 1800 + 2);
                if (!rx_skb[index]) {
                    INR_LOG_debug (loglevel_err"Cant alloc RX SKB\n");
                    INR_PCI_enable_error_LED
                    dropmode[index] = 1;
                    return 0;
                }
                rx_skb[index]->dev = get_nwdev (index);
                skb_reserve (rx_skb[index], 2);
                skb_put (rx_skb[index], RX_descriptor->length-(sizeof(hw_timestamp)*INR_PCI_HW_timestamp));	//should everytime be true, but is not :/
                memcpy (rx_skb[index]->data, data_rx[INR_PCI_rx_descriptor_current[index]]
                        [index]->data+(sizeof(hw_timestamp)*INR_PCI_HW_timestamp), RX_descriptor->length-(sizeof(hw_timestamp)*INR_PCI_HW_timestamp));	//copy whole fragment into SKB
                if(INR_PCI_HW_timestamp) {
                    memcpy (&hw_timestamp,data_rx[INR_PCI_rx_descriptor_current[index]]
                            [index]->data, sizeof(hw_timestamp));// copy hardware timestamp

                }

            }
            //###########Timestamping
            if (INR_PCI_HW_timestamp) {
                struct skb_shared_hwtstamps *skbtimestamp = skb_hwtstamps(rx_skb[index]);
                memset(skbtimestamp, 0, sizeof(struct skb_shared_hwtstamps));
                if (RX_DBG_mod) INR_LOG_debug("HW timestamp:0x%lx, corrected value:0x%llx ns:0x%lli\n",hw_timestamp,INR_TIME_correct_HW_timestamp(hw_timestamp),ns_to_ktime(INR_TIME_correct_HW_timestamp(hw_timestamp)));
                skbtimestamp->hwtstamp=ns_to_ktime((u64)INR_TIME_correct_HW_timestamp(hw_timestamp)); //0: insert timestamp here...
                //skb_tstamp_rx(rx_skb[index],&skbtimestamp);
                __net_timestamp(rx_skb[index]);

            } else {
                //skb_rx_timestamp(rx_skb[index]);
            }
            //########################
            rx_skb[index]->ip_summed = CHECKSUM_UNNECESSARY;	//set checksumflag in skb
            rx_skb[index]->protocol = eth_type_trans (rx_skb[index], rx_skb[index]->dev);	//set ethertype in skb
            if (RX_descriptor->length-(sizeof(hw_timestamp)*INR_PCI_HW_timestamp) > ETH_HLEN)
                if (zerocopy_rx)	//if zerocopy append unhandled part of packet to the skb
                    skb_add_rx_frag (rx_skb[index], skb_shinfo (rx_skb[index])->nr_frags, data_rx[INR_PCI_rx_descriptor_current[index]]
                                     [index]->page, data_rx[INR_PCI_rx_descriptor_current[index]]
                                     [index]->offset + ETH_HLEN+(sizeof(hw_timestamp)*INR_PCI_HW_timestamp), RX_descriptor->length - ETH_HLEN-(sizeof(hw_timestamp)*INR_PCI_HW_timestamp), ALIGN (RX_descriptor->length - ETH_HLEN-(sizeof(hw_timestamp)*INR_PCI_HW_timestamp), L1_CACHE_BYTES));
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,4,200)
            atomic_inc (&data_rx[INR_PCI_rx_descriptor_current[index]][index]->page->_refcount);	//INCREMTN PAGECOUNT.. creazy voodoo
#else
            atomic_inc (&data_rx[INR_PCI_rx_descriptor_current[index]][index]->page->_count);	//INCREMTN PAGECOUNT.. creazy voodoo
#endif

        }
//##############################OTHER FRAGMENT handling
        else {			//some other fragment of packet
            if (zerocopy_rx) {
                IfNotRuss if (!rx_skb[index]) {
                    INR_LOG_debug (loglevel_err"rx skb nullpointer!\n");
                    INR_PCI_enable_error_LED
                }
                IfNotRuss if (!data_rx[INR_PCI_rx_descriptor_current[index]][index]->page) {
                    INR_LOG_debug (loglevel_err"rx page nullpointer!\n");
                    INR_PCI_enable_error_LED
                }
                IfNotRuss if (!RX_descriptor->length) {
                    INR_LOG_debug (loglevel_err"rx length nullpointer!\n");
                    INR_PCI_enable_error_LED
                }

                //if (data_rx[INR_PCI_rx_descriptor_current[index]][index]->fragmentindex == 0) {	//fragment are start of new page
                if(1) { //needed for deffeent buffersizes if timestamping is enabled
                    skb_add_rx_frag (rx_skb[index], skb_shinfo (rx_skb[index])->nr_frags, data_rx[INR_PCI_rx_descriptor_current[index]][index]->page,
                                     data_rx[INR_PCI_rx_descriptor_current[index]][index]->offset, RX_descriptor->length, ALIGN (RX_descriptor->length,
                                             L1_CACHE_BYTES));
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,4,200)
                    atomic_inc (&data_rx[INR_PCI_rx_descriptor_current[index]][index]->page->_refcount);	//INCREMT PAGECOUNT.. creazy voodoo
#else
                    atomic_inc (&data_rx[INR_PCI_rx_descriptor_current[index]][index]->page->_count);	//INCREMT PAGECOUNT.. creazy voodoo
#endif
                }
                else {			//dont add new page, only extend last frag (pages are sorted by rx-ring )
                    skb_shinfo (rx_skb[index])->frags[skb_shinfo (rx_skb[index])->nr_frags - 1].size += RX_descriptor->length;
                    rx_skb[index]->len += RX_descriptor->length;
                    rx_skb[index]->data_len += RX_descriptor->length;
                }
            }
            else {			//copy whole descriptor-data in skb

                if (rx_skb[index]->len < 1700)
                    memcpy (skb_put (rx_skb[index], RX_descriptor->length),	//should everytime true but isn not.. :/
                            data_rx[INR_PCI_rx_descriptor_current[index]]
                            [index]->data, RX_descriptor->length);
            }


        }
        if (RX_DBG_mod) {		//debug output
            uint8_t *tmpdata = data_rx[INR_PCI_rx_descriptor_current[index]][index]->data;
            uint16_t tmp;
            INR_LOG_debug (loglevel_warn"RX skb skb-len:%i offset:%i len:%i page:%llx ring:%i\n", rx_skb[index]->len,
                           data_rx[INR_PCI_rx_descriptor_current[index]][index]->offset, RX_descriptor->length,
                           data_rx[INR_PCI_rx_descriptor_current[index]][index]->data, index);
            printk ("RXdump:");
            for (tmp = 0; (tmp < RX_descriptor->length); tmp++) {//dump packet data
                if (tmp % 32 == 0)
                    printk ("\n");
                printk ("%x ", tmpdata[tmp]);
            }
            printk ("\n\n");
        }
        if (data_rx[INR_PCI_rx_descriptor_current[index]][index]->fragmentindex == (rx_descriptor_pool_length - 1)) {
            dma_unmap_page (&globdev->dev, data_rx[INR_PCI_rx_descriptor_current[index]][index]->dma_root, pagesize, DMA_FROM_DEVICE);	//if last fragment of page is used unmap page
            if (!zerocopy_rx)		//if not zerocopy free the memeory page because date is copied already
                __free_page (data_rx[INR_PCI_rx_descriptor_current[index]]
                             [index]->page);
        }
//########################LAST FRAGMENT handling
        if (RX_descriptor->Status & 0x2)	//end of paket or not?
        {
            struct INR_NW_priv *priv = netdev_priv (get_nwdev (index));
            firstpkg[index] = 1;
            count++;
            if (dropmode[index]) {	//if not drop...send
                priv->stats.rx_dropped++;
                kfree_skb (rx_skb[index]);
            }
            else if (rx_skb[index])
                if (rx_skb[index]->dev->flags & IFF_UP)	//check if interface is up
                    if (NET_RX_DROP == netif_receive_skb (rx_skb[index])) {
                        if (RX_DBG_mod)
                            INR_LOG_debug (loglevel_err"PKT dropped RX :/\n");
                        priv->stats.rx_dropped++;
                        //kfree_skb(rx_skb[index]);//free skb because nw-stack don''t do this now ;)
                    }
                    else {
                        priv->stats.rx_packets++;
                        priv->stats.rx_bytes += RX_descriptor->length;
                    }
            dropmode[index] = 0;	//reset dropmode
        }

        if (data_rx[INR_PCI_rx_descriptor_current[index]][index])
            kfree (data_rx[INR_PCI_rx_descriptor_current[index]][index]);	//free descriptor
        INR_PCI_alloc_new_rx_skb (INR_PCI_rx_descriptor_current[index], index);	//alloc new skb
        if (!data_rx[INR_PCI_rx_descriptor_current[index]][index]) {
            INR_LOG_debug (loglevel_err"error pci alloc zeropointer, rxprocess ring:%i\n", index);
            INR_PCI_enable_error_LED
            return 0;
        }
        RX_descriptor->buffer = data_rx[INR_PCI_rx_descriptor_current[index]][index]->dma;	// save memory address in descriptor
        RX_descriptor->Status &= ~(0x3);	//status zurücksetzen
        RX_descriptor->length = data_size_rx;
        if (INR_PCI_rx_descriptor_current[index] == (INR_PCI_rx_descriptor_ring_length - 1)) {	//update RX-rint Tailcounters
            INR_PCI_rx_descriptor_current[index] = 0;
        }
        else {
            INR_PCI_rx_descriptor_current[index]++;
        }
        if (lastbot[index] >= ((INR_PCI_rx_descriptor_ring_length - 1) * INR_PCI_rx_descriptor_length)) {
            INR_PCI_BAR0_write ((INR_PCI_rx_descriptor_ring_length - 1) * INR_PCI_rx_descriptor_length, INR_PCI_rx_descriptor_tail_reg + (64 * index));
            lastbot[index] = 0;
        }
        else {
            INR_PCI_BAR0_write (lastbot[index], INR_PCI_rx_descriptor_tail_reg + (64 * index));
            lastbot[index] += INR_PCI_rx_descriptor_length;
        }
    }
exit_rxloop:
    return count;
}

//*****************************************************************************************************************
/**
*main ISR
*@param irq irq
*@param dev_id PCI-device
*@param regs flags
*/
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,4,200)
static irqreturn_t
XPCIe_IRQHandler (int irq, void *dev_id)
#else
static irqreturn_t
XPCIe_IRQHandler (int irq, void *dev_id, struct pt_regs *regs)
#endif
{
    uint64_t intcause = INR_PCI_BAR0_read(INR_PCI_interrupt_cause_reg); //not implemented yet :/ -> read all rings  0xffff;//
    if (RX_DBG_mod)INR_LOG_debug(loglevel_warn"Int cause:0x%lx\n",INR_PCI_BAR0_read(INR_PCI_interrupt_cause_reg));

    uint8_t i = 0;
    for (i = 0; i < INR_PCI_rx_descriptor_ring_count; i++)
        if ((get_nwdev (i)->flags) & IFF_UP)
            if (intcause & (1 << i)) {
                if (NAPI_enable) {	//enable polling if NAPI is used
                    INR_NAPI_schedule (i);
                }
                else
                    INR_PCI_process_rx_descriptor_ring (i); //handle descriptor ring self if no NAPI used
            }
    if(intcause&(1<<16)) {
        if (RX_DBG_mod) {
            INR_LOG_debug(loglevel_warn"MMI interrupt\n");
        }
        INR_MMI_interrupt();
        //INR_TIME_TX_transmit_interrupt();
    }

//    }
    if(intcause&(1<<17)) {
        if (TX_DBG_mod) {
            INR_LOG_debug(loglevel_warn"old TX interrupt\n");
        }

    }
    IfNotRuss if(intcause>=(1<<18)) {
        if (RX_DBG_mod)INR_LOG_debug(loglevel_warn"unknown Interrupt\n");
        if (RX_DBG_mod)INR_PCI_enable_error_LED
        }
    INR_PCI_BAR0_write(intcause,INR_PCI_interrupt_cause_reg);//writing back int register
    return IRQ_HANDLED;
}

//*****************************************************************************************************************
/**
*Enable Interrupts
*@param *dev PCI-device
*/
int
INR_int_enable (struct pci_dev *dev)
{
    INR_LOG_debug (loglevel_info"ISR Setup..starting\n");
    int intcount = pci_enable_msi_range (dev, 1, 32);
    INR_LOG_debug (loglevel_info"theoretical int count:%i\n",pci_msi_vec_count(dev));
//    int intcount = pci_alloc_irq_vectors(dev,1,255,PCI_IRQ_ALL_TYPES);
    INR_LOG_debug (loglevel_info"ISR range requested..\n");
    if (intcount > 0) {
        INR_STATUS_set (INR_STATUS_MSI);
        INR_LOG_debug (loglevel_info"%i MSIs enabled\n", intcount);	//enable MSI
        gIrq = dev->irq;		//get irq from device
        INR_LOG_debug (loglevel_info" Init: Device IRQ: %lli\n", gIrq);
        INR_LOG_debug (loglevel_info"ISR Setup..\n");
        if (0 > request_irq (gIrq, XPCIe_IRQHandler, 0, "INRBusmaster", dev))	//SA_INTERRUPT
        {
            INR_LOG_debug (loglevel_info"Init: Unable to allocate IRQ\n");
            return (1);
        }
    }
    else {
        INR_LOG_debug (loglevel_err"MSI error\n");
        return 1;
    }
}
//*****************************************************************************************************************
/**
*allocate new rx skb and map meory to DMA
*@param current descriptor index in rx-ring
*@param index index of RX-ring
*/
void
INR_PCI_alloc_new_rx_skb (uint16_t current_descriptor, uint16_t ringindex)
{
    data_rx[current_descriptor][ringindex] = INR_PCI_get_new_rx_descriptor_ring_entry (ringindex);
}
//*****************************************************************************************************************
/**
*init RX descriptor ring
*@param *dev pci device
*@param index index of RX-ring
*/
void
INR_RX_RING_init (struct pci_dev *dev, uint8_t index)
{
    INR_LOG_debug (loglevel_info"Init RX-Ring %i\n", index);
    INR_PCI_rx_descriptor_base[index] = pci_alloc_consistent (dev, INR_PCI_rx_descriptor_ring_length * INR_PCI_rx_descriptor_length, (dma_addr_t)&INR_RX_Ring[index]);	//allocate TX-Ring in DMA
    INR_LOG_debug (loglevel_info"RX-Ring: cpu_adress:0x%llx  dma-adress:0x%llx\n", INR_PCI_rx_descriptor_base[index], INR_RX_Ring[index]);
    uint32_t tmpaddr_h, tmpaddr_l;
    tmpaddr_l = INR_RX_Ring[index] & 0xffffffff;
    tmpaddr_h = (INR_RX_Ring[index] >> 32) & 0xffffffff;
    INR_LOG_debug (loglevel_info"addh:%x addrl:%x\n", tmpaddr_h, tmpaddr_l);
    if (!INR_PCI_rx_descriptor_base[index])
        INR_LOG_debug (loglevel_info"error cant alloc rx-ring %i\n", index);
    INR_STATUS_set (INR_STATUS_RX_RING);
    INR_PCI_BAR0_write (0, INR_PCI_rx_descriptor_tail_reg + (64 * index));	//set bottom-pointer =0
    INR_PCI_BAR0_write64 (INR_RX_Ring[index], INR_PCI_rx_descriptor_base_address_reg + (64 * index));
    INR_LOG_debug (loglevel_info"write RX_Ring_address %llx to 0x%llx\n",INR_PCI_BAR0_read64 (INR_PCI_rx_descriptor_base_address_reg + (64 * index)), INR_PCI_rx_descriptor_base_address_reg + (64 * index));
    uint64_t i = 0;
    INR_PCI_rx_descriptor_ring[0][index] = INR_PCI_rx_descriptor_base[index];
    for (i = 1; i < INR_PCI_rx_descriptor_ring_length; i++) {	//fill descriptor-ring with dma-addresses
        INR_PCI_rx_descriptor_ring[i][index] = INR_PCI_rx_descriptor_ring[i - 1][index] + INR_PCI_rx_descriptor_length;
    }
    INR_LOG_debug (loglevel_info"alloc RX-SKBs and fill them width descriptors\n");
    for (i = 0; i < INR_PCI_rx_descriptor_ring_length; i++) {
        INR_PCI_alloc_new_rx_skb (i, index);	//alloc skb and map dma
        if (!data_rx[i][index]) {
            INR_LOG_debug (loglevel_err"error pci alloc zeropointer\n");
            return 0;
        }
        struct INR_PCI_rx_descriptor *RX_descriptor = INR_PCI_rx_descriptor_ring[i][index];
        RX_descriptor->buffer = data_rx[i][index]->dma;	// save memory address in descriptor
        RX_descriptor->length = data_size_rx;
    }
    INR_PCI_rx_descriptor_current[index] = 0;
    INR_PCI_BAR0_write ((INR_PCI_rx_descriptor_ring_length - 1) * INR_PCI_rx_descriptor_length, INR_PCI_rx_descriptor_tail_reg + (64 * index));	//set RX Tail pointer
    firstpkg[index] = 1;
    INR_PCI_BAR0_write (INR_PCI_rx_descriptor_length * INR_PCI_rx_descriptor_ring_length, INR_PCI_rx_descriptor_length_reg + (64 * index));	//write length of descriptorring
}
//*****************************************************************************************************************
/**
*debug function to sport FPGA cluttering
*/
void
INR_PCI_ringtailtest (void *nix)
{
    DECLARE_WAITQUEUE (wait3, current);
    INR_LOG_debug (loglevel_info"reading RDT_0 and RDT_1...\n");
    allow_signal (SIGKILL);
    add_wait_queue (&INR_tailtest, &wait3);
    while (1) {
        set_current_state (TASK_INTERRUPTIBLE);
        if (signal_pending (current))
            break;			//exit on thermination
        if (INR_PCI_BAR0_read (INR_PCI_rx_descriptor_tail_reg) % INR_PCI_rx_descriptor_length)
            INR_LOG_debug (loglevel_err"Error: RDT_0 is %x :/\n", INR_PCI_BAR0_read (INR_PCI_rx_descriptor_tail_reg));
        if (INR_PCI_BAR0_read (INR_PCI_rx_descriptor_tail_reg + 64) % INR_PCI_rx_descriptor_length)
            INR_LOG_debug (loglevel_err"Error: RDT_1 is %x :/\n", INR_PCI_BAR0_read (INR_PCI_rx_descriptor_tail_reg + 64));
    }
    remove_wait_queue (&INR_tailtest, &wait3);
}

//*****************************************************************************************************************
/**
*tread function which automatically preallocate RX-pages
*/
int
INR_PCI_rx_pageallocator (void *nix)// contains empirical thesholds..but it works ;)
{
    DECLARE_WAITQUEUE (wait2, current);
    allow_signal (SIGKILL);
    add_wait_queue (&INR_PCI_rx_pageallocator_waittingqueu, &wait2);
    while (1) {
        set_current_state (TASK_INTERRUPTIBLE);
        schedule ();
        if (signal_pending (current))
            break;			//exit on thermination
        if (page_prealloc_current < INR_PCI_page_prealloc_count) {
            down (&INR_PCI_rx_pageallocator_sem);
            //INR_LOG_debug("Pageallocator\n");
            if (page_prealloc_current < (INR_PCI_page_prealloc_count / 4)) {
                INR_LOG_debug (loglevel_info"Refill all pages\n");
                while (page_prealloc_current < (INR_PCI_page_prealloc_count - 1)) {
                    set_current_state (TASK_UNINTERRUPTIBLE);
                    INR_PCI_rx_preallocated_page[page_prealloc_current++] = alloc_page (GFP_ATOMIC);	//if less then half fill whole buffer
                    set_current_state (TASK_INTERRUPTIBLE);
                }
            }
            else if (page_prealloc_current < (INR_PCI_page_prealloc_count - 5)) {
                uint64_t refill = (INR_PCI_page_prealloc_count * 2) / page_prealloc_current;
                if (page_prealloc_current + refill > INR_PCI_page_prealloc_count - 1)
                    refill = INR_PCI_page_prealloc_count - 2 - page_prealloc_current;
                set_current_state (TASK_UNINTERRUPTIBLE);
                while (refill--)
                    INR_PCI_rx_preallocated_page[page_prealloc_current++] = alloc_page (GFP_ATOMIC);
                set_current_state (TASK_INTERRUPTIBLE);
            }
            up (&INR_PCI_rx_pageallocator_sem);
        }
    }
    set_current_state (TASK_RUNNING);
    remove_wait_queue (&INR_PCI_rx_pageallocator_waittingqueu, &wait2);
    return 0;
}

//*****************************************************************************************************************
/**
*push unmap old TX buffer
*@param buffer pointer to data
*@param length length of buffer
*/
void
INR_TX_unmap_d (void *nix) //deactivated
{
    DECLARE_WAITQUEUE (wait, current);
    allow_signal (SIGKILL);
    add_wait_queue (&INR_PCI_TX_unmapd_waittingqueu, &wait);
    while (1) {
        INR_LOG_debug (loglevel_info"möp!\n");
        set_current_state (TASK_INTERRUPTIBLE);
        schedule ();
        if (signal_pending (current))
            break;			//exit on thermination
        uint64_t tmp = INR_PCI_BAR0_read (INR_PCI_tx_descriptor_head_reg) / INR_PCI_tx_descriptor_length;	//get possition of FPGA
        INR_CHECK_fpga_read_val (tmp, "unmapd:INR_PCI_tx_descriptor_head_reg", 0);
        while (tmp != INR_PCI_tx_descriptor_unmap_current) {
            if (INR_PCI_tx_descriptor_unmap_current == (INR_PCI_tx_descriptor_ring_length - 1)) {
                INR_PCI_tx_descriptor_unmap_current = 0;
            }
            else {
                INR_PCI_tx_descriptor_unmap_current++;
            }
            struct INR_PCI_tx_descriptor *TX_descriptor = INR_PCI_tx_descriptor_ring[INR_PCI_tx_descriptor_unmap_current];	//create temp. TDESC-structure
            if ((TX_descriptor->STA & 0x1) == 0)	//look for DD-Bit
            {
                INR_LOG_debug (loglevel_err"Unmapd-error descriptor not free\n");   //descriptor is not free, abort
            }
            else if (TX_descriptor->buffer) {	//if there is an address stored, first unmap before override
                dma_unmap_single (&globdev->dev, TX_descriptor->buffer, TX_descriptor->length, DMA_TO_DEVICE);	//unmap DMA
                TX_descriptor->buffer = 0;	//buffer leeren
            }
        }
    }
    set_current_state (TASK_RUNNING);
    remove_wait_queue (&INR_PCI_TX_unmapd_waittingqueu, &wait);
    return 0;
}

//*****************************************************************************************************************
/**
*init TX descriptor ring
*@param *dev pci device
*/
void
INR_TX_RING_init (struct pci_dev *dev)
{
    INR_LOG_debug (loglevel_info"Init TX-Ring\n");
    INR_PCI_tx_descriptor_base = pci_alloc_consistent (dev, INR_PCI_tx_descriptor_ring_length * INR_PCI_tx_descriptor_length, (dma_addr_t)&INR_TX_Ring);	//allocate TX-Ring in DMA
    INR_LOG_debug (loglevel_info"TX-Ring: cpu_adress:0x%llx  dma-adress:0x%llx\n", INR_PCI_tx_descriptor_base, INR_TX_Ring);
    if (!INR_PCI_tx_descriptor_base)
        INR_LOG_debug (loglevel_err"error cant alloc tx-ring\n");
    INR_STATUS_set (INR_STATUS_TX_RING);
    INR_PCI_BAR0_write (0, INR_PCI_tx_descriptor_tail_reg);	//set bottom-pointer =0
    INR_LOG_debug (loglevel_info"write TX_Ring_bottom %i to 0x%x\n", INR_PCI_BAR0_read (INR_PCI_tx_descriptor_tail_reg), INR_PCI_tx_descriptor_tail_reg);
    INR_PCI_BAR0_write (INR_PCI_tx_descriptor_length * INR_PCI_tx_descriptor_ring_length, INR_PCI_tx_descriptor_length_reg);	//write length of descriptorring
    INR_LOG_debug (loglevel_info"write TX_Ring_length %i to 0x%x\n", INR_PCI_BAR0_read (INR_PCI_tx_descriptor_length_reg), INR_PCI_tx_descriptor_length_reg);
    INR_PCI_BAR0_write (INR_TX_Ring & 0xffffffff, INR_PCI_tx_descriptor_base_address_reg);	//write TX-Ring-address to bar0
    INR_PCI_BAR0_write (INR_TX_Ring >> 32, INR_PCI_tx_descriptor_base_address_reg_h);	//write TX-Ring-address to bar0
    INR_LOG_debug (loglevel_info"write TX_Ring_address %x to 0x%llx\n", (((uint64_t) INR_PCI_tx_descriptor_base_address_reg_h) << 32)
                   | INR_PCI_BAR0_read (INR_PCI_tx_descriptor_base_address_reg), INR_PCI_tx_descriptor_base_address_reg);
    uint64_t i = 0;
    INR_LOG_debug (loglevel_info"fill ring with addresses\n");
    INR_PCI_tx_descriptor_ring[0] = INR_PCI_tx_descriptor_base;
    for (i = 1; i < INR_PCI_tx_descriptor_ring_length; i++) {	//fill descriptor-ring with dma-addresses
        INR_PCI_tx_descriptor_ring[i] = INR_PCI_tx_descriptor_ring[i - 1] + INR_PCI_tx_descriptor_length;

    }
    for (i = 0; i < INR_PCI_tx_descriptor_ring_length; i++) {
        volatile struct INR_PCI_tx_descriptor *TX_descriptor = INR_PCI_tx_descriptor_ring[i];
        TX_descriptor->STA = 0x1;	// mark every descriptor as free
        TX_descriptor->buffer = 0;	//clear buffer
    }
//enable TX unmap deamon(thread)
    //kthread_run (&INR_TX_unmap_d, NULL, "INR_TX_unmap_d");
}

//*****************************************************************************************************************
/**
*print function for tx descriptor
*@param *dev device
*@param id id of descriptor
*/
void
INR_PRINT_DESC_status (struct pci_dev *dev, uint64_t id)
{
    struct INR_PCI_tx_descriptor *TX_descriptor = INR_PCI_tx_descriptor_ring[id];	//create temp. TDESC-structure
    INR_LOG_debug (loglevel_info"Descriptor %i: Buffer:0x%x\n", id, TX_descriptor->buffer);
    INR_LOG_debug (loglevel_info"  Datalength:0x%x\n", TX_descriptor->length);
    INR_LOG_debug (loglevel_info"  CSO:0x%x\n", TX_descriptor->CSO);
    INR_LOG_debug (loglevel_info"  CMD:0x%x\n", TX_descriptor->CMD);
    INR_LOG_debug (loglevel_info"  STA:0x%x\n", TX_descriptor->STA);
    INR_LOG_debug (loglevel_info"  Rsvd:0x%x\n", TX_descriptor->Rsvd);
    INR_LOG_debug (loglevel_info"  CSS:0x%x\n", TX_descriptor->CSS);
    INR_LOG_debug (loglevel_info"  VLAN:0x%x\n", TX_descriptor->VLAN);
}

//*****************************************************************************************************************
/**
*unmap all descriptores
*@param *dev device
*/
void
INR_UNMAP_all (struct pci_dev *dev)
{
    uint64_t i = 0;
    INR_LOG_debug (loglevel_info"unmap all descriptors\n");
    for (i = 0; i < INR_PCI_tx_descriptor_ring_length; i++) {
        struct INR_PCI_tx_descriptor *TX_descriptor = (struct INR_PCI_tx_descriptor*)INR_PCI_tx_descriptor_ring[i];	//create temp. TDESC-structure
        if (TX_descriptor->buffer) {	//if there is an address stored, first unmap before override
            dma_unmap_single (&dev->dev, TX_descriptor->buffer, TX_descriptor->length, DMA_TO_DEVICE);	//unmap DMA
        }
    }
    for (i = 0; i < INR_PCI_rx_descriptor_ring_length; i++) {
        struct INR_PCI_rx_descriptor *RX_descriptor = (struct INR_PCI_rx_descriptor*)INR_PCI_rx_descriptor_ring[i];	//create temp. TDESC-structure
        if (RX_descriptor->buffer) {	//if there is an address stored, first unmap before override
            dma_unmap_single (&dev->dev, RX_descriptor->buffer, RX_descriptor->length, DMA_FROM_DEVICE);	//unmap DMA
        }
    }
}
//*****************************************************************************************************************
/**
*calculate the distance between two pointers
*@param tail tailpointer
*@raram head headpointer
*@param max maximum ring value
*/
uint16_t
INR_PCI_pointerdistance (uint16_t tail, uint16_t head, uint16_t max)
{
    if (head > tail) {
        return head - tail;
    }
    else {
        return max - tail + head;
    }
}

//*****************************************************************************************************************

/**
*push new packet into TX descriptor ring
*@param *datalength length of data in memory
*@param *dma_addr dma-address of packet buffer
*@param eof last_packet?
*@param port trustnode output-port 0..15
*@param ll local loop (send to cpu)
*/
int
INR_TX_push (struct net_device *nwdev,struct sk_buff *skb, uint8_t * data, uint16_t datalength, uint8_t eof, uint8_t port, uint8_t ll, uint8_t paged, uint16_t fragcount,uint8_t time_queue)
{
    uint32_t tmp66 = tx_head_backup;
    uint8_t error = 0;
    uint64_t head, tail;
    //if (TX_DBG_mod)INR_LOG_debug("time_queue:%i\n",time_queue);
    if(get_HW_user_feature(HW_feature_frame_injection)==0)time_queue=0;
    if (INR_STATUS_get (INR_STATUS_DRV_INIT_done) == 0) { //check if device ready after startup
        return 1;
        if (TX_DBG_mod)INR_LOG_debug("drop tx pkg, driver not init\n");
    }
    preempt_disable ();
    if (fragcount > INR_PCI_TX_maxfragments)
    {   return 1;			// to much fragments, drop
        if (TX_DBG_mod)INR_LOG_debug("drop tx pkg, fragcount to high:%i\n",fragcount);
    }
    if (txsemophore) {		//INR_LOG_debug("error TX semaphore hit\n");
        return 1;
    }
    txsemophore = 1;

    head = INR_PCI_BAR0_read (INR_PCI_tx_descriptor_head_reg) / INR_PCI_tx_descriptor_length;
    tail = INR_PCI_BAR0_read (INR_PCI_tx_descriptor_tail_reg) / INR_PCI_tx_descriptor_length;
    IfNotRuss if ((INR_PCI_pointerdistance (tail, head, INR_PCI_tx_descriptor_ring_length)) <= fragcount + 5) {//no free space in TX ring
        txsemophore = 0;
        if (TX_DBG_mod)INR_LOG_debug("drop tx pkg, pointer distance to small:%i\n",INR_PCI_pointerdistance (tail, head, INR_PCI_tx_descriptor_ring_length));
        return 1;
    }

    if (TX_DBG_mod)// check if FPGA have forgotten a TX descriptor
        while (tx_head_backup != head) {
            volatile struct INR_PCI_tx_descriptor *TX_descriptor_headbackup = INR_PCI_tx_descriptor_ring[tx_head_backup];
            if (!TX_descriptor_headbackup->STA) {
                INR_LOG_debug (loglevel_err"error: found unprocessed descriptor: tail:%i, head:%i, position:%i, headbackup:%i\n", tail, head, tx_head_backup, tmp66);
                INR_PCI_enable_error_LED
                error = 1;
            }
            if (tx_head_backup == (INR_PCI_tx_descriptor_ring_length - 1)) {
                tx_head_backup = 0;
            }
            else {
                tx_head_backup++;
            }
        }

    tx_head_backup = head;
    if (error) {//dump ring, if error spotted
        uint32_t k = 0;
        printk (loglevel_info"dumping ring:\n");
        while (k < INR_PCI_tx_descriptor_ring_length) {
            volatile struct INR_PCI_tx_descriptor *TX_descriptor_tmp = INR_PCI_tx_descriptor_ring[k];
            INR_LOG_debug (loglevel_info"id:%ibuffer:0x%x, len:%i cso:0x%x, cmd:0x%x, sta:0x%x, rsvd:0x%x, css:0x%x, vlan:0x%x\n", k, TX_descriptor_tmp->buffer,
                           TX_descriptor_tmp->length, TX_descriptor_tmp->CSO, TX_descriptor_tmp->CMD, TX_descriptor_tmp->STA, TX_descriptor_tmp->Rsvd,
                           TX_descriptor_tmp->CSS, TX_descriptor_tmp->VLAN);
            k++;
        }
    }
    volatile struct INR_PCI_tx_descriptor *TX_descriptor = INR_PCI_tx_descriptor_ring[INR_PCI_tx_descriptor_current];	//create temp. TDESC-structure
    /*if ((TX_descriptor->STA & 0x1) == 0) {
        INR_PCI_BAR0_write ((head << 16) | (0xffff & tail), 0x7004);
    }*/
    uint64_t dma_addr = dma_map_single (&globdev->dev, data, datalength, DMA_TO_DEVICE);	//map packet  to DMA
    if (dma_mapping_error (&globdev->dev, dma_addr)) {
        INR_LOG_debug (loglevel_err"TX dma mapping error! current:%llu \n", INR_PCI_tx_descriptor_current);
        INR_PCI_enable_error_LED
    }
    dma_sync_single_for_device (&globdev->dev, dma_addr, datalength, DMA_TO_DEVICE);
    if (TX_DBG_mod) {//dump packet content
        uint16_t tmp;
        INR_LOG_debug (loglevel_info"TX-packet len:%i eop:%i port:%i ll:%i t_queue:%i\n", datalength, eof, port,ll,time_queue);
        printk (loglevel_info"TXdump:");
        for (tmp = 0; tmp < datalength; tmp++) {
            if ((tmp % 40) == 0)
                printk ("\n");
            printk ("%x ", data[tmp]);
        }
        printk ("\n\n");
    }
    if (TX_descriptor->buffer)	//if buffer was used before
    {

        dma_unmap_single (&globdev->dev, TX_descriptor->buffer, TX_descriptor->length, DMA_TO_DEVICE);	//unmap DMA
        if (data_tx[INR_PCI_tx_descriptor_current].eop)
            if (data_tx[INR_PCI_tx_descriptor_current].skb)

                if(data_tx[INR_PCI_tx_descriptor_current].wait_for_timestamp==0) kfree_skb (data_tx[INR_PCI_tx_descriptor_current].skb);	//if last fragment of skb->free skb

        if (!data_tx[INR_PCI_tx_descriptor_current].paged)
            if (data_tx[INR_PCI_tx_descriptor_current].data)
                kfree (data_tx[INR_PCI_tx_descriptor_current].data);	//if not paged free kmalloc mem
    }

    TX_descriptor->buffer = cpu_to_le64 (dma_addr);	//store address of packet in descriptor-ring
    TX_descriptor->length = datalength;
    TX_descriptor->CSO = 0xff & 0x00;
    TX_descriptor->CMD = 0xff & ((1 & eof) | ((1 & ll) << 7));	//send eof and to-cpu-bit
    TX_descriptor->STA = 0xf & 0x0;
    TX_descriptor->Rsvd = 0xf & port;	//triger output-port
    TX_descriptor->CSS = 0xff & 0x00;
    TX_descriptor->VLAN = 0xffff & time_queue;//maping of TAS processoer queue feature to vlan field
    data_tx[INR_PCI_tx_descriptor_current].nwdev = nwdev;
    data_tx[INR_PCI_tx_descriptor_current].skb = skb;
    if (skb_shinfo(skb)->tx_flags& SKBTX_IN_PROGRESS) data_tx[INR_PCI_tx_descriptor_current].wait_for_timestamp=1;
    else data_tx[INR_PCI_tx_descriptor_current].wait_for_timestamp=0;
    data_tx[INR_PCI_tx_descriptor_current].length = datalength;
    data_tx[INR_PCI_tx_descriptor_current].paged = 1 & paged;
    data_tx[INR_PCI_tx_descriptor_current].eop = 1 & eof;
    data_tx[INR_PCI_tx_descriptor_current].data = data;
    data_tx[INR_PCI_tx_descriptor_current].dma = dma_addr;
    uint64_t descriptor_current_tmp = INR_PCI_tx_descriptor_current;	//protect curretn value
    if (INR_PCI_tx_descriptor_current == (INR_PCI_tx_descriptor_ring_length - 1)) {
        INR_PCI_tx_descriptor_current = 0;
    }
    else {
        INR_PCI_tx_descriptor_current++;
    }				//increment INR_PCI_tx_descriptor_current
    if (eof) {
        INR_PCI_BAR0_write ((INR_PCI_tx_descriptor_length * (INR_PCI_tx_descriptor_current)), INR_PCI_tx_descriptor_tail_reg);	//if last fragment, update tailpointer
        descriptor_current_lastwritten = INR_PCI_tx_descriptor_current;
    }//store address-offset of this last written descriptor in bar0
    txsemophore = 0;
    preempt_enable ();
    return 0;
}

//*****************************************************************************************************************
/**
*probe dma-mask
*@param *dev PCI-device
*/
void
INR_PROBE_dma (struct pci_dev *dev)
{
    INR_LOG_debug (loglevel_info"probing DMA_BIT_MASK\n");
    uint64_t j = 64;
    for (j = 64; j > 0; j--) {
        INR_LOG_debug ("%llu ", j);
        if (0 == dma_set_mask (&dev->dev, DMA_BIT_MASK (j)))
            break;
    }
    if (j)
        INR_LOG_debug (loglevel_info"set dma-mask to %llu\n", j);
    else
        INR_LOG_debug (loglevel_err"fail to set DMA-Mask!\n");

    INR_LOG_debug (loglevel_info"probing DMA_BIT_MASK\n");

    for (j = 64; j > 0; j--) {
        INR_LOG_debug (loglevel_info"%i ", j);
        if (0 == dma_set_coherent_mask (&dev->dev, DMA_BIT_MASK (j)))
            break;
    }
    if (j)
        INR_LOG_debug (loglevel_info"dma_set_coherent_mask to %llu\n", j);
    else
        INR_LOG_debug (loglevel_err"fail to set DMA_coherent-Mask!\n");

    INR_LOG_debug (loglevel_info"probing DMA_BIT_MASK\n");
    for (j = 64; j > 0; j--) {
        INR_LOG_debug ("%i ", j);
        if (0 == pci_set_dma_mask (dev, DMA_BIT_MASK (j)))
            break;
    }
    if (j)
        INR_LOG_debug (loglevel_info"set pci_set_dma_mask to %llu\n", j);
    else
        INR_LOG_debug (loglevel_err"fail to set PCI-DMA-Mask!\n");
}

//*****************************************************************************************************************
/**
*remove driver
*@param *dev PCI-device
*/
void
INR_remove_drv (struct pci_dev *dev)
{
    INR_UNMAP_all (dev);
    if (INR_STATUS_get (INR_STATUS_BUSMASTER))
        pci_clear_master (dev);	//enable Master
    if (INR_STATUS_get (INR_STATUS_BAR0))
        release_mem_region (gBaseHdwr0, REGISTER_SIZE);	//release bar0
    if (INR_STATUS_get (INR_STATUS_BAR1))
        release_mem_region (gBaseHdwr1, REGISTER_SIZE);	//release bar1
    if (INR_STATUS_get (INR_STATUS_TX_RING))
        pci_free_consistent (dev, INR_PCI_tx_descriptor_ring_length * INR_PCI_tx_descriptor_length,(void*)(uintptr_t) INR_PCI_tx_descriptor_base, INR_TX_Ring);	//release DMA-mem
    if (INR_STATUS_get (INR_STATUS_INT1))
        free_irq (gIrq, dev);	//release interrupt
    if (INR_STATUS_get (INR_STATUS_MSI))
        pci_disable_msi (dev);
    if (INR_STATUS_get (INR_STATUS_DEVICE))
        pci_disable_device (dev);	//disable device
    INR_LOG_debug (loglevel_info"device disabled\n\n");
}

//*****************************************************************************************************************
/**
*init driver
*@param *dev PCI-device
*/
int
INR_init_drv (struct pci_dev *dev)
{
    globdev = dev;
    pagesize = PAGE_SIZE;		//getpagesize();//get pagesze
    INR_LOG_debug (loglevel_info"Pagesize:%lli\n", pagesize);
    INR_LOG_debug (loglevel_info"Zerocopy: TX:%i RX:%i\n", zerocopy_tx, zerocopy_rx);
    INR_LOG_debug (loglevel_info"DEBUGMOD: TX:%i RX:%i\n", TX_DBG_mod, RX_DBG_mod);
    INR_LOG_debug (loglevel_info"AutorepeatonTXdrop: %i\n", INR_NW_repeatonbusy);
    rx_descriptor_pool_length = (pagesize / data_size_rx);	//count of descriptor in one page , pool is empty at start
    uint64_t i = 0;
    for (i = 0; i < INR_PCI_rx_descriptor_ring_count; i++)
        rx_descriptor_pool_free[i] = rx_descriptor_pool_length;
    gBaseHdwr0 = pci_resource_start (dev, 0);
    // Print Base Address to kernel log
    INR_LOG_debug (loglevel_info"Init: Base hw val 0x%lx\n", (unsigned int) gBaseHdwr0);   // Get the Base Address Length
    gBaseLen0 = pci_resource_len (dev, 0);    // Print the Base Address Length to Kernel Log
    INR_LOG_debug (loglevel_info"Init: Base hw len %i\n", (unsigned int) gBaseLen0);
    gBaseVirt0 = pci_iomap (dev, 0, 0);    //gBaseVirt = ioremap (gBaseHdwr, gBaseLen);  //prepare bar0 for access
    INR_LOG_debug (loglevel_info"Init: Bar0 mapped\n");
    if (!gBaseVirt0) {
        INR_LOG_debug (loglevel_err"Init: Could not remap memory.\n");
        return (1);
    }

    INR_LOG_debug (loglevel_info"Init: Base hw val 0x%lx\n", (unsigned int) gBaseHdwr1);   // Get the Base Address Length
    gBaseLen1 = pci_resource_len (dev, 1);    // Print the Base Address Length to Kernel Log
    INR_LOG_debug (loglevel_info"Init: Base hw len %i\n", (unsigned int) gBaseLen1);
    gBaseVirt1 = pci_iomap (dev, 1, 0);    //gBaseVirt = ioremap (gBaseHdwr, gBaseLen);  //prepare bar1 for access
    INR_LOG_debug (loglevel_info"Init: Bar1 mapped :0x%lx\n",gBaseVirt1);
    if (!gBaseVirt1) {
        INR_LOG_debug (loglevel_err"Init: Could not remap memory.\n");
        return (1);
    }

    if (request_mem_region (gBaseHdwr1, REGISTER_SIZE, "INR_mem"))
        INR_STATUS_set (INR_STATUS_BAR1);

    INR_LOG_debug (loglevel_info"Init: mem region checked\n");
    pci_set_master (dev);		//enable Master
    INR_LOG_debug (loglevel_info"Init: device mastered\n");
    INR_STATUS_set (INR_STATUS_BUSMASTER);
    INR_PROBE_dma (dev);
    //INR_TIME_set_bar1_base(gBaseVirt1);
    INR_LOG_debug (loglevel_info"Init: try to get dma acces\n");    // Try to gain exclusive control of memory
    if (request_mem_region (gBaseHdwr0, REGISTER_SIZE, "INR_mem"))
        INR_STATUS_set (INR_STATUS_BAR0);
    INR_LOG_debug (loglevel_info"Init: dma mem requested\n");

    if(C_SUB_ADDR_COMMON_ADDR_MAP_REV)INR_PCI_HW_timestamp=(INR_PCI_BAR1_read((C_BASE_ADDR_COMMON_LOWER<<8)+C_SUB_ADDR_COMMON_ADDR_MAP_REV)>>31)&1;
    INR_LOG_debug (loglevel_info"Hardware timestamping:%i.\n",INR_PCI_HW_timestamp);
#ifdef C_SUB_ADDR_COMMON_ADDR_MAP_REV
    HW_addr_map_revision=INR_PCI_BAR1_read((C_BASE_ADDR_COMMON_LOWER<<8)+C_SUB_ADDR_COMMON_ADDR_MAP_REV);
#endif
    INR_LOG_debug("HW address map revision:0x%llx\n",HW_addr_map_revision);
#ifdef C_SUB_ADDR_COMMON_FEATURES_USER
    HW_features_user=INR_PCI_BAR1_read((C_BASE_ADDR_COMMON_LOWER<<8)+C_SUB_ADDR_COMMON_FEATURES_USER);
#endif
    if((HW_features_user==0xffffffff)||(HW_features_user==0xEEEEEEEE))HW_features_user=0;
    INR_LOG_debug("HW user features:0x%llx\n",HW_features_user);
    if((HW_addr_map_revision==0xffffffff)||(HW_addr_map_revision==0xEEEEEEEE))HW_addr_map_revision=0;
#ifdef C_SUB_ADDR_NET_ENABLE
    if(HW_addr_map_revision>=5)INR_TIME_set_enable(1);//enable timing interrupt only if HW version support addresses
#endif
    if (INR_int_enable (dev))
        return 1;
    else
        INR_STATUS_set (INR_STATUS_INT1);
    kthread_run (&INR_PCI_rx_pageallocator, NULL, "INR_rx_pageallocator");
    wake_up_interruptible (&INR_PCI_rx_pageallocator_waittingqueu);	//wakeop preallocator
    INR_LOG_debug (loglevel_info"Page preallocator is running.\n");
    INR_TX_RING_init (dev);
    for (i = 0; i < INR_PCI_rx_descriptor_ring_count; i++)
        INR_RX_RING_init (dev, i);	//init all RX-Rings
    if (NAPI_enable)
        for (i = 0; i < INR_PCI_rx_descriptor_ring_count; i++)
            INR_NAPI_init (i);
    INR_LOG_debug (loglevel_info"Hardware setup done\n");
    if(HW_addr_map_revision>=5)INR_MMI_interrupt_start((uint64_t)(uintptr_t)gBaseVirt1);
    INR_STATUS_set (INR_STATUS_HW_RUNNING);
    INR_PCI_disable_error_LED
    if(HW_addr_map_revision>=5)INR_MMI_interrupt();
#ifdef C_SUB_ADDR_NET_ENABLE
    if(HW_addr_map_revision>=5)INR_PCI_BAR1_write(FPGA_PORT_mask,(C_BASE_ADDR_NET_LOWER<<8)+C_SUB_ADDR_NET_ENABLE);// enable ports

#endif

}


