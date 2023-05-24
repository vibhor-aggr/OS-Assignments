
VibhorAggarwal_2021111 : VibhorAggarwal_2021111.o
	gcc -m64 -no-pie VibhorAggarwal_2021111.o -o VibhorAggarwal_2021111

VibhorAggarwal_2021111.o : VibhorAggarwal_2021111.asm
	yasm -f elf64 -g dwarf2 -l VibhorAggarwal_2021111.lst VibhorAggarwal_2021111.asm
