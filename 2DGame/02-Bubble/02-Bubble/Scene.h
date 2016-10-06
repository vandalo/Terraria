#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"
#include "Monster.h"
#include "Skull.h"


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
	glm::vec2 getPlayerPos();

private:
	void initShaders();
	bool LoadImage(int, char*, int);
	void renderBackground();
	void initBackground();

private:
	TileMap *map;
	Player *player;
	Monster *monsters[256];
	Skull *skull;
	GLuint numMonsters;
	ShaderProgram texProgram;
	GLint posLocation, texCoordLocation;
	GLuint vao;
	GLuint vbo;
	float currentTime;
	glm::mat4 projection;
	Texture background;

};


#endif // _SCENE_INCLUDE

