/**
*@file 
*@brief Functions for NewNetworkAPI
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
#include <linux/kthread.h>
#include <linux/skbuff.h>
#include <linux/unistd.h>
#include "INR.h"
#include "INR-NW.h"
#include "INR-PCI.h"
#include "INR-NAPI.h"
struct napi_struct INR_NAPI_rx[INR_PCI_rx_descriptor_ring_count];



//*****************************************************************************************************************
/**
*init NAPI for specal netdev device
*@param index index of netdev/RX_descriptorring
*/
void
INR_NAPI_init (uint8_t index)
{
  INR_LOG_debug ("Init NAPI for ring %i\n", index);

  switch (index) {
  case 0:
    netif_napi_add (get_nwdev (index), &INR_NAPI_rx[index], INR_NAPI_poll_0, INR_NAPI_maxpoll);
    break;
  case 1:
    netif_napi_add (get_nwdev (index), &INR_NAPI_rx[index], INR_NAPI_poll_1, INR_NAPI_maxpoll);
    break;
  case 2:
    netif_napi_add (get_nwdev (index), &INR_NAPI_rx[index], INR_NAPI_poll_2, INR_NAPI_maxpoll);
    break;
  case 3:
    netif_napi_add (get_nwdev (index), &INR_NAPI_rx[index], INR_NAPI_poll_3, INR_NAPI_maxpoll);
    break;
  case 4:
    netif_napi_add (get_nwdev (index), &INR_NAPI_rx[index], INR_NAPI_poll_4, INR_NAPI_maxpoll);
    break;
  case 5:
    netif_napi_add (get_nwdev (index), &INR_NAPI_rx[index], INR_NAPI_poll_5, INR_NAPI_maxpoll);
    break;
  case 6:
    netif_napi_add (get_nwdev (index), &INR_NAPI_rx[index], INR_NAPI_poll_6, INR_NAPI_maxpoll);
    break;
  case 7:
    netif_napi_add (get_nwdev (index), &INR_NAPI_rx[index], INR_NAPI_poll_7, INR_NAPI_maxpoll);
    break;
  case 8:
    netif_napi_add (get_nwdev (index), &INR_NAPI_rx[index], INR_NAPI_poll_8, INR_NAPI_maxpoll);
    break;
  case 9:
    netif_napi_add (get_nwdev (index), &INR_NAPI_rx[index], INR_NAPI_poll_9, INR_NAPI_maxpoll);
    break;
  case 10:
    netif_napi_add (get_nwdev (index), &INR_NAPI_rx[index], INR_NAPI_poll_10, INR_NAPI_maxpoll);
    break;
  case 11:
    netif_napi_add (get_nwdev (index), &INR_NAPI_rx[index], INR_NAPI_poll_11, INR_NAPI_maxpoll);
    break;
  case 12:
    netif_napi_add (get_nwdev (index), &INR_NAPI_rx[index], INR_NAPI_poll_12, INR_NAPI_maxpoll);
    break;
  case 13:
    netif_napi_add (get_nwdev (index), &INR_NAPI_rx[index], INR_NAPI_poll_13, INR_NAPI_maxpoll);
    break;
  case 14:
    netif_napi_add (get_nwdev (index), &INR_NAPI_rx[index], INR_NAPI_poll_14, INR_NAPI_maxpoll);
    break;
  case 15:
    netif_napi_add (get_nwdev (index), &INR_NAPI_rx[index], INR_NAPI_poll_15, INR_NAPI_maxpoll);
    break;
  default:
    break;
  }
  napi_enable (&INR_NAPI_rx[index]);




}

//*****************************************************************************************************************
/**
*deinit NAPI for specal netdev device
*@param index index of netdev/RX_descriptorring
*/
void
INR_NAPI_remove (uint8_t index)
{
  netif_napi_del (&INR_NAPI_rx[index]);
}

