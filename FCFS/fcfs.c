#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define PROCESS 10


/*
    프로세스 구조체
*/
typedef struct process {
    int arrive_time;
    int burst_time;
    int deadline;
    char *pname;
}process;

int cnt = 0;


void makeline() {
    printf("==============================\n");
}

/*
    정렬 과정에서 필요한 swap함수 입니다.
*/
void swap(process *proc1, process *proc2) {
    int tmp_arr, tmp_burst, tmp_dead;
    char *tmp_name;
    tmp_arr = proc1->arrive_time;
    tmp_burst = proc1->burst_time;
    tmp_dead = proc1->deadline;
    tmp_name = proc1->pname;

    proc1->arrive_time = proc2->arrive_time;
    proc1->burst_time = proc2->burst_time;
    proc1->deadline = proc2->deadline;
    proc1->pname = proc2->pname;

    proc2->arrive_time = tmp_arr;
    proc2->burst_time = tmp_burst;
    proc2->deadline = tmp_dead;
    proc2->pname = tmp_name;
}


/*
    도착 시간순으로 프로세스를 정렬합니다.
*/
void sort(process *proc) {
    for(int i = 0 ; i < cnt ; i++) {
        for(int j = i + 1 ; j < cnt ; j++) {
            if(proc[i].arrive_time > proc[j].arrive_time) {
                swap(&proc[i], &proc[j]);
            }
        }
    }
    return;
}

int main() {
    FILE* fi;
    process proc[PROCESS];

    fi = fopen("process.txt", "r");
    int i = 0;
    for(i = 0 ; !feof(fi) ; i++) {
        ++cnt;
        fscanf(fi, "%d %d %d\n", &proc[i].arrive_time, &proc[i].burst_time, &proc[i].deadline);
        char *pnum;
        sprintf(pnum, "Process[%d]", i + 1);
        proc[i].pname = malloc(sizeof(char) * 10);
        strcpy(proc[i].pname, pnum);
    }

    for(int j = 0 ; j < i ; j++) {
        printf("%s : %d %d %d\n", proc[j].pname, proc[j].arrive_time, proc[j].burst_time, proc[j].deadline);
    }
    sort(proc);
    makeline();
    printf("After Sorting...\n");
    makeline();
    for(int j = 0 ; j < i ; j++) {
        printf("%s : %d %d %d\n", proc[j].pname, proc[j].arrive_time, proc[j].burst_time, proc[j].deadline);
    }  
    fclose(fi);
    return 0;
}