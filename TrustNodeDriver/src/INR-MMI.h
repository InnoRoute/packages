void INR_MMI_interrupt (void);
void INR_MMI_interrupt_start(uint64_t bar1);
void INR_MMI_interrupt_stop(void);
extern void INR_MMI_interrupt_handler(void);
int INR_MMI_interrupt_watchd (void *nix);
void INR_MMI_interrupt_watchd_wakeup(void);
void INR_MMI_set_interrupt_allowed(uint8_t val);
