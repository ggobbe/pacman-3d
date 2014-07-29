// View.cpp
#include "View.h"

// Les membres static non constants doient êtres initialisés dans le .cpp
View *View::instance = 0;

// Rafraichissement de l'affichage
void View::redraw()
{
	glutPostRedisplay();
}

// Implémentation classique du pattern singleton
View::View()
{
	model = Model::getInstance();
	glClearColor(0.0,0.0,0.0,0); // Définit la couleur à utiliser lors des appels à glClear(...)

	initDisplayLists(); // initialisation des display lists
}

View::~View()
{
	delete instance;
	delete model;
}

View *View::getInstance()
{
	if(View::instance == 0) {
		delete instance;
		instance = new View();
	}
	return instance;
}

void View::setInstance(View * newView)
{
	instance = newView;
}

void View::setHGlobal(int newHGlobal) {
	h_global = newHGlobal;
}

void View::setWGlobal(int newWGlobal) {
	w_global = newWGlobal;
}

// Définit une viewport pour le monde et ses caractéristiques de projection
void View::viewportWorld()
{
}

// Définit une viewport et ses caractéristiques de projection
void View::viewportInfos()
{
	GLdouble vp1 = w_global*TAILLE_X_VIEWPORT_INFOS;
	GLdouble vp2 = 0;
	GLdouble w = w_global - vp1;
	GLdouble h = h_global*TAILLE_Y_VIEWPORT_INFOS;

	GLdouble d1 = 0;
	GLdouble delta1 = 2500 - d1;
	GLdouble d2 = 0;
	GLdouble delta2 = 100 - d2;

	glViewport (vp1, vp2, w, h); // Taille et positionnement de la viewport
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity(); // Initialisation de la matrice de projection

	if((w/h) > (delta1/delta2)) 
	{
		gluOrtho2D(d1 - 0.5*((w * delta2/h) - delta1), d1+delta1+0.5*((w*delta2/h)-delta1), d2, d2 + delta2);
	}
	else 
	{ 
		gluOrtho2D(d1, d1 + delta1, d2 - 0.5*((h*delta1 / w) - delta2), d2 + delta2 + 0.5*((h*delta1 / w) - delta2));
	}

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity(); // Initialisation de la matrice de modelview
}

void View::viewportControls() 
{
	int heightInfos = h_global*TAILLE_Y_VIEWPORT_INFOS;
	GLdouble vp1 = w_global - (w_global*TAILLE_X_VIEWPORT_CONTROLS);
	GLdouble vp2 = heightInfos;
	GLdouble w = w_global*TAILLE_X_VIEWPORT_CONTROLS;
	GLdouble h = h_global - heightInfos;
	GLdouble d1 = 0;
	GLdouble d2 = -50;
	GLdouble delta1 = 200 - d1;
	GLdouble delta2 = 200 - d2;

	glViewport(vp1, vp2, w, h); // Taille et positionnement de la viewport
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity(); // Initialisation de la matrice de projection

	if((w/h) > (delta1/delta2)) 
	{
		gluOrtho2D(d1 - 0.5*((w * delta2/h) - delta1), d1+delta1+0.5*((w*delta2/h)-delta1), d2, d2 + delta2);
		d1_reel = d1 - 0.5*((w * delta2/h) - delta1);
		d2_reel = d2;
		delta1_reel = d1+delta1+0.5*((w*delta2/h)-delta1) - d1_reel;
		delta2_reel = d2 + delta2 - d2_reel;
	}
	else 
	{ 
		gluOrtho2D(d1, d1 + delta1, d2 - 0.5*((h*delta1 / w) - delta2), d2 + delta2 + 0.5*((h*delta1 / w) - delta2));
		d1_reel = d1;
		d2_reel = d2 - 0.5*((h*delta1 / w) - delta2);
		delta1_reel = d1 + delta1 - d1_reel;
		delta2_reel = d2 + delta2 + 0.5*((h*delta1 / w) - delta2) - d2_reel;
	}

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity(); // Initialisation de la matrice de modelview
}

void View::initDlPacMan() {
	idList_PacMan=glGenLists(10);
	//création de la liste pour le dessin du PacMan
	glNewList(idList_PacMan,GL_COMPILE);
	GLUquadricObj *sphere;
	sphere = gluNewQuadric();
	gluQuadricDrawStyle(sphere, GLU_FILL);
	gluSphere(sphere, model->getPacMan()->getRadius(), 25, 15);	// avec les lumières
	gluDeleteQuadric(sphere);
	glEndList();
}

