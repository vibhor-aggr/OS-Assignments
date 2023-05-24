#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

#define N 5
#define THINKING 0
#define WAITING 1
#define EATING 2

#define LEFT (i + 4) % N
#define RIGHT (i + 1) % N
#define LOOP_COUNT 5
#define TOTAL_EAT_COUNT 5*LOOP_COUNT

int state[N];
int phil[N] = { 0, 1, 2, 3, 4 };

void serve_philosphers()
{
    int serveCnt = 0;
    while (1) {
        for (int i = 0; i < N; i++) {
            if (state[i] == WAITING) {
                state[i] = EATING;

                // take up forks
                printf("Philosopher %d takes fork %d and %d\n",
                            i + 1, LEFT + 1, i + 1);

                printf("Philosopher %d is Eating\n", i + 1);

                sleep(1);

                // put down forks
                printf("Philosopher %d putting fork %d and %d down\n", i + 1, LEFT + 1, i + 1);
                printf("Philosopher %d is thinking\n", i + 1);

                state[i] = THINKING;

                serveCnt++;
            }
        }
        if (serveCnt == TOTAL_EAT_COUNT) break;
    }
}

void* philosopher(void* num)
{
    int id = *((int *)num);
	while(true){
        state[id] = WAITING;

        printf("Philosopher %d is Hungry\n", id + 1);

        while (state[id] != THINKING) ;
        sleep(1);
    }
}

int main()
{
    int i;
    pthread_t thread_id[N];

    for (i = 0; i < N; i++) {
        state[i] = THINKING;
    }
    for (i = 0; i < N; i++) {
        // create philosopher threads
        pthread_create(&thread_id[i], NULL, philosopher, &phil[i]);
    }

    serve_philosphers();

    for (i = 0; i < N; i++)
        pthread_join(thread_id[i], NULL);

    return 0;
}
