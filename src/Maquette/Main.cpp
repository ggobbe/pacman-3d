//Main.cpp
#include "Controller.h"


// Chaque call back glut est d�l�gu� � une m�thode de la classe View ou Controller
// Ceci est n�cessaire car glut ne connait que les fonctions C.
void displayCallBack()
{
	View::getInstance()->display();
}

void reshapeCallBack(int width, int height)
{
	View::getInstance()->reshape(width, height);
}

void specialCallBack(int key, int x, int y)
{
	Controller::getInstance()->special(key, x, y);
}

void mouseCallBack(int button, int state, int x, int y)
{
	Controller::getInstance()->handlePicking(button, state, x, y);
}


int main(int argc, char** argv)
{
	// On initialise la seed du random
	srand((unsigned)time(NULL));

	// Intialisation de glut
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); // Initialise les modes li�s � l'affichage
	glutInitWindowSize(500,500); // D�finit la taille de la fen�tre d'�cran
	glutInitWindowPosition(100, 100); // D�finit la position de la fen�tre d'�cran
	glutCreateWindow(argv[0]); // Cr�e la fen�tre d'�cran avec le nom pass� en argument

	// Sp�cification de la fonction Call back pour le dessin de la vue
	// Cette foncton sera appell�e chaque fois que la vue est redessin�e
	glutDisplayFunc(displayCallBack);

	// Sp�cification de la fonction Call back de redimensionnement de la fen�tre
	// Cette foncton sera appell�e chaque fois que l'utilisateur redimensionne la fen�tre
	glutReshapeFunc(reshapeCallBack);

	// Sp�cification de la fonction Call back de l'appui des touches clavier
	// Cette foncton sera appell�e chaque fois que l'utilisateur appuie sur une touche du clavier
	glutSpecialFunc(specialCallBack);

	// Sp�cification de la fonction Call back de la souris
	// Cette foncton sera appell�e chaque fois que l'utilisateur sollicite la souris
	glutMouseFunc(mouseCallBack);

	// Le controller est un singleton
	Controller *controller = Controller::getInstance();

	// D�marre une boucle �ternelle d'�coute des call back
	glutMainLoop();

	return 0;
}