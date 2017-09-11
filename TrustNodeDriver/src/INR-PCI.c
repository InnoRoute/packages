/**
*@file 
*@brief Functions for PCI communication
*@author M.Ulbricht 2015
**/
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/pci.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/delay.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/netdev_features.h>
#include <linux/kthread.h>
#include <linux/skbuff.h>
#include <linux/unistd.h>
#include "INR.h"
#include "INR-NW.h"
#include "INR-PCI.h"
#include "INR-NAPI.h"
#include <asm/signal.h>
#include <linux/semaphore.h>
#include <asm/cacheflush.h>

volatile uint32_t tx_head_backup = 0;
volatile uint8_t TX_DBG_mod=0;
volatile uint8_t RX_DBG_mod=0;
volatile uint64_t gIrq; /**< interrupt id*/
struct pci_dev *globdev;
void *gBaseVirt = NULL;/**< pointer for start-address of bar0*/
uint64_t gBaseHdwr,/**< start-address of bar0*/
  gBaseLen; /**<legth of bar0*/
uint64_t INR_TX_Ring;	      /**<Address of TX-descriptor ring*/
uint64_t INR_RX_Ring[INR_PCI_rx_descriptor_ring_count];	      /**<Address of RX-descriptor ring*/
//struct INR_TDESC *TXdesc = NULL;/**<Tx-description ring accessable from CPU*/
//const int dma_length = 4;/**<legth of DMA-memory*/
uint64_t INR_PCI_tx_descriptor_ring[INR_PCI_tx_descriptor_ring_length] = { 0 };/**<Saving the addresses of TX descriptors*/

uint64_t INR_PCI_rx_descriptor_ring[INR_PCI_rx_descriptor_ring_length]
  [INR_PCI_rx_descriptor_ring_count] = {
  0
};					     /**<Saving the addresses of RX descriptors*/

volatile uint8_t txsemophore = 0;
uint64_t INR_PCI_tx_descriptor_base = NULL;/**<Base address of TX ring*/
volatile uint64_t INR_PCI_tx_descriptor_current = 0;/**<next free tx descriptor*/
uint64_t INR_PCI_tx_descriptor_unmap_current = 0;/**<next free tx descriptor*/

uint64_t INR_PCI_rx_descriptor_base[INR_PCI_rx_descriptor_ring_count] = { 0 };/**<Base addresses of RX rings*/
uint64_t INR_PCI_rx_descriptor_current[INR_PCI_rx_descriptor_ring_count] = { 0 };

	/**<next free tx descriptor*/

//struct INR_PCI_rx_descriptor_ring_entry *rx_descriptor_pool[];/**<pool of rx_descriptores (one fragmented page)*/
uint32_t page_prealloc_current = 0;
struct page *INR_PCI_rx_preallocated_page[INR_PCI_page_prealloc_count] = { NULL };

struct page *INR_PCI_rx_descriptor_pool_root[INR_PCI_rx_descriptor_ring_count];/**<root of the rx descriptor pool*/
uint64_t INR_PCI_rx_descriptor_pool_dma_root[INR_PCI_rx_descriptor_ring_count];/**<dma_root of the rx descriptor pool*/
uint64_t rx_descriptor_pool_length = 0;
				    /**<count of rx-descriptors in one page*/
uint64_t rx_descriptor_pool_free[INR_PCI_rx_descriptor_ring_count];/**<count of free descriptors in rx-pool*/
struct sk_buff *rx_skb[INR_PCI_rx_descriptor_ring_count];
//uint8_t *data_tx[INR_PCI_tx_descriptor_ring_length];
struct INR_PCI_tx_descriptor_ring_entry data_tx[INR_PCI_tx_descriptor_ring_length];/**<stores memory information aberout every fragment in tx ring*/
struct INR_PCI_tx_descriptor_ring_entry tx_descriptor_unmap_queue[INR_PCI_tx_descriptor_ring_length] = { 0 };

													 /**<tx descript unmap queue*/

//uint8_t INR_PCI_rx_descriptor_pointer[INR_PCI_tx_descriptor_ring_count] = 0;
uint32_t tx_unmap_queue_write_pointer = 0;/**<points to the next free space in tx unmap queue*/
uint32_t tx_unmap_queue_read_pointer = 1;/**<points to the tx-fragment which will be unpapped on next call of unmapper*/
//readpointer befor write, so wee wait the whole ring until handeling
uint8_t firstpkg[INR_PCI_rx_descriptor_ring_count] = { 1 };/**<is the curretn SKB the first of packet?*/
uint8_t dropmode[INR_PCI_rx_descriptor_ring_count] = { 0 };/**<drop all next fragments until eop*/

uint32_t refillcount[INR_PCI_rx_descriptor_ring_count] = { 0 };/**<count the refills of descriptor-pools  */

struct sk_buff *skb_head_rx[INR_PCI_rx_descriptor_ring_count];
struct INR_PCI_rx_descriptor_ring_entry *data_rx[INR_PCI_rx_descriptor_ring_length]
  [INR_PCI_rx_descriptor_ring_count];/**<Structure to store the received data*/
uint32_t pagesize = 0;

uint64_t dma_rx[INR_PCI_rx_descriptor_ring_length][INR_PCI_rx_descriptor_ring_count];
									      /**<DMA adresses of the received data*/
static DECLARE_WAIT_QUEUE_HEAD (INR_PCI_TX_unmapd_waittingqueu);/**<waiting lock for TX_descriptor_unmap_deamon*/
static DECLARE_WAIT_QUEUE_HEAD (INR_PCI_rx_pageallocator_waittingqueu);/**<waiting lock for rx pageallocator*/
static DECLARE_WAIT_QUEUE_HEAD (INR_tailtest);/**<waiting lock for rx tailtest*/
uint64_t lastbot[INR_PCI_rx_descriptor_ring_count] = { (INR_PCI_rx_descriptor_ring_length - 1) * INR_PCI_rx_descriptor_length };	//< store last bottom value
DEFINE_SEMAPHORE (INR_PCI_rx_pageallocator_sem);
DEFINE_SEMAPHORE (INR_PCI_tx_ring_sem);
DEFINE_SEMAPHORE (INR_PCI_rx_ring_sem0);
DEFINE_SEMAPHORE (INR_PCI_rx_ring_sem1);
DEFINE_SEMAPHORE (INR_PCI_rx_ring_sem2);
DEFINE_SEMAPHORE (INR_PCI_rx_ring_sem3);
DEFINE_SEMAPHORE (INR_PCI_rx_ring_sem4);
DEFINE_SEMAPHORE (INR_PCI_rx_ring_sem5);
DEFINE_SEMAPHORE (INR_PCI_rx_ring_sem6);
DEFINE_SEMAPHORE (INR_PCI_rx_ring_sem7);
DEFINE_SEMAPHORE (INR_PCI_rx_ring_sem8);
DEFINE_SEMAPHORE (INR_PCI_rx_ring_sem9);
DEFINE_SEMAPHORE (INR_PCI_rx_ring_sem10);
DEFINE_SEMAPHORE (INR_PCI_rx_ring_sem11);
DEFINE_SEMAPHORE (INR_PCI_rx_ring_sem12);
DEFINE_SEMAPHORE (INR_PCI_rx_ring_sem13);
DEFINE_SEMAPHORE (INR_PCI_rx_ring_sem14);
DEFINE_SEMAPHORE (INR_PCI_rx_ring_sem15);
/**
*setts all global variables to zero
*/
void
INR_PCI_zerovars(){
uint64_t j=0;
txsemophore = 0;
INR_PCI_tx_descriptor_current = 0;
INR_PCI_tx_descriptor_unmap_current = 0;
for(j=0;j<INR_PCI_rx_descriptor_ring_count;j++)INR_PCI_rx_descriptor_base[j];
for(j=0;j<INR_PCI_rx_descriptor_ring_count;j++)INR_PCI_rx_descriptor_current[j];
page_prealloc_current=0;
for(j=0;j<INR_PCI_page_prealloc_count;j++)INR_PCI_rx_preallocated_page[j] =  NULL ;
rx_descriptor_pool_length=0;
tx_unmap_queue_write_pointer = 0;
tx_unmap_queue_read_pointer = 1;
for(j=0;j<INR_PCI_rx_descriptor_ring_count;j++)firstpkg[j] = 1;
for(j=0;j<INR_PCI_rx_descriptor_ring_count;j++)dropmode[j] = 0;
for(j=0;j<INR_PCI_rx_descriptor_ring_count;j++)refillcount[j] = 0 ;
for(j=0;j<INR_PCI_rx_descriptor_ring_count;j++)lastbot[j] = (INR_PCI_rx_descriptor_ring_length - 1) * INR_PCI_rx_descriptor_length ;
pagesize = 0;
}




