#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"
#include "Monster.h"
#include "Skull.h"
#include "EyeBoss.h"
#include "StaticInterface.h"
#include "DinamicInterface.h"

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
	int getPlayerLife();
	int getPlayerMaxLife();
	void changeModeInterface();
	int inventaryClick(int x, int y);

private:
	void initShaders();
	bool LoadImage(int, char*, int);
	void renderBackground();
	void initBackground();
	void initBackground2();
	void initBackground3();

private:
	StaticInterface *staticInterface;
	DinamicInterface *dinamicInterface;
	bool showDinamicInterface;
	float sizeWorldX, sizeWorldY;
	TileMap *map;
	Player *player;
	Monster *monsters[256];
	Skull *skull;
	EyeBoss * eyeBoss;
	GLuint numMonsters;
	ShaderProgram texProgram;
	GLint posLocation, texCoordLocation, posLocation2, texCoordLocation2, posLocation3, texCoordLocation3;
	GLuint vao, vao2, vao3;
	GLuint vbo, vbo2, vbo3;
	float currentTime;
	glm::mat4 projection;
	Texture background, background2, background3;
	int posXobjectsInventary, posYobjectsInventary, posXset, posYset, posXcraftBasic, posYcraftBasic, posXchest, posYchest,
		posXobjectsNeed, posYobjectsNeed, posXrubish, posYrubish;


};


#endif // _SCENE_INCLUDE