void View::initDlMouseControls() {
	// création de la liste pour le dessin des contrôles souris
	idList_Mouse_Controls = idList_PacMan + 1;
	glNewList(idList_Mouse_Controls, GL_COMPILE);
	int gap = 5;
	int largeur = 20;
	int hauteur = 15;

	glColor3f(1.0,1.0,1.0);
	// gauche

	glScaled(2.5,2,1);
	glColor3f(1.0,1.0,1.0);

	//gauche
	glRectf(gap,0,largeur,hauteur);

	//droite
	glRectf(largeur+(3*gap),0,(2*largeur)+(gap*2),hauteur);

	//haut
	glRectf(largeur,hauteur,largeur + hauteur,hauteur + largeur);

	// bas
	glRectf(largeur,0,largeur + hauteur,-largeur);
	glEndList();
}

void View::initDlPills() {
	// création de la liste pour le dessin d'une "pill"
	idList_Pill = idList_Mouse_Controls + 1;
	glNewList(idList_Pill, GL_COMPILE);
	GLUquadricObj* pill = gluNewQuadric();	
	gluQuadricDrawStyle(pill, GLU_FILL);
	//glutWireSphere(0.5, 10, 5);
	gluSphere(pill, 0.5, 10, 5);	// avec les lumières
	gluDeleteQuadric(pill);
	glEndList();
}

void View::initDlSuperPills() {
	idList_SuperPill = idList_Pill + 1;
	glNewList(idList_SuperPill, GL_COMPILE);
	GLUquadricObj* spill = gluNewQuadric();	
	gluQuadricDrawStyle(spill, GLU_FILL);
	//glutWireSphere(1.5, 10, 5);
	gluSphere(spill, 1.0, 10, 5);	// avec les lumières
	gluDeleteQuadric(spill);
	glEndList();
}

void View::initDlPacMan2D() {
	// création de la liste pour le dessin du PacMan 2D
	idList_PacMan_2D = idList_SuperPill + 1;
	glNewList(idList_PacMan_2D, GL_COMPILE);
	GLUquadricObj *disque = gluNewQuadric();
	gluQuadricDrawStyle(disque, GLU_FILL);
	gluPartialDisk(disque, 0, 10, 100, 1, 135, 270);
	gluDeleteQuadric(disque);
	glEndList();
}

void View::initDisplayLists() 
{
	this->initDlPacMan();
	this->initDlMouseControls();
	this->initDlPills();
	this->initDlSuperPills();
	this->initDlPacMan2D();
}

void View::drawHLine() {
	// Inutile d'implémenter cette méthode ici
}

void View::drawWLine() {
	// Inutile d'implémenter cette méthode ici
}


// Définit le modèle pour affichage à l'écran
void View::display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Initialisation du buffer de couleur à la couleur de fond


	// Viewport contenant les informations sur la partie en cours
	viewportInfos();
	if(model->getLifes() < 0) {
		model->stopGame();
		gameOver();
	}
	else {
		drawScore();
		drawLifes();
	}

	// Viewport contenant les contrôles souris (boutons)
	viewportControls();
	drawMouseControls();
	drawButton("Reset",100);
	drawButton("Close",150);
	drawButton("View", 200);



	// Viewport contenant le plateau de jeu
	glEnable(GL_DEPTH_TEST);

	initLights();

	viewportWorld(); // Définit une viewport et ses caractéristiques de projection
	drawWorld();
	drawPills();
	drawPacMan();
	drawGhosts();

	glDisable(GL_LIGHT0);
	glDisable(GL_LIGHT1);
	glDisable(GL_LIGHT2);
	glDisable(GL_LIGHT3);
	glDisable(GL_LIGHTING);	// nuit !
	glDisable(GL_DEPTH_TEST);


	glutSwapBuffers(); // Car on travaille en mode "double buffer", cf. main.cpp
}

// Récupère les dimensions de la fenêtre d'écran
void View::reshape(int w, int h)
{	
	w_global=w;
	h_global=h;
}

void View::drawCentralControl(GLenum selectMode) {
}

void View::gameOver() {
	glPushMatrix();
	stringstream txtGameOver;
	txtGameOver << "GAME OVER";
	glColor3f(1, 0, 0); // TODO : glColor3fv
	/* Antialiasing du texte */ 

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glEnable(GL_LINE_SMOOTH);

	for(unsigned int i = 0; i < txtGameOver.str().size(); i++)
	{
		glutStrokeCharacter(GLUT_STROKE_ROMAN, txtGameOver.str()[i]);
	}

	stringstream txtScore;
	txtScore << "  Score: " << model->getScore();
	glColor3f(1, 1, 1); // TODO : glColor3fv
	/* Antialiasing du texte */ 

	for(unsigned int i = 0; i < txtScore.str().size(); i++)
	{
		glutStrokeCharacter(GLUT_STROKE_ROMAN, txtScore.str()[i]);
	}
	glPopMatrix();
}

void View::drawMouseControls() 
{
	glPushMatrix();
	glTranslated(0, 0, 0);
	glCallList(idList_Mouse_Controls);
	glPopMatrix();
}

