
#include "cScene.h"
#include <gl/glut.h>

cScene::cScene(void)
{
}

cScene::~cScene(void)
{
}

void cScene::Draw()
{
	glBegin(GL_TRIANGLES);
		glColor3f(1.0f,0.0f,0.0f);	glVertex2i(100,100);
		glColor3f(0.0f,1.0f,0.0f);	glVertex2i(300,100);
		glColor3f(0.0f,0.0f,1.0f);	glVertex2i(200,200);
	glEnd();
}
