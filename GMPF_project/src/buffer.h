#ifndef BUFFER_H
#define BUFFER_H

#define BUF_SIZE 100

typedef struct savebuf s_savebuf;
typedef struct buffer s_buffer;

enum action {
    MOVE_UP,
    MOVE_DOWN,
    MODIF_IMAGE,
    CHANGE_NAME,
    DELETE,
    ADD
};

struct savebuf {
    enum action act;
    int layer; /* Peut-etre mettre des id uniques aux calques.*/
};

struct buffer {
    FILE buff[BUF_SIZE];
    int begin;
    int size;
    int pos;
};

s_buffer *buffer_create();
void buffer_init(s_buffer *buf);
void buffer_destroy(s_buffer *buf);

void buffer_add(s_buffer *buf, FILE *f);
FILE * buffer_undo(s_buffer *buf);
FILE * buffer_redo(s_buffer *buf);

#endif


// # include <semaphore.h>
// # include <stdlib.h>
// # include <err.h>
// # include <stdio.h>
//
// #include "shared_queue.h"
//
//
// struct queue {
//     FILE val;
//     struct queue *next;
// };
//
// struct queue *queue_push(struct queue *queue, FILE val)
// {
//     struct queue *q = malloc (sizeof (struct queue));
//     q->val = val;
//
//     if (!queue)
//         q->next = q;
//
//     else
//     {
//         q->next = queue->next;
//         queue->next = q;
//     }
//
//     return q;
// }
//
// FILE queue_pop(struct queue *queue)
// {
//     if (!queue)
//         err(1, "shared_queue_pop(), queue is empty");
//
//     struct queue *q = queue->next;
//     int x = q->val;
//
//     if (q->next == q)
//     {
//         free (q);
//         queue = NULL;
//         return x;
//     }
//
//     else
//         queue->next = q->next;
//
//     free (q);
//     printf("queue->val = %i\n", queue->val);
//     return x;
// }
//
// void queue_destroy(struct queue *queue)
// {
//     struct queue *q = queue->next;
//
//     while (queue)
//     {
//         free (queue);
//         queue = q;
//         if (!q)
//             break;
//         q = q->next;
//     }
// }
