all: myftp

myftp: main.c ftp.o
	gcc -o myftp main.c ftp.o -lpthread

ftp.o: ftp.c ftp.h
	gcc -c ftp.c
