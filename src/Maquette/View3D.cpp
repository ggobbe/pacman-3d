#include "View3D.h"

View3D::View3D(void) : View()
{
	time = 0.0;
}

View3D::~View3D()
{
	delete model;
}

View3D* View3D::getInstance() {
	return (View3D*) instance;
}

void View3D::viewportWorld() 
{
	int heightInfos = h_global*TAILLE_Y_VIEWPORT_INFOS;
	int widthControls = w_global*TAILLE_X_VIEWPORT_CONTROLS;
	glViewport (0, heightInfos, w_global - widthControls, h_global - heightInfos); // Taille et positionnement de la viewport
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity(); // Initialisation de la matrice de projection
	int zoom = model->getZoom();
	if(model->isSpinning()) 
	{
		time+=1;
	}
	glRotated(time, 0, 0, 1);

	if(w_global == 0) 
		w_global = 1;
	if(h_global == 0) 
		h_global = 1;

	//Caractéristiques de projection 2D dans la viewport
	if(w_global <= h_global) 
	{
		d1World =-zoom;
		delta1World = zoom - d1World; 
		d2World = -zoom*h_global/w_global;
		delta2World = zoom*h_global/w_global - d2World;
	}
	else 
	{
		d1World =-zoom*w_global/h_global;
		delta1World = zoom*w_global/h_global - d1World; 
		d2World = -zoom;
		delta2World = zoom - d2World;
	}


	if(model->getProjectionMode() == PARALLEL_PROJECTION) 
	{
		glOrtho(d1World, d1World + delta1World, d2World, d2World + delta2World, 1, 251);
	}
	else if(model->getProjectionMode() == PERSPECTIVE_PROJECTION) 
	{
		double fieldView = zoom;
		double aspectRatio = (double) w_global/h_global;
		double near = 0.1;
		double far = 500.0;
		gluPerspective(fieldView, aspectRatio, near, far);
	}

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity(); // Initialisation de la matrice de modelview
	double posXPacMan = (((model->getPacMan()->getPosX()-(model->getWorld()->getMaxCols() / 2))) * SCALE) + 2.5;
	double posYPacMan = (((model->getPacMan()->getPosY()-(model->getWorld()->getMaxRows() / 2))) * SCALE) + 2.5;
	double radius = model->getPacMan()->getRadius();

	// delta pour la position de la caméra
	int deltaCamX = 0;
	int deltaCamY = 0;

	// delta pour savoir où la caméra regarde
	int deltaPacX = 0;
	int deltaPacY = 0;

	// orientation de la caméra
	float orientation_x = 0;
	float orientation_y = 0;

	int delta = model->getPacMan()->getDelta();

	if(delta == 0) delta = SCALE;

	switch(model->getPacMan()->getDir()) 
	{
	case PAC_NORTH: 
		deltaCamY = -FPS + delta;
		deltaPacY = -delta;
		orientation_y = 1;
		break;
	case PAC_SOUTH:	// OK
		deltaCamY = FPS - delta;
		deltaPacY = delta;
		orientation_y = -1;
		break;
	case PAC_EAST: 
		deltaCamX = -FPS + delta;
		deltaPacX = -delta;
		orientation_x = 1;
		break;
	case PAC_WEST: // OK
		deltaCamX = FPS - delta;
		deltaPacX = delta;
		orientation_x = -1;
		break;
	}

	if(model->isInPacManViewMode() && model->getProjectionMode() == PERSPECTIVE_PROJECTION)
	{
		//TODO
		gluLookAt(posXPacMan + deltaCamX, -posYPacMan + deltaCamY, FPS, posXPacMan - deltaPacX, -posYPacMan - deltaPacY, radius, orientation_x, orientation_y, 0);
		//gluLookAt(-posXPacMan, -posYPacMan, 5, 0, 0, 0, 0, 0, 1);
	}
	else 
	{
		gluLookAt(model->getCamPosX(), model->getCamPosY(), model->getCamPosZ(), 0, 0, 0, 1, 1, 1);
	}
}


/**
* Dessine une sphère au centre du labyrinthe permettant au joueur de cliquer pour commencer la rotation de celui-ci
*/
void View3D::drawCentralControl(GLenum selectMode) {
	if (selectMode == GL_SELECT) {
		glLoadName(1);
	}
	glPushMatrix();
		glColor3f(1, 0, 0);
		glTranslated(0, 0, 1);
		glutSolidSphere(SCALE / 4.0, 10, 10);
	glPopMatrix();
}