void View::drawButton(string buttonName, int deltaY) {
	int gap = 5;
	int largeur = 150;
	int hauteur = 40;
	stringstream txtScore;
	txtScore << buttonName;

	glPushMatrix();
	glTranslated(0,deltaY,0);
	glColor3f(1.0,1.0,1.0);
	glRectf(gap,0,largeur,hauteur);

	glTranslated(30,5,0);
	glScaled(0.3,0.3,1);

	// Anti-aliasing
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glEnable(GL_LINE_SMOOTH);

	glColor3f(0.0,0.0,0.0);
	for(unsigned int i = 0; i < txtScore.str().size(); i++)
	{
		glutStrokeCharacter(GLUT_STROKE_ROMAN, txtScore.str()[i]);
	}
	glPopMatrix();
}

void View::drawPills()
{
	// Inutile d'implémenter drawPills ici
}

void View::drawPacMan() {
	// Inutile d'implémenter drawPacMan ici
}

void View::drawWorld()
{
	int maxRow = model->getWorld()->getMaxRows();
	int maxCol = model->getWorld()->getMaxCols();

	glColor3f(0, 0, 1);	// couleur du labyrinthe
	for(int i = 0; i < maxRow; i++) 
	{
		for(int j = 0; j < maxCol; j++) 
		{
			if(i == 0 || j == 0 || i == maxRow-1 || j == maxCol-1) 
				continue;

			drawCell(*model->getWorld()->getMap()[i][j],i,j);
		}
	}
}

void View::drawCell(Cell cell, int i, int j) 
{
	int x = cell.getX();
	int y = cell.getY();
	float x1, y1;
	Cell*** map = model->getWorld()->getMap();

	cell = *model->getWorld()->getMap()[i][j];
	x = j * SCALE;
	y = i * SCALE;

	// pour centrer
	x -= (model->getWorld()->getMaxCols() / 2) * SCALE;
	y -= (model->getWorld()->getMaxRows() / 2) * SCALE;

	// si emplacement libre on dessine les murs nécessaires
	if((cell.getType() & EMPTY) == EMPTY)
	{
		// mur du haut
		if((map[i-1][j]->getType() & HARD) == HARD)
		{
			x1 = x;
			y1 = y;
			glPushMatrix();
			glTranslated(x1, -y1, 0);
			drawHLine();
			glPopMatrix();
		}

		// mur du bas
		if((map[i+1][j]->getType() & HARD) == HARD)
		{
			x1 = x;
			y1 = y + SCALE;
			glPushMatrix();
			glTranslated(x1, -y1, 0);
			drawHLine();
			glPopMatrix();
		}

		// mur de droite
		if((map[i][j+1]->getType() & HARD) == HARD)
		{
			x1 = x + SCALE;
			y1 = y + SCALE;
			glPushMatrix();
			glTranslated(x1, -y1, 0);
			drawWLine();
			glPopMatrix();
		}

		// mur de gauche
		if((map[i][j-1]->getType() & HARD) == HARD)
		{
			x1 = x;
			y1 = y + SCALE;
			glPushMatrix();
			glTranslated(x1, -y1, 0);
			drawWLine();
			glPopMatrix();
		}
	}
}

void View::drawScore() 
{
	glPushMatrix();
	stringstream txtScore;
	txtScore << "Score : " << model->getScore();
	glColor3f(1, 1, 1); // TODO : glColor3fv

	/* Antialiasing du texte */ 
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glEnable(GL_LINE_SMOOTH);

	for(unsigned int i = 0; i < txtScore.str().size(); i++)
	{
		glutStrokeCharacter(GLUT_STROKE_ROMAN, txtScore.str()[i]);
	}
	glPopMatrix();
}

void View::drawLifes() 
{
	glPushMatrix();
	glScaled(5,5,1);
	glTranslated(200, 10, 0);
	glColor3f(1.0, 1.0, 0.0); // Définit la couleur jaune de pacman
	for(int i = 1; i <= model->getLifes(); i++) 
	{
		glTranslated(25, 0, 0);
		glCallList(idList_PacMan_2D);
	}
	glPopMatrix();
}

void View::drawGhosts() 
{
	// Inutile d'implémenter drawGhosts ici
}

float * View::getNormal(float * p1, float * p2, float * p3){
	float xVertex[2];
	float yVertex[2];
	float zVertex[2];
	//float * normale = (float *) malloc(3*sizeof(float));
	float normale[3];

	xVertex[0] = p1[0]-p2[0];
	xVertex[1] = p2[0]-p3[0];
	yVertex[0] = p1[1]-p2[1];
	yVertex[1] = p2[1]-p3[1];
	zVertex[0] = p1[2]-p2[2];
	zVertex[1] = p2[2]-p3[2];
	normale[0] = yVertex[0]*zVertex[1] - zVertex[0]*yVertex[1];
	normale[1] = zVertex[0]*xVertex[1] - xVertex[0]*zVertex[1];
	normale[2] = xVertex[0]*yVertex[1] - yVertex[0]*xVertex[1];

	return normale;
}

void View::initLights()
{
}