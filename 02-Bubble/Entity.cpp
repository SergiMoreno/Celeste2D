#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Entity.h"
#include "Game.h"

enum EntityType
{
	SPIKE, STRAWBERRY, L_WING, R_WING, KEY, CHEST, BALLOON, STRING, L_CLOUD, R_CLOUD, BOX, TRAMPOLINE, FLAG, L_SPIKE, R_SPIKE, MAX_ENTITIES
};


void Entity::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, int entity)
{
	spritesheet.loadFromFile("images/entities.png", TEXTURE_PIXEL_FORMAT_RGBA);
	float dimx = 8.f / 64.f;
	float dimy = 8.f / 32.f;
	size = glm::ivec2(32, 32);
	sprite = Sprite::createSprite(size, glm::vec2(dimx, dimy), &spritesheet, &shaderProgram);
	type = entity;

	switch (type) {
	case L_SPIKE:
		sprite->setNumberAnimations(1);
		sprite->setAnimationSpeed(0, 8);
		sprite->addKeyframe(0, glm::vec2(3 * dimx, 3 * dimy));
		break;
	case R_SPIKE:
		sprite->setNumberAnimations(1);
		sprite->setAnimationSpeed(0, 8);
		sprite->addKeyframe(0, glm::vec2(4 * dimx, 3 * dimy));
		break;
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
	case L_CLOUD:
		sprite->setNumberAnimations(1);
		sprite->setAnimationSpeed(0, 6);
		sprite->addKeyframe(0, glm::vec2(0, 2 * dimy));
		currentDisp = 36;
		up = false;
		break;
	case R_CLOUD:
		sprite->setNumberAnimations(1);
		sprite->setAnimationSpeed(0, 6);
		sprite->addKeyframe(0, glm::vec2(dimx, 2 * dimy));
		currentDisp = 36;
		up = false;
		break;
	case BOX:
		sprite->setNumberAnimations(2);

		sprite->setAnimationSpeed(0, 6);
		sprite->addKeyframe(0, glm::vec2(2 * dimx, 2 * dimy));

		sprite->setAnimationSpeed(1, 6);
		sprite->addKeyframe(1, glm::vec2(2 * dimx, 2 * dimy));
		sprite->addKeyframe(1, glm::vec2(3 * dimx, 2 * dimy));
		sprite->addKeyframe(1, glm::vec2(4 * dimx, 2 * dimy));
		break;
	case TRAMPOLINE:
		sprite->setNumberAnimations(2);
		sprite->setAnimationSpeed(0, 6);
		sprite->addKeyframe(0, glm::vec2(5 * dimx, 2 * dimy));
		sprite->setAnimationSpeed(1, 6);
		sprite->addKeyframe(1, glm::vec2(6 * dimx, 2 * dimy));
		trampolineDown = 0;
		break;
	case FLAG:
		sprite->setNumberAnimations(1);
		sprite->setAnimationSpeed(0, 6);
		sprite->addKeyframe(0, glm::vec2(0, 3 * dimy));
		sprite->addKeyframe(0, glm::vec2(dimx, 3 * dimy));
		sprite->addKeyframe(0, glm::vec2(2 * dimx, 3 * dimy));
		break;
	default:
		break;
	}

	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEntity.x), float(tileMapDispl.y + posEntity.y)));
}

void Entity::update(int deltaTime)
{
	sprite->update(deltaTime);
	switch (type) {
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
	case CHEST:
		if (sprite->animation() == 1) {
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
		}
		break;
	case L_CLOUD:
	case R_CLOUD:
		if (currentDisp >= 0 && !up) {
			if (currentDisp % 6 == 0) posEntity.x -= 2;
			currentDisp--;
			if (currentDisp == 0) up = true;
		}
		else {
			if (currentDisp % 6 == 0) posEntity.x += 2;
			currentDisp++;
			if (currentDisp == 36) up = false;
		}
		break;
	default:
		break;
	}
	if ((type == BALLOON || type == STRING) && hideBalloon > 0) hideBalloon--;
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
	if (type == L_WING) posEntity.x += 4;
	else if (type == R_WING) posEntity.x -= 4;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEntity.x), float(tileMapDispl.y + posEntity.y)));
}

bool Entity::collisionEntity(const glm::ivec2& pos, const glm::ivec2& psize) const
{
	int pminx, pmaxx, pminy, pmaxy, eminx, emaxx, eminy, emaxy;
	if (type == CHEST && sprite->animation() != 1) return false;
	pminx = pos.x;
	pmaxx = (pos.x + psize.x - 1);
	pminy = pos.y;
	pmaxy = (pos.y + psize.y - 1);

	eminx = posEntity.x;
	emaxx = (posEntity.x + size.x - 1);
	eminy = posEntity.y;
	emaxy = (posEntity.y + size.y - 1);

	//if (((pminx + 8) < emaxx) && (eminx < (pmaxx - 8)) && ((pminy + 8) < emaxy) && (eminy < (pmaxy - 8))) return true;//((pminy - 8) > emaxy)) return true;
	if (((pminx + 8) < emaxx) && (eminx < (pmaxx - 8)) && ((pminy + 8) < emaxy) && (eminy < (pmaxy - 8))) {
		return true;
	}
	return false;
}

bool Entity::collisionCloud(const glm::ivec2& pos, const glm::ivec2& psize) const
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

	if (((pminx + 8) < emaxx) && (eminx < (pmaxx - 8)) && (eminy == (pmaxy - 8))) {
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

void Entity::onTrampoline()
{
	sprite->changeAnimation(1);
	trampolineDown = 25;
}

void Entity::notOnTrampoline()
{
	if (trampolineDown == 0) {
		sprite->changeAnimation(0);

	}
	else trampolineDown--;
}