void View3D::drawPills() {
	int x, y;
	int maxRows = model->getWorld()->getMaxRows();
	int maxCols = model->getWorld()->getMaxCols();
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
				glTranslated(x, -y, radius);	// déplacement

				if((cell.getType() & SUPER_PILL) == SUPER_PILL) 
				{
					glCallList(idList_SuperPill);
				}
				else {
					glCallList(idList_Pill);
				}

				glPopMatrix();
			}
		}
	}
}

void View3D::drawHLine() {
	// Quelques vecteurs pour les points des rectangles
	float a[3];
	float b[3];
	float c[3];
	float d[3];

	// création de la liste pour le dessin des murs horizontaux
	glBegin(GL_POLYGON); // petite face gauche OK
	a[0] = 0, a[1] = SCALE*WALL_DEPTH_RATIO, a[2] = 0;
	b[0] = 0, b[1] = SCALE*WALL_DEPTH_RATIO, b[2] = SCALE;
	c[0] = 0, c[1] = 0, c[2] = SCALE;
	d[0] = 0, d[1] = 0, d[2] = 0;
	glVertex3fv(a);	glVertex3fv(b);	glVertex3fv(c);	glVertex3fv(d);
	glNormal3fv(View::getNormal(c, b, a));	// Normale
	glEnd();

	glBegin(GL_POLYGON); // grande face avant OK
	a[0] = 0, a[1] = SCALE*WALL_DEPTH_RATIO, a[2] = 0;
	b[0] = 0, b[1] = SCALE*WALL_DEPTH_RATIO, b[2] = SCALE;
	c[0] = SCALE+(SCALE*WALL_DEPTH_RATIO), c[1] = SCALE*WALL_DEPTH_RATIO, c[2] = 0;
	d[0] = SCALE+(SCALE*WALL_DEPTH_RATIO), d[1] = SCALE*WALL_DEPTH_RATIO, d[2] = SCALE;
	glVertex3fv(d);	glVertex3fv(b);	glVertex3fv(a);	glVertex3fv(c);
	glNormal3fv(View::getNormal(c, a, b));	// Normale
	glEnd();

	glBegin(GL_POLYGON); // petite face droite OK
	a[0] = SCALE+(SCALE*WALL_DEPTH_RATIO), a[1] = SCALE*WALL_DEPTH_RATIO, a[2] = 0;
	b[0] = SCALE+(SCALE*WALL_DEPTH_RATIO), b[1] = 0, b[2] = 0;
	c[0] = SCALE+(SCALE*WALL_DEPTH_RATIO), c[1] = 0, c[2] = SCALE;
	d[0] = SCALE+(SCALE*WALL_DEPTH_RATIO), d[1] = SCALE*WALL_DEPTH_RATIO, d[2] = SCALE;
	glVertex3fv(a);	glVertex3fv(b);	glVertex3fv(c);	glVertex3fv(d);
	glNormal3fv(View::getNormal(c, b, a));	// Normale
	glEnd();

	glBegin(GL_POLYGON); // petite face haut OK
	a[0] = SCALE+(SCALE*WALL_DEPTH_RATIO), a[1] = SCALE*WALL_DEPTH_RATIO, a[2] = SCALE;
	b[0] = SCALE+(SCALE*WALL_DEPTH_RATIO), b[1] = 0, b[2] = SCALE;
	c[0] = 0, c[1] = 0, c[2] = SCALE;
	d[0] = 0, d[1] = SCALE*WALL_DEPTH_RATIO, d[2] = SCALE;
	glVertex3fv(a);	glVertex3fv(b);	glVertex3fv(c);	glVertex3fv(d);
	glNormal3fv(View::getNormal(c, b, a));	// Normale
	glEnd();

	glBegin(GL_POLYGON); // grande face arrière OK
	a[0] = 0, a[1] = 0, a[2] = 0;
	b[0] = SCALE+(SCALE*WALL_DEPTH_RATIO), b[1] = 0, b[2] = 0;
	c[0] = SCALE+(SCALE*WALL_DEPTH_RATIO), c[1] = 0, c[2] = SCALE;
	d[0] = 0, d[1] = 0, d[2] = SCALE;
	glVertex3fv(d);	glVertex3fv(c);	glVertex3fv(b);	glVertex3fv(a);
	glNormal3fv(View::getNormal(c, b, a));	// Normale
	glEnd();
}

