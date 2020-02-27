int TN_sysrepo_config_change_cb (sr_session_ctx_t * session, const char *module_name, sr_notif_event_t ev, void *private_ctx);
static void TN_sysrepo_config_handle_change (sr_session_ctx_t * session, sr_change_oper_t op, sr_val_t * old_val, sr_val_t * new_val, int64_t * changeid);
void TN_sysrepo_config_apply (sr_session_ctx_t * session,sr_val_t *value);

