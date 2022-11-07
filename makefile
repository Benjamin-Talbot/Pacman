# -*- Makefile -*- #

# Target: Dependencies
#     Action

#
# NEED TO ADD .h FILES!!!
#

main: main.o functions.o pacman.o map.o powerup.o ghost.o
	gcc -o main main.o map.o pacman.o powerup.o functions.o ghost.o -lncurses

main.o: main.c
	gcc -c main.c

ghost.o: ghost.c
	gcc -c ghost.c

powerup.o: powerup.c
	gcc -c powerup.c

map.o: map.c
	gcc -c map.c map.h

pacman.o: pacman.c
	gcc -c pacman.c

functions.o: functions.c
	gcc -c functions.c

clean:
	rm -f *.o a.out map.h.gch main
#map
#pacman

# map: map.o
# 	gcc -o map map.c -lm

# pacman: pacman.o
# 	gcc -o pacman pacman.o