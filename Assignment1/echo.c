#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "option.h"

#define BUFSIZE 10*1024

int echo(int argc, char* argv[]);

#ifdef STANDALONE
int main(int argc, char* argv[]){
	int sOptc = 0;
	char** sOptv = (char **)malloc(sizeof(char*)*ARGCOUNT);
	splitOpts(argc, argv, &sOptc, sOptv, 1);
	int ret = echo(sOptc, sOptv);
	for(int i=0;i<sOptc;i++){
		free(sOptv[i]);
	}
	free(sOptv);
	return ret;
}
#endif

int echo(int argc, char* argv[]){
	char* optStr[]={"-n","--help"};
	bool  optVal[]={false, false};

	if (!searchOpts(argc, argv, 2, optStr, optVal)) {
    	printf("Usage: echo [-n] [STRING]...\n");
		return -1;
	}

    bool sLine = optVal[0];
    bool sHelp = optVal[1];
	if(sHelp){	
    	printf("Usage: echo [-n] [STRING]...\n");
		return 0;
	}

	int noOptc = 0;
	char** noOptv = (char **)malloc(sizeof(char*)*argc);
	separateOpts(argc, argv, &noOptc, noOptv);

	for(int i=1;i<noOptc;i++){
		char s[BUFSIZE];
		int flag=0;
		int k=0;
		bool insideSQ=false;
		for(int j=0;j<strlen(noOptv[i]);j++){
			if(!isalnum(noOptv[i][j])){
				if(k!=0){
					s[k] = '\0';
					char* dir=getenv(s);
					if(dir!=NULL)
						printf("%s",dir);
					k=0;
					flag=0;
				}
				if(noOptv[i][j]=='\"'){
					continue;
				}
				if(noOptv[i][j]=='\''){
					insideSQ=insideSQ ? false : true;
					continue;
				}
				if(noOptv[i][j]=='$' && !insideSQ){
					flag=1;
				}
				else{ 
					printf("%c",noOptv[i][j]);
				}
			} else if(flag) {
				s[k++]=noOptv[i][j];
			} else {
				printf("%c",noOptv[i][j]);
			}
		}
		if(k!=0){
			s[k] = '\0';
			char* dir=getenv(s);
			if(dir!=NULL)
				printf("%s",dir);
		}
		if(i!=noOptc-1)
			printf(" ");
	}
	if(!sLine)	
		printf("\n");
	free(noOptv);
	return 0;
}
