#ifndef _DinamicInterface_INCLUDE
#define _DinamicInterface_INCLUDE


#include "Sprite.h"
#include "TileMap.h"


// dinamicInterface is basically a Sprite that represents the dinamicInterface. As such it has
// all properties it needs to track its movement, jumping, and collisions.


class DinamicInterface
{

public:
	void init(ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render(bool chest);

	int getX();
	int getY();
	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);

private:
	glm::ivec2 posdinamicInterface;
	Texture spritesheetInventary, spritesheetInventarySmall;
	Sprite *spriteBackObjects, *spriteBackSet, *spritePossibleCraft, *spriteObjectCraft, *spriteBackChest, *rubish, *spriteCraftingActive;
		/* *spriteHelmet, *spriteArmor, *spriteBoots*/
	int posXrubish, posYrubish;


	TileMap *map;

};

#endif // _dinamicInterface_INCLUDE


