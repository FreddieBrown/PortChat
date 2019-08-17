#include "queue.h"

void init_q(struct queue * que) {
    que->front = NULL;
    que->rear = NULL;
    que->len = 0;
    if(pthread_mutex_init(&(que->rwmutex), NULL) != 0){
        printf("Mutex failed to initialise for queue\n");
    }
    if(pthread_cond_init(&(que->filled), NULL) != 0){
        printf("Conditional Variable failed to initialise for queue\n");
    }

}

int push(struct data * data, struct queue * que) {
    pthread_mutex_lock(&(que->rwmutex));
    if(que->front == NULL){
        que->front = data;
        que->rear = data;
    }
    else{
        que->rear->next = data;
        que->rear = data;
    }
    que->len++;
    pthread_mutex_unlock(&(que->rwmutex));
        if(pthread_cond_signal(&(que->filled)) != 0){
            printf("Signal failed in add\n");
        }
    return 0;
}

struct data * pull(struct queue * que) {
    pthread_mutex_lock(&(que->rwmutex));
    if(que->len == 0){
        return NULL;
    }
    struct data * taken = que->front;
    if(taken->next == NULL){

        que->front = NULL;
        que->rear = NULL;

    }
    else{
        que->front = taken->next;
    }
    que->len -= 1;
    pthread_mutex_unlock(&(que->rwmutex));
    return taken;
}

void free_q(struct queue * que) {
    struct data * d;
    while(que->len!= 0){
        d = pull(que);
        free(d);
    }
}