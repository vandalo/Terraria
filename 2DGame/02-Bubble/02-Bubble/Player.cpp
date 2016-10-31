#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Player.h"
#include "Game.h"


#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 96
#define FALL_STEP 4


enum PlayerAnims
{
	STAND_LEFT, STAND_RIGHT, MOVE_LEFT, MOVE_RIGHT
};


void Player::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	bJumping = false;
	atacking = false;
	atack = 1;
	life = 15;
	maxLife = 15;
	weaponSprite = NULL;
	activeItem = 1;
	angleWeapon = -2.5;
	spritesheet.loadFromFile("images/pj.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.5, 0.045), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(4);
	
		sprite->setAnimationSpeed(STAND_LEFT, 8);
		sprite->addKeyframe(STAND_LEFT, glm::vec2(0.f, 0.f));
		
		sprite->setAnimationSpeed(STAND_RIGHT, 8);
		sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.5f, 0.f));
		
		sprite->setAnimationSpeed(MOVE_LEFT, 12);
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.f));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.045f));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.09f));

		sprite->setAnimationSpeed(MOVE_RIGHT, 12);
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.5f, 0.f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.5f, 0.045f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.5f, 0.09f));
		
		
	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
	
}

void Player::update(int deltaTime)
{
	sprite->update(deltaTime);
	if(Game::instance().getSpecialKey(GLUT_KEY_LEFT))
	{
		if(sprite->animation() != MOVE_LEFT)
			sprite->changeAnimation(MOVE_LEFT);
		    posPlayer.x -= 4;
		if(map->collisionMoveLeft(posPlayer, glm::ivec2(32, 32)))
		{
			posPlayer.x += 4;
			sprite->changeAnimation(STAND_LEFT);
		}
	}
	else if(Game::instance().getSpecialKey(GLUT_KEY_RIGHT))
	{
		if(sprite->animation() != MOVE_RIGHT)
			sprite->changeAnimation(MOVE_RIGHT);
		posPlayer.x += 4;
		if(map->collisionMoveRight(posPlayer, glm::ivec2(32, 32)))
		{
			posPlayer.x -= 4;
			sprite->changeAnimation(STAND_RIGHT);
		}
	}
	else
	{
		if(sprite->animation() == MOVE_LEFT)
			sprite->changeAnimation(STAND_LEFT);
		else if(sprite->animation() == MOVE_RIGHT)
			sprite->changeAnimation(STAND_RIGHT);
	}
	
	if(bJumping)
	{
		if (!map->collisionMoveUp(posPlayer, glm::ivec2(32, 32), &posPlayer.y)){
			jumpAngle += JUMP_ANGLE_STEP;
			if(jumpAngle == 180)
			{
				bJumping = false;
				posPlayer.y = startY;
			}
			else
			{
				posPlayer.y = int(startY - 96 * sin(3.14159f * jumpAngle / 180.f));
				if (jumpAngle > 90)
					bJumping = !map->collisionMoveDown(posPlayer, glm::ivec2(32, 32), &posPlayer.y);
			}
		}
		else
		{
			bJumping = false;
		}
	}
	else
	{
		posPlayer.y += FALL_STEP;
		if(map->collisionMoveDown(posPlayer, glm::ivec2(32, 32), &posPlayer.y))
		{
			if(Game::instance().getSpecialKey(GLUT_KEY_UP))
			{
				bJumping = true;
				jumpAngle = 0;
				startY = posPlayer.y;
			}
		}
	}
	
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

void Player::setWeaponSprite(Sprite * weapon){
	weaponSprite = weapon;
}


void Player::render()
{
	sprite->render();
	if (weaponSprite != NULL){
		
		if (sprite->animation() == STAND_RIGHT || sprite->animation() == MOVE_RIGHT){
			weaponSprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x + 16), float(tileMapDispl.y + posPlayer.y + 8)));
			weaponSprite->render(angleWeapon + 3.1416, 0.);

		}
		else{
			weaponSprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x - 16), float(tileMapDispl.y + posPlayer.y + 8)));
			weaponSprite->render(angleWeapon,0.);
		}
	}
}

void Player::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

void Player::setPosition(const glm::vec2 &pos)
{
	posPlayer = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

int Player::getX(){
	return posPlayer.x;
}

int Player::getY(){
	return posPlayer.y;
}

int Player::getLife(){
	return life;
}
int Player::getMaxLife(){
	return maxLife;
}

void Player::setActiveItem(int idItem){
	activeItem = idItem;
}

void Player::upgareAngleWeapon(){
	angleWeapon += 0.3;
	if (angleWeapon > -1.5) angleWeapon = -3.;
}

void Player::setAngleWeapon(){
	angleWeapon = -2.4;
}

void Player::setAtacking(){
	atacking = true;
}

void Player::unsetAtacking(){
	atacking = false;
}

glm::vec2 Player::getBoundingBoxMin(){
	glm::vec2 mins = glm::vec2(posPlayer.x - 16, posPlayer.y - 16);
	return mins;
}

glm::vec2 Player::getBoundingBoxMax(){
	glm::vec2 maxs = glm::vec2(posPlayer.x + 16, posPlayer.y + 16);
	return maxs;
}

void Player::updateLife(int diff){
	life += diff;
}

void Player::updatePlayerSet(){
	//Check inventary set position (50-58)
	for (int i = 50; i < 58; i++){
		if (Game::instance().getScene()->getInventary()->getId(i) != 0){
			//TODO: 
		}
	}
}