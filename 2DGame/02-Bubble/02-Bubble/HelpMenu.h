#ifndef _HelpMenu_INCLUDE
#define _HelpMenu_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"
#include "Monster.h"
#include "Skull.h"
#include "EyeBoss.h"
#include "StaticInterface.h"
#include "DinamicInterface.h"

// HelpMenu contains all the entities of our game.
// It is responsible for updating and render them.


class HelpMenu
{

public:
	HelpMenu();
	~HelpMenu();

	void init();
	void update(int deltaTime);
	void render();

private:
	void initShaders();
	bool LoadImage(int, char*, int);
	void renderBackground();
	void initBackground();
	void initBackground2();
	void initBackground3();

private:
	float sizeWorldX, sizeWorldY;
	float Yposition;
	ShaderProgram texProgram;
	GLint posLocation, texCoordLocation;
	GLuint vao;
	GLuint vbo;
	float currentTime;
	glm::mat4 projection;
	Texture background;
	Texture spritesheetInventary;
	Sprite *back;

};


#endif // _HelpMenu_INCLUDE

