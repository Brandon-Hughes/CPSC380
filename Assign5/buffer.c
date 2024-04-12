#include "buffer.h"

/* the buffer */
struct buffer_item buffer[BUFFER_SIZE];
int front = 0;
int back = 0;
int count = 0;

sem_t empty, full;
pthread_mutex_t mutex;

void buffer_initialize(){
    sem_init(&empty, 0, BUFFER_SIZE);
    sem_init(&full, 0, 0);
    pthread_mutex_init(&mutex, NULL);
}

int insert_item(struct buffer_item item) {
    /*insert item into buffer
        return 0 if successful, otherwise
        return -1 indictating an error condition */
    sem_wait(&empty);
    pthread_mutex_lock(&mutex);

    if (count < BUFFER_SIZE){
        buffer[back] = item;
        ++back;
        if(back > 10){
            back = 0;
        }
        ++count;
        pthread_mutex_unlock(&mutex);
        sem_post(&full);
        return 0;
    } else {
        printf("Buffer Full\n");
        pthread_mutex_unlock(&mutex);
        sem_post(&empty);
        return -1;
    }
}

int remove_item(struct buffer_item *item) {
    /* remove an object from buffer
        placing it in item
        return 0 if successful, otherwise
        return -1 indiciating an error condition */
    sem_wait(&full);
    pthread_mutex_lock(&mutex);
    if (count > 0){
        *item = buffer[front];
        ++front;
        if(front > 10){
            front = 0;
        }
        --count;
        pthread_mutex_unlock(&mutex);
        sem_post(&empty);
        return 0;
    } else {
        printf("Buffer Empty\n");
        pthread_mutex_unlock(&mutex);
        sem_post(&full);
        return -1;
    }
}

void close_buffer() {
    sem_destroy(&empty);
    sem_destroy(&full);
    pthread_mutex_destroy(&mutex);
}