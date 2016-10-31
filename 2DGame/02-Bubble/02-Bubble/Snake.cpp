#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Snake.h"
#include "Game.h"
#include <GL/glut.h>



#define SCREEN_X 32
#define SCREEN_Y 16
#define SPACEBAR 32


#define INIT_PLAYER_X_TILES 4
#define INIT_PLAYER_Y_TILES 25


Snake::Snake()
{

}

Snake::~Snake()
{

}


void Snake::init()
{
	initShaders();
	sizeWorldX = 10000;
	sizeWorldY = 10000;
	projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1.f), float(SCREEN_HEIGHT - 1), 0.f);
	mapwidth = SCREEN_WIDTH / 32;
	mapheight = SCREEN_HEIGHT / 32;
	size = mapwidth * mapheight;
	
	spritesheet.loadFromFile("images/pj.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesnake = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.5, 0.045), &spritesheet, &texProgram);

	spritesheetHeart.loadFromFile("images/Heart.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritefood = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(.25, .5), &spritesheetHeart, &texProgram);
	running = true;
	initMap();
	direction = GLUT_KEY_LEFT;
	initBackground();
}

void Snake::update(int deltaTime)
{
	if (!running)Game::instance().setModeGame();
		currentTime += deltaTime;

		if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT)){
			changeDirection(GLUT_KEY_RIGHT);
		}
		else if (Game::instance().getSpecialKey(GLUT_KEY_UP)){
			changeDirection(GLUT_KEY_UP);
		}
		else if (Game::instance().getSpecialKey(GLUT_KEY_LEFT)){
			changeDirection(GLUT_KEY_LEFT);
		}
		else if (Game::instance().getSpecialKey(GLUT_KEY_DOWN)){
			changeDirection(GLUT_KEY_DOWN);
		}
	if (fps == 0){
		fps = 10;
		//Then
		// Move in direction indicated
		switch (direction) {
		case GLUT_KEY_LEFT: move(-1, 0);
			break;
		case GLUT_KEY_DOWN: move(0, 1);
			break;
		case GLUT_KEY_RIGHT: move(1, 0);
			break;
		case GLUT_KEY_UP: move(0, -1);
			break;
		}

		// Reduce snake values on map by 1
		for (int i = 0; i < size; i++) {
			if (map[i] > 0) map[i]--;
		}
	}
	else fps--;
}

void Snake::changeDirection(int key) {
	switch (key) {
	case GLUT_KEY_UP:
		if (direction != GLUT_KEY_DOWN) direction = GLUT_KEY_UP;
		break;
	case GLUT_KEY_RIGHT:
		if (direction != GLUT_KEY_LEFT) direction = GLUT_KEY_RIGHT;
		break;
	case GLUT_KEY_DOWN:
		if (direction != GLUT_KEY_UP) direction = GLUT_KEY_DOWN;
		break;
	case GLUT_KEY_LEFT:
		if (direction != GLUT_KEY_RIGHT) direction = GLUT_KEY_LEFT;
		break;
	}
}

void Snake::move(int dx, int dy) {
	// determine new head position
	int newx = headxpos + dx;
	int newy = headypos + dy;

	// Check if there is food at location
	if (map[newx + newy * mapwidth] == -2) {
		// Increase food value (body length)
		food++;

		// Generate new food on map
		generateFood();
	}

	// Check location is free
	else if (map[newx + newy * mapwidth] != 0) {
		running = false;
	}

	// Move head to new location
	headxpos = newx;
	headypos = newy;
	map[headxpos + headypos * mapwidth] = food + 1;

}

