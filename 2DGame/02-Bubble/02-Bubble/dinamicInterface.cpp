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
	spritesheetInventary.loadFromFile("images/Inventory_Back12.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheetInventarySmall.loadFromFile("images/Inventory_Back12_small.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spriteBackObjects = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(1., 1.), &spritesheetInventary, &shaderProgram);
	spriteBackSet = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(1., 1.), &spritesheetInventary, &shaderProgram);
	spriteBackChest = Sprite::createSprite(glm::ivec2(28, 28), glm::vec2(1., 1.), &spritesheetInventary, &shaderProgram);
	spritePossibleCraft = Sprite::createSprite(glm::ivec2(28, 28), glm::vec2(1., 1.), &spritesheetInventary, &shaderProgram);
	spriteObjectCraft = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(1., 1.), &spritesheetInventary, &shaderProgram);
	rubish = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(1., 1.), &spritesheetInventary, &shaderProgram);

	//Poiscions de la interficie
	posXobjectsInventary = 16;
	posYobjectsInventary = SCREEN_HEIGHT - 50;
	posXset = SCREEN_WIDTH - 50;
	posYset = 8*35;
	//16 = distancia de marge, 35 distancia de linventari superior, 4 diferencia entre 35-31
	posXchest = 16 + 35 + 4;
	posYchest = SCREEN_HEIGHT - 50 - 35*5; 
	posXcraftBasic = 16+4;
	posYcraftBasic = SCREEN_HEIGHT / 3 + 24;
	posXobjectsNeed = 16 + 4 + 35;
	posYobjectsNeed = SCREEN_HEIGHT / 3 + 24;
	posXrubish;
	posYrubish;
}

void DinamicInterface::update(int deltaTime)
{
	//spriteHeart->update(deltaTime);
	//spriteBackObjects->update(deltaTime);
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
		spriteBackSet->setPosition(glm::vec2(float(posXset), float(posYset - (i*35))));
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
	int increment = 31;
	for (int i = 0; i < 5; i++){
		spriteBackChest->setPosition(glm::vec2(float(posXcraftBasic), float(posYcraftBasic - increment*i)));
		if (i == 2){
			increment += 2;
			spriteObjectCraft->setPosition(glm::vec2(float(posXcraftBasic), float(posYcraftBasic - increment*i)));
			spriteObjectCraft->render();
			//drawCraftingRequiredItems
			for (int i2 = 0; i2 < 3; i2++){
				spriteBackChest->setPosition(glm::vec2(float(posXobjectsNeed + i2 * 31), float(posYcraftBasic - increment*i)));
				spriteBackChest->render();
			}
		}
		else{
			spriteBackChest->render();
		}
	}
	rubish->setPosition(glm::vec2(float(posXrubish), float(posYrubish)));
	rubish->render();
}

void DinamicInterface::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}



