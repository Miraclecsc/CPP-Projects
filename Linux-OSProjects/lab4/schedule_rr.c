#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "cpu.h"
#include "task.h"
#include "list.h"
#include "schedulers.h"
    
int taskCount = 0;
struct node *taskList = NULL;

void add(char *name, int priority, int burst){
    Task *newTask = malloc(sizeof(Task));
    newTask->name = malloc(sizeof(char) * (strlen(name) + 1));
    strcpy(newTask->name, name);
    newTask->priority = priority;
    newTask->burst = newTask->burst_remaining = burst;
    newTask->tid = taskCount++;
    insert(&taskList, newTask);
}
void schedule()
{
    struct node *current = taskList, *next = NULL, *front = NULL;
    while(current != NULL && current->next != NULL)
    {
        next = current->next;
        current->next = front;
        front = current;
        current = next;
    }
    if(current != NULL && current->next == NULL)
        current->next = front;
    taskList = current;

    Task *task;
    int time = 0;
    int turn_time[500]={0};
    int response_time[500]={0};
    int response_flag[50]={0};

    int flag = 1;
    while(flag)
    {
        flag = 0;
        current = taskList;
        while(current != NULL)
        {
            //printf("Time: %d\n", time);
            task = current -> task;
            if (task->burst_remaining > QUANTUM)
            {
                //printf("Task %s is running for %d units.\n", task->name, QUANTUM);
                run (task, QUANTUM);
                task->burst_remaining -= QUANTUM;
                flag = 1;
                if(response_flag[task->tid]==0)
                {
                    response_flag[task->tid]=1;
                    response_time[task->tid]=time;
                }
                time += QUANTUM;
            }
            else if (task->burst_remaining > 0)
            {
                run (task, task->burst_remaining);
                int tmp = task->burst_remaining;
                task->burst_remaining = 0;
                flag = 1;
                
                if(response_flag[task->tid]==0)
                {
                    response_flag[task->tid]=1;
                    response_time[task->tid]=time;
                }
                time += tmp;
                turn_time[task->tid] = time;
            }
            current = current->next;
        }
    }
    if(taskCount > 0)
    {
        double turn = 0, wait = 0, response = 0;
        for(int i = 0; i < taskCount; i++)
        {
            turn += turn_time[i];
            response += response_time[i];
        }
        wait = turn;
        struct node *temp = taskList;
        while(temp != NULL)
        {
            wait -= temp->task->burst;
            temp = temp->next;
        }
        printf("Average Turnaround Time: %f\n", turn/taskCount);
        printf("Average Waiting Time: %f\n", wait/taskCount);
        printf("Average Response Time: %f\n", response/taskCount);
    }
}
