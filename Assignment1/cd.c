#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include "option.h"

void check_cwd();

int cd(int argc, char *argv[]);

#ifdef STANDALONE
int main(int argc, char* argv[]){
	int sOptc = 0;
	char** sOptv = (char **)malloc(sizeof(char*)*ARGCOUNT);
	splitOpts(argc, argv, &sOptc, sOptv, 1);
	int ret = cd(sOptc, sOptv);
	for(int i=0;i<sOptc;i++){
		free(sOptv[i]);
	}
	free(sOptv);
	return ret;
}
#endif

int cd(int argc, char *argv[])
{	
	if(argc>2){
		printf("cd: too many arguments\n");
		return -1;
	}
	//check_cwd();
	char* dir=NULL;
	if(argc==1){
		dir=getenv("HOME");
	}else{
		dir=argv[1];
	}
	int ret = chdir(dir);
	if(ret!=0){
		printf("cd: %s: %s\n",dir, strerror(errno));
		return -1;
	}
	//check_cwd();
	//printf("%s\n", pwd);
	return 0;
}

void check_cwd()
{
	char *cwd = get_current_dir_name();      
	printf("%s\n", cwd);
	return;
}
