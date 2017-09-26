//IDs used for finding the EMA rule type
#define ID_MAC_SRC 1
#define ID_MAC_DST 2
#define ID_VLAN_ID 4
#define ID_VLAN_PRIO 5
#define ID_IPv4_SRC 6
#define ID_IPv4_DST 7
#define ID_PROTOCOL 8
#define ID_PORT_SRC 10
#define ID_PORT_DST 11
#define ID_ETHERTYPE 3
#define ID_INPORT 0
#define ID_TOS 9

/* Structure to store hashes */
struct HASH
{
	uint16_t EMH;	/**<hardcoded fields hash storage*/
	uint8_t gauto; /**<automatically generate hash values for rule entry*/
	struct INR_FC_EMA_HashTable_entry_data EMA; /**<masked fields hash storage*/
} __attribute__ ((__packed__));

/* Structure to store table possition where entry is stored */
struct tabid
{
	uint64_t EMH_RT;	/**ruletable*/
	uint64_t EMH_HT;	/**hashtable*/
	uint64_t EMH_CT;	/**collision table*/
	uint64_t EMA_RT;	/**ruletable*/
	uint64_t EMA_HT;	/**hashtable*/
	uint64_t ActT;	/**Action table*/
} __attribute__ ((__packed__));

struct arguments
{
	char *args[1];	/* filename */
	uint8_t used;	/**<entry used */
	int verbose;	/* The -v flag */
	uint64_t MAC_SRC;	/**<source MAC*/
	uint8_t dohave_MAC_SRC;/**<source MAC was set by user*/
	uint64_t MAC_DST;	/**<destination MAC*/
	uint8_t dohave_MAC_DST;/**<destination MAC was set by user*/
	uint32_t VLAN_ID;	/**<VLAN_ID*/
	uint8_t dohave_VLAN_ID;/**<VLAN_ID was set by user*/
	uint32_t VLAN_PRIO;	/**<VLAN_PRIORITY*/
	uint8_t dohave_VLAN_PRIO;/**<VLAN_PRIORITY was set by user*/
	uint32_t IPv4_SRC;	/**<IPv4 source address*/
	uint8_t dohave_IPv4_SRC;/**<IPv4 source address was set by user*/
	uint32_t IPv4_DST;	/**<IPv4 destination address*/
	uint8_t dohave_IPv4_DST;/**<IPv4 destination address was set by user*/
	uint32_t PROTOCOL;/**<IP protocol*/
	uint8_t dohave_PROTOCOL;/**<IP protocol was set by user*/
	uint32_t PORT_SRC;	/**<TCP/UDP source port*/
	uint8_t dohave_PORT_SRC;/**<TCP/UDP source port was set by user*/
	uint32_t PORT_DST;	/**<TCP/UDP destination port*/
	uint8_t dohave_PORT_DST;/**<TCP/UDP destination port was set by user*/
	uint16_t ETHERTYPE;/**<Ethertype*/
	uint8_t dohave_ETHERTYPE;/**<Ethertype was set by user*/
	uint16_t TOS;/**<Type of service*/
	uint8_t dohave_TOS;/**<Type of service was set by user*/
	uint8_t INPORT;/**<physical input port*/
	uint8_t dohave_INPORT;/**<physical input port was set by user*/
	uint64_t MASK_MAC_SRC;	/**<Mask for source MAC*/
	uint8_t dohave_MASK_MAC_SRC;/**<Mask for source MAC was set by user*/
	uint64_t MASK_MAC_DST;	/**<Mask for destination MAC*/
	uint8_t dohave_MASK_MAC_DST;/**<Mask for destination MAC was set by user*/
	uint32_t MASK_VLAN_ID;	/**<Mask for VLAN_ID*/
	uint8_t dohave_MASK_VLAN_ID;/**<Mask for VLAN_ID was set by user*/
	uint32_t MASK_VLAN_PRIO;	/**<Mask for VLAN_Priority*/
	uint8_t dohave_MASK_VLAN_PRIO;/**<Mask for VLAN_Priority was set by user*/
	uint32_t MASK_IPv4_SRC;	/**<Mask for IPv4 source address*/
	uint8_t dohave_MASK_IPv4_SRC;/**<Mask for IPv4 source address was set by user*/
	uint32_t MASK_IPv4_DST;	/**<Mask for IPv4 destination address*/
	uint8_t dohave_MASK_IPv4_DST;/**<Mask for IPv4 destination address was set by user*/
	uint32_t MASK_PROTOCOL;/**<Mask for IP protocol*/
	uint8_t dohave_MASK_PROTOCOL;/**<Mask for IP protocol was set by user*/
	uint32_t MASK_PORT_SRC;	/**<Mask for TCP/UDP source port*/
	uint8_t dohave_MASK_PORT_SRC;/**<Mask for TCP/UDP source port was set by user*/
	uint32_t MASK_PORT_DST;	/**<Mask for TCP/UDP destination port*/
	uint8_t dohave_MASK_PORT_DST;/**<Mask for TCP/UDP destination port was set by user*/
	uint16_t MASK_ETHERTYPE;/**<Mask for Ethertype*/
	uint8_t dohave_MASK_ETHERTYPE;/**<Mask for Ethertype was set by user*/
	uint16_t MASK_TOS;/**<Mask for type of service*/
	uint8_t dohave_MASK_TOS;/**<Mask for type of service was set by user*/
	uint8_t MASK_INPORT;/**<Mask for physical inputport*/
	uint8_t dohave_MASK_INPORT;/**<Mask for physical inputport was set by user*/
	uint32_t TYPE_ID;	/**<type of rule*/
	uint32_t PRIORITY;/**<rule priority*/
	uint32_t ACTION_ID;	/**<pointer to action table*/
	uint32_t RULEPOINTER;/**<ID of rule, will be used for search actions*/
	uint64_t ID; /**<userspecific table ID*/
	struct HASH HASH;	/**<structure for hash storage*/
	uint64_t COUNT;/**<count value for print actions*/
	uint8_t OutPort_enable;/**<Action enable output port*/
	uint8_t dohave_OutPort_enable;	/**<Action enable output port was set by user*/
	uint8_t OutPort;/**<Action select output port*/
	uint8_t dohave_OutPort;	/**<Action select output port was set by user*/
	uint8_t Bad_enable;/**<bad enable  */
	uint8_t dohave_Bad_enable;	/**<bad enable was set by user*/
	uint8_t BadValue;/**<bad assignment  */
	uint8_t dohave_BadValue;	/**<bad assignment was set by user*/
	uint8_t BadReason;/**<bad reason  */
	uint8_t dohave_BadReason;	/**<bad reason was set by user*/
	uint8_t Cut_enable;/**<cut through enable  */
	uint8_t dohave_Cut_enable;	/**<cut through enable was set by user*/
	uint8_t CutValue;/**<cut through assignment  */
	uint8_t dohave_CutValue;	/**<cut through assignment was set by user*/
	uint8_t autoaction;	/**<automatic write to actiontable*/
	struct tabid TableID; /**<storage for positions if rule was stored */

} __attribute__ ((__packed__));
void clear_arguments (struct arguments *arguments);
void HT_EMH_add (struct arguments *arguments);
void HT_EMH_del (struct arguments arguments);
void HT_EMH_clear (struct arguments arguments);
void HT_EMH_print (struct arguments arguments);
void RT_EMH_add (struct arguments *arguments);
void RT_EMH_del (struct arguments arguments);
void RT_EMH_clear (struct arguments arguments);
void RT_EMH_print (struct arguments arguments);
void get_HASH (struct arguments *arguments);
void HT_EMA_add (struct arguments *arguments);
void HT_EMA_del (struct arguments arguments);
void HT_EMA_clear (struct arguments arguments);
void HT_EMA_print (struct arguments arguments);
void RT_EMA_add (struct arguments *arguments);
void RT_EMA_del (struct arguments arguments);
void RT_EMA_clear (struct arguments arguments);
void RT_EMA_print (struct arguments arguments);
void CT_EMH_add (struct arguments *arguments);
void CT_EMH_del (struct arguments arguments);
void CT_EMH_clear (struct arguments arguments);
void CT_EMH_print (struct arguments arguments);
void AT_add (struct arguments *arguments);
void AT_del (struct arguments arguments);
void AT_clear (struct arguments arguments);
void AT_print (struct arguments arguments);
void RT_EMA_TYPE_override (struct arguments *arguments);
