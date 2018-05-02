#include "tn_env.h"
#define PRINT_ERROR \
	do { \
		fprintf(stderr, "Error at line %d, file %s (%d) [%s]\n", \
		__LINE__, __FILE__, errno, strerror(errno)); exit(1); \
	} while(0)

#define MAP_SIZE 16384000UL
#define MAP_MASK (MAP_SIZE - 1)

#define PHY0_base (C_BASE_ADDR_MAC_0<<8)
#define PHY1_base (C_BASE_ADDR_MAC_1<<8)
#define PHY2_base (C_BASE_ADDR_MAC_2<8)
#define PHY3_base (C_BASE_ADDR_MAC_3<<8)
#define PHY4_base (C_BASE_ADDR_MAC_4<<8)
#define PHY5_base (C_BASE_ADDR_MAC_5<<8)
#define PHY6_base (C_BASE_ADDR_MAC_6<<8)
#define PHY7_base (C_BASE_ADDR_MAC_7<<8)
#define PHY8_base (C_BASE_ADDR_MAC_8<<8)
#define PHY9_base (C_BASE_ADDR_MAC_9<<8)
#define PHY10_base (C_BASE_ADDR_MAC_10<<8)
#define PHY11_base (C_BASE_ADDR_MAC_11<<8)

#define INR_6T_ADDR_L C_SUB_ADDR_MAC_6T_ADDR_L
#define INR_6T_ADDR_H C_SUB_ADDR_MAC_6T_ADDR_H //high bits excluding "AF"
#define INR_6T_SRC_PTR C_SUB_ADDR_MAC_6T_SRC_PTR
#define INR_6T_DST_PTR C_SUB_ADDR_MAC_6T_DST_PTR
#define INR_6T_own_MAC_L C_SUB_ADDR_MAC_OWN_MAC_ADDR_L
#define INR_6T_own_MAC_H C_SUB_ADDR_MAC_OWN_MAC_ADDR_H
