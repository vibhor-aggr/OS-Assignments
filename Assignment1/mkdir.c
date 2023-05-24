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

void print_mkdir_usage()
{
	printf("Usage: mkdir [-p|--parents] [-v|--verbose] DIRECTORY...\n");
	return;
}

int mymkdir(int argc, char* argv[]);

#ifdef STANDALONE
int main(int argc, char* argv[]){
	int sOptc = 0;
	char** sOptv = (char **)malloc(sizeof(char*)*ARGCOUNT);
	splitOpts(argc, argv, &sOptc, sOptv, 1);
	int ret = mymkdir(sOptc, sOptv);
	for(int i=0;i<sOptc;i++){
		free(sOptv[i]);
	}
	free(sOptv);
	return ret;
}
#endif

int mymkdir(int argc, char* argv[]){
	char* optStr[]={"-p","--parents","-v","--verbose","--help"};
	bool  optVal[]={false, false, false, false, false};

	if (!searchOpts(argc, argv, 5, optStr, optVal)) {
		print_mkdir_usage();
		return -1;
	}

	if (optVal[4]) {
		print_mkdir_usage();
		return 0;
	}

    bool sParent = optVal[0] || optVal[1];
    bool sVerbose = optVal[2] || optVal[3];
	
	int noOptc = 0;
	char** noOptv = (char **)malloc(sizeof(char*)*argc);
	separateOpts(argc, argv, &noOptc, noOptv);
	if(noOptc==1){
		printf("mkdir: missing operand\n");
		print_mkdir_usage();
		free(noOptv);
		return -1;
	}

	for(int i=1;i<noOptc;i++){
		int dir=mkdir(noOptv[i],0777);
		if(dir==-1) {
			if (!sParent){
				printf("mkdir: cannot create directory '%s': %s\n",noOptv[i], strerror(errno));
				free(noOptv);	
				print_mkdir_usage();
				return -1;
			} else {
                bool isDir = false;
				struct stat sb;
				int ret = stat(noOptv[i], &sb);
				if (!ret && S_ISDIR(sb.st_mode)) {
					isDir = true;
				}
                if (errno == EEXIST && !isDir || errno != EEXIST && errno != ENOENT) {
					printf("mkdir: cannot create directory '%s': %s\n",noOptv[i], strerror(errno));
					free(noOptv);
					print_mkdir_usage();
					return -1;
				}
			}
		} else {
			if (sVerbose) {
				printf("mkdir: created directory '%s'\n", noOptv[i]);
			}
			continue;
		}
		char s[BUFSIZE];
		int k=0;
		for(int j=0;j<strlen(noOptv[i]);j++){
			if(noOptv[i][j]=='/'){
				s[k]='\0';
				int dir=mkdir(s,0777);
				if(dir==-1 && errno != EEXIST){
					printf("mkdir: cannot create directory '%s': %s\n",noOptv[i], strerror(errno));
					free(noOptv);
					print_mkdir_usage();
					return -1;
				} else if (dir != -1 && sVerbose) {
					printf("mkdir: created directory '%s'\n", s);
				}
				s[k++]='/';
			}else{
				s[k++]=noOptv[i][j];
			}
		}
		if(s[k-1]!='/'){
			s[k]='\0';
			int dir=mkdir(s,0777);
			if(dir==-1 && errno != EEXIST){
				printf("mkdir: cannot create directory '%s': %s\n",noOptv[i], strerror(errno));
				free(noOptv);
				print_mkdir_usage();
				return -1;
			} else if (dir != -1 && sVerbose) {
				printf("mkdir: created directory '%s'\n", s);
			}
		}
	}
	free(noOptv);
	return 0;
}
