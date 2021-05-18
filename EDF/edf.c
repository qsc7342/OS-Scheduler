#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include "../heap_deadline.h"

#define PROCESS 10
#define ANSI_COLOR_RED		"\x1b[31m"
#define ANSI_COLOR_GREEN 	"\x1b[32m"
#define ANSI_COLOR_YELLOW 	"\x1b[33m"
#define ANSI_COLOR_BLUE 	"\x1b[34m"
#define ANSI_COLOR_MAGENTA 	"\x1b[35m"
#define ANSI_COLOR_CYAN 	"\x1b[36m"
#define ANSI_COLOR_RESET 	"\x1b[0m"

int cnt = 0;
int visit[101][PROCESS] = {}; // 간트차트 생성용
int inready[101] = {};
int tot_wait = 0;
int isdead[101] = {};
int tot_turnaround = 0;
void move_cur(int x, int y)
{
	printf("\033[%dd\033[%dG", y, x);
}

int min(int a, int b) {
    return a > b ? b : a;
}

int max(int a, int b) {
    return a > b ? a : b;
}

void makeline() {
    for(int i = 0 ; i < 120 ; i++) {
        printf("=");
    }
    printf("\n");
}

void goup(int n) {
    printf ("\x1b[%dA", n);
}

void clear_terminal() {
    pid_t pid;
    pid = fork();
    int status;
    if(pid == 0) {
        execlp("clear", "clear", NULL);
    }
    else {
        wait(&status);
    }
}
/*
    정렬 과정에서 필요한 swap함수 입니다.
*/
void swap(process *proc1, process *proc2) {
    int tmp_arr, tmp_burst, tmp_dead, tmp_num;
    char *tmp_name;
    tmp_arr = proc1->arrive_time;
    tmp_burst = proc1->burst_time;
    tmp_dead = proc1->deadline;
    tmp_name = proc1->pname;
    tmp_num = proc1->pnum;

    proc1->arrive_time = proc2->arrive_time;
    proc1->burst_time = proc2->burst_time;
    proc1->deadline = proc2->deadline;
    proc1->pname = proc2->pname;
    proc1->pnum = proc2->pnum;

    proc2->arrive_time = tmp_arr;
    proc2->burst_time = tmp_burst;
    proc2->deadline = tmp_dead;
    proc2->pname = tmp_name;
    proc2->pnum = tmp_num;
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
    int cursor = 0;
    // 파일에서 프로세스 정보 읽어오는 과정
    fi = fopen("../process.txt", "r");
    for(int i = 0 ; !feof(fi) ; i++) {
        ++cnt;
        proc[i].pnum = cnt;
        fscanf(fi, "%d %d %d\n", &proc[i].arrive_time, &proc[i].burst_time, &proc[i].deadline);
        char *pnum;
        pnum = malloc(sizeof(char) * 10);
        sprintf(pnum, "Process %d", i + 1);
        proc[i].pname = malloc(sizeof(char) * 10);
        strcpy(proc[i].pname, pnum);
    } 

    // 정렬
    sort(proc);
    Heap ready_queue;
    Initialize(&ready_queue, 101);
    Heap execute_queue;
    Initialize(&execute_queue, 1);

    for(int cur_time = 0 ; cur_time <= 100 ; cur_time++) { // 시간이 지날수록
        system("clear");
        while(cursor < cnt &&  proc[cursor].arrive_time <= cur_time) {
            inready[proc[cursor].pnum] = 1;
            push(&ready_queue, proc[cursor++]);
        }
        while(size(&ready_queue) && cur_time > print_top_arrive(&ready_queue) + print_top_deadline(&ready_queue)) {
            process dead_proc = pop(&ready_queue);
            isdead[dead_proc.pnum] = 1;
            visit[cur_time][dead_proc.pnum] = -2;
        }
        while(size(&execute_queue) && cur_time > print_top_arrive(&execute_queue) + print_top_deadline(&execute_queue)) {
            process dead_proc = pop(&execute_queue);
            isdead[dead_proc.pnum] = 1;
            visit[cur_time][dead_proc.pnum] = -2;
        }
        if(size(&ready_queue) && size(&execute_queue) == 0) {
            process exec_proc = pop(&ready_queue);
            inready[exec_proc.pnum] = 0;
            push(&execute_queue, exec_proc);
        }
        if(size(&ready_queue) > 0 && size(&execute_queue) > 0) {
            if(print_top_deadline(&ready_queue) < print_top_deadline(&execute_queue)) {
                process new_execute = pop(&ready_queue);
                process new_ready = pop(&execute_queue);
                inready[new_execute.pnum] = 0;
                inready[new_ready.pnum] = 1;
                push(&execute_queue, new_execute);
                push(&ready_queue, new_ready);
            }
        }
        move_cur(0, 0);
        printf(ANSI_COLOR_GREEN"Current Time : %d\n\n", cur_time);
        printf(ANSI_COLOR_YELLOW"==== Ready Queue ====\n");
        if(size(&ready_queue) > 0) {
            print_queue(&ready_queue);
            for(int ready = 0 ; ready < 100 ; ready++) {
                if(isdead[ready] == 1) continue;
                else if(inready[ready] == 1) {
                    tot_wait++;
                    visit[cur_time][ready] = -1;
                }
            }
        }
        else printf("No Process Remain\n");
        printf("=====================\n");

        printf(ANSI_COLOR_CYAN);
        if(size(&execute_queue) > 0) {
            move_cur(30, 3);
            printf("===============================\n");

            move_cur(30, 4);
            printf("+ Executing Process : %s\n", print_top(&execute_queue));
            move_cur(30, 5);
            printf("+ Remaining Time : %d\n", print_top_time(&execute_queue));

            move_cur(30, 6);
            printf("===============================\n");

            execution(&execute_queue);
            visit[cur_time][print_top_num(&execute_queue)] = 1;
            if(check_isend(&execute_queue)) {
                process remove_proc = pop(&execute_queue); 
                tot_turnaround += (cur_time + 1) - remove_proc.arrive_time;
            }
        }
        else {
            move_cur(30, 3);
            printf("===============================\n");
            move_cur(30, 4);
            printf("No Process Executing Now\n");
            move_cur(30, 5);
            printf("===============================\n");
        }
        printf(ANSI_COLOR_RESET);
        move_cur(0, 10);
        printf("-Gantt Chart-\n");
        printf("===================================================\n");
        printf(ANSI_COLOR_YELLOW"Waiting : █    ");
        printf(ANSI_COLOR_GREEN"Executing : █    ");
        printf(ANSI_COLOR_RED"Deadline Occur : █     \n");
        printf(ANSI_COLOR_RESET);
        printf("===================================================\n");
        printf("            ");
        for(int j = 0 ; j <= cur_time ; j++) {
            if(j % 10 == 0) printf("%d", j / 10);
            else printf(" ");
        }
        printf("\n");
        printf("            ");
        for(int j = 0 ; j <= cur_time ; j++) {
            if(j % 5 == 0) printf("+");
            else printf("-");
        }
        printf("\n");
        for(int k = 0 ; k < cnt ; k++) {
            printf(ANSI_COLOR_RESET);
            printf("PROCESS %d : ", k + 1);
            for(int j = 0 ; j <= cur_time ; j++) {
                if(visit[j][k + 1] == -1) printf(ANSI_COLOR_YELLOW"█");
                else if(visit[j][k + 1] == 1) printf(ANSI_COLOR_GREEN"█");
                else if(visit[j][k + 1] == -2) printf(ANSI_COLOR_RED"█");
                else printf(" ");
            }
            printf("\n");
        }
        fflush(stdout);

        usleep(1000);
    }
    
    
    fclose(fi);
    printf(ANSI_COLOR_RESET);
    move_cur(0, 30);
    printf("==========================================================\n");
    printf("\nAverage Wait Time : %.2lf\n\n", (double)tot_wait/cnt);
    printf("Average Turn_Around Time : %.2lf\n\n", (double)tot_turnaround/cnt);
    printf("==========================================================\n");
    
    return 0;
}