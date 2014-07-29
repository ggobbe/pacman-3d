#include "PacMan.h"

PacMan::PacMan()
{
	color = new float[3];
	color[0] = 1.0;
	color[1] = 1.0;
	color[2] = 0.0;
	mode = NORMAL;
	dir = PAC_WEST;
	nextDir = -1;
	mouthOpen = false;
	radius = PACMAN_RADIUS;
	posX = 5;
	posY = 3;
	delta = 0;
	dying = false;
}

PacMan::~PacMan() 
{
	delete[] color;
}

void PacMan::setNextDir(int newNextDir) 
{
	nextDir = newNextDir;
}

void PacMan::switchMouth()
{
	mouthOpen = !mouthOpen;
}

void PacMan::animSuperMode(bool value)
{
	mode = value;
	// Si c'est la fin du mode super, on verifie la couleur pour qu'il ne reste pas en couleur extra
	if (!value && color[1] != 1.0)
	{
		color[0] = 1.0;
		color[1] = 1.0;
		color[2]  = 0.0;
	}
}

void PacMan::couleurAleatoire() 
{
	if(color[1] == 1.0) 
	{ // jaune -> rouge
		color[0] = 1.0;
		color[1] = 0.0;
		color[2] = 0.0;
	}
	else 
	{ // rouge -> jaune
		color[0] = 1.0;
		color[1] = 1.0;
		color[2] = 0.0;
	}
}

void PacMan::setDying(int value)
{
	dying = value;
}