#define MASTERTABLE_length 100000
#define entry_is_EMH1(arg) ((arg)->dohave_MAC_DST&&(arg)->dohave_MAC_SRC&&(arg)->dohave_ETHERTYPE&&(arg)->dohave_VLAN_ID)
#define entry_is_EMH2(arg) ((arg)->dohave_IPv4_DST&&(arg)->dohave_IPv4_SRC&&(arg)->dohave_PROTOCOL&&(arg)->dohave_PORT_DST&&(arg)->dohave_PORT_SRC)
#define MT_matchfiled(entry, arguments, match, type) {if( (entry)-> type != (arguments)->type ) (match) =0;}
#define MT_dohaveprint(entry, type) {if( entry ->dohave_ ## type )printf(#type":0x%lx ", entry -> type);}

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
uint8_t FC_MasterT_del_entry(struct arguments *arguments);
uint64_t FC_MT_count_action (uint64_t actid);
