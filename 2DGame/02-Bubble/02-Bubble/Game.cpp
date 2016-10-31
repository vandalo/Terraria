#include <GL/glew.h>
#include <GL/glut.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Game.h"

void Game::init()
{
	bPlay = true;
	glClearColor(0.4f, 0.0f, 0.9f, 0.3f);
	
	mainMenu.init();
	snake.init();
	screen = MAINMENU;
	//screen = GAME;
}

bool Game::update(int deltaTime)
{
	switch (screen){
	case MAINMENU:
		mainMenu.update(deltaTime);
		break;
	case GAME:
		scene.update(deltaTime);
		break;
	case SNAKE:
		snake.update(deltaTime);
		break;
	}
	return bPlay;
}

void Game::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	switch (screen){
		case MAINMENU:
			mainMenu.render();
			break;
		case GAME:
			scene.render();
			break;
		case SNAKE:
			snake.render();
	}
}

void Game::keyPressed(int key)
{
	if(key == 27) // Escape code
		bPlay = false;
	if (key == 'i' || key == 'I')
		scene.changeModeInterface();
	if (key == '1')
		scene.setPlayerItem(0);
	if (key == '2')
		scene.setPlayerItem(1);
	if (key == '3')
		scene.setPlayerItem(2);
	if (key == '4')
		scene.setPlayerItem(3);
	if (key == '5')
		scene.setPlayerItem(4);
	if (key == '6')
		scene.setPlayerItem(5);
	if (key == '7')
		scene.setPlayerItem(6);
	if (key == '8')
		scene.setPlayerItem(7);
	if (key == '9')
		scene.setPlayerItem(8);
	if (key == '0')
		scene.setPlayerItem(9);
	keys[key] = true;
}

void Game::keyReleased(int key)
{
	keys[key] = false;
}

void Game::specialKeyPressed(int key)
{
	specialKeys[key] = true;
}

void Game::specialKeyReleased(int key)
{
	specialKeys[key] = false;
}

void Game::mouseMove(int x, int y)
{
	mouse.x = x;
	mouse.y = y;
}

void Game::mousePress(int button)
{
	mouse.button[button] = true;
	scene.mousePress();
}

void Game::mouseRelease(int button)
{
	mouse.button[button] = false;
	scene.mouseRealease();
}

void Game::getScreenMousePos(int * x, int * y) {
	*x = mouse.x;
	*y = mouse.y;
}



void Game::getWorldMousePos(double * x, double * y, glm::mat4 &modelview, glm::mat4 &projectionMatrix) {
	float mouse_current_z;
	glReadPixels(mouse.x, mouse.y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &mouse_current_z);
	glm::vec3 windowCoordinates = glm::vec3((double)mouse.x,(double)( SCREEN_HEIGHT - mouse.y), double(mouse_current_z));	

	GLint m_viewport[4];

	glGetIntegerv(GL_VIEWPORT, m_viewport);

	glm::vec4 viewport = glm::make_vec4( m_viewport);


	glm::vec3 worldCoordinates = glm::unProject(windowCoordinates, modelview, projectionMatrix, viewport);
	//printf("(%f, %f, %f)\n", worldCoordinates.x, worldCoordinates.y, worldCoordinates.z);
	

	*x = worldCoordinates.x;
	*y = worldCoordinates.y;

}

bool Game::isMousePressed(int button) {
	return mouse.button[button];
}

bool Game::getKey(int key) const
{
	return keys[key];
}

bool Game::getSpecialKey(int key) const
{
	return specialKeys[key];
}

glm::vec2 Game::getPlayerPos(){
	return scene.getPlayerPos();
}

int Game::getPlayerLife(){
	return scene.getPlayerLife();
}

int Game::getPlayerMaxLife(){
	return scene.getPlayerMaxLife();
}

void Game::setPlayerItem(int idItem, Sprite * sprite){
	scene.player->setWeaponSprite(sprite);
	scene.player->setActiveItem(idItem);
}

void Game::setModeGame(){
	scene.init();
	screen = GAME;
}

void Game::setModeSnake(){
	screen = SNAKE;
}
