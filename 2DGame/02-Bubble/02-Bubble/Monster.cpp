#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Monster.h"
#include "Game.h"


#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 40
#define FALL_STEP 4


enum MonsterAnims
{
	STAND_LEFT, STAND_RIGHT, MOVE_LEFT, MOVE_RIGHT, SLIME
};


void Monster::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	atack = 1;
	id = 1;
	deathTime = 0;
	vida = 20;
	velocitat = 0.7;
	maxLife = 20;
	spritesheet.loadFromFile("images/monsters.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(32, 34), glm::vec2(0.5, 0.156), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(5);

	sprite->setAnimationSpeed(STAND_LEFT, 8);
	sprite->addKeyframe(STAND_LEFT, glm::vec2(0.f, 0.f));

	sprite->setAnimationSpeed(STAND_RIGHT, 8);
	sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.5f, 0.f));

	sprite->setAnimationSpeed(MOVE_LEFT, 8);
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.156));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.3125));

	sprite->setAnimationSpeed(MOVE_RIGHT, 8);
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.5, 0.f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.5, 0.156));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.5, 0.3125));

	sprite->setAnimationSpeed(SLIME, 8);
	sprite->addKeyframe(SLIME, glm::vec2(0., 0.4685f));
	sprite->addKeyframe(SLIME, glm::vec2(0.5, 0.4685f));

	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	
	bJumping = false;
	progres = false;
	tryJump = false;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posMonster.x), float(tileMapDispl.y + posMonster.y)));

}

void Monster::setId(int idMonstre){
	id = idMonstre;
}
void Monster::update(int deltaTime)
{
	sprite->update(deltaTime);
	if (vida > 0){
		if (getDistancia(Game::instance().getPlayerPos(), glm::vec2(posMonster)) < radiPerseguir){
			patrullar = false;
			velocitat = 1;
		}
		else{
			patrullar = true;
			velocitat = 1;
		}
		if (patrullar){
			if(id == 1) 
				doPatrullarWithJump();
			else if (id == 2)
				doPatrullarWithJumpV2();
		}
		else{
			if (id == 1)
				doPerseguirWithJump();
			else if (id == 2)
				doPerseguirWithJumpV2();
		}
		colisionPlayer();
		sprite->setPosition(glm::vec2(float(tileMapDispl.x + posMonster.x), float(tileMapDispl.y + posMonster.y)));
	}
	else{
		//Just death
		if (deathTime == 0){
			deathTime = 60*360;
		}
		else if (deathTime > 0) deathTime -= deltaTime;
		else{
			deathTime = 0;
			vida = maxLife;
		}
	}
}

void Monster::render()
{
	if(vida > 0)
		sprite->render();
}

void Monster::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

void Monster::setPosition(const glm::vec2 &pos)
{
	posMonster = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posMonster.x), float(tileMapDispl.y + posMonster.y)));
}

int Monster::getX(){
	return posMonster.x;
}

int Monster::getY(){
	return posMonster.y;
}

void Monster::setPatrullar(bool bpatrullar){
	patrullar = bpatrullar;
}

void Monster::setEsMouDreta(bool besMouDreta){
	esMouDreta = besMouDreta;
}
void Monster::setVida(int vidaMonster){
	vida = vidaMonster;
}

void Monster::setRadiPatrulla(int radi){
	radiPatrulla = radi;
}

void Monster::setRadiPErseguir(int radi){
	radiPerseguir = radi;
}

void Monster::setInitPosition(glm::vec2 pos){
	xInicial = pos[0];
	yInicial = pos[1];
}

void Monster::setVelocitat(int vel){
	velocitat = vel;
}

void Monster::doPatrullar(){
	if (!esMouDreta)
	{
		if (xInicial - radiPatrulla > posMonster.x){
			setEsMouDreta(true);
		}
		else{
			if (sprite->animation() != MOVE_LEFT)
				sprite->changeAnimation(MOVE_LEFT);
			posMonster.x -= velocitat;
			if (map->collisionMoveLeft(posMonster, glm::ivec2(32, 32)))
			{
				posMonster.x += velocitat;
				sprite->changeAnimation(STAND_LEFT);
				setEsMouDreta(true);
			}
		}
	}
	else if (esMouDreta)
	{
		if (xInicial + radiPatrulla < posMonster.x){
			setEsMouDreta(false);
		}
		if (sprite->animation() != MOVE_RIGHT)
			sprite->changeAnimation(MOVE_RIGHT);
		posMonster.x += velocitat;
		if (map->collisionMoveRight(posMonster, glm::ivec2(32, 32)))
		{
			posMonster.x -= velocitat;
			sprite->changeAnimation(STAND_RIGHT);
			setEsMouDreta(false);
		}
	}
	posMonster.y += FALL_STEP;
	bJumping = false;
	map->collisionMoveDown(posMonster, glm::ivec2(32, 32), &posMonster.y);
}

