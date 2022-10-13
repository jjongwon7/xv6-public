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
        long start_time = uptime(); // 자식 프로세스 생성 시 uptime 값 저장
        int weight = weightset(n+1); // 자식 프로세스 생성 시 weight 값 부여 (1부터 증가시킴)
        long long counter = 0;

        while(1)
        {
          counter++; // 반복문을 돌 때마다 counter 값을 하나씩 증가시킴
          if(counter == TOTAL_COUNTER) // counter 값이 TOTAL_COUNTER 값과 동일해지면 프로세스 종료
            exit();
          if(counter == PRINT_CYCLE) // counter 값이 PRINT_CYCLE 값과 동일해지면 정보 출력
            printf(1, "PID: %d, WEIGHT: %d, TIMES: %d ms\n", getpid(), weight, (uptime()-start_time)*10);
        }
      }
    }
  
  // for문을 돌며 생성한 자식프로세스를 wait로 정리하고 정리한 프로세스 PID 출력
  for(; n > 0; n--){
    printf(1, "PID: %d terminated\n", wait());
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