#include "Object.h"
#include "Game.h"


Object::Object()
{
	id = -1;
	atack = 0;
	defence = 0;
	velocity = 0;
}

void Object::setPropertis(int id){
	switch (id){
		case PICK:
			atack = 1;
			defence = 1;
	}
}

Object::~Object()
{
}
