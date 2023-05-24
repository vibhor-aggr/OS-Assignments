#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>
#include "option.h"

#define BUFSIZE 10*1024

extern int cat(int argc, char* argv[]);
extern int ls(int argc, char* argv[]);
extern int rm(int argc, char* argv[]);
extern int mymkdir(int argc, char* argv[]);
extern int rm(int argc, char* argv[]);
extern int date(int argc, char* argv[]);
extern int cd(int argc, char* argv[]);
extern int echo(int argc, char* argv[]);
extern int pwd();

void* sys_call(void* args){
	//printf("system() args: %s\n", (char*)args);
	system((char*)args);
}

void get_cmd_n_args(char* cdir, char* cmdStr, char* command, char* args, bool escapeSpace)
{
	int i=0;
	int j=0;
	int cdir_len=strlen(cdir);
	for(;i<cdir_len;i++) {
		if (cdir[i] == ' ' && escapeSpace) {
			command[j++]='\\';
		}
		command[j++]=cdir[i];
	}
	command[j++]='/';
	command[j++]='m';
	command[j++]='y';
	int k=0;
	bool startSpace=true;
	bool argsSpace=false;
	int l=0;
	while(cmdStr[k]!='\0'){
		if(cmdStr[k]!=' '){
			startSpace=false;
		}
		if(!startSpace){
			if (cmdStr[k]==' ') {
				argsSpace = true;
			}
			if (argsSpace==false) {
				command[j++]=cmdStr[k++];
			} else {
				args[l++]=cmdStr[k++];
			}
		} else{
			k++;
		}
	}
	command[j]='\0';
	args[l]='\0';
	return;
}

void execute_process(char* cdir, char* cmdStr, char* cmdName){
	int cdir_len=strlen(cdir);
	int cmdStr_len=strlen(cmdStr);
	char cmd[2*cdir_len+3];
	char args[cmdStr_len];
	char command[2*cdir_len+3+cmdStr_len];

	get_cmd_n_args(cdir, cmdStr, cmd, args, false);
	sprintf(command, "%s %s", cmd, args);
	//printf("process command = %s\n", command);

	pid_t pid;
	pid=fork();
	if(pid==0){
		//printf("Inside child process\n");
		execl(cmd, cmdName, args, (char*)0);
	}
	else if(pid>0){
		//printf("Inside parent process\n");
		wait(0);
		return;
	}
}

void execute_thread(char* cdir, char* cmdStr){
	int cdir_len=strlen(cdir);
	int cmdStr_len=strlen(cmdStr);
	char cmd[2*cdir_len+3];
	char args[cmdStr_len];
	char command[2*cdir_len+3+cmdStr_len];

	get_cmd_n_args(cdir, cmdStr, cmd, args, true);
	sprintf(command, "%s %s", cmd, args);
	pthread_t p;
	//printf("thread command = %s\n", command);
	pthread_create(&p, NULL, sys_call, (void**)&command);
	pthread_join(p, NULL);
}

void replaceStrWithSpace(char line[], char* str) {
	int k=0;
	char *needle = strstr(line, str);
	if (!needle) return;

	for (k=0; k < strlen(str); k++) {
		needle[k] = ' ';
	}
}

int main()
{
	int cnt=1;
	char* line=NULL;
	long unsigned int len=BUFSIZE;
	printf("vibhor shell %d> ", cnt++);
    fflush(stdout);
	int sOptc = 0;
	char** sOptv = (char **)malloc(sizeof(char*)*ARGCOUNT);
	char* cdir = getenv("PWD");
	while(getline(&line,&len,stdin)!=-1){
		char* argv[1];
		if (line[strlen(line)-1] == '\n') {
			line[strlen(line)-1] = '\0';
		}
		argv[0] = strdup(line);
		splitOpts(1, argv, &sOptc, sOptv, 0);
		if (sOptc == 0){
			printf("vibhor shell %d> ", cnt++);
    		fflush(stdout);
			free(argv[0]);
			continue;
		}
		if (!strcmp(sOptv[0], "cd")) {
			cd(sOptc, sOptv);
		}
		else if (!strcmp(sOptv[0], "pwd")) {
			pwd();
		}
		else if (!strcmp(sOptv[0], "echo")) {
			echo(sOptc, sOptv);
		}
		else if (!strcmp(sOptv[0], "cat&f")) {
			cat(sOptc, sOptv);
		}
		else if (!strcmp(sOptv[0], "ls&f")) {
			ls(sOptc, sOptv);
		}
		else if (!strcmp(sOptv[0], "mkdir&f")) {
			mymkdir(sOptc, sOptv);
		}
		else if (!strcmp(sOptv[0], "rm&f")) {
			rm(sOptc, sOptv);
		}
		else if (!strcmp(sOptv[0], "date&f")) {
			date(sOptc, sOptv);
		}
		else if (!strcmp(sOptv[0], "cat&t") ||
				 !strcmp(sOptv[0], "ls&t") ||
				 !strcmp(sOptv[0], "mkdir&t") ||
				 !strcmp(sOptv[0], "rm&t") ||
				 !strcmp(sOptv[0], "date&t")
				) {
			replaceStrWithSpace(line, "&t");
			execute_thread(cdir, line);
		}
		else if (!strcmp(sOptv[0], "cat") ||
				 !strcmp(sOptv[0], "ls") ||
				 !strcmp(sOptv[0], "mkdir") ||
				 !strcmp(sOptv[0], "rm") ||
				 !strcmp(sOptv[0], "date")
				) {
			//replaceStrWithSpace(line, "&p");
			//sOptv[0][strlen(sOptv[0])-2]='\0';
			char cmdName[10];
			sprintf(cmdName, "my%s", sOptv[0]);
			execute_process(cdir, line, cmdName);
		}
		else if (!strcmp(sOptv[0], "exit") || (!strcmp(sOptv[0], "quit"))) {
			break;
		} else {
			printf("Unknown command: '%s'\n", sOptv[0]);
		}
		free(line);
		line=NULL;
		for(int i=0;i<sOptc;i++){
			free(sOptv[i]);
		}
		free(argv[0]);
		printf("vibhor shell %d> ", cnt++);
    	fflush(stdout);
	}
	return 0;
}
