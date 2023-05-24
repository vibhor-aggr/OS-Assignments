#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <limits.h>

#define STR_SENT_LEN 128
#define SHM_SIZE 5*(STR_SENT_LEN+1) + 7*sizeof(int)
#define USE_CLOCK  CLOCK_PROCESS_CPUTIME_ID

char** random_str(int size){	

    time_t seconds = time(NULL);
    srandom((unsigned int)seconds);

    char** ret=(char**)malloc(50*sizeof(char*));
    for (int i = 0; i < 50; i++) {
		ret[i]=(char*)malloc((size+1)*sizeof(char));
        for (int j=0; j < size; j++){
            ret[i][j]=(random()/(float)INT_MAX)*93+33;
        }
        ret[i][size]='\0';
    }
    return ret;
}

void free_arr(char** arr){
	for(int i=0; i<50; i++){
		free(arr[i]);
	}
	free(arr);
    return;
}

int main()
{
    printf("Sender process with pid %d\n", getpid());

	int fd = shm_open("/shared_memory", O_CREAT | O_EXCL | O_RDWR, 0600);
    if (fd < 0) {
        perror("Sender: shm_open failed");
        return EXIT_FAILURE;
    }

    /*Resize the shm*/
    ftruncate(fd, SHM_SIZE );

    void* ret = mmap(0, SHM_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
    if (ret == MAP_FAILED) {
        perror("Sender: mmap failed");
        return EXIT_FAILURE;
    }
    char* data = (char*) ret;
    char** sender=random_str(STR_SENT_LEN);
 
    int* sendLoc = (int *)data;
    int* recLoc = (int *)&data[4];
    *sendLoc = -1;
    *recLoc = -1;

	int i=0;
    struct timespec tp1, tp2;
    clock_gettime(USE_CLOCK, &tp1);
    while (i < 50) {//check *data<49
		int pos=i%5;
        void* write_status = memcpy(data+8+(STR_SENT_LEN+1+sizeof(int))*pos, sender[i], sizeof(char)*(strlen(sender[i])+1));
        printf("Sender: String sent '%s' by sender\n", sender[i]);
        write_status = memcpy(data+8+(STR_SENT_LEN+1+sizeof(int))*pos+STR_SENT_LEN+1, &i, sizeof(int));
        printf("Sender: Id sent %d\n", i);
        write_status = memcpy(sendLoc, &i, sizeof(int));
        i++;
        if (i%5 != 0) continue;
        while(*recLoc != *sendLoc);
        printf("Sender: Acknowledgement recieved by sender %d\n", *recLoc);
    }
	while(*recLoc<49);
    clock_gettime(USE_CLOCK, &tp2);
    double ttime = (double)tp2.tv_sec+1.0e-9*tp2.tv_nsec-(double)tp1.tv_sec-1.0e-9*tp1.tv_nsec;
    printf("\n********************************************\n");
    printf("Shared Memory runtime: %lf\n", ttime);
    printf("********************************************\n");
    free_arr(sender);
    int value = munmap(data, SHM_SIZE);
    if (value == -1) {
      perror("Sender: munmap failed");
      return EXIT_FAILURE;
    }
    close(fd);
    shm_unlink("/shared_memory");
    return EXIT_SUCCESS;
}
