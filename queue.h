#include <stdio.h>
#include <stdlib.h>
#include "process.h"
#define MAX 1000001

typedef struct {
	int max; // max size of queue
	process *proc;
	int rear; // input side
	int front; // output side
	int num; // how many processes in queue
}Queue;

void Initialize(Queue *q, int max) {
	q -> num = 0;
	q -> proc = calloc(max, sizeof(process));
	q -> max = max;
	q -> rear = 0;
	q -> front = 0;
}

int push(Queue *q, process new_proc) {
	if(q -> num >= q -> max) {
		return -1;  // queue is full
	}
	q -> proc[q -> rear++] = new_proc;
	q -> num++;
	if(q -> rear >= q -> max) {
		q -> rear = 0;
	}
    return 0;
}

process pop(Queue *q) {
	q -> num--;
	if( (q -> front) + 1 == q -> max ) {
		process pop_proc = q -> proc[q -> front];
		q -> front = 0;
		return pop_proc;
	}
	return q -> proc[q -> front++];
}

int size(Queue *q) {
	return q -> num;
}

char* print_top(Queue *q) {
    return q -> proc[q -> front].pname;
}

int print_top_num(Queue *q) {
    return q -> proc[q -> front].pnum;
} 

int print_top_time(Queue *q) {
    return q -> proc[q -> front].burst_time;
}

void execution(Queue *q) {
    q -> proc[q -> front].burst_time--;
}

int check_isend(Queue *q) {
    return q -> proc[q -> front].burst_time == 0;
}
void print_queue(Queue *q) {
    Queue n = *q;
    while(size(&n) > 0) {
        printf("+     %s     +\n",pop(&n).pname);
    }
}

void print_queue2(Queue *q) {
	for(int i = q -> front ; i < q -> rear ; i++) {
		printf("+     %s     +\n", q -> proc[i].pname);
	}
}

int check_num(Queue *q) {
    Queue n = *q;
    if(size(&n) > 0) {
        process p = pop(&n);
        check_num(&n);
        return p.pnum;
    }
    return -1;
}

void check_queue(Queue *q) {
    while(q -> num != 0) {
        printf("pop : %s\n", q -> proc[q->front].pname);
        pop(q);
    }
}