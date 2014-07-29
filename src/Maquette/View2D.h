#pragma once
#include "View.h"
#include "Model.h"

class View2D : public View
{
private:
	void drawPills();
	void drawCentralControl(GLenum);
	void drawPacMan();
	void drawGhosts();
	void drawHLine();
	void drawWLine();

public:
	void viewportWorld();
	View2D(void);
	~View2D();
};