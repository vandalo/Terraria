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
#include "Inventary.h"
#include "Crafting.h"
#include "Define.h"

// Scene contains all the entities of our game.
// It is responsible for updating and render them.

class Scene
{
	Texture spritesheet;

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
	void setPlayerItem(int idCasella);
	int getMovingItem(){ return idMovingItem; };
	Player *player;
	void mousePress();
	void mouseRealease();
	ShaderProgram getShaderProgram(){ return texProgram; };
	int getActiveItem(){ return activeItem; };
	Inventary* getInventary(){ return inventary; };
	Crafting* getCrafting(){ return crafting; };
	float distance(glm::vec2 pos1, glm::vec2 pos2);
	bool getTheBoots(){ return theBoots; };
	void setTheBoots(){ theBoots = true; };
	void setTheBoss(){ theBoss = true; };
	void setAlert(){ alertMessage = true; };

private:
	void initShaders();
	bool LoadImage(int, char*, int);
	void renderBackground();
	void initBackground();
	void initBackground2();
	void initBackground3();
	
	

private:
	bool theBoss, theBoots, alertMessage;
	float alertTime;
	double wx, wy;
	StaticInterface *staticInterface;
	DinamicInterface *dinamicInterface;
	Crafting *crafting;
	bool showDinamicInterface;
	float sizeWorldX, sizeWorldY;
	TileMap *map;
	Monster *monsters[NUM_MONSTERS];
	Skull *skull;
	EyeBoss * eyeBoss;
	GLuint numMonsters;
	ShaderProgram texProgram;
	GLint posLocation, texCoordLocation, posLocation2, texCoordLocation2, posLocation3, texCoordLocation3;
	GLuint vao, vao2, vao3;
	GLuint vbo, vbo2, vbo3;
	float currentTime;
	glm::mat4 projection, modelview;
	Texture background, background2, background3, bossAlert;
	Inventary *inventary;
	int idMovingItem;
	bool mouse, pressed, incremented, decremented;
	int activeItem;
	void initMosntersPosition();
	Sprite * textAlert;
	

	int posXrubish, posYrubish;



};


#endif // _SCENE_INCLUDE

