#pragma once
#define SIZE_INVENTARY 50

#define PICK 1
#define WOODEN_SWORD 2

#include "Texture.h"
#include "Sprite.h"

struct Item {
	int id;
	Sprite *spriteItem;
};

class Inventary
{
	Item arrayInventary[SIZE_INVENTARY];
	Texture spriteSheetItems;
	ShaderProgram *texProgram2;

public:
	void putItem(int idItem, int idCasella, ShaderProgram &texProgram);
	void setActiveItem(int position);
	void render();
	Inventary(ShaderProgram &texProgram);
	void update();
	void moveItem(int idItem, int x, int y);
	int getId(int pos);
	void removeItem(int pos);
	void swapItem(int pos1, int pos2);
private:
	void render10objects();
	Sprite *setSprite(int idSprite, ShaderProgram &texProgram);
};

