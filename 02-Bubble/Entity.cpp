#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Entity.h"
#include "Game.h"

enum EntityType
{
	SPIKE, STRAWBERRY, L_WING, R_WING, KEY, CHEST, BALLOON, STRING, MAX_ENTITIES
};


void Entity::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, int entity)
{
	spritesheet.loadFromFile("images/entities.png", TEXTURE_PIXEL_FORMAT_RGBA);
	float dimx = 8.f / 64.f;
	float dimy = 8.f / 16.f;
	size = glm::ivec2(32, 32);
	sprite = Sprite::createSprite(size, glm::vec2(dimx, dimy), &spritesheet, &shaderProgram);

	switch (entity) {
	case SPIKE:
		sprite->setNumberAnimations(1);
		sprite->setAnimationSpeed(0, 8);
		sprite->addKeyframe(0, glm::vec2(0.f, 0.f));
		break;
	case STRAWBERRY:
		sprite->setNumberAnimations(1);
		sprite->setAnimationSpeed(0, 8);
		sprite->addKeyframe(0, glm::vec2(dimx, 0.f));
		currentDisp = 24;
		up = false;
		break;
	case L_WING:
		sprite->setNumberAnimations(1);
		sprite->setAnimationSpeed(0, 4);
		sprite->addKeyframe(0, glm::vec2(2 * dimx, 0.f));
		sprite->addKeyframe(0, glm::vec2(3 * dimx, 0.f));
		//sprite->addKeyframe(3, glm::vec2(4 * dimx, 0.f));
		currentDisp = 24;
		up = false;
		break;
	case R_WING:
		sprite->setNumberAnimations(1);
		sprite->setAnimationSpeed(0, 4);
		sprite->addKeyframe(0, glm::vec2(5 * dimx, 0.f));
		sprite->addKeyframe(0, glm::vec2(6 * dimx, 0.f));
		//sprite->addKeyframe(2, glm::vec2(7 * dimx, 0.f));
		currentDisp = 24;
		up = false;
		break;
	case KEY:
		sprite->setNumberAnimations(1);
		sprite->setAnimationSpeed(0, 3);
		sprite->addKeyframe(0, glm::vec2(0.f, dimy));
		sprite->addKeyframe(0, glm::vec2(dimx, dimy));
		sprite->addKeyframe(0, glm::vec2(2 * dimx, dimy));
		sprite->addKeyframe(0, glm::vec2(dimx, dimy));
		break;
	case CHEST:
		sprite->setNumberAnimations(2);
		sprite->setAnimationSpeed(0, 8);
		sprite->addKeyframe(0, glm::vec2(3 * dimx, dimy));

		sprite->setAnimationSpeed(1, 8);
		sprite->addKeyframe(1, glm::vec2(dimx, 0.f));
		currentDisp = 24;
		up = false;
		//transformChest = false;
		break;
	case BALLOON:
		sprite->setNumberAnimations(1);
		sprite->setAnimationSpeed(0, 8);
		sprite->addKeyframe(0, glm::vec2(4 * dimx, dimy));
		currentDisp = 24;
		up = false;
		hideBalloon = 0;
		break;
	case STRING:
		sprite->setNumberAnimations(1);
		sprite->setAnimationSpeed(0, 6);
		sprite->addKeyframe(0, glm::vec2(5 * dimx, dimy));
		sprite->addKeyframe(0, glm::vec2(6 * dimx, dimy));
		sprite->addKeyframe(0, glm::vec2(7 * dimx, dimy));
		currentDisp = 24;
		up = false;
		break;
	default:
		break;
	}

	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEntity.x), float(tileMapDispl.y + posEntity.y)));
}

void Entity::update(int deltaTime, int entity)
{
	sprite->update(deltaTime);
	switch (entity) {
	case STRAWBERRY:
	case L_WING:
	case R_WING:
	case BALLOON:
	case STRING:
		if (currentDisp >= 0 && !up) {
			if (currentDisp % 6 == 0) posEntity.y -= 2;
			currentDisp--;
			if (currentDisp == 0) up = true;
		}
		else {
			if (currentDisp % 6 == 0) posEntity.y += 2;
			currentDisp++;
			if (currentDisp == 24) up = false;
		}
		break;
	default:
		break;
	}
	if ((entity == BALLOON || entity == STRING) && hideBalloon > 0) hideBalloon--;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEntity.x), float(tileMapDispl.y + posEntity.y)));
}

void Entity::render()
{
	if (hideBalloon == 0 && !elim) sprite->render();
}

void Entity::setTileMap(TileMap* tileMap)
{
	map = tileMap;
}

void Entity::setPosition(const glm::vec2& pos)
{
	posEntity = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEntity.x), float(tileMapDispl.y + posEntity.y)));
}

bool Entity::collisionEntity(const glm::ivec2& pos, const glm::ivec2& psize) const
{
	int pminx, pmaxx, pminy, pmaxy, eminx, emaxx, eminy, emaxy;

	pminx = pos.x;
	pmaxx = (pos.x + psize.x - 1);
	pminy = pos.y;
	pmaxy = (pos.y + psize.y - 1);

	eminx = posEntity.x;
	emaxx = (posEntity.x + size.x - 1);
	eminy = posEntity.y;
	emaxy = (posEntity.y + size.y - 1);

	//if (((pminx + 8) < emaxx) && (eminx < (pmaxx - 8)) && ((pminy + 8) < emaxy) && (eminy < (pmaxy - 8))) return true;//((pminy - 8) > emaxy)) return true;
	if (((pminx+8) < emaxx) && (eminx < (pmaxx-8)) && ((pminy+8) < emaxy) && (eminy < (pmaxy-8))) {
		return true;
	}
	return false;
}

void Entity::setHideBalloon() 
{
	hideBalloon = 100;
}

int Entity::getHideBalloon()
{
	return hideBalloon;
}

void Entity::transformChest()
{
	sprite->changeAnimation(1);
	
}

void Entity::eliminar()
{
	elim = true;
}

