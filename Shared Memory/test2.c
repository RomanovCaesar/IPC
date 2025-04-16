#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SHM_KEY 1234
#define SHM_SIZE 1024

int main() {
    int shmid = shmget(SHM_KEY, SHM_SIZE, 0666);
    if (shmid == -1) {
        perror("shmget failed");
        exit(1);
    }

    char *shm = (char *) shmat(shmid, NULL, 0);
    if (shm == (char *) -1) {
        perror("shmat failed");
        exit(1);
    }

    printf("test2 read: %s\n", shm);

    shmdt(shm);

    shmctl(shmid, IPC_RMID, NULL);

    return 0;
}