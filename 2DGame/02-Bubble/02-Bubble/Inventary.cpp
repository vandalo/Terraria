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
	for (int i = 1; i < NUM_ITEMS; i++){
		putItem(i, i, texProgram);
	}
	/*putItem(PICK, 0, texProgram);
	putItem(WOOD, 1, texProgram);
	putItem(WOOD, 2, texProgram);
	putItem(WOOD, 4, texProgram);
	putItem(WOOD, 3, texProgram);
	putItem(FLY_BOOTS, 8, texProgram);
	putItem(DIAMOND_SWORD, 11, texProgram);*/

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
			arrayInventary[i].spriteItem->render(-3.1416);
		}
		else if (idMovingItem == i) arrayInventary[i].spriteItem->render(-3.14162);
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
			arrayInventary[i].spriteItem->render(-3.1416);
		}
		else if (idMovingItem == i) arrayInventary[i].spriteItem->render(-3.1416);
	}
	for (int i = 50; i < 58; i++){
		if (arrayInventary[i].id != 0 && idMovingItem != i){
			arrayInventary[i].spriteItem->setPosition(glm::vec2(float(posXset), float(posYset - ((i-50) * 35))));
			arrayInventary[i].spriteItem->render(-3.1416);
		}
		else if (idMovingItem == i) arrayInventary[i].spriteItem->render(-3.1416);
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
	case IRON_SWORD:
		item.spriteItem->setTextCord(glm::vec2((movmentTextCord * (IRON_SWORD - 1)), 0.f));
		item.id = IRON_SWORD;
		return item;
		break;
	case DIAMOND_BAR:
		item.spriteItem->setTextCord(glm::vec2((movmentTextCord * (DIAMOND_BAR - 1)), 0.f));
		item.id = DIAMOND_BAR;
		return item;
		break;
	case GOLD_BAR:
		item.spriteItem->setTextCord(glm::vec2((movmentTextCord * (GOLD_BAR - 1)), 0.f));
		item.id = GOLD_BAR;
		return item;
		break;
	case BRONZE_BAR:
		item.spriteItem->setTextCord(glm::vec2((movmentTextCord * (BRONZE_BAR - 1)), 0.f));
		item.id = BRONZE_BAR;
		return item;
		break;
	case IRON_BAR:
		item.spriteItem->setTextCord(glm::vec2((movmentTextCord * (IRON_BAR - 1)), 0.f));
		item.id = IRON_BAR;
		return item;
		break;
	case FLY_BOOTS:
		item.spriteItem->setTextCord(glm::vec2((movmentTextCord * (FLY_BOOTS - 1)), 0.f));
		item.id = FLY_BOOTS;
		return item;
		break;
	case DIAMOND_SWORD:
		item.spriteItem->setTextCord(glm::vec2((movmentTextCord * (DIAMOND_SWORD - 1)), 0.f));
		item.id = DIAMOND_SWORD;
		return item;
		break;
	case BRONZE_BOOTS:
		item.spriteItem->setTextCord(glm::vec2((movmentTextCord * (BRONZE_BOOTS - 1)), 0.f));
		item.id = BRONZE_BOOTS;
		return item;
		break;
	case IRON_BOOTS:
		item.spriteItem->setTextCord(glm::vec2((movmentTextCord * (IRON_BOOTS - 1)), 0.f));
		item.id = IRON_BOOTS;
		return item;
		break;
	case DIAMOND_BOOTS:
		item.spriteItem->setTextCord(glm::vec2((movmentTextCord * (DIAMOND_BOOTS - 1)), 0.f));
		item.id = DIAMOND_BOOTS;
		return item;
		break;
	case GOLD_BOOTS:
		item.spriteItem->setTextCord(glm::vec2((movmentTextCord * (GOLD_BOOTS - 1)), 0.f));
		item.id = GOLD_BOOTS;
		return item;
		break;
	case BRONZE_ARMOR:
		item.spriteItem->setTextCord(glm::vec2((movmentTextCord * (BRONZE_ARMOR - 1)), 0.f));
		item.id = BRONZE_ARMOR;
		return item;
		break;
	case IRON_ARMOR:
		item.spriteItem->setTextCord(glm::vec2((movmentTextCord * (IRON_ARMOR - 1)), 0.f));
		item.id = IRON_ARMOR;
		return item;
		break;
	case DIAMOND_ARMOR:
		item.spriteItem->setTextCord(glm::vec2((movmentTextCord * (DIAMOND_ARMOR - 1)), 0.f));
		item.id = DIAMOND_ARMOR;
		return item;
		break;
	case GOLD_ARMOR:
		item.spriteItem->setTextCord(glm::vec2((movmentTextCord * (GOLD_ARMOR - 1)), 0.f));
		item.id = GOLD_ARMOR;
		return item;
		break;
	case BRONZE_HELMET:
		item.spriteItem->setTextCord(glm::vec2((movmentTextCord * (BRONZE_HELMET - 1)), 0.f));
		item.id = BRONZE_HELMET;
		return item;
		break;
	case IRON_HELMET:
		item.spriteItem->setTextCord(glm::vec2((movmentTextCord * (IRON_HELMET - 1)), 0.f));
		item.id = IRON_HELMET;
		return item;
		break;
	case DIAMOND_HELMET:
		item.spriteItem->setTextCord(glm::vec2((movmentTextCord * (DIAMOND_HELMET - 1)), 0.f));
		item.id = DIAMOND_HELMET;
		return item;
		break;
	case GOLD_HELMET:
		item.spriteItem->setTextCord(glm::vec2((movmentTextCord * (GOLD_HELMET - 1)), 0.f));
		item.id = GOLD_HELMET;
		return item;
		break;
	case HEARTH_RING:
		item.spriteItem->setTextCord(glm::vec2((movmentTextCord * (HEARTH_RING - 1)), 0.f));
		item.id = HEARTH_RING;
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