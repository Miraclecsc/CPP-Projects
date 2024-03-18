#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


#define NUMBER_OF_CUSTOMERS 5
#define NUMBER_OF_RESOURCES 4

int available[NUMBER_OF_RESOURCES];
int maximum[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];
int allocation[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];
int need[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];

int request_resources(int customer_num, int request[])
{
    for(int i = 0; i < NUMBER_OF_RESOURCES; i++)
    {
        if(request[i] > need[customer_num][i])
            {
                printf("%d customer doesn't need so many resources!\n", customer_num);
                return -1;
            }
        if(request[i] > available[i])
            {
                printf("Not enough resources!\n");
                return -1;
            }
    }
    for(int i = 0; i < NUMBER_OF_RESOURCES; i++)
    {
        available[i] -= request[i];
        allocation[customer_num][i] += request[i];
        need[customer_num][i] -= request[i];
    }
    int safe = 0;
    int finish[NUMBER_OF_CUSTOMERS] = {0};
    int work[NUMBER_OF_RESOURCES];
    for(int i = 0; i < NUMBER_OF_RESOURCES; i++)
        work[i] = available[i];
    for(int i = 0; i < NUMBER_OF_CUSTOMERS; i++)
    {
        if(finish[i] == 0)
        {
            int flag = 0;
            for(int j = 0; j < NUMBER_OF_RESOURCES; j++)
            {
                if(need[i][j] > work[j])
                {
                    flag = 1;
                    break;
                }
            }
            if(flag == 0)
            {
                finish[i] = 1;
                for(int j = 0; j < NUMBER_OF_RESOURCES; j++)
                    work[j] += allocation[i][j];
                i = -1;
            }
        }
    }
    for(int i = 0; i < NUMBER_OF_CUSTOMERS; i++)
    {
        if(finish[i] == 0)
        {
            safe = -1;
            break;
        }
    }
    if(safe == -1)
    {
        for(int i = 0; i < NUMBER_OF_RESOURCES; i++)
        {
            available[i] += request[i];
            allocation[customer_num][i] -= request[i];
            need[customer_num][i] += request[i];
        }
        printf("The state is not safe!\n");
        return -1;
    }
    else
    {
        printf("Successfully allocate the resources!\n");
        return 0;
    }
}
void release_resources(int customer_num, int release[])
{
    for(int i = 0; i < NUMBER_OF_RESOURCES; i++)
    {
        if(release[i] > allocation[customer_num][i])
        {
            printf("%d customer doesn't have so many resources!\n", customer_num);
            return;
        }
    }
    for(int i = 0; i < NUMBER_OF_RESOURCES; i++)
    {
        available[i] += release[i];
        allocation[customer_num][i] -= release[i];
        need[customer_num][i] += release[i];
    }
    printf("Successfully release the resources!\n");
}


int main(int argc,char* argv[])
{
    if(argc != NUMBER_OF_RESOURCES+1)
    {
        printf("Invalid Number of Arguments.\n");
        return -1;
    }
    for(int i = 0; i < NUMBER_OF_RESOURCES; i++)
        available[i] = atoi(argv[i+1]);
    FILE *input;
    input = fopen("input.txt","r");
    char *line = (char*)malloc(sizeof(char)*100);
    for(int i = 0; i < NUMBER_OF_CUSTOMERS; i++)
    {
        // deal with the input file.
        fgets(line,100,input);
        char *token = strtok(line,",");
        for(int j = 0; j < NUMBER_OF_RESOURCES; j++)
        {
            maximum[i][j] = atoi(token);
            allocation[i][j] = 0;
            need[i][j] = maximum[i][j];
            token = strtok(NULL,",");
        }
    }
    free(line);
    fclose(input);

    char *command = (char*)malloc(sizeof(char)*100);
    int request[NUMBER_OF_RESOURCES] = {0};
    while(1)
    {
        printf("Enter Command:\n");
        fgets(command,100,stdin);
        if(strcmp(command,"exit\n") == 0)
            break;
        if(strcmp(command,"*\n") == 0)
        {
            printf("Available:\n");
            for(int i = 0; i < NUMBER_OF_RESOURCES; i++)
                printf("%d ",available[i]);
            printf("\n");
            printf("Maximum:\n");
            for(int i = 0; i < NUMBER_OF_CUSTOMERS; i++)
            {
                for(int j = 0; j < NUMBER_OF_RESOURCES; j++)
                    printf("%d ",maximum[i][j]);
                printf("\n");
            }
            printf("Allocation:\n");
            for(int i = 0; i < NUMBER_OF_CUSTOMERS; i++)
            {
                for(int j = 0; j < NUMBER_OF_RESOURCES; j++)
                    printf("%d ",allocation[i][j]);
                printf("\n");
            }
            printf("Need:\n");
            for(int i = 0; i < NUMBER_OF_CUSTOMERS; i++)
            {
                for(int j = 0; j < NUMBER_OF_RESOURCES; j++)
                    printf("%d ",need[i][j]);
                printf("\n");
            }
            printf("\n");
            continue;
        }
        char *token = strtok(command," ");
        char *command_type = token;
        token = strtok(NULL," ");
        int customer_num = atoi(token);
        token = strtok(NULL," ");
        for(int i = 0; i < NUMBER_OF_RESOURCES; i++)
        {
            request[i] = atoi(token);
            token = strtok(NULL," ");
        }
        if(strcmp(command_type,"RQ") == 0)
        {
            if(request_resources(customer_num,request) == 0)
                printf("Request Granted.\n\n");
            else
                printf("Request Denied.\n\n");
        }
        else if(strcmp(command_type,"RL") == 0)
        {
            release_resources(customer_num,request);
            printf("Resources Released.\n\n");
        }
        else
            printf("Invalid Command.\n");
        
    }
    free(command);
    return 0;

}