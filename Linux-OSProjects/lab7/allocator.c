#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int *Memory_Page;
const int PageSize = 4096;
const int PageNum = 256;
void init()
{
    Memory_Page = (int*)malloc(PageNum * sizeof(int));
    for(int i = 0; i < PageNum; i++)
        Memory_Page[i] = -1;
}
void memory_request(int process, int memory_size, char *allocation_type)
{
    int page_num = memory_size / PageSize;
    if (memory_size % PageSize)
        page_num ++;
    

    if(allocation_type[0] == 'F')
    {
        int i = 0;
        while(i < PageNum)
        {
            if(Memory_Page[i] == -1)
            {
                int j = i;
                while(j < PageNum && Memory_Page[j] == -1)
                    j++;
                if(j - i >= page_num)
                {
                    for(int k = i; k < i + page_num; k++)   
                        Memory_Page[k] = process;
                    printf("Memory allocated successfully!\n");
                    return;
                }
                else
                    i = j;
            }
            else
                i++;
        }
        printf("Not enough memory!\n");
    }
    else if(allocation_type[0] == 'W')
    {
        int i = 0, start = 0;
        int max = 0;
        while(i < PageNum)
        {
            if(Memory_Page[i] == -1)
            {
                int j = i;
                while(j < PageNum && Memory_Page[j] == -1)
                    j++;
                if(j - i >= page_num && j - i > max)
                {
                    max = j - i;
                    start = i;
                }
                i = j;
            }
            else
                i++;
        }
        if(max == 0)
            printf("Not enough memory!\n");
        else
        {
            for(int k = start; k < start + page_num; k++)   
                Memory_Page[k] = process;
            printf("Memory allocated successfully!\n");
        }
    }
    else if(allocation_type[0] == 'B')
    {
        int i = 0, start = 0;
        int min = PageNum;
        while(i < PageNum)
        {
            if(Memory_Page[i] == -1)
            {
                int j = i;
                while(j < PageNum && Memory_Page[j] == -1)
                    j++;
                if(j - i >= memory_size && j - i < min)
                {
                    min = j - i;
                    start = i;
                }
                i = j;
            }
            else
                i++;
        }
        if(min == PageNum)
            printf("Not enough memory!\n");
        else
        {
            for(int k = start; k < start + page_num; k++)   
                Memory_Page[k] = process;
            printf("Memory allocated successfully!\n");
        }
    }
    else
    {
        printf("Invalid allocation type!\n");
    }
}
void memory_release(int process)
{
    for(int i = 0; i < PageNum; i++)
    {
        if(Memory_Page[i] == process)
            Memory_Page[i] = -1;
    }
    printf("Memory released successfully!\n");
}

void status_report()
{
    int i = 0;
    while(i < PageNum)
    {
        int begin = i;
        int proc = Memory_Page[i];
        while(i < PageNum && Memory_Page[i] == proc)
        {
            i++;
        }
        if(proc != -1)
            printf("Addresses[%d:%d] Process P%d.\n", begin * PageSize, i * PageSize - 1, proc);
        else
            printf("Addresses[%d:%d] Unused.\n", begin * PageSize, i * PageSize - 1);
    }
}
int main()
{
    init();
    while(1)
    {
        printf("\nallocator>");
        char *command = (char*)malloc(100);
        fgets(command, 100, stdin);

        if(strcmp(command, "exit\n") == 0)
            break;
        if(strcmp(command, "STAT\n") == 0)
        {
            status_report();
            continue;
        } 

        char *token = strtok(command, " ");
        if(strcmp(token, "RQ") == 0)
        {
            int process, memory_size;
            char *allocation_type;
            token = strtok(NULL, " ");
            process = token[1] - '0';
            token = strtok(NULL, " ");
            memory_size = atoi(token);
            token = strtok(NULL, " ");
            allocation_type = token;
            memory_request(process, memory_size, allocation_type);
        }
        else if(strcmp(token, "RL") == 0)
        {
            int process;
            token = strtok(NULL, " ");
            process = token[1] - '0';

            memory_release(process);
        }
        else
        {
            printf("Invalid command\n");
        }
    }
    return 0;
}