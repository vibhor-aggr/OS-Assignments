#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h> 
#include <limits.h>

#define STR_SENT_LEN 128
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

int main(int argc, char *argv[])
{
    printf("Sender process with pid %d\n", getpid());
    int lfd = 0, fd = 0;
    struct sockaddr_in soc_addr; 

    lfd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&soc_addr, '0', sizeof(soc_addr));

    soc_addr.sin_family = AF_INET;
    soc_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    soc_addr.sin_port = htons(5000); 

    bind(lfd, (struct sockaddr*)&soc_addr, sizeof(soc_addr)); 

    listen(lfd, 1); 

    char** sender=random_str(STR_SENT_LEN);

    fd = accept(lfd, (struct sockaddr*)NULL, NULL);
     
	int i=0;
	int ack=0;
    struct timespec tp1, tp2;
    clock_gettime(USE_CLOCK, &tp1);
    while (i < 50) {
        int write_status = write(fd, sender[i], sizeof(char)*(strlen(sender[i])+1));
        
        if (write_status == -1) {
            perror("Sender: Error in string write");
            free_arr(sender);
            close(fd);
            return EXIT_FAILURE;
        }

        printf("Sender: String sent '%s' by sender\n", sender[i]);
        write_status = write(fd, &i, sizeof(int));

        if (write_status == -1) {
            perror("Sender: Error in id write\n");
            free_arr(sender);
            close(fd);
            return EXIT_FAILURE;
        }
        
        printf("Sender: Id sent %d\n", i);
        i++;
        if (i%5 != 0) continue;

        int read_status=0;

        while(read_status!=sizeof(int))
        {
            read_status = read(fd, &ack, sizeof(int));

            if (read_status == -1)
            {
                perror("Sender: Error in acknowledgement read\n");
                free_arr(sender);
                close(fd);
                return EXIT_FAILURE;
            }

            printf("Sender: Acknowledgement recieved by sender %d\n", ack);
            
            if(read_status == sizeof(int) && ack != i-1){
                printf("Sender: Incorrect acknowledgement received");
                free_arr(sender);
                close(fd);
                return EXIT_FAILURE;
            }
        }
    }
    clock_gettime(USE_CLOCK, &tp2);
    double ttime = (double)tp2.tv_sec+1.0e-9*tp2.tv_nsec-(double)tp1.tv_sec-1.0e-9*tp1.tv_nsec;
    printf("\n********************************************\n");
    printf("Socket runtime: %lf\n", ttime);
    printf("********************************************\n");
    free_arr(sender);
    close(fd);
    return EXIT_SUCCESS;
}
