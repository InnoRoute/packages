/**
*@file
*@brief settings and definitions PCI communication
*@author M.Ulbricht 2015
**/
int INR_init_drv (struct pci_dev *dev);
void INR_UNMAP_all (struct pci_dev *dev);
void INR_PCI_reset (void);
void INR_remove_drv (struct pci_dev *dev);
static irqreturn_t XPCIe_IRQHandler (int irq, void *dev_id, struct pt_regs *regs);
int INR_TX_push (struct sk_buff *skb, uint8_t * data, uint16_t datalength, uint8_t eof, uint8_t port, uint8_t ll, uint8_t paged, uint16_t fragcount);
void INR_PCI_alloc_new_rx_skb (uint16_t current_descriptor, uint16_t ringindex);
struct INR_PCI_rx_descriptor_ring_entry *INR_PCI_get_new_rx_descriptor_ring_entry (uint8_t index);
void INR_PCI_rx_pageallocator (void *nix);
uint16_t INR_PCI_process_rx_descriptor_ring (uint8_t index);
uint16_t INR_PCI_pointerdistance (uint16_t tail, uint16_t head, uint16_t max);
void INR_PCI_zerovars(void);
void set_tx_dbg(uint8_t txdbg);
void set_rx_dbg(uint8_t rxdbg);
uint8_t get_rx_dbg(void);
uint8_t get_tx_dbg(void);

#define INR_PCI_TX_maxfragments 16  /*<maximum number of segments the hardware supports */
#define INR_PCI_FPGA_max_tx_length  1600	/*<number of bytes the FPGA accept in one TX packet */
#define INR_PCI_tx_descriptor_ring_length   128 /*<number of TDESCs which can be stored in TX-Ring */	//INR_TX_Ring_length
#define INR_PCI_tx_descriptor_length 16/*<length of one TDESC in Bytes*/	//INR_TDESC_length

#define INR_PCI_rx_descriptor_ring_length 4048 /*<number of TDESCs which can be stored in TX-Ring */	//INR_RX_Ring_lengthdia add shape
#define INR_PCI_rx_descriptor_length 16/*<length of one TDESC in Bytes*/	//INR_RDESC_length
#define INR_PCI_rx_max_loops INR_PCI_rx_descriptor_ring_length*2

#define INR_PCI_version 7000	/*<Register for hardware version */

#define INR_PCI_BAR0_read(addr)		readl(gBaseVirt0+addr)	/*<read from bar0 */
#define INR_PCI_BAR0_write(content, addr)	writel(content, (gBaseVirt0 + addr))	/*<write to bar0 */
#define INR_PCI_BAR1_read(addr)		readl(gBaseVirt1+addr)	/*<read from bar0 */
#define INR_PCI_BAR1_write(content, addr)	writel(content, (gBaseVirt1 + addr))	/*<write to bar0 */

#define INR_PCI_BAR0_read64(addr)		((((uint64_t)readl(gBaseVirt0+addr+0x4))<<32)|((uint64_t)readl(gBaseVirt0+addr)))	/*<read from bar0 */
#define INR_PCI_BAR0_write64(content, addr)	writel(content&0xffffffff, (gBaseVirt0 + addr));writel((content>>32)&0xffffffff, (gBaseVirt0 + addr+0x4));	/*<write to bar0 */

#define testdate_size 	300	/*<size of the dma-test packet in byte */
#define data_size_rx 	64/*<size of the rx descriptors in byte*/	//das ist zu wenig...!
#define data_size_tx 	64	/*<max fragmentsize of the transmitted packets in byte */
#define INR_NW_TX_fragmentation 0	/*<enable TX fragmentation */
#define zerocopy_rx	1	/*<enable rx zerocopy packet handling */
#define zerocopy_tx	1	/*<enable tx zerocopy packet handling */
#define INR_PCI_page_prealloc_count 32	/*<prealloc pages */

