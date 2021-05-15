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
int visit[100][PROCESS] = {}; // 간트차트 생성용

int min(int a, int b) {
    return a > b ? b : a;
}

int max(int a,int b) {
    return a > b ? a : b;
}

void makeline() {
    for(int i = 0 ; i < 120 ; i++) {
        printf("=");
    }
    printf("\n");
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

    // 파일에서 프로세스 정보 읽어오는 과정
    fi = fopen("process.txt", "r");
    for(int i = 0 ; !feof(fi) ; i++) {
        ++cnt;
        fscanf(fi, "%d %d %d\n", &proc[i].arrive_time, &proc[i].burst_time, &proc[i].deadline);
        char *pnum;
        pnum = malloc(sizeof(char) * 10);
        sprintf(pnum, "Process %d", i + 1);
        proc[i].pname = malloc(sizeof(char) * 10);
        strcpy(proc[i].pname, pnum);
    } 

    // 정렬
    sort(proc);
    
    int tot_wait = 0; // 총 대기시간, 이후에 평균 대기시간 구할 때 사용
    int cur_time = 0; // CPU 시간. 대기 시간 등을 구할 때 사용
    int wait_time[PROCESS] = {}; // 각 프로세스 별 대기 시간, 필요는 있을지 모르겠음.

    for(int i = 0 ; i < cnt ; i++) {
        int cur_arrive = proc[i].arrive_time;
        int cur_burst = proc[i].burst_time;
        int cur_dead = proc[i].deadline;
        char *pnum;
        // 프로세스 이름에서 프로세스 넘버를 추출하기 위함. (정렬로 인한 필요)
        pnum = strtok(proc[i].pname, " ");
        pnum = strtok(NULL, " ");
        int real_pnum = atoi(pnum); // 문자열 -> 정수형 변환
        wait_time[real_pnum] = max(0, cur_time - cur_arrive); // 대기시간 : 현재 시간 - 도착 시간과 0 중 최솟값
        tot_wait += wait_time[real_pnum]; // 토탈에 더함
        for(int j = cur_arrive ; j < cur_time ; j++) {
            if(i == 0) printf("wait %d\n", j);
            visit[j][real_pnum] = -1;
        } // 도착 ~ 현재시간까지는 대기시간이므로 -1

        for(int j = cur_time ; j < cur_time + cur_burst ; j++) {
            if(i == 0) printf("execute %d\n", j);
            visit[j][real_pnum] = 1;
        }  // 현재시각 ~ 현재시각 + 실행시간은 실행시간이므로 1
        cur_time += cur_burst;
    }


    // 간트 차트 생성
    makeline();
    printf("            X : not arrive,  W : waiting,  O : executing\n            Time\n");
    printf("            ");
    int chk = 0;
    for(int k = 0 ; k <= 100 ; k++) {
        if(k % 10 == 0) printf("%d", chk++);
        else printf(" ");
    }
    printf("\n");

    printf("            ");
    for(int k = 0 ; k <= 100 ; k++) {
        printf("%d", k%10);
    }
    printf("\n");
    for(int i = 0 ; i < cnt ; i++) {
        printf("PROCESS %d : ", i + 1);
        for(int j = 0 ; j <= 100 ; j++) {
            if(visit[j][i + 1] == -1) {
                printf("W");
            }
            else if(visit[j][i + 1] == 1) {
                printf("O");
            }
            else if(visit[j][i + 1] == 0) {
                printf("X");
            }
        }
        printf("\n");
    }

    printf("Average wait time : %.2lf\n",(double)tot_wait/cnt);
    makeline();
    
    fclose(fi);
    return 0;
}