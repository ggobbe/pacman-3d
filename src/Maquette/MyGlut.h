#pragma once
//MyGlut.h

// Cette "encapsulation" des includes des librairies glut et gl est nécessaire pour éviter un problème de redéfinition
// d'une méthode d'une librairie standard (la fonction "exit"), qui provoque l'erreur suivante :
// Error C2381: 'exit' : redefinition; __declspec(noreturn) differs Z:/include\GL\glut.h(146) : see declaration of 'exit.

namespace glut
{
#include "GL/glut.h"
#include "GL/gl.h"
}
