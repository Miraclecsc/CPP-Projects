#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int *Memory_Page;
const int PageSize = 4096;
const int PageNum = 256;

typedef struct MemoryNode
{
    int start;
    int end;
    struct MemoryNode *prev;
    struct MemoryNode *next;
} MemoryNode;

MemoryNode *head;
MemoryNode *tail;

void init()
{
    Memory_Page = (int*)malloc(PageNum * sizeof(int));
    for(int i = 0; i < PageNum; i++)
        Memory_Page[i] = -1;

    head = (MemoryNode*)malloc(sizeof(MemoryNode));
    tail = (MemoryNode*)malloc(sizeof(MemoryNode));
    MemoryNode *node = (MemoryNode*)malloc(sizeof(MemoryNode));

    head->start = head->end = -1;
    head->prev = NULL;
    head->next = node;

    node->start = 0;
    node->end = PageNum - 1;
    node->prev = head;
    node->next = tail;

    tail->start = tail->end = -1;
    tail->prev = node;
    tail->next = NULL;
}
void clear()
{
    MemoryNode *p = head->next;
    while(p != tail)
    {
        MemoryNode *q = p;
        p = p->next;
        free(q);
    }
    free(head);
    free(tail);
    free(Memory_Page);
}
void sort_forward(MemoryNode *node)
{
    if(node == head->next)
        return;
    MemoryNode *p = node->prev;
    while(p != head && ((p->end - p->start) > (node->end - node->start)))
        p = p->prev;
    p = p->next;
    if(p == node)
        return;
    node->prev->next = node->next;
    node->next->prev = node->prev;
    node->next = p;
    node->prev = p->prev;
    p->prev->next = node;
    p->prev = node;
}
void sort_backward(MemoryNode *node)
{
    if(node == tail->prev)
        return;
    MemoryNode *p = node->next;
    while(p != tail && ((p->end - p->start) < (node->end - node->start)))
        p = p->next;
    p = p->prev;
    if(p == node)
        return;
    node->prev->next = node->next;
    node->next->prev = node->prev;
    node->next = p->next;
    node->prev = p;
    p->next->prev = node;
    p->next = node;
}
void division(MemoryNode *node, int num)
{
    if(num == node->end - node->start + 1)
    {
        node->start = node->end = -1;
        node->prev->next = node->next;
        node->next->prev = node->prev;
        free(node);
        
        return;
    }
    node->start += num;
    sort_forward(node);
    
}
void merge(int start, int end)
{
    if(start == 0 && end != PageNum - 1)
    {
        if(Memory_Page[end + 1] == -1)
        {
            MemoryNode *node = head->next;
            while(node != tail && node->start != end + 1)
                node = node->next;
            node->start = start;
            sort_backward(node);
        }
        else
        {
            MemoryNode *node =malloc(sizeof(MemoryNode));
            node->start = start;
            node->end = end;
            node->prev = head;
            node->next = head->next;
            head->next->prev = node;
            head->next = node;            
            sort_backward(node);            
        }
    }
    else if(end == PageNum - 1 && start != 0)
    {
        if(Memory_Page[start - 1] == -1)
        {
            MemoryNode *node = head->next;
            while(node != tail && node->end != start - 1)
                node = node->next;
            node->end = end;
            sort_forward(node);
        }
        else
        {
            MemoryNode *node =malloc(sizeof(MemoryNode));
            node->start = start;
            node->end = end;
            node->prev = tail->prev;
            node->next = tail;
            tail->prev->next = node;
            tail->prev = node;
            sort_forward(node);           
        }
    }
    else if(start == 0 && end == PageNum - 1)
    {
        MemoryNode *tmp = head->next;
        while(tmp != tail)
        {
            MemoryNode *p = tmp;
            tmp = tmp->next;
            free(p);
        }
        MemoryNode *node = (MemoryNode*)malloc(sizeof(MemoryNode));
        node->start = 0;
        node->end = PageNum - 1;
        node->prev = head;
        node->next = tail;
        head->next = tail;
        tail->prev = head;
    }
    else
    {
        if(Memory_Page[start - 1] == -1 && Memory_Page[end + 1] == -1)
        {
            MemoryNode *node1 = head->next;
            while(node1 != tail && node1->end != start - 1)
                node1 = node1->next;
            MemoryNode *node2 = head->next;
            while(node2 != tail && node2->start != end + 1)
                node2 = node2->next;
            node1->start = start;
            node1->end = node2->end;
            node2->prev->next = node2->next;
            node2->next->prev = node2->prev;
            free(node2);
            sort_backward(node1);
            
        }
        else if(Memory_Page[start - 1] == -1 && Memory_Page[end + 1] != -1)
        {
            MemoryNode *node = head->next;
            while(node != tail && node->end != start - 1)
                node = node->next;
            node->end = end;
            sort_backward(node);    
        }
        else if(Memory_Page[start - 1] != -1 && Memory_Page[end + 1] == -1)
        {
            MemoryNode *node = head->next;
            while(node != tail && node->start != end + 1)
                node = node->next;
            node->start = start;
            sort_backward(node);    
        }
        else
        {
            MemoryNode *node =malloc(sizeof(MemoryNode));
            node->start = start;
            node->end = end;
            node->next = head->next;
            node->prev = head;
            head->next->prev = node;
            head->next = node;
            sort_forward(node);    
        }
    }
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
                    MemoryNode *node = head->next;
                    while(node != tail && node->start != i)
                        node = node->next;
                    division(node, page_num);
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
        if(tail->prev->end - tail->prev->start + 1 > page_num)
        {
            for(int k = tail->prev->start; k < tail->prev->start + page_num; k++)
                Memory_Page[k] = process;
            division(tail->prev, page_num);
            printf("Memory allocated successfully!\n");
        }
        else
            printf("Not enough memory!\n");
    }
    else if(allocation_type[0] == 'B')
    {
        MemoryNode *node = head->next;
        while(node != tail && (node->end - node->start + 1 < page_num))
            node = node->next;
        if(node == tail)
            printf("Not enough memory!\n");
        else
        {  
            for(int k = node->start; k < node->start + page_num; k++)
                Memory_Page[k] = process;
            division(node, page_num);
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
    int start = -1, end = -1;
    for(int i = 0; i < PageNum; i++)
    {
        int flag = 0;
        if(Memory_Page[i] == process)
        {
            flag = 1;
            Memory_Page[i] = -1;
            if(start == -1)
                start = i;
            end = i;
        }
    }
    merge(start, end);
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
    clear();
    return 0;
}