#include<time.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>
#include "option.h"
//#include <sys/types.h>
//#include <sys/stat.h>
//#include <unistd.h>
//#define BUFSIZE 10*1024

void print_date_usage()
{
	printf("Usage: date [-u|--utc|--universal] [-I|--iso-8601]\n");
	return;
}

int date(int argc, char* argv[]);

#ifdef STANDALONE
int main(int argc, char* argv[]){
	int sOptc = 0;
	char** sOptv = (char **)malloc(sizeof(char*)*ARGCOUNT);
	splitOpts(argc, argv, &sOptc, sOptv, 1);
	int ret = date(sOptc, sOptv);
	for(int i=0;i<sOptc;i++){
		free(sOptv[i]);
	}
	free(sOptv);
	return ret;
}
#endif

int date(int argc, char* argv[]){
	char* optStr[]={"-u","--utc","--universal","-I","--iso-8601","--help"};
	bool  optVal[]={false, false, false, false, false, false};

	if (!searchOpts(argc, argv, 6, optStr, optVal)) {
		print_date_usage();
		return -1;
	}

	if (optVal[5]) {
		print_date_usage();
		return 0;
	}

    bool utc = optVal[0] || optVal[1] || optVal[2];
    bool iso = optVal[3] || optVal[4];
	
	int noOptc = 0;
	char** noOptv = (char **)malloc(sizeof(char*)*argc);
	separateOpts(argc, argv, &noOptc, noOptv);

	time_t rtime;
  	time (&rtime);
	if(noOptc>1){
		free(noOptv);
		print_date_usage();
		return -1;
	}
	if(argc==1){
  		struct tm* timeinfo;
  		timeinfo = localtime (&rtime);
  		printf("%s", asctime(timeinfo));
  	}
  	//printf("tzname = %s %s\n", tzname[0], tzname[1]);
  	//printf("timezone = %ld\n", timezone);
  	//printf("daylight = %d\n", daylight);

	// C program to get current UTC time
    //time_t tmi;
	if(utc){
    	struct tm* utcTime;
    	//time(&tmi);
    	utcTime = gmtime(&rtime);
    	printf("%s", asctime(utcTime));
  	}
   	// printf("UTC Time: %2d:%02d:%02d\n", (utcTime->tm_hour) % 24, utcTime->tm_min, utcTime->tm_sec);
   	// printf("Time in India: %2d:%02d:%02d\n", (utcTime->tm_hour + 5) % 24, utcTime->tm_min, utcTime->tm_sec);

  	//printf("tzname = %s %s\n", tzname[0], tzname[1]);
  	//printf("timezone = %ld\n", timezone);
  	//printf("daylight = %d\n", daylight);

#if 0
           struct tm {
               int tm_sec;    /* Seconds (0-60) */
               int tm_min;    /* Minutes (0-59) */
               int tm_hour;   /* Hours (0-23) */
               int tm_mday;   /* Day of the month (1-31) */
               int tm_mon;    /* Month (0-11) */
               int tm_year;   /* Year - 1900 */
               int tm_wday;   /* Day of the week (0-6, Sunday = 0) */
               int tm_yday;   /* Day in the year (0-365, 1 Jan = 0) */
               int tm_isdst;  /* Daylight saving time */
           };
#endif
	if(iso){
  		struct tm* isoTime;
  		isoTime = localtime (&rtime);
  		printf("%d-%d-%d\n", 1900+isoTime->tm_year, isoTime->tm_mon+1, isoTime->tm_mday);
	}
  	free(noOptv);
	return 0;
}