void Monster::doPatrullarWithJump(){
	if (!esMouDreta)
	{
		if (xInicial - radiPatrulla > posMonster.x){
			setEsMouDreta(true);
			bJumping = false;
		}
		else{
			if (sprite->animation() != MOVE_LEFT)
				sprite->changeAnimation(MOVE_LEFT);
			posMonster.x -= velocitat;
			if (map->collisionMoveLeft(posMonster, glm::ivec2(32, 32)))
			{
				if (!tryJump && xInicial - radiPatrulla < posMonster.x){
					tryJump = true;
					bJumping = true;
					jumpAngle = 20;
					startY = posMonster.y;
					startX = posMonster.x;
				}
				else if (tryJump && !progres){
					if (xInicial - radiPatrulla < posMonster.x){
						posMonster.x += velocitat;
						//velocitat = 0;
						setEsMouDreta(true);
						bJumping = false;
						sprite->changeAnimation(STAND_LEFT);
					}
					else{
						setEsMouDreta(true);
					}
				}
				else if (tryJump && progres){
					tryJump = false;
					progres = false;
				}
			}
		}
	}
	else if (esMouDreta)
	{
		if (xInicial + radiPatrulla < posMonster.x){
			bJumping = false;
			setEsMouDreta(false);
		}
		if (sprite->animation() != MOVE_RIGHT)
			sprite->changeAnimation(MOVE_RIGHT);
		posMonster.x += velocitat;
		if (map->collisionMoveRight(posMonster, glm::ivec2(32, 32)))
		{
			if (!tryJump && xInicial + radiPatrulla < posMonster.x){
				tryJump = true;
				bJumping = true;
				jumpAngle = 8;
				startY = posMonster.y;
 			}
			else if (tryJump && !progres){
				if (xInicial + radiPatrulla < posMonster.x){
					posMonster.x -= velocitat;
					//velocitat = 0;
					sprite->changeAnimation(STAND_RIGHT);
					setEsMouDreta(false);
				}
			}
			else if (tryJump && progres){
				tryJump = false;
				progres = false;
			}
		}
	}

	if (bJumping)
	{
		if (!map->collisionMoveUp(posMonster, glm::ivec2(32, 32), &posMonster.y)){
			jumpAngle += JUMP_ANGLE_STEP;
			if (jumpAngle == 90)
			{
				bJumping = false;
				posMonster.y = startY;
			}
			else
			{
				posMonster.y = int((startY - 50 * sin(3.14159f * jumpAngle / 180.f)));
				if (jumpAngle > 90)
					bJumping = !map->collisionMoveDown(posMonster, glm::ivec2(32, 32), &posMonster.y);
			}
		}
		else
		{
			bJumping = false;
		}
	}
	else
	{
		posMonster.y += FALL_STEP;
		if (map->collisionMoveDown(posMonster, glm::ivec2(32, 32), &posMonster.y))
		{
			if (map->collisionMoveLeft(posMonster, glm::ivec2(32, 32)) || map->collisionMoveRight(posMonster, glm::ivec2(32, 32)))
			{
				bJumping = true;
				jumpAngle = 0;
				startY = posMonster.y;
			}
		}
	}
	if (startX != posMonster.x) progres = true;
	else progres = false;
}

void Monster::doPerseguir(){
	glm::vec2 playerPos = Game::instance().getPlayerPos();
	if (playerPos[0] < posMonster.x - velocitat){
		setEsMouDreta(false);
		if (!map->collisionMoveLeft(posMonster, glm::ivec2(32, 32)))
			posMonster.x -= velocitat;
		else sprite->changeAnimation(STAND_LEFT);
		if (sprite->animation() != MOVE_LEFT && !map->collisionMoveLeft(posMonster, glm::ivec2(32, 32)))
			sprite->changeAnimation(MOVE_LEFT);
	}
	else if (playerPos[0] > posMonster.x + velocitat){
		setEsMouDreta(true);
		if (!map->collisionMoveRight(posMonster, glm::ivec2(32, 32)))
			posMonster.x += velocitat;
		else sprite->changeAnimation(STAND_RIGHT);
		if (sprite->animation() != MOVE_RIGHT && !map->collisionMoveRight(posMonster, glm::ivec2(32, 32)))
			sprite->changeAnimation(MOVE_RIGHT);
	}
}

