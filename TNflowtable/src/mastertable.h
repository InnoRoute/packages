//#include "flowtableactions.h"
#define MASTERTABLE_length 65536
#if EMH_hash_revision == 1
	#define entry_is_EMH1(arg) ((arg)->dohave_MAC_DST&&(arg)->dohave_MAC_SRC&&(arg)->dohave_ETHERTYPE&&(arg)->dohave_VLAN_ID)
	#define entry_is_EMH2(arg) ((arg)->dohave_IPv4_DST&&(arg)->dohave_IPv4_SRC&&(arg)->dohave_PROTOCOL&&(arg)->dohave_PORT_DST&&(arg)->dohave_PORT_SRC)
	#define entry_is_EMH3(arg) 0
	#define entry_is_EMH4(arg) 0
#endif
#if EMH_hash_revision == 2
	#define entry_is_EMH1(arg) ((arg)->dohave_PROTOCOL&&(arg)->dohave_IPv4_SRC&&(arg)->dohave_IPv4_DST&&(arg)->dohave_MAC_DST&&(arg)->dohave_ETHERTYPE)
	#define entry_is_EMH2(arg) ((arg)->dohave_PROTOCOL&&(arg)->dohave_IPv4_DST&&(arg)->dohave_PORT_DST&&(arg)->dohave_MAC_DST&&(arg)->dohave_ETHERTYPE)
	#define entry_is_EMH3(arg) ((arg)->dohave_IPv4_DST&&(arg)->dohave_IPv4_SRC&&(arg)->dohave_MAC_DST)
	#define entry_is_EMH4(arg) ((arg)->dohave_MAC_DST)
#endif
#if EMH_hash_revision == 3
	#define entry_is_EMH1(arg) ((arg)->dohave_INPORT)
	#define entry_is_EMH2(arg) ((arg)->dohave_VLAN_PRIO)
	#define entry_is_EMH3(arg) ((arg)->dohave_VLAN_ID&&(arg)->dohave_MAC_DST)
	#define entry_is_EMH4(arg) ((arg)->dohave_VLAN_ID&&(arg)->dohave_MAC_DST&&(arg)->dohave_VLAN_PRIO)
#endif
#define MT_matchfiled(entry, arguments, match, type) {if( (entry)-> type != (arguments)->type ) (match) =0;}
#define MT_dohaveprint(entry, type) {if( entry ->dohave_ ## type )printf(#type":0x%lx ", entry -> type);}
#define MAXprio 255

void FCinit_MasterTable (uint64_t * baseaddr_master);
uint64_t INR_MasterT_get_addr (uint64_t id);
uint64_t INR_MasterT_get_next_free_entry (uint64_t id);
void FC_MasterT_clear (void);
uint8_t FC_MasterT_del (uint64_t ID);
uint8_t FC_MT_autotable (struct arguments *arguments);
uint8_t FC_MT_have_EMH_hash (uint16_t hash);
uint64_t FC_MT_have_action (struct arguments *arguments);
void FC_MasterT_print (struct arguments *arguments);
void FC_MasterT_update (struct arguments *arguments);
uint64_t FC_MT_FIND_entry (struct arguments *arguments);
uint8_t FC_MasterT_del_entry (struct arguments *arguments);
uint64_t FC_MT_count_action (uint64_t actid);
void FC_MT_apply_priority (void);
uint32_t INR_MasterT_get_used (void);
