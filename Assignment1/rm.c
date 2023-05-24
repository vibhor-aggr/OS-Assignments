#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "option.h"

#define BUFSIZE 10*1024

void print_rm_usage()
{
	printf("Usage: rm [-f|--force] [-v|--verbose] FILE...\n");
	return;
}

int rm(int argc, char* argv[]);

#ifdef STANDALONE
int main(int argc, char* argv[]){
	int sOptc = 0;
	char** sOptv = (char **)malloc(sizeof(char*)*ARGCOUNT);
	splitOpts(argc, argv, &sOptc, sOptv, 1);
	int ret = rm(sOptc, sOptv);
	for(int i=0;i<sOptc;i++){
		free(sOptv[i]);
	}
	free(sOptv);
	return ret;
}
#endif

int rm(int argc, char* argv[]){
	char* optStr[]={"-f","--force","-v","--verbose","--help"};
	bool  optVal[]={false, false, false, false, false};

	if (!searchOpts(argc, argv, 5, optStr, optVal)) {
		print_rm_usage();
		return -1;
	}

	if (optVal[4]) {
		print_rm_usage();
		return 0;
	}

    bool sForce = optVal[0] || optVal[1];
    bool sVerbose = optVal[2] || optVal[3];
	
	int noOptc = 0;
	char** noOptv = (char **)malloc(sizeof(char*)*argc);
	separateOpts(argc, argv, &noOptc, noOptv);
	if(noOptc==1){
		printf("rm: missing operand\n");
		print_rm_usage();
		free(noOptv);
		return -1;
	}	
	for(int i=1;i<noOptc;i++){
   		bool isDir = false;
		struct stat sb;
		int ret = stat(noOptv[i], &sb);
		if (!ret && S_ISDIR(sb.st_mode)) {
			isDir = true;
		}
		if(isDir){
			printf("rm: cannot remove '%s': Is a directory\n",noOptv[i]);
			continue;
		}
		int retval=remove(noOptv[i]);
		if(!sForce && retval==-1){
			printf("rm: cannot remove '%s': %s\n",noOptv[i],strerror(errno));
		}
		if(sVerbose && retval==0){
			printf("removed '%s'\n",noOptv[i]);
		}
	}
	free(noOptv);
	return 0;
}				
