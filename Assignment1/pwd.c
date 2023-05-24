#include <unistd.h>
#include <stdio.h>

int pwd();

#ifdef STANDALONE
int main()
{
	return pwd();
}
#endif

int pwd()
{
	char *cwd = get_current_dir_name();      
	printf("%s\n", cwd);
	return 0;
}
