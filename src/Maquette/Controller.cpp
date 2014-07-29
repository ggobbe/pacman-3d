//Controller.cpp
#include "Controller.h"

// Les membres static non constants doivent êtres initialisés dans le .cpp
Controller *Controller::instance=0;

// Chaque call back glut est délégué à une méthode de la classe View ou Controller
// Ceci est nécessaire car glut ne connait que les fonctions C.
void animateDisplayCallBack(int val)
{
	Controller::getInstance()->animateDisplay();
}

// Implémentation classique du pattern singleton
Controller::Controller() 
{
	model = Model::getInstance();
	
	View::getInstance()->setInstance(new View3D());
	view = View::getInstance();

	timeGhostInPrison = new clock_t[model->getWorld()->getNbGhosts()];

	glutTimerFunc(TEMPSBASE,animateDisplayCallBack,false); // Lance le "controleur" de l'animation
	stepOK = 0;
}

Controller::~Controller()
{
	delete model;
	delete view;
}

Controller *Controller::getInstance()
{
	if (instance == 0)
		instance = new Controller();

	return instance;
}

void Controller::togglePacManViewMode() {
	model->setPacManViewMode(!model->isInPacManViewMode());
}

void Controller::changeDirGhost(Ghost* g, int dir, int nbAvailables, int* availables) {
	int nextDir; // Sert à déterminer la position suivante si nécessaire
	switch(dir) {
	case PAC_SOUTH:
		nextDir = PAC_NORTH;
		break;
	case PAC_NORTH:
		nextDir = PAC_SOUTH;
		break;
	case PAC_EAST:
		nextDir = PAC_WEST;
		break;
	case PAC_WEST:
		nextDir = PAC_EAST;
		break;
	}
	bool dontChange = false; // indique si le ghost doit changer de direction ou pas, si true il ne change pas sinon il peut changer
	if(g->getDir() == nextDir)
	{
		for(int k = 0; k < nbAvailables; k++)
		{
			if(availables[k] == nextDir)
			{
				dontChange = true;
				break;
			}
		}
	}

	if(!dontChange)
		g->setDir(dir);
}

void Controller::handleCollisions(int numGhost, Ghost* g) {
	// Y a-t-il un fantôme à cet endroit? Si oui die()
	PacMan * p = model->getPacMan();
			
	if(g->getPosX() == p->getPosX() && g->getPosY() == p->getPosY()) 
	{
		if(model->getPacMan()->getMode())
		{
			// On le renvoie à sa position de départ
			g->setRunning(false);
			g->setDelta(SCALE);
			g->setPosX(g->getInitPosX());
			g->setPosY(g->getInitPosY());

			// On augmente le score de SCORE_GHOST
			model->incScore(SCORE_GHOST);
			timeGhostInPrison[numGhost] = clock();
		}
		else if(!(p->getPosX() == p->getInitPosX() && p->getPosY() == p->getInitPosY()))
		{
			model->pacManDie();

			// On remet les fantômes à leurs positions de départ lorsque pacman meurt
			for(int i=0; i < model->getWorld()->getNbGhosts(); i++) 
			{
				Ghost* ghost = model->getGhost(i);
				ghost->setPosX(ghost->getInitPosX());
				ghost->setPosY(ghost->getInitPosY());
				ghost->setRunning(false);
				timeGhostInPrison[i] = clock();
				ghost->setDelta(SCALE);
			}
		}
	}
}

