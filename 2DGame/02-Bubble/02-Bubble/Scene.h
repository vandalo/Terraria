#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"


// Scene contains all the entities of our game.
// It is responsible for updating and render them.


class Scene
{

public:
	Scene();
	~Scene();

	void init();
	void update(int deltaTime);
	void render();

private:
	void initShaders();
	bool LoadImage(int, char*, int);
	void renderBackground();
	void initBackground();

private:
	TileMap *map;
	Player *player;
	ShaderProgram texProgram;
	GLint posLocation, texCoordLocation;
	GLuint vao;
	GLuint vbo;
	float currentTime;
	glm::mat4 projection;
	Texture background;

};


#endif // _SCENE_INCLUDE

