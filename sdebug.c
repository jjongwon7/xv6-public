#include "types.h"
#include "stat.h"
#include "user.h"

#define PNUM 5  // Process 개수
#define PRINT_CYCLE 10000000   // Process가 정보를 출력하는 주기
#define TOTAL_COUNTER 500000000 // Process가 종료할 때 counter 값

void sdebug_func(void)
{
    int n, pid; // for문에 활용될 변수 n, fork를 위한 pid 변수 선언 
    
    printf(1, "start sdebug command\n");
    
    // 프로세스 개수만큼 for문을 돌면서 fork 수행
    for(n=0; n<PNUM; n++) {
      pid = fork();
      // pid가 음수인 경우 처리
      if(pid < 0){
        printf(1, "fork failed\n");
        exit();
      }
      // 자식 프로세스 작업 내용
      if(pid == 0){
        int start_ticks = uptime(); // 자식 프로세스 생성 시 uptime 값 저장
        int end_ticks;
        int weight = weightset(n+1); // 자식 프로세스 생성 시 weight 값 부여 (1부터 증가시킴)
        int first = 1; // 프로세스 정보를 1회만 출력하기 위해 사용되는 변수
        long long counter = 0; // TOTAL_COUNTER와 비교하기 위한 변수
        long long print_counter = PRINT_CYCLE; // PRINT_CYCLE과 비교하기 위한 변수

        // 잘못된 weight 값이 들어온 경우 해당 프로세스는 종료시킴
        if(weight <= -1)
        {
          printf(1, "***** WRONG WEIGHT *****\n");
          exit();
        }

        // TOTAL_COUNTER 만큼 while문 반복
        while(counter <= TOTAL_COUNTER)
        {
          counter++; // 반복문을 돌 때마다 counter 값을 하나씩 증가시킴
          print_counter--; // 반복문을 돌 때마다 print_counter 값을 하나씩 감소시킴

          // PRINT_CYCLE만큼 while문이 수행된 경우
          if(print_counter == 0)
          {
            // 정보는 1번만 출력
            if (first)
            {
              end_ticks = uptime(); // 출력하기 전 end_ticks에 uptime 값 저장
              printf(1, "PID: %d, WEIGHT: %d, ", getpid(), weight);
              printf(1, "TIMES: %d ms\n", (end_ticks-start_ticks) * 10); // 1tick == 10ms이므로 10을 곱하여 출력
              first = 0; // 출력한 경우 first를 0으로 바꿔 그 뒤로는 정보 출력하지 않음
            }
            print_counter = PRINT_CYCLE; // print_counter 변수에 다시 PRINT_CYCLE을 대입
          }
        }
        // while문이 종료된 경우 terminate 정보를 출력하고 exit
        printf(1, "PID: %d terminated\n", getpid());
        exit();
      }
    }
  
  // for문을 돌며 생성한 자식프로세스를 wait로 정리
  for(; n > 0; n--){
    if(wait() < 0){ // wait 에러처리
      printf(1, "wait stopped early\n");
      exit();
    }
  }
  // 생성한 프로세스가 다 종료되지 않은 경우 에러 처리
  if(wait() != -1){
    printf(1, "wait error\n");
    exit();
  }

  printf(1, "end of sdebug command\n");
  return ;
}

int main(void)
{
    sdebug_func();
    exit();
}