void Controller::animateGhosts()
{				
	if(model->getWorld()->getNbGhosts() == 0) 
		return;

	for(int i = 0; i < model->getWorld()->getNbGhosts(); i++)
	{
		Ghost * g = model->getGhost(i);

		if(((clock()-timeGhostInPrison[i]) / CLOCKS_PER_SEC) > GHOSTS_PRISON_TIME) {
			g->setRunning(true);
		}

		if(g->isRunning()) {
			// Smooth
			g->setDelta(g->getDelta() + 1);

			// On ne dépasse pas le SCALE
			if(g->getDelta() >= SCALE)
				g->setDelta(0);

			if(g->getDelta() == 0)
			{
				int x = g->getPosX();
				int y = g->getPosY();

				int available = 0;

				int * availables = new int[4];
				int nbAvailables = 0;


				// à droite
				if(x + 1 < model->getWorld()->getMaxCols() && (model->getWorld()->getMap()[y][x+1]->getType() & EMPTY) == EMPTY)
				{
					availables[nbAvailables++] = PAC_EAST;
				}
				// à gauche
				if(x - 1 > 0 && (model->getWorld()->getMap()[y][x-1]->getType() & EMPTY) == EMPTY)
				{
					availables[nbAvailables++] = PAC_WEST;
				}
				// en haut
				if(y - 1 > 0 && (model->getWorld()->getMap()[y-1][x]->getType() & EMPTY) == EMPTY)
				{
					availables[nbAvailables++] = PAC_NORTH;
				}
				// en bas
				if(y + 1 < model->getWorld()->getMaxRows() && (model->getWorld()->getMap()[y+1][x]->getType() & EMPTY) == EMPTY)
				{
					availables[nbAvailables++] = PAC_SOUTH;
				}

				// améliorer le déplacement des fantômes
				int newDir = availables[rand() % nbAvailables];

				this->changeDirGhost(g, newDir, nbAvailables, availables);

				// On change la position du ghost
				switch(g->getDir())
				{
				case PAC_NORTH:
					g->setPosY(y-1);
					break;
				case PAC_EAST:
					g->setPosX(x+1);
					break;
				case PAC_SOUTH:
					g->setPosY(y+1);
					break;
				case PAC_WEST:
					g->setPosX(x-1);
					break;
				}
			}

			this->handleCollisions(i, g);
		}
	}
}

void Controller::handleStargate(int iX, int iY, int maxRow, int maxCol, int* direction) {
	stargate = false;

	if(!stargate && (iX == 0 || iY == 0)) 
	{
		model->getPacMan()->setRunning(false);
		*direction=PAC_EAST;
	}
	if(!stargate && (iX == maxCol-1 || iY == maxRow-1)) 
	{
		model->getPacMan()->setRunning(false);
		*direction=PAC_WEST; 
	}

	if(stargate && (iX == 1 || iY == 1)) 
	{
		stargate = false;
	}
	if(stargate && (iX == maxCol-2 || iY == maxRow-2)) 
	{
		stargate = false;
	}
}

void Controller::changeDirPacMan(int nextDir, bool changeDir) {
	int iX = model->getPacMan()->getPosX();
	int iY = model->getPacMan()->getPosY();
	Cell*** map = model->getWorld()->getMap();

	switch(nextDir) 
	{
		case PAC_NORTH: 
			if((map[iY-1][iX]->getType() & EMPTY) == EMPTY)
				changeDir = true;
			break;
		case PAC_EAST:
			if((map[iY][iX+1]->getType() & EMPTY) == EMPTY)
				changeDir = true;
			break;
		case PAC_WEST:
			if((map[iY][iX-1]->getType() & EMPTY) == EMPTY)
				changeDir = true;
			break;
		case PAC_SOUTH:
			if((map[iY+1][iX]->getType() & EMPTY) == EMPTY)
				changeDir = true;
			break;
	}

	if(changeDir) 
	{
		model->getPacMan()->setDir(nextDir);
		model->getPacMan()->setNextDir(-1);
	}
}

