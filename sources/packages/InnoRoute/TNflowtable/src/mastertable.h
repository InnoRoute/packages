/**
*@file mastertable.h
*@brief functions to handle mastertable
*M.Ulbricht 2018
**/
//#include "flowtableactions.h"
#define MASTERTABLE_length 65536
#if EMH_hash_revision == 1
	#define entry_is_EMH1(arg) (((arg)->dohave_MAC_DST&&(arg)->dohave_MAC_SRC&&(arg)->dohave_ETHERTYPE&&(arg)->dohave_VLAN_ID)&&((arg)->matchcount==4))
	#define entry_is_EMH2(arg) (((arg)->dohave_IPv4_DST&&(arg)->dohave_IPv4_SRC&&(arg)->dohave_PROTOCOL&&(arg)->dohave_PORT_DST&&(arg)->dohave_PORT_SRC)&&((arg)->matchcount==5))
	#define entry_is_EMH3(arg) 0
	#define entry_is_EMH4(arg) 0
#endif
#if EMH_hash_revision == 2
	#define entry_is_EMH1(arg) (((arg)->dohave_PROTOCOL&&(arg)->dohave_IPv4_SRC&&(arg)->dohave_IPv4_DST&&(arg)->dohave_MAC_DST&&(arg)->dohave_ETHERTYPE)&&((arg)->matchcount==5))
	#define entry_is_EMH2(arg) (((arg)->dohave_PROTOCOL&&(arg)->dohave_IPv4_DST&&(arg)->dohave_PORT_DST&&(arg)->dohave_MAC_DST&&(arg)->dohave_ETHERTYPE)&&((arg)->matchcount==5))
	#define entry_is_EMH3(arg) (((arg)->dohave_IPv4_DST&&(arg)->dohave_IPv4_SRC&&(arg)->dohave_MAC_DST)&&((arg)->matchcount==3))
	#define entry_is_EMH4(arg) (((arg)->dohave_MAC_DST)&&((arg)->matchcount==1))
#endif
#if EMH_hash_revision == 3
	#define entry_is_EMH1(arg) (((arg)->dohave_INPORT)&&((arg)->matchcount=1))
	#define entry_is_EMH2(arg) (((arg)->dohave_VLAN_PRIO)&&((arg)->matchcount==1))
	#define entry_is_EMH3(arg) (((arg)->dohave_VLAN_ID&&(arg)->dohave_MAC_DST)&&((arg)->matchcount==2))
	#define entry_is_EMH4(arg) (((arg)->dohave_VLAN_ID&&(arg)->dohave_MAC_DST&&(arg)->dohave_VLAN_PRIO)&&((arg)->matchcount==3))
#endif
#if EMH_hash_revision == 4
	#define entry_is_EMH1(arg) (((arg)->dohave_INPORT)&&(!(arg)->dohave_ETHERTYPE)&&((arg)->matchcount==2))
	#define entry_is_EMH2(arg) (((arg)->dohave_VLAN_PRIO)&&((arg)->matchcount==1))
	#define entry_is_EMH3(arg) (((arg)->dohave_VLAN_ID&&(arg)->dohave_MAC_DST)&&((arg)->matchcount==2))
	#define entry_is_EMH4(arg) (((arg)->dohave_VLAN_ID&&(arg)->dohave_MAC_DST&&(arg)->dohave_VLAN_PRIO)&&((arg)->matchcount==3))
#endif
#if EMH_hash_revision == 5
	#define entry_is_EMH1(arg) (((arg)->dohave_INPORT)&&(!(arg)->dohave_ETHERTYPE)&&((arg)->matchcount==2))
	#define entry_is_EMH2(arg) (((arg)->dohave_VLAN_PRIO)&&(!(arg)->dohave_MAC_DST))&&((arg)->matchcount==2))
	#define entry_is_EMH3(arg) (((arg)->dohave_VLAN_ID&&(arg)->dohave_MAC_DST)&&((arg)->matchcount==2))
	#define entry_is_EMH4(arg) (((arg)->dohave_INPORT)&&((arg)->dohave_ETHERTYPE)&&((arg)->matchcount==2))
#endif
#if EMH_hash_revision == 4
#define entry_is_EMH1(arg) (((arg)->dohave_INPORT)&&(!(arg)->dohave_ETHERTYPE)&&((arg)->matchcount==2))
#define entry_is_EMH2(arg) (((arg)->dohave_VLAN_PRIO)&&((arg)->matchcount==1))
#define entry_is_EMH3(arg) (((arg)->dohave_VLAN_ID&&(arg)->dohave_MAC_DST)&&((arg)->matchcount==2))
#define entry_is_EMH4(arg) (((arg)->dohave_VLAN_ID&&(arg)->dohave_MAC_DST&&(arg)->dohave_VLAN_PRIO)&&((arg)->matchcount==3))
#endif
#if EMH_hash_revision == 5
#define entry_is_EMH1(arg) (((arg)->dohave_INPORT)&&(!(arg)->dohave_ETHERTYPE)&&((arg)->matchcount==2))
#define entry_is_EMH2(arg) (((arg)->dohave_VLAN_PRIO)&&((arg)->matchcount==1))
#define entry_is_EMH3(arg) (((arg)->dohave_VLAN_ID&&(arg)->dohave_MAC_DST)&&((arg)->matchcount==2))
#define entry_is_EMH4(arg) ((((arg)->dohave_INPORT)&&((arg)->dohave_ETHERTYPE))&&((arg)->matchcount==2))
#endif
#if EMH_hash_revision == 6
#define entry_is_EMH1(arg) (((arg)->dohave_PORT_DST)&&((arg)->matchcount==1))
#define entry_is_EMH2(arg) (((arg)->dohave_VLAN_PRIO)&&((arg)->matchcount==1))
#define entry_is_EMH3(arg) (((arg)->dohave_VLAN_ID&&(arg)->dohave_MAC_DST)&&((arg)->matchcount==2))
#define entry_is_EMH4(arg) ((((arg)->dohave_INPORT)&&((arg)->dohave_ETHERTYPE))&&((arg)->matchcount==2))
#endif
#if EMH_hash_revision == 7
	#define entry_is_EMH4(arg) (((arg)->dohave_PORT_DST)&&((arg)->matchcount==1))
	#define entry_is_EMH2(arg) (((arg)->dohave_VLAN_PRIO)&&((arg)->matchcount==1))
	#define entry_is_EMH3(arg) (((arg)->dohave_VLAN_ID&&(arg)->dohave_MAC_DST)&&((arg)->matchcount==2))
	#define entry_is_EMH1(arg) ((((arg)->dohave_INPORT)&&((arg)->dohave_ETHERTYPE))&&((arg)->matchcount==2))
