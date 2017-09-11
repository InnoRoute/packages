/**
*@file sh_mem.h
*@brief shared mem definitions
*/
#define LOCK -1
#define UNLOCK 1
#define PERM 0666
static int semaphore_operation (int op, int semid);
int init_semaphore (int SEMKEY);
uint8_t cpfromshmem (uint16_t * buf, int shID, int semid);
void cptoshmem (uint16_t * buf, int shID, int semid, uint8_t wait, uint8_t flag);
void destroy_shmem (int shID, int semid);
int init_shmem (int SHMKEY, int SHMSIZE);