//*****************************************************************************************************************
/**
*adds an TX entry to the unmap queue.
*This is needed to give the hanrdware some time to acces and dont cause memory violations.
*/
void
INR_PCI_tx_unmapper (struct INR_PCI_tx_descriptor_ring_entry *entry)
{

  INR_LOG_debug ("Unmapper: wr:%i re:%i addr:%x\n", tx_unmap_queue_write_pointer, tx_unmap_queue_read_pointer, entry->dma);
  memcpy (&tx_descriptor_unmap_queue[tx_unmap_queue_write_pointer], entry, sizeof (entry));	//put tx-descriptor in the queue

  if (tx_descriptor_unmap_queue[tx_unmap_queue_read_pointer].dma) {	//if not empty, unmap
    dma_unmap_single (&globdev->dev, tx_descriptor_unmap_queue[tx_unmap_queue_read_pointer].dma, tx_descriptor_unmap_queue[tx_unmap_queue_read_pointer].length, DMA_TO_DEVICE);	//unmap DMA
    if (tx_descriptor_unmap_queue[tx_unmap_queue_read_pointer].eop)
      kfree_skb (tx_descriptor_unmap_queue[tx_unmap_queue_read_pointer].skb);	//if last fragment of skb->free skb
    if (!tx_descriptor_unmap_queue[tx_unmap_queue_read_pointer].paged)
      kfree (tx_descriptor_unmap_queue[tx_unmap_queue_read_pointer].data);	//if not paged free kmalloc mem
    INR_LOG_debug ("Unmapper: %i freeed\n", tx_unmap_queue_read_pointer);
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
*read version form hardware
*/
uint32_t
get_pci_version (void)
{

  return INR_PCI_BAR0_read (INR_PCI_version);
}
//*****************************************************************************************************************
/**
*activate TX debug messages
*/
void set_tx_dbg(uint8_t txdbg)
{

  TX_DBG_mod=txdbg;
}

//*****************************************************************************************************************
/**
*activate RX debug messages
*/
void set_rx_dbg(uint8_t rxdbg)
{

  RX_DBG_mod=rxdbg;
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
*refill rx-descriptor pool
*/
uint8_t
INR_PCI_rx_descriptor_pool_refill (uint8_t index)
{
  refillcount[index]++;
  //INR_LOG_debug ("RX-Pool refill:%i:%i\n", refillcount[index], index);


  if ((page_prealloc_current))	//if dont get sem , bring onself of
  {
    if (down_trylock (&INR_PCI_rx_pageallocator_sem)) {
      INR_PCI_rx_descriptor_pool_root[index] = alloc_page (GFP_ATOMIC);
      if (unlikely (INR_PCI_rx_descriptor_pool_root[index] == NULL)) {
	INR_LOG_debug ("RX Page alloc faild\n");
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
    INR_LOG_debug ("RX Page alloc faild(noprealloc)\n");
    return 1;
  }
  INR_PCI_rx_descriptor_pool_dma_root[index] = dma_map_page (&globdev->dev, INR_PCI_rx_descriptor_pool_root[index], 0, pagesize, DMA_FROM_DEVICE);	//gen new root dma
  if (dma_mapping_error (&globdev->dev, INR_PCI_rx_descriptor_pool_dma_root[index]))
    INR_LOG_debug ("RX page mappng error! \n");
  rx_descriptor_pool_free[index] = 0;	//reset pool counter
  //wake_up_interruptible (&INR_PCI_rx_pageallocator_waittingqueu);     //wakeop preallocator
  return 0;
}

//*****************************************************************************************************************
/**
*return pagefragment as rx_descriptior memeory
*/
struct INR_PCI_rx_descriptor_ring_entry *
INR_PCI_get_new_rx_descriptor_ring_entry (uint8_t index)
{
  if (rx_descriptor_pool_free[index] >= rx_descriptor_pool_length)
    while (INR_PCI_rx_descriptor_pool_refill (index));
//    if(INR_PCI_rx_descriptor_pool_refill (index))return 0;    // if empty refill
  struct INR_PCI_rx_descriptor_ring_entry *tmp_rx_desc = kmalloc (sizeof (struct INR_PCI_rx_descriptor_ring_entry), GFP_ATOMIC);
  while (!tmp_rx_desc)
    tmp_rx_desc = kmalloc (sizeof (struct INR_PCI_rx_descriptor_ring_entry), GFP_ATOMIC);	//block if kmalloc fails
  tmp_rx_desc->page = INR_PCI_rx_descriptor_pool_root[index];
  tmp_rx_desc->offset = rx_descriptor_pool_free[index] * data_size_rx;
  tmp_rx_desc->data = page_address (INR_PCI_rx_descriptor_pool_root[index]) + tmp_rx_desc->offset;
  tmp_rx_desc->dma = INR_PCI_rx_descriptor_pool_dma_root[index] + tmp_rx_desc->offset;
  tmp_rx_desc->dma_root = INR_PCI_rx_descriptor_pool_dma_root[index];
  tmp_rx_desc->fragmentindex = rx_descriptor_pool_free[index];
  if (!INR_PCI_rx_descriptor_pool_root[index])
    INR_LOG_debug ("error: zero rx descriptor pool root\n");
  if (!page_address (INR_PCI_rx_descriptor_pool_root[index]))
    INR_LOG_debug ("error: zero page_address (INR_PCI_rx_descriptor_pool_root[index])\n");
  if (!INR_PCI_rx_descriptor_pool_dma_root[index])
    INR_LOG_debug ("error: zero INR_PCI_rx_descriptor_pool_dma_root[index]\n");
  rx_descriptor_pool_free[index] += 1;
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
  INR_LOG_debug ("Hardware version:%i  Date:%i.%i\n", 0xff & ver, (0xff00 & ver) >> 8, (0xffff0000 & ver) >> 16);

}

//*****************************************************************************************************************
/**
*reset Hardware
*/
void
INR_PCI_reset ()
{
  writel (1 << 26, (gBaseVirt + (4 * 0)));	//reset device
}

//*****************************************************************************************************************


/**
*Processing rx-descriptor-ring
*@param index RX-descriptor-ring-index
*/
uint16_t
INR_PCI_process_rx_descriptor_ring (uint8_t index)
{
  uint16_t count = 0;
/* switch (index){
case 0:down(&INR_PCI_rx_ring_sem0);break;
case 1:down(&INR_PCI_rx_ring_sem1);break;
case 2:down(&INR_PCI_rx_ring_sem2);break;
case 3:down(&INR_PCI_rx_ring_sem3);break;
case 4:down(&INR_PCI_rx_ring_sem4);break;
case 5:down(&INR_PCI_rx_ring_sem5);break;
case 6:down(&INR_PCI_rx_ring_sem6);break;
case 7:down(&INR_PCI_rx_ring_sem7);break;
case 8:down(&INR_PCI_rx_ring_sem8);break;
case 9:down(&INR_PCI_rx_ring_sem9);break;
case 10:down(&INR_PCI_rx_ring_sem10);break;
case 11:down(&INR_PCI_rx_ring_sem11);break;
case 12:down(&INR_PCI_rx_ring_sem12);break;
case 13:down(&INR_PCI_rx_ring_sem13);break;
case 14:down(&INR_PCI_rx_ring_sem14);break;
case 15:down(&INR_PCI_rx_ring_sem15);break;
default: break;
}*/

//INR_LOG_debug ("store eth-type in skb\n");
//INR_LOG_debug("Vorher: Ring %i head:%llx Ring bottom:%llx current:%i currenthex:%llx\n",index,INR_PCI_BAR0_read (INR_PCI_rx_descriptor_head_reg + (64 * index)),INR_PCI_BAR0_read (INR_PCI_rx_descriptor_tail_reg + (64 * index)),INR_PCI_rx_descriptor_current[index],INR_PCI_rx_descriptor_current[index] * INR_PCI_rx_descriptor_length);
  uint32_t ringhead = INR_PCI_BAR0_read (INR_PCI_rx_descriptor_head_reg + (64 * index));
  INR_CHECK_fpga_read_val (ringhead, "process_rx_ring:INR_PCI_rx_descriptor_head_reg", 0);
//INR_LOG_debug("Rx-head readed:%i * %i\n",ringhead/INR_PCI_rx_descriptor_length, INR_PCI_rx_descriptor_length);
  uint16_t loopcount = 0;
  while (ringhead != (INR_PCI_rx_descriptor_current[index] * INR_PCI_rx_descriptor_length))	//read all available fragments
  {
    loopcount++;
    if (loopcount > INR_PCI_rx_max_loops)
      goto exit_rxloop;		//exit strategy if FPGA crashes
    volatile struct INR_PCI_rx_descriptor *RX_descriptor = INR_PCI_rx_descriptor_ring[INR_PCI_rx_descriptor_current[index]][index];	//get address of descriptor
//INR_LOG_debug("Sync RX dma Range: dma:%llx offset:%i\n",data_rx[INR_PCI_rx_descriptor_current[index]][index]->dma_root,data_rx[INR_PCI_rx_descriptor_current[index]][index]->offset);
    dma_sync_single_range_for_cpu (&globdev->dev, data_rx[INR_PCI_rx_descriptor_current[index]][index]->dma_root, data_rx[INR_PCI_rx_descriptor_current[index]][index]->offset, data_size_rx, DMA_FROM_DEVICE);	//sync memory of descriptor to CPU

    if (RX_DBG_mod)
      INR_LOG_debug ("RX-Descriptor-dump RX_Current:%i DescrBase:0x%x Status:%x Length:%i Index:%i FirstPKG:%i Buffer:%llx DMA:%llx MEM:%llx offset:0x%i page:%llx, fragmentindex:%i\n", INR_PCI_rx_descriptor_current[index], INR_PCI_rx_descriptor_ring[INR_PCI_rx_descriptor_current[index]][index], RX_descriptor->Status, RX_descriptor->length, index, firstpkg[index], RX_descriptor->buffer, data_rx[INR_PCI_rx_descriptor_current[index]][index]->dma, data_rx[INR_PCI_rx_descriptor_current[index]][index]->data, data_rx[INR_PCI_rx_descriptor_current[index]][index]->offset, page_address (data_rx[INR_PCI_rx_descriptor_current[index]][index]->page), data_rx[INR_PCI_rx_descriptor_current[index]][index]->fragmentindex);	//long debug print

    if ((RX_descriptor->Errors & (0x1))) {	//handle descriptor error flag
      INR_LOG_debug ("error received fragment with error flag\n");
      dropmode[index] = 1;

    }
    //prefetch(data_rx[INR_PCI_rx_descriptor_current[index]][index]->data);//prefetch data
    if ((RX_descriptor->Status & (0x3)) == 0) {	//hand bad status value
      INR_LOG_debug ("error received fragment with clear dd flag\n");
      dropmode[index] = 1;

    }
//##############################FIRST FRAGMENT handling
    if (firstpkg[index]) {	//first fragment of packet
      firstpkg[index] = 0;
      if (RX_descriptor->length < ETH_HLEN) {	//handle to short framents
	INR_LOG_debug ("error received to short first fragment\n");
	dropmode[index] = 1;
	return 0;
      }


      if (zerocopy_rx) {	//if zerocopy alloc small SKB and copy just first part
	rx_skb[index] = netdev_alloc_skb (get_nwdev (index), ETH_HLEN + 2);
	if (!rx_skb[index]) {
	  INR_LOG_debug ("Cant alloc RX SKB\n");
	  dropmode[index] = 1;
	  return 0;
	}
	rx_skb[index]->dev = get_nwdev (index);
	skb_reserve (rx_skb[index], 2);
	skb_put (rx_skb[index], ETH_HLEN);
	memcpy (rx_skb[index]->data, data_rx[INR_PCI_rx_descriptor_current[index]]
		[index]->data, ETH_HLEN);
      }
      else {			//if no zerocopy alloc big skb and coppy whole fragment into
	rx_skb[index] = netdev_alloc_skb (get_nwdev (index), 1800 + 2);
	if (!rx_skb[index]) {
	  INR_LOG_debug ("Cant alloc RX SKB\n");
	  dropmode[index] = 1;
	  return 0;
	}
	rx_skb[index]->dev = get_nwdev (index);
	skb_reserve (rx_skb[index], 2);
	skb_put (rx_skb[index], RX_descriptor->length);	//should everytime be true, but is not :/
	memcpy (rx_skb[index]->data, data_rx[INR_PCI_rx_descriptor_current[index]]
		[index]->data, RX_descriptor->length);	//copy whole fragment into SKB

      }
      rx_skb[index]->ip_summed = CHECKSUM_UNNECESSARY;	//set checksumflag in skb
      rx_skb[index]->protocol = eth_type_trans (rx_skb[index], rx_skb[index]->dev);	//set ethertype in skb
      if (RX_descriptor->length > ETH_HLEN)
	if (zerocopy_rx)	//if zerocopy appent unhandled part of packet to the skb
	  skb_add_rx_frag (rx_skb[index], skb_shinfo (rx_skb[index])->nr_frags, data_rx[INR_PCI_rx_descriptor_current[index]]
			   [index]->page, data_rx[INR_PCI_rx_descriptor_current[index]]
			   [index]->offset + ETH_HLEN, RX_descriptor->length - ETH_HLEN, ALIGN (RX_descriptor->length - ETH_HLEN, L1_CACHE_BYTES));
      atomic_inc (&data_rx[INR_PCI_rx_descriptor_current[index]][index]->page->_count);	//INCREMTN PAGECOUNT.. creazy voodoo
    }
//##############################OTHER FRAGMENT handling
    else {			//some other fragment of packet
      if (zerocopy_rx) {
	if (!rx_skb[index])
	  INR_LOG_debug ("rx skb nullpointer!\n");
	if (!data_rx[INR_PCI_rx_descriptor_current[index]][index]->page)
	  INR_LOG_debug ("rx page nullpointer!\n");
	if (!RX_descriptor->length)
	  INR_LOG_debug ("rx length nullpointer!\n");

	if (data_rx[INR_PCI_rx_descriptor_current[index]][index]->fragmentindex == 0) {	//fragment are start of new page
	  skb_add_rx_frag (rx_skb[index], skb_shinfo (rx_skb[index])->nr_frags, data_rx[INR_PCI_rx_descriptor_current[index]][index]->page,
			   data_rx[INR_PCI_rx_descriptor_current[index]][index]->offset, RX_descriptor->length, ALIGN (RX_descriptor->length,
														       L1_CACHE_BYTES));
	  atomic_inc (&data_rx[INR_PCI_rx_descriptor_current[index]][index]->page->_count);	//INCREMT PAGECOUNT.. creazy voodoo
	}
	else {			//dont add new page, only extend last frag (pages are sorted by rx-ring )
	  skb_shinfo (rx_skb[index])->frags[skb_shinfo (rx_skb[index])->nr_frags - 1].size += RX_descriptor->length;

	  rx_skb[index]->len += RX_descriptor->length;
	  rx_skb[index]->data_len += RX_descriptor->length;




	}
//INR_LOG_debug("Number of fragments:%i\n",skb_shinfo(rx_skb[index])->nr_frags);
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
      INR_LOG_debug ("RX skb skb-len:%i offset:%i len:%i page:%llx ring:%i\n", rx_skb[index]->len,
		     data_rx[INR_PCI_rx_descriptor_current[index]][index]->offset, RX_descriptor->length,
		     data_rx[INR_PCI_rx_descriptor_current[index]][index]->data, index);
      printk ("RXdump:");
      for (tmp = 0; (tmp < RX_descriptor->length); tmp++) {
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
      //INR_LOG_timelog("RX port:%i\n",index);


      firstpkg[index] = 1;
      //if (NET_RX_DROP == netif_rx (rx_skb[index]))
      count++;

      if (dropmode[index]) {	//if not drop...send
	priv->stats.rx_dropped++;
	kfree_skb (rx_skb[index]);
      }
      else			//if (!RX_DBG_mod)
      if (rx_skb[index])
	if (rx_skb[index]->dev->flags & IFF_UP)	//check if interface is up
	  if (NET_RX_DROP == netif_receive_skb (rx_skb[index])) {


	    if (RX_DBG_mod)
	      INR_LOG_debug ("PKT dropped RX :/\n");
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
      INR_LOG_debug ("error pci alloc zeropointer, rxprocess ring:%i\n", index);
      return 0;
    }
//      if (!data_rx[INR_PCI_rx_descriptor_current[index]][index]) goto DROP;//page alloc fail -> drop
    RX_descriptor->buffer = data_rx[INR_PCI_rx_descriptor_current[index]][index]->dma;	// save memory address in descriptor
    RX_descriptor->Status &= ~(0x3);	//status zurücksetzen
    RX_descriptor->length = data_size_rx;
    if (INR_PCI_rx_descriptor_current[index] == (INR_PCI_rx_descriptor_ring_length - 1)) {	//update RX-rint Tailcounters
      INR_PCI_rx_descriptor_current[index] = 0;
    }
    else {
      INR_PCI_rx_descriptor_current[index]++;
    }

    /*uint64_t bottmp = INR_PCI_BAR0_read (INR_PCI_rx_descriptor_tail_reg + (64 * index));
       if (bottmp%INR_PCI_rx_descriptor_length){INR_LOG_debug("Strange RX_Tail value 0x%x  written value was 0x%x, using local value\n",bottmp,lastbottmp);
       INR_PCI_BAR0_write(0x1,0x7004);//trigger for chipscope
       bottmp=lastbottmp;//using local stored value
       }
     */
    if (lastbot[index] >= ((INR_PCI_rx_descriptor_ring_length - 1) * INR_PCI_rx_descriptor_length)) {
      INR_PCI_BAR0_write ((INR_PCI_rx_descriptor_ring_length - 1) * INR_PCI_rx_descriptor_length, INR_PCI_rx_descriptor_tail_reg + (64 * index));
      lastbot[index] = 0;
    }
    else {
      INR_PCI_BAR0_write (lastbot[index], INR_PCI_rx_descriptor_tail_reg + (64 * index));
      lastbot[index] += INR_PCI_rx_descriptor_length;
    }
//      if(INR_PCI_BAR0_read (INR_PCI_rx_descriptor_tail_reg + (64 * index))!=lastbot[index])INR_LOG_debug("RX-Tail error\n");
//      INR_LOG_debug("Rx-bottomwritten:%i * %i\n",lastbot[index]/INR_PCI_rx_descriptor_length,INR_PCI_rx_descriptor_length);
  }

//INR_LOG_debug("Nachher: Ring %i head:%llx Ring bottom:%llx current:%i currenthex:%llx\n",index,INR_PCI_BAR0_read (INR_PCI_rx_descriptor_head_reg + (64 * index)),INR_PCI_BAR0_read (INR_PCI_rx_descriptor_tail_reg + (64 * index)),INR_PCI_rx_descriptor_current[index],INR_PCI_rx_descriptor_current[index] * INR_PCI_rx_descriptor_length);

/*
switch (index){//semaphore freigeben
case 0:up(&INR_PCI_rx_ring_sem0);break;
case 1:up(&INR_PCI_rx_ring_sem1);break;
case 2:up(&INR_PCI_rx_ring_sem2);break;
case 3:up(&INR_PCI_rx_ring_sem3);break;
case 4:up(&INR_PCI_rx_ring_sem4);break;
case 5:up(&INR_PCI_rx_ring_sem5);break;
case 6:up(&INR_PCI_rx_ring_sem6);break;
case 7:up(&INR_PCI_rx_ring_sem7);break;
case 8:up(&INR_PCI_rx_ring_sem8);break;
case 9:up(&INR_PCI_rx_ring_sem9);break;
case 10:up(&INR_PCI_rx_ring_sem10);break;
case 11:up(&INR_PCI_rx_ring_sem11);break;
case 12:up(&INR_PCI_rx_ring_sem12);break;
case 13:up(&INR_PCI_rx_ring_sem13);break;
case 14:up(&INR_PCI_rx_ring_sem14);break;
case 15:up(&INR_PCI_rx_ring_sem15);break;
default: break;
}
*/
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
static irqreturn_t
XPCIe_IRQHandler (int irq, void *dev_id, struct pt_regs *regs)
{

//INR_LOG_timelog("Interrupt Handler %x Start ..............................................\n",irq);
  uint64_t intcause = 0xffff & 0xffff;	//INR_PCI_BAR0_read(INR_PCI_interrupt_cause_reg);
if (RX_DBG_mod)INR_LOG_debug("Int cause:%lx\n",INR_PCI_BAR0_read(INR_PCI_interrupt_cause_reg));
  INR_PCI_BAR0_write (1, INR_PCI_interrupt_cause_reg);
  uint8_t i = 0;
  for (i = 0; i < INR_PCI_rx_descriptor_ring_count; i++)
    if ((get_nwdev (i)->flags) & IFF_UP)
      if (intcause & (1 << i)) {
	if (NAPI_enable) {	//enable polling if NAPI is used
	  INR_NAPI_schedule (i);


	}
	else
	  INR_PCI_process_rx_descriptor_ring (i);
      }

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
  INR_LOG_debug ("ISR Setup..starting\n");
  int intcount = pci_enable_msi_range (dev, 1, 255);
//pci_enable_msi(dev);
//  INR_LOG_debug ("num:%i\n",);
//int intcount=1;

  INR_LOG_debug ("ISR range requested..\n");
  if (intcount > 0) {
    INR_STATUS_set (INR_STATUS_MSI);
    INR_LOG_debug ("%i MSIs enabled\n", intcount);	//enable MSI
    gIrq = dev->irq;		//get irq from device

    INR_LOG_debug (" Init: Device IRQ: %lli\n", gIrq);

    INR_LOG_debug ("ISR Setup..\n");
    if (0 > request_irq (gIrq, XPCIe_IRQHandler, 0, "INRBusmaster", dev))	//SA_INTERRUPT
    {
      INR_LOG_debug ("Init: Unable to allocate IRQ\n");
      return (1);
    }

  }
  else
    INR_LOG_debug ("MSI error\n");



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

  //wake_up_interruptible (&INR_PCI_rx_pageallocator_waittingqueu);     //loof if page can be preallocated
//INR_LOG_debug("Alloc new RX page fragment%i:%i: data:0x%llx dma:0x%llx index:%i dma_root:0x%llx offset:%i\n",current_descriptor,ringindex,data_rx[current_descriptor][ringindex]->data,data_rx[current_descriptor][ringindex]->dma,data_rx[current_descriptor][ringindex]->fragmentindex,data_rx[current_descriptor][ringindex]->dma_root,data_rx[current_descriptor][ringindex]->offset);

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

  INR_LOG_debug ("Init RX-Ring %i\n", index);

  INR_PCI_rx_descriptor_base[index] = pci_alloc_consistent (dev, INR_PCI_rx_descriptor_ring_length * INR_PCI_rx_descriptor_length, &INR_RX_Ring[index]);	//allocate TX-Ring in DMA
  INR_LOG_debug ("RX-Ring: cpu_adress:0x%llx  dma-adress:0x%llx\n", INR_PCI_rx_descriptor_base[index], INR_RX_Ring[index]);
  uint32_t tmpaddr_h, tmpaddr_l;
  tmpaddr_l = INR_RX_Ring[index] & 0xffffffff;
  tmpaddr_h = (INR_RX_Ring[index] >> 32) & 0xffffffff;
  INR_LOG_debug ("addh:%x addrl:%x\n", tmpaddr_h, tmpaddr_l);
  if (!INR_PCI_rx_descriptor_base[index])
    INR_LOG_debug ("error cant alloc rx-ring %i\n", index);
  INR_STATUS_set (INR_STATUS_RX_RING);
  INR_PCI_BAR0_write (0, INR_PCI_rx_descriptor_tail_reg + (64 * index));	//set bottom-pointer =0
  //INR_LOG_debug ("write RX_Ring_bottom %i to 0x%x\n",  INR_PCI_BAR0_read (INR_PCI_rx_descriptor_tail_reg+(64*index)),          INR_PCI_rx_descriptor_tail_reg+(64*index));
  //INR_PCI_BAR0_write (0, INR_PCI_rx_descriptor_head_reg);     //set head-pointer =0
  //INR_LOG_debug ("write RX_Ring_head %i to 0x%x\n", INR_PCI_BAR0_read (INR_PCI_rx_descriptor_head_reg),        INR_PCI_rx_descriptor_head_reg);

  //INR_LOG_debug ("write RX_Ring_length %i to 0x%x\n",          INR_PCI_BAR0_read (INR_PCI_rx_descriptor_length_reg+(64*index)),                INR_PCI_rx_descriptor_length_reg(64*index));
  INR_PCI_BAR0_write64 (INR_RX_Ring[index], INR_PCI_rx_descriptor_base_address_reg + (64 * index));
  //INR_PCI_BAR0_write (INR_RX_Ring[index]&0xffffffff, INR_PCI_rx_descriptor_base_address_reg + (64 * index));  //write RX-Ring-address to bar0
  //INR_PCI_BAR0_write (INR_RX_Ring[index]>>32, INR_PCI_rx_descriptor_base_address_reg_h + (64 * index));       //write RX-Ring-address_h  to bar0
  INR_LOG_debug ("write RX_Ring_address %llx to 0x%llx\n",
		 INR_PCI_BAR0_read64 (INR_PCI_rx_descriptor_base_address_reg + (64 * index)), INR_PCI_rx_descriptor_base_address_reg + (64 * index));
  uint64_t i = 0;
  //INR_LOG_debug ("fill ring with addresses\n");
  INR_PCI_rx_descriptor_ring[0][index] = INR_PCI_rx_descriptor_base[index];
  for (i = 1; i < INR_PCI_rx_descriptor_ring_length; i++) {	//fill descriptor-ring with dma-addresses
    //INR_LOG_debug ("Fill Descriptor: %i\n",i);
    INR_PCI_rx_descriptor_ring[i][index] = INR_PCI_rx_descriptor_ring[i - 1][index] + INR_PCI_rx_descriptor_length;

  }
  INR_LOG_debug ("alloc RX-SKBs and fill them width descriptors\n");
  for (i = 0; i < INR_PCI_rx_descriptor_ring_length; i++) {
    //data_rx[i] = kmalloc (data_size_rx, GFP_KERNEL);        //rx-buffer somewhere in kernelspace
    //data_rx[i]=dev_alloc_skb(data_size_rx + 2);
    //if (!data_rx[i]) {INR_LOG_debug("cant alloc RX-SKB!\n");}
    // INR_LOG_debug ("buffer %i allocated\n",i);
    //skb_reserve (data_rx[i], 2);

    INR_PCI_alloc_new_rx_skb (i, index);	//alloc skb and map dma
    if (!data_rx[i][index]) {
      INR_LOG_debug ("error pci alloc zeropointer\n");
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

void
INR_PCI_ringtailtest (void *nix)
{
  DECLARE_WAITQUEUE (wait3, current);
  INR_LOG_debug ("reading RDT_0 and RDT_1...\n");
  allow_signal (SIGKILL);
  add_wait_queue (&INR_tailtest, &wait3);
  while (1) {
    set_current_state (TASK_INTERRUPTIBLE);
    //schedule ();
    if (signal_pending (current))
      break;			//exit on thermination
//INR_LOG_debug ("möp\n");
    if (INR_PCI_BAR0_read (INR_PCI_rx_descriptor_tail_reg) % INR_PCI_rx_descriptor_length)
      INR_LOG_debug ("Error: RDT_0 is %x :/\n", INR_PCI_BAR0_read (INR_PCI_rx_descriptor_tail_reg));
    if (INR_PCI_BAR0_read (INR_PCI_rx_descriptor_tail_reg + 64) % INR_PCI_rx_descriptor_length)
      INR_LOG_debug ("Error: RDT_1 is %x :/\n", INR_PCI_BAR0_read (INR_PCI_rx_descriptor_tail_reg + 64));
  }
  remove_wait_queue (&INR_tailtest, &wait3);
}

//*****************************************************************************************************************
/**
*push unmap old TX buffer
*@param buffer pointer to data
*@param length length of buffer
*/
void
INR_PCI_rx_pageallocator (void *nix)
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
	INR_LOG_debug ("Refill all pages\n");
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
INR_TX_unmap_d (void *nix)
{
  DECLARE_WAITQUEUE (wait, current);
  allow_signal (SIGKILL);
  add_wait_queue (&INR_PCI_TX_unmapd_waittingqueu, &wait);
  while (1) {
    INR_LOG_debug ("möp!\n");
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
      //INR_LOG_debug ("moep!\n");
      struct INR_PCI_tx_descriptor *TX_descriptor = INR_PCI_tx_descriptor_ring[INR_PCI_tx_descriptor_unmap_current];	//create temp. TDESC-structure
      //INR_LOG_debug ("current descriptor offset:%i\n",             INR_PCI_tx_descriptor_current);
//INR_LOG_debug("TX_current:%i\n",INR_PCI_tx_descriptor_current);
      if ((TX_descriptor->STA & 0x1) == 0)	//look for DD-Bit
      {
	INR_LOG_debug ("Unmapd-error descriptor not free\n");
	//descriptor is not free, abort
      }
      else if (TX_descriptor->buffer) {	//if there is an address stored, first unmap before override
	//INR_LOG_debug ("unmap old packet\n");
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
  INR_LOG_debug ("Init TX-Ring\n");

  INR_PCI_tx_descriptor_base = pci_alloc_consistent (dev, INR_PCI_tx_descriptor_ring_length * INR_PCI_tx_descriptor_length, &INR_TX_Ring);	//allocate TX-Ring in DMA
  INR_LOG_debug ("TX-Ring: cpu_adress:0x%llx  dma-adress:0x%llx\n", INR_PCI_tx_descriptor_base, INR_TX_Ring);
  if (!INR_PCI_tx_descriptor_base)
    INR_LOG_debug ("error cant alloc tx-ring\n");
  INR_STATUS_set (INR_STATUS_TX_RING);
  INR_PCI_BAR0_write (0, INR_PCI_tx_descriptor_tail_reg);	//set bottom-pointer =0
  INR_LOG_debug ("write TX_Ring_bottom %i to 0x%x\n", INR_PCI_BAR0_read (INR_PCI_tx_descriptor_tail_reg), INR_PCI_tx_descriptor_tail_reg);
  //INR_PCI_BAR0_write (0, INR_PCI_tx_descriptor_head_reg);     //set head-pointer =0
  //INR_LOG_debug ("write TX_Ring_head %i to 0x%x\n", INR_PCI_BAR0_read (INR_PCI_tx_descriptor_head_reg),        INR_PCI_tx_descriptor_head_reg);
  INR_PCI_BAR0_write (INR_PCI_tx_descriptor_length * INR_PCI_tx_descriptor_ring_length, INR_PCI_tx_descriptor_length_reg);	//write length of descriptorring
  INR_LOG_debug ("write TX_Ring_length %i to 0x%x\n", INR_PCI_BAR0_read (INR_PCI_tx_descriptor_length_reg), INR_PCI_tx_descriptor_length_reg);
  INR_PCI_BAR0_write (INR_TX_Ring & 0xffffffff, INR_PCI_tx_descriptor_base_address_reg);	//write TX-Ring-address to bar0
  INR_PCI_BAR0_write (INR_TX_Ring >> 32, INR_PCI_tx_descriptor_base_address_reg_h);	//write TX-Ring-address to bar0
  INR_LOG_debug ("write TX_Ring_address %x to 0x%llx\n", (((uint64_t) INR_PCI_tx_descriptor_base_address_reg_h) << 32)
		 | INR_PCI_BAR0_read (INR_PCI_tx_descriptor_base_address_reg), INR_PCI_tx_descriptor_base_address_reg);
  uint64_t i = 0;
  INR_LOG_debug ("fill ring with addresses\n");
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
*unmap all descriptore/s
*@param *dev device
*@param id id of descriptor
*/
void
INR_PRINT_DESC_status (struct pci_dev *dev, uint64_t id)
{

  struct INR_PCI_tx_descriptor *TX_descriptor = INR_PCI_tx_descriptor_ring[id];	//create temp. TDESC-structure
  INR_LOG_debug ("Descriptor %i: Buffer:0x%x\n", id, TX_descriptor->buffer);
  INR_LOG_debug ("  Datalength:0x%x\n", TX_descriptor->length);
  INR_LOG_debug ("  CSO:0x%x\n", TX_descriptor->CSO);
  INR_LOG_debug ("  CMD:0x%x\n", TX_descriptor->CMD);
  INR_LOG_debug ("  STA:0x%x\n", TX_descriptor->STA);
  INR_LOG_debug ("  Rsvd:0x%x\n", TX_descriptor->Rsvd);
  INR_LOG_debug ("  CSS:0x%x\n", TX_descriptor->CSS);
  INR_LOG_debug ("  VLAN:0x%x\n", TX_descriptor->VLAN);
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
  INR_LOG_debug ("unmap all descriptors\n");
  for (i = 0; i < INR_PCI_tx_descriptor_ring_length; i++) {
    struct INR_PCI_tx_descriptor *TX_descriptor = INR_PCI_tx_descriptor_ring[i];	//create temp. TDESC-structure
    if (TX_descriptor->buffer) {	//if there is an address stored, first unmap before override
      //INR_LOG_debug ("unmap descriptor %i  address:0x%x\n", i, TX_descriptor->buffer);
      dma_unmap_single (&dev->dev, TX_descriptor->buffer, TX_descriptor->length, DMA_TO_DEVICE);	//unmap DMA
    }
  }
  for (i = 0; i < INR_PCI_rx_descriptor_ring_length; i++) {
    struct INR_PCI_rx_descriptor *RX_descriptor = INR_PCI_rx_descriptor_ring[i];	//create temp. TDESC-structure
    if (RX_descriptor->buffer) {	//if there is an address stored, first unmap before override
      //INR_LOG_debug ("unmap descriptor %i  address:0x%x\n", i, RX_descriptor->buffer);
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
uint64_t descriptor_current_lastwritten = 0;
/**
*push new packet into TX descriptor ring
*@param *datalength length of data in memory
*@param *dma_addr dma-address of packet buffer
*@param eof last_packet?
*@param port trustnode output-port 0..15
*@param ll local loop (send to cpu)
*/
int
INR_TX_push (struct sk_buff *skb, uint8_t * data, uint16_t datalength, uint8_t eof, uint8_t port, uint8_t ll, uint8_t paged, uint16_t fragcount)
{
  if (INR_STATUS_get (INR_STATUS_DRV_INIT_done) == 0)
    return 1;
  preempt_disable ();
  if (fragcount > INR_PCI_TX_maxfragments)
    return 1;			// to much fragments, drop
  if (txsemophore) {		//INR_LOG_debug("error TX semaphore hit\n");
    return 1;
  }
  txsemophore = 1;

//INR_LOG_debug("pointerdistance:%i, reverse:%i\n",INR_PCI_pointerdistance(INR_PCI_BAR0_read (INR_PCI_tx_descriptor_tail_reg)/INR_PCI_tx_descriptor_length,INR_PCI_BAR0_read (INR_PCI_tx_descriptor_head_reg)/INR_PCI_tx_descriptor_length,INR_PCI_tx_descriptor_ring_length),INR_PCI_tx_descriptor_ring_length-INR_PCI_pointerdistance(INR_PCI_BAR0_read (INR_PCI_tx_descriptor_tail_reg)/INR_PCI_tx_descriptor_length,INR_PCI_BAR0_read (INR_PCI_tx_descriptor_head_reg)/INR_PCI_tx_descriptor_length,INR_PCI_tx_descriptor_ring_length));
  uint64_t head, tail;
  head = INR_PCI_BAR0_read (INR_PCI_tx_descriptor_head_reg) / INR_PCI_tx_descriptor_length;
  tail = INR_PCI_BAR0_read (INR_PCI_tx_descriptor_tail_reg) / INR_PCI_tx_descriptor_length;
  if ((INR_PCI_pointerdistance (tail, head, INR_PCI_tx_descriptor_ring_length)) <= fragcount + 5) {
//INR_LOG_debug("full TX-queue, tail:%i, head:%i fragcount:%i\n",tail,head,fragcount);
    txsemophore = 0;
    return 1;
  }
  uint32_t tmp66 = tx_head_backup;
  uint8_t error = 0;
  if (TX_DBG_mod)
    while (tx_head_backup != head) {
      volatile struct INR_PCI_tx_descriptor *TX_descriptor_headbackup = INR_PCI_tx_descriptor_ring[tx_head_backup];
      if (!TX_descriptor_headbackup->STA) {
	INR_LOG_debug ("error: found unprocessed descriptor: tail:%i, head:%i, position:%i, headbackup:%i\n", tail, head, tx_head_backup, tmp66);
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
  if (0) {			//error
    uint32_t k = 0;
    printk ("dumping ring:\n");
    while (k < INR_PCI_tx_descriptor_ring_length) {
      volatile struct INR_PCI_tx_descriptor *TX_descriptor_tmp = INR_PCI_tx_descriptor_ring[k];
      INR_LOG_debug ("id:%ibuffer:0x%x, len:%i cso:0x%x, cmd:0x%x, sta:0x%x, rsvd:0x%x, css:0x%x, vlan:0x%x\n", k, TX_descriptor_tmp->buffer,
		     TX_descriptor_tmp->length, TX_descriptor_tmp->CSO, TX_descriptor_tmp->CMD, TX_descriptor_tmp->STA, TX_descriptor_tmp->Rsvd,
		     TX_descriptor_tmp->CSS, TX_descriptor_tmp->VLAN);
      k++;
  }}

  //swap tail and head because driver increments tail

//down(&INR_PCI_tx_ring_sem);//block ring access
  volatile struct INR_PCI_tx_descriptor *TX_descriptor = INR_PCI_tx_descriptor_ring[INR_PCI_tx_descriptor_current];	//create temp. TDESC-structure
/*  if ((TX_descriptor->STA & 0x1) == 0)	//look for DD-Bit
  {
    INR_LOG_debug ("error descriptor not free\n");
    //up(&INR_PCI_tx_ring_sem);//free ring access
    while (INR_PCI_tx_descriptor_current != descriptor_current_lastwritten) {	//roleback
      INR_LOG_debug ("TX roleback\n");
      if (INR_PCI_tx_descriptor_current == 0) {
	INR_PCI_tx_descriptor_current = INR_PCI_tx_descriptor_ring_length - 1;
      }
      else {
	INR_PCI_tx_descriptor_current--;
      }				//decrement INR_PCI_tx_descriptor_current
      struct INR_PCI_tx_descriptor *TX_descriptor_roleback = INR_PCI_tx_descriptor_ring[INR_PCI_tx_descriptor_current];
      TX_descriptor_roleback->STA = 0x1;	//reset status

    }

    //restore descriptor on begin of malformed packet
    preempt_enable ();
    return 1;			//descriptor is not free, abort
  }
*/
  if ((TX_descriptor->STA & 0x1) == 0) {
    INR_PCI_BAR0_write ((head << 16) | (0xffff & tail), 0x7004);
//INR_LOG_debug ("error descriptor not free, tail:%i, head:%i\n",tail,head);
//txsemophore=0;
//return 1;
  }

  uint64_t dma_addr = dma_map_single (&globdev->dev, data, datalength, DMA_TO_DEVICE);	//map packet  to DMA
  if (dma_mapping_error (&globdev->dev, dma_addr))
    INR_LOG_debug ("TX dma mappng error! current:%i \n", INR_PCI_tx_descriptor_current);
  dma_sync_single_for_device (&globdev->dev, dma_addr, datalength, DMA_TO_DEVICE);

  if (TX_DBG_mod) {
    uint16_t tmp;
    INR_LOG_debug ("TX-packet len:%i eop:%i port:%i ll:%i\n", datalength, eof, port,ll);
    printk ("TXdump:");
    for (tmp = 0; tmp < datalength; tmp++) {
      if ((tmp % 40) == 0)
	printk ("\n");
      printk ("%x ", data[tmp]);
    }
    printk ("\n\n");
  }

//  if((TX_descriptor->STA & 0x1))
  if (TX_descriptor->buffer)	//if buffer was used before
  {
    //INR_PCI_BAR0_write (TX_descriptor->buffer, 0x7004);
    dma_unmap_single (&globdev->dev, TX_descriptor->buffer, TX_descriptor->length, DMA_TO_DEVICE);	//unmap DMA
    if (data_tx[INR_PCI_tx_descriptor_current].eop)
      if (data_tx[INR_PCI_tx_descriptor_current].skb)
	kfree_skb (data_tx[INR_PCI_tx_descriptor_current].skb);	//if last fragment of skb->free skb
    if (!data_tx[INR_PCI_tx_descriptor_current].paged)
      if (data_tx[INR_PCI_tx_descriptor_current].data)
	kfree (data_tx[INR_PCI_tx_descriptor_current].data);	//if not paged free kmalloc mem
  }


  //   INR_PCI_tx_unmapper (&data_tx[INR_PCI_tx_descriptor_current]);    //add fragment to unmapper que

//if(eof)INR_LOG_timelog("TX port:%i\n",port);

  TX_descriptor->buffer = cpu_to_le64 (dma_addr);	//store address of packet in descriptor-ring
  TX_descriptor->length = datalength;
  TX_descriptor->CSO = 0xff & 0x00;
  TX_descriptor->CMD = 0xff & ((1 & eof) | ((1 & ll) << 7));	//send eof and to-cpu-bit
//  if (!eof)    INR_LOG_debug ("eob not 1\n");
  TX_descriptor->STA = 0xf & 0x0;
  TX_descriptor->Rsvd = 0xf & port;	//triger output-port
  TX_descriptor->CSS = 0xff & 0x00;
  TX_descriptor->VLAN = 0xffff & 0;	//christians solution for giving thew port to the hardware
//INR_LOG_debug("TX length:%i, data:0x%x, dma:0x%x, eop:%i\n",datalength,data,dma_addr, eof);
  data_tx[INR_PCI_tx_descriptor_current].skb = skb;
//skb_tx_timestamp(skb);//setting timestamp
  data_tx[INR_PCI_tx_descriptor_current].length = datalength;
  data_tx[INR_PCI_tx_descriptor_current].paged = 1 & paged;
  data_tx[INR_PCI_tx_descriptor_current].eop = 1 & eof;
  data_tx[INR_PCI_tx_descriptor_current].data = data;
  data_tx[INR_PCI_tx_descriptor_current].dma = dma_addr;
//cacheflush(INR_PCI_tx_descriptor_ring[INR_PCI_tx_descriptor_current], INR_PCI_tx_descriptor_length,BCACHE);
//flush_cache_all();
  uint64_t descriptor_current_tmp = INR_PCI_tx_descriptor_current;	//protect curretn value
  if (INR_PCI_tx_descriptor_current == (INR_PCI_tx_descriptor_ring_length - 1)) {
    INR_PCI_tx_descriptor_current = 0;
  }
  else {
    INR_PCI_tx_descriptor_current++;
  }				//increment INR_PCI_tx_descriptor_current

  // if (eof) {
  if (eof) {
    INR_PCI_BAR0_write ((INR_PCI_tx_descriptor_length * (INR_PCI_tx_descriptor_current)), INR_PCI_tx_descriptor_tail_reg);	//if last fragment, update tailpointer
    descriptor_current_lastwritten = INR_PCI_tx_descriptor_current;
  }
//store address-offset of this last written descriptor in bar0
  //wake_up_interruptible (&INR_PCI_TX_unmapd_waittingqueu);       //wakeup TX_unmap_d
//  }

//up(&INR_PCI_tx_ring_sem);//free ring access
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
  INR_LOG_debug ("probing DMA_BIT_MASK\n");
  uint64_t j = 64;
//dev->coherent_dma_mask=0xffffffffffffffff;
//dev->dma_mask = &dev.coherent_dma_mask;

  for (j = 64; j > 0; j--) {
    INR_LOG_debug ("%i ", j);
    if (0 == dma_set_mask (&dev->dev, DMA_BIT_MASK (j)))
      break;
  }
  if (j)
    INR_LOG_debug ("set dma-mask to %i\n", j);
  else
    INR_LOG_debug ("fail to set DMA-Mask!\n");

  INR_LOG_debug ("probing DMA_BIT_MASK\n");

  for (j = 64; j > 0; j--) {
    INR_LOG_debug ("%i ", j);
    if (0 == dma_set_coherent_mask (&dev->dev, DMA_BIT_MASK (j)))
      break;
  }

  if (j)
    INR_LOG_debug ("dma_set_coherent_mask to %i\n", j);
  else
    INR_LOG_debug ("fail to set DMA_coherent-Mask!\n");

  INR_LOG_debug ("probing DMA_BIT_MASK\n");
  for (j = 64; j > 0; j--) {
    INR_LOG_debug ("%i ", j);
    if (0 == pci_set_dma_mask (dev, DMA_BIT_MASK (j)))
      break;
  }
  if (j)
    INR_LOG_debug ("set pci_set_dma_mask to %i\n", j);
  else
    INR_LOG_debug ("fail to set PCI-DMA-Mask!\n");
  //if(dma_set_mask (dev, DMA_BIT_MASK(64)))debug("fail to set DMA-Mask!\n");   //set width of DMA to 32bit
  //if(dma_set_coherent_mask (dev, DMA_BIT_MASK(64)))debug("fail to set DMA_coherent-Mask!\n");
  //if(pci_set_dma_mask (dev, DMA_BIT_MASK(64)))debug("fail to set PCI-DMA-Mask!\n");     //set width of DMA to 32bit

}

//*****************************************************************************************************************
/**
*remove t driver
*@param *dev PCI-device
*/
void
INR_remove_drv (struct pci_dev *dev)
{

  INR_UNMAP_all (dev);
  if (INR_STATUS_get (INR_STATUS_BUSMASTER))
    pci_clear_master (dev);	//enable Master
  if (INR_STATUS_get (INR_STATUS_BAR0))
    release_mem_region (gBaseHdwr, REGISTER_SIZE);	//release bar0
  if (INR_STATUS_get (INR_STATUS_TX_RING))
    pci_free_consistent (dev, INR_PCI_tx_descriptor_ring_length * INR_PCI_tx_descriptor_length, INR_PCI_tx_descriptor_base, INR_TX_Ring);	//release DMA-mem
  if (INR_STATUS_get (INR_STATUS_INT1))
    free_irq (gIrq, dev);	//release interrupt
  if (INR_STATUS_get (INR_STATUS_MSI))
    pci_disable_msi (dev);


  if (INR_STATUS_get (INR_STATUS_DEVICE))
    pci_disable_device (dev);	//disable device
  INR_LOG_debug ("device disabled\n\n");



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
  INR_LOG_debug ("Pagesize:%lli\n", pagesize);
  INR_LOG_debug ("Zerocopy: TX:%i RX:%i\n", zerocopy_tx, zerocopy_rx);
  INR_LOG_debug ("DEBUGMOD: TX:%i RX:%i\n", TX_DBG_mod, RX_DBG_mod);
  INR_LOG_debug ("AutorepeatonTXdrop: %i\n", INR_NW_repeatonbusy);

  rx_descriptor_pool_length = (pagesize / data_size_rx);	//count of descriptor in one page , pool is empty at start
  uint64_t i = 0;
  for (i = 0; i < INR_PCI_rx_descriptor_ring_count; i++)
    rx_descriptor_pool_free[i] = rx_descriptor_pool_length;
  gBaseHdwr = pci_resource_start (dev, 0);
  // Print Base Address to kernel log
  INR_LOG_debug ("Init: Base hw val %x\n", (unsigned int) gBaseHdwr);

  // Get the Base Address Length
  gBaseLen = pci_resource_len (dev, 0);
  // Print the Base Address Length to Kernel Log
  INR_LOG_debug ("Init: Base hw len %i\n", (unsigned int) gBaseLen);
  gBaseVirt = pci_iomap (dev, 0, 0);
  //gBaseVirt = ioremap (gBaseHdwr, gBaseLen);  //prepare bar0 for access
  INR_LOG_debug ("Init: Bar0 mapped\n");
  if (!gBaseVirt) {
    INR_LOG_debug ("Init: Could not remap memory.\n");
    return (1);
  }

/*no more available in Kernel 4.x
  if (0 > check_mem_region (gBaseHdwr, REGISTER_SIZE))	//check if wee con access bar0
  {
    INR_LOG_debug ("Init: Memory in use.\n");
    return (1);
  }
*/
  INR_LOG_debug ("Init: mem region checked\n");
  //INR_PCI_reset ();

  pci_set_master (dev);		//enable Master
  INR_LOG_debug ("Init: device mastered\n");
  INR_STATUS_set (INR_STATUS_BUSMASTER);


  INR_PROBE_dma (dev);
  INR_LOG_debug ("Init: try to get dma acces\n");
  // Try to gain exclusive control of memory for demo hardware.
  if (request_mem_region (gBaseHdwr, REGISTER_SIZE, "INR_mem"))
    INR_STATUS_set (INR_STATUS_BAR0);
  INR_LOG_debug ("Init: dma mem requested\n");
  if (INR_int_enable (dev))
    return 1;
  else
    INR_STATUS_set (INR_STATUS_INT1);
//INR_PCI_BAR0_write(12345,0);
//debug("test %i\n",INR_PCI_BAR0_read(0));
  kthread_run (&INR_PCI_rx_pageallocator, NULL, "INR_rx_pageallocator");
  wake_up_interruptible (&INR_PCI_rx_pageallocator_waittingqueu);	//wakeop preallocator
  INR_LOG_debug ("Page preallocator is running.\n");
  INR_TX_RING_init (dev);
//  uint8_t i = 0;
  for (i = 0; i < INR_PCI_rx_descriptor_ring_count; i++)
    INR_RX_RING_init (dev, i);	//init all RX-Rings
  if (NAPI_enable)
    for (i = 0; i < INR_PCI_rx_descriptor_ring_count; i++)
      INR_NAPI_init (i);

// kthread_run (&INR_PCI_ringtailtest, NULL, "INR_PCI_ringtailtest");
//wake_up_interruptible (&INR_tailtest);
  //*testdata_rx

  //INR_PCI_BAR0_write (dma_rx, INR_RX_buffer);     //set bottom-pointer =0
  //debug ("write RX-Buffer address 0x%x to 0x%x\n",
  //INR_PCI_BAR0_read (INR_RX_buffer), INR_RX_buffer);

//uint16_t i = 0;
/*
  uint8_t *testdata = kmalloc (testdate_size, GFP_KERNEL);	//packetdata somewhere in kernelspace

  
  for (i = 0; i < testdate_size; i++)
    testdata[i] = i;		//fill testdata with ramp

  INR_LOG_debug ("init test data\n");

  dma_addr_t dma = dma_map_single (&dev->dev, testdata, testdate_size, DMA_TO_DEVICE);	//map packet  to DMA
  INR_LOG_debug ("map testdate to dma: cpu_add=%x  pci_addr=%x\n", &testdata, dma);
  INR_TX_push (dev, dma, testdate_size, 1,0,1);	//push packet to descriptorring
*/
  /*INR_TX_push (dev, dma, testdate_size, 1);   //push packet to descriptorring
     INR_TX_push (dev, dma, testdate_size, 1);  //push packet to descriptorring
     INR_TX_push (dev, dma, testdate_size, 1);  //push packet to descriptorring
     INR_TX_push (dev, dma, testdate_size, 1);  //push packet to descriptorring
     INR_TX_push (dev, dma, testdate_size, 1);  //push packet to descriptorring
     INR_TX_push (dev, dma, testdate_size, 1);  //push packet to descriptorring
     INR_TX_push (dev, dma, testdate_size, 1);  //push packet to descriptorring
   */
/*
  INR_LOG_timelog ("packet pushed\n");
  //INR_TX_push (dev, dma, testdate_size);
  INR_LOG_debug ("testpacket pushed to TX-Ring\n");
  INR_LOG_debug ("wait fo send complet...\n");
  i = 0;
  while (INR_PCI_BAR0_read (INR_PCI_tx_descriptor_head_reg) !=
	 INR_PCI_BAR0_read (INR_PCI_tx_descriptor_tail_reg))
    {
      i++;
      if (i >= 60000)
	{
	  INR_LOG_debug ("stop waiting...needs to long\n");
	  break;
	}
    }
  INR_LOG_debug ("TX_Ring_head:0x%x\n",
		 INR_PCI_BAR0_read (INR_PCI_tx_descriptor_head_reg));
  //INR_PRINT_DESC_status (dev, INR_PCI_tx_descriptor_current - 2);
  INR_PRINT_DESC_status (dev, INR_PCI_tx_descriptor_current - 1);
*/
  INR_LOG_debug ("Hardware setup done\n");
  INR_STATUS_set (INR_STATUS_HW_RUNNING);


}
