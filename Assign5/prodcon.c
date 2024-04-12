#include <stdlib.h>
#include <stdio.h> 
#include <unistd.h> 
#include "buffer.h"

void *consumer(void *param);
void *producer(void *param);
uint16_t calculateChecksum(const uint8_t data[], int length);

int main(int argc, char *argv[]) {
    /*
        1. Get command line arguments argv[1], argv[2], argv[3]
        2. Initialize buffer
        3. Create producer thread(s)
        4. Create consume thread(s)
        5. Sleep
        6. Exit
    */
    if(argc != 4){
        fprintf(stderr,"usage: ./prodcon <delay> <#producer threads> <#consumer threads>\n");
        return -1;
    }

    int delay = atoi(argv[1]);
    int producer_threads = atoi(argv[2]);
    int consumer_threads = atoi(argv[3]);

    buffer_initialize();

    pthread_t pThreads[producer_threads];
    pthread_t cThreads[consumer_threads];
    
    for(int i = 0; i < producer_threads; ++i){
        pthread_create(&pThreads[i], NULL, producer, NULL);
    }

    for(int i = 0; i < consumer_threads; ++i){
        pthread_create(&cThreads[i], NULL, consumer, NULL);
    }

    sleep(delay);

    for (int i = 0; i < producer_threads; ++i) {
        pthread_join(pThreads[i], NULL);
    }

    for (int i = 0; i < consumer_threads; ++i) {
        pthread_join(cThreads[i], NULL);
    }

    close_buffer();

    return 0;


}

void *producer(void *param){
    struct buffer_item item;

    while(1) {
        /*sleep for a random period of time */
        sleep(rand() % 6);
        /* generate a random number */
        for(int i = 0; i < 30; ++i){
            item.data[i] = (uint8_t)(rand() % 256);
        }
        item.cksum = calculateChecksum(item.data, 30);
        if(insert_item(item) == -1){
            printf("Error occured while inserting item\n");
            break;
        }
        else{
            printf("Producer produced %d\n", item.cksum);
            break;
        }
    }
}

void *consumer(void *param){
    struct buffer_item item;

    while(1) {
        /*sleep for a random period of time */
        sleep(rand() % 6);
        if (remove_item(&item) == -1) {
            printf("Error occured while removing item\n");
            break;
        } else {
            uint16_t consumer_checksum = calculateChecksum(item.data, 30);
            if(consumer_checksum == item.cksum){
                printf("Consumer consumed %d\n", item.cksum);
            } else {
                printf("Checksum mismatch\n");
            }
            break;
        }
    }
}


uint16_t calculateChecksum(const uint8_t data[], int length) {
    uint16_t checksum = 0;

    for (int i = 0; i < length; i++) {
        checksum ^= data[i];
    }

    return checksum;
}