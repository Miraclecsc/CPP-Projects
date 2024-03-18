#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "cpu.h"
#include "task.h"
#include "list.h"
#include "schedulers.h"

struct node *taskList = NULL;
int taskCount = 0;
void add(char *name, int priority, int burst)
{
    Task *newTask = malloc(sizeof(Task));
    newTask->name = malloc(sizeof(char) * (strlen(name) + 1));
    strcpy(newTask->name, name);
    newTask->priority = priority;
    newTask->burst = burst;
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
    while(current != NULL)
    {
        task = current->task;
        run(task, task->burst);
        current = current->next;
    }

    if(taskCount > 0)
    {
        double turn = 0, wait = 0, response = 0;
        struct node *temp = taskList;
        for(int i = 0; i < taskCount; i++)
        {
            int time = temp->task->burst;
            temp = temp->next;
            turn += time * (taskCount-i);
            wait += time * (taskCount-i-1);
            response += time * (taskCount-i-1);
        }
        printf("Average Turnaround Time: %f\n", turn/taskCount);
        printf("Average Waiting Time: %f\n", wait/taskCount);
        printf("Average Response Time: %f\n", response/taskCount);
    }
}