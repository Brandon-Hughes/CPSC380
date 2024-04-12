#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

/* buffer.h */
typedef struct buffer_item {
	uint8_t  data[30];
	uint16_t cksum;
} BUFFER_ITEM;

#define BUFFER_SIZE 10

void buffer_initialize();
int insert_item(struct buffer_item item);
int remove_item(struct buffer_item *item);
void close_buffer();
