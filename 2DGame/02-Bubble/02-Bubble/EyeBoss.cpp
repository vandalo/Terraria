#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "EyeBoss.h"
#include "Game.h"


#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 40
#define FALL_STEP 0
#define M_PI 3.14159265358979323846264338327950288

enum EyeBossAnims
{
	STAND_ONE, ATACK_ONE, STAND_TWO, ATACK_TWO
};


void EyeBoss::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	//Creem els sprites necesaris
	spritesheet.loadFromFile("images/eye_1.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(110, 161), glm::vec2(.107, .63), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(4);

	sprite->setAnimationSpeed(STAND_ONE, 4);
	sprite->addKeyframe(STAND_ONE, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(STAND_ONE, glm::vec2(.107f, 0.f));

	sprite->setAnimationSpeed(ATACK_ONE, 8);
	sprite->addKeyframe(ATACK_ONE, glm::vec2(.214f, 0.f));

	sprite->setAnimationSpeed(STAND_TWO, 4);
	sprite->addKeyframe(STAND_TWO, glm::vec2(.321f, 0.f));
	sprite->addKeyframe(STAND_TWO, glm::vec2(.428f, 0.f));

	sprite->setAnimationSpeed(ATACK_TWO, 8);
	sprite->addKeyframe(ATACK_TWO, glm::vec2(.535f, 0.f));

	modo = 0;
	numAtack = 0;
	numPersecusio = 0;
	rotateSprite = 0;
	delay = 5 * 60;
	patrullar = true;
	estat = 1;
	angle = 0;
	vida = 20;
	playerXanterior = Game::instance().getPlayerPos().x;

	tileMapDispl = tileMapPos;
	velocitat = 10;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEyeBoss.x), float(tileMapDispl.y + posEyeBoss.y)));
	sprite->changeAnimation(0);

}

void EyeBoss::update(int deltaTime)
{
	if (vida > 0){
		sprite->update(deltaTime);
		angle += deltaTime;
		glm::vec2 playerPos = Game::instance().getPlayerPos();
		float direccioAtackXAux = (playerPos.x - posEyeBoss.x);
		float direccioAtackYAux = (playerPos.y - posEyeBoss.y);

		float dirModul = sqrt(pow(direccioAtackXAux, 2) + pow(direccioAtackYAux, 2));
		float direccioAux = direccioAtackXAux / dirModul;
		rotateSprite = -sin(direccioAux);
		if (direccioAtackYAux < 0) {
			rotateSprite = -rotateSprite + M_PI;
		}
		colisionPlayer();
		//Moviment per default, patrulla fins que te el player aprop
		int deltaXPlayer = playerPos.x - playerXanterior;
		playerXanterior = playerPos.x;
		if (modo == 0){
			float dist = getDistanciaEixX(playerPos, posEyeBoss);
			if ((dist < 150 && abs(playerPos.x - posEyeBoss.x) < 36) || !patrullar) modo++;
			else doPatrullar(angle);
		}
		//Ja ha detectat el player, i te mes del X% de la vida
		else if (modo == 1){
			xInicial += deltaXPlayer;
			if (patrullar){
				doPatrullar(angle);
				deltaXPlayer = 0;
			}
			else if (numAtack < 3){
				doAtack1();
				if (vida < 6)
					sprite->changeAnimation(3);
				else sprite->changeAnimation(1);
			}
			else {
				doRecuperaPoiscio();
				if (vida < 6)
					sprite->changeAnimation(2);
				else sprite->changeAnimation(0);
			}
		}

		sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEyeBoss.x), float(tileMapDispl.y + posEyeBoss.y)));
	}
}

void EyeBoss::render()
{
	if (vida > 0)
		sprite->render(rotateSprite);
}

void EyeBoss::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

void EyeBoss::setPosition(const glm::vec2 &pos)
{
	posEyeBoss = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEyeBoss.x), float(tileMapDispl.y + posEyeBoss.y)));
}

int EyeBoss::getX(){
	return posEyeBoss.x;
}

int EyeBoss::getY(){
	return posEyeBoss.y;
}

void EyeBoss::setPatrullar(bool bpatrullar){
	//patrullar = bpatrullar;
}