void Controller::handleChangeDirPacMan() {
	float step = 1; // déplacement du pacman
	int iX = model->getPacMan()->getPosX();
	int iY = model->getPacMan()->getPosY();

	int direction = model->getPacMan()->getDir();
	int nextDir = model->getPacMan()->getNextDir();

	int maxRow = model->getWorld()->getMaxRows();
	int maxCol = model->getWorld()->getMaxCols();

	this->handleStargate(iX, iY, maxRow, maxCol, &direction);

	Cell*** map = model->getWorld()->getMap();

	if(model->getPacMan()->getDelta() == 0)
	{
		switch(direction)
		{
		case PAC_NORTH:
			if((map[iY-1][iX]->getType() & EMPTY) == EMPTY) {
				if((iY-1) == 0) { // téléportation s'il y a bien un trou dans le mur de chaque côté
					if((map[maxRow-1][iX]->getType() & EMPTY) == EMPTY) 
					{
						model->getPacMan()->setPosY(maxRow-1);
						stargate = true;
					}
				}
				model->getPacMan()->setPosY(model->getPacMan()->getPosY() - step);
				//model->incCamPosX(step);
			} else
				model->getPacMan()->setRunning(false);
			break;
		case PAC_EAST:
			if((map[iY][iX+1]->getType() & EMPTY) == EMPTY) 
			{
				if((iX+2) == maxCol) { // téléportation s'il y a bien un trou dans le mur de chaque côté
					if((map[iY][0]->getType() & EMPTY) == EMPTY) 
					{
						model->getPacMan()->setPosX(0);
						stargate = true;
					}
				}
				model->getPacMan()->setPosX(model->getPacMan()->getPosX() + step);
			}
			else 
			{
				model->getPacMan()->setRunning(false);
			}
			break;
		case PAC_SOUTH:
			if((map[iY+1][iX]->getType() & EMPTY) == EMPTY) 
			{
				if((iY+2) == maxRow) { // téléportation s'il y a bien un trou dans le mur de chaque côté
					if((map[0][iX]->getType() & EMPTY) == EMPTY) 
					{
						model->getPacMan()->setPosY(0);
						stargate = true;
					}
				}
				model->getPacMan()->setPosY(model->getPacMan()->getPosY() + step);
			}
			else 
			{
				model->getPacMan()->setRunning(false);
			}
			break;
		case PAC_WEST:
			if((map[iY][iX-1]->getType() & EMPTY) == EMPTY) 
			{
				if((iX-1) == 0) { // téléportation s'il y a bien un trou dans le mur de chaque côté
					if((map[iY][maxCol-1]->getType() & EMPTY) == EMPTY) 
					{
						model->getPacMan()->setPosX(maxCol-1);
						stargate = true;
					}
				}

				model->getPacMan()->setPosX(model->getPacMan()->getPosX() - step);
			}
			else
			{
				model->getPacMan()->setRunning(false);
			}
			break;
		}
	}
	
	bool changeDir = false;
	// On change de direction si possible et que la direction souhaitée est disponible
	if(model->getPacMan()->getDelta() == (SCALE - 1) && nextDir != -1 && nextDir != direction) 
	{
		this->changeDirPacMan(nextDir, changeDir);
	}
}

void Controller::switchSuperMode() {
	/* PacMan en mode normal / super */
	if(model->getPacMan()->getMode() == SUPER)
	{
		model->getPacMan()->couleurAleatoire();
		if(((clock()-timeModeSuper) / CLOCKS_PER_SEC) > SUPER_TIME)
			model->getPacMan()->animSuperMode(NORMAL);
	}
}

void Controller::handlePills() {
	int nextX = model->getPacMan()->getPosX();
	int nextY = model->getPacMan()->getPosY();
	Cell*** map = model->getWorld()->getMap();

	//On simplifie le traitement des pills en ne le faisant qu'une fois
	if(model->getPacMan()->isRunning() && ((map[nextY][nextX]->getType() & PILL) == PILL || (map[nextY][nextX]->getType() & SUPER_PILL) == SUPER_PILL)) 
	{
		if(map[nextY][nextX]->getType() == SUPER_PILL) 
		{
			model->incScore(SCORE_SUPER_PILL);
			int nbSuperPills = model->getWorld()->getNbSuperPills();
			model->getWorld()->setNbSuperPills(nbSuperPills - 1);
			timeModeSuper = clock();
			model->getPacMan()->animSuperMode(SUPER);
		}
		else 
		{
			model->incScore(SCORE_PILL);
			int nbPills = model->getWorld()->getNbPills();
			model->getWorld()->setNbPills(nbPills - 1);
		}
		map[nextY][nextX]->setType(EMPTY);
	}

	//Si toutes les pills ont été mangées par le PacMan, on passe au niveau suivant
	int nbPills = model->getWorld()->getNbPills();
	int nbSuperPills = model->getWorld()->getNbSuperPills();
	if(nbPills == 0 && nbSuperPills == 0) 
	{
		model->getPacMan()->setRunning(false);
		model->nextLevel();
	}
}

void Controller::handlePacManSmooth() {
	// Smooth pacman move
	if(model->getPacMan()->isRunning()) 
	{
		model->getPacMan()->setDelta(model->getPacMan()->getDelta() + 1);

		// On ne dépasse pas le SCALE
		if(model->getPacMan()->getDelta() >= SCALE)
			model->getPacMan()->setDelta(0);
	} 
	else 
	{
		model->getPacMan()->setDelta(0);
	}
}

// Déplacement du PacMan
void Controller::animateDisplay() 
{
	if(model->isStarted()) {
		// Mouvement des fantômes
		animateGhosts();
	}
	switchSuperMode();

	if(model->getPacMan()->isRunning() && model->isStarted()) 
	{
		bool changeDir = false;
		this->handleChangeDirPacMan();
		this->handlePills();
		this->handlePacManSmooth();
	}

	View::getInstance()->display(); // Rafraichissement de l'affichage
	glutTimerFunc(TEMPSBASE,animateDisplayCallBack,false); // Rappel du "controleur" de l'animation (cadence = TEMPBASE)
	// Attention de ne pas lancer plusieurs "controleurs" de la même animation en parallèle !!!
}

