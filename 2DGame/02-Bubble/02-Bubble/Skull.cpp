#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Skull.h"
#include "Game.h"


#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 40
#define FALL_STEP 0
#define M_PI 3.14159265358979323846264338327950288

enum SkullAnims
{
	STAND_LEFT, STAND_RIGHT, MOVE_LEFT, MOVE_RIGHT
};


void Skull::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	//Creem els sprites necesaris
	spritesheet.loadFromFile("images/bub.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.25, 0.25), &spritesheet, &shaderProgram);
	bracD1 = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.25, 0.25), &spritesheet, &shaderProgram);
	bracD2 = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.25, 0.25), &spritesheet, &shaderProgram);
	bracE1 = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.25, 0.25), &spritesheet, &shaderProgram);
	bracE2 = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.25, 0.25), &spritesheet, &shaderProgram);
	maD = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.25, 0.25), &spritesheet, &shaderProgram);
	maE = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.25, 0.25), &spritesheet, &shaderProgram);

	rotateSprite = (float)M_PI;
	rotateBracD1 = 0;
	rotateBracD2 = 0;
	rotateBracE1 = 0;
	rotateBracE2 = 0;
	rotateMaD = 0;
	rotateMaE = 0;

	tileMapDispl = tileMapPos;
	velocitat = 1;
	//Setejem posicio inicial
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posSkull.x), float(tileMapDispl.y + posSkull.y)));
	bracD1->setPosition(glm::vec2(float(tileMapDispl.x + posBracD1.x), float(tileMapDispl.y + posBracD1.y)));
	bracD2->setPosition(glm::vec2(float(tileMapDispl.x + posBracD2.x), float(tileMapDispl.y + posBracD2.y)));
	bracE1->setPosition(glm::vec2(float(tileMapDispl.x + posBracE1.x), float(tileMapDispl.y + posBracE1.y)));
	bracE2->setPosition(glm::vec2(float(tileMapDispl.x + posBracE2.x), float(tileMapDispl.y + posBracE2.y)));
	maD->setPosition(glm::vec2(float(tileMapDispl.x + posMaD.x), float(tileMapDispl.y + posMaD.y)));
	maE->setPosition(glm::vec2(float(tileMapDispl.x + posMaE.x), float(tileMapDispl.y + posMaE.y)));

}

void Skull::update(int deltaTime)
{
	sprite->update(deltaTime);
	bracD1->update(deltaTime);
	bracD2->update(deltaTime);
	bracE1->update(deltaTime);
	bracE2->update(deltaTime);
	maD->update(deltaTime);
	maE->update(deltaTime);

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posSkull.x), float(tileMapDispl.y + posSkull.y)));
	bracD1->setPosition(glm::vec2(float(tileMapDispl.x + posBracD1.x), float(tileMapDispl.y + posBracD1.y)));
	bracD2->setPosition(glm::vec2(float(tileMapDispl.x + posBracD2.x), float(tileMapDispl.y + posBracD2.y)));
	bracE1->setPosition(glm::vec2(float(tileMapDispl.x + posBracE1.x), float(tileMapDispl.y + posBracE1.y)));
	bracE2->setPosition(glm::vec2(float(tileMapDispl.x + posBracE2.x), float(tileMapDispl.y + posBracE2.y)));
	maD->setPosition(glm::vec2(float(tileMapDispl.x + posMaD.x), float(tileMapDispl.y + posMaD.y)));
	maE->setPosition(glm::vec2(float(tileMapDispl.x + posMaE.x), float(tileMapDispl.y + posMaE.y)));
}

void Skull::render()
{
	sprite->render(rotateSprite);
	bracD1->render(rotateBracD1);
	bracD2->render(rotateBracD2);
	bracE1->render(rotateBracE1);
	bracE2->render(rotateBracE2);
	maD->render(rotateMaD);
	maE->render(rotateMaE);
}

void Skull::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

void Skull::setPosition(const glm::vec2 &pos)
{
	posSkull = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posSkull.x), float(tileMapDispl.y + posSkull.y)));
}

void Skull::setPositionBracD1(const glm::vec2 &pos)
{
	posBracD1 = pos;
	bracD1->setPosition(glm::vec2(float(tileMapDispl.x + posBracD1.x), float(tileMapDispl.y + posBracD1.y)));
}

void Skull::setPositionBracD2(const glm::vec2 &pos)
{
	posBracD2 = pos;
	bracD2->setPosition(glm::vec2(float(tileMapDispl.x + posBracD2.x), float(tileMapDispl.y + posBracD2.y)));
}

void Skull::setPositionBracE1(const glm::vec2 &pos)
{
	posBracE1 = pos;
	bracE1->setPosition(glm::vec2(float(tileMapDispl.x + posBracE1.x), float(tileMapDispl.y + posBracE1.y)));
}

void Skull::setPositionBracE2(const glm::vec2 &pos)
{
	posBracE2 = pos;
	bracE2->setPosition(glm::vec2(float(tileMapDispl.x + posBracE2.x), float(tileMapDispl.y + posBracE2.y)));
}

void Skull::setPositionMaD(const glm::vec2 &pos)
{
	posMaD = pos;
	maD->setPosition(glm::vec2(float(tileMapDispl.x + posMaD.x), float(tileMapDispl.y + posMaD.y)));
}

void Skull::setPositionMaE(const glm::vec2 &pos)
{
	posMaE = pos;
	maE->setPosition(glm::vec2(float(tileMapDispl.x + posMaE.x), float(tileMapDispl.y + posMaE.y)));
}

int Skull::getX(){
	return posSkull.x;
}

int Skull::getY(){
	return posSkull.y;
}

void Skull::setPatrullar(bool bpatrullar){
	//patrullar = bpatrullar;
}

void Skull::setEsMouDreta(bool besMouDreta){
	//esMouDreta = besMouDreta;
}
void Skull::setVida(int vidaSkull){
	vida = vidaSkull;
}

void Skull::setRadiPatrulla(int radi){
	//radiPatrulla = radi;
}

void Skull::setRadiPErseguir(int radi){
	//radiPerseguir = radi;
}

void Skull::setInitPosition(glm::vec2 pos){
	xInicial = pos[0];
	yInicial = pos[1];
}

void Skull::setVelocitat(int vel){
	velocitat = vel;
}

void Skull::doPatrullar(){
	
}

void Skull::doPatrullarWithJump(){
	
}

void Skull::doPerseguir(){
	
}

void Skull::doPerseguirWithJump(){
	
}

float Skull::getDistancia(glm::vec2 posPlayer, glm::vec2 posSkull){
	return sqrt(pow(posPlayer[0] - posSkull[0], 2) + pow(posPlayer[1] - posSkull[1], 2));
}


