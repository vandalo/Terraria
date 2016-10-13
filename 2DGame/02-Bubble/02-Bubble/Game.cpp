#include <GL/glew.h>
#include <GL/glut.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Game.h"


void Game::init()
{
	bPlay = true;
	glClearColor(0.4f, 0.0f, 0.9f, 0.3f);
	scene.init();
}

bool Game::update(int deltaTime)
{
	scene.update(deltaTime);
	
	return bPlay;
}

void Game::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	scene.render();
}

void Game::keyPressed(int key)
{
	if(key == 27) // Escape code
		bPlay = false;
	if (key == 'i')
		scene.changeModeInterface();
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
}

void Game::mouseRelease(int button)
{
	mouse.button[button] = false;
}

void Game::getScreenMousePos(int * x, int * y) {
	*x = mouse.x;
	*y = mouse.y;
}

void Game::getWorldMousePos(double * x, double * y) {
	float mouse_current_z;
	glReadPixels(mouse.x, mouse.y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &mouse_current_z);
	glm::vec3 windowCoordinates = glm::vec3(mouse.x, mouse.y, mouse_current_z);
	glm::vec4 viewport = glm::vec4(0.0f, 0.0f, (float)SCREEN_WIDTH, (float)SCREEN_HEIGHT);

	GLdouble modelviewArray[16]; //var to hold the modelview info
	GLdouble projectionArray[16]; //var to hold the projection matrix info

	glGetDoublev(GL_MODELVIEW_MATRIX, modelviewArray); //get the modelview info
	glGetDoublev(GL_PROJECTION_MATRIX, projectionArray); //get the projection matrix info

	glm::mat4 modelview = glm::make_mat4(modelviewArray);
	glm::mat4 projection = glm::make_mat4(projectionArray);

	glm::vec3 worldCoordinates = glm::unProject(windowCoordinates, modelview, projection, viewport);
	printf("(%f, %f, %f)\n", worldCoordinates.x, worldCoordinates.y, worldCoordinates.z);

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


