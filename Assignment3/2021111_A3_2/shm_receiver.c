#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

#define STR_RECEIVED_LEN 128
#define SHM_SIZE 5*(STR_RECEIVED_LEN+1) + 7*sizeof(int)

int main()
{
    printf("Receiver process with pid %d\n", getpid());
    int fd = shm_open("/shared_memory", O_RDWR, 0666);
    while(fd==-1) {
        printf("Waiting for sender to create /shared_memory\n");
        fflush(stdout);
        sleep(1);
        fd = shm_open("/shared_memory", O_RDWR, 0666);
    }

    void* ret = mmap(0, SHM_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
    if (ret == MAP_FAILED) {
        perror("Receiver: mmap failed");
        return EXIT_FAILURE;
    }
    char* data = (char*) ret;
 
    int* sendLoc = (int *)data;
    int* recLoc = (int *)&data[4];

    char received[STR_RECEIVED_LEN+1];
    int strIndex=0;

    while(strIndex<49){
	  while(*recLoc<*sendLoc){
		*recLoc+=1;
	    memcpy(received, data+8+(STR_RECEIVED_LEN+1+sizeof(int))*((*recLoc)%5), STR_RECEIVED_LEN+1);	
        printf("Receiver: String recieved '%s' by receiver\n", received);
        strIndex=*((int *)(data+8+(STR_RECEIVED_LEN+1+sizeof(int))*((*recLoc)%5)+STR_RECEIVED_LEN+1));
		printf("Receiver: Id received %d\n", strIndex);
	  }
    }
    int value = munmap(data, SHM_SIZE);
    if (value == -1) {
      perror("Receiver: munmap failed");
      return EXIT_FAILURE;
    }
    close(fd);
    shm_unlink("/shared_memory");
    return EXIT_SUCCESS;
}