void Snake::render()
{

	glm::mat4 modelview;
	projection = glm::ortho((float)(0), (float)SCREEN_WIDTH,
		(float)SCREEN_HEIGHT, (float)(0));


	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);

	renderBackground();

	for (int i = 0; i < tauler.size(); i++){
		for (int j = 0; j < tauler[i].size(); j++){
			spritesnake->setPosition(glm::vec2(i*32, j*32));
			spritesnake->render();
		}
	}
	
	for (int x = 0; x < mapwidth; ++x) {
		for (int y = 0; y < mapheight; ++y) {
			// Prints the value at current x,y location
			char aux = getMapValue(map[x + y * mapwidth]);
			if (aux == 'o' || aux == 'X'){
				spritesnake->setPosition(glm::vec2(x * 32, y * 32));
				spritesnake->render();
			}
			else if (aux == 'O'){
				spritefood->setPosition(glm::vec2(x * 32, y * 32));
				spritefood->render();
			}
		}
	}

	glColor3b(0xFF, 0x00, 0x00);
	glutSolidCube(1);

}

void Snake::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if (!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if (!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	texProgram.init();
	texProgram.addShader(vShader);
	texProgram.addShader(fShader);
	texProgram.link();
	if (!texProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;		cout << "" << texProgram.log() << endl << endl;
	}

	texProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}

void Snake::renderBackground(){

	//Background1
	glEnable(GL_TEXTURE_2D);
	background.use();
	glBindVertexArray(vao);
	glEnableVertexAttribArray(posLocation);
	glEnableVertexAttribArray(texCoordLocation);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisable(GL_TEXTURE_2D);

}

void Snake::initBackground(){

	//carreguem textura (el background bo es el 23)
	background.loadFromFile("images/Background_0.png", TEXTURE_PIXEL_FORMAT_RGBA);
	int width = background.width();
	int heigth = background.height();
	//Aqui definim la geometria i les cordenades de textura
	glm::vec2 geom[2] = { glm::vec2(0.f, (float)SCREEN_HEIGHT), glm::vec2((float)SCREEN_WIDTH, 0.f) };
	glm::vec2 texCoords[2] = { glm::vec2(0.f, 1.f), glm::vec2((float)SCREEN_WIDTH / width, .0f) };

	//Definim els 24 vertex per pintar la textura 12 per cada triangle
	float vertices[24] = {
		geom[0].x, geom[0].y, texCoords[0].x, texCoords[0].y,
		geom[1].x, geom[0].y, texCoords[1].x, texCoords[0].y,
		geom[1].x, geom[1].y, texCoords[1].x, texCoords[1].y,
		geom[0].x, geom[0].y, texCoords[0].x, texCoords[0].y,
		geom[1].x, geom[1].y, texCoords[1].x, texCoords[1].y,
		geom[0].x, geom[1].y, texCoords[0].x, texCoords[1].y };

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(float), vertices, GL_STATIC_DRAW);

	posLocation = texProgram.bindVertexAttribute("position", 2, 4 * sizeof(float), 0);
	texCoordLocation = texProgram.bindVertexAttribute("texCoord", 2, 4 * sizeof(float), (void *)(2 * sizeof(float)));

}

void Snake::initMap()
{
	// Places the initual head location in middle of map
	headxpos = mapwidth / 2;
	headypos = mapheight / 2;
	map[headxpos + headypos * mapwidth] = 1;

	// Places top and bottom walls 
	for (int x = 0; x < mapwidth; ++x) {
		map[x] = -1;
		map[x + (mapheight - 1) * mapwidth] = -1;
	}

	// Places left and right walls
	for (int y = 0; y < mapheight; y++) {
		map[0 + y * mapwidth] = -1;
		map[(mapwidth - 1) + y * mapwidth] = -1;
	}

	// Generates first food
	generateFood();
}

// Generates new food on map
void Snake::generateFood() {
	int x = 0;
	int y = 0;
	do {
		// Generate random x and y values within the map
		x = rand() % (mapwidth - 2) + 1;
		y = rand() % (mapheight - 2) + 1;

		// If location is not free try again
	} while (map[x + y * mapwidth] != 0);

	// Place new food
	map[x + y * mapwidth] = -2;
}

char Snake::getMapValue(int value)
{
	// Returns a part of snake body
	if (value > 0) return 'o';

	switch (value) {
		// Return wall
	case -1: return 'X';
		// Return food
	case -2: return 'O';
	}
}