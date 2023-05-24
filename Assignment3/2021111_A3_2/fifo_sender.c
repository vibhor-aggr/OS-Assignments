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

    unlink("/tmp/myfifow");
    /*Create a fifo*/
    int status = mkfifo("/tmp/myfifow", 0666);
    if (status == -1) {
        perror("Sender: Error in mkfifo");
        return EXIT_FAILURE;
    }
    
    char** sender=random_str(STR_SENT_LEN);
    int fdw; /*Descriptor for FIFO*/
    fdw = open("/tmp/myfifow", O_WRONLY);    
    if (fdw == -1) {
        perror("Sender: Error in open write fifo");
        free_arr(sender);
        return EXIT_FAILURE;
    }
    fsync(fdw);

    int fdr = -1; /*Descriptor for FIFO*/
    int ack=0;
    int i=0;
    struct timespec tp1, tp2;
    clock_gettime(USE_CLOCK, &tp1);
    while (i < 50) {
        int write_status = write(fdw, sender[i], sizeof(char)*(strlen(sender[i])+1));
        
        if (write_status == -1) {
            perror("Sender: Error in string write");
            unlink("/tmp/myfifow");
            free_arr(sender);
            return EXIT_FAILURE;
        }

        printf("Sender: String sent '%s' by sender\n", sender[i]);
        write_status = write(fdw, &i, sizeof(int));
        fsync(fdw);

        if (write_status == -1) {
            perror("Sender: Error in id write\n");
            unlink("/tmp/myfifow");
            free_arr(sender);
            return EXIT_FAILURE;
        }
        
        printf("Sender: Id sent %d\n", i);
        i++;
        if (i%5 != 0) continue;

        int read_status=0;
        if (fdr == -1) {
            fdr = open("/tmp/myfifor", O_RDONLY);
    
            if (fdr == -1) {
                perror("Sender: Error in open read fifo");
                unlink("/tmp/myfifow");
                free_arr(sender);
                return EXIT_FAILURE;
            }
        }

        while(read_status!=sizeof(int))
        {
            read_status = read(fdr, &ack, sizeof(int));

            if (read_status == -1)
            {
                perror("Sender: Error in acknowledgement read\n");
                unlink("/tmp/myfifow");
                free_arr(sender);
                return EXIT_FAILURE;
            }

            printf("Sender: Acknowledgement recieved by sender %d\n", ack);
            
            if(read_status == sizeof(int) && ack != i-1){
                printf("Sender: Incorrect acknowledgement received");
                unlink("/tmp/myfifow");
                free_arr(sender);
                return EXIT_FAILURE;
            }
        }
    }
    clock_gettime(USE_CLOCK, &tp2);
    double ttime = (double)tp2.tv_sec+1.0e-9*tp2.tv_nsec-(double)tp1.tv_sec-1.0e-9*tp1.tv_nsec;
    printf("\n********************************************\n");
    printf("FIFO runtime: %lf\n", ttime);
    printf("********************************************\n");
    close(fdw);
    close(fdr);

    unlink("/tmp/myfifow");
    free_arr(sender);
    return EXIT_SUCCESS;
}
