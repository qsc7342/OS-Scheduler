#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#define _MAX_PATH 260

void move_cur(int x, int y)
{
	printf("\033[%dd\033[%dG", y, x);
}
#define ANSI_COLOR_RED		"\x1b[31m"
#define ANSI_COLOR_GREEN 	"\x1b[32m"
#define ANSI_COLOR_YELLOW 	"\x1b[33m"
#define ANSI_COLOR_BLUE 	"\x1b[34m"
#define ANSI_COLOR_MAGENTA 	"\x1b[35m"
#define ANSI_COLOR_CYAN 	"\x1b[36m"
#define ANSI_COLOR_RESET 	"\x1b[0m"

int main() {
    system("clear");
    char *path;
    path = getcwd(NULL, BUFSIZ);
    move_cur(0, 10);
    printf(ANSI_COLOR_RESET);
   // while(1) {
        printf("====================================================\n");
        printf("+                Select Scheduler                  +\n");
        printf("+                                                  +\n");
        printf("+                                                  +\n");
        printf("+        1. FCFS (First Come First Served)         +\n");
        printf("+           2. SJF (Shorest job First)             +\n");
        printf("+      3. SRTF (Shorest Remaining Time First)      +\n");
        printf("+               4. RR (Round Robin)                +\n");
        printf("+         5. EDF (Earliest Deadline First)         +\n");
        printf("+             6. RM (Rate Monotonic)               +\n");
        printf("====================================================\n");
        int n;
        printf("Select Scheduler : ");
        scanf("%d", &n);
        switch(n) {
            case 1:
                system("cd FCFS && ./a.out");
                break;
            case 2:
                system("cd SJF && ./a.out");
                break;
            case 3:
                system("cd SRTF && ./a.out");
                break;
            case 4:
                system("cd RR && ./a.out");
                break;
            case 5:
                system("cd EDF && ./a.out");
                break;
            case 6:
                system("cd RM && ./a.out");
                break;
            default:
                printf("Unavailable Input\n");
                break;
        }
        return 0;
//    }
}