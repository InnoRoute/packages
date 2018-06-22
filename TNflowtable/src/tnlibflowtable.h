#include "tn_env.h"


void FCinit_EMH (uint64_t * baseaddr, uint64_t * baseaddr_shadow);
uint8_t INR_HashTable_EMH_clear_entry (uint64_t id);
uint64_t INR_RuleTable_EMH_get_addr (uint64_t id);
uint8_t INR_RuleTable_EMH_clear_entry (uint64_t id);
uint64_t INR_HashTable_EMH_get_addr (uint64_t id);
uint64_t INR_HashTable_EMH_shadow_get_addr (uint64_t id);
uint64_t INR_RuleTable_EMH_shadow_get_addr (uint64_t id);
uint64_t INR_RuleTable_EMH_get_next_free_entry (uint64_t id);
uint64_t INR_HashTable_EMA_get_next_free_entry (uint64_t id);

uint64_t INR_CTable_EMH_shadow_get_addr (uint64_t id);
uint64_t INR_CTable_EMH_get_next_free_entry (uint64_t id);
uint64_t INR_CTable_EMH_get_addr (uint64_t id);
uint8_t INR_CTable_EMH_clear_entry (uint64_t id);

uint64_t INR_RuleTable_EMA_shadow_get_addr (uint64_t id);
uint64_t INR_RuleTable_EMA_get_next_free_entry (uint64_t id);
uint64_t INR_RuleTable_EMA_get_addr (uint64_t id);
uint8_t INR_RuleTable_EMA_clear_entry (uint64_t id);

uint64_t INR_ActT_shadow_get_addr (uint64_t id);
uint64_t INR_ActT_get_next_free_entry (uint64_t id,uint8_t have_PQUEUE, uint8_t PQUEUE);
uint64_t INR_ActT_get_addr (uint64_t id);
uint8_t INR_ActT_clear_entry (uint64_t id);
void FCmemcpy (void *dst, const void *src, size_t len);
uint8_t INR_FC_get_HW_write (void);
void INR_FC_set_HW_write (uint8_t value);
void printallconst (void);
void FC_statistics_print (void);
uint16_t INR_ActT_get_used (void);
uint16_t INR_HashTable_EMA_get_used (void);
uint16_t INR_RuleTable_EMA_get_used (void);
uint16_t INR_CTable_EMH_get_used (void);
uint32_t INR_RuleTable_EMH_get_used (void);
void memdump_en (void);
uint8_t INR_EMH_check (void);
uint8_t INR_EMA_check (void);
uint8_t INR_collT_check (void);
uint8_t INR_FC_check (void);

uint8_t get_verbose (void);
void set_verbose (uint8_t i);

int hwaddr_aton2 (const char *txt, uint8_t * addr);
static int hex2num (char c);
uint32_t parseIPV4string (char *ipAddress);
#define verblog if(get_verbose())	//macro for checking verbose bit
#ifndef C_BASE_ADDR_COMMON_LOWER
#ifdef C_BASE_ADDR_TN_COMMON_L
#define C_BASE_ADDR_COMMON_LOWER C_BASE_ADDR_TN_COMMON_L
#endif
#endif
//base addresses:
//hashtable with hardcoded fields
#define  INR_FC_ActT_valid_mask	0x3ff
#define  INR_FC_ActT_entry_length 4	//length of entry in byte
#define  INR_FC_ActT_entry_length_memcpy  4	//length of entry in byte
#define  INR_FC_ActT_base (C_BASE_ADDR_NOC_ACTION_LOWER<<8)
#define  INR_FC_ActT_length ((((C_BASE_ADDR_NOC_ACTION_UPPER+1)<<8)-(C_BASE_ADDR_NOC_ACTION_LOWER<<8))/INR_FC_ActT_entry_length)

//hashtable with hardcoded fields
#define  INR_FC_EMH_HashTable_entry_length  4	//length of entry in byte
#define  INR_FC_EMH_HashTable_entry_length_memcpy 4	//length of entry in byte
#define  INR_FC_EMH_HashTable_base  (C_BASE_ADDR_FLOW_CACHE_HASH_LOWER<<8)
#define  INR_FC_EMH_HashTable_length  ((((C_BASE_ADDR_FLOW_CACHE_HASH_UPPER+1)<<8)-(C_BASE_ADDR_FLOW_CACHE_HASH_LOWER<<8))/INR_FC_EMH_HashTable_entry_length)