void EyeBoss::setEsMouDreta(bool besMouDreta){
	//esMouDreta = besMouDreta;
}
void EyeBoss::setVida(int vidaEyeBoss){
	vida = vidaEyeBoss;
}

void EyeBoss::setRadiPatrulla(int radi){
	//radiPatrulla = radi;
}

void EyeBoss::setRadiPerseguir(int radi){
	//radiPerseguir = radi;
}

void EyeBoss::setInitPosition(glm::vec2 pos){
	xInicial = pos[0];
	yInicial = pos[1];
}

void EyeBoss::setVelocitat(int vel){
	velocitat = vel;
}

//Volara a una distancia X del player fen moviments sinusoidals dins del radi definit
void EyeBoss::doPatrullar(int deltaTime, float deltaPlayerX){
	if (delay == 0){
		patrullar = false;
		numAtack = 0;
	}
	else{
		playerPos = Game::instance().getPlayerPos();
		posEyeBoss.y = (yInicial + 60 * (sin((angle / 5)*3.1416 / 180.f)));
		posEyeBoss.x = (xInicial + 150 * (sin((angle / 10)*3.1416 / 180.f)));
		if(modo != 0)delay--;
		if (delay < 0) delay = 0;
	}
}

void EyeBoss::doAtack1(){
	if (delay == 0){
		if (atackAcavat){
			glm::vec2 playerPos = Game::instance().getPlayerPos();
			direccioAtackX = (playerPos.x - posEyeBoss.x);
			direccioAtackY = (playerPos.y - posEyeBoss.y);
			float dirModul = sqrt(pow(direccioAtackX, 2) + pow(direccioAtackY, 2));
			direccio = glm::vec2(direccioAtackX / dirModul, direccioAtackY / dirModul);
			posFinalAtack = glm::vec2(posEyeBoss.x + (direccioAtackX + 300 * direccio.x), posEyeBoss.y + (direccioAtackY + 300 * direccio.y));
			atackAcavat = false;
		}
		//Moviment
		if (abs(posEyeBoss.y - posFinalAtack.y) < 5){
			atackAcavat = true;
			numAtack++;
			//60 FPS * 1.2 = 1.2 segons
			delay = (60 * 1.2);
		}
		else{
			posEyeBoss.x += velocitat * direccio.x;
			posEyeBoss.y += velocitat * direccio.y;
		}
	}
	else{
		delay--;
	}
	
}

void EyeBoss::doRecuperaPoiscio(){
	direccioAtackX = (xInicial - posEyeBoss.x);
	direccioAtackY = (yInicial - posEyeBoss.y);
	posFinalAtack = glm::vec2(posEyeBoss.x + direccioAtackX, posEyeBoss.y + direccioAtackY);
	float dirModul = sqrt(pow(direccioAtackX, 2) + pow(direccioAtackY, 2));
	direccio = glm::vec2(direccioAtackX / dirModul, direccioAtackY / dirModul);
	if (abs(posEyeBoss.y - posFinalAtack.y) < 10 && abs(posEyeBoss.y - posFinalAtack.y) < 10){
		patrullar = true;
		delay = 5 * 60;
		angle = 0;
	}
	else{
		if (abs(posEyeBoss.x - posFinalAtack.x) > 5)posEyeBoss.x += velocitat * direccio.x;
		if (abs(posEyeBoss.y - posFinalAtack.y) > 5)posEyeBoss.y += velocitat * direccio.y;
	}
}

float EyeBoss::getDistanciaEixX(glm::vec2 posPlayer, glm::vec2 posEyeBoss){
	return sqrt(pow(posPlayer[0] - posEyeBoss[0], 2) + pow(posPlayer[1] - posEyeBoss[1], 2));
}


bool EyeBoss::colision(){
	glm::vec2 playerBoxMin = Game::instance().getScene()->player->getBoundingBoxMin();
	glm::vec2 playerBoxMax = Game::instance().getScene()->player->getBoundingBoxMax();
	glm::vec2 EyeBossBoxMin = getBoundingBoxMin();
	glm::vec2 EyeBossBoxMax = getBoundingBoxMax();
	bool res = false;
	if (playerBoxMin.x < EyeBossBoxMax.x && EyeBossBoxMin.x < playerBoxMax.x){
		if (playerBoxMin.y < EyeBossBoxMax.y && EyeBossBoxMin.y < playerBoxMax.y){
			res = true;
		}
	}
	return res;
}

