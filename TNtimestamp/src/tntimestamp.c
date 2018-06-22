/*
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 * original code from https://gist.github.com/austinmarton/1922600
 * extended to TSN test program ulbricht@innoroute.de 2018
 */

#include <arpa/inet.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <net/if.h>
#include <netinet/ether.h>
#include <errno.h>
#include <linux/net_tstamp.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <net/if.h>

#include <asm/types.h>
#include <linux/net_tstamp.h>
#include <linux/errqueue.h>
#include <linux/if_packet.h>

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include <sys/time.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <net/if.h>

#include <asm/types.h>
#include <linux/net_tstamp.h>
#include <linux/errqueue.h>

#ifndef SIOCSHWTSTAMP
	# define SIOCSHWTSTAMP 0x89b0
#endif
#ifndef PACKET_INR_EXT
	#define PACKET_INR_EXT	666
#endif


#define MY_DEST_MAC0	0x66
#define MY_DEST_MAC1	0x60
#define MY_DEST_MAC2	0x12
#define MY_DEST_MAC3	0x34
#define MY_DEST_MAC4	0x56
#define MY_DEST_MAC5	0x78

#define DEFAULT_IF	"eth0"
#define BUF_SIZ		1024
void die(char* s)
{
    perror(s);
    //exit(1);
}
static void bail(const char *error)
{
	printf("%s: %s\n", error, strerror(errno));
	//exit(1);
}
int main(int argc, char *argv[])
{
	int sockfd;
	struct ifreq if_idx;
	struct ifreq if_mac;
	int so_timestamping_flags = 0;
	struct ifreq hwtstamp;
	struct hwtstamp_config hwconfig, hwconfig_requested;
	int tx_len = 0;
	char sendbuf[BUF_SIZ];
	struct ether_header *eh = (struct ether_header *) sendbuf;
	struct iphdr *iph = (struct iphdr *) (sendbuf + sizeof(struct ether_header));
	struct sockaddr_ll socket_address;
	char ifName[IFNAMSIZ];
	
	/* Get interface name */
	if (argc > 1)
		strcpy(ifName, argv[1]);
	else
		strcpy(ifName, DEFAULT_IF);

	/* Open RAW socket to send on */
	if ((sockfd = socket(AF_PACKET, SOCK_RAW, IPPROTO_RAW)) == -1) {
	    perror("socket");
	}

	/* Get the index of the interface to send on */
	memset(&if_idx, 0, sizeof(struct ifreq));
	strncpy(if_idx.ifr_name, ifName, IFNAMSIZ-1);
	if (ioctl(sockfd, SIOCGIFINDEX, &if_idx) < 0)
	    perror("SIOCGIFINDEX");
	/* Get the MAC address of the interface to send on */
	memset(&if_mac, 0, sizeof(struct ifreq));
	strncpy(if_mac.ifr_name, ifName, IFNAMSIZ-1);
	if (ioctl(sockfd, SIOCGIFHWADDR, &if_mac) < 0)
	    perror("SIOCGIFHWADDR");

	/* Construct the Ethernet header */
	memset(sendbuf, 0, BUF_SIZ);
	/* Ethernet header */
	eh->ether_shost[0] = ((uint8_t *)&if_mac.ifr_hwaddr.sa_data)[0];
	eh->ether_shost[1] = ((uint8_t *)&if_mac.ifr_hwaddr.sa_data)[1];
	eh->ether_shost[2] = ((uint8_t *)&if_mac.ifr_hwaddr.sa_data)[2];
	eh->ether_shost[3] = ((uint8_t *)&if_mac.ifr_hwaddr.sa_data)[3];
	eh->ether_shost[4] = ((uint8_t *)&if_mac.ifr_hwaddr.sa_data)[4];
	eh->ether_shost[5] = ((uint8_t *)&if_mac.ifr_hwaddr.sa_data)[5];
	eh->ether_dhost[0] = MY_DEST_MAC0;
	eh->ether_dhost[1] = MY_DEST_MAC1;
	eh->ether_dhost[2] = MY_DEST_MAC2;
	eh->ether_dhost[3] = MY_DEST_MAC3;
	eh->ether_dhost[4] = MY_DEST_MAC4;
	eh->ether_dhost[5] = MY_DEST_MAC5;
	/* Ethertype field */
	eh->ether_type = htons(ETH_P_IP);
	tx_len += sizeof(struct ether_header);

	/* Packet data */
	sendbuf[tx_len++] = 0xde;
	sendbuf[tx_len++] = 0xad;
	sendbuf[tx_len++] = 0xbe;
	sendbuf[tx_len++] = 0xef;

	/* Index of the network device */
	socket_address.sll_ifindex = if_idx.ifr_ifindex;
	/* Address length*/
	socket_address.sll_halen = ETH_ALEN;
	/* Destination MAC */
	socket_address.sll_addr[0] = MY_DEST_MAC0;
	socket_address.sll_addr[1] = MY_DEST_MAC1;
	socket_address.sll_addr[2] = MY_DEST_MAC2;
	socket_address.sll_addr[3] = MY_DEST_MAC3;
	socket_address.sll_addr[4] = MY_DEST_MAC4;
	socket_address.sll_addr[5] = MY_DEST_MAC5;


	so_timestamping_flags |= SOF_TIMESTAMPING_RX_HARDWARE;
	so_timestamping_flags |= SOF_TIMESTAMPING_TX_HARDWARE;
	memset(&hwtstamp, 0, sizeof(hwtstamp));
	strncpy(hwtstamp.ifr_name, ifName, sizeof(hwtstamp.ifr_name));
	hwtstamp.ifr_data = (void *)&hwconfig;
	memset(&hwconfig, 0, sizeof(hwconfig));
	hwconfig.tx_type =
		(so_timestamping_flags & SOF_TIMESTAMPING_TX_HARDWARE) ?
		HWTSTAMP_TX_ON : HWTSTAMP_TX_OFF;
	hwconfig.rx_filter =
		(so_timestamping_flags & SOF_TIMESTAMPING_RX_HARDWARE) ?
		HWTSTAMP_FILTER_PTP_V1_L4_SYNC : HWTSTAMP_FILTER_NONE;
	hwconfig_requested = hwconfig;
	if (ioctl(sockfd, SIOCSHWTSTAMP, &hwtstamp) < 0) {
		if ((errno == EINVAL || errno == ENOTSUP) &&
		    hwconfig_requested.tx_type == HWTSTAMP_TX_OFF &&
		    hwconfig_requested.rx_filter == HWTSTAMP_FILTER_NONE)
			printf("SIOCSHWTSTAMP: disabling hardware time stamping not possible\n");
		else
			bail("SIOCSHWTSTAMP");
	}
	printf("SIOCSHWTSTAMP: tx_type %d requested, got %d; rx_filter %d requested, got %d\n",
	       hwconfig_requested.tx_type, hwconfig.tx_type,
	       hwconfig_requested.rx_filter, hwconfig.rx_filter);
	       
	if(setsockopt(sockfd, SOL_SOCKET, SO_TIMESTAMPING, &so_timestamping_flags, sizeof(so_timestamping_flags)) < 0)
	bail("setsockopt SO_TIMESTAMPING");
	int one = 1;
	if(setsockopt(sockfd, SOL_PACKET, PACKET_QDISC_BYPASS, &one, sizeof(one)) < 0)
	bail("setsockopt PACKET_QDISC_BYPASS");
	int inr_ext=666;
	if(setsockopt(sockfd, SOL_PACKET, PACKET_INR_EXT, &inr_ext, sizeof(inr_ext)) < 0)
	bail("setsockopt PACKET_INR_EXT");



	/* Send packet */
	if (sendto(sockfd, sendbuf, tx_len, 0, (struct sockaddr*)&socket_address, sizeof(struct sockaddr_ll)) < 0)
	    printf("Send failed\n");


	int res;
	char data[256];
        struct msghdr msg;
        struct iovec entry;
        struct sockaddr_in from_addr;
        struct {
            struct cmsghdr cm;
            char control[512];
        } control;
        memset(&msg, 0, sizeof(msg));
        msg.msg_iov = &entry;
        msg.msg_iovlen = 1;
        entry.iov_base = data;
        entry.iov_len = sizeof(data);
        msg.msg_name = (caddr_t)&from_addr;
        msg.msg_namelen = sizeof(from_addr);
        msg.msg_control = &control;
        msg.msg_controllen = sizeof(control);        
        if (recvmsg(sockfd, &msg, MSG_ERRQUEUE) < 0) {
            die("recvmsg()");
        }

	return 0;
}