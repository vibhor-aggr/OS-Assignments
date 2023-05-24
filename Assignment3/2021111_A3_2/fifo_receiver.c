#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>

#define STR_RECEIVED_LEN 128

int main()
{
    printf("Receiver process with pid %d\n", getpid());

    /*Create a fifo*/
    //mkfifo("/tmp/myfifo", 0666);
        
    /*Get Input from user*/
    char received[STR_RECEIVED_LEN+1];
    int strIndex=0;
    
    unlink("/tmp/myfifor");
    int fd = open("/tmp/myfifow", O_RDONLY);
    while(fd==-1) {
        printf("Waiting for sender to create /tmp/myfifo\n");
        sleep(1);
        fd = open("/tmp/myfifow", O_RDONLY);
    }
    
    if (fd == -1)
    {
        perror("Receiver: Error in string fifo open");
        return EXIT_FAILURE;
    }

    /*Create a fifo*/
    int status = mkfifo("/tmp/myfifor", 0666);
    if (status == -1)
    {
        perror("Receiver: Error in ack mkfifo");
        return EXIT_FAILURE;
    }
        
    int fd1 ; /*Descriptor for FIFO*/
    fd1 = open("/tmp/myfifor", O_WRONLY);
    
    if (fd1 == -1)
    {
        perror("Receiver: Error in ack open");
        return EXIT_FAILURE;
    }
    while(strIndex<49){
      int read_status;
      while(read_status!=sizeof(char)*(STR_RECEIVED_LEN+1)){
        read_status = read(fd, &received, sizeof(char)*(STR_RECEIVED_LEN+1));

        if (read_status == -1)
        {
          perror("Receiver: Error in write");
          unlink("/tmp/myfifor");
          return EXIT_FAILURE;
        }
      }

      printf("Receiver: String recieved '%s' by receiver\n", received);
      read_status=0;
      while(read_status!=sizeof(int)){
        read_status = read(fd, &strIndex, sizeof(int));

        if (read_status == -1)
        {
          perror("Receiver: Error in write");
          unlink("/tmp/myfifor");
          return EXIT_FAILURE;
        }
      }
      printf("Receiver: Id received %d\n", strIndex);
    
      if((strIndex+1)%5==0)
      {
        int write_status = write(fd1, &strIndex, sizeof(int));

        if (write_status == -1)
        {
          perror("Receiver: Error in write\n");
          unlink("/tmp/myfifor");
          return EXIT_FAILURE;
        }
      }
    }
    close(fd1);
    close(fd);
    unlink("/tmp/myfifor");
    return EXIT_SUCCESS;
}
