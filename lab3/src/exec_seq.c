#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char **argv) {
    if (argc != 3) {
        printf("Usage: %s seed arraysize\n", argv[0]);
        return 1;
    }
    int seed = atoi(argv[1]);
    if (seed <= 0) {
        printf("seed is a positive number\n");
        return 1;
    }
    int array_size = atoi(argv[2]);
    if (array_size <= 0) {
        printf("array_size is a positive number\n");
        return 1;
    }
    pid_t pid = fork();
    if (pid == -1) {
        perror("fork error\n");
    }
    else if (pid == 0) {
        execl(
            "sequential_min_max",  "sequential_min_max", argv[1], argv[2], NULL
        );
        perror("execl error\n");
    }
    else {
        int status;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status))
            printf("Child proccess exited with code %d\n", WEXITSTATUS(status));
        else
            printf("Child proccess exited with error");
    }

    return 0;
}