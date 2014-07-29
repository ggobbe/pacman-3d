// View.h
#pragma once
#include "Cell.h"
#include "Model.h"
#include <sstream>
#include <iostream>
#include <math.h>
//#include "View3D.h";
//#include "View2D.h";

using namespace std;

// Le V de MVC
class View
{
protected:
	static View *instance;		// Instance de la vue pour le pattern singleton
	Model *model;				// Instance du Model pour acc�der aux donn�es modifiables
	int h_global, w_global;		// Pour stocker hauteur et largeur de la fen�tre d'�cran
	GLdouble d1_reel,d2_reel;
	GLdouble delta1_reel,delta2_reel;
	GLdouble d1World, d2World;
	GLdouble delta1World, delta2World;
	int idList_PacMan, idList_Mouse_Controls,
		idList_Pill, idList_SuperPill,
		idList_PacMan_2D, idList_Ghost;

	void viewportInfos();			// D�finit une viewport pour les informations concernant la partie en cours
	void drawScore();		// Affiche le score dans la viewport correspondante
	void drawLifes();		// Affiche le nombre de vies dans la viewport correspondante
	void drawWorld();		// Dessine le labyrinthe
	void drawCell(Cell cell, int i, int j);
	virtual void drawPacMan();		// D�finit un carr� ayant subi une certaine rotation autour de son centre
	virtual void drawGhosts();		// Dessine les fant�mes
	virtual void drawHLine();
	virtual void drawWLine();
	virtual void drawPills();	// Dessine les pastilles

	void viewportControls(); // D�finit une viewport pour les contr�les souris
	void drawMouseControls();
	void drawButton(string,int);

	void initDlPacMan();
	void initDlMouseControls();
	void initDlPills();
	void initDlSuperPills();
	void initDlPacMan2D();
	void initDisplayLists(void);

public:
	void display(void);				// D�finit les formes dans l'univers de dessin
	void reshape(int w, int h);		// Appel� en cas de redimensionnement de la fen�tre d'�cran
	void redraw(void);				// Rafraichit l'affichage
	void zoomInOut(int);			// Zoom In/Zoom out sur le labyrinthe
	void gameOver(void);			// Affiche un GAME OVER
	void setInstance(View*);		// Pattern strat�gie pour changer entre View3D et 2D
	virtual void viewportWorld();			// D�finit une viewport pour le monde
	virtual void drawCentralControl(GLenum);		// Dessine un point central permettant au joueur de cliquer pour commencer la rotation du plateau de jeu
	void setHGlobal(int);			// Positionne la hauteur globale
	void setWGlobal(int);			// Positionne la largeur globale
	virtual void initLights(void);			// Initialisation de l'�clairage

	GLint getHGlobal() 
	{
		return h_global;
	}

	GLint getWGlobal()
	{
		return w_global;
	}

	GLdouble getD1()
	{
		return d1_reel;
	}

	GLdouble getD2()
	{
		return d2_reel;
	}

	GLdouble getDelta1()
	{
		return delta1_reel;
	}

	GLdouble getDelta2()
	{
		return delta2_reel;
	}

	View();
	virtual ~View();

	// Pattern singleton
	static View *getInstance();
	static float * getNormal(float * p1, float * p2, float * p3);
};