#define INR_PCI_tx_descriptor_base_address_reg 	0x6000/*<TX ring config register*/	//INR_TX_Ring_address
#define INR_PCI_tx_descriptor_base_address_reg_h 	0x6004/*<TX ring config register high*/	//INR_TX_Ring_address
#define INR_PCI_tx_descriptor_tail_reg  0x6018/*<TX ring bottom pointer register*/	//INR_TX_Ring_bottom
#define INR_PCI_tx_descriptor_head_reg  0x6010/*<TX ring head pointer register*/	//INR_TX_Ring_head
#define INR_PCI_tx_descriptor_length_reg    0x6008/*<Register for length of descriptorring*/	//INR_TDLEN

#define INR_PCI_rx_descriptor_base_address_reg 	0x1000/*<RX ring config register*/	//INR_RX_Ring_address
#define INR_PCI_rx_descriptor_base_address_reg_h 	0x1004 /*<RX ring config register high*/	//INR_RX_Ring_address
#define INR_PCI_rx_descriptor_tail_reg		0x1018/*<RX ring bottom pointer register*/	//INR_RX_Ring_bottom
#define INR_PCI_rx_descriptor_head_reg		0x1010/*<RX ring head pointer register*/	//INR_RX_Ring_head
#define INR_PCI_rx_descriptor_length_reg	0x1008/*<Register for length of descriptorring*/	//INR_RDLEN
#define INR_PCI_rx_descriptor_ring_count 	16	/*<cout of RX-Descriptor-rings */
#define INR_PCI_interrupt_cause_reg		0x0800	/*<which rx queue trigger interrupt */
#define INR_PCI_error_LED_addr			0xC0008 /*address of error LED on BAR1*/

#define INR_PCI_enable_error_LED INR_PCI_BAR1_write(((INR_PCI_BAR1_read(INR_PCI_error_LED_addr))|(1<<10)),INR_PCI_error_LED_addr);
#define INR_PCI_disable_error_LED INR_PCI_BAR1_write(((INR_PCI_BAR1_read(INR_PCI_error_LED_addr))&(~(1<<10))),INR_PCI_error_LED_addr);

//****************************************************************************************************************
/**
*TX-descriptor ring entry
*/
struct INR_PCI_tx_descriptor_ring_entry
{
    struct sk_buf *skb; /**<store skb*/
    uint8_t eop:1;      /**<is last fragment of packet?*/
    uint8_t paged:1;    /**<is paged fragment?*/
    uint8_t *data;      /**<address of data*/
    uint8_t *dma;      /**<address of dma*/
    uint16_t length;   /**<length of data*/
};

//****************************************************************************************************************
/**
*RX-descriptor ring entry
*/
struct INR_PCI_rx_descriptor_ring_entry
{
    uint8_t fragmentindex;/**<dindex of this fragment in the page*/
    uint64_t data;        /**<data pointer*/
    uint64_t dma;         /**<dama pointer*/
    struct page *page;    /**<page start*/
    uint64_t dma_root;    /**<pointer to page root*/
    uint64_t offset;	/**<ofset of data in page*/
};

//****************************************************************************************************************
/*
*TX-descriptor entry, see doku
*/
struct INR_PCI_tx_descriptor
{
    uint64_t buffer;	/**<data-buffer*/
    uint16_t length;	/**<length of data*/
    uint8_t CSO;
    uint8_t CMD;	/**<special commsnds: checksum etc.*/
    uint8_t STA:4;	/**<status-bits*/
    uint8_t Rsvd:4;
    uint8_t CSS;
    uint16_t VLAN;	/**<VLAN-control*/
};
__attribute__ ((__packed__));

//****************************************************************************************************************
/*
*RX-descriptor entry, see doku
*/
struct INR_PCI_rx_descriptor
{
    uint64_t buffer;      	/**<data-buffer*/
    uint16_t length;      	/**<length of data*/
    uint16_t FragmentCS;  	/**<fragment checksum*/
    uint8_t Status;      	/**<Status*/
    uint8_t Errors;      	/**<Errors*/
    uint16_t VLAN;       	/*<VLAN-control */
};
__attribute__ ((__packed__));

void INR_PCI_tx_unmapper (struct INR_PCI_tx_descriptor_ring_entry *entry);
