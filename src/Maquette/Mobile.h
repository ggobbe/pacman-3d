#pragma once
#include "Globals.h"

class Mobile
{
protected:
	int initPosX;
	int initPosY;
	int posX;
	int posY;
	float radius;
	int dir;
	float* color;
	int delta;
	bool running;

	Mobile();	// Constructeur uniquement accessible aux classes filles

public:
	bool isRunning() {
		return running;
	}

	int getPosX() 
	{
		return posX;
	}

	int getPosY() 
	{
		return posY;
	}

	float getRadius()
	{
		return radius;
	}

	int getDir()
	{
		return dir;
	}

	float* getColor()
	{
		return color;
	}

	int getDelta()
	{
		return delta;
	}

	int getInitPosX()
	{
		return initPosX;
	}

	int getInitPosY()
	{
		return initPosY;
	}

	void setInitPosX(int);
	void setInitPosY(int);
	void setRunning(bool);
	void setPosX(int);
	void setPosY(int);
	void setRadius(float);
	void setDir(int);
	void setColor(float*);
	void setDelta(int);
};