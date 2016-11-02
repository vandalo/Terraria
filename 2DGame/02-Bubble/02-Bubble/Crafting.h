#pragma once

#include "Texture.h"
#include "Sprite.h"
#include "Define.h"
#include "Inventary.h"

struct Craft{
	int idItem;
	glm::vec2 textCord;
	bool crafteable;
	bool isObjectCraft;
	int itemsNeedId[3];
};

class Crafting
{
	Texture spriteSheetItems;
	ShaderProgram *texProgram2;
	Sprite *spriteItem;
	Craft arrayCraft[NUM_ITEMS];
	int itemPointerCraft;
	
public:
	Crafting(ShaderProgram &texProgram);
	~Crafting();
	void render();
	int getIdObejctToCraft();
	void craftItem(int idItem, ShaderProgram &texProgram);
	void update();
	void incrementPointer();
	void decrementPointer();

private:
	void setCrafingItems();
	bool isCrafeable(int idItem);
	glm::vec2 setSprite(int idItem);
	int setPointer(int pointer);
	int nextPointer(int pointer);
	void setItemsNeeded(int i);
};
