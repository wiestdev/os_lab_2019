#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <pthread.h>

#include <sys/time.h>
#include "sum.h"
#include "utils.h"



void *ThreadSum(void *args) {
  struct SumArgs *sum_args = (struct SumArgs *)args;
  return (void *)(size_t)Sum(sum_args);
}

int main(int argc, char **argv) {
  if (argc != 7) {
    fprintf(stderr, "Usage: %s --threads_num <num> --seed <num> --array_size <num>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  uint32_t threads_num = 0;
  uint32_t array_size = 0;
  uint32_t seed = 0;

  for (int i = 1; i < argc; i += 2) {
    if (strcmp(argv[i], "--threads_num") == 0) {
      threads_num = atoi(argv[i + 1]);
      if (threads_num <= 0) {
        printf("threads_num must be > 0");
        return 1;
      }
    }
    if (strcmp(argv[i], "--seed") == 0) {
      seed = atoi(argv[i + 1]);
      if (seed <= 0) {
        printf("seed must be > 0");
        return 1;
      }
    }
    if (strcmp(argv[i], "--array_size") == 0) {
      array_size = atoi(argv[i + 1]);
      if (array_size <= 0) {
        printf("array_size must be > 0");
        return 1;
      }
    }
  }
  pthread_t threads[threads_num];
  int *array = malloc(sizeof(int) * array_size);
  GenerateArray(array, array_size, seed);

  int m = array_size / threads_num;

  struct SumArgs args[threads_num];

  struct timeval start_time;
  gettimeofday(&start_time, NULL);

  for (uint32_t i = 0; i < threads_num; i++) {
    args[i].array = array;
    args[i].begin = i * m;
    if (i == threads_num - 1)
      args[i].end = array_size;
    else
      args[i].end = (i + 1) * m;
    if (pthread_create(&threads[i], NULL, ThreadSum, (void *)&args[i])) {
      printf("Error: pthread_create failed!\n");
      return 1;
    }
  }
  int total_sum = 0;
  void *sum;
  for (uint32_t i = 0; i < threads_num; i++) {
    pthread_join(threads[i], &sum);
    total_sum += (uint32_t)(size_t)sum;
  }

  struct timeval finish_time;
  gettimeofday(&finish_time, NULL);

  double elapsed_time = (finish_time.tv_sec - start_time.tv_sec) * 1000.0;
  elapsed_time += (finish_time.tv_usec - start_time.tv_usec) / 1000.0;

  free(array);
  printf("Total: %d\n", total_sum);
  printf("Elapsed time: %fms\n", elapsed_time);
  return 0;
}
