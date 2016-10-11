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
	STAND_LEFT, STAND_RIGHT, MOVE_LEFT, MOVE_RIGHT
};


void EyeBoss::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	//Creem els sprites necesaris
	spritesheet.loadFromFile("images/NPC_126.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(110, 135), glm::vec2(.43, .08), &spritesheet, &shaderProgram);
	//sprite = Sprite::createSprite(glm::ivec2(36, 36), glm::vec2(.25, .25), &spritesheet, &shaderProgram);
	modo = 0;
	numAtack = 0;
	numPersecusio = 0;
	rotateSprite = 0;
	delay = 5 * 60;
	patrullar = true;
	estat = 1;
	angle = 0;
	playerXanterior = Game::instance().getPlayerPos().x;

	tileMapDispl = tileMapPos;
	velocitat = 10;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEyeBoss.x), float(tileMapDispl.y + posEyeBoss.y)));

}

void EyeBoss::update(int deltaTime)
{
	sprite->update(deltaTime);
	angle += deltaTime;
	glm::vec2 playerPos = Game::instance().getPlayerPos();
	float direccioAtackXAux = (playerPos.x - posEyeBoss.x);
	float direccioAtackYAux = (playerPos.y - posEyeBoss.y);

	float dirModul = sqrt(pow(direccioAtackXAux, 2) + pow(direccioAtackYAux, 2));
	float direccioAux = direccioAtackXAux / dirModul;
	rotateSprite = -sin(direccioAux);
	if (direccioAtackYAux < 0) {
		rotateSprite =  -rotateSprite + M_PI;
	}
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
		else if (numAtack < 3)
			doAtack1();
		else doRecuperaPoiscio();
	}

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEyeBoss.x), float(tileMapDispl.y + posEyeBoss.y)));
	
}

void EyeBoss::render()
{
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
			posFinalAtack = glm::vec2(posEyeBoss.x + (1.5 * direccioAtackX), posEyeBoss.y + (1.5 * direccioAtackY));
			float dirModul = sqrt(pow(direccioAtackX, 2) + pow(direccioAtackY, 2));
			direccio = glm::vec2(direccioAtackX / dirModul, direccioAtackY / dirModul);
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


