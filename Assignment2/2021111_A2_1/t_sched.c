#include <stdio.h>
#include <limits.h>
#include <sched.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/resource.h>

#define LOOP_MAX UINT_MAX
//#define USE_CLOCK CLOCK_THREAD_CPUTIME_ID      
#define USE_CLOCK CLOCK_REALTIME      

double Atime[10];
double Btime[10];
double Ctime[10];
int currA = 0;
int currB = 0;
int currC = 0;

void countA(void* inc){
  clockid_t clockA=USE_CLOCK;
  struct timespec tpA1, tpA2;

  struct sched_param paramA;
  paramA.sched_priority=0;
  int policyA=SCHED_OTHER;
  pthread_setschedparam(pthread_self(), policyA, &paramA);

  struct rlimit rlim;
  //getrlimit(RLIMIT_NICE,&rlim);
  //printf("%ld %ld\n", rlim.rlim_cur, rlim.rlim_max);
  rlim.rlim_cur=40;
  rlim.rlim_max=40;
  setrlimit(RLIMIT_NICE,&rlim);

  //getrlimit(RLIMIT_NICE,&rlim);
  //printf("%ld %ld\n", rlim.rlim_cur, rlim.rlim_max);
  errno = 0;
  int val=nice(*((int*)inc));
  if(errno != 0){
    printf("A error: nice failed: %s",strerror(errno));
  }
  //printf("A nice : %d\n",val);

  clock_gettime(clockA, &tpA1);
  for(unsigned i=0;i<LOOP_MAX;i++);
  clock_gettime(clockA, &tpA2);

  double t = (double)tpA2.tv_sec+1.0e-9*tpA2.tv_nsec-(double)tpA1.tv_sec-1.0e-9*tpA1.tv_nsec;
  Atime[currA++] = t;
  printf("A (Nice: %d): %lf\n", val, t);

  return;
}

void countB(void* prio){
  clockid_t clockB=USE_CLOCK;
  struct timespec tpB1, tpB2;

  struct sched_param paramB;
  paramB.sched_priority=*((int*)prio);
  //printf("B argprio: %d\n", paramB.sched_priority);
  int policyB=SCHED_RR;
  int ret=pthread_setschedparam(pthread_self(), policyB, &paramB);
  if(ret!=0){
    printf("B error: pthread_setschedparam failed: %s",strerror(errno));
  }

  /*struct sched_param paramB1;
  int policyB1;
  pthread_getschedparam(pthread_self(), &policyB1, &paramB1);
  printf("B prio: %d\n", paramB1.sched_priority);*/

  clock_gettime(clockB, &tpB1);
  for(unsigned i=0;i<LOOP_MAX;i++);
  clock_gettime(clockB, &tpB2);

  double t = (double)tpB2.tv_sec+1.0e-9*tpB2.tv_nsec-(double)tpB1.tv_sec-1.0e-9*tpB1.tv_nsec;
  Btime[currB++] = t;
  printf("B (Priority: %d): %lf\n", paramB.sched_priority, t);

  return;
}

void countC(void* prio){
  clockid_t clockC=USE_CLOCK;
  struct timespec tpC1, tpC2;

  struct sched_param paramC;
  paramC.sched_priority=*((int*)prio);
  //printf("C argprio: %d\n", paramC.sched_priority);
  int policyC=SCHED_FIFO;
  int ret=pthread_setschedparam(pthread_self(), policyC, &paramC);
  if(ret!=0){
    printf("C error: pthread_setschedparam failed: %s",strerror(errno));
  }
  /*struct sched_param paramC1;
  int policyC1;
  pthread_getschedparam(pthread_self(), &policyC1, &paramC1);
  printf("C prio: %d\n", paramC1.sched_priority);*/

  clock_gettime(clockC, &tpC1);
  for(unsigned i=0;i<LOOP_MAX;i++);
  clock_gettime(clockC, &tpC2);

  double t = (double)tpC2.tv_sec+1.0e-9*tpC2.tv_nsec-(double)tpC1.tv_sec-1.0e-9*tpC1.tv_nsec;
  Ctime[currC++] = t;
  printf("C (Priority: %d): %lf\n", paramC.sched_priority, t);

  return;
}

