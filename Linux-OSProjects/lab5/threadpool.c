/**
 * Implementation of thread pool.
 */

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include "threadpool.h"

#define QUEUE_SIZE 100
#define NUMBER_OF_THREADS 3

// this represents work that has to be 
// completed by a thread in the pool
typedef struct 
{
    void (*function)(void *p);
    void *data;
}
task;

// the work queue
task worktodo;
task task_queue[QUEUE_SIZE];
int length = 0;
// the worker bee
pthread_t threads_pool[NUMBER_OF_THREADS];
pthread_mutex_t mutex;
sem_t semphore;

int threads_flag[NUMBER_OF_THREADS];
// insert a task into the queue
// returns 0 if successful or 1 otherwise, 
int enqueue(task t) 
{
    if(length == QUEUE_SIZE)
        return 1;
    else
    {
        pthread_mutex_lock(&mutex);
        task_queue[length] = t;
        length++;
        pthread_mutex_unlock(&mutex);
        return 0;
    }
}

// remove a task from the queue
task dequeue() 
{
    if(length == 0)
    {
        worktodo.function = NULL;
        worktodo.data = NULL;
    }
    else
    {
        pthread_mutex_lock(&mutex);
        worktodo = task_queue[0];
        for(int i = 0; i < length - 1; i++)
            task_queue[i] = task_queue[i + 1];
        task_queue[--length].function = NULL;
        task_queue[length].data = NULL;
        pthread_mutex_unlock(&mutex);
    }
    return worktodo;
}

// the worker thread in the thread pool
void *worker(void *param)
{
    // execute the task
    int *intPTR = (int*) param;
    int id = *intPTR;
    worktodo = dequeue();
    pthread_mutex_lock(&mutex);
    printf("The NO.%d thread is running...\n", id);
    execute(worktodo.function, worktodo.data);
    printf("The NO.%d thread is done\n", id);
    printf("\n");
    threads_flag[id] = 0;
    sem_post(&semphore);
    pthread_mutex_unlock(&mutex);
    pthread_exit(0);
}

/**
 * Executes the task provided to the thread pool
 */
void execute(void (*somefunction)(void *p), void *p)
{
    (*somefunction)(p);
}

/**
 * Submits work to the pool.
 */
int pool_submit(void (*somefunction)(void *p), void *p)
{
    worktodo.function = somefunction;
    worktodo.data = p;
    int flag = enqueue(worktodo);
    int id = 0;
    if(flag)
        return 1;
    else
    {
        sem_wait(&semphore);
        while(threads_flag[id])
            id = (id + 1) % NUMBER_OF_THREADS;
        threads_flag[id] = 1;
        pthread_create(&threads_pool[id], NULL, worker, &id);
    }
    return 0;
}

// initialize the thread pool
void pool_init(void)
{
    length = 0;
    for(int i = 0; i < NUMBER_OF_THREADS; i++)
        threads_flag[i] = 0;
    pthread_mutex_init(&mutex, NULL);
    sem_init(&semphore, 0, NUMBER_OF_THREADS);
}

// shutdown the thread pool
void pool_shutdown(void)
{
    for(int i = 0; i < NUMBER_OF_THREADS; i++)
        sem_post(&semphore);
    for(int i = 0; i < NUMBER_OF_THREADS; i++)
        pthread_join(threads_pool[i], NULL);
    pthread_mutex_destroy(&mutex);
    sem_destroy(&semphore);
}
