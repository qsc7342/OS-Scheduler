#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include "process.h"

typedef struct {
    process *proc;
}Node;

typedef struct {
    Node* node;
	int max; // max size of queue
	int num; // how many processes in queue
}Heap;

void Initialize(Heap *h, int max) {
	h -> max = max;
	h = (Heap*)malloc(sizeof(Heap));
    h -> num = 0;
    return;
}

int is_prior_by_remain(process *proc1, process *proc2) { // 주기 확인
    int f = proc1 -> burst_time; // proc1의 주기
    int s = proc2 -> burst_time; /// proc2의 주기
    if(f < s) return 1; // f의 주기가 더 짧음. 1 리턴
    else return 0; // s의 주기가 더 짧음. 0 리턴 
}

void swapnode(Heap *h, int idx1, int idx2) {
    Node tmp;
    tmp.proc = h -> node[idx1].proc;
    h -> node[idx1].proc = h -> node[idx2].proc;
    h -> node[idx2].proc = tmp.proc;
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

void push(Heap *h, process *proc) {
    int idx = h -> num;
    int paridx = getpar(idx);
    while(idx > 0 && is_prior_by_remain(h -> node[idx].proc, h -> node[paridx].proc)) {
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
    process* pop_proc = h -> node[0].proc;

    h -> node[0].proc = NULL;
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
            if(is_prior_by_remain(h -> node[leftidx].proc, h -> node[rightidx].proc)) {
                curidx = rightidx;
            }
            else curidx = leftidx;
        }

        if(is_prior_by_remain(h -> node[paridx].proc, h -> node[curidx].proc)) {
            swapnode(h, paridx, curidx);
            paridx = curidx;
        }
        else break;

        leftidx = getleft(paridx);
        rightidx = getright(paridx);
    }
}

void printheap(Heap *h) {
    for(int i = 0 ; i < h -> num ; i++) {
        printf("+     %s     +\n", h->node[i].proc->pname);
    }
    return;
}
 