void Monster::doPerseguirWithJump(){
	glm::vec2 playerPos = Game::instance().getPlayerPos();
	if (playerPos[0] < posMonster.x - velocitat){
		setEsMouDreta(false);
		if (!map->collisionMoveLeft(posMonster, glm::ivec2(32, 32)))
			posMonster.x -= velocitat;
		else sprite->changeAnimation(STAND_LEFT);
		if (sprite->animation() != MOVE_LEFT && !map->collisionMoveLeft(posMonster, glm::ivec2(32, 32)))
			sprite->changeAnimation(MOVE_LEFT);
	}
	else if (playerPos[0] > posMonster.x + velocitat){
		setEsMouDreta(true);
		if (!map->collisionMoveRight(posMonster, glm::ivec2(32, 32)))
			posMonster.x += velocitat;
		else sprite->changeAnimation(STAND_RIGHT);
		if (sprite->animation() != MOVE_RIGHT && !map->collisionMoveRight(posMonster, glm::ivec2(32, 32)))
			sprite->changeAnimation(MOVE_RIGHT);
	}
	if (bJumping)
	{
		if (!map->collisionMoveUp(posMonster, glm::ivec2(32, 32), &posMonster.y)){
			jumpAngle += JUMP_ANGLE_STEP;
			if (jumpAngle == 180)
			{
				bJumping = false;
				posMonster.y = startY;
			}
			else
			{
				posMonster.y = int(startY - 50 * sin(3.14159f * jumpAngle / 180.f));
				if (jumpAngle > 90)
					bJumping = !map->collisionMoveDown(posMonster, glm::ivec2(32, 32), &posMonster.y);
			}
		}
		else
		{
			bJumping = false;
		}
	}
	else
	{
		posMonster.y += FALL_STEP;
		if (map->collisionMoveDown(posMonster, glm::ivec2(32, 32), &posMonster.y))
		{
			if (map->collisionMoveLeft(posMonster, glm::ivec2(32, 32)) || map->collisionMoveRight(posMonster, glm::ivec2(32, 32)))
			{
				bJumping = true;
				jumpAngle = 0;
				startY = posMonster.y;
			}
		}
	}

}

float Monster::getDistancia(glm::vec2 posPlayer, glm::vec2 posMonster){
	return sqrt(pow(posPlayer[0] - posMonster[0], 2) + pow(posPlayer[1] - posMonster[1], 2));
}

bool Monster::colision(){
	glm::vec2 playerBoxMin = Game::instance().getScene()->player->getBoundingBoxMin();
	glm::vec2 playerBoxMax = Game::instance().getScene()->player->getBoundingBoxMax();
	glm::vec2 monsterBoxMin = getBoundingBoxMin();
	glm::vec2 monsterBoxMax = getBoundingBoxMax();
	bool res = false;
	if (playerBoxMin.x < monsterBoxMax.x && monsterBoxMin.x < playerBoxMax.x){
		if (playerBoxMin.y < monsterBoxMax.y && monsterBoxMin.y < playerBoxMax.y){
			res = true;
		}
	}
	return res;
}

bool Monster::colisionWeapon(){
	glm::vec2 weaponBoxMin = Game::instance().getScene()->player->getBoundingBoxMin();
	glm::vec2 weaponBoxMax = Game::instance().getScene()->player->getBoundingBoxMax();
	glm::vec2 monsterBoxMin = getBoundingBoxMin();
	glm::vec2 monsterBoxMax = getBoundingBoxMax();
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
	if (weaponBoxMin.x < monsterBoxMax.x && monsterBoxMin.x < weaponBoxMax.x){
		if (weaponBoxMin.y < monsterBoxMax.y && monsterBoxMin.y < weaponBoxMax.y){
			res = true;
		}
	}
	return res;
}