void View3D::drawWLine() {
	// Quelques vecteurs pour les points des rectangles
	float a[3];
	float b[3];
	float c[3];
	float d[3];

	glBegin(GL_POLYGON); // grande face gauche OK
	a[0] = 0, a[1] = 0, a[2] = SCALE;
	b[0] = 0, b[1] = SCALE, b[2] = SCALE;
	c[0] = 0, c[1] = SCALE, c[2] = 0;
	d[0] = 0, d[1] = 0, d[2] = 0;
	glVertex3fv(d);	glVertex3fv(c);	glVertex3fv(b);	glVertex3fv(a);
	glNormal3fv(View::getNormal(a, b, c));	// Normale
	glEnd();
	glBegin(GL_POLYGON); // petite face haut OK
	a[0] = 0, a[1] = 0, a[2] = SCALE;
	b[0] = SCALE*WALL_DEPTH_RATIO, b[1] = 0, b[2] = SCALE;
	c[0] = SCALE*WALL_DEPTH_RATIO, c[1] = SCALE, c[2] = SCALE;
	d[0] = 0, d[1] = SCALE, d[2] = SCALE;
	glVertex3fv(d);	glVertex3fv(c);	glVertex3fv(b);	glVertex3fv(a);
	glNormal3fv(View::getNormal(a, b, c));	// Normale
	glEnd();
	glBegin(GL_POLYGON); // grande face droite OK
	a[0] = SCALE*WALL_DEPTH_RATIO, a[1] = 0, a[2] = SCALE;
	b[0] = SCALE*WALL_DEPTH_RATIO, b[1] = 0, b[2] = 0;
	c[0] = SCALE*WALL_DEPTH_RATIO, c[1] = SCALE, c[2] = 0;
	d[0] = SCALE*WALL_DEPTH_RATIO, d[1] = SCALE, d[2] = SCALE;
	glVertex3fv(d);	glVertex3fv(c);	glVertex3fv(b);	glVertex3fv(a);
	glNormal3fv(View::getNormal(a, b, c));	// Normale
	glEnd();
	glBegin(GL_POLYGON); // petite face droite OK
	a[0] = SCALE*WALL_DEPTH_RATIO, a[1] = SCALE, a[2] = SCALE;
	b[0] = SCALE*WALL_DEPTH_RATIO, b[1] = SCALE, b[2] = 0;
	c[0] = 0, c[1] = SCALE, c[2] = 0;
	d[0] = 0, d[1] = SCALE, d[2] = SCALE;
	glVertex3fv(d);	glVertex3fv(c);	glVertex3fv(b);	glVertex3fv(a);
	glNormal3fv(View::getNormal(a, b, c));	// Normale
	glEnd();
	glBegin(GL_POLYGON); // petite face gauche OK
	a[0] = 0, a[1] = 0, a[2] = 0;
	b[0] = SCALE*WALL_DEPTH_RATIO, b[1] = 0, b[2] = 0;
	c[0] = SCALE*WALL_DEPTH_RATIO, c[1] = 0, c[2] = SCALE;
	d[0] = 0, d[1] = 0, d[2] = SCALE;
	glVertex3fv(d);	glVertex3fv(c);	glVertex3fv(b);	glVertex3fv(a);
	glNormal3fv(View::getNormal(a, b, c));	// Normale
	glEnd();
}

void View3D::drawTopPacman(double r, int lats, int longs) 
{
	int i, j;
	for(i = 1; i <= lats; i++) 
	{
		double lat0 = PI * (-0.5 + (double) (i - 1) / lats);
		double z0  = r*sin(lat0);
		double zr0 =  r*cos(lat0);

		double lat1 = PI * (-0.5 + (double) i / lats);
		double z1 = r*sin(lat1);
		double zr1 = r*cos(lat1);

		glBegin(GL_QUAD_STRIP);
		for(j = 0; j <= longs-6; j++) 
		{
			double lng = 2*PI * (double) (j - 1) / longs/2;
			double x = r*cos(lng);
			double y = r*sin(lng);

			glNormal3f(x * zr0, y * zr0, r*z0);
			glVertex3f(x * zr0, y * zr0, r*z0);
			glNormal3f(x * zr1, y * zr1, r*z1);
			glVertex3f(x * zr1, y * zr1, r*z1);
		}
		glEnd();
	}
}

