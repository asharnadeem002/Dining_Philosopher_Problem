#include<stdlib.h>
#include <stdio.h>
#include<unistd.h>
#include <pthread.h>
#include <string.h>
#include <semaphore.h>
#define think 0
#define want 1
#define eat 2
sem_t mutex;
sem_t *chpstk;
int n;
int *currST;
void check(int i) {
    if (currST[i] == want && currST[(i+n-1)%n] != eat && currST[(i+1)%n] != eat) {
        currST[i] = eat;
        printf("Philosopher %d eat\n", i+1);
        sem_post(&chpstk[i]);
    }
}
void *ThrFunc(void *arg) {
    int i = *(int*) arg;
    int c=0;
    while (1) {
        printf("Philosopher %d think\n", i+1);
        sleep(1);
        sem_wait(&mutex);
        currST[i] = want;
        printf("Philosopher %d want\n", i+1);
        check(i);
        sem_post(&mutex);
        sem_wait(&chpstk[i]);
        sleep(1);
        sem_wait(&mutex);
        currST[i] = think;
        printf("Philosopher %d eat\n", i+1);
        check((i+n-1)%n);
        check((i+1)%n);
        sem_post(&mutex);
        //c++;
    }
}
int main(int argc, char** argv){
    if (argc != 2) {
        printf("Please enter Number of Philosopher as an argument!\n");
        return 1;
    }
    n = atoi(argv[1]);
    if (n <= 0) {
        printf("N cannot be negative or 0!\n");
        return 1;
    }
    currST = malloc(sizeof(int) * n);
    chpstk = malloc(sizeof(sem_t) * n);
    int i;
    pthread_t thr1[n];
    sem_init(&mutex, 0, 1);
    for (i = 0; i < n; i++)
        sem_init(&chpstk[i], 0, 1);
    for (i = 0; i < n; i++)
        pthread_create(&thr1[i], NULL, ThrFunc, &i);
    for (i = 0; i < n; i++)
        pthread_join(thr1[i], NULL);
    return 0;
}

