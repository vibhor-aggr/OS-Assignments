
compile : mycd mypwd mycat myecho mymkdir mydate myls myrm myshell

mycd : cd.c option.o
	gcc -o mycd cd.c option.o -D_GNU_SOURCE -DSTANDALONE

mypwd : pwd.c
	gcc -o mypwd pwd.c -D_GNU_SOURCE -DSTANDALONE

mycat : cat.c option.o
	gcc -o mycat cat.c option.o -DSTANDALONE

myecho : echo.c option.o
	gcc -o myecho echo.c option.o -DSTANDALONE

mymkdir : mkdir.c option.o
	gcc -o mymkdir mkdir.c option.o -DSTANDALONE

mydate : date.c option.o
	gcc -o mydate date.c option.o -DSTANDALONE

myls : ls.c option.o
	gcc -o myls ls.c option.o -DSTANDALONE

myrm : rm.c option.o
	gcc -o myrm rm.c option.o -DSTANDALONE

option.o : option.c
	gcc -c option.c

myshell : shell.c cd.c pwd.c cat.c mkdir.c date.c ls.c rm.c echo.c option.c
	gcc -c shell.c -g
	gcc -o myshell shell.o cd.c pwd.c cat.c mkdir.c date.c ls.c rm.c echo.c option.c -D_GNU_SOURCE -g -lpthread

