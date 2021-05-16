#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>

#define PERM 0644 // 파일 권한 설정
#define PROCESS 5 // 프로세스 갯수

int getsize(int n) {
    int cnt = 0;
    if(n == 0) return 1;
    while(n > 0) {
        n /= 10;
        cnt++;
    }
    return cnt;
}

void makeline() {
    printf("------------------------------\n");
    return;
}

int main() {
    srand(time(NULL)); // 시드값 적용

    // 프로세스의 제시 양식 '도착시간, 버스트시간, 데드라인'
    FILE* fi = NULL;
    fi = fopen("process.txt", "w+");
    double cpu_util = 0;
    for(int i = 0 ; i < PROCESS ; i++) {
        // 도착시간, 버스트시간, 데드라인
        int arrive = 0;
        int burst = 0;
        int deadline = 0;
        if(!i | i) arrive = 0; // 모든 프로세스 0에 도착으로 가정
        // else arrive = rand() % (i * 10);
        burst = rand() % 10 + 5;
        deadline = burst + rand() % 10 + 40;

        fprintf(fi, "%d %d %d\n",arrive, burst, deadline);
        cpu_util += (double) burst / deadline;
    }
    makeline();
    printf("Process make complete\nCPU Utilization : %.3lf\n", cpu_util);
    makeline();
    fclose(fi);
    return 0;
}