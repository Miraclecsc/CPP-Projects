#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>
#include <unistd.h>

typedef int buffer_item;
#define BUFFER_SIZE 5

/* the buffer */
buffer_item buffer[BUFFER_SIZE];
int wait_list[BUFFER_SIZE];

int insert_item(buffer_item item)
{

    int flag = -1;
    for (int i = 0; i < BUFFER_SIZE; i++)
    {
        if (wait_list[i])
        {
            buffer[i] = item;
            wait_list[i] = 0;
            flag = 0;
            break;
        }
    }
    return flag;
}

int remove_item(buffer_item *item)
{

    int flag = -1;
    for (int i = 0; i < BUFFER_SIZE; i++)
    {
        if (!wait_list[i])
        {
            (*item) = buffer[i];
            wait_list[i] = 1;
            flag = 0;
            break;
        }
    }

    return flag;
}

pthread_mutex_t mutex;
sem_t full;
sem_t empty;

void *producer(void *param)
{
    buffer_item item;
    while (1)
    {
        int time = rand() % 3;
        sleep(time);

        
        sem_wait(&empty);
        pthread_mutex_lock(&mutex);

        item = rand();
        if (insert_item(item))
            printf("Producing error\n");
        else
            printf("The %d is produced\n", item);

        pthread_mutex_unlock(&mutex);
        sem_post(&full);
        
    }
}

void *consumer(void *param)
{
    buffer_item item;
    while (1)
    {
        int time = rand() % 3;
        sleep(time);

        sem_wait(&full);
        pthread_mutex_lock(&mutex);
        
        if (remove_item(&item))
            printf("Consuming error\n");
        else
            printf("The %d is consumed\n", item);

        pthread_mutex_unlock(&mutex);
        sem_post(&empty);
    }
}

int main(int argc, char *argv[])
{
    //part 1
    int time = atoi(argv[1]);
    int num_producer = atoi(argv[2]);
    int num_consumer = atoi(argv[3]);

    //part 2
    pthread_mutex_init(&mutex, NULL);
    sem_init(&full, 0, 0);
    sem_init(&empty, 0, BUFFER_SIZE);
    for (int i = 0; i < BUFFER_SIZE; i++)
        wait_list[i] = 1;
    printf("Start\n");
    //part 3
    pthread_t producer_thread[num_producer];
    for (int i = 0; i < num_producer; i++)
    {
        pthread_create(&producer_thread[i], NULL, producer, NULL);
    }

    //part 4
    pthread_t consumer_thread[num_consumer];
    for (int i = 0; i < num_consumer; i++)
    {
        pthread_create(&consumer_thread[i], NULL, consumer, NULL);
    }

    //part 5
    printf("Sleeping begin\n");
    sleep(time);
    printf("Sleeping end\n");

    //part 6

    for (int i = 0; i < num_producer; i++)
        pthread_cancel(producer_thread[i]);
    for (int i = 0; i < num_consumer; i++)
        pthread_cancel(consumer_thread[i]);

    sem_destroy(&full);
    sem_destroy(&empty);
    pthread_mutex_destroy(&mutex);
    printf("Finish\n");
    return 0;
}