#include <iostream>
#include <unistd.h>
#include <pthread.h>

// Мьютексы для блокировки
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex2 = PTHREAD_MUTEX_INITIALIZER;

void* thread1_function(void*) {
    std::cout << "Thread 1: Attempting to lock mutex1\n";
    pthread_mutex_lock(&mutex1);
    std::cout << "Thread 1: Locked mutex1\n";

    sleep(2);

    std::cout << "Thread 1: Attempting to lock mutex2\n";
    pthread_mutex_lock(&mutex2);
    std::cout << "Thread 1: Locked mutex2\n";

    pthread_mutex_unlock(&mutex1);
    pthread_mutex_unlock(&mutex2);

    return NULL;
}

void* thread2_function(void*) {
    std::cout << "Thread 2: Attempting to lock mutex2\n";
    pthread_mutex_lock(&mutex2);
    std::cout << "Thread 2: Locked mutex2\n";

    sleep(2);

    std::cout << "Thread 2: Attempting to lock mutex1\n";
    pthread_mutex_lock(&mutex1);
    std::cout << "Thread 2: Locked mutex1\n";

    pthread_mutex_unlock(&mutex2);
    pthread_mutex_unlock(&mutex1);

    return NULL;
}

int main() {
    pthread_t thread1, thread2;

    if (pthread_create(&thread1, NULL, thread1_function, NULL) != 0) {
        std::cerr << "Error creating thread 1\n";
        return 1;
    }

    if (pthread_create(&thread2, NULL, thread2_function, NULL) != 0) {
        std::cerr << "Error creating thread 2\n";
        return 1;
    }

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    std::cout << "Main thread exits\n";

    return 0;
}