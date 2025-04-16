#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>

#define SHM_KEY 1234
#define SHM_SIZE 1024

int main() {
    int shmid = shmget(SHM_KEY, SHM_SIZE, IPC_CREAT | 0666);
    if (shmid == -1) {
        perror("shmget failed");
        exit(1);
    }

    char *shm = (char *) shmat(shmid, NULL, 0);
    if (shm == (char *) -1) {
        perror("shmat failed");
        exit(1);
    }

    const char *message = "Hello from test1!";
    strncpy(shm, message, SHM_SIZE);
    printf("test1 wrote: %s\n", message);

    shmdt(shm);

    return 0;
}