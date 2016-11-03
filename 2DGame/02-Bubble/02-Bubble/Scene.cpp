#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
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
	bossAlert.loadFromFile("images/alertBoss.png", TEXTURE_PIXEL_FORMAT_RGBA);
	textAlert = Sprite::createSprite(glm::ivec2(300.f, 300.f), glm::vec2(1.f, 1.f), &bossAlert, &texProgram);
	textAlert->setPosition(glm::vec2(SCREEN_WIDTH / 2, 200.f));
	sizeWorldX = 50000;
	sizeWorldY = 50000;
	idMovingItem = -1;
	theBoss = false;
	theBoots = false;
	incremented = false;
	alertMessage = false;
	alertTime = 300 * 60;
	mouse = false;
	initBackground();
	initBackground2();
	initBackground3();
	player = new Player();
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);

	player->setPosition(glm::vec2(384 * map->getTileSize(), 324 * map->getTileSize()));
	player->setTileMap(map);
	inventary = new Inventary(texProgram);
	inventary->setActiveItem(1);
	if (Game::instance().getFood() > 10){
		inventary->putItem(FLY_BOOTS, 40, texProgram);
	}

	for (int i = 0; i < NUM_MONSTERS; i++){
		monsters[i] = new Monster();
		monsters[i]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	}
	initMosntersPosition();

	//Creem el boss eye
	eyeBoss = new EyeBoss();
	eyeBoss->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	eyeBoss->setPosition(glm::vec2(718 * map->getTileSize(), 141 * map->getTileSize()));

	eyeBoss->setInitPosition(glm::vec2(718 * map->getTileSize(), 135 * map->getTileSize()));
	eyeBoss->setRadiPatrulla(60);
	eyeBoss->setTileMap(map);
	eyeBoss->setEsMouDreta(false);
	eyeBoss->setRadiPerseguir(70);

	staticInterface = new StaticInterface();
	staticInterface->init(texProgram);
	dinamicInterface = new DinamicInterface();
	dinamicInterface->init(texProgram);
	crafting = new Crafting(texProgram);

	showDinamicInterface = false;

	currentTime = 0.0f;
}

