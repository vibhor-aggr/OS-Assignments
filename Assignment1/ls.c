#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include "option.h"

#define BUFSIZE 10*1024
#define FILECOUNT 10*1024

void print_files(int fcnt, char* fileArr[], bool lfile);

void print_ls_usage()
{
	printf("Usage: ls [-a|--all] [-A|--almost-all] [FILE]...\n");
	return;
}

int ls(int argc, char* argv[]);

#ifdef STANDALONE
int main(int argc, char* argv[]){
	int sOptc = 0;
	char** sOptv = (char **)malloc(sizeof(char*)*ARGCOUNT);
	splitOpts(argc, argv, &sOptc, sOptv, 1);
	int ret = ls(sOptc, sOptv);
	for(int i=0;i<sOptc;i++){
		free(sOptv[i]);
	}
	free(sOptv);
	return ret;
}
#endif

int ls(int argc, char* argv[]){
	char* optStr[]={"-a","--all","-A","--almost-all","--help"};
	bool  optVal[]={false, false, false, false, false};

	if (!searchOpts(argc, argv, 5, optStr, optVal)) {
		print_ls_usage();
		return -1;
	}

	if (optVal[4]) {
		print_ls_usage();
		return 0;
	}

    bool sAll = optVal[0] || optVal[1];
    bool sNall = optVal[2] || optVal[3];

	int noOptc = 0;
	char** noOptv = (char **)malloc(sizeof(char*)*argc);
	separateOpts(argc, argv, &noOptc, noOptv);

	if(noOptc==1){
		DIR* dir=opendir(".");
        if(dir==NULL){
            printf("ls: cannot access '.': %s\n",strerror(errno));
			free(noOptv);
			print_ls_usage();
			return -1;
        } // see printf
        else{
            struct dirent* direc;
			errno=0;
			bool lfile = false;
			int  fcnt = 0;
			char* fileArr[FILECOUNT];
            while((direc=readdir(dir))!=NULL){
                if(!sAll && !sNall && direc->d_name[0]=='.' || (sNall && (strcmp(direc->d_name,".")==0 || strcmp(direc->d_name,"..")==0)))
                    continue;
				fileArr[fcnt++] = strdup(direc->d_name);
				if(strlen(direc->d_name)>=20){
					lfile = true;
				}
			}
			if(errno!=0){
				printf("ls: %s\n",strerror(errno));
				free(noOptv);
				print_ls_usage();
				return -1;
			}
			closedir(dir);//check -1 for error
			
			print_files(fcnt, fileArr, lfile);
			//printf("\n");
		}
	} else{
		int  sfcnt = 0;
		char* sfileArr[FILECOUNT];
		bool slfile = false;
		for(int i=1;i<noOptc;i++){
			DIR* dir=opendir(noOptv[i]);
			if(dir==NULL){
				if (errno != ENOTDIR) {
					printf("ls: cannot access '%s': %s\n",noOptv[i],strerror(errno));
				} else {
					sfileArr[sfcnt++] = strdup(noOptv[i]);
					if(strlen(noOptv[i])>=20){
						slfile = true;
					}
				}
			} // see printf
			else{
				struct dirent* direc;
				errno=0;
				bool lfile = false;
				int  fcnt = 0;
				char* fileArr[FILECOUNT];
				while((direc=readdir(dir))!=NULL){
					if(!sAll && !sNall && direc->d_name[0]=='.' || (sNall && (strcmp(direc->d_name,".")==0 || strcmp(direc->d_name,"..")==0)))
						continue;
					fileArr[fcnt++] = strdup(direc->d_name);
					if(strlen(direc->d_name)>=20){
						lfile = true;
					}
				}
				if(errno!=0){
					printf("ls: %s",strerror(errno));
				}//for error avoid closedir
				closedir(dir);

				printf("%s:\n",noOptv[i]);
				print_files(fcnt, fileArr, lfile);
			}
		}
		print_files(sfcnt, sfileArr, true);
	}
	free(noOptv);
	return 0;
}

void print_files(int fcnt, char* fileArr[], bool lfile)
{
	int c=0;
	int j=0;
	for (j = 0; j < fcnt; j++) {
		if (lfile) {
			printf("%s\n",fileArr[j]);
		} else {
			c++;
			if(c==5){
				c=1;
				printf("\n");
			}
			printf("%20s",fileArr[j]);
		}
		free(fileArr[j]);
	}
	if (lfile) {
		printf("\n");
	} else {
		printf("\n\n");
	}
}
