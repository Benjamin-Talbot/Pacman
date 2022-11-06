# -*- Makefile -*- #

# Target: Dependencies
#     Action

#
# NEED TO ADD .h FILES!!!
#

main: main.o pacman.o map.o
	gcc -o main main.o pacman.o map.o -lncurses

main.o: main.c
	gcc -c main.c

pacman: pacman.o
	gcc -o pacman pacman.o

pacman.o: pacman.c
	gcc -c pacman.c

map.o: map.c
	gcc -c map.c map.h

# map: map.o
# 	gcc -o map map.c -lm

clean:
	rm -f *.o pacman a.out map.h.gch main
#map