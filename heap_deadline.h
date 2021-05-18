#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include "process.h"

typedef struct {
    process *proc;
	int max; // max size of queue
	int num; // how many processes in queue
}Heap;

void Initialize(Heap *h, int max) {
	h -> max = max;
    h -> proc = calloc(max, sizeof(process));
    h -> num = 0;
    return;
}

int is_prior_by_deadline(process proc1, process proc2) { // 주기 확인
    int f = proc1.deadline + proc1.arrive_time; // proc1의 데드라인
    int s = proc2.deadline + proc2.arrive_time; /// proc2의 데드라인
    if(f > s) return 1; // f의 데드라인이 더 김 1 리턴
    else return 0; // s의 데드라인이 더 김 0 리턴
}


void swapnode(Heap *h, int idx1, int idx2) {
    process tmp;
    tmp = h -> proc[idx1];
    h -> proc[idx1] = h -> proc[idx2];
    h -> proc[idx2] = tmp;
}

int getpar(int idx) {
    return (int)((idx - 1) / 2);
}

int getleft(int idx) {
    return 2 * idx + 1;
}

int getright(int idx) {
    return 2 * idx + 2;
}

void push(Heap *h, process proc) {
    int idx = h -> num;
    int paridx = getpar(idx);
    h -> proc[idx] = proc;
    while(idx > 0 && is_prior_by_deadline(h -> proc[paridx], h -> proc[idx])) {
        swapnode(h, idx, paridx);
        idx = paridx;
        paridx = getpar(idx);
    }
    h -> num++;
} 

process pop(Heap *h) {
    int paridx = 0;
    int leftidx = 0;
    int rightidx = 0;
    process pop_proc = h->proc[0];
 
    h -> num--;
    swapnode(h, 0, h -> num);
    leftidx = getleft(0);
    rightidx = getright(0);

    while(1) {
        int curidx = 0;
        if(leftidx >= h -> num) break;
        if(rightidx >= h -> num) {
            curidx = leftidx;
        }
        else {
            if(is_prior_by_deadline(h -> proc[leftidx], h -> proc[rightidx])) {
                curidx = rightidx;
            }
            else curidx = leftidx;
        }

        if(is_prior_by_deadline(h -> proc[paridx], h -> proc[curidx])) {
            swapnode(h, paridx, curidx);
            paridx = curidx;
        }
        else break;

        leftidx = getleft(paridx);
        rightidx = getright(paridx);
    }
    return pop_proc;
}

void print_queue(Heap *h) {
    for(int i = 0 ; i < h -> num ; i++) {
        printf("+     %s     +\n", h->proc[i].pname);
    }
    return;
}

char* print_top(Heap *h) {
    return h -> proc[0].pname;
}

int print_top_num(Heap *h) {
    return h -> proc[0].pnum;
} 

int print_top_time(Heap *h) {
    return h -> proc[0].burst_time;
}

int print_top_deadline(Heap *h) {
    return h -> proc[0].deadline + h -> proc[0].arrive_time;
}

int print_top_arrive(Heap *h) {
    return h -> proc[0].arrive_time;
}

process print_top_process(Heap *h) {
    return h -> proc[0];
}
void execution(Heap *h) {
    h -> proc[0].burst_time--;
}

int check_isend(Heap *h) {
    return h -> proc[0].burst_time == 0;
}

int size(Heap *h) {
	return h -> num;
}

void reconstruct(Heap *h, int idx) {
    int smallest = idx;
    int leftidx = getleft(idx);
    int rightidx = getright(idx);
    if(leftidx < h->num && is_prior_by_deadline(h->proc[smallest], h->proc[leftidx])) smallest = leftidx;
    if(rightidx < h -> num && is_prior_by_deadline(h->proc[smallest], h->proc[rightidx])) smallest = rightidx;
    if(smallest != idx) {
        swapnode(h, smallest, idx);
        reconstruct(h, smallest);
    }
}
