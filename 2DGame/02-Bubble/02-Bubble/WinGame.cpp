#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "WinGame.h"
#include "Game.h"
#include <GL/glut.h>



#define SCREEN_X 32
#define SCREEN_Y 16
#define SPACEBAR 32


#define INIT_PLAYER_X_TILES 4
#define INIT_PLAYER_Y_TILES 25


WinGame::WinGame()
{

}

WinGame::~WinGame()
{

}


void WinGame::init()
{
	initShaders();
	sizeWorldX = 10000;
	sizeWorldY = 10000;
	projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1.f), float(SCREEN_HEIGHT - 1), 0.f);
	timeToStart = 5000;
	initBackground();
}

void WinGame::update(int deltaTime)
{
	currentTime += deltaTime;
	timeToStart -= deltaTime;
	if (timeToStart < 0){
		timeToStart = 5000;
		Game::instance().setModeMenu();
	}
}

void WinGame::render()
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
	glColor3b(0xFF, 0x00, 0x00);
	glutSolidCube(1);

}

void WinGame::initShaders()
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

void WinGame::renderBackground(){

	//Background1
	glEnable(GL_TEXTURE_2D);
	background.use();
	glBindVertexArray(vao);
	glEnableVertexAttribArray(posLocation);
	glEnableVertexAttribArray(texCoordLocation);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisable(GL_TEXTURE_2D);

}

void WinGame::initBackground(){

	//carreguem textura (el background bo es el 23)
	background.loadFromFile("images/BackgroundWin.png", TEXTURE_PIXEL_FORMAT_RGBA);
	int width = background.width();
	int heigth = background.height();
	//Aqui definim la geometria i les cordenades de textura
	glm::vec2 geom[2] = { glm::vec2(0.f, (float)SCREEN_HEIGHT), glm::vec2((float)SCREEN_WIDTH, 0.f) };
	glm::vec2 texCoords[2] = { glm::vec2(0.f, 1.f), glm::vec2(1., .0f) };

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
