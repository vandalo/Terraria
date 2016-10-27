#include "Inventary.h"
#include "Game.h"

Inventary::Inventary(ShaderProgram &texProgram)
{
	for (int i = 0; i < SIZE_INVENTARY; i++){
		arrayInventary[i].id = 0;
		arrayInventary[i].spriteItem = NULL;
	}

	spriteSheetItems.loadFromFile("images/items.png", TEXTURE_PIXEL_FORMAT_RGBA);
	hasChange = false;
	//TESTING
	//putItem(PICK, 0, texProgram);
	//putItem(PICK, 1, texProgram);
	//putItem(WOODEN_SWORD, 1, texProgram);
	putItem(WOOD, 1, texProgram);
	putItem(WOOD, 0, texProgram);
	putItem(WOOD, 2, texProgram);
	putItem(WOOD, 4, texProgram);
	putItem(WOOD, 3, texProgram);

}

void Inventary::putItem(int idItem, int idCasella, ShaderProgram &texProgram){
	arrayInventary[idCasella] = setItem(idItem, texProgram);
}

void Inventary::setActiveItem(int position){
	Item item = arrayInventary[position];
	Game::instance().setPlayerItem(item.id, item.spriteItem);
}

void Inventary::render(bool showDinamicInterface){
	render10objects();
	if (showDinamicInterface) renderRestobjects();
}

void Inventary::update(){
	for (int i = 0; i < 10; i++){
		if (arrayInventary[i].id != 0){
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

void Inventary::renderRestobjects(){
	int idMovingItem = Game::instance().getMovingItem();

	int cont = 0;
	for (int i = 10; i < 50; i++){
		if (i % 10 == 0){
			cont++;
		}
		if (arrayInventary[i].id != 0 && idMovingItem != i){
			arrayInventary[i].spriteItem->setPosition(glm::vec2(float(posXobjectsInventary + ((i % 10) * 35)), float(posYobjectsInventary - (cont * 35))));
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

Item Inventary::setItem(int idItem, ShaderProgram &texProgram){
	Item item;
	item.spriteItem = Sprite::createSprite(glm::ivec2(32.f, 32.f), glm::vec2((32.f / (float)SPRITESHEETWIDTH), 1.f), &spriteSheetItems, &texProgram);
	float movmentTextCord = (32.f / (float)SPRITESHEETWIDTH);
	switch (idItem){
	case PICK:
		item.id = PICK;
		return item;
		break;
	case WOODEN_SWORD:
		item.spriteItem->setTextCord(glm::vec2((movmentTextCord * (WOODEN_SWORD - 1)), 0.f));
		item.id = WOODEN_SWORD;
		return item;
		break;
	case WOOD:
		item.spriteItem->setTextCord(glm::vec2((movmentTextCord * (WOOD - 1)), 0.f));
		item.id = WOOD;
		return item;
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

int Inventary::getFirstEmptySlot(){
	int i = 0;
	while (i < SIZE_INVENTARY && arrayInventary[i].id != 0)i++;
	return i;
}

bool Inventary::hasItem(int idItem, int times){
	int count = 0;
	for (int i = 0; i < SIZE_INVENTARY; i++){
		if (arrayInventary[i].id == idItem)count++;
	}
	return count >= times;
}

void Inventary::removeItemNtimes(int idItem, int times){
	int count = 0;
	for (int i = 0; i < SIZE_INVENTARY && count < times; i++){
		if (arrayInventary[i].id == idItem){
			count++;
			removeItem(i);
		}
	}
}