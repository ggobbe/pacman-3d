// Model.h
#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include "Globals.h"
#include "PacMan.h"
#include "Ghost.h"
#include "World.h"


#define INCRANGLE .5 // Incrément d'angle (degrés) pour la rotation du carré

using namespace glut;
using namespace std;

// Le M de MVC
class Model
{
private:
	static Model *instance;	// Instance du Model pour le pattern singleton
	bool started;
	int level;				// niveau de jeu
	int lifes;				// nombre de vies du PacMan
	int score;				// score total
	int scoreLevel;			// score du niveau en cours
	int scoreBearing;		// score atteint pour le gain de vie
	int projectionMode;		// type de projection utilisé pour afficher la viewportWorld (celle qui contient le labyrinthe)
	bool spinningMode;		// définit si le plateau de jeu est en train de tourner ou pas
	bool pacmanViewMode;	// définit si l'on place la caméra derrière le PacMan pour jouer
	PacMan *pacman;			// Bonhomme pacman
	World *world;			// carte
	Ghost **ghosts;			// fantômes

	int posCamX;			// position de la caméra sur l'axe des X
	int posCamY;			// position de la caméra sur l'axe des Y
	int posCamZ;			// position de la caméra sur l'axe des Z

	int zoom;				// zoom sur le labyrinthe

	int **loadCarte();		// méthode pour charger la carte

public:
	bool hasBegun(void) 
	{
		return started;
	}

	World *getWorld()
	{
		return world;
	}

	// Pattern singleton
	Model();
	~Model();
	static Model *getInstance();

	PacMan *getPacMan() 
	{
		return pacman;
	}

	int getScore() 
	{
		return score;
	}

	int getScoreLevel() 
	{
		return scoreLevel;
	}

	int getLifes() 
	{
		return lifes;
	}

	int getLevel() 
	{
		return level;
	}

	Ghost* getGhost(int i)
	{
		return ghosts[i];
	}

	int getCamPosX() {
		return posCamX;
	}

	int getCamPosY() {
		return posCamY;
	}

	int getCamPosZ() {
		return posCamZ;
	}

	int getZoom() {
		return zoom;
	}

	int getProjectionMode() {
		return projectionMode;
	}

	bool isStarted() {
		return started;
	}

	bool isInPacManViewMode() {
		return pacmanViewMode;
	}

	bool isSpinning() {
		return spinningMode;
	}

	void incLifes(int);
	void setScore(int);
	void setProjectionMode(int);
	void incScore(int step);
	void nextLevel(void);
	void reset(void);
	void initGhosts();
	void initPacMan();
	void startGame();
	void stopGame();
	void pacManDie();
	void incCamPosX(int);
	void incCamPosY(int);
	void incCamPosZ(int);
	void zoomInOut(int);
	void reinitPosCam(void);
	void reinitZoom(void);
	void toggleSpinning();
	void reinitAttributes(char*);
	void setPacManViewMode(bool);
};