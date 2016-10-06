#ifndef _SKULL_INCLUDE
#define _SKULL_INCLUDE


#include "Sprite.h"
#include "TileMap.h"


// Skull is basically a Sprite that represents the enemys. As such it has
// all properties it needs to track its movement, jumping, and collisions.


class Skull
{

public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();

	int getX();
	int getY();
	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);
	void setPositionBracD1(const glm::vec2 &pos);
	void setPositionBracD2(const glm::vec2 &pos);
	void setPositionBracE1(const glm::vec2 &pos);
	void setPositionBracE2(const glm::vec2 &pos);
	void setPositionMaD(const glm::vec2 &pos);
	void setPositionMaE(const glm::vec2 &pos);

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
	float getDistancia(glm::vec2 posPlayer, glm::vec2 posSkull);

private:
	int vida, vidaD, vidaE;
	float velocitat;
	int xInicial, yInicial;
	glm::ivec2 tileMapDispl, posSkull, posBracD1, posBracD2, posBracE1, posBracE2, posMaD, posMaE;;
	
	Texture spritesheet;
	Sprite *sprite, *bracD1, *bracD2, *bracE1, *bracE2, *maD, *maE;
	float rotateSprite, rotateBracD1, rotateBracD2, rotateBracE1, rotateBracE2, rotateMaD, rotateMaE;

	TileMap *map;

};


#endif // _Skull_INCLUDE