int main(int argc, char* argv[]){
  pthread_t Thr_A,Thr_B,Thr_C;

  if (argc < 2) {
	  printf("Usage: %s <python plot file>\n", argv[0]);
		return -1;
  }

  int inc=16;
  for(int i=1;i<100;i+=10){  
    pthread_create(&Thr_A, NULL, (void*)(*countA) , (void*)&inc);
    pthread_create(&Thr_B, NULL, (void*)(*countB) , (void*)&i);
    pthread_create(&Thr_C, NULL, (void*)(*countC) , (void*)&i);
    pthread_join(Thr_A, NULL);
    pthread_join(Thr_B, NULL);
    pthread_join(Thr_C, NULL);
    printf("\n");
    inc-=4;
    //printf("%d\n",inc);
  }

  // generating python file for plotting
  FILE *fp = fopen(argv[1], "w");
  fprintf(fp, "import matplotlib.pyplot as timeplt\n\n");
  //fprintf(fp, "# x-coordinates of left sides of bars\n"); 
  int offset = 2;
  fprintf(fp, "left1 = [");
  int first = 1;
  for (int i = 1, cnt = 0; cnt < 10; cnt++) {
    if (first)
      fprintf(fp, "%d", i);
    else
      fprintf(fp, ", %d", i);
    first = 0;
    i += 3 + offset;
  }
  fprintf(fp, "]\n\n");

  fprintf(fp, "left2 = [");
  first = 1;
  for (int i = 1, cnt = 0; cnt < 10; cnt++) {
    if (first)
      fprintf(fp, "%d", i+1);
    else
      fprintf(fp, ", %d", i+1);
    first = 0;
    i += 3 + offset;
  }
  fprintf(fp, "]\n\n");

  fprintf(fp, "left3 = [");
  first = 1;
  for (int i = 1, cnt = 0; cnt < 10; cnt++) {
    if (first)
      fprintf(fp, "%d", i+2);
    else
      fprintf(fp, ", %d", i+2);
    first = 0;
    i += 3 + offset;
  }
  fprintf(fp, "]\n\n");

  //fprintf(fp, "# heights of bars\n");
  fprintf(fp, "time1 = [");
  first = 1;
  for (int i = 0; i < 10; i++) {
    if (first)
      fprintf(fp, "%lf", Atime[i]);
    else
      fprintf(fp, ", %lf", Atime[i]);
    first = 0;
  }
  fprintf(fp, "]\n\n");

  fprintf(fp, "time2 = [");
  first = 1;
  for (int i = 0; i < 10; i++) {
    if (first)
      fprintf(fp, "%lf", Btime[i]);
    else
      fprintf(fp, ", %lf", Btime[i]);
    first = 0;
  }
  fprintf(fp, "]\n\n");

  fprintf(fp, "time3 = [");
  first = 1;
  for (int i = 0; i < 10; i++) {
    if (first)
      fprintf(fp, "%lf", Ctime[i]);
    else
      fprintf(fp, ", %lf", Ctime[i]);
    first = 0;
  }
  fprintf(fp, "]\n\n");
#if 0
  //fprintf(fp, "# labels for bars\n");
  fprintf(fp, "tick_label1 = [");
  int ap = 16, bp = 1, cp = 1;
  first = 1;
  for (int i = 0; i < 10; i++) {
    if (first)
      fprintf(fp, "%d", ap);
    else
      fprintf(fp, ", %d", ap);
    first = 0;
    ap -= 4;
  }
  fprintf(fp, "]\n\n");

  fprintf(fp, "tick_label2 = [");
  first = 1;
  for (int i = 0; i < 10; i++) {
    if (first)
      fprintf(fp, "%d", bp);
    else
      fprintf(fp, ", %d", bp);
    first = 0;
    bp += 10;
  }
  fprintf(fp, "]\n\n");

  fprintf(fp, "tick_label3 = [");
  first = 1;
  for (int i = 0; i < 10; i++) {
    if (first)
      fprintf(fp, "%d", cp);
    else
      fprintf(fp, ", %d", cp);
    first = 0;
    cp += 10;
  }
  fprintf(fp, "]\n\n");
#endif

  fprintf(fp, "tick_label = [");
  int ap = 16, bp = 1, cp = 1;
  first = 1;
  for (int i = 0; i < 10; i++) {
    if (first)
      fprintf(fp, "\"%d  %d  %d\"", ap, bp, cp);
    else
      fprintf(fp, ", \"%d  %d  %d\"", ap, bp, cp);
    first = 0;
    ap -= 4;
    bp += 10;
    cp += 10;
  }
  fprintf(fp, "]\n\n");

  //fprintf(fp, "# plotting a bar chart\ntimeplt.bar(left, time, tick_label = tick_label, width = 1, color = ['red', 'green', 'blue'])\n\n# naming the x-axis\ntimeplt.xlabel('Scheduling priority')\n\n# naming the y-axis\ntimeplt.ylabel('Time')\n\n# plot title\ntimeplt.title('My thread timing chart!')\n\n# function to show the plot\ntimeplt.show()\n");
  //fprintf(fp, "timeplt.bar(left, time, tick_label = tick_label, width = 1, color = ['red', 'green', 'blue'])\n\ntimeplt.xlabel('Scheduling priority')\n\ntimeplt.ylabel('Time')\n\ntimeplt.title('My thread timing chart!')\n\ntimeplt.show()\n");
  //fprintf(fp, "timeplt.bar(left1, time1, tick_label = tick_label1, width = 1, color = ['red'], label = ['SCHED_OTHER'])\n\ntimeplt.bar(left2, time2, tick_label = tick_label2, width = 1, color = ['green'], label = ['SCHED_RR'])\n\ntimeplt.bar(left3, time3, tick_label = tick_label3, width = 1, color = ['blue'], label = ['SCHED_FIFO'])\n\ntimeplt.xlabel('Scheduling priority')\n\ntimeplt.ylabel('Time')\n\ntimeplt.title('My thread timing chart!')\n\ntimeplt.legend()\n\ntimeplt.show()\n");
  fprintf(fp, "timeplt.bar(left1, time1, width = 1, color = ['red'], label = ['SCHED_OTHER'])\n\ntimeplt.bar(left2, time2, tick_label = tick_label, width = 1, color = ['green'], label = ['SCHED_RR'])\n\ntimeplt.bar(left3, time3, width = 1, color = ['blue'], label = ['SCHED_FIFO'])\n\ntimeplt.xlabel('Scheduling priority')\n\ntimeplt.ylabel('Time')\n\ntimeplt.title('My thread timing chart!')\n\ntimeplt.legend()\n\ntimeplt.show()\n");
  
  fclose(fp);

  return 0;
}
