#ifndef QUEUE_H
#define QUEUE_H

typedef struct a{

    int a;
    struct a *next;

} elm;


int read_first_element(elm *);

elm* delete(elm *);

void insert(elm *, int);

int count_queue(elm *);

void printf_queue(elm *);

#endif
