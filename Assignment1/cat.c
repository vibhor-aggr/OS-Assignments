#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "option.h"

#define BUFSIZE 10*1024

int cat(int argc, char* argv[]);

void print_cat_usage()
{
	printf("Usage: cat [-E|--show-ends] [-n|--number] [FILE]...\n");
}

#ifdef STANDALONE
int main(int argc, char* argv[]){
	int sOptc = 0;
	char** sOptv = (char **)malloc(sizeof(char*)*ARGCOUNT);
	splitOpts(argc, argv, &sOptc, sOptv, 1);
	int ret = cat(sOptc, sOptv);
	for(int i=0;i<sOptc;i++){
		free(sOptv[i]);
	}
	free(sOptv);
	return ret;
}
#endif

int cat(int argc, char* argv[]){
	char* optStr[]={"-E","--show-ends","-n","--number","--help"};
	bool  optVal[]={false, false, false, false, false};

	if (!searchOpts(argc, argv, 5, optStr, optVal)) {
		print_cat_usage();
		return -1;
	}

	if (optVal[4]) {
		print_cat_usage();
		return 0;
	}

    bool sEnd = optVal[0] || optVal[1];
    bool sLine = optVal[2] || optVal[3];
	
	int noOptc = 0;
	char** noOptv = (char **)malloc(sizeof(char*)*argc);
	separateOpts(argc, argv, &noOptc, noOptv);

	int c=1;
	if(noOptc==1){
		char* tmp=(char*)malloc(BUFSIZE*sizeof(char));
		while(scanf("%[^\n]s",tmp)!=EOF){
			if(sLine)
				printf("%6d\t",c++);
			printf("%s",tmp);
			if(sEnd)
				printf("$");
			printf("\n");
			scanf("\n");
		}
		free(tmp);
	}
	FILE* ptr;
	int i=1;
	while(i<=noOptc-1){
   		bool isDir = false;
		struct stat sb;
		int ret = stat(noOptv[i], &sb);
		if (!ret && S_ISDIR(sb.st_mode)) {
			isDir = true;
		}
		if(isDir){
			printf("cat: '%s': Is a directory\n",noOptv[i]);
			i++;
			continue;
		}

		ptr=fopen(noOptv[i],"r");
		if(ptr==NULL){
			printf("cat: %s: %s\n",argv[i],strerror(errno));
			i++;
			continue;
		}
		char* line=NULL;
		long unsigned int len=BUFSIZE;
		while(getline(&line,&len,ptr)!=-1){
			if(sLine)
				printf("%6d\t",c++);
			if(sEnd) {
				if (line[strlen(line)-1] == '\n') {
					line[strlen(line)-1] = '\0';
					printf("%s",line);
					printf("$");
					printf("\n");
				} else {
					printf("%s",line);
					printf("$");
				}
			} else {
				printf("%s",line);
			}
			free(line);
			line=NULL;
		}
		fclose(ptr);
		i++;
	}
	free(noOptv);
	return 0;
}
