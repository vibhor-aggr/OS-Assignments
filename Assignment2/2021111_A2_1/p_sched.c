#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <limits.h>
#include <sched.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/resource.h>
#include <stdlib.h>

//#define USE_CLOCK CLOCK_THREAD_CPUTIME_ID      
//#define USE_CLOCK CLOCK_REALTIME      
#define USE_CLOCK  CLOCK_PROCESS_CPUTIME_ID

int main(){
  char* cdir = getenv("PWD");
  unsigned cdir_len = strlen(cdir);
  char command[cdir_len+10];

  struct timespec tpA1, tpA2;
  struct timespec tpB1, tpB2;
  struct timespec tpC1, tpC2;

  clock_gettime(USE_CLOCK, &tpA1);
  pid_t pid1;
  pid1=fork();
  if(pid1==0){
    int ret = setpriority(PRIO_PROCESS, 0, -20);
    if (ret == -1) {
      printf("Process 1 setpriority failed: %s\n", strerror(errno));
      exit(-1);
    }
    printf("Inside first child process\n");
    sprintf(command, "%s/compile1.bash", cdir);
    execl(command, "compile1.bash", NULL, (char*)0);
    printf("Inside first child process\n");
  }

  //printf("Inside parent process\n");
  clock_gettime(USE_CLOCK, &tpB1);
  pid_t pid2;
  pid2=fork();
  if(pid2==0){
    int ret = setpriority(PRIO_PROCESS, 0, 0);
    if (ret == -1) {
      printf("Process 2 setpriority failed: %s\n", strerror(errno));
      exit(-1);
    }
    printf("Inside second child process\n");
    sprintf(command, "%s/compile2.bash", cdir);
    execl(command, "compile2.bash", NULL, (char*)0);
    printf("Inside second child process\n");
  }

  clock_gettime(USE_CLOCK, &tpC1);
  pid_t pid3;
  pid3=fork();
  if(pid3==0){
    int ret = setpriority(PRIO_PROCESS, 0, 19);
    if (ret == -1) {
      printf("Process 3 setpriority failed: %s\n", strerror(errno));
      exit(-1);
    }
    printf("Inside third child process\n");
    sprintf(command, "%s/compile3.bash", cdir);
    execl(command, "compile3.bash", NULL, (char*)0);
    printf("Inside third child process\n");
  }
  /*
  waitpid(pid1, NULL, 0);
  waitpid(pid2, NULL, 0);
  waitpid(pid3, NULL, 0);
  */
  int childFinished = 0;
  int wstatus;
  double t1 = 0, t2 = 0, t3 = 0;
  while (1) {
    pid_t cpid = waitpid(-1, &wstatus, WNOHANG);
    if (cpid > 0) {
      childFinished++;
      if (cpid == pid1) {
        clock_gettime(USE_CLOCK, &tpA2);
        t1 = (double)tpA2.tv_sec+1.0e-9*tpA2.tv_nsec-(double)tpA1.tv_sec-1.0e-9*tpA1.tv_nsec;
      } else if (cpid == pid2) {
        clock_gettime(USE_CLOCK, &tpB2);
        t2 = (double)tpB2.tv_sec+1.0e-9*tpB2.tv_nsec-(double)tpB1.tv_sec-1.0e-9*tpB1.tv_nsec;
      } else if (cpid == pid3) {
        clock_gettime(USE_CLOCK, &tpC2);
        t3 = (double)tpC2.tv_sec+1.0e-9*tpC2.tv_nsec-(double)tpC1.tv_sec-1.0e-9*tpC1.tv_nsec;
      }
    }
    if (childFinished == 3) { // all childs have finished
      break;
    }
  }
  printf("A (Priority: %d): %lf\n", -20, t1);
  printf("B (Priority: %d): %lf\n", 0, t2);
  printf("C (Priority: %d): %lf\n", 19, t3);
  return 0;
}
