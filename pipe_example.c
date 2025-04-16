#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define MSG_SIZE 100

int main() {
    int pipefd[2];
    pid_t pid1, pid2;
    char buffer[MSG_SIZE];

    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid1 = fork();
    if (pid1 < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid1 == 0) {
        close(pipefd[0]);
        const char *msg1 = "Child 1 is sending a message!\n";
        write(pipefd[1], msg1, strlen(msg1));
        close(pipefd[1]);
        exit(0);
    }

    pid2 = fork();
    if (pid2 < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid2 == 0) {
        close(pipefd[0]);
        const char *msg2 = "Child 2 is sending a message!\n";
        write(pipefd[1], msg2, strlen(msg2));
        close(pipefd[1]);
        exit(0);
    }

    close(pipefd[1]);

    wait(NULL);
    wait(NULL);

    printf("Parent is reading messages from the pipe:\n");
    int bytesRead;
    while ((bytesRead = read(pipefd[0], buffer, sizeof(buffer) - 1)) > 0) {
        buffer[bytesRead] = '\0';
        printf("%s", buffer);
    }

    close(pipefd[0]);

    return 0;
}