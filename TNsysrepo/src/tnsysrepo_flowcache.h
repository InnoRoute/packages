int TN_sysrepo_fc_change_cb (sr_session_ctx_t * session, const char *module_name, sr_notif_event_t ev, void *private_ctx);
void TN_sysrepo_flowcache_init (void);
void TN_sysrepo_flowcache_exit (void);

#define TN_sysrepo_v2a_string(entry_arg, value_val, match_arg, type_arg,  sr_type_arg){if(sr_str_ends_with( value_val .xpath , match_arg )){verblog printf(" #type_arg changed\n");strcpy(&tmp_strg[0], value_val .data. sr_type_arg ); (entry_arg). type_arg =atoi( value_val .data. sr_type_arg ); entry_arg .dohave_ ##type_arg =1;}}
#define TN_sysrepo_v2a_number(entry_arg, value_val, match_arg, type_arg,  sr_type_arg){if(sr_str_ends_with( value_val .xpath , match_arg )){verblog printf(" #type_arg changed\n"); (entry_arg). type_arg = value_val .data. sr_type_arg ; entry_arg .dohave_ ##type_arg =1;}}
#define TN_sysrepo_v2a_MAC(entry_arg, value_val, match_arg, type_arg,  sr_type_arg){if(sr_str_ends_with( value_val .xpath , match_arg )){verblog printf(" #type_arg changed\n");hwaddr_aton2 (value_val .data. sr_type_arg, &(entry_arg). type_arg);(entry_arg). type_arg = htobe64 ( (entry_arg). type_arg ) >> 16; entry_arg .dohave_ ##type_arg =1;}}
#define TN_sysrepo_v2a_IP(entry_arg, value_val, match_arg, type_arg,  sr_type_arg){if(sr_str_ends_with( value_val .xpath , match_arg )){verblog printf(" #type_arg changed\n"); (entry_arg). type_arg = htobe32 (inet_addr ( value_val .data. sr_type_arg )) ; entry_arg .dohave_ ##type_arg =1; }}
#define TN_sysrepo_v2a_outport(entry_arg, value_val, match_arg, type_arg,  sr_type_arg){if(sr_str_ends_with( value_val .xpath , match_arg )){verblog printf(" #type_arg changed\n"); (entry_arg). type_arg = value_val .data. sr_type_arg ; entry_arg .dohave_ ##type_arg =1; entry_arg .OutPort_enable = 1; entry_arg .dohave_OutPort_enable = 1;}}
