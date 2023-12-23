#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;

int k, pnum, mod;
long long result = 1;

void *factorial(void *arg) {
    int start = *((int*)arg);
    long long part_result = 1;
    for (int i = start; i <= k; i += pnum)
        part_result = (part_result * i) % mod;
    pthread_mutex_lock(&mut);
    result = (result * part_result) % mod;
    pthread_mutex_unlock(&mut);
}

int main(int argc, char* argv[]) {
    if (argc != 7) {
        fprintf(stderr, "Usage: %s -k <num> --pnum <num> --mod <num>\n", argv[0]);
        return 1;
    }
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-k") == 0) {
            k = atoi(argv[++i]);
            if (k < 0) {
                printf("k must be >= 0\n");
                return 1;
            }
        }
        else if (strcmp(argv[i], "--pnum") == 0) {
            pnum = atoi(argv[++i]);
            if (pnum <= 0) {
                printf("pnum must be > 0\n");
                return 1;
            }
        }
        else if (strcmp(argv[i], "--mod") == 0) {
            mod = atoi(argv[++i]);
            if (mod <= 0) {
                printf("mod must be > 0\n");
                return 1;
            }
        }
    }
    pthread_t threads[pnum];
    int thread_id[pnum];
    pthread_mutex_init(&mut, NULL);

    for (int i = 0; i < pnum; i++) {
        thread_id[i] = i + 1;
        pthread_create(&threads[i], NULL, factorial, &thread_id[i]);
    }
    for (int i = 0; i < pnum; i++) {
        pthread_join(threads[i], NULL);
    }
    printf("Result: %lld\n", result);

    pthread_mutex_destroy(&mut);

    return 0;
}