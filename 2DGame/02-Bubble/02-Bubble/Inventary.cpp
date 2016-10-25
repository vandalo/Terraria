#include "Inventary.h"
#include "Game.h"

Inventary::Inventary(ShaderProgram &texProgram)
{
	for (int i = 0; i < SIZE_INVENTARY; i++){
		arrayInventary[i].id = 0;
		arrayInventary[i].spriteItem = NULL;
	}

	spriteSheetItems.loadFromFile("images/items.png", TEXTURE_PIXEL_FORMAT_RGBA);
	//TESTING
	arrayInventary[0].id = PICK;
	//arrayInventary[0].spriteItem = Sprite::createSprite(glm::ivec2(32.f, 32.f), glm::vec2(0.5f, 1.f), &spriteSheetItems, &texProgram2);
	putItem(PICK, 0, texProgram);
	arrayInventary[1].id = WOODEN_SWORD;
	putItem(WOODEN_SWORD, 1, texProgram);
	//arrayInventary[1].spriteItem = Sprite::createSprite(glm::ivec2(32.f, 32.f), glm::vec2(0.5f, 1.f), &spriteSheetItems, &texProgram);
	//arrayInventary[1].spriteItem->setTextCord(glm::vec2(0.5f, 0.f));

}

void Inventary::putItem(int idItem, int idCasella, ShaderProgram &texProgram){
	arrayInventary[idCasella].id = idItem;
	arrayInventary[idCasella].spriteItem = setSprite(idItem, texProgram);
}

void Inventary::setActiveItem(int position){
	Item item = arrayInventary[position];
	Game::instance().setPlayerItem(item.id, item.spriteItem);
}

void Inventary::render(){
	render10objects();
}

void Inventary::update(){
	for (int i = 0; i < 10; i++){
		if (arrayInventary[i].id != 0){
			//arrayInventary[i].spriteItem->setPosition(glm::vec2(float(posX10objectsInventary + ((i % 10) * 35)), float(posY10objectsInventary)));
			arrayInventary[i].spriteItem->render();
		}
	}
}

void Inventary::render10objects(){
	int idMovingItem = Game::instance().getMovingItem();
	for (int i = 0; i < 10; i++){
		if (arrayInventary[i].id != 0 && idMovingItem != i){
			arrayInventary[i].spriteItem->setPosition(glm::vec2(float(posX10objectsInventary + ((i % 10) * 35)), float(posY10objectsInventary)));
			arrayInventary[i].spriteItem->render();
		}
		else if (idMovingItem == i) arrayInventary[i].spriteItem->render();
	}
}

void Inventary::moveItem(int idItem, int x, int y){
	if (arrayInventary[idItem].id != -1){
		arrayInventary[idItem].spriteItem->setPosition(glm::vec2(x, y));
	}
}

Sprite *Inventary::setSprite(int idItem, ShaderProgram &texProgram){
	switch (idItem){
	case PICK:
		return Sprite::createSprite(glm::ivec2(32.f, 32.f), glm::vec2(0.5f, 1.f), &spriteSheetItems, &texProgram);
		break;
	case WOODEN_SWORD:
	    arrayInventary[1].spriteItem = Sprite::createSprite(glm::ivec2(32.f, 32.f), glm::vec2(0.5f, 1.f), &spriteSheetItems, &texProgram);
	    arrayInventary[1].spriteItem->setTextCord(glm::vec2(0.5f, 0.f));
		return arrayInventary[1].spriteItem;
		break;
	default:
		break;
	}
}

int Inventary::getId(int pos){
	return arrayInventary[pos].id;
}

void Inventary::removeItem(int pos){
	arrayInventary[pos].id = 0;
	arrayInventary[pos].spriteItem = NULL;
}

void Inventary::swapItem(int pos1, int pos2){
	Item aux;
	aux.id = arrayInventary[pos1].id;
	aux.spriteItem = arrayInventary[pos1].spriteItem;
	arrayInventary[pos1].id = arrayInventary[pos2].id;
	arrayInventary[pos1].spriteItem = arrayInventary[pos2].spriteItem;
	arrayInventary[pos2].id = aux.id;
	arrayInventary[pos2].spriteItem = aux.spriteItem;
}