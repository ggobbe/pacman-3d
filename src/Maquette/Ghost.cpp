#include "Ghost.h"

Ghost::Ghost()
{
	posX = 0;
	posY = 0;
	dir = PAC_WEST;
	color = new float[3];
	color[0] = 1.0;
	color[1] = 0.0;
	color[2] = 1.0;
	delta = SCALE;
	radius = GHOST_RADIUS;
}

Ghost::~Ghost()
{
	delete[] color;
}