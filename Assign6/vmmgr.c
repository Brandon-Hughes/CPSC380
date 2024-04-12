#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>


typedef struct {
    uint8_t page_offset:8;
    uint8_t pg_num:8;
    uint32_t unused:16;
} pl_TRANS;

typedef union {
    uint32_t ul;
    pl_TRANS bf;
} U_PLTRANS;

typedef struct {
    int page_num;
    int frame_num;
    int time_from_access;
} TLB;

U_PLTRANS splitInt(const char* line);

int checkTable(int page_num);
int checkPage(int page_num);
int getFromBackingStore(int pg_num);

TLB tlb[16];

int main(int argc, char *argv[]){
    FILE *in;
    FILE *back;
    char line[6];
    U_PLTRANS uaddr;

    uint32_t pg_offset;
    uint32_t pg_num;
    char *frame_nums[256];
    char *buffer;
    int free_frame_counter = 0;
    int pageIndex;
    int frame_number;
    int page_fault_rate = 0;
    int tlb_hit_rate = 0;
    size_t page_size = 256;
    int index = 0;
    int pg_nums[256];
    int highest_index;

    if (argc != 2) {
        printf("Usage: %s <addressess>\n", argv[0]);
        return -1;
    }

    for(int i = 0; i < 16; ++i){
        tlb[i].page_num = -1;
        tlb[i].frame_num = -1;
        tlb[i].time_from_access = 0;
    }
    for(int i = 0; i < 256; ++i){
        pg_nums[i] = -1;
    }
    in = fopen(argv[1], "r");
    back = fopen("BACKING_STORE.bin", "rb");

    if (in == NULL) {
        perror("Error opening file");
        return 1;
    }

    if (back == NULL) {
        perror("Error opening file");
        fclose(in);
        return 1;
    }
    while (fgets(line, sizeof(line), in) != NULL) {
        int number = atoi(line);
        if (number != 0 || (number == 0 && line[0] == '0')) {
            printf("Local Address: %s\n", line);
            uaddr = splitInt(line);
            pg_offset = uaddr.bf.page_offset;
            pg_num = uaddr.bf.pg_num;
            printf("Physical Address (Page Number): %d\n", pg_num);
            printf("Physical Address (Page Offset): %d\n", pg_offset);
        } else {
            continue;
        }

        if (checkTable(pg_num) != -1){
            ++tlb_hit_rate;
            frame_number = checkTable(pg_num);
            buffer = frame_nums[frame_number];
        } else {
            if (pg_nums[pg_num] != -1) {
                frame_number = pg_nums[pg_num];
                highest_index = 0;
            } else {
                ++page_fault_rate;
                fseek(back, pg_num * page_size, SEEK_SET);
                buffer = (char *)malloc(page_size);
                if (buffer == NULL) {
                    perror("Error allocating memory");
                    fclose(back);
                    fclose(in);
                    return 1;
                }

                size_t read_size = fread(buffer, 1, page_size, back);
                if (read_size != page_size) {
                    perror("Error reading file");
                    free(buffer);
                    fclose(back);
                    fclose(in);
                    return 1;
                }
                frame_nums[free_frame_counter] = buffer;
                pg_nums[pg_num] = free_frame_counter;
                
                ++free_frame_counter;
            }
            highest_index = 0;
            for(int i = 0; i < 16; ++i){
                if(tlb[i].time_from_access > tlb[highest_index].time_from_access){
                    highest_index = i;
                }
            }
            tlb[highest_index].frame_num = pg_nums[pg_num];
            tlb[highest_index].page_num = pg_num;
            tlb[highest_index].time_from_access = 0;
        }
        printf("Byte Value: %u\n", buffer[pg_offset]);
        for(int i = 0; i < 16; ++i){
            ++tlb[i].time_from_access;
        }
        ++index;
    }
    printf("Page Fault: %d\n", page_fault_rate);
    printf("Page Fault: %.3f%%\n", (double)page_fault_rate/1000);
    printf("TLB Hit Rate: %d\n", tlb_hit_rate);
    printf("TLB Hit Rate: %.3f%%\n", (double)tlb_hit_rate/1000);
    fclose(in);
    fclose(back);
    }   


U_PLTRANS splitInt(const char* line){
    U_PLTRANS u_addr;
    int addr = atoi(line);
    if (addr > 4095){
        u_addr.ul = addr;
    } else {
        u_addr.ul = addr;
        u_addr.bf.pg_num |= 0x1000;
    }
    return u_addr;
}

int checkTable(int page_num) {
    for (int i = 0; i < 16; ++i) {
        if (tlb[i].page_num == page_num) {
            tlb[i].time_from_access = 0;
            return tlb[i].frame_num;
        }
    }
    return -1;
}