//ruletable with hardcoded fields
#define  INR_FC_EMH_RuleTable_entry_length  64	//length of entry in byte + stuffbits
#define  INR_FC_EMH_RuleTable_entry_length_memcpy 36	//length of entry in byte
#define  INR_FC_EMH_RuleTable_base  (C_BASE_ADDR_FLOW_CACHE_FIELD_LOWER<<8)
#define  INR_FC_EMH_RuleTable_length  ((((C_BASE_ADDR_FLOW_CACHE_FIELD_UPPER+1)<<8)-(C_BASE_ADDR_FLOW_CACHE_FIELD_LOWER<<8))/INR_FC_EMH_RuleTable_entry_length)

//collision table with hardcoded fields
#define  INR_FC_EMH_CTable_entry_length_memcpy  36	//length of entry in byte
#define  INR_FC_EMH_CTable_entry_length 64	//length of entry in byte + stuffbits
#define  INR_FC_EMH_CTable_base (C_BASE_ADDR_FLOW_CACHE_LINEAR_LOWER<<8)
#define  INR_FC_EMH_CTable_length ((((C_BASE_ADDR_FLOW_CACHE_LINEAR_UPPER+1)<<8)-(C_BASE_ADDR_FLOW_CACHE_LINEAR_LOWER<<8))/INR_FC_EMH_CTable_entry_length)

//hashtable with flexible fields
#define  INR_FC_EMA_TCAM_entry_length 16
#ifndef C_BASE_ADDR_FLOW_CACHE_EMA_CAM_LOWER	//backwardcompatibilty
#define  INR_FC_EMA_TCAM_base (C_BASE_ADDR_FLOW_CACHE_EMA_CAM<<8)	//old notation
	////#define  INR_FC_EMA_TCAM_length 9 //0x80   hotfig for bug#52
#define INR_FC_EMA_TCAM_length 0x80
#define INR_HashT_EMA_impl 0	//version of EMA implementation (0:strange addresspace;1:continous memory layout)
#else
#define  INR_FC_EMA_TCAM_base (C_BASE_ADDR_FLOW_CACHE_EMA_CAM_LOWER<<8)
#define  INR_FC_EMA_TCAM_length ((((C_BASE_ADDR_FLOW_CACHE_EMA_CAM_UPPER+1)<<8)-(C_BASE_ADDR_FLOW_CACHE_EMA_CAM_LOWER<<8))/INR_FC_EMA_TCAM_entry_length)
#define  INR_HashT_EMA_impl 1	//version of EMA implementation (0:strange addresspace;1:continous memory layout)
#endif

//ruletable with flexible fields
#define  INR_FC_EMA_RuleTable_entry_length  64	//length of entry in byte + stuffbits
#define  INR_FC_EMA_RuleTable_entry_length_memcpy 36	//length of entry in byte
#define  INR_FC_EMA_RuleTable_base  (C_BASE_ADDR_FLOW_CACHE_EMA_LOWER<<8)
#define  INR_FC_EMA_RuleTable_length  ((((C_BASE_ADDR_FLOW_CACHE_EMA_UPPER+1)<<8)-(C_BASE_ADDR_FLOW_CACHE_EMA_LOWER<<8))/INR_FC_EMA_RuleTable_entry_length)
#ifndef C_FLOW_CACHE_EMH_VERSION
#define C_FLOW_CACHE_EMH_VERSION 1	//backwardcompatibility
#endif
#define EMH_hash_revision C_FLOW_CACHE_EMH_VERSION	//revision of EMH has implementation
#define  THW if(touch_HW)
#define ActT_queue_entrys_queue_mask 0xF80 //0b111110000000  area where flowid are used for queing...this wil not used for automatic rule storage
#define ActT_default_flow 0x0


#if EMH_hash_revision == 1
struct INR_FC_EMH_RULE_TYPE1
{
  uint64_t MAC_SRC:48;	/**<source MAC*/
  uint64_t MAC_DST:48;	/**<destination MAC*/
  uint16_t VLAN_ID:12;	/**<VLAN_ID*/
  uint8_t unused:4;
  uint16_t ETHERTYPE:16;  /**<ETHERTYPE*/
  uint8_t filling[16];
} __attribute__ ((__packed__));

