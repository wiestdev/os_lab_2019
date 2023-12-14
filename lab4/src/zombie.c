#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>

int main() {
    pid_t pid = fork();
    if (pid > 0) {
        sleep(100);
    }
    else if (pid == 0) {
        exit(0);
    }
    else {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    return 0;
}