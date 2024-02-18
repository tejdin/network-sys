all: client server
client: client.c network.o graphics.o game.o config.h
	gcc -o client client.c network.o graphics.o game.o `sdl2-config --cflags --libs` -lSDL2_ttf -lpthread -lm
server: server.c network.o game.o config.h
	gcc -o server server.c network.o game.o -lpthread -lm
network.o: network.h network.c
	gcc -c network.c
graphics.o: graphics.h graphics.c
	gcc -c graphics.c `sdl2-config --cflags`
game.o: game.h game.c config.h
	gcc -c game.c
