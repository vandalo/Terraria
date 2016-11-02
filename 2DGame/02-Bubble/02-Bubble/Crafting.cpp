#include "Crafting.h"
#include "Game.h"

#include "Texture.h"
#include "Sprite.h"

Crafting::Crafting(ShaderProgram &texProgram)
{
	itemPointerCraft = -1;
	spriteItem = Sprite::createSprite(glm::ivec2(32.f, 32.f), glm::vec2((32.f / (float)SPRITESHEETWIDTH), 1.f), &spriteSheetItems, &texProgram);
	spriteSheetItems.loadFromFile("images/items.png", TEXTURE_PIXEL_FORMAT_RGBA);
	for (int i = 0; i < NUM_ITEMS; i++){
		arrayCraft[i].idItem = i;
		arrayCraft[i].textCord = setSprite(i);
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
		if (i == -1) i = NUM_ITEMS - 1;
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
				//arrayCraft[pointer].spriteItem->setPosition(glm::vec2(float(posXcraftBasic), float(posYcraftBasic - 35 * i)));
				//arrayCraft[pointer].spriteItem->render(3.1415);
				spriteItem->setPosition(glm::vec2(float(posXcraftBasic), float(posYcraftBasic - 35 * i)));
				spriteItem->setTextCord(arrayCraft[pointer].textCord);
				spriteItem->render(3.1415);
			}
			// items necesaris per crafetejar
			if (i == 2){
				for (int i2 = 0; i2 < 3 && arrayCraft[pointer].itemsNeedId[i2] != -1; i2++){
					//arrayCraft[(arrayCraft[pointer].itemsNeedId[i2])].spriteItem->setPosition(glm::vec2(float(posXobjectsNeed + i2 * 35), float(posYcraftBasic - 35 * i)));
					//arrayCraft[(arrayCraft[pointer].itemsNeedId[i2])].spriteItem->render(3.1415);
					spriteItem->setPosition(glm::vec2(float(posXobjectsNeed + i2 * 35), float(posYcraftBasic - 35 * i)));
					spriteItem->setTextCord(arrayCraft[(arrayCraft[pointer].itemsNeedId[i2])].textCord);
					spriteItem->render(3.1415);

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
	case IRON_SWORD:
		return Game::instance().getScene()->getInventary()->hasItem(IRON_BAR, 3);
		break;
	case DIAMOND_SWORD:
		return Game::instance().getScene()->getInventary()->hasItem(DIAMOND_BAR, 3);
		break;
	case BRONZE_BOOTS:
		return Game::instance().getScene()->getInventary()->hasItem(BRONZE_BAR, 1);
		break;
	case IRON_BOOTS:
		return Game::instance().getScene()->getInventary()->hasItem(IRON_BAR, 1);
		break;
	case DIAMOND_BOOTS:
		return Game::instance().getScene()->getInventary()->hasItem(DIAMOND_BAR, 1);
		break;
	case GOLD_BOOTS:
		return Game::instance().getScene()->getInventary()->hasItem(GOLD_BAR, 1);
		break;
	case BRONZE_ARMOR:
		return Game::instance().getScene()->getInventary()->hasItem(BRONZE_BAR, 3);
		break;
	case IRON_ARMOR:
		return Game::instance().getScene()->getInventary()->hasItem(IRON_BAR, 3);
		break;
	case DIAMOND_ARMOR:
		return Game::instance().getScene()->getInventary()->hasItem(DIAMOND_BAR, 3);
		break;
	case GOLD_ARMOR:
		return Game::instance().getScene()->getInventary()->hasItem(GOLD_BAR, 3);
		break;
	case BRONZE_HELMET:
		return Game::instance().getScene()->getInventary()->hasItem(BRONZE_BAR, 2);
		break;
	case IRON_HELMET:
		return Game::instance().getScene()->getInventary()->hasItem(IRON_BAR, 2);
		break;
	case DIAMOND_HELMET:
		return Game::instance().getScene()->getInventary()->hasItem(DIAMOND_BAR, 2);
		break;
	case GOLD_HELMET:
		return Game::instance().getScene()->getInventary()->hasItem(GOLD_BAR, 2);
		break;
	case HEARTH_RING:
		return Game::instance().getScene()->getInventary()->hasItem(IRON_BAR, 1) &&
			Game::instance().getScene()->getInventary()->hasItem(GOLD_BAR, 1) &&
			Game::instance().getScene()->getInventary()->hasItem(DIAMOND_BAR, 1);
		break;
	case IRON_BAR:
		return Game::instance().getScene()->getInventary()->hasItem(BRONZE_BAR, 2);
		break;
	default:
		return false;
		break;
	}
}

int Crafting::getIdObejctToCraft(){
	if (itemPointerCraft == -1) return -1;
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
	case IRON_SWORD:
		for (int i = 0; i < 3; i++){
			Game::instance().getScene()->getInventary()->removeItemNtimes(arrayCraft[IRON_SWORD].itemsNeedId[i], 1);
		}
		Game::instance().getScene()->getInventary()->putItem(IRON_SWORD,
			Game::instance().getScene()->getInventary()->getFirstEmptySlot(), texProgram);
		break;
	case DIAMOND_BAR:
		for (int i = 0; i < 3; i++){
			Game::instance().getScene()->getInventary()->removeItemNtimes(arrayCraft[DIAMOND_BAR].itemsNeedId[i], 1);
		}
		Game::instance().getScene()->getInventary()->putItem(DIAMOND_BAR,
			Game::instance().getScene()->getInventary()->getFirstEmptySlot(), texProgram);
		break;
	case GOLD_BAR:
		for (int i = 0; i < 3; i++){
			Game::instance().getScene()->getInventary()->removeItemNtimes(arrayCraft[GOLD_BAR].itemsNeedId[i], 1);
		}
		Game::instance().getScene()->getInventary()->putItem(GOLD_BAR,
			Game::instance().getScene()->getInventary()->getFirstEmptySlot(), texProgram);
		break;
	case BRONZE_BAR:
		for (int i = 0; i < 3; i++){
			Game::instance().getScene()->getInventary()->removeItemNtimes(arrayCraft[BRONZE_BAR].itemsNeedId[i], 1);
		}
		Game::instance().getScene()->getInventary()->putItem(BRONZE_BAR,
			Game::instance().getScene()->getInventary()->getFirstEmptySlot(), texProgram);
		break;
	case IRON_BAR:
		for (int i = 0; i < 3; i++){
			Game::instance().getScene()->getInventary()->removeItemNtimes(arrayCraft[IRON_BAR].itemsNeedId[i], 1);
		}
		Game::instance().getScene()->getInventary()->putItem(IRON_BAR,
			Game::instance().getScene()->getInventary()->getFirstEmptySlot(), texProgram);
		break;
	case FLY_BOOTS:
		for (int i = 0; i < 3; i++){
			Game::instance().getScene()->getInventary()->removeItemNtimes(arrayCraft[FLY_BOOTS].itemsNeedId[i], 1);
		}
		Game::instance().getScene()->getInventary()->putItem(FLY_BOOTS,
			Game::instance().getScene()->getInventary()->getFirstEmptySlot(), texProgram);
		break;
	case DIAMOND_SWORD:
		for (int i = 0; i < 3; i++){
			Game::instance().getScene()->getInventary()->removeItemNtimes(arrayCraft[DIAMOND_SWORD].itemsNeedId[i], 1);
		}
		Game::instance().getScene()->getInventary()->putItem(DIAMOND_SWORD,
			Game::instance().getScene()->getInventary()->getFirstEmptySlot(), texProgram);
		break;
	case BRONZE_BOOTS:
		for (int i = 0; i < 3; i++){
			Game::instance().getScene()->getInventary()->removeItemNtimes(arrayCraft[BRONZE_BOOTS].itemsNeedId[i], 1);
		}
		Game::instance().getScene()->getInventary()->putItem(BRONZE_BOOTS,
			Game::instance().getScene()->getInventary()->getFirstEmptySlot(), texProgram);
		break;
	case IRON_BOOTS:
		for (int i = 0; i < 3; i++){
			Game::instance().getScene()->getInventary()->removeItemNtimes(arrayCraft[IRON_BOOTS].itemsNeedId[i], 1);
		}
		Game::instance().getScene()->getInventary()->putItem(IRON_BOOTS,
			Game::instance().getScene()->getInventary()->getFirstEmptySlot(), texProgram);
		break;
	case DIAMOND_BOOTS:
		for (int i = 0; i < 3; i++){
			Game::instance().getScene()->getInventary()->removeItemNtimes(arrayCraft[DIAMOND_BOOTS].itemsNeedId[i], 1);
		}
		Game::instance().getScene()->getInventary()->putItem(DIAMOND_BOOTS,
			Game::instance().getScene()->getInventary()->getFirstEmptySlot(), texProgram);
		break;
	case GOLD_BOOTS:
		for (int i = 0; i < 3; i++){
			Game::instance().getScene()->getInventary()->removeItemNtimes(arrayCraft[GOLD_BOOTS].itemsNeedId[i], 1);
		}
		Game::instance().getScene()->getInventary()->putItem(GOLD_BOOTS,
			Game::instance().getScene()->getInventary()->getFirstEmptySlot(), texProgram);
		break;
	case BRONZE_ARMOR:
		for (int i = 0; i < 3; i++){
			Game::instance().getScene()->getInventary()->removeItemNtimes(arrayCraft[BRONZE_ARMOR].itemsNeedId[i], 1);
		}
		Game::instance().getScene()->getInventary()->putItem(BRONZE_ARMOR,
			Game::instance().getScene()->getInventary()->getFirstEmptySlot(), texProgram);
		break;
	case IRON_ARMOR:
		for (int i = 0; i < 3; i++){
			Game::instance().getScene()->getInventary()->removeItemNtimes(arrayCraft[IRON_ARMOR].itemsNeedId[i], 1);
		}
		Game::instance().getScene()->getInventary()->putItem(IRON_ARMOR,
			Game::instance().getScene()->getInventary()->getFirstEmptySlot(), texProgram);
		break;
	case DIAMOND_ARMOR:
		for (int i = 0; i < 3; i++){
			Game::instance().getScene()->getInventary()->removeItemNtimes(arrayCraft[DIAMOND_ARMOR].itemsNeedId[i], 1);
		}
		Game::instance().getScene()->getInventary()->putItem(DIAMOND_ARMOR,
			Game::instance().getScene()->getInventary()->getFirstEmptySlot(), texProgram);
		break;
	case GOLD_ARMOR:
		for (int i = 0; i < 3; i++){
			Game::instance().getScene()->getInventary()->removeItemNtimes(arrayCraft[GOLD_ARMOR].itemsNeedId[i], 1);
		}
		Game::instance().getScene()->getInventary()->putItem(GOLD_ARMOR,
			Game::instance().getScene()->getInventary()->getFirstEmptySlot(), texProgram);
		break;
	case BRONZE_HELMET:
		for (int i = 0; i < 3; i++){
			Game::instance().getScene()->getInventary()->removeItemNtimes(arrayCraft[BRONZE_HELMET].itemsNeedId[i], 1);
		}
		Game::instance().getScene()->getInventary()->putItem(BRONZE_HELMET,
			Game::instance().getScene()->getInventary()->getFirstEmptySlot(), texProgram);
		break;
	case IRON_HELMET:
		for (int i = 0; i < 3; i++){
			Game::instance().getScene()->getInventary()->removeItemNtimes(arrayCraft[IRON_HELMET].itemsNeedId[i], 1);
		}
		Game::instance().getScene()->getInventary()->putItem(IRON_HELMET,
			Game::instance().getScene()->getInventary()->getFirstEmptySlot(), texProgram);
		break;
	case DIAMOND_HELMET:
		for (int i = 0; i < 3; i++){
			Game::instance().getScene()->getInventary()->removeItemNtimes(arrayCraft[DIAMOND_HELMET].itemsNeedId[i], 1);
		}
		Game::instance().getScene()->getInventary()->putItem(DIAMOND_HELMET,
			Game::instance().getScene()->getInventary()->getFirstEmptySlot(), texProgram);
		break;
	case GOLD_HELMET:
		for (int i = 0; i < 3; i++){
			Game::instance().getScene()->getInventary()->removeItemNtimes(arrayCraft[GOLD_HELMET].itemsNeedId[i], 1);
		}
		Game::instance().getScene()->getInventary()->putItem(GOLD_HELMET,
			Game::instance().getScene()->getInventary()->getFirstEmptySlot(), texProgram);
		break;
	case HEARTH_RING:
		for (int i = 0; i < 3; i++){
			Game::instance().getScene()->getInventary()->removeItemNtimes(arrayCraft[HEARTH_RING].itemsNeedId[i], 1);
		}
		Game::instance().getScene()->getInventary()->putItem(HEARTH_RING,
			Game::instance().getScene()->getInventary()->getFirstEmptySlot(), texProgram);
		break;
	default:
		break;
	}
}
/*
#define PICK 1
#define WOODEN_SWORD 2
#define WOOD 3
#define IRON_SWORD 4

#define DIAMOND_BAR 5
#define GOLD_BAR 6
#define BRONZE_BAR 7
#define IRON_BAR 8
#define FLY_BOOTS 9

#define DIAMOND_SWORD 10
#define BRONZE_BOOTS 11
#define IRON_BOOTS 12
#define DIAMOND_BOOTS 13
#define GOLD_BOOTS 14
#define BRONZE_ARMOR 15
#define IRON_ARMOR 16
#define DIAMOND_ARMOR 17
#define GOLD_ARMOR 18
#define BRONZE_HELMET 19
#define IRON_HELMET 20
#define DIAMOND_HELMET 21
#define GOLD_HELMET 22
#define HEARTH_RING 23
*/

