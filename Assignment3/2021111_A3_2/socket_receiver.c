#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h> 

#define STR_RECEIVED_LEN 128

int main()
{
    printf("Receiver process with pid %d\n", getpid());

    int fd = 0;
    struct sockaddr_in soc_addr; 

    char received[STR_RECEIVED_LEN+1];
    int strIndex=0;
    if((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Receiver: Could not create socket");
        return EXIT_FAILURE;
    } 

    memset(&soc_addr, '0', sizeof(soc_addr)); 

    soc_addr.sin_family = AF_INET;
    soc_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    soc_addr.sin_port = htons(5000); 

    int cvalue = connect(fd, (struct sockaddr *)&soc_addr, sizeof(soc_addr));
    while( cvalue < 0)
    {
       printf("Waiting for sender to create socket connection\n");
       sleep(1);
       cvalue = connect(fd, (struct sockaddr *)&soc_addr, sizeof(soc_addr));
    } 

    while(strIndex<49){
      int read_status;
      while(read_status!=sizeof(char)*(STR_RECEIVED_LEN+1)){
        read_status = read(fd, &received, sizeof(char)*(STR_RECEIVED_LEN+1));

        if (read_status == -1)
        {
          perror("Receiver: Error in write");
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
          return EXIT_FAILURE;
        }
      }
      printf("Receiver: Id received %d\n", strIndex);
    
      if((strIndex+1)%5==0)
      {
        int write_status = write(fd, &strIndex, sizeof(int));

        if (write_status == -1)
        {
          perror("Receiver: Error in write\n");
          return EXIT_FAILURE;
        }
      }
    }
	
    //close(fd);
    return EXIT_SUCCESS;
}
