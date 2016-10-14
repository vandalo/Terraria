#ifndef _MainMenu_INCLUDE
#define _MainMenu_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"
#include "Monster.h"
#include "Skull.h"
#include "EyeBoss.h"
#include "StaticInterface.h"
#include "DinamicInterface.h"

// MainMenu contains all the entities of our game.
// It is responsible for updating and render them.


class MainMenu
{

public:
	MainMenu();
	~MainMenu();

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
	ShaderProgram texProgram;
	GLint posLocation, texCoordLocation, posLocation2, texCoordLocation2, posLocation3, texCoordLocation3;
	GLuint vao, vao2, vao3;
	GLuint vbo, vbo2, vbo3;
	float currentTime;
	glm::mat4 projection;
	Texture background, background2, background3;

};


#endif // _MainMenu_INCLUDE

