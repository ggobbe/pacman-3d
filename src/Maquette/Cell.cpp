#include "Cell.h"

using namespace std;
using namespace glut;

Cell::Cell()
{
	Cell(0, 0, 0);
}

Cell::Cell(int mx,int my,int t) 
{
	color=new float[3];
	x=mx;
	y=my;
	type=t;
}

Cell::~Cell() {
	//delete color;
}

void Cell::setType(int t) 
{
	type=t;
}

void Cell::getxy(int *mx,int *my) 
{
	*mx=x;
	*my=y;
}

void Cell::setColor(float * newColor) 
{
	color=newColor;
}