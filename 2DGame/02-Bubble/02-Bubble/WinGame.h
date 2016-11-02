#ifndef _WinGame_INCLUDE
#define _WinGame_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"
#include "Monster.h"
#include "Skull.h"
#include "EyeBoss.h"
#include "StaticInterface.h"
#include "DinamicInterface.h"

// WinGame contains all the entities of our game.
// It is responsible for updating and render them.


class WinGame
{

public:
	WinGame();
	~WinGame();

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


#endif // _WinGame_INCLUDE