void Scene::update(int deltaTime)
{
	int sx, sy;
	if (alertMessage){
		alertTime -= deltaTime;
		if (alertTime < 0)alertMessage = false;
	}
	Game::instance().getScreenMousePos(&sx, &sy);
	Game::instance().getWorldMousePos(&wx, &wy, player->getX() - SCREEN_WIDTH / 2, player->getY() - SCREEN_HEIGHT / 2);
	currentTime += deltaTime;
	if (Game::instance().getPlayerLife() <= 0){
		Game::instance().setModeDeath();
	}
	else if (eyeBoss->getLife() <= 0){
		Game::instance().setModeWin();
	}
	else{
		//inventary->update();
		if (!Game::instance().getSpecialKey(GLUT_KEY_UP))incremented = false;
		if (!Game::instance().getSpecialKey(GLUT_KEY_DOWN))decremented = false;
		if (!Game::instance().isMousePressed(GLUT_LEFT_BUTTON)) pressed = false;
		if (showDinamicInterface){
			if (Game::instance().getSpecialKey(GLUT_KEY_UP) && !incremented){
				crafting->incrementPointer();
				incremented = true;
			}
			if (Game::instance().getSpecialKey(GLUT_KEY_DOWN) && !decremented){
				crafting->decrementPointer();
				decremented = true;
			}
		}
		else{
			player->update(deltaTime);
			if (Game::instance().isMousePressed(GLUT_LEFT_BUTTON) && player->getActiveItem() == PICK
				&& distance(glm::vec2(wx,wy), glm::vec2(player->getX(),player->getY())) < 32*3.f ){
				map->decreaseWorldTileLife(wx, wy, 1);

			}
			else if (Game::instance().isMousePressed(GLUT_LEFT_BUTTON) && player->getActiveItem() == WOOD
				&& distance(glm::vec2(wx, wy), glm::vec2(player->getX(), player->getY())) < 32 * 3.f) {
				bool done = map->setWorldTile(TILE_WOOD_WALL, wx, wy);
				if (done){
					inventary->removeItem(activeItem);
					setPlayerItem(getActiveItem());
				}
			}
		}
		for (int i = 0; i < NUM_MONSTERS; i++){
			monsters[i]->update(deltaTime);
		}
		//Only once we have the diamondSword
		if(theBoss) eyeBoss->update(deltaTime);
		staticInterface->update(deltaTime);

		int idClick = inventaryClick(sx, sy);
		if (Game::instance().isMousePressed(GLUT_LEFT_BUTTON)) {
			if (!showDinamicInterface){
				if (idClick >= 0 && idClick < 10){
					inventary->setActiveItem(idClick);
					activeItem = idClick;
				}
				else{
					player->setAtacking();
					player->upgareAngleWeapon();
				}
			}
			//crafting
			else if (idClick == 59 && !pressed){
				crafting->craftItem(crafting->getIdObejctToCraft(), texProgram);
				crafting->update();
				pressed = true;
				setPlayerItem(getActiveItem());
			}
			//Help Button
			else if (idClick == 60){
				Game::instance().setModeHelp();
			}
			//Drag and droop
			else{
				if (mouse && idMovingItem == -1){
					if (idClick != -1 && idClick >= 0 && idClick < 58 && inventary->getId(idClick) != 0){
						idMovingItem = idClick;
						mouse = true;
					}

				}
				if (mouse && idMovingItem != -1)inventary->moveItem(idMovingItem, sx - 16, SCREEN_HEIGHT - sy - 16);
			}
		}
		//Just mouseRelease
		else if (idMovingItem != -1){
			if (idClick != -1){
				if (idClick == 58) {
					inventary->removeItem(idMovingItem);
					crafting->update();
				}
				else inventary->swapItem(idMovingItem, idClick);
				player->updatePlayerSet();
				setPlayerItem(getActiveItem());
			}
			idMovingItem = -1;
		}
		else{
			player->unsetAtacking();
			player->setAngleWeapon();
		}
	}
}

