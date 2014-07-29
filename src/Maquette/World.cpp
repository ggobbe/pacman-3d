#include "World.h"

using namespace std;

/* Initialise le terrain de jeu grâce à un fichier texte contenant la map à charger */
// TODO : vérifier si fichier carte existe et le passer en paramètre
// TODO : taille du world dynamique selon fichier chargé
// TODO : surcharge opérateur >> pour charger fichier (cfr G. Seront)

World *World::instance = 0;
int World::maxRows = 0;
int World::maxCols = 0;
string World::nomFichier = "";
int World::nbPills = 0;
int World::nbSuperPills = 0;
int World::nbGhosts = 0;

//TODO : fonction friend qui n'est pas dans une classe
std::ifstream &operator>>(std::ifstream &fichier, Cell*** &map) 
{
	if(fichier && fichier.is_open()) 
	{
		World* w = World::getInstance();
		w->initMap(fichier,map);
	}
	return fichier;
}

World::World() {
}

World::World(string name) 
{
	nomFichier = name;
	maxRows = 0;
	maxCols = 0;
	ifstream fichier(name, ios_base::in);  // on ouvre le fichier en lecture
	nbPills = 0;
	nbSuperPills = 0;
	nbGhosts = 0;

	fichier >> map;
	if(maxRows == 0) {
		exit(EXIT_SUCCESS);
	}
}

// Destructeur
World::~World()
{
	this->destroyMap();
}


World *World::getInstance()
{
	if (instance == 0)
		instance = new World();

	return instance;
}

void World::setMaxRows(int newMaxRows) {
	maxRows = newMaxRows;
}

void World::setMaxCols(int newMaxCols) {
	maxCols = newMaxCols;
}

void World::setNbPills(int nvNbPills) {
	nbPills = nvNbPills;
}

void World::setNbSuperPills(int nvNbSuperPills) {
	nbSuperPills = nvNbSuperPills;
}

void World::initMap(std::ifstream &fichier, Cell*** &map) 
{
	string ligne;
	int cmpt=0;


	// initialisation de la matrice pour le World
	while(getline(fichier, ligne))  // on lit une première fois pour déterminer le nombre de lignes et de colonnes
	{
		if(maxRows == 0) // on compte les colonnes seulement une fois
		{ 
			for(unsigned int i=0;i<ligne.size();i++)
			{
				maxCols++;
			}
		}
		maxRows++;
	}
	fichier.close(); // on ferme le fichier

	map=new Cell**[maxRows];
	for(int i=0;i<maxRows;i++)
	{
		map[i]=new Cell*[maxCols];
		for(int j=0; j<maxCols; j++) 
		{
			map[i][j] = new Cell();
		}
	}

	fichier.open(nomFichier, ios_base::in);

	if(fichier && fichier.is_open())
	{
		while(getline(fichier, ligne))  // tant que l'on peut mettre la ligne dans "contenu"
		{
			for(unsigned int i=0;i<ligne.size();i++)
			{
				char c = ligne[i];

				int type = -1;

				// Position de départ du pacman (traitement de la lettre à part)
				if(c == 'P')
					type = PACMAN_POS | EMPTY;

				int x = cmpt * SCALE;
				int y = i * SCALE;

				// si pas eu de lettre, on convertit en nombre
				if(type == -1)
					type = atoi(&c);

				// pour centrer
				x -= 5 * SCALE;
				y -= 3 * SCALE;
				y = -y;

				if(type == PILL)
					nbPills++;
				else if(type == SUPER_PILL) 
					nbSuperPills++;
				else if(type == GHOST)
					nbGhosts++;

				map[cmpt][i] = new Cell(x,y,type);
			}
			cmpt++;
		}
	}
}


void World::destroyMap() {
for (int i = 0; i < maxRows; i++) 
	{
		for(int j = 0; j < maxCols; j++) 
		{
			delete map[i][j];
		}
		delete[] map[i];
	}
	delete[] map;
}