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

#define TEMPSBASE 80 //10 // Temps de cadençage du rafraichissement de l'image

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
	clock_t *timeGhostInPrison;		// timer pour les fantômes en prison
	bool stargate;					// pour gérer les téléportations du PacMan
	
	void animateGhosts();
	void handleCollisions(int, Ghost*);
	void switchSuperMode();								// Passage du mode super au mode normal le cas échéant
	void changeDirGhost(Ghost*, int, int, int*);		// Détermine s'il faut changer la position du PacMan ou pas et la change le cas échéant
	void handleChangeDirPacMan();						// Détermine s'il faut changer la direction du PacMan
	void changeDirPacMan(int, bool);					// Change la position du PacMan si le booléen passé est à true
	void handlePills();									// S'occupe de gérer la disparition des pills
	void handlePacManSmooth();							// S'occupe de gérer le mouvement fluide du PacMan
	void handleSwitchDimension();						// S'occupe du changement de dimension (2D <> 3D)
	void handleNewDir(int);								// S'occupe de changer la direction dans le modèle
	void handleStargate(int,int,int,int,int*);			// S'occupe de la téléportation du PacMan

public:
	void mouse(int button, int state, int x, int y);	// Appellé lors d'actions à la souris
	void special(int key, int x, int y);				// Appellé lors d'actions via touches du clavier étendu
	void animateDisplay();								// Gestion de l'animation
	static Controller *getInstance();					// Singleton
	void togglePacManViewMode();						// Switch the PacMan view mode
	void mouseRight(int button, int state, float x, float y, int* viewport);
	void mouseMiddle(float x, float y, int* viewport);
	void handlePicking(int button, int state, int x, int y);
};