void View3D::drawBottomPacman(double r, int lats, int longs) 
{
	int i, j;
	for(i = 1; i <= lats; i++) 
	{
		double lat0 = PI * (-0.5 + (double) (i - 1) / lats);
		double z0  = r*sin(lat0);
		double zr0 =  r*cos(lat0);

		double lat1 = PI * (-0.5 + (double) i / lats);
		double z1 = r*sin(lat1);
		double zr1 = r*cos(lat1);

		glBegin(GL_QUAD_STRIP);
		for(j = 1; j <= longs-5; j++) 
		{
			double lng = -2*PI* (double) (j - 1) / longs/2;
			double x = r*cos(lng);
			double y = r*sin(lng);

			glNormal3f(x * zr0, y * zr0, r*z0);
			glVertex3f(x * zr0, y * zr0, r*z0);
			glNormal3f(x * zr1, y * zr1, r*z1);
			glVertex3f(x * zr1, y * zr1, r*z1);
		}
		glEnd();
	}
}

// Dessine un carré ayant subi une certaine rotation autour de son centre
void View3D::drawPacMan()
{
	// Smooth du pacman
	int delta = model->getPacMan()->getDelta();

	if(delta != 0) 
		delta -= SCALE;

	// on récupère les informations utiles pour dessiner pacman !
	double posX = (((model->getPacMan()->getPosX()-(model->getWorld()->getMaxCols() / 2))) * SCALE) + 2.5;
	double posY = (((model->getPacMan()->getPosY()-(model->getWorld()->getMaxRows() / 2))) * SCALE) + 2.5;
	posY = -posY;
	double start = 45 + (model->getPacMan()->getDir() * 90);
	double sweep = 270;

	int dying = model->getPacMan()->getDying();
	if(model->getPacMan()->getDying() >= 1)
	{
		model->getPacMan()->setRadius(model->getPacMan()->getRadius() - 0.25);
		model->getPacMan()->setDying(dying + 1);

		if(model->getPacMan()->getDying() > 5)
		{
			model->getPacMan()->setDying(10);
		}
	}

	// Gestion du smooth
	static int angleRotPacManX = 90;
	static int angleRotPacManZ = 0;

	switch(model->getPacMan()->getDir())
	{
	case PAC_NORTH:
		posY += delta;
		angleRotPacManZ = 270;
		break;
	case PAC_EAST:
		posX += delta;
		angleRotPacManZ = 180;
		break;
	case PAC_SOUTH:
		posY -= delta;
		angleRotPacManZ = 90;
		break;
	case PAC_WEST:
		posX -= delta;
		angleRotPacManZ = 0;
		break;
	}

	double radius = model->getPacMan()->getRadius();
	glPushMatrix();
	int topMouthAngle = 58;
	int bottomMouthAngle = 116;
	static int longsTop = 40;
	static int longsBottom = 40;

	// Mouvement de la bouche
	if(longsTop < 40) {
		longsTop += 10;
		longsBottom += 10;
	}
	else {
		longsTop -= 10;
		longsBottom -= 10;
	}

	//On trace le haut et le bas du PacMan
	glTranslated(posX, posY, radius);	// déplacement
	glTranslated(0,0,0.5);
	glScaled(0.6,0.6,0.6);
	glPushMatrix();
	glColor3fv(model->getPacMan()->getColor());
	glRotatef(angleRotPacManZ, 0.0,0.0,1.0);
	glRotatef(angleRotPacManX, 1.0, 0.0, 0.0);
	drawTopPacman(2, 40, longsTop);
	drawBottomPacman(2, 40, longsBottom);
	glPopMatrix();
	glPopMatrix();
}