glm::vec2 Crafting::setSprite(int idSprite){
	glm::vec2 textCord(0.f,0.f);
	float movmentTextCord = (32.f / (float)SPRITESHEETWIDTH);
	switch (idSprite){
	case PICK:
		return textCord;
		break;
	case WOODEN_SWORD:
		textCord = (glm::vec2((movmentTextCord * (WOODEN_SWORD - 1)), 0.f));
		return textCord;
		break;
	case WOOD:
		textCord = (glm::vec2((movmentTextCord * (WOOD - 1)), 0.f));
		return textCord;
		break;
	case IRON_SWORD:
		textCord = (glm::vec2((movmentTextCord * (IRON_SWORD - 1)), 0.f));
		return textCord;
		break;
	case DIAMOND_BAR:
		textCord = (glm::vec2((movmentTextCord * (DIAMOND_BAR - 1)), 0.f));
		return textCord;
		break;
	case GOLD_BAR:
		textCord = (glm::vec2((movmentTextCord * (GOLD_BAR - 1)), 0.f));
		return textCord;
		break;
	case BRONZE_BAR:
		textCord = (glm::vec2((movmentTextCord * (BRONZE_BAR - 1)), 0.f));
		return textCord;
		break;
	case IRON_BAR:
		textCord = (glm::vec2((movmentTextCord * (IRON_BAR - 1)), 0.f));
		return textCord;
		break;
	case FLY_BOOTS:
		textCord = (glm::vec2((movmentTextCord * (FLY_BOOTS - 1)), 0.f));
		return textCord;
		break;
	case DIAMOND_SWORD:
		textCord = (glm::vec2((movmentTextCord * (DIAMOND_SWORD - 1)), 0.f));
		return textCord;
		break;
	case BRONZE_BOOTS:
		textCord = (glm::vec2((movmentTextCord * (BRONZE_BOOTS - 1)), 0.f));
		return textCord;
		break;
	case IRON_BOOTS:
		textCord = (glm::vec2((movmentTextCord * (IRON_BOOTS - 1)), 0.f));
		return textCord;
		break;
	case DIAMOND_BOOTS:
		textCord = (glm::vec2((movmentTextCord * (DIAMOND_BOOTS - 1)), 0.f));
		return textCord;
		break;
	case GOLD_BOOTS:
		textCord = (glm::vec2((movmentTextCord * (GOLD_BOOTS - 1)), 0.f));
		return textCord;
		break;
	case BRONZE_ARMOR:
		textCord = (glm::vec2((movmentTextCord * (BRONZE_ARMOR - 1)), 0.f));
		return textCord;
		break;
	case IRON_ARMOR:
		textCord = (glm::vec2((movmentTextCord * (IRON_ARMOR - 1)), 0.f));
		return textCord;
		break;
	case DIAMOND_ARMOR:
		textCord = (glm::vec2((movmentTextCord * (DIAMOND_ARMOR - 1)), 0.f));
		return textCord;
		break;
	case GOLD_ARMOR:
		textCord = (glm::vec2((movmentTextCord * (GOLD_ARMOR - 1)), 0.f));
		return textCord;
		break;
	case BRONZE_HELMET:
		textCord = (glm::vec2((movmentTextCord * (BRONZE_HELMET - 1)), 0.f));
		return textCord;
		break;
	case IRON_HELMET:
		textCord = (glm::vec2((movmentTextCord * (IRON_HELMET - 1)), 0.f));
		return textCord;
		break;
	case DIAMOND_HELMET:
		textCord = (glm::vec2((movmentTextCord * (DIAMOND_HELMET - 1)), 0.f));
		return textCord;
		break;
	case GOLD_HELMET:
		textCord = (glm::vec2((movmentTextCord * (GOLD_HELMET - 1)), 0.f));
		return textCord;
		break;
	case HEARTH_RING:
		textCord = (glm::vec2((movmentTextCord * (HEARTH_RING - 1)), 0.f));
		return textCord;
		break;
	default:
		return textCord;
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
	case IRON_SWORD:
		arrayCraft[i].itemsNeedId[0] = IRON_BAR;
		arrayCraft[i].itemsNeedId[1] = IRON_BAR;
		arrayCraft[i].itemsNeedId[2] = IRON_BAR;
		break;
	case DIAMOND_SWORD:
		arrayCraft[i].itemsNeedId[0] = DIAMOND_BAR;
		arrayCraft[i].itemsNeedId[1] = DIAMOND_BAR;
		arrayCraft[i].itemsNeedId[2] = DIAMOND_BAR;
		break;
	case BRONZE_BOOTS:
		arrayCraft[i].itemsNeedId[0] = BRONZE_BAR;
		arrayCraft[i].itemsNeedId[1] = -1;
		arrayCraft[i].itemsNeedId[2] = -1;
		break;
	case BRONZE_HELMET:
		arrayCraft[i].itemsNeedId[0] = BRONZE_BAR;
		arrayCraft[i].itemsNeedId[1] = BRONZE_BAR;
		arrayCraft[i].itemsNeedId[2] = -1;
		break;
	case BRONZE_ARMOR:
		arrayCraft[i].itemsNeedId[0] = BRONZE_BAR;
		arrayCraft[i].itemsNeedId[1] = BRONZE_BAR;
		arrayCraft[i].itemsNeedId[2] = BRONZE_BAR;
		break;
	case IRON_BOOTS:
		arrayCraft[i].itemsNeedId[0] = IRON_BAR;
		arrayCraft[i].itemsNeedId[1] = -1;
		arrayCraft[i].itemsNeedId[2] = -1;
		break;
	case IRON_HELMET:
		arrayCraft[i].itemsNeedId[0] = IRON_BAR;
		arrayCraft[i].itemsNeedId[1] = IRON_BAR;
		arrayCraft[i].itemsNeedId[2] = -1;
		break;
	case IRON_ARMOR:
		arrayCraft[i].itemsNeedId[0] = IRON_BAR;
		arrayCraft[i].itemsNeedId[1] = IRON_BAR;
		arrayCraft[i].itemsNeedId[2] = IRON_BAR;
		break;
	case GOLD_BOOTS:
		arrayCraft[i].itemsNeedId[0] = GOLD_BAR;
		arrayCraft[i].itemsNeedId[1] = -1;
		arrayCraft[i].itemsNeedId[2] = -1;
		break;
	case GOLD_HELMET:
		arrayCraft[i].itemsNeedId[0] = GOLD_BAR;
		arrayCraft[i].itemsNeedId[1] = GOLD_BAR;
		arrayCraft[i].itemsNeedId[2] = -1;
		break;
	case GOLD_ARMOR:
		arrayCraft[i].itemsNeedId[0] = GOLD_BAR;
		arrayCraft[i].itemsNeedId[1] = GOLD_BAR;
		arrayCraft[i].itemsNeedId[2] = GOLD_BAR;
		break;
	case DIAMOND_BOOTS:
		arrayCraft[i].itemsNeedId[0] = DIAMOND_BAR;
		arrayCraft[i].itemsNeedId[1] = -1;
		arrayCraft[i].itemsNeedId[2] = -1;
		break;
	case DIAMOND_HELMET:
		arrayCraft[i].itemsNeedId[0] = DIAMOND_BAR;
		arrayCraft[i].itemsNeedId[1] = DIAMOND_BAR;
		arrayCraft[i].itemsNeedId[2] = -1;
		break;
	case DIAMOND_ARMOR:
		arrayCraft[i].itemsNeedId[0] = DIAMOND_BAR;
		arrayCraft[i].itemsNeedId[1] = DIAMOND_BAR;
		arrayCraft[i].itemsNeedId[2] = DIAMOND_BAR;
		break;
	case HEARTH_RING:
		arrayCraft[i].itemsNeedId[0] = DIAMOND_BAR;
		arrayCraft[i].itemsNeedId[1] = GOLD_BAR;
		arrayCraft[i].itemsNeedId[2] = IRON_BAR;
		break;
	case IRON_BAR:
		arrayCraft[i].itemsNeedId[0] = BRONZE_BAR;
		arrayCraft[i].itemsNeedId[1] = BRONZE_BAR;
		arrayCraft[i].itemsNeedId[2] = -1;
		break;
	default:
		arrayCraft[i].itemsNeedId[0] = -1;
		arrayCraft[i].itemsNeedId[1] = -1;
		arrayCraft[i].itemsNeedId[2] = -1;
		arrayCraft[i].isObjectCraft = false;
		break;
	}
	

}