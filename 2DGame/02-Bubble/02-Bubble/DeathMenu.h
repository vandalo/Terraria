#ifndef _DeathMenu_INCLUDE
#define _DeathMenu_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"
#include "Monster.h"
#include "Skull.h"
#include "EyeBoss.h"
#include "StaticInterface.h"
#include "DinamicInterface.h"

// DeathMenu contains all the entities of our game.
// It is responsible for updating and render them.


class DeathMenu
{

public:
	DeathMenu();
	~DeathMenu();

	void init();
	void update(int deltaTime);
	void render();

private:
	void initShaders();
	bool LoadImage(int, char*, int);
	void renderBackground();
	void initBackground();

private:
	int timeToStart;
	float sizeWorldX, sizeWorldY;
	ShaderProgram texProgram;
	GLint posLocation, texCoordLocation;
	GLuint vao;
	GLuint vbo;
	float currentTime;
	glm::mat4 projection;
	Texture background;

};


#endif // _DeathMenu_INCLUDE

