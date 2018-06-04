#include "tn_env.h"

uint8_t INR_accdp_check (void);
void INR_ACCDP_init (uint64_t * baseaddr, uint64_t * baseaddr_shadow);
uint64_t INR_ACCDP_get_addr (uint64_t id);
uint64_t INR_ACCDP_shadow_get_addr (uint64_t id);
uint8_t INR_ACCDP_clear_entry (uint64_t id);
uint64_t INR_ACCDP_get_next_free_entry (uint64_t id);
uint64_t INR_ACCDP_getbase (uint8_t i);

#define INR_ACCDP_available (C_SUB_ADDR_COMMON_FEATURES_ACC_DP)
#define INR_ACC_FIELD_TABLE_0 (C_BASE_ADDR_ACC_0<<8)
#define INR_ACC_FIELD_TABLE_1 (C_BASE_ADDR_ACC_1<<8)

#define  INR_ACCDP_entry_length  12	//length of entry in byte + stuffbits
#define  INR_ACCDP_entry_length_memcpy 12	//length of entry in byte

struct INR_ACCDP_RULE
{
  uint16_t enable:1;
  uint16_t reserved:7;
  uint16_t IPv4_12lsb:12;
  uint16_t UDP_DST:16;
  uint16_t NAL_ID:6;
  uint16_t pad1:6;
  uint16_t pad2;
  uint16_t pad3;
  uint16_t pad4;

} __attribute__ ((__packed__));
