#ifndef _MONSTER_INCLUDE
#define _MONSTER_INCLUDE


#include "Sprite.h"
#include "TileMap.h"


// Monster is basically a Sprite that represents the enemys. As such it has
// all properties it needs to track its movement, jumping, and collisions.


class Monster
{

public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();

	int getX();
	int getY();
	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);
	void setPatrullar(bool patrullar);
	void setEsMouDreta(bool esMouDreta);
	void setVida(int vida);
	void setRadiPatrulla(int radi);
	void setRadiPErseguir(int radi);
	void setInitPosition(glm::vec2 iniPos);
	void setVelocitat(int vel);
	void doPatrullar();
	void doPatrullarWithJump();
	void doPerseguir();
	void doPerseguirWithJump();
	float getDistancia(glm::vec2 posPlayer, glm::vec2 posMonster);

private:
	int vida;
	float velocitat;
	int radiPatrulla, radiPerseguir;
	//El boolean progres es per quan fagi una colisio intenti saltar un cop, 
	//si no progresa gira cua si progresa continua
	bool patrullar, esMouDreta, bJumping, progres, tryJump;
	int xInicial, yInicial;
	glm::ivec2 tileMapDispl, posMonster;
	int jumpAngle, startY, startX;
	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;

};


#endif // _MONSTER_INCLUDE