void View3D::drawGhosts() {
	// Si pas de fantômes on quitte la méthode drawGhosts()
	if(model->getWorld()->getNbGhosts() == 0)
		return;

	for(int i = 0; i < model->getWorld()->getNbGhosts(); i++)
	{
		Ghost *g = model->getGhost(i);

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
			GLUquadricObj* cylinder = gluNewQuadric();
			glTranslated(posX - 0.75, posY, g->getRadius());	// déplacement

			glColor3fv(model->getGhost(0)->getColor());
			glRotatef(90,1.0,0.0,0.0);
			glScaled(GHOST_RADIUS,GHOST_RADIUS,GHOST_RADIUS);


			//Draw the base
			glPushMatrix();
			glRotatef(90, 1.0, 0.0, 0.0);
			glutWireCone(0.3, 0.5, 10, 10);
			glutSolidCone(0.3, 0.5, 10, 10);
			glPopMatrix();

			glPushMatrix();
			glTranslatef(0.5, 0.0, 0.5);
			glRotatef(90, 1.0, 0.0, 0.0);
			glutSolidCone(0.3, 0.5, 10, 10);
			glPopMatrix();

			glPushMatrix();
			glTranslatef(0.5, 0.0, -0.5);
			glRotatef(90, 1.0, 0.0, 0.0);
			glutSolidCone(0.3, 0.5, 10, 10);
			glPopMatrix();

			glPushMatrix();
			glTranslatef(1.0, 0.0, 0.0);
			glRotatef(90, 1.0, 0.0, 0.0);
			glutWireCone(0.3, 0.5, 10, 10);
			glutSolidCone(0.3, 0.5, 10, 10);
			glPopMatrix();


			//Draw the cylindrical body
			glPushMatrix();
			glTranslatef(0.5, 0.78, 0.0);
			glRotatef(90, 1.0, 0.0, 0.0);
			gluCylinder(cylinder, 0.8, 0.8, 0.8, 20, 20);
			glPopMatrix();

			//Draw the top spherical head
			glPushMatrix();
			glTranslatef(0.5, 0.8, 0.0);
			//glutWireSphere(0.8, 20, 20);
			glutSolidSphere(0.8, 20, 20);
			glPopMatrix();
			gluDeleteQuadric(cylinder);
		glPopMatrix();
		
	}
}

void View3D::initLights()
{
	// Lumières 
	/* Pour la avoir un effet de shininess */
	float mat_specular[] = {1, 1, 1, 1};
	float mat_shininess[] = {50};

	/* Spot sur light1 */
	float light1_specular[] = {1, 1, 1, 1};
	float light1_ambient[] = {0, 0, 0, 1};
	float light1_diffuse[] = {1, 1, 1, 1};	// donner une couleur au spot
	float light1_position[] = {0, 0, 15, 1};	// position de la source de lumière, positionnelle
	float light1_direction[] = {0, 0, 1};	// direction du spot

	/* Light2 */
	float light2_position[] = {-10, 10, 10, 0};	// position de la source de lumière, directionelle
	float light2_ambient[] = {0.2, 0.2, 0.2, 1};
	float light2_diffuse[] = {0.7, 0.7, 0.7, 1};
	float light2_specular[] = {1, 1, 1, 1};

	glEnable(GL_BLEND);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_NORMALIZE);
	glEnable(GL_DEPTH_TEST); //active le test de profondeur
	glEnable(GL_LIGHTING); // on active l'éclairage
	glEnable(GL_LIGHT2); // on allume/active la lumière 2
	glEnable(GL_LIGHT3);
	//glDisable(GL_LIGHT1); //désactive la lumière1
	//glDisable(GL_LIGHT0); //désactive la lumière0

	glEnable(GL_COLOR_MATERIAL); //afficher les couleurs lorsque l'éclairage est activé

	/*On active la shininess(brilliance)*/
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

	/*On initialise le spot*/
	glLightfv(GL_LIGHT3, GL_AMBIENT, light1_ambient);
	glLightfv(GL_LIGHT3, GL_DIFFUSE, light1_diffuse);
	glLightfv(GL_LIGHT3, GL_POSITION, light1_position);
	glLightfv(GL_LIGHT3, GL_SPECULAR, light1_specular);
	//spot direction
	glLightfv(GL_LIGHT3, GL_SPOT_DIRECTION, light1_direction);
	glLightf(GL_LIGHT3, GL_SPOT_CUTOFF, 15.0);

	glLightf(GL_LIGHT3, GL_CONSTANT_ATTENUATION, 0.02);
	glLightf(GL_LIGHT3, GL_LINEAR_ATTENUATION, 0.02);
	glLightf(GL_LIGHT3, GL_QUADRATIC_ATTENUATION, 0.02);
	glLightf(GL_LIGHT3, GL_SPOT_EXPONENT, 0.0);

	/*Lumière 2*/
	glLightfv(GL_LIGHT2, GL_AMBIENT, light2_ambient);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, light2_diffuse);
	glLightfv(GL_LIGHT2, GL_POSITION, light2_position);
	glLightfv(GL_LIGHT2, GL_SPECULAR, light2_specular);

	/*Modele de lumiere*/
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
}

void View3D::resetSpin() {
	time = 0;
}
