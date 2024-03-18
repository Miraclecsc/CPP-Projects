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
    newTask->burst = burst;
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

void schedule(){
    struct node* current_node;
    current_node = taskList;

    while(current_node!=NULL)
    {
        run(current_node->task,current_node->task->burst);
        current_node = current_node->next;
    }

    if(taskCount > 0){
        double turn = 0;
        double wait = 0;
        double response = 0;

        struct node *temp = taskList;

        for(int i=0; i<taskCount; i++){
            int time = temp->task->burst;
            temp = temp->next;

            turn += time * (taskCount-i);
            wait += time * (taskCount-i-1);
            response += time * (taskCount-i-1);
        }

        printf("Average turnaround time: %f\n", turn / taskCount);
        printf("Average waiting time : %f\n", wait / taskCount);
        printf("Average response time : %f\n",response / taskCount);

    }
}