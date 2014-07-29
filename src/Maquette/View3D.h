#pragma once
#include "View.h"

class View3D : public View
{
private:
	double time;
	void drawCentralControl(GLenum);
	void drawTopPacman(double r, int lats, int longs);
	void drawBottomPacman(double r, int lats, int longs);
	void drawPacMan();
	void drawGhosts();
	void drawPills();
	void drawHLine();
	void drawWLine();

public:
	View3D(void);
	~View3D();
	void viewportWorld();
	void initLights(void);
	void resetSpin();
	static View3D* getInstance();
};