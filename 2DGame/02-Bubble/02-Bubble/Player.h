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
	int getActiveItem(){ return activeItem; };
	void upgareAngleWeapon();
	void setAngleWeapon();
	void setAtacking();
	void unsetAtacking();
	bool isAtacking(){ return atacking; };
	glm::vec2 getBoundingBoxMin();
	glm::vec2 getBoundingBoxMax();
	int getAnimation(){ return sprite->animation(); };
	void updateLife(int diff);
	int getAtack(){ return atack; };
	void updatePlayerSet();
	
private:


	int life, maxLife;
	int atack;
	bool bJumping, atacking;
	glm::ivec2 tileMapDispl, posPlayer;
	int jumpAngle, startY;
	Texture spritesheet;
	Sprite *sprite;
	Sprite *weaponSprite;
	TileMap *map;
	int activeItem;
	float angleWeapon;

};


#endif // _PLAYER_INCLUDE