void Controller::handleNewDir(int newDir) {
	if(newDir != -1)
	{
		if(model->getLifes() >= 0) {
			model->startGame();	// si le jeu n'avais pas démarré on le démarre
		}

		if(!model->getPacMan()->isRunning()) 
		{
			model->getPacMan()->setRunning(true);
			model->getPacMan()->setDir(newDir);
		}

		model->getPacMan()->setNextDir(newDir);	// On dit qu'on va changer de direction
	}
}

/*
 * Appellée lors d'actions à la souris pour la viewport contenant les contrôles souris
 */
void Controller::mouseRight(int button, int state, float x, float y, int* viewport) {
    int gap = 5;
	gap+=180;
	int largeur = 20;
	largeur+=180;
	int hauteur = 15;
	int h_global = view->getHGlobal();
	hauteur+= h_global - 20;

	int newDir = -1;

	GLint d1 = view->getD1();
	GLint d2 = view->getD2();
	GLint delta1 = view->getDelta1();
	GLint delta2 = view->getDelta2();
	
	GLint vp1 = viewport[0];
	GLint vp2 = viewport[1];
	GLdouble w_viewport = (double) viewport[2];
	GLdouble h_viewport = (double) viewport[3];
	GLdouble x_viewport = (double) (x - vp1);
	GLdouble y_viewport = (double) (h_global - vp2 - y);

	double x_dessin = d1 + ((x_viewport * delta1) / w_viewport);
	x_dessin -= 301;
	double y_dessin = d2 + ((y_viewport * delta2) / h_viewport);

	cout << "(" << x_dessin << " ; " << y_dessin << ")" << endl;

	switch (button)
	{
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN) 
		{
			/* contrôle sur le bouton view */
			if((x_dessin >= BTN_VIEW_X_LEFT && x_dessin <= BTN_VIEW_X_RIGHT) && (y_dessin >= BTN_VIEW_Y_DOWN && y_dessin <= BTN_VIEW_Y_UP)) {
				model->setProjectionMode(!model->getProjectionMode());
			}
			/* contrôle sur le bouton reset */
			if((x_dessin >= BTN_RESET_X_LEFT && x_dessin <= BTN_RESET_X_RIGHT) && (y_dessin >= BTN_RESET_Y_DOWN && y_dessin <= BTN_RESET_Y_UP)) {
				model->getPacMan()->setRunning(false);
				model->reset();
			}

			/* contrôle sur le bouton close */
			if((x_dessin >= BTN_CLOSE_X_LEFT && x_dessin <= BTN_CLOSE_X_RIGHT) && (y_dessin >= BTN_CLOSE_Y_DOWN && y_dessin <= BTN_CLOSE_Y_UP))  {
				exit(EXIT_SUCCESS);
			}

			/* contrôle sur les flèches directionnelles */

			if((x_dessin >= BTN_WEST_X_LEFT && x_dessin <= BTN_WEST_X_RIGHT) && (y_dessin >= BTN_WEST_Y_DOWN && y_dessin <= BTN_WEST_Y_UP)) 
			{
				newDir = PAC_WEST;
			}
			else if((x_dessin >= BTN_SOUTH_X_LEFT && x_dessin <= BTN_SOUTH_X_RIGHT) && (y_dessin >= BTN_SOUTH_Y_DOWN && y_dessin <= BTN_SOUTH_Y_UP)) 
			{
				newDir = PAC_SOUTH;
			}
			else if((x_dessin >= BTN_EAST_X_LEFT && x_dessin <= BTN_EAST_X_RIGHT) && (y_dessin >= BTN_EAST_Y_DOWN && y_dessin <= BTN_EAST_Y_UP)) 
			{
				newDir = PAC_EAST;
			}
			else if((x_dessin >= BTN_NORTH_X_LEFT && x_dessin <= BTN_NORTH_X_RIGHT) && (y_dessin >= BTN_NORTH_Y_DOWN && y_dessin <= BTN_NORTH_Y_UP)) 
			{
				newDir = PAC_NORTH;
			}
		}
		break;
	case GLUT_MIDDLE_BUTTON:
		if(state == GLUT_DOWN) {
			model->zoomInOut(-ZOOM_DEFAULT);
		}
		break;
	default:
		break;
	}

	this->handleNewDir(newDir);
}

