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

int is_prior_by_cycle(process proc1, process proc2) { // 주기 확인
    int f = proc1.deadline - proc1.arrive_time; // proc1의 주기
    int s = proc2.deadline - proc2.arrive_time; /// proc2의 주기
    if(f < s) return 1; // f의 주기가 더 짧음. 1 리턴
    else return 0; // s의 주기가 더 짧음. 0 리턴 
}

/*
    1 리턴 시 proc1이 더 우선순위를 갖게됩니다.
*/
int is_prior_by_remain(process proc1, process proc2) { // 주기 확인
    int f = proc1.burst_time; // proc1의 주기
    int s = proc2.burst_time; /// proc2의 주기
    if(f == s) {
        return proc1.arrive_time > proc2.arrive_time;
    }
    if(f > s) return 1; // f > s 이므로 s가 작업시간 짧음.
    else return 0; // s의 주기가 더 짧음. 0 리턴 
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
    while(idx > 0 && is_prior_by_remain(h -> proc[paridx], h -> proc[idx])) {
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
            if(is_prior_by_remain(h -> proc[leftidx], h -> proc[rightidx])) {
                curidx = rightidx;
            }
            else curidx = leftidx;
        }

        if(is_prior_by_remain(h -> proc[paridx], h -> proc[curidx])) {
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

void execution(Heap *h) {
    h -> proc[0].burst_time--;
}

int check_isend(Heap *h) {
    return h -> proc[0].burst_time == 0;
}

int size(Heap *h) {
	return h -> num;
}