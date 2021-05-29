#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "../queue.h"

#define PROCESS 105
#define ANSI_COLOR_RED		"\x1b[31m"
#define ANSI_COLOR_GREEN 	"\x1b[32m"
#define ANSI_COLOR_YELLOW 	"\x1b[33m"
#define ANSI_COLOR_BLUE 	"\x1b[34m"
#define ANSI_COLOR_MAGENTA 	"\x1b[35m"
#define ANSI_COLOR_CYAN 	"\x1b[36m"
#define ANSI_COLOR_RESET 	"\x1b[0m"

int cnt = 0; // 프로세스의 갯수를 나타냄

int waiting[PROCESS][2] = {};
int executing[PROCESS][2] = {};
int response_time[PROCESS] = {};
int turnaround_time[PROCESS] = {};
int waiting_time[PROCESS] = {};
int origin_burst[PROCESS] = {};
int tot_wait = 0;
int tot_response = 0;
int tot_turnaround = 0;
void getcolor(int n) {
    switch(n % 7) {
        case 0:
            printf(ANSI_COLOR_RESET);
            break;
        case 1:
            printf(ANSI_COLOR_RED);
            break;
        case 2:
            printf(ANSI_COLOR_GREEN);
            break;
        case 3:
            printf(ANSI_COLOR_YELLOW);
            break;
        case 4:
            printf(ANSI_COLOR_BLUE);
            break;
        case 5:
            printf(ANSI_COLOR_MAGENTA);
            break;
        default:
            printf(ANSI_COLOR_CYAN);
    }
} 

void init() {
    for(int i = 0 ; i < PROCESS ; i++) {
        for(int j = 0 ; j < 2 ; j++) {
            waiting[i][j] = -1; executing[i][j] = -1;
        }
    }
}

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
    for(int i = 0 ; i < 50 ; i++) {
        printf("=");
    }
    printf("\n");
}

void makeline2() {
    for(int i = 0 ; i < 50 ; i++) {
        printf("-");
    }
    printf("\n");
}
// 프로세스 스왑
void swap(process *proc1, process *proc2) {
    process tmp;
    tmp = *proc1;
    *proc1 = *proc2;
    *proc2 = tmp;
}

// 프로세스 정렬
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
    makeline();
    init();
    FILE* fi;
    process proc[PROCESS];
    int cursor = 1;
    // 파일에서 프로세스 정보 읽어오는 과정
    fi = fopen("../process.txt", "r");
    FILE* fd = NULL;
    fd = fopen("../log.txt", "w+");
    for(int i = 0 ; !feof(fi) ; i++) {
        ++cnt;
        proc[i].pnum = cnt;
        fscanf(fi, "%d %d %d\n", &proc[i].arrive_time, &proc[i].burst_time, &proc[i].deadline);
        char *pnum;
        pnum = malloc(sizeof(char) * 150);
        sprintf(pnum, "Process[%d]", i + 1);
        proc[i].pname = malloc(sizeof(char) * 150);
        strcpy(proc[i].pname, pnum);
        origin_burst[cnt] = proc[i].burst_time;
    } 

    // 정렬
    sort(proc);

    Queue ready_queue, execute_queue;
    Initialize(&ready_queue, 101);
    Initialize(&execute_queue, 1);
    int cur_time = 0;
    while(cursor <= cnt || size(&ready_queue) || size(&execute_queue)) { // 프로세스가 남아있다면.
        while(cursor <= cnt && proc[cursor - 1].arrive_time <= cur_time) { // 현재 시간 >= 도착시간
            push(&ready_queue, proc[cursor - 1]); // 레디 큐에 푸쉬
            waiting[proc[cursor - 1].pnum][0] = cur_time; // cursor 프로세스는 waiting을 시작하게 됨.
            getcolor(cursor);
            printf("( Process [%d] is inserted in Ready Queue )\n", cursor);
            cursor++;
        }
        if(size(&ready_queue) && !size(&execute_queue)) { // 레디 큐에만 프로세스가 존재
            process exec_proc = pop(&ready_queue); // 새로 실행하게 될 프로세스
            if(cur_time != waiting[exec_proc.pnum][0]) { // 들어오자마자 실행되는 것이 아닐 경우 wait 존재
                fprintf(fd, "%s %d %d %s\n",exec_proc.pname, waiting[exec_proc.pnum][0], cur_time, "Waiting");
            }
            getcolor(exec_proc.pnum);
            waiting[exec_proc.pnum][1] = cur_time;

            waiting_time[exec_proc.pnum] += waiting[exec_proc.pnum][1] - waiting[exec_proc.pnum][0];
            response_time[exec_proc.pnum] = cur_time - exec_proc.arrive_time;
            
            printf("( Process [%d] is starting at %d )\n",exec_proc.pnum, cur_time);
            executing[exec_proc.pnum][0] = cur_time; // exec_proc 실행 시작  
            push(&execute_queue, exec_proc); // execute_queue에 삽입
        }

        if(size(&execute_queue)) { // 실행을 위한 프로세스 존재
            execution(&execute_queue); // 실행
            if(check_isend(&execute_queue)) { // burst_time이 남아있지 않을 경우
                process remove_proc = pop(&execute_queue); // execute_queue에서 제거  
                getcolor(remove_proc.pnum);
                executing[remove_proc.pnum][1] = cur_time + 1;
                printf("( Process [%d] is ended at %d )\n",remove_proc.pnum, cur_time + 1);
                fprintf(fd, "%s %d %d %s\n", remove_proc.pname, executing[remove_proc.pnum][0], cur_time + 1, "Executing");
            }
        }
        cur_time++;
    }
    printf(ANSI_COLOR_RESET);
    makeline();
    printf("\n\n");
    for(int i = 1 ; i <= cnt ; i++) {
        turnaround_time[i] = origin_burst[i] + waiting_time[i]; 
    }

    for(int i = 1 ; i <= cnt ; i++) {
        tot_wait += waiting_time[i];
        tot_response += response_time[i];
        tot_turnaround += turnaround_time[i];

        printf("Process [%d] Information\n", i);
        printf("Waiting Time : %d\nResponse Time : %d\nTurn-Around Time : %d\n", waiting_time[i], response_time[i], turnaround_time[i]);
        makeline2();
    }

    printf("\n\n");

    printf("Result\nAverage Waiting Time : %.2lf\nAverage Response Time : %.2lf\nAverage Turn-Around Time : %.2lf\n\n",
    (double)tot_wait/cnt, (double)tot_response/cnt, (double)tot_turnaround/cnt);
    return 0;
}