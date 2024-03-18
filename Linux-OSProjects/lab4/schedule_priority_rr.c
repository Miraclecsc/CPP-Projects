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

    struct node *current_node, *front;
    current_node = taskList;
    front = NULL;
    if(current_node==NULL)
    {
        insert(&taskList, newTask);
        return ;
    }
    while(current_node!=NULL)
    {
        if(priority <= current_node->task->priority )
        {
            front = current_node;
            current_node = current_node->next;
        }
        else
        {
            struct node* node_newtask = malloc(sizeof(struct node));
            node_newtask->task = newTask;
            node_newtask->next = current_node;
            if(front==NULL)
                taskList = node_newtask;
            else
                front->next = node_newtask;
            return;
        }
    }
    if(current_node==NULL)
    {
        struct node* node_newtask = malloc(sizeof(struct node));
        node_newtask->task = newTask;
        node_newtask->next = NULL;
        front->next = node_newtask;
        return;
    }
}
void schedule()
{
    struct node *current_head = taskList;
    struct node *current = taskList;

    Task *task;
    int time = 0;
    int turn_time[500]={0};
    int response_time[500]={0};
    int response_flag[50]={0};

    
    while(1)
    {
        int pri = current_head->task->priority;
        int flag = 1;
        while(flag)
        {
            flag = 0;
            current = current_head;
            struct node *temp;
            while(current != NULL && current->task->priority == pri)
            {
                task = current -> task;
                if (task->burst_remaining > QUANTUM)
                {
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
                temp = current->next;
                if(temp != NULL && (temp->task->priority == pri))
                    current = current->next;
                else break;
            }
        }
        if(current->next != NULL)
        {
            current = current->next;
            current_head = current;
        }
        else break;
    }
    /*int flag = 1;
    while(flag)
    {
        flag = 0;
        current = taskList;
        while(current != NULL)
        {
            task = current -> task;
            if (task->burst_remaining > QUANTUM)
            {
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
    }*/

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