struct INR_FC_EMH_RULE_TYPE2
{
  uint32_t IPv4_SRC:32;	  /**<IPv4 source address*/
  uint32_t IPv4_DST:32;	  /**<IPv4 destination address*/
  uint8_t PROTOCOL:8;	/**<layer 3 protocol*/
  uint16_t PORT_SRC:16;	  /**<TCP/UDP source port*/
  uint16_t PORT_DST:16;	  /**<TCP/UDP destination port*/
  uint8_t filling[19];
  uint8_t unused;
} __attribute__ ((__packed__));

struct INR_FC_EMH_RULE_TYPE3
{
} __attribute__ ((__packed__));

struct INR_FC_EMH_RULE_TYPE4
{
} __attribute__ ((__packed__));
#endif

#if EMH_hash_revision == 3
struct INR_FC_EMH_RULE_TYPE1
{
  uint8_t INPORT:5;	/**<input port*/
  uint8_t unused:3;
  uint8_t filling[31];
} __attribute__ ((__packed__));

struct INR_FC_EMH_RULE_TYPE2
{
  uint16_t VLAN_PRIO;	  /**<VLAN Priority*/
  uint8_t filling[30];
  
} __attribute__ ((__packed__));

struct INR_FC_EMH_RULE_TYPE3
{
	uint64_t MAC_DST:48;	/**<destination MAC*/
	uint16_t VLAN_ID;	  /**<VLAN ID*/
  uint8_t filling[24];
	
} __attribute__ ((__packed__));

struct INR_FC_EMH_RULE_TYPE4
{
		uint64_t MAC_DST:48;	/**<destination MAC*/
		uint16_t VLAN_ID;	  /**<VLAN ID*/
		uint16_t VLAN_PRIO;	  /**<VLAN Priority*/
  	uint8_t filling[22];
} __attribute__ ((__packed__));
#endif

union INR_FC_EMH_RULE_TYPES	//put all types in same meory
{
  struct INR_FC_EMH_RULE_TYPE1 TYPE1;
  struct INR_FC_EMH_RULE_TYPE2 TYPE2;
  struct INR_FC_EMH_RULE_TYPE3 TYPE3;
  struct INR_FC_EMH_RULE_TYPE4 TYPE4;
};


#if C_MMI_ADDR_MAP_REVISION<5
struct INR_FC_EMH_RULE
{
  uint8_t VALID_BIT:1;	    /**<use this rule or not*/
  uint8_t TYPE_ID:5;	/**<type of rule*/
  uint8_t dc1:2;	 /**<dont care bit 1*/
  uint8_t PRIORITY:8;
  uint16_t ACTION_ID:16;     /**<pointer to action table*/
  union INR_FC_EMH_RULE_TYPES COMP_FIELDS;	/**<fields used for comparison, depends on ruletype*/
} __attribute__ ((__packed__));

struct INR_FC_EMA_RULE
{
  uint8_t VALID_BIT:1;	    /**<use this rule or not*/
  uint16_t reserved:9;
  uint16_t TYPE_ID:12;	  /**<type of rule*/
  uint8_t PRIORITY:8;
  uint16_t ACTION_ID:16;     /**<pointer to action table*/
  uint8_t INGRESS_PORT:5;
  uint64_t MAC_SRC:48;	    /**<source MAC*/
  uint64_t MAC_DST:48;	    /**<destination MAC*/
  uint16_t ETHERTYPE:16;      /**<ETHERTYPE*/
  uint16_t VLAN_ID:12;	    /**<VLAN_ID*/
  uint8_t VLAN_PRIO:3;	    /**<VLAN_Priority*/
  uint32_t IPv4_SRC:32;	  /**<IPv4 source address*/
  uint32_t IPv4_DST:32;	  /**<IPv4 destination address*/
  uint8_t PROTOCOL:8;	/**<layer 3 protocol*/
  uint8_t TOS:6;    /**<ToS field*/
  uint16_t PORT_SRC:16;	  /**<TCP/UDP source port*/
  uint16_t PORT_DST:16;	  /**<TCP/UDP destination port*/
} __attribute__ ((__packed__));
#endif
#if C_MMI_ADDR_MAP_REVISION>4
struct INR_FC_EMH_RULE
{
  uint8_t VALID_BIT:1;	    /**<use this rule or not*/
  uint8_t TYPE_ID:5;	/**<type of rule*/
  uint8_t dc1:2;	 /**<dont care bit 1*/
  uint8_t PRIORITY:8;
  uint32_t ACTION_ID:24;     /**<pointer to action table*/
  union INR_FC_EMH_RULE_TYPES COMP_FIELDS;	/**<fields used for comparison, depends on ruletype*/
} __attribute__ ((__packed__));

