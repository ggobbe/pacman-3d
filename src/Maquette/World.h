#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include "Globals.h"
#include "Cell.h"
#include "MyGlut.h"


using namespace glut;
using namespace std;

class World 
{	
private:
	static World* instance;
	static string nomFichier;
	static int nbPills;
	static int nbSuperPills;
	static int nbGhosts;
	static int maxRows;
	static int maxCols;
	Cell*** map;

public:
	World();
	World(string);
	~World();
	//int isPillCollide(int,int);	/* Checks if Pacman ate a pill */
	//int isBoundaryCollide(int,int); /* Checks if Pacman/Monster collided with the boundary of the maze*/
	Cell*** getMap() 
	{
		return map;
	}

	int getNbPills() 
	{
		return nbPills;
	}

	int getNbSuperPills() 
	{
		return nbSuperPills;
	}

	int getMaxRows() 
	{
		return maxRows;
	}

	int getMaxCols() 
	{
		return maxCols;
	}

	int getNbGhosts()
	{
		return nbGhosts;
	}

	static World *getInstance();
	void initMap(std::ifstream&,Cell***&);
	void destroyMap(void);
	void setMaxRows(int);
	void setMaxCols(int);
	void setNbPills(int);
	void setNbSuperPills(int);
};
