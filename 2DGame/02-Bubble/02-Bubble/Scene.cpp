#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"
#include <GL/glut.h>



#define SCREEN_X 32
#define SCREEN_Y 16

#define INIT_PLAYER_X_TILES 4
#define INIT_PLAYER_Y_TILES 25


Scene::Scene()
{
	map = NULL;
	player = NULL;
}

Scene::~Scene()
{
	if(map != NULL)
		delete map;
	if(player != NULL)
		delete player;
}


void Scene::init()
{
	initShaders();
	map = TileMap::createTileMap("levels/level01.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);

	numMonsters = 2;
	sizeWorldX = 10000;
	sizeWorldY = 10000;

	initBackground();
	initBackground2();
	initBackground3();
	player = new Player();
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
	player->setTileMap(map);

	//monsters sera un array i farem un for
	for (int i = 0; i < numMonsters; i++){
		monsters[i] = new Monster();
		monsters[i]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		monsters[i]->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize() + 300 * (i+1), INIT_PLAYER_Y_TILES * map->getTileSize()));
		monsters[i]->setInitPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize() + 300 * (i+1), INIT_PLAYER_Y_TILES * map->getTileSize()));
		monsters[i]->setRadiPatrulla(60);
		monsters[i]->setTileMap(map);
		monsters[i]->setEsMouDreta(i % 2 == 0);
		monsters[i]->setRadiPErseguir(70);
	}

	//Creem el boss eye
	eyeBoss = new EyeBoss();
	eyeBoss->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	eyeBoss->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize() + 800, INIT_PLAYER_Y_TILES * map->getTileSize() - 300));
	eyeBoss->setInitPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize() + 800, INIT_PLAYER_Y_TILES * map->getTileSize() - 300));
	eyeBoss->setRadiPatrulla(60);
	eyeBoss->setTileMap(map);
	eyeBoss->setEsMouDreta(false);
	eyeBoss->setRadiPerseguir(70);

	projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1.f), float(SCREEN_HEIGHT - 1), 0.f);
	staticInterface = new StaticInterface();
	staticInterface->init(texProgram);
	dinamicInterface = new DinamicInterface();
	dinamicInterface->init(texProgram);
	showDinamicInterface = false;
	currentTime = 0.0f;
}

void Scene::update(int deltaTime)
{
	currentTime += deltaTime;
	staticInterface->update(deltaTime);
	dinamicInterface->update(deltaTime);
	player->update(deltaTime);
	for (int i = 0; i < numMonsters; i++){
		monsters[i]->update(deltaTime);
	}
	//skull->update(deltaTime);
	eyeBoss->update(deltaTime);
	int sx, sy;
	double wx, wy;
	if (Game::instance().isMousePressed(GLUT_LEFT_BUTTON)) {
		Game::instance().getScreenMousePos(&sx, &sy);
		cout << "Screen (" << sx << ", " << sy << ")" << endl;
		Game::instance().getWorldMousePos(&wx, &wy);
		cout << "World (" << wx << ", " << wy << ")" << endl;
		cout << endl;
	}
}

void Scene::render()
{

	glm::mat4 modelview;
	projection = glm::ortho((float)(player->getX() - (SCREEN_WIDTH/2)), float((SCREEN_WIDTH/2) + player->getX()),
		(float)(SCREEN_HEIGHT / 2) + player->getY(), (float)(player->getY() - (SCREEN_HEIGHT / 2)));


	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);

	renderBackground();
	map->render();
	player->render();
	for (int i = 0; i < numMonsters; i++){
		monsters[i]->render();
	}
	eyeBoss->render();

	projection = glm::ortho(0.f, (float)SCREEN_WIDTH,
		0.f, (float)SCREEN_HEIGHT);
	texProgram.setUniformMatrix4f("projection", projection);
	staticInterface->render();
	if (showDinamicInterface)dinamicInterface->render(false);
	//glColor3b(0xFF, 0x00, 0x00);
	//glutSolidCube(1);
	
}