struct INR_FC_EMA_RULE
{
  uint8_t VALID_BIT:1;	    /**<use this rule or not*/
  uint16_t TYPE_ID:12;	  /**<type of rule*/
  uint8_t PRIORITY:8;
  uint32_t ACTION_ID:24;     /**<pointer to action table*/
  uint8_t INGRESS_PORT:5;
  uint64_t MAC_SRC:48;	    /**<source MAC*/
  uint64_t MAC_DST:48;	    /**<destination MAC*/
  uint16_t ETHERTYPE:16;      /**<ETHERTYPE*/
  uint16_t VLAN_ID:12;	    /**<VLAN_ID*/
  uint8_t VLAN_PRIO:3;	    /**<VLAN_Priority*/
  uint32_t IPv4_SRC:32;	  /**<IPv4 source address*/
  uint32_t IPv4_DST:32;	  /**<IPv4 destination address*/
  uint8_t PROTOCOL:8;	/**<layer 3 protocol*/
  uint8_t TOS:6;    /**<ToS field*/
  uint16_t PORT_SRC:16;	  /**<TCP/UDP source port*/
  uint16_t PORT_DST:16;	  /**<TCP/UDP destination port*/
  uint8_t reserved:1;
} __attribute__ ((__packed__));
#endif

struct INR_FC_EMH_HashTable_entry
{
  uint32_t RULEPOINTER:9;
  uint32_t unused:23;		//dontcare
} __attribute__ ((__packed__));

struct INR_FC_EMA_HashTable_entry_data
{
  uint8_t reserved:1;
  uint8_t INGRESS_PORT:5;
  uint8_t MAC_SRC:8;	  /**<source MAC*/
  uint8_t MAC_DST:8;	  /**<destination MAC*/
  uint8_t ETHERTYPE:4;	    /**<ETHERTYPE*/
  uint8_t VLAN_ID:4;	  /**<VLAN_ID*/
  uint8_t VLAN_PRIO:3;	    /**<VLAN_Priority*/
  uint8_t IPv4_SRC:8;	/**<IPv4 source address*/
  uint8_t IPv4_DST:8;	/**<IPv4 destination address*/
  uint8_t PROTOCOL:4;	/**<layer 3 protocol*/
  uint8_t TOS:3;    /**<ToS field*/
  uint8_t PORT_SRC:4;	/**<TCP/UDP source port*/
  uint8_t PORT_DST:4;	/**<TCP/UDP destination port*/
} __attribute__ ((__packed__));

struct INR_FC_EMA_HashTable_entry_fields
{
  struct INR_FC_EMA_HashTable_entry_data din;
  struct INR_FC_EMA_HashTable_entry_data mask;
} __attribute__ ((__packed__));

struct INR_FC_EMA_HashTable_entry_words
{
  uint32_t dinL;		//tcam acces fileds
  uint32_t dinH;
  uint32_t maskL;
  uint32_t maskH;
} __attribute__ ((__packed__));

union INR_FC_EMA_HashTable_entry
{				//put all types in same memory
  struct INR_FC_EMA_HashTable_entry_fields fields;
  struct INR_FC_EMA_HashTable_entry_words words;
} __attribute__ ((__packed__));

uint8_t INR_HashT_EMA_write (union INR_FC_EMA_HashTable_entry entry, uint16_t ID);	//TCAM
uint8_t INR_HashTable_EMA_clear_entry (uint64_t id);
uint64_t INR_HashTable_EMA_read_entry (uint16_t ID);

struct INR_FC_ActT_RULE
{
  uint8_t OutPort_enable:1;
  /**<Outputport enable  */
  uint8_t OutPort:5;
  /**<outputport assignment  */
  uint8_t Bad_enable:1;
  /**<bad enable  */
  //uint8_t BadValue: 1; //removed 08.12.2017
  /**<bad assignment  */
  //uint8_t BadReason: 5;//removed 08.12.2017
  /**<bad reason  */
  uint8_t Cut_enable:1;		// is diable now, but lets keep the name
  /**<cut through enable  */
  //uint8_t CutValue: 1; //removed 08.12.2017
  /**<cut through assignment  */
  uint32_t unused:24;
  /**<fill bits  */
} __attribute__ ((__packed__));
