#ifndef _Snake_INCLUDE
#define _Snake_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"
#include "Monster.h"
#include "Skull.h"
#include "EyeBoss.h"
#include "StaticInterface.h"
#include "DinamicInterface.h"
#include <vector>
#include <iostream>

// Snake contains all the entities of our game.
// It is responsible for updating and render them.

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 600

class Snake
{

public:
	Snake();
	~Snake();

	void init();
	void update(int deltaTime);
	void render();
	void initMap();
	void generateFood();
	void changeDirection(int key);
	void move(int dx, int dy);
	char getMapValue(int value);
	int getFood(){ return food; };

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
	Texture background;
	Texture spritesheet;
	Texture spritesheetHeart;
	Sprite * spritesnake;
	Sprite * spritefood;
	vector<vector<int> > tauler;

	
	int mapwidth;
	int mapheight;
	int size;
	int map[SCREEN_HEIGHT/32 * SCREEN_WIDTH/32];
	int fps = 30;

	// Snake head details
	int headxpos;
	int headypos;
	int direction;

	// Amount of food the snake has (How long the body is)
	int food = 3;

	// Determine if game is running
	bool running;


};


#endif // _Snake_INCLUDE

