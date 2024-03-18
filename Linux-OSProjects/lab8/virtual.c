#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define TLB_SIZE 16
#define PAGE_TABLE_SIZE 256

#define PAGE_SIZE 256
#define FRAME_SIZE 256
#define FRAME_NUMBER 256

int tlb_hit = 0, page_fault = 0, address_count = 0;
typedef struct {
    int last_use_time;
    int page_number;
    int frame_number;
} TLB_struct;
TLB_struct TLB[TLB_SIZE];

typedef struct {
    int valid;
    int frame_number;
} PageTable_struct;
PageTable_struct PageTable[PAGE_TABLE_SIZE];

typedef struct {
    int last_use_time;
    char frame[FRAME_SIZE];
} Memory_struct;
Memory_struct Memory[FRAME_NUMBER];

FILE *backing_store, *address_file, *output_file;
void init()
{
    for(int i = 0; i < TLB_SIZE; i++)
    {
        TLB[i].last_use_time = -1;
        TLB[i].page_number = -1;
        TLB[i].frame_number = -1;
    }
    for(int i = 0; i < PAGE_TABLE_SIZE; i++)
    {
        PageTable[i].valid = 0;
        PageTable[i].frame_number = -1;
    }
    for(int i = 0; i < FRAME_NUMBER; i++)
    {
        for(int j = 0; j < FRAME_SIZE; j++)
        {
            Memory[i].last_use_time = -1;
            Memory[i].frame[j] = -1;
        }
    }
}
int get_frame(int num)
{
    for(int i=0; i<TLB_SIZE; i++)
    {
        if(TLB[i].page_number == num)
        {
            tlb_hit++;
            TLB[i].last_use_time = address_count;
            return TLB[i].frame_number;
        }
    }
    if(PageTable[num].valid)
    {
        int min_time = 0x7fffffff, min_time_index = 0;
        for(int i=0; i<TLB_SIZE; i++)
        {
            if(TLB[i].last_use_time < min_time)
            {
                min_time = TLB[i].last_use_time;
                min_time_index = i;
            }
        }
        TLB[min_time_index].last_use_time = address_count;
        TLB[min_time_index].page_number = num;
        TLB[min_time_index].frame_number = PageTable[num].frame_number;
        return TLB[min_time_index].frame_number;
    }
    else
    {
        page_fault++;
        int min_time = 0x7fffffff, min_time_index = 0;
        for(int i=0; i<FRAME_NUMBER; i++)
        {
            if(Memory[i].last_use_time < min_time)
            {
                min_time = Memory[i].last_use_time;
                min_time_index = i;
            }
        }
        Memory[min_time_index].last_use_time = address_count;
        fseek(backing_store, num * PAGE_SIZE, SEEK_SET);
        fread(Memory[min_time_index].frame, sizeof(char), FRAME_SIZE, backing_store);
        for(int i=0; i<PAGE_TABLE_SIZE; i++)
        {
            if(PageTable[i].frame_number == min_time_index)
            {
                PageTable[i].valid = 0;
                break;
            }
        }
        PageTable[num].valid = 1;
        PageTable[num].frame_number = min_time_index;
        return get_frame(num);
    }
}
int main()
{
    address_file = fopen("addresses.txt", "r");
    output_file = fopen("output.txt", "w");
    backing_store = fopen("BACKING_STORE.bin", "rb");
    
    init();
    int vir_addr, vir_page, vir_offset, phy_addr, phy_frame, phy_offset;
    while(~fscanf(address_file, "%d", &vir_addr))
    {
        address_count++;
        vir_addr = vir_addr & 0x0000ffff;
        vir_page = (vir_addr & 0x0000ff00) >> 8;
        vir_offset = vir_addr & 0x000000ff;
        phy_offset = vir_offset;
        phy_frame = get_frame(vir_page);
        Memory[phy_frame].last_use_time = address_count;
        phy_addr = (phy_frame << 8) + phy_offset;
        int val = (int)Memory[phy_frame].frame[phy_offset];
        int value = (val > 127) ? val - 256 : val;
        fprintf(output_file, "Virtual address: %d Physical address: %d Value: %d\n", vir_addr, phy_addr, value);
    }
    double tlb_hit_rate = 100.0*tlb_hit / address_count;
    double page_fault_rate = 100.0*page_fault / address_count;
    printf("TLB hit rate: %lf %%\n", tlb_hit_rate);
    printf("Page fault rate: %lf %%\n", page_fault_rate);
    fclose(address_file);
    fclose(output_file);
    fclose(backing_store);
    return 0;
}