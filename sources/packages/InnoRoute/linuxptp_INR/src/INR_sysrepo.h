void INR_new_pathdelay(int port, int64_t pathdelay);
void INR_sysrepo_init(void);
void INR_sysrepo_exit(void);

#define verblog if(1)	//macro for checking verbose bit

#define FD_TO_CLOCKID(fd) ((~(clockid_t) (fd) <<3) | CLOCKFD)
#define CLOCKFD 3
