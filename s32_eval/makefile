all: main serveur_euclide client_euclide

main: main.c eval.o
	gcc -o main main.c eval.o

eval.o: eval.c eval.h
	gcc -c eval.c

client_euclide: client_euclide.c eval.o
	gcc -o client_euclide client_euclide.c eval.o

serveur_euclide: serveur_euclide.c eval.o
	gcc -o serveur_euclide serveur_euclide.c eval.o