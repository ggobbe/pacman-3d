#include "View2D.h"

View2D::View2D(void) : View()
{
}

View2D::~View2D() {
	delete instance;
	delete model;
}

void View2D::viewportWorld()
{
	glViewport (0, 50, w_global, h_global - 50); // Taille et positionnement de la viewport
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity(); // Initialisation de la matrice de projection

	// Caractéristiques de projection 2D dans la viewport
	if(w_global <= h_global) 
	{
		glOrtho(-50, 50, -50*h_global/w_global, 50*h_global/w_global, -1, 1);
	}
	else 
	{
		glOrtho(-50*w_global/h_global, 50*w_global/h_global, -50, 50, -1, 1);
	}

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity(); // Initialisation de la matrice de modelview
}

void View2D::drawCentralControl(GLenum selectMode) 
{
	// Inutile d'implémenter cette fonction pour la 2D
}

void View2D::drawPills()
{
	int x, y;
	int maxRows = Model::getInstance()->getWorld()->getMaxRows();
	int maxCols = Model::getInstance()->getWorld()->getMaxCols();
	Cell cell;
	for(int i = 0; i < maxRows; i++)
	{
		for(int j = 0; j < maxCols; j++)
		{
			if(i == 0 || i == (maxRows-1) || j == 0 || j == (maxCols-1)) 
				continue;

			cell = *model->getWorld()->getMap()[i][j];

			if(((cell.getType() & PILL) == PILL) || ((cell.getType() & SUPER_PILL) == SUPER_PILL))
			{
				glPushMatrix();
				x = j * SCALE;
				y = i * SCALE;

				// pour centrer
				x -= (model->getWorld()->getMaxCols() / 2) * SCALE;	// 4.5
				y -= (model->getWorld()->getMaxRows() / 2) * SCALE;	// 2.5

				x += 0.5 * SCALE;
				y += 0.5 * SCALE;

				GLdouble radius = 0.5;
				if((cell.getType() & SUPER_PILL) == SUPER_PILL) 
				{
					radius = 1.5;
				}

				// pastille
				glColor3f(0.8, 0.8, 0.8); // Définit la couleur des pastilles
				glTranslated(x, -y, 0);	// déplacement
				GLUquadricObj *disque;	// étape 1
				disque = gluNewQuadric();	// étape 1
				gluQuadricDrawStyle(disque, GLU_FILL);	// étape 2
				gluDisk(disque, 0, radius, 100, 1);
				glPopMatrix();
			}
		}
	}
}

void View2D::drawHLine() {
	glBegin(GL_LINES);
	glVertex2f(0, 0);
	glVertex2f(SCALE, 0);
	glEnd();
}

void View2D::drawWLine() {
	glBegin(GL_LINES);
	glVertex2f(0, 0);
	glVertex2f(0, SCALE);
	glEnd();
}

void View2D::drawPacMan() {
	// Smooth du pacman
	int delta = model->getPacMan()->getDelta();

	if(delta != 0) 
		delta -= SCALE;

	// on récupère les informations utiles pour dessiner pacman !
	double radius = model->getPacMan()->getRadius();
	double posX = (((model->getPacMan()->getPosX()-(model->getWorld()->getMaxCols() / 2))) * SCALE) + 2.5;
	double posY = (((model->getPacMan()->getPosY()-(model->getWorld()->getMaxRows() / 2))) * SCALE) + 2.5;
	posY = -posY;
	double start = 45 + (model->getPacMan()->getDir() * 90);
	double sweep = 270;

	int dying = model->getPacMan()->getDying();
	if(model->getPacMan()->getDying() >= 1)
	{
		//start += dying * 30;
		//sweep -= dying * 30;
		model->getPacMan()->setRadius(model->getPacMan()->getRadius() - 0.25);
		model->getPacMan()->setDying(dying + 1);
		
		if(model->getPacMan()->getDying() > 5)
		{
			model->getPacMan()->setDying(10);
		}
	}

	// Gestion du smooth
	switch(model->getPacMan()->getDir())
	{
	case PAC_NORTH:
		posY += delta;
		break;
	case PAC_EAST:
		posX += delta;
		break;
	case PAC_SOUTH:
		posY -= delta;
		break;
	case PAC_WEST:
		posX -= delta;
		break;
	}

	// si bouche fermée on change pour le partial disk
	if(!model->getPacMan()->isMouthOpen())
	{
		start -= 35;
		sweep = 340;
	}
	if(model->getPacMan()->isRunning())
		model->getPacMan()->switchMouth();

	// On dessine pac man à l'écran
	glPushMatrix();
	glColor3fv(model->getPacMan()->getColor()); // Définit la couleur jaune de pacman
	glTranslated(posX, posY, 0);	// déplacement
	GLUquadricObj *disque;	// étape 1
	disque = gluNewQuadric();	// étape 1
	gluQuadricDrawStyle(disque, GLU_FILL);	// étape 2
	gluPartialDisk(disque, 0, radius, 100, 1, start, sweep);
	glPopMatrix();
}

void View2D::drawGhosts() 
{
	// Si pas de fantômes on quitte
	if(Model::getInstance()->getWorld()->getNbGhosts() == 0)
		return;

	for(int i = 0; i < Model::getInstance()->getWorld()->getNbGhosts(); i++)
	{
		Ghost *g = Model::getInstance()->getGhost(i);

		float posX = (g->getPosX() * SCALE) -  ((model->getWorld()->getMaxCols() / 2) * SCALE) + g->getRadius();
		float posY = -((g->getPosY() * SCALE) - ((model->getWorld()->getMaxRows() / 2) * SCALE) + g->getRadius());

		int delta = g->getDelta();

		delta -= SCALE;

		// Gestion du smooth
		switch(g->getDir())
		{
		case PAC_NORTH:
			posY += delta;
			break;
		case PAC_EAST:
			posX += delta;
			break;
		case PAC_SOUTH:
			posY -= delta;
			break;
		case PAC_WEST:
			posX -= delta;
			break;
		}

		glPushMatrix();
		glColor3fv(g->getColor()); // TODO : changer avec le getColor
		glTranslated(posX, posY, 0);	// déplacement
		GLUquadricObj *disque;	// étape 1
		disque = gluNewQuadric();	// étape 1
		gluQuadricDrawStyle(disque, GLU_FILL);	// étape 2
		gluDisk(disque, 0, g->getRadius(), 100, 1);
		glPopMatrix();
	}
}