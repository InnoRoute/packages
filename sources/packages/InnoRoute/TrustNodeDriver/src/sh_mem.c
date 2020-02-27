/**
*@file sh_mem.c
*@brief Contains functions to access the Shared-memory
*/

#include <linux/ipc.h>
#include <linux/shm.h>
#include <linux/types.h>
#include <linux/stat.h>
#include <linux/sem.h>
#include "sh_mem.h"

int n;
static struct sembuf semaphore;
uint16_t *shmPtr;

/**
*@brief Copied a Tiefenframe of 320*240*2 bytes in the SharedMemory
*@param *buf Rohdaten
*@param shID ID des Speicherbereiches
*@param semid ID der Semaphore für die Zugriffssteuerung
*@param wait wird "1" übergeben wartet die Funktion solange, bis alte Daten aus dem Speicher gelesen wurden und schreibt erst danach die Übergeben Daten in den Speicher.
*@param flag Kontrollflag des an den Lesenden prozess übermitteln wird
*/
void
cptoshmem (uint16_t * buf, int shID, int semid, uint8_t wait, uint8_t flag)
{
    int p = 0;
    uint8_t wdh = 1;
    while (wdh) {
        semaphore_operation (LOCK, semid);
        shmPtr = (uint16_t *) shmat (shID, 0, 0);
        if (shmPtr == (uint16_t *) - 1) {
            printf ("kein ShMem zugriff\n");
        } else {
            shmPtr[(320 * 240) + 1] = flag;
            if ((wait & shmPtr[0]) | (wait == 0)) {
                for (p = 1; p <= 320 * 240; p++) {
                    shmPtr[p] = buf[p - 1];
                }
                shmPtr[0] = (uint16_t) 0;
                wdh = 0;
            }
            shmdt (shmPtr);
            semaphore_operation (UNLOCK, semid);
        }
        if (wait == 0) {
            wdh = 0;
        }
    }
}

/**
*@brief Kopiert einen Tiefenframe 320*240*2 bytes aus dem SharedMemory, gibt übermitteltes Flag zurück (siehe cptoshmem)
*@param *buf Rohdaten
*@param shID ID des Speicherbereiches
*@param semid ID der Semaphore für die Zugriffssteuerung
*/
uint8_t
cpfromshmem (uint16_t * buf, int shID, int semid)
{
    uint8_t ret = 1, flag = 0;
    while (ret) {
        semaphore_operation (LOCK, semid);
        shmPtr = (uint16_t *) shmat (shID, 0, 0);
        if (shmPtr == (uint16_t *) - 1) {
            printf ("kein sharedMem zugriff\n");
        } else {
            flag = shmPtr[(320 * 240) + 1];
            if (shmPtr[0] == 0) {
                int k;
                for (k = 0; k < 320 * 240; k++) {
                    buf[k] = shmPtr[k + 1];
                }
                shmPtr[0] = 1;
                ret = 0;
            }
            shmdt (shmPtr);
        }
        semaphore_operation (UNLOCK, semid);
        printf ("");
    }
    return flag;
}

/**
*@brief Initialisiert die Semaphore
*@param SEMKEY Schlüssel
*/
int
init_semaphore (int SEMKEY)
{
    int semid;
    semid = semget (SEMKEY, 0, IPC_PRIVATE);
    if (semid < 0) {
        umask (0);
        semid = semget (SEMKEY, 1, IPC_CREAT | IPC_EXCL | PERM);
        if (semid < 0) {
            printf ("Fehler beim Anlegen der Semaphore...\n");
            return -1;
        } else {
            printf ("Semaphore angelegt...\n");
            if (semctl (semid, 0, SETVAL, (int) 1) == -1) {
                return -1;
            }
        }
        return semid;
    }
}

static int
semaphore_operation (int op, int semid)
{
    semaphore.sem_op = op;
    semaphore.sem_flg = SEM_UNDO;
    if (semop (semid, &semaphore, 1) == -1) {
        perror (" semop ");
    }
    return 1;
}

/**
*@brief gibt den Shared Memory wieder frei
*@param shid ID des Shared memory
*@param Semid ID der Semaphore
*/
void
destroy_shmem (int shid, int Semid)
{
    shmctl (shid, IPC_RMID, 0); //destroy Shared Memory
    semctl (Semid, 0, IPC_RMID, 0); // destroy semaphore
}

/**
*@brief initialisiert den SharedMemory
*@param SHMKEY Schlüssel
*@param SHMSIZE größe des Speicherbereiches
*/
int
init_shmem (int SHMKEY, int SHMSIZE)
{
    int shid = 0;
    shid = shmget (SHMKEY, SHMSIZE, IPC_CREAT | 0666);
    return shid;
}
