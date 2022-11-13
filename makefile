# -*- Makefile -*- #

# Target: Dependencies
#     Action

#
# NEED TO ADD .h FILES!!!
#

main: main.o mainFunctions.o pacmanFunctions.o mapFunctions.o powerupFunctions.o ghostFunctions.o
	gcc -o main main.o mainFunctions.o pacmanFunctions.o mapFunctions.o powerupFunctions.o ghostFunctions.o -lncurses -lm

main.o: main.c
	gcc -c main.c

ghostFunctions.o: ghost/ghostFunctions.c
	gcc -c ghost/ghostFunctions.c

powerupFunctions.o: powerup/powerupFunctions.c
	gcc -c powerup/powerupFunctions.c

mapFunctions.o: map/mapFunctions.c
	gcc -c map/mapFunctions.c

pacmanFunctions.o: pacman/pacmanFunctions.c
	gcc -c pacman/pacmanFunctions.c

mainFunctions.o: mainFunctions/mainFunctions.c
	gcc -c mainFunctions/mainFunctions.c

clean:
	rm -f *.o a.out main

# included in clean
#map
#pacman
#map.h.gch

# map: map.o
# 	gcc -o map map.c -lm

# pacman: pacman.o
# 	gcc -o pacman pacman.o

# main: main.o mainFunctions.o pacmanFunctions.o pacman.o mapFunctions.o map.o powerupFunctions.o powerup.o ghostFunctions.o ghost.o
# 	gcc -o main main.o mainFunctions.o pacmanFunctions.o pacman.o mapFunctions.o map.o powerupFunctions.o powerup.o ghostFunctions.o ghost.o -lncurses

# ghost.o: ghost/ghost.c
# 	gcc -c ghost/ghost.c
# powerup.o: powerup/powerup.c
# 	gcc -c powerup/powerup.c
# map.o: map/map.c
# 	gcc -c map/map.c map/map.h
# pacman.o: pacman/pacman.c
# 	gcc -c pacman/pacman.c