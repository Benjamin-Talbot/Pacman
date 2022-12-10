#include "portal.h"

#include <stdio.h>
#include <ncurses.h>

#include "../pacman/pacman.h"

void portalInit(pPortal this, int x, int y);

void portalLink(pPortal first, pPortal second);

void teleport(pPortal this, pPacman pacman);

void portalDraw(pPortal this);

void portalsDraw(pPortal* portals, int numPortals);