void Monster::colisionPlayer(){
	if (Game::instance().getScene()->player->isAtacking()){
		if (colisionWeapon()){
			//esquerra
			if (Game::instance().getScene()->getPlayerPos().x > posMonster.x){
				setPosition(glm::vec2(posMonster.x - 40., posMonster.y - 20));
				if (map->collisionMoveRight(Game::instance().getScene()->getPlayerPos(), glm::ivec2(32, 32)))
				{
					setPosition(glm::vec2(posMonster.x + 40., posMonster.y + 20));
				}
			}
			//dreata
			else{
				setPosition(glm::vec2(posMonster.x + 40., posMonster.y - 20));
				if (map->collisionMoveLeft(Game::instance().getScene()->getPlayerPos(), glm::ivec2(32, 32)))
				{
					setPosition(glm::vec2(posMonster.x - 40., posMonster.y + 20));
				}
			}
			vida -= Game::instance().getScene()->player->getAtack();
		}
	}
	//else{
	if (colision()){
		//esquerra
		if (Game::instance().getScene()->getPlayerPos().x > posMonster.x){
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
		Game::instance().getScene()->player->updateLife(-atack);
	}
	//}
}

glm::vec2 Monster::getBoundingBoxMin(){
	glm::vec2 mins = glm::vec2(posMonster.x - sprite->getSpriteSize().x / 2, posMonster.y - sprite->getSpriteSize().y / 2);
	return mins;
}

glm::vec2 Monster::getBoundingBoxMax(){
	glm::vec2 maxs = glm::vec2(posMonster.x + sprite->getSpriteSize().x / 2, posMonster.y + sprite->getSpriteSize().y / 2);
	return maxs;
}


void Monster::doPatrullarWithJumpV2(){
	if (sprite->animation() != SLIME) sprite->changeAnimation(SLIME);
	if (!esMouDreta)
	{
		posMonster.x -= velocitat;
		if (map->collisionMoveLeft(posMonster, glm::ivec2(32, 32)))
		{
		setEsMouDreta(true);
		posMonster.x += velocitat;
		}
	}
	else if (esMouDreta)
	{
		posMonster.x += velocitat;
		if (map->collisionMoveRight(posMonster, glm::ivec2(32, 32)))
		{
			setEsMouDreta(false);
			posMonster.x -= velocitat;
		}
	}

	if (bJumping)
	{
		if (!map->collisionMoveUp(posMonster, glm::ivec2(32, 32), &posMonster.y)){
			jumpAngle += JUMP_ANGLE_STEP;
			if (jumpAngle == 90)
			{
				bJumping = false;
				posMonster.y = startY;
			}
			else
			{
				posMonster.y = int((startY - 150 * sin(3.14159f * jumpAngle / 180.f)));
				if (jumpAngle > 90)
					bJumping = !map->collisionMoveDown(posMonster, glm::ivec2(32, 32), &posMonster.y);
			}
		}
		else
		{
			bJumping = false;
		}
	}
	else
	{
		posMonster.y += FALL_STEP;
		if (map->collisionMoveDown(posMonster, glm::ivec2(32, 32), &posMonster.y))
		{
			bJumping = true;
			jumpAngle = 0;
			startY = posMonster.y;
		}
	}
}


void Monster::doPerseguirWithJumpV2(){
	glm::vec2 playerPos = Game::instance().getPlayerPos();
	if (playerPos[0] < posMonster.x - velocitat){
		setEsMouDreta(false);
		if (!map->collisionMoveLeft(posMonster, glm::ivec2(32, 32)))
			posMonster.x -= velocitat;
	}
	else if (playerPos[0] > posMonster.x + velocitat){
		setEsMouDreta(true);
		if (!map->collisionMoveRight(posMonster, glm::ivec2(32, 32)))
			posMonster.x += velocitat;
	}
	if (bJumping)
	{
		if (!map->collisionMoveUp(posMonster, glm::ivec2(32, 32), &posMonster.y)){
			jumpAngle += JUMP_ANGLE_STEP;
			if (jumpAngle == 180)
			{
				bJumping = false;
				posMonster.y = startY;
			}
			else
			{
				posMonster.y = int(startY - 150 * sin(3.14159f * jumpAngle / 180.f));
				if (jumpAngle > 90)
					bJumping = !map->collisionMoveDown(posMonster, glm::ivec2(32, 32), &posMonster.y);
			}
		}
		else
		{
			bJumping = false;
		}
	}
	else
	{
		posMonster.y += FALL_STEP;
		if (map->collisionMoveDown(posMonster, glm::ivec2(32, 32), &posMonster.y))
		{
			bJumping = true;
			jumpAngle = 0;
			startY = posMonster.y;
		}
	}

}