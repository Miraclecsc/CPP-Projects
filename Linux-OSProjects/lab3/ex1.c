#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
/*
6 2 4 5 3 9 1 8 7          
5 1 9 7 2 8 6 3 4
8 3 7 6 1 4 2 9 5
1 4 3 8 6 5 7 2 9 
9 5 8 2 4 7 3 6 1
7 6 2 3 9 1 4 5 8
3 7 1 9 5 6 8 4 2
4 9 6 1 8 2 5 7 3
2 8 5 4 7 3 9 1 6
*/
int array[9][9] = {0}; 
int checkArray[11] = {0};
typedef struct
{
    int kind;
}arg_struct;
void checkBlock (int i)
{
    int row_begin = (i % 3) * 3;
    int col_begin = (i / 3) * 3;
    int checklist[9] = {0};
    for(int j=row_begin; j<row_begin+3; j++)
    {
        for(int k=col_begin; k<col_begin+3; k++)
        {
            if(checklist[array[j][k]-1] == 0)
            {
                checklist[array[j][k]-1] = 1;
            }
            else
            {
                checkArray[i] = 1;
                return;
            }
        }
    }
    for (int j = 0; j < 9; j++)
    {
        if (checklist[j] == 0)
        {
            checkArray[i] = 1;
            return;
        }
    }
}
void checkRow ()
{

    for(int i=0; i<9; i++)
    {
        int checklist[9] = {0};
        for(int j=0; j<9; j++)
        {
            if(checklist[array[i][j]-1] == 0)
            {
                checklist[array[i][j]-1] = 1;
            }
            else
            {
                checkArray[9] = 1;
                return;
            }
        }
        for (int j = 0; j < 9; j++)
        {
            if (checklist[j] == 0)
            {
                checkArray[9] = 1;
                return;
            }
        }
    }
}
void checkCol ()
{
    for(int i=0; i<9; i++)
    {
        int checklist[9] = {0};
        for(int j=0; j<9; j++)
        {
            if(checklist[array[j][i]-1] == 0)
            {
                checklist[array[j][i]-1] = 1;
            }
            else
            {
                checkArray[10] = 1;
                return;
            }
        }
        for (int j = 0; j < 9; j++)
        {
            if (checklist[j] == 0)
            {
                checkArray[10] = 1;
                return;
            }
        }
    }
}
void *check(void *args)
{
    arg_struct *arg = (arg_struct *)args;
    int i = arg->kind;
    if (i >= 0 && i < 9)
    {
        checkBlock(i);   
    }
    else if (i == 9)
    {
        checkRow();
    }
    else
    {
        checkCol();
    }
}
int main()
{
    printf("Enter the elements of the 9x9 array: \n");
    for(int i=0; i<9; i++)
    {
        for(int j=0; j<9; j++)
        {
            scanf("%d", &array[i][j]);
        }
    }
    pthread_t threads[11];
    arg_struct *thread_args[11];
    for (int i=0; i<11; i++)
    {
        thread_args[i] = malloc(sizeof(arg_struct));
        thread_args[i] -> kind = i;
    }
    for (int i=0; i<11; i++)
    {
        pthread_create(&threads[i], NULL, check, (void *) thread_args[i]);
    }
    for(int i=0; i<11; i++)
    {
        pthread_join(threads[i], NULL);
    }
    for(int i=0; i<11; i++)
    {
        if(checkArray[i] == 1)
        {
            printf("The sudoku is invalid, the error is in the ");
            if(i < 9)
            {
                printf("block %d\n", i+1);
            }
            else if(i == 9)
            {
                printf("row\n");
            }
            else
            {
                printf("column\n");
            }
            return 0;
        }
    }
    printf("The sudoku is valid\n");
    return 0;

}