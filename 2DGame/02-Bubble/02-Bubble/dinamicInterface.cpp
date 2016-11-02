#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "DinamicInterface.h"
#include "Game.h"


#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 96
#define FALL_STEP 4


enum DinamicInterfaceAnims
{
	STAND_LEFT, STAND_RIGHT, MOVE_LEFT, MOVE_RIGHT
};


void DinamicInterface::init(ShaderProgram &shaderProgram)
{
	spritesheetInventary.loadFromFile("images/Inventory.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheetInventarySmall.loadFromFile("images/Inventory_Back12_small.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spriteBackObjects = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(.1, 1.), &spritesheetInventary, &shaderProgram);
	spriteBackSet = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(.1, 1.), &spritesheetInventary, &shaderProgram);
	spriteBackSet->setTextCord(glm::vec2(.1*3,0));
	spriteObjectCraft = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(.1, 1.), &spritesheetInventary, &shaderProgram);
	spriteCraftingActive = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(.1, 1.), &spritesheetInventary, &shaderProgram);
	spriteCraftingActive->setTextCord(glm::vec2(0.1*2, 0.));
	rubish = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(.1, 1.), &spritesheetInventary, &shaderProgram);
	rubish->setTextCord(glm::vec2(.1*4,0.f));
	spriteSetGuide = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(.1, 1.), &spritesheetInventary, &shaderProgram);
	spriteHelp = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(.1, 1.), &spritesheetInventary, &shaderProgram);
	spriteHelp->setTextCord(glm::vec2(.1 * 8, 0.f));
	//16 = distancia de marge, 35 distancia de linventari superior, 4 diferencia entre 35-31
	posXrubish;
	posYrubish;
}

void DinamicInterface::update(int deltaTime)
{

}

void DinamicInterface::render(bool chest)
{
	//Draw backpack
	int cont = 0;
	for (int i = 0; i < 40; i++){
		if (i % 10 == 0){
			cont++;
		}
		spriteBackObjects->setPosition(glm::vec2(float(posXobjectsInventary + ((i % 10) * 35)), float(posYobjectsInventary - (cont * 35))));
		spriteBackObjects->render();
	}
	posXrubish = posXobjectsInventary + 315;
	posYrubish = posYobjectsInventary - ((cont + 1) * 35);
	//draw set
	for (int i = 0; i < 8; i++){
		spriteBackSet->setPosition(glm::vec2(float(posXset), float(posYset - (i * 35))));
		spriteBackSet->render();
	}
	cont = -1;
	//draw if open chest
	if (chest){
		for (int i = 0; i < 40; i++){
			if (i % 10 == 0){
				cont++;
			}
			spriteBackChest->setPosition(glm::vec2(float(posXchest + ((i % 10) * 31)), float(posYchest - cont * 31)));
			spriteBackChest->render();
		}
		posYrubish = posYchest - cont * 31 - 34;
	}
	//drawCraftingPosibilitys
	for (int i = 0; i < 5; i++){
		spriteObjectCraft->setPosition(glm::vec2(float(posXcraftBasic), float(posYcraftBasic - 35 * i)));
		if (i == 2){
			for (int i2 = 0; i2 < 3; i2++){
				spriteBackObjects->setPosition(glm::vec2(float(posXobjectsNeed + i2 * 35), float(posYcraftBasic - 35 * i)));
				spriteBackObjects->render();
			}
			spriteCraftingActive->setPosition(glm::vec2(float(posXcraftBasic), float(posYcraftBasic - 35 * i)));
			spriteCraftingActive->render();
		}
		else spriteObjectCraft->render();
	}

	spriteHelp->setPosition(glm::vec2(float(16), float(16)));
	spriteHelp->render(3.1416);
	

	spriteBackObjects->setPosition(glm::vec2(float(posXrubish), float(posYrubish)));
	spriteBackObjects->render();
	rubish->setPosition(glm::vec2(float(posXrubish), float(posYrubish)));
	rubish->render(3.1415);

	//Pintem siluetes de on ha d'anar cada part del set
	float movmentTextCord = (32.f / (float)320.);
	for (int i = 50; i < 53; i++){
		spriteSetGuide->setPosition(glm::vec2(float(posXset) - 35, float(posYset - ((i - 50) * 35))));
		if(i == 50)spriteSetGuide->setTextCord(glm::vec2((movmentTextCord * 6), 0.f));
		else if (i == 51)spriteSetGuide->setTextCord(glm::vec2((movmentTextCord * 5), 0.f));
		else spriteSetGuide->setTextCord(glm::vec2((movmentTextCord * 7), 0.f));
		spriteSetGuide->render(-3.1416);
	}
}

void DinamicInterface::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}