bool EyeBoss::colisionWeapon(){
	glm::vec2 weaponBoxMin = Game::instance().getScene()->player->getBoundingBoxMin();
	glm::vec2 weaponBoxMax = Game::instance().getScene()->player->getBoundingBoxMax();
	glm::vec2 EyeBossBoxMin = getBoundingBoxMin();
	glm::vec2 EyeBossBoxMax = getBoundingBoxMax();
	bool res = false;
	float increment;
	bool dreta = Game::instance().getScene()->player->getAnimation() == 1 || Game::instance().getScene()->player->getAnimation() == 3;
	if (dreta) {
		weaponBoxMax.x += 16;
		weaponBoxMin.x += 16;
	}
	else{
		weaponBoxMin.x -= 16;
		weaponBoxMax.x -= 16;
	}
	if (weaponBoxMin.x < EyeBossBoxMax.x && EyeBossBoxMin.x < weaponBoxMax.x){
		if (weaponBoxMin.y < EyeBossBoxMax.y && EyeBossBoxMin.y < weaponBoxMax.y){
			res = true;
		}
	}
	return res;
}

void EyeBoss::colisionPlayer(){
	if (Game::instance().getScene()->player->isAtacking()){
		if (colisionWeapon()){
			//esquerra
			if (Game::instance().getScene()->getPlayerPos().x > posEyeBoss.x){
				setPosition(glm::vec2(posEyeBoss.x - 40., posEyeBoss.y - 20));
				if (map->collisionMoveRight(Game::instance().getScene()->getPlayerPos(), glm::ivec2(32, 32)))
				{
					setPosition(glm::vec2(posEyeBoss.x + 40., posEyeBoss.y + 20));
				}
			}
			//dreata
			else{
				setPosition(glm::vec2(posEyeBoss.x + 40., posEyeBoss.y - 20));
				if (map->collisionMoveLeft(Game::instance().getScene()->getPlayerPos(), glm::ivec2(32, 32)))
				{
					setPosition(glm::vec2(posEyeBoss.x - 40., posEyeBoss.y + 20));
				}
			}
			vida -= Game::instance().getScene()->player->getAtack();
		}
	}
	//else{
	if (colision()){
		//esquerra
		if (Game::instance().getScene()->getPlayerPos().x > posEyeBoss.x){
			Game::instance().getScene()->player->setPosition(glm::vec2(Game::instance().getScene()->player->getX() + 40., Game::instance().getScene()->player->getY() - 20));
			if (map->collisionMoveRight(Game::instance().getScene()->getPlayerPos(), glm::ivec2(32, 32)))
			{
				Game::instance().getScene()->player->setPosition(glm::vec2(Game::instance().getScene()->player->getX() - 40., Game::instance().getScene()->player->getY() + 20));
			}
		}
		//dreata
		else{
			Game::instance().getScene()->player->setPosition(glm::vec2(Game::instance().getScene()->player->getX() - 40., Game::instance().getScene()->player->getY() - 20));
			if (map->collisionMoveLeft(Game::instance().getScene()->getPlayerPos(), glm::ivec2(32, 32)))
			{
				Game::instance().getScene()->player->setPosition(glm::vec2(Game::instance().getScene()->player->getX() + 40., Game::instance().getScene()->player->getY() + 20));
			}
		}
		Game::instance().getScene()->player->updateLife(-1);
	}
	//}
}

glm::vec2 EyeBoss::getBoundingBoxMin(){
	glm::vec2 mins = glm::vec2(posEyeBoss.x - sprite->getSpriteSize().x / 2, posEyeBoss.y - sprite->getSpriteSize().y / 2);
	return mins;
}

glm::vec2 EyeBoss::getBoundingBoxMax(){
	glm::vec2 maxs = glm::vec2(posEyeBoss.x + sprite->getSpriteSize().x / 2, posEyeBoss.y + sprite->getSpriteSize().y / 2);
	return maxs;
}
