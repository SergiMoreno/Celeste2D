#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Player.h"
#include "Game.h"


#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 96
#define FALL_STEP 4


enum PlayerAnims
{
	STAND_LEFT, STAND_RIGHT, MOVE_LEFT, MOVE_RIGHT, LOOK_RIGHT_UP, LOOK_RIGHT_DOWN, LOOK_LEFT_UP, LOOK_LEFT_DOWN,
	JUMPING_RIGHT, JUMPING_LEFT, DUSHING_RIGHT, DUSHING_LEFT
};


void Player::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	bJumping = false;
	spritesheet.loadFromFile("images/player.png", TEXTURE_PIXEL_FORMAT_RGBA);
	float dimx = 8.f / 24.f;
	float dimy = 8.f / 48.f;
	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(dimx, dimy), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(12);
	
		sprite->setAnimationSpeed(STAND_RIGHT, 8);
		sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.f, 0.f));
		
		sprite->setAnimationSpeed(STAND_LEFT, 8);
		sprite->addKeyframe(STAND_LEFT, glm::vec2(0.f, dimy));
		
		sprite->setAnimationSpeed(MOVE_RIGHT, 8);
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.f, 0.f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(dimx, 0.f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(2 * dimx, 0.f));
		
		sprite->setAnimationSpeed(MOVE_LEFT, 8);
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, dimy));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(dimx, dimy));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(2 * dimx, dimy));

		sprite->setAnimationSpeed(LOOK_RIGHT_UP, 8);
		sprite->addKeyframe(LOOK_RIGHT_UP, glm::vec2(dimx, 3 * dimy));

		sprite->setAnimationSpeed(LOOK_LEFT_UP, 8);
		sprite->addKeyframe(LOOK_LEFT_UP, glm::vec2(dimx, 4 * dimy));

		sprite->setAnimationSpeed(LOOK_RIGHT_DOWN, 8);
		sprite->addKeyframe(LOOK_RIGHT_DOWN, glm::vec2(0.f, 3 * dimy));

		sprite->setAnimationSpeed(LOOK_LEFT_DOWN, 8);
		sprite->addKeyframe(LOOK_LEFT_DOWN, glm::vec2(0.f, 4 * dimy));

		sprite->setAnimationSpeed(JUMPING_RIGHT, 8);
		sprite->addKeyframe(JUMPING_RIGHT, glm::vec2(0.f, 2 * dimy));

		sprite->setAnimationSpeed(JUMPING_LEFT, 8);
		sprite->addKeyframe(JUMPING_LEFT, glm::vec2(dimx, 2 * dimy));

		sprite->setAnimationSpeed(DUSHING_RIGHT, 8);
		sprite->addKeyframe(DUSHING_RIGHT, glm::vec2(0.f, 5 * dimy));

		sprite->setAnimationSpeed(DUSHING_LEFT, 8);
		sprite->addKeyframe(DUSHING_LEFT, glm::vec2(dimx, 5 * dimy));
		
	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
	
}