void Scene::render()
{
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
	for (int i = 0; i < NUM_MONSTERS; i++){
		monsters[i]->render();
	}
	if (theBoss) eyeBoss->render();

	//Interface
	projection = glm::ortho(0.f, (float)SCREEN_WIDTH,
		0.f, (float)SCREEN_HEIGHT);
	texProgram.setUniformMatrix4f("projection", projection);

	if (alertMessage) textAlert->render();

	staticInterface->render();
	if (showDinamicInterface){
		dinamicInterface->render(false);
		crafting->render();
	}
	inventary->render(showDinamicInterface);
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
	glm::vec2 geom[2] = { glm::vec2(-sizeWorldX, 356*16.f), glm::vec2(sizeWorldX, 0.f) };
	glm::vec2 texCoords[2] = { glm::vec2(0.f, 1.f), glm::vec2(sizeWorldX * 2 / width, - 200*16 / heigth) };

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
	glm::vec2 geom[2] = { glm::vec2(-sizeWorldX, 356 * 16.f), glm::vec2(sizeWorldX, sizeWorldY) };
	glm::vec2 texCoords[2] = { glm::vec2(0.f, 0.f), glm::vec2(sizeWorldX * 2 / width, (sizeWorldY - 356 * 16.f) / heigth) };

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
	glm::vec2 geom[2] = { glm::vec2(-sizeWorldX, 356 * 16.f), glm::vec2(sizeWorldX, 356 * 16.f - 16.f) };
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

int Scene::inventaryClick(int x, int y){
	y = SCREEN_HEIGHT - y;
	//Draw backpack
	int res = -1;
	int cont = 0;
	for (int i = 0; i < 40; i++){
		if (i % 10 == 0){
			cont++;
		}
		if (x > posXobjectsInventary + ((i % 10) * 35) && x < (posXobjectsInventary + ((i % 10) * 35)) + 32){
			if (y < (posYobjectsInventary - ((cont - 1) * 35)) && y > posYobjectsInventary - ((cont - 1) * 35) - 32){
				res = i + 10;
			}
		}
	}
	//10 Objectes principals
	if (res == -1){
		for (int i = 0; i < 10; i++){
			if (x > posXobjectsInventary + ((i % 10) * 35) && x < (posXobjectsInventary + ((i % 10) * 35)) + 32){
				if (y < (posYobjectsInventary + (35)) && y > posYobjectsInventary + (35) - 32){
					res = i;
				}
			}
		}
	}
	if (res == -1){
		//rubish
		if (x > posXobjectsInventary + 315 && x < posXobjectsInventary + 315 + 32){
			if (y < posYobjectsInventary - (5 * 35 - 32) && y > posYobjectsInventary - (5 * 35)){
				res = 58;
			}
		}
	}
	//draw set
	if (res == -1){
		if (x > posXset && x < posXset + 32){
			for (int i = -1; i < 7; i++){
				if (y < posYset - (i * 35) && y > posYset - (i * 35) - 32){
					res = i + 51;
				}
			}
		}
	}
	cont = -1;
	//draw craftingPosibilitys
	int increment = 31;
	for (int i = 0; i < 5; i++){
		if (i == 2){
			increment += 2;
			if (x > posXcraftBasic && x < posXcraftBasic + 32){
				if (y < posYcraftBasic - increment*(i - 1) && y > posYcraftBasic - increment*(i - 1) - 32){
					res = 59;
				}
			}
		}
	}
	//help button
	if (x > 16 && x < 48){
		if (y > 16 && y < 48) res = 60;
	}
	return res;
}

void Scene::setPlayerItem(int idCasella){
	activeItem = idCasella;
	inventary->setActiveItem(idCasella);
}

void Scene::mouseRealease(){
	if(showDinamicInterface)mouse = false;
}

void Scene::mousePress(){
	if (showDinamicInterface)mouse = true;
}

void Scene::initMosntersPosition(){
	for (int i = 0; i < NUM_MONSTERS; i++){
		monsters[i]->setTileMap(map);
		if(i < 24)
			monsters[i]->setId(1);
		else 
			monsters[i]->setId(2);
		monsters[i]->setEsMouDreta(i % 2 == 0);
		monsters[i]->setRadiPErseguir(200);
		monsters[i]->setRadiPatrulla(200);
		switch (i){
			//TODO CANVIAR EL 0
		case 0:
			monsters[i]->setPosition(glm::vec2(227 * map->getTileSize(), 294 * map->getTileSize()));
			monsters[i]->setInitPosition(glm::vec2(227 * map->getTileSize(), 294 * map->getTileSize()));
			break;
			break;
		case 1:
			monsters[i]->setPosition(glm::vec2(77 * map->getTileSize(), 332 * map->getTileSize()));
			monsters[i]->setInitPosition(glm::vec2(77 * map->getTileSize(), 332 * map->getTileSize()));
			break;
		case 2:
			monsters[i]->setPosition(glm::vec2(106 * map->getTileSize(), 331 * map->getTileSize()));
			monsters[i]->setInitPosition(glm::vec2(106 * map->getTileSize(), 331 * map->getTileSize()));
			break;
		case 3:
			monsters[i]->setPosition(glm::vec2(145 * map->getTileSize(), 321 * map->getTileSize()));
			monsters[i]->setInitPosition(glm::vec2(145 * map->getTileSize(), 321 * map->getTileSize()));
			break;
		case 4:
			monsters[i]->setPosition(glm::vec2(217 * map->getTileSize(), 346 * map->getTileSize()));
			monsters[i]->setInitPosition(glm::vec2(217 * map->getTileSize(), 346 * map->getTileSize()));
			break;
		case 5:
			monsters[i]->setPosition(glm::vec2(266 * map->getTileSize(), 353 * map->getTileSize()));
			monsters[i]->setInitPosition(glm::vec2(266 * map->getTileSize(), 353 * map->getTileSize()));
			break;
		case 6:
			monsters[i]->setPosition(glm::vec2(309 * map->getTileSize(), 365 * map->getTileSize()));
			monsters[i]->setInitPosition(glm::vec2(309 * map->getTileSize(), 365 * map->getTileSize()));
			break;
		case 7:
			monsters[i]->setPosition(glm::vec2(127 * map->getTileSize(), 371 * map->getTileSize()));
			monsters[i]->setInitPosition(glm::vec2(127 * map->getTileSize(), 371 * map->getTileSize()));
			break;
		case 8:
			monsters[i]->setPosition(glm::vec2(146 * map->getTileSize(), 373 * map->getTileSize()));
			monsters[i]->setInitPosition(glm::vec2(146 * map->getTileSize(), 373 * map->getTileSize()));
			break;
		case 9:
			monsters[i]->setPosition(glm::vec2(185 * map->getTileSize(), 378 * map->getTileSize()));
			monsters[i]->setInitPosition(glm::vec2(185 * map->getTileSize(), 378 * map->getTileSize()));
		case 10:
			monsters[i]->setPosition(glm::vec2(210 * map->getTileSize(), 379 * map->getTileSize()));
			monsters[i]->setInitPosition(glm::vec2(210 * map->getTileSize(), 379 * map->getTileSize()));
			//Dreta
		case 11:
			monsters[i]->setPosition(glm::vec2(716 * map->getTileSize(), 307 * map->getTileSize()));
			monsters[i]->setInitPosition(glm::vec2(716 * map->getTileSize(), 307 * map->getTileSize()));
			break;
		case 12:
			monsters[i]->setPosition(glm::vec2(703 * map->getTileSize(), 308 * map->getTileSize()));
			monsters[i]->setInitPosition(glm::vec2(703 * map->getTileSize(), 308 * map->getTileSize()));
			break;
		case 13:
			monsters[i]->setPosition(glm::vec2(729 * map->getTileSize(), 255 * map->getTileSize()));
			monsters[i]->setInitPosition(glm::vec2(729 * map->getTileSize(), 255 * map->getTileSize()));
			break;
		case 14:
			monsters[i]->setPosition(glm::vec2(735 * map->getTileSize(), 256 * map->getTileSize()));
			monsters[i]->setInitPosition(glm::vec2(735 * map->getTileSize(), 256 * map->getTileSize()));
			break;
		case 15:
			monsters[i]->setPosition(glm::vec2(732 * map->getTileSize(), 198 * map->getTileSize()));
			monsters[i]->setInitPosition(glm::vec2(732 * map->getTileSize(), 198 * map->getTileSize()));
			break;
		case 16:
			monsters[i]->setPosition(glm::vec2(772 * map->getTileSize(), 290 * map->getTileSize()));
			monsters[i]->setInitPosition(glm::vec2(772 * map->getTileSize(), 290 * map->getTileSize()));
			break; 
		case 17:
			monsters[i]->setPosition(glm::vec2(798 * map->getTileSize(), 292 * map->getTileSize()));
			monsters[i]->setInitPosition(glm::vec2(798 * map->getTileSize(), 292 * map->getTileSize()));
			break;
		case 18:
			monsters[i]->setPosition(glm::vec2(816 * map->getTileSize(), 301 * map->getTileSize()));
			monsters[i]->setInitPosition(glm::vec2(816 * map->getTileSize(), 301 * map->getTileSize()));
			break;
		case 19:
			monsters[i]->setPosition(glm::vec2(835 * map->getTileSize(), 340 * map->getTileSize()));
			monsters[i]->setInitPosition(glm::vec2(835 * map->getTileSize(), 340 * map->getTileSize()));
			break;
		case 20:
			monsters[i]->setPosition(glm::vec2(795 * map->getTileSize(), 342 * map->getTileSize()));
			monsters[i]->setInitPosition(glm::vec2(795 * map->getTileSize(), 342 * map->getTileSize()));
			break;
		case 21:
			monsters[i]->setPosition(glm::vec2(678 * map->getTileSize(), 371 * map->getTileSize()));
			monsters[i]->setInitPosition(glm::vec2(678 * map->getTileSize(), 371 * map->getTileSize()));
			break;
		case 22:
			monsters[i]->setPosition(glm::vec2(696 * map->getTileSize(), 374 * map->getTileSize()));
			monsters[i]->setInitPosition(glm::vec2(696 * map->getTileSize(), 374 * map->getTileSize()));
			break;
		case 23:
			monsters[i]->setPosition(glm::vec2(712 * map->getTileSize(), 372 * map->getTileSize()));
			monsters[i]->setInitPosition(glm::vec2(712 * map->getTileSize(), 372 * map->getTileSize()));
			break;
		//SLIMES
		case 24: 			
			monsters[i]->setPosition(glm::vec2(72 * map->getTileSize(), 220 * map->getTileSize()));
			monsters[i]->setInitPosition(glm::vec2(72 * map->getTileSize(), 220 * map->getTileSize()));
			break;
		case 25:
			monsters[i]->setPosition(glm::vec2(131 * map->getTileSize(), 224 * map->getTileSize()));
			monsters[i]->setInitPosition(glm::vec2(131 * map->getTileSize(), 224 * map->getTileSize()));
			break;
		case 26:
			monsters[i]->setPosition(glm::vec2(220 * map->getTileSize(), 238 * map->getTileSize()));
			monsters[i]->setInitPosition(glm::vec2(220 * map->getTileSize(), 238 * map->getTileSize()));
			break;
		case 27:
			monsters[i]->setPosition(glm::vec2(281 * map->getTileSize(), 301 * map->getTileSize()));
			monsters[i]->setInitPosition(glm::vec2(281 * map->getTileSize(), 301 * map->getTileSize()));
			break;
		case 28:
			monsters[i]->setPosition(glm::vec2(331 * map->getTileSize(), 322 * map->getTileSize()));
			monsters[i]->setInitPosition(glm::vec2(331 * map->getTileSize(), 322 * map->getTileSize()));
			break;
		case 29:
			monsters[i]->setPosition(glm::vec2(430 * map->getTileSize(), 315 * map->getTileSize()));
			monsters[i]->setInitPosition(glm::vec2(430 * map->getTileSize(), 315 * map->getTileSize()));
			break;
		case 30:
			monsters[i]->setPosition(glm::vec2(447 * map->getTileSize(), 323 * map->getTileSize()));
			monsters[i]->setInitPosition(glm::vec2(447 * map->getTileSize(), 323 * map->getTileSize()));
			break;
		case 31:
			monsters[i]->setPosition(glm::vec2(512 * map->getTileSize(), 352 * map->getTileSize()));
			monsters[i]->setInitPosition(glm::vec2(512 * map->getTileSize(), 352 * map->getTileSize()));
			break;
		case 32:
			monsters[i]->setPosition(glm::vec2(577 * map->getTileSize(), 351 * map->getTileSize()));
			monsters[i]->setInitPosition(glm::vec2(577 * map->getTileSize(), 351 * map->getTileSize()));
			break;
		case 33:
			monsters[i]->setPosition(glm::vec2(845 * map->getTileSize(), 267 * map->getTileSize()));
			monsters[i]->setInitPosition(glm::vec2(845 * map->getTileSize(), 297 * map->getTileSize()));
			break;
		case 34:
			monsters[i]->setPosition(glm::vec2(945 * map->getTileSize(), 308 * map->getTileSize()));
			monsters[i]->setInitPosition(glm::vec2(945 * map->getTileSize(), 308 * map->getTileSize()));
			break;

		default:
			break;
		}
		
		

	}
}

float Scene::distance(glm::vec2 pos1, glm::vec2 pos2){
	return sqrt(pow(pos1[0] - pos2[0], 2) + pow(pos1[1] - pos2[1], 2));
}