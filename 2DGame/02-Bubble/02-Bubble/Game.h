#ifndef _GAME_INCLUDE
#define _GAME_INCLUDE


#include "Scene.h"
#include "MainMenu.h"
#include "Snake.h"
#include "HelpMenu.h"
#include "DeathMenu.h"
#include "WinGame.h"


#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 600

// Game is a singleton (a class with a single instance) that represents our whole application

#define  GLUT_LEFT_BUTTON                   0x0000
#define  GLUT_MIDDLE_BUTTON                 0x0001
#define  GLUT_RIGHT_BUTTON                  0x0002

class Game
{
	struct Mouse{
		bool button[5];
		int x, y;
	};
public:


	Game() {}
	
	
	static Game &instance()
	{
		static Game G;
	
		return G;
	}
	
	void init();
	bool update(int deltaTime);
	void render();
	
	// Input callback methods
	void keyPressed(int key);
	void keyReleased(int key);
	void specialKeyPressed(int key);
	void specialKeyReleased(int key);
	void mouseMove(int x, int y);
	void mousePress(int button);
	void mouseRelease(int button);
	
	bool getKey(int key) const;
	bool getSpecialKey(int key) const;
	void getScreenMousePos(int * x, int * y);
	void getWorldMousePos(double * x, double * y, double left, double bottom);
	bool isMousePressed(int button);
	void setModeGame();
	void setModeGameWithOutInit();
	void setModeSnake();
	void setModeDeath();
	void setModeMenu();
	void setModeHelp();
	void setModeWin();
	void setPlayerItem(int idItem, glm::vec2 texCord);
	int getMovingItem(){ return scene.getMovingItem(); };
	ShaderProgram getShaderProgram(){ return scene.getShaderProgram(); };
	bool hasObject(int idItem, int times);
	Scene* getScene(){ return &scene; };

	glm::vec2 getPlayerPos();
	int getPlayerLife();
	int getPlayerMaxLife();
	int getFood(){ return snake.getFood(); };

private:
	enum sceneStates
	{
		MAINMENU, GAME, CREDITS, HELP, SNAKE, DEATH, WIN
	};

	bool bPlay;                       // Continue to play game?
	sceneStates screen;				  //wich screen we mush show
	Snake snake;					  //Easter Egg snake
	Scene scene;                      // Scene to render
	MainMenu mainMenu;			      // Main menu to render
	HelpMenu help;				      //Help menu
	WinGame winGame;
	DeathMenu deathMenu;
	bool keys[256], specialKeys[256]; // Store key states so that 
	                                  // we can have access at any time
	Mouse mouse;
};


#endif // _GAME_INCLUDE


