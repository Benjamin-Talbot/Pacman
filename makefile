# -*- Makefile -*- #

# Target: Dependencies
#     Action

#
# NEED TO ADD .h FILES!!!
#

main: main.o score.o mainFunctions.o pacmanFunctions.o pacmanAI.o mapFunctions.o powerupFunctions.o ghostFunctions.o portalFunctions.o
	gcc -o main main.o score.o mainFunctions.o pacmanFunctions.o pacmanAI.o mapFunctions.o powerupFunctions.o ghostFunctions.o portalFunctions.o -lncurses -lm

# debug: main.o mainFunctions.o pacmanFunctions.o mapFunctions.o powerupFunctions.o ghostFunctions.o
# 	gcc -o main main.o -g mainFunctions.o pacmanFunctions.o mapFunctions.o powerupFunctions.o ghostFunctions.o -lncurses -lm

main.o: main.c
	gcc -c main.c -g

portalFunctions.o: portal/portalFunctions.c
	gcc -c portal/portalFunctions.c -g

ghostFunctions.o: ghost/ghostFunctions.c
	gcc -c ghost/ghostFunctions.c -g

powerupFunctions.o: powerup/powerupFunctions.c
	gcc -c powerup/powerupFunctions.c -g

mapFunctions.o: map/mapFunctions.c
	gcc -c map/mapFunctions.c -g

pacmanAI.o: pacman/pacmanAI.c
	gcc -c pacman/pacmanAI.c -g

pacmanFunctions.o: pacman/pacmanFunctions.c
	gcc -c pacman/pacmanFunctions.c -g

mainFunctions.o: mainFunctions/mainFunctions.c
	gcc -c mainFunctions/mainFunctions.c -g

score.o: score/score.c
	gcc -c score/score.c -g

clean:
	rm -f *.o a.out main

# included in clean
#map
#pacman
#map.h.gch
# map/map.h.gch

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