void Scene::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if(!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if(!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	texProgram.init();
	texProgram.addShader(vShader);
	texProgram.addShader(fShader);
	texProgram.link();
	if(!texProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << texProgram.log() << endl << endl;
	}
	texProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}

void Scene::renderBackground(){

	//Background1
	glEnable(GL_TEXTURE_2D);
	background.use();
	glBindVertexArray(vao);
	glEnableVertexAttribArray(posLocation);
	glEnableVertexAttribArray(texCoordLocation);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisable(GL_TEXTURE_2D);

	//Background2
	glEnable(GL_TEXTURE_2D);
	background2.use();
	glBindVertexArray(vao2);
	glEnableVertexAttribArray(posLocation2);
	glEnableVertexAttribArray(texCoordLocation2);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisable(GL_TEXTURE_2D);

	//Background3
	glEnable(GL_TEXTURE_2D);
	background3.use();
	glBindVertexArray(vao3);
	glEnableVertexAttribArray(posLocation3);
	glEnableVertexAttribArray(texCoordLocation3);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisable(GL_TEXTURE_2D);
}

void Scene::initBackground(){

	//carreguem textura (el background bo es el 23)
	background.loadFromFile("images/background_0.png", TEXTURE_PIXEL_FORMAT_RGBA);
	background.setWrapT(GL_CLAMP_TO_EDGE);
	int width = background.width();
	int heigth = background.height();
	//Aqui definim la geometria i les cordenades de textura
	glm::vec2 geom[2] = { glm::vec2(-sizeWorldX, 480.f), glm::vec2(sizeWorldX, -sizeWorldY) };
	glm::vec2 texCoords[2] = { glm::vec2(0.f, 1.f), glm::vec2(sizeWorldX *2 / width, (480+sizeWorldY)/heigth) };

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

void Scene::initBackground2(){
	background2.loadFromFile("images/background_71.png", TEXTURE_PIXEL_FORMAT_RGBA);
	int heigth = background2.height();
	int width = background2.width();
	//Aqui definim la geometria i les cordenades de textura
	glm::vec2 geom[2] = { glm::vec2(-sizeWorldX, 480.f), glm::vec2(sizeWorldX, 960.f) };
	glm::vec2 texCoords[2] = { glm::vec2(0.f, 0.f), glm::vec2(sizeWorldX * 2 /width, 480.f/heigth) };

	//Definim els 24 vertex per pintar la textura 12 per cada triangle
	float vertices[24] = {
		geom[0].x, geom[0].y, texCoords[0].x, texCoords[0].y,
		geom[1].x, geom[0].y, texCoords[1].x, texCoords[0].y,
		geom[1].x, geom[1].y, texCoords[1].x, texCoords[1].y,
		geom[0].x, geom[0].y, texCoords[0].x, texCoords[0].y,
		geom[1].x, geom[1].y, texCoords[1].x, texCoords[1].y,
		geom[0].x, geom[1].y, texCoords[0].x, texCoords[1].y };

	glGenVertexArrays(1, &vao2);
	glBindVertexArray(vao2);
	glGenBuffers(1, &vbo2);
	glBindBuffer(GL_ARRAY_BUFFER, vbo2);
	glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(float), vertices, GL_STATIC_DRAW);

	posLocation2 = texProgram.bindVertexAttribute("position", 2, 4 * sizeof(float), 0);
	texCoordLocation2 = texProgram.bindVertexAttribute("texCoord", 2, 4 * sizeof(float), (void *)(2 * sizeof(float)));

	//carreguem textura (el background bo es el 23)
	
}

void Scene::initBackground3(){
	background3.loadFromFile("images/background_70.png", TEXTURE_PIXEL_FORMAT_RGBA);
	int heigth = background3.height();
	int width = background3.width();
	//Aqui definim la geometria i les cordenades de textura
	glm::vec2 geom[2] = { glm::vec2(-sizeWorldX, 480.f), glm::vec2(sizeWorldX, 480.f-16.f) };
	glm::vec2 texCoords[2] = { glm::vec2(0.f, 1.f), glm::vec2(sizeWorldX  * 2 / width, 0.f) };

	//Definim els 24 vertex per pintar la textura 12 per cada triangle
	float vertices[24] = {
		geom[0].x, geom[0].y, texCoords[0].x, texCoords[0].y,
		geom[1].x, geom[0].y, texCoords[1].x, texCoords[0].y,
		geom[1].x, geom[1].y, texCoords[1].x, texCoords[1].y,
		geom[0].x, geom[0].y, texCoords[0].x, texCoords[0].y,
		geom[1].x, geom[1].y, texCoords[1].x, texCoords[1].y,
		geom[0].x, geom[1].y, texCoords[0].x, texCoords[1].y };

	glGenVertexArrays(1, &vao3);
	glBindVertexArray(vao3);
	glGenBuffers(1, &vbo3);
	glBindBuffer(GL_ARRAY_BUFFER, vbo3);
	glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(float), vertices, GL_STATIC_DRAW);

	posLocation3 = texProgram.bindVertexAttribute("position", 2, 4 * sizeof(float), 0);
	texCoordLocation3 = texProgram.bindVertexAttribute("texCoord", 2, 4 * sizeof(float), (void *)(2 * sizeof(float)));

	//carreguem textura (el background bo es el 23)

}

glm::vec2 Scene::getPlayerPos(){
	return glm::vec2(player->getX(), player->getY());
}

int Scene::getPlayerLife(){
	return player->getLife();
}
int Scene::getPlayerMaxLife(){
	return player->getMaxLife();
}

void Scene::changeModeInterface(){
	showDinamicInterface = !showDinamicInterface;
}