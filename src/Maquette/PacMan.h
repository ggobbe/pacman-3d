#pragma once
#include <iostream>
#include "Globals.h"
#include "Mobile.h"

using namespace std;

class PacMan : public Mobile
{
private:
	int mode; // mode du pacman (normal ou super)
	int nextDir;	// ancienne direction    TODO : bien utile?
	bool mouthOpen; // ouverture de la bouche du PacMan
	int dying;

public:
	PacMan();
	~PacMan();

	int getMode()
	{
		return mode;
	}

	int getNextDir()
	{
		return nextDir;
	}

	bool isMouthOpen()
	{
		return mouthOpen;
	}

	int getDying()
	{
		return dying;
	}

	void setNextDir(int);
	void switchMouth();
	void couleurAleatoire();
	void animSuperMode(bool);
	void setDying(int);
};