//*****************************************************************************************************************
/**
*enable NAPI sceduling for sec netdev
*@param index index of netdev/RX_descriptorring
*/
void
INR_NAPI_schedule (uint8_t index)
{

  napi_schedule (&INR_NAPI_rx[index]);	//enable NAPI poll
  //INR_LOG_debug ("Enable NAPI polling for Ring %i\n", index);

}

//*****************************************************************************************************************
/**
*disable NAPI sceduling for sec netdev
*@param index index of netdev/RX_descriptorring
*/
void
INR_NAPI_complete (uint8_t index)
{

  napi_complete (&INR_NAPI_rx[index]);	//enable NAPI poll
  //INR_LOG_debug ("Disable NAPI polling for Ring %i\n", index);

}

//*****************************************************************************************************************
/**
*NAPI poll fkt for ring 0
*@param *napi NAPI
*@param budget how much to poll max
*/
int
INR_NAPI_poll_0 (struct napi_struct *napi, int budget)
{
  uint16_t ret = INR_PCI_process_rx_descriptor_ring (0);	//poll paketes form descriptor ring
  if (ret < budget)
    INR_NAPI_complete (0);	// if less packets then budget, disable polling
  return ret;
}

//*****************************************************************************************************************
/**
*NAPI poll fkt for ring 0
*@param *napi NAPI
*@param budget how much to poll max
*/
int
INR_NAPI_poll_1 (struct napi_struct *napi, int budget)
{
  uint16_t ret = INR_PCI_process_rx_descriptor_ring (1);	//poll paketes form descriptor ring
  if (ret < budget)
    INR_NAPI_complete (1);	// if less packets then budget, disable polling
  return ret;
}

//*****************************************************************************************************************
/**
*NAPI poll fkt for ring 0
*@param *napi NAPI
*@param budget how much to poll max
*/
int
INR_NAPI_poll_2 (struct napi_struct *napi, int budget)
{
  uint16_t ret = INR_PCI_process_rx_descriptor_ring (2);	//poll paketes form descriptor ring
  if (ret < budget)
    INR_NAPI_complete (2);	// if less packets then budget, disable polling
  return ret;
}

//*****************************************************************************************************************
/**
*NAPI poll fkt for ring 0
*@param *napi NAPI
*@param budget how much to poll max
*/
int
INR_NAPI_poll_3 (struct napi_struct *napi, int budget)
{
  uint16_t ret = INR_PCI_process_rx_descriptor_ring (3);	//poll paketes form descriptor ring
  if (ret < budget)
    INR_NAPI_complete (3);	// if less packets then budget, disable polling
  return ret;
}

//*****************************************************************************************************************
/**
*NAPI poll fkt for ring 0
*@param *napi NAPI
*@param budget how much to poll max
*/
int
INR_NAPI_poll_4 (struct napi_struct *napi, int budget)
{
  uint16_t ret = INR_PCI_process_rx_descriptor_ring (4);	//poll paketes form descriptor ring
  if (ret < budget)
    INR_NAPI_complete (4);	// if less packets then budget, disable polling
  return ret;
}

//*****************************************************************************************************************
/**
*NAPI poll fkt for ring 0
*@param *napi NAPI
*@param budget how much to poll max
*/
int
INR_NAPI_poll_5 (struct napi_struct *napi, int budget)
{
  uint16_t ret = INR_PCI_process_rx_descriptor_ring (5);	//poll paketes form descriptor ring
  if (ret < budget)
    INR_NAPI_complete (5);	// if less packets then budget, disable polling
  return ret;
}

//*****************************************************************************************************************
/**
*NAPI poll fkt for ring 0
*@param *napi NAPI
*@param budget how much to poll max
*/
int
INR_NAPI_poll_6 (struct napi_struct *napi, int budget)
{
  uint16_t ret = INR_PCI_process_rx_descriptor_ring (6);	//poll paketes form descriptor ring
  if (ret < budget)
    INR_NAPI_complete (6);	// if less packets then budget, disable polling
  return ret;
}

