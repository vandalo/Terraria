#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "StaticInterface.h"
#include "Scene.h"
#include "Game.h"

void StaticInterface::init(ShaderProgram &shaderProgram)
{
	spritesheet.loadFromFile("images/Heart.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheetInventary.loadFromFile("images/Inventory_Back12.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spriteHeart = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(1., 1.), &spritesheet, &shaderProgram);
	spriteBack10objects = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(1., 1.), &spritesheetInventary, &shaderProgram);

	life = Game::instance().getPlayerLife();
	maxLife = Game::instance().getPlayerMaxLife();
}

void StaticInterface::update(int deltaTime)
{
	//spriteHeart->update(deltaTime);
	//spriteBack10objects->update(deltaTime);
	life = Game::instance().getPlayerLife();
	maxLife = Game::instance().getPlayerMaxLife();
}

void StaticInterface::render()
{
	int cont = -1;
	for (int i = 0; i < maxLife; i++){
		if (i % 10 == 0){
			cont++;
		}
		spriteHeart->setPosition(glm::vec2(float(posXheart + ((i%10)*20)), float(posYheart - (cont*20))));
		spriteHeart->render();
	}
	for (int i = 0; i < 10; i++){
		spriteBack10objects->setPosition(glm::vec2(float(posX10objectsInventary + ((i % 10) * 35)), float(posY10objectsInventary)));
		spriteBack10objects->render();
	}
}

void StaticInterface::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

void StaticInterface::setPosition(const glm::vec2 &pos)
{
	posStaticInterface = pos;
	spriteHeart->setPosition(glm::vec2(float(posStaticInterface.x), float(posStaticInterface.y)));
}

int StaticInterface::getX(){
	return posStaticInterface.x;
}

int StaticInterface::getY(){
	return posStaticInterface.y;
}



