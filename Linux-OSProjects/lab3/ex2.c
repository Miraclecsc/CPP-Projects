#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
int *array;
int *array_sort;
int n = 0;
typedef struct
{
    int begin;
    int mid;
    int end;
}arg_struct;
void *threads_sort(void *args)
{
    arg_struct *arg = (arg_struct *)args;
    int begin = arg->begin;
    int end = arg->end;
    //printf("begin: %d,  end: %d\n", begin, end);
    if (begin >= end)
        return NULL;
    int pivot = array[end];
    int i = begin - 1;
    for(int j=begin; j<end; j++)
    {
        if (array[j] < pivot)
        {
            i++;
            int temp = array[i];
            array[i] = array[j];
            array[j] = temp;
        }
    }
    int temp = array[i+1];
    array[i+1] = array[end];
    array[end] = temp;
    arg_struct *args1, *args2;
    args1 = (arg_struct *)malloc(sizeof(arg_struct));
    args2 = (arg_struct *)malloc(sizeof(arg_struct));
    args1->begin = begin;
    args1->end = i;
    args2->begin = i+2;
    args2->end = end;
    pthread_t tid1, tid2;
    pthread_create(&tid1, NULL, threads_sort, (void *)args1);
    pthread_create(&tid2, NULL, threads_sort, (void *)args2);
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    free(args1);
    free(args2);
}
void *threads_merge(void *args)
{
    arg_struct *arg = (arg_struct *)args;
    int begin = arg->begin;
    int mid = arg->mid;
    int end = arg->end;
    
    int i = begin, j = mid+1, k = begin;
    while(i <= mid && j <= end)
    {
        if (array[i] < array[j])
            array_sort[k++] = array[i++];
        else
            array_sort[k++] = array[j++];
    }
    while(i <= mid)
        array_sort[k++] = array[i++];
    while(j <= end)
        array_sort[k++] = array[j++];
}
int main()
{
    printf("Enter n: ");
    scanf("%d", &n);
    array = (int *)malloc(n * sizeof(int));
    array_sort = (int *)malloc(n * sizeof(int));
    printf("Enter the elements of the array: \n");
    for(int i=0; i<n; i++)
    {
        scanf("%d", &array[i]);
    }
    if (n == 1)
    {
        printf("Sorted array: %d\n", array[0]);
        return 0;
    }  
    pthread_t tid1, tid2, tid3;
    arg_struct *args1, *args2, *args3;
    args1 = (arg_struct *)malloc(sizeof(arg_struct));
    args2 = (arg_struct *)malloc(sizeof(arg_struct));
    args3 = (arg_struct *)malloc(sizeof(arg_struct));

    int mid = (n-1)/2;
    args1->begin = 0;
    args1->end = mid;
    args2->begin = mid+1;
    args2->end = n-1;
    args3->begin = 0;
    args3->mid = mid;
    args3->end = n-1;

    pthread_create(&tid1, NULL, threads_sort, (void *) args1);
    pthread_create(&tid2, NULL, threads_sort, (void *) args2);

    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);

    pthread_create(&tid3, NULL, threads_merge, (void *) args3);
    pthread_join(tid3, NULL);

    printf("Sorted array: \n");
    for(int i=0; i<n; i++)
    {
        printf("%d ", array_sort[i]);
    }
    free(array);
    free(array_sort);
    free(args1);
    free(args2);
    free(args3);

    return 0;

}