//*****************************************************************************************************************
/**
*NAPI poll fkt for ring 0
*@param *napi NAPI
*@param budget how much to poll max
*/
int
INR_NAPI_poll_7 (struct napi_struct *napi, int budget)
{
  uint16_t ret = INR_PCI_process_rx_descriptor_ring (7);	//poll paketes form descriptor ring
  if (ret < budget)
    INR_NAPI_complete (7);	// if less packets then budget, disable polling
  return ret;
}

//*****************************************************************************************************************
/**
*NAPI poll fkt for ring 0
*@param *napi NAPI
*@param budget how much to poll max
*/
int
INR_NAPI_poll_8 (struct napi_struct *napi, int budget)
{
  uint16_t ret = INR_PCI_process_rx_descriptor_ring (8);	//poll paketes form descriptor ring
  if (ret < budget)
    INR_NAPI_complete (8);	// if less packets then budget, disable polling
  return ret;
}

//*****************************************************************************************************************
/**
*NAPI poll fkt for ring 0
*@param *napi NAPI
*@param budget how much to poll max
*/
int
INR_NAPI_poll_9 (struct napi_struct *napi, int budget)
{
  uint16_t ret = INR_PCI_process_rx_descriptor_ring (9);	//poll paketes form descriptor ring
  if (ret < budget)
    INR_NAPI_complete (9);	// if less packets then budget, disable polling
  return ret;
}

//*****************************************************************************************************************
/**
*NAPI poll fkt for ring 0
*@param *napi NAPI
*@param budget how much to poll max
*/
int
INR_NAPI_poll_10 (struct napi_struct *napi, int budget)
{
  uint16_t ret = INR_PCI_process_rx_descriptor_ring (10);	//poll paketes form descriptor ring
  if (ret < budget)
    INR_NAPI_complete (10);	// if less packets then budget, disable polling
  return ret;
}

//*****************************************************************************************************************
/**
*NAPI poll fkt for ring 0
*@param *napi NAPI
*@param budget how much to poll max
*/
int
INR_NAPI_poll_11 (struct napi_struct *napi, int budget)
{
  uint16_t ret = INR_PCI_process_rx_descriptor_ring (11);	//poll paketes form descriptor ring
  if (ret < budget)
    INR_NAPI_complete (11);	// if less packets then budget, disable polling
  return ret;
}

//*****************************************************************************************************************
/**
*NAPI poll fkt for ring 0
*@param *napi NAPI
*@param budget how much to poll max
*/
int
INR_NAPI_poll_12 (struct napi_struct *napi, int budget)
{
  uint16_t ret = INR_PCI_process_rx_descriptor_ring (12);	//poll paketes form descriptor ring
  if (ret < budget)
    INR_NAPI_complete (12);	// if less packets then budget, disable polling
  return ret;
}

//*****************************************************************************************************************
/**
*NAPI poll fkt for ring 0
*@param *napi NAPI
*@param budget how much to poll max
*/
int
INR_NAPI_poll_13 (struct napi_struct *napi, int budget)
{
  uint16_t ret = INR_PCI_process_rx_descriptor_ring (13);	//poll paketes form descriptor ring
  if (ret < budget)
    INR_NAPI_complete (13);	// if less packets then budget, disable polling
  return ret;
}

//*****************************************************************************************************************
/**
*NAPI poll fkt for ring 0
*@param *napi NAPI
*@param budget how much to poll max
*/
int
INR_NAPI_poll_14 (struct napi_struct *napi, int budget)
{
  uint16_t ret = INR_PCI_process_rx_descriptor_ring (14);	//poll paketes form descriptor ring
  if (ret < budget)
    INR_NAPI_complete (14);	// if less packets then budget, disable polling
  return ret;
}

//*****************************************************************************************************************
/**
*NAPI poll fkt for ring 0
*@param *napi NAPI
*@param budget how much to poll max
*/
int
INR_NAPI_poll_15 (struct napi_struct *napi, int budget)
{
  uint16_t ret = INR_PCI_process_rx_descriptor_ring (15);	//poll paketes form descriptor ring
  if (ret < budget)
    INR_NAPI_complete (15);	// if less packets then budget, disable polling
  return ret;
}
