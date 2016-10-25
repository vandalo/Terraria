#pragma once
#include "Sprite.h"

#define PICK 1
#define WOODEN_SWORD 2

class Object
{
	int id;
	Sprite *sprite;
	int atack;
	int defence;
	int velocity;

public:
	Object();
	void setPropertis(int id);
	~Object();
};

