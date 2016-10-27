#pragma once

#include "Texture.h"
#include "Sprite.h"
#include "Define.h"

struct Item {
	int id;
	Sprite *spriteItem;
	//bool stackable;
};

class Inventary
{
	Item arrayInventary[SIZE_INVENTARY];
	Texture spriteSheetItems;
	ShaderProgram *texProgram2;
	bool hasChange;

public:
	void putItem(int idItem, int idCasella, ShaderProgram &texProgram);
	void setActiveItem(int position);
	void render(bool showDinamicInterface);
	Inventary(ShaderProgram &texProgram);
	void update();
	void moveItem(int idItem, int x, int y);
	int getId(int pos);
	void removeItem(int pos);
	void swapItem(int pos1, int pos2);
	int getFirstEmptySlot();
	bool hasChangeInventary(){ return hasChange; };
	bool hasItem(int idItem, int times);
	void removeItemNtimes(int idItem, int times);

private:
	void render10objects();
	void renderRestobjects();
	Item setItem(int idSprite, ShaderProgram &texProgram);
};