void Player::update(int deltaTime)
{
	bool hasCollision = false;
	if (sprite->animation() == LOOK_LEFT_DOWN || sprite->animation() == LOOK_LEFT_UP)
		sprite->changeAnimation(STAND_LEFT);
	else if (sprite->animation() == LOOK_RIGHT_DOWN || sprite->animation() == LOOK_RIGHT_UP)
		sprite->changeAnimation(STAND_RIGHT);
	sprite->update(deltaTime);
	if(Game::instance().getSpecialKey(GLUT_KEY_LEFT))
	{
		if(sprite->animation() != MOVE_LEFT)
			sprite->changeAnimation(MOVE_LEFT);
		posPlayer.x -= 2;
		if(map->collisionMoveLeft(posPlayer, glm::ivec2(32, 32)))
		{
			posPlayer.x += 2;
			sprite->changeAnimation(STAND_LEFT);
			hasCollision = true;
		}
	}
	else if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT))
	{
		if (sprite->animation() != MOVE_RIGHT)
			sprite->changeAnimation(MOVE_RIGHT);
		posPlayer.x += 2;
		if (map->collisionMoveRight(posPlayer, glm::ivec2(32, 32)))
		{
			posPlayer.x -= 2;
			sprite->changeAnimation(STAND_RIGHT);
			hasCollision = true;
		}
	}
	/*else if (Game::instance().getSpecialKey(GLUT_KEY_UP) && !bJumping) {
		if (sprite->animation() == STAND_LEFT || sprite->animation() == MOVE_LEFT)
			sprite->changeAnimation(LOOK_LEFT_UP);
		else if (sprite->animation() == STAND_RIGHT || sprite->animation() == MOVE_RIGHT)
			sprite->changeAnimation(LOOK_RIGHT_UP);
	}*/
	else if (!bJumping)
	{
		if(sprite->animation() == MOVE_LEFT)
			sprite->changeAnimation(STAND_LEFT);
		else if(sprite->animation() == MOVE_RIGHT)
			sprite->changeAnimation(STAND_RIGHT);
	}
	
	if(bJumping)
	{
		jumpAngle += JUMP_ANGLE_STEP;
		if(jumpAngle == 180)
		{
			bJumping = false;
			posPlayer.y = startY;
		}
		else
		{
			posPlayer.y = int(startY - 96 * sin(3.14159f * jumpAngle / 180.f));
			if(jumpAngle > 90)
				bJumping = !map->collisionMoveDown(posPlayer, glm::ivec2(32, 32), &posPlayer.y);
		}
	}
	else
	{
		if (hasCollision) posPlayer.y = posPlayer.y + FALL_STEP - 3;
		else posPlayer.y += FALL_STEP;
		if(map->collisionMoveDown(posPlayer, glm::ivec2(32, 32), &posPlayer.y))
		{
			if (Game::instance().getKey('c') || Game::instance().getKey('C'))
			{
				bJumping = true;
				if (sprite->animation() == STAND_LEFT || sprite->animation() == MOVE_LEFT || sprite->animation() == LOOK_LEFT_DOWN || sprite->animation() == LOOK_LEFT_UP)
					sprite->changeAnimation(JUMPING_LEFT);
				else if (sprite->animation() == STAND_RIGHT || sprite->animation() == MOVE_RIGHT || sprite->animation() == LOOK_RIGHT_DOWN || sprite->animation() == LOOK_RIGHT_UP)
					sprite->changeAnimation(JUMPING_RIGHT);
				jumpAngle = 0;
				startY = posPlayer.y;
			}
			else if (Game::instance().getSpecialKey(GLUT_KEY_UP)) {
				if (sprite->animation() == STAND_LEFT || sprite->animation() == MOVE_LEFT || sprite->animation() == LOOK_LEFT_DOWN)
					sprite->changeAnimation(LOOK_LEFT_UP);
				else if (sprite->animation() == STAND_RIGHT || sprite->animation() == MOVE_RIGHT || sprite->animation() == LOOK_RIGHT_DOWN)
					sprite->changeAnimation(LOOK_RIGHT_UP);
			}
			else if (Game::instance().getSpecialKey(GLUT_KEY_DOWN)) {
				if (sprite->animation() == STAND_LEFT || sprite->animation() == MOVE_LEFT || sprite->animation() == LOOK_LEFT_UP)
					sprite->changeAnimation(LOOK_LEFT_DOWN);
				else if (sprite->animation() == STAND_RIGHT || sprite->animation() == MOVE_RIGHT || sprite->animation() == LOOK_RIGHT_UP)
					sprite->changeAnimation(LOOK_RIGHT_DOWN);
			}
			else {
				if (sprite->animation() == JUMPING_LEFT)
					sprite->changeAnimation(STAND_LEFT);
				else if (sprite->animation() == JUMPING_RIGHT)
					sprite->changeAnimation(STAND_RIGHT);
			}
		}
		else {
			if (sprite->animation() == STAND_LEFT || sprite->animation() == MOVE_LEFT || sprite->animation() == LOOK_LEFT_DOWN || sprite->animation() == LOOK_LEFT_UP)
				sprite->changeAnimation(JUMPING_LEFT);
			else if (sprite->animation() == STAND_RIGHT || sprite->animation() == MOVE_RIGHT || sprite->animation() == LOOK_RIGHT_DOWN || sprite->animation() == LOOK_RIGHT_UP)
				sprite->changeAnimation(JUMPING_RIGHT);
		}
	}
	
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

void Player::render()
{
	sprite->render();
}

void Player::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

void Player::setPosition(const glm::vec2 &pos)
{
	posPlayer = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}




