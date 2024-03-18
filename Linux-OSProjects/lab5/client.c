/**
 * Example client program that uses thread pool.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "threadpool.h"

struct data
{
    int a;
    int b;
};

void add(void *param)
{
    struct data *temp;
    temp = (struct data*)param;

    printf("I add two values %d and %d, result = %d\n",temp->a, temp->b, temp->a + temp->b);
}

int main(void)
{
    // create some work to do
    struct data *work;
    int num = 0;
    printf("input numbers of works!\n");
    scanf("%d",&num);
    work = (struct data*)malloc(sizeof(struct data)*num);

    for(int i = 0; i < num; i++)
    {
        printf("input two numbers for the NO.%d work:\n", i+1);
        scanf("%d %d",&work[i].a,&work[i].b);
    }

    // initialize the thread pool
    pool_init();

    // submit the work to the queue
    for(int i = 0; i < num; i++)
    {
        pool_submit(&add,&work[i]);
    }

    // may be helpful 
    sleep(3);

    pool_shutdown();

    return 0;
}
