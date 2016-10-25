#ifndef _StaticInterface_INCLUDE
#define _StaticInterface_INCLUDE


#include "Sprite.h"
#include "TileMap.h"


// StaticInterface is basically a Sprite that represents the StaticInterface. As such it has
// all properties it needs to track its movement, jumping, and collisions.


class StaticInterface
{

public:
	void init(ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();

	int getX();
	int getY();
	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);

private:
	glm::ivec2 posStaticInterface;
	int life, maxLife;
	Texture spritesheet, spritesheetInventary;
	Sprite *spriteHeart, *spriteBack10objects;
	int posXheart, posYheart;
	
	TileMap *map;

};


#endif // _StaticInterface_INCLUDE