#endif
#if EMH_hash_revision == 8
	#define entry_is_EMH1(arg) (((arg)->dohave_INPORT)&&((arg)->matchcount==1))
	#define entry_is_EMH2(arg) (((arg)->dohave_ETHERTYPE)&&((arg)->matchcount==1))
	#define entry_is_EMH3(arg) (((arg)->dohave_VLAN_ID&&(arg)->dohave_MAC_DST)&&((arg)->matchcount==2))
	#define entry_is_EMH4(arg) (((arg)->dohave_PORT_DST)&&((arg)->matchcount==1))
#endif
#if EMH_hash_revision == 9
	#define entry_is_EMH1(arg) (((arg)->dohave_INPORT)&&((arg)->matchcount==1))
	#define entry_is_EMH2(arg) (((arg)->dohave_VLAN_PRIO)&&((arg)->matchcount==1))
	#define entry_is_EMH3(arg) (((arg)->dohave_VLAN_ID&&(arg)->dohave_MAC_DST)&&((arg)->matchcount==2))
	#define entry_is_EMH4(arg) (((arg)->dohave_PORT_DST&&(arg)->dohave_IPv4_DST&&(arg)->dohave_PROTOCOL&&(arg)->dohave_ETHERTYPE)&&((arg)->matchcount==4))
#endif
#if EMH_hash_revision == 10
	#define entry_is_EMH1(arg) (((arg)->dohave_INPORT)&&((arg)->matchcount==1))
	#define entry_is_EMH2(arg) (((arg)->dohave_VLAN_PRIO)&&((arg)->matchcount==1))
	#define entry_is_EMH3(arg) (((arg)->dohave_MAC_DST)&&((arg)->matchcount==1))
	#define entry_is_EMH4(arg) (((arg)->dohave_PORT_DST&&(arg)->dohave_IPv4_DST&&(arg)->dohave_PROTOCOL&&(arg)->dohave_ETHERTYPE)&&((arg)->matchcount==4))
#endif
#if EMH_hash_revision == 11
	#define entry_is_EMH1(arg) (((arg)->dohave_INPORT)&&((arg)->dohave_VLAN_ID)&&((arg)->matchcount==2))
	#define entry_is_EMH2(arg) (((arg)->dohave_INPORT)&&((arg)->dohave_MAC_DST)&&((arg)->matchcount==2))
	#define entry_is_EMH3(arg) (((arg)->dohave_INPORT)&&((arg)->dohave_ETHERTYPE)&&(arg)->dohave_IPv4_DST&&((arg)->matchcount==3))
	#define entry_is_EMH4(arg) (((arg)->dohave_INPORT)&&((arg)->dohave_ETHERTYPE)&&(arg)->dohave_PORT_DST&&(arg)->dohave_IPv4_DST&&((arg)->matchcount==4))
#endif
#if EMH_hash_revision == 12
	#define entry_is_EMH1(arg) (((arg)->dohave_VLAN_ID)&&((arg)->matchcount==1))
	#define entry_is_EMH2(arg) (((arg)->dohave_MAC_DST)&&((arg)->matchcount==1))
	#define entry_is_EMH3(arg) (((arg)->dohave_MAC_DST)&&((arg)->dohave_VLAN_ID)&&((arg)->matchcount==2))
	#define entry_is_EMH4(arg) (((arg)->dohave_VLAN_PRIO)&&((arg)->dohave_MAC_DST)&&((arg)->dohave_VLAN_ID)&&((arg)->matchcount==3))
#endif
#define MT_matchfiled(entry, arguments, match, type) {if( (entry)-> type != (arguments)->type ) (match) =0;}
#define MT_dohaveprint(entry, type) {if(! arguments ->machinereadable )if( entry ->dohave_ ## type )printf(#type":0x%lx ", entry -> type);if( arguments ->machinereadable ){printf("\""#type"\":%lli,", entry -> type);printf("\"dohave_"#type"\":%i", entry ->dohave_ ## type );}}
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
uint32_t INR_MasterT_get_used (uint64_t * optioncount);
uint32_t FC_MasterT_get_MAX (void);
uint8_t FC_MasterT_get_entry (struct arguments *entry, uint32_t id);
