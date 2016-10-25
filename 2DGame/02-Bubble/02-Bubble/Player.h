#ifndef _PLAYER_INCLUDE
#define _PLAYER_INCLUDE


#include "Sprite.h"
#include "TileMap.h"


// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.


class Player
{

public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();
	
	int getX();
	int getY();
	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);
	int getLife();
	int getMaxLife();
	void setWeaponSprite(Sprite * weapon);
	void setActiveItem(int idItem);
	
private:


	int life, maxLife;
	bool bJumping;
	glm::ivec2 tileMapDispl, posPlayer;
	int jumpAngle, startY;
	Texture spritesheet;
	Sprite *sprite;
	Sprite *weaponSprite;
	TileMap *map;
	int activeItem;

};


#endif // _PLAYER_INCLUDE


