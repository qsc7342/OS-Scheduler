# OS-Scheduler
[COM2005] 운영체제론 기말 프로젝트

## Process.h	
 - 프로세스 구조체를 정의합니다.

## Queue.h
- Ready Queue 입니다.
- 현재는 FCFS를 위한 큐만 구현되어있습니다.
- SJF 등의 구현을 위해 Heap 형태로 추가로 구현을 할 지 고민하고 있습니다.

## Heap.h
- 우선순위를 부여하기 위해 힙을 따로 구현하였습니다.
- 파일별로 어떤 것을 우선순위로 할 지 나누었습니다.

## Progress
  - 2021.05.13 : 프로세스 임의 생성 코드 작성
  - 2021.05.15 : FCFS 뼈대 코드 작성
  - 2021.05.16 : 프로세스의 진행 상황 출력 구현
  - 2021.05.17 : Queue를 사용한 구현으로 FCFS 변경 및 간트 차트 시각화 구현
  - 2021.05.18 : SJF, SRTF 구현 및 프로세스 평균 반환 시간, 대기 시간 계산 구현
  - 2021.05.19 : RM, EDF 구현
