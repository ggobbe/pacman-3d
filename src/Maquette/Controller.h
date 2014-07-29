// Controller.h
#pragma once
#include "Model.h"
#include "View.h"
#include <time.h>
#include <exception>
#include <iostream>
#include "windows.h"
#include "View2D.h"
#include "View3D.h"

using namespace std;

#define TEMPSBASE 80 //10 // Temps de caden�age du rafraichissement de l'image

// Le C de MVC
class Controller
{
private:
	static Controller *instance;	// Instance du controlleur pour le pattern singleton
	Model *model;					// Le M de MVC
	View  *view;					// Le V de MVC
	Controller();
	~Controller();
	int stepOK;
	clock_t timeModeSuper;			// timer pour le mode SUPER du PacMan
	clock_t *timeGhostInPrison;		// timer pour les fant�mes en prison
	bool stargate;					// pour g�rer les t�l�portations du PacMan
	
	void animateGhosts();
	void handleCollisions(int, Ghost*);
	void switchSuperMode();								// Passage du mode super au mode normal le cas �ch�ant
	void changeDirGhost(Ghost*, int, int, int*);		// D�termine s'il faut changer la position du PacMan ou pas et la change le cas �ch�ant
	void handleChangeDirPacMan();						// D�termine s'il faut changer la direction du PacMan
	void changeDirPacMan(int, bool);					// Change la position du PacMan si le bool�en pass� est � true
	void handlePills();									// S'occupe de g�rer la disparition des pills
	void handlePacManSmooth();							// S'occupe de g�rer le mouvement fluide du PacMan
	void handleSwitchDimension();						// S'occupe du changement de dimension (2D <> 3D)
	void handleNewDir(int);								// S'occupe de changer la direction dans le mod�le
	void handleStargate(int,int,int,int,int*);			// S'occupe de la t�l�portation du PacMan

public:
	void mouse(int button, int state, int x, int y);	// Appell� lors d'actions � la souris
	void special(int key, int x, int y);				// Appell� lors d'actions via touches du clavier �tendu
	void animateDisplay();								// Gestion de l'animation
	static Controller *getInstance();					// Singleton
	void togglePacManViewMode();						// Switch the PacMan view mode
	void mouseRight(int button, int state, float x, float y, int* viewport);
	void mouseMiddle(float x, float y, int* viewport);
	void handlePicking(int button, int state, int x, int y);
};

