#pragma once

#include "Texture.h"
#include "Sprite.h"
#include "Define.h"
#include "Inventary.h"

struct Craft{
	int idItem;
	Sprite *spriteItem;
	bool crafteable;
};

class Crafting
{
	Texture spriteSheetItems;
	ShaderProgram *texProgram2;
	Craft arrayCraft[NUM_ITEMS];
	int itemPointerCraft;
	
public:
	Crafting(ShaderProgram &texProgram);
	~Crafting();
	void render();
	int getIdObejctToCraft();
	void craftItem(int idItem, ShaderProgram &texProgram);
	void update();

private:
	void setCrafingItems();
	bool isCrafeable(int idItem);
	Sprite *setSprite(int idItem, ShaderProgram &texProgram);
	int setPointer(int pointer);
	int nextPointer(int pointer);
};
