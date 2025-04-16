#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <sys/wait.h>

#define MQ_NAME "/my_mq"
#define MAX_SIZE 1024
#define MSG "Hello from sender process!"

int main() {
    mqd_t mq;
    struct mq_attr attr;

    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = MAX_SIZE;
    attr.mq_curmsgs = 0;

    mq = mq_open(MQ_NAME, O_CREAT | O_RDWR, 0644, &attr);
    if (mq == -1) {
        perror("mq_open");
        exit(1);
    }

    pid_t sender = fork();
    if (sender == 0) {
        if (mq_send(mq, MSG, strlen(MSG) + 1, 0) == -1) {
            perror("mq_send");
            exit(1);
        }
        exit(0);
    }

    pid_t receiver = fork();
    if (receiver == 0) {
        char buffer[MAX_SIZE];

        sleep(1);

        if (mq_receive(mq, buffer, MAX_SIZE, NULL) == -1) {
            perror("mq_receive");
            exit(1);
        }

        printf("Receiver got message: %s\n", buffer);

        mq_close(mq);
        mq_unlink(MQ_NAME);

        exit(0);
    }

    wait(NULL);
    wait(NULL);

    return 0;
}
