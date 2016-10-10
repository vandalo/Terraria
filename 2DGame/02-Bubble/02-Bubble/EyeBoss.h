#ifndef _EyeBoss_INCLUDE
#define _EyeBoss_INCLUDE


#include "Sprite.h"
#include "TileMap.h"


// EyeBoss is basically a Sprite that represents the enemys. As such it has
// all properties it needs to track its movement, jumping, and collisions.


class EyeBoss
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
	void setRadiPerseguir(int radi);
	void setInitPosition(glm::vec2 iniPos);
	void setVelocitat(int vel);
	void doPatrullar(int deltaTime, float deltaPlayerX = 0);
	void doAtack1();
	void doRecuperaPoiscio();
	float getDistanciaEixX(glm::vec2 posPlayer, glm::vec2 posEyeBoss);

private:
	int playerXanterior;
	int vida;
	float velocitat;
	bool atackAcavat, patrullar;
	float direccioAtackX, direccioAtackY;
	int xInicial, yInicial;
	glm::ivec2 tileMapDispl, posEyeBoss;
	glm::vec2 playerPos, posFinalAtack, direccio;
	int startY, startX;
	int modo, estat, numAtack, numPersecusio, angle;
	int delay;
	float deltaPlayer;
	Texture spritesheet;
	Sprite *sprite;
	float rotateSprite;

	TileMap *map;

};


#endif // _EyeBoss_INCLUDE