/*
 * Appellée lors d'actions à la souris pour la viewport du milieu
 */
void Controller::mouseMiddle(float x, float y, int* viewport) {
        unsigned selectBuf[BUFSIZE];
        int hits;
        glSelectBuffer(BUFSIZE, selectBuf);

        glRenderMode(GL_SELECT);
        glInitNames();
        glPushName(0); //évite GL_INVALID_OPERATION
        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();
        gluPickMatrix((float) x, (float) y, 5, 5, viewport);
		view->viewportWorld();
       
		view->drawCentralControl(GL_SELECT);
        glPopMatrix();
        glFlush();
		
        hits = glRenderMode(GL_RENDER);
        
        if (hits == 1) { //si la forme middlePoint est clickée par la souris
			model->toggleSpinning();
        }
}


/**
 * Gestion de l'interaction entre l'utilisateur et l'application grâce à la souris
 * dans la viewportMiddle
 */
void Controller::handlePicking(int button, int state, int x, int y) {
	    int viewportMiddle[4];
        int viewportRight[4];
        if (button != GLUT_LEFT_BUTTON || state != GLUT_DOWN) {
                return;
        }
        glGetIntegerv(GL_VIEWPORT, viewportMiddle); //la dernière viewport dessinée est la viewport du milieu

		viewportRight[0] = view->getWGlobal() - (view->getWGlobal() * TAILLE_X_VIEWPORT_WORLD);
        viewportRight[1] = view->getHGlobal() * TAILLE_Y_VIEWPORT_INFOS;
		viewportRight[2] = view->getWGlobal() * TAILLE_X_VIEWPORT_CONTROLS;
        viewportRight[3] = viewportMiddle[3];

        int realY = view->getHGlobal() - y;		// on calcule le y réel en fonction de la hauteur de la fenêtre

		if (realY > viewportRight[1] && x > (view->getWGlobal() - viewportMiddle[2])) { //right
                mouseRight(button, state, x, y, viewportRight);
        } else if(realY > viewportRight[1]) { //middle
                mouseMiddle(x, y, viewportMiddle);
        }
}

void Controller::handleSwitchDimension() {
	int h_glob = view->getHGlobal();
	int w_glob = view->getWGlobal();

	if(dynamic_cast<View3D*>(view)) {
		view->setInstance(new View2D());
	}
	else if(dynamic_cast<View2D*>(view)){
		view->setInstance(new View3D());
	}
	view = View::getInstance();
	view->setHGlobal(h_glob);
	view->setWGlobal(w_glob);
}

// Gestion du clavier étendu
void Controller::special(int key, int x, int y)
{
	// Mettre ici tout traitement désiré suite à l'appui sur des touches du clavier étendu

	int newDir = -1;

	switch(key) 
	{
	case GLUT_KEY_DOWN:
		// vers le bas;
		newDir = PAC_SOUTH;
		break;
	case GLUT_KEY_UP: 
		// vers le haut
		newDir = PAC_NORTH;
		break;
	case GLUT_KEY_LEFT:
		// gauche
		newDir = PAC_WEST;
		break;
	case GLUT_KEY_RIGHT:
		// droite
		newDir = PAC_EAST;
		break;
	case GLUT_KEY_F1:
		// Insert
		model->incCamPosX(SCALE);
		break;
	case GLUT_KEY_F2:
		model->incCamPosX(-SCALE);
		break;
	case GLUT_KEY_F3:
		model->zoomInOut(ZOOM_DEFAULT);
		break;
	case GLUT_KEY_F4:
		model->zoomInOut(-ZOOM_DEFAULT);
		break;
	case GLUT_KEY_F5:
		this->togglePacManViewMode();
		break;
	case GLUT_KEY_F6:
		this->handleSwitchDimension();
		break;
	case GLUT_KEY_HOME:
		model->incCamPosY(SCALE);
		break;
	case GLUT_KEY_END:
		model->incCamPosY(-SCALE);
		break;
	case GLUT_KEY_PAGE_UP:
		model->incCamPosZ(SCALE);
		break;
	case GLUT_KEY_PAGE_DOWN:
		model->incCamPosZ(-SCALE);
		break;
	case GLUT_KEY_INSERT:
		model->reinitZoom();
		model->reinitPosCam();
		break;
	}

	this->handleNewDir(newDir);
}
