#include <pthread.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <semaphore.h>

#define N 5
#define THINKING 0
#define WAITING 1
#define EATING 2

#define LEFT (id + 4) % N
#define RIGHT (id) % N
#define LOOP_COUNT 5
#define TOTAL_EAT_COUNT 5*LOOP_COUNT

int state[N];
int phil[N] = { 0, 1, 2, 3, 4 };
sem_t forks[N];

void serve_philospher(int id)
{
    if (state[id] == WAITING) {
        state[id] = EATING;

        // take up forks
        printf("Philosopher %d takes forks %d and %d\n",
                    id + 1, LEFT + 1, id + 1);

        printf("Philosopher %d is Eating\n", id + 1);

        sleep(1);

        // put down forks
        printf("Philosopher %d putting forks %d and %d down\n", id + 1, LEFT + 1, id + 1);
        printf("Philosopher %d is thinking\n", id + 1);

        state[id] = THINKING;
    }
}

void* philosopher(void* num)
{
    int id = *((int *)num);
	while(true){
        state[id] = WAITING;

        printf("Philosopher %d is Hungry\n", id + 1);

        // take min fork followed by max fork
        int min = LEFT < RIGHT ? LEFT : RIGHT;
        int max = LEFT < RIGHT ? RIGHT : LEFT;
        //printf("Philosopher: %d, thread id: %ld, min fork: %d, max fork: %d\n", id+1, pthread_self(), min+1, max+1);
        sem_wait(&forks[min]);
        sem_wait(&forks[max]);

        serve_philospher(id);

        // release the forkss
        sem_post(&forks[min]);
        sem_post(&forks[max]);

        sleep(1);
    }
}

int main()
{
    int i;
    pthread_t thread_id[N];

    // initialize semaphores
    for (i = 0; i < N; i++) {
        sem_init(&forks[i], 0, 1);
    }

    for (i = 0; i < N; i++) {
        state[i] = THINKING;
    }
    for (i = 0; i < N; i++) {
        // create philosopher threads
        pthread_create(&thread_id[i], NULL, philosopher, &phil[i]);
    }

    for (i = 0; i < N; i++)
        pthread_join(thread_id[i], NULL);

    return 0;
}
