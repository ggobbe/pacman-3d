/* The Cell header file. The maze is divided into cells of equal size.
Each Cell can contain either a maze boundary element, cell, supercell 
or be empty. */
#pragma once
#include "Globals.h"
#include <string>
#include <fstream>
#include <iostream>
#include "MyGlut.h"


using namespace glut;

class Cell 
{
private:
	int type;			/* Can be any of the types discussed above. */
	int x;			/* x,y coords what else */
	int y;
	float* color;

public: 
	Cell();
	~Cell();
	Cell(int,int,int);

	int getType() 
	{
		return type;
	}

	float* getColor() 
	{
		return color;
	}

	int getX() 
	{
		return x;
	}

	int getY() 
	{
		return y;
	}

	void setColor(float *);

	void getxy(int *, int*);
	void setType(int);
};