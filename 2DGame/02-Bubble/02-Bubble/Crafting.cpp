#include "Crafting.h"
#include "Game.h"

#include "Texture.h"
#include "Sprite.h"

Crafting::Crafting(ShaderProgram &texProgram)
{
	itemPointerCraft = -1;
	spriteSheetItems.loadFromFile("images/items.png", TEXTURE_PIXEL_FORMAT_RGBA);
	for (int i = 0; i < NUM_ITEMS; i++){
		arrayCraft[i].idItem = i;
		arrayCraft[i].spriteItem = setSprite(i, texProgram);
		arrayCraft[i].crafteable = isCrafeable(i);
		setItemsNeeded(i);
		if (arrayCraft[i].crafteable) itemPointerCraft = i;
	}
}

void Crafting::update(){
	itemPointerCraft = -1;
	for (int i = 0; i < NUM_ITEMS; i++){
		if (arrayCraft[i].isObjectCraft)
			arrayCraft[i].crafteable = isCrafeable(i);
		if (arrayCraft[i].crafteable) itemPointerCraft = i;
	}
}

Crafting::~Crafting()
{
}

void Crafting::incrementPointer(){
	itemPointerCraft = nextPointer(itemPointerCraft);
}

void Crafting::decrementPointer(){
	int res = itemPointerCraft;
	int count = 0;
	for (int i = (itemPointerCraft - 1); i != itemPointerCraft && count != 1; i--){
		if (i == -1) i = NUM_ITEMS;
		if (arrayCraft[i].crafteable){
			count++;
			res = i;
		}
	}
	itemPointerCraft = res;
}

void Crafting::render(){
	if (itemPointerCraft != -1){
		int pointer = setPointer(itemPointerCraft);
		for (int i = 0; i < 5; i++){
			if (itemPointerCraft != -1){
				arrayCraft[pointer].spriteItem->setPosition(glm::vec2(float(posXcraftBasic), float(posYcraftBasic - 35 * i)));
				arrayCraft[pointer].spriteItem->render();
			}
			// items necesaris per crafetejar
			if (i == 2){
				for (int i2 = 0; i2 < 3 && arrayCraft[pointer].itemsNeedId[i2] != -1; i2++){
					arrayCraft[(arrayCraft[pointer].itemsNeedId[i2])].spriteItem->setPosition(glm::vec2(float(posXobjectsNeed + i2 * 35), float(posYcraftBasic - 35 * i)));
					arrayCraft[(arrayCraft[pointer].itemsNeedId[i2])].spriteItem->render();
					}
			}
			pointer = nextPointer(pointer);
		}
	}
}

int Crafting::setPointer(int intemPointer){
	int res = intemPointer;
	if (intemPointer == -1) return intemPointer;
	else{
		int count = 0;
		for (int i = intemPointer - 1; i != res && count != 2; i--){
			if (i == -1) i = NUM_ITEMS - 1;
			if (arrayCraft[i].crafteable){
				count++;
				res = i;
			}
		}
		return res;
	}
}

int Crafting::nextPointer(int pointer){
	int res = pointer;
	int count = 0;
	for (int i = (pointer + 1); i != pointer && count != 1; i++){
		if (i == NUM_ITEMS) i = 0;
		if (arrayCraft[i].crafteable){
			count++;
			res = i;
		}
	}
	return res;
}

bool Crafting::isCrafeable(int idItem){
	switch (idItem){
	case PICK:
		return Game::instance().getScene()->getInventary()->hasItem(WOOD, 2);
		break;
	case WOODEN_SWORD:
		return Game::instance().getScene()->getInventary()->hasItem(WOOD, 3);
		break;
	default:
		return false;
		break;
	}
}

int Crafting::getIdObejctToCraft(){
	if (itemPointerCraft == -1) return itemPointerCraft;
	else return arrayCraft[itemPointerCraft].idItem;
}

void Crafting::craftItem(int idItem, ShaderProgram &texProgram){
	switch (idItem){
	case PICK:
		Game::instance().getScene()->getInventary()->removeItemNtimes(WOOD, 2);
		Game::instance().getScene()->getInventary()->putItem(PICK,
			Game::instance().getScene()->getInventary()->getFirstEmptySlot(), texProgram);
		break;
	case WOODEN_SWORD:
		Game::instance().getScene()->getInventary()->removeItemNtimes(WOOD, 3);
		Game::instance().getScene()->getInventary()->putItem(WOODEN_SWORD,
			Game::instance().getScene()->getInventary()->getFirstEmptySlot(), texProgram);
		break;
	default:
		break;
	}
}


Sprite *Crafting::setSprite(int idSprite, ShaderProgram &texProgram){
	Sprite *item;
	item = Sprite::createSprite(glm::ivec2(32.f, 32.f), glm::vec2((32.f / (float)SPRITESHEETWIDTH), 1.f), &spriteSheetItems, &texProgram);
	float movmentTextCord = (32.f / (float)SPRITESHEETWIDTH);
	switch (idSprite){
	case PICK:
		return item;
		break;
	case WOODEN_SWORD:
		item->setTextCord(glm::vec2((movmentTextCord * (WOODEN_SWORD - 1)), 0.f));
		return item;
		break;
	case WOOD:
		item->setTextCord(glm::vec2((movmentTextCord * (WOOD - 1)), 0.f));
		return item;
		break;
	default:
		break;
	}
}

void Crafting::setItemsNeeded(int i){
	arrayCraft[i].isObjectCraft = true;
	switch (i){
	case PICK:
		arrayCraft[i].itemsNeedId[0] = WOOD;
		arrayCraft[i].itemsNeedId[1] = WOOD;
		arrayCraft[i].itemsNeedId[2] = -1;
		break;
	case WOODEN_SWORD:
		arrayCraft[i].itemsNeedId[0] = WOOD;
		arrayCraft[i].itemsNeedId[1] = WOOD;
		arrayCraft[i].itemsNeedId[2] = WOOD;
		break;
	default:
		arrayCraft[i].itemsNeedId[0] = -1;
		arrayCraft[i].itemsNeedId[1] = -1;
		arrayCraft[i].itemsNeedId[2] = -1;
		arrayCraft[i].isObjectCraft = false;
		break;
	}
	

}