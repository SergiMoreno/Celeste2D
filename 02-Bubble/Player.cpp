#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Player.h"
#include "Game.h"


#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 96
#define FALL_STEP 4
#define DASH_MOVEMENT 0
#define DIV 2


enum PlayerAnims
{
	STAND_LEFT, STAND_RIGHT, MOVE_LEFT, MOVE_RIGHT, LOOK_RIGHT_UP, LOOK_RIGHT_DOWN, LOOK_LEFT_UP, LOOK_LEFT_DOWN,
	JUMPING_RIGHT, JUMPING_LEFT, DUSHING_RIGHT, DUSHING_LEFT
};

enum PlayerDirection
{
	UP, RIGHT_UP, RIGHT, RIGHT_DOWN, DOWN, LEFT_DOWN, LEFT, LEFT_UP
};


void Player::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	bJumping = false;
	spritesheet.loadFromFile("images/player.png", TEXTURE_PIXEL_FORMAT_RGBA);
	float dimx = 8.f / 24.f;
	float dimy = 8.f / 48.f;
	dimyPlayer = 28;
	dimxPlayer = 28;
	direction = RIGHT;
	dashing_count = 0;
	sprite = Sprite::createSprite(glm::ivec2(dimxPlayer, dimyPlayer), glm::vec2(dimx, dimy), &spritesheet, &shaderProgram);
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
	sprite->update(deltaTime);
	if (bDashing) {
		if (dashing_count == 0) {
			dash_direction = direction;
			dashing_count = 24;
		}
		else if (dashing_count != 1) {
			--dashing_count;
			switch (direction) {
			case 0:		//UP
				posPlayer.y -= DASH_MOVEMENT + dashing_count / DIV;
				if (map->collisionMoveUp(posPlayer, glm::ivec2(32, 32))) {
					posPlayer.y += DASH_MOVEMENT + dashing_count / DIV;
					dashing_count = 1;
				}
				break;
			case 1:		//RIGHT_UP
				posPlayer.y -= DASH_MOVEMENT + dashing_count / DIV;
				if (map->collisionMoveUp(posPlayer, glm::ivec2(dimxPlayer, dimyPlayer))) {
					posPlayer.y += DASH_MOVEMENT + dashing_count / DIV;
				}
				posPlayer.x += DASH_MOVEMENT + dashing_count / DIV;
				if (map->collisionMoveRight(posPlayer, glm::ivec2(dimxPlayer, dimyPlayer))) {
					posPlayer.x -= DASH_MOVEMENT + dashing_count / DIV;
				}
				break;
			case 2:		//RIGHT
				posPlayer.x += DASH_MOVEMENT + dashing_count / DIV;
				if (map->collisionMoveRight(posPlayer, glm::ivec2(dimxPlayer, dimyPlayer))) {
					posPlayer.x -= DASH_MOVEMENT + dashing_count / DIV;
					dashing_count = 1;
				}
				break;
			case 3:		//RIGHT_DOWN
				posPlayer.x += DASH_MOVEMENT + dashing_count / DIV;
				if (map->collisionMoveRight(posPlayer, glm::ivec2(dimxPlayer, dimyPlayer))) {
					posPlayer.x -= DASH_MOVEMENT + dashing_count / DIV;
				}
				if (DASH_MOVEMENT + dashing_count / DIV >= FALL_STEP) posPlayer.y += DASH_MOVEMENT + dashing_count / DIV;
				else  posPlayer.y += FALL_STEP;
				if (map->collisionMoveDown(posPlayer, glm::ivec2(dimxPlayer, dimyPlayer), &posPlayer.y)) {
					//posPlayer.y -= DASH_MOVEMENT + dashing_count / DIV;
				}
				break;
			case 4:		//DOWN
				if (DASH_MOVEMENT + dashing_count / DIV >= FALL_STEP) posPlayer.y += DASH_MOVEMENT + dashing_count / DIV;
				else  posPlayer.y += FALL_STEP;
				if (map->collisionMoveDown(posPlayer, glm::ivec2(dimxPlayer, dimyPlayer), &posPlayer.y)) {
					//posPlayer.y -= DASH_MOVEMENT + dashing_count / DIV;
					dashing_count = 1;
				}
				break;
			case 5:		//LEFT_DOWN
				if (DASH_MOVEMENT + dashing_count / DIV >= FALL_STEP) posPlayer.y += DASH_MOVEMENT + dashing_count / DIV;
				else  posPlayer.y += FALL_STEP;
				if (map->collisionMoveDown(posPlayer, glm::ivec2(dimxPlayer, dimyPlayer), &posPlayer.y)) {
					//posPlayer.y -= DASH_MOVEMENT + dashing_count / DIV;
				}

				posPlayer.x -= DASH_MOVEMENT + dashing_count / DIV;
				if (map->collisionMoveLeft(posPlayer, glm::ivec2(dimxPlayer, dimyPlayer))) {
					posPlayer.x += DASH_MOVEMENT + dashing_count / DIV;
				}
				break;
			case 6:		//LEFT
				posPlayer.x -= DASH_MOVEMENT + dashing_count / DIV;
				if (map->collisionMoveLeft(posPlayer, glm::ivec2(dimxPlayer, dimyPlayer))) {
					posPlayer.x += DASH_MOVEMENT + dashing_count / DIV;
					dashing_count = 1;
				}
				break;
			case 7:		//LEFT_UP
				posPlayer.x -= DASH_MOVEMENT + dashing_count / DIV;
				if (map->collisionMoveLeft(posPlayer, glm::ivec2(dimxPlayer, dimyPlayer))) {
					posPlayer.x += DASH_MOVEMENT + dashing_count / DIV;
				}
				posPlayer.y -= DASH_MOVEMENT + dashing_count / DIV;
				if (map->collisionMoveUp(posPlayer, glm::ivec2(dimxPlayer, dimyPlayer))) {
					posPlayer.y += DASH_MOVEMENT + dashing_count / DIV;
				}
				break;
			}
			//if (dashing_count < 6) {
			//	posPlayer.y += 10 / dashing_count;
			//}
		}
	}
	if (dashing_count < 2) {
		if (Game::instance().getSpecialKey(GLUT_KEY_LEFT))
		{
			if (Game::instance().getSpecialKey(GLUT_KEY_UP)) {
				direction = LEFT_UP;
			}
			else if (Game::instance().getSpecialKey(GLUT_KEY_DOWN)) {
				direction = LEFT_DOWN;
			}
			else {
				direction = LEFT;
			}
			if (sprite->animation() != MOVE_LEFT)
				sprite->changeAnimation(MOVE_LEFT);
			posPlayer.x -= 2;
			if (map->collisionMoveLeft(posPlayer, glm::ivec2(dimxPlayer, dimyPlayer)))
			{
				posPlayer.x += 2;
				sprite->changeAnimation(STAND_LEFT);
				hasCollision = true;
			}
		}
		else if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT))
		{
			if (Game::instance().getSpecialKey(GLUT_KEY_UP)) {
				direction = RIGHT_UP;
			}
			else if (Game::instance().getSpecialKey(GLUT_KEY_DOWN)) {
				direction = RIGHT_DOWN;
			}
			else {
				direction = RIGHT;
			}
			if (sprite->animation() != MOVE_RIGHT)
				sprite->changeAnimation(MOVE_RIGHT);
			posPlayer.x += 2;
			if (map->collisionMoveRight(posPlayer, glm::ivec2(dimxPlayer, dimyPlayer)))
			{
				posPlayer.x -= 2;
				sprite->changeAnimation(STAND_RIGHT);
				hasCollision = true;
			}
		}
		else if (Game::instance().getSpecialKey(GLUT_KEY_UP)) {
			direction = UP;
		}
		else if (Game::instance().getSpecialKey(GLUT_KEY_DOWN)) {
			direction = DOWN;
		}
		else if (!bJumping) // doing nothing
		{
			if (sprite->animation() == MOVE_LEFT || sprite->animation() == JUMPING_LEFT || sprite->animation() == LOOK_LEFT_DOWN || sprite->animation() == LOOK_LEFT_UP){
				sprite->changeAnimation(STAND_LEFT);
				direction = LEFT;
			}
			else if (sprite->animation() == MOVE_RIGHT || sprite->animation() == JUMPING_RIGHT || sprite->animation() == LOOK_RIGHT_DOWN || sprite->animation() == LOOK_RIGHT_UP) {
				sprite->changeAnimation(STAND_RIGHT);
				direction = RIGHT;
			}
		}

		if (bJumping)
		{
			if (hasCollision && jumpAngle >= 90) {
				bJumping = false;
			}
			else {
				jumpAngle += JUMP_ANGLE_STEP;

				if (map->collisionMoveUp(posPlayer, glm::ivec2(dimxPlayer, dimyPlayer))) {
					startY = posPlayer.y + 97;
					jumpAngle = 92.f; // 4*23
				}


				if (jumpAngle == 180)
				{
					bJumping = false;
					posPlayer.y = startY;
				}
				else
				{
					posPlayer.y = int(startY - 96 * sin(3.14159f * jumpAngle / 180.f));
					if (jumpAngle > 90)
						bJumping = !map->collisionMoveDown(posPlayer, glm::ivec2(dimxPlayer, dimyPlayer), &posPlayer.y);
				}
			}
		}
		else
		{
			if (hasCollision) posPlayer.y = posPlayer.y + FALL_STEP - 3;
			else posPlayer.y += FALL_STEP;
			if (map->collisionMoveDown(posPlayer, glm::ivec2(dimxPlayer, dimyPlayer), &posPlayer.y))
			{
				if (bDashing) {
					bDashing = false;
					dashing_count = 0;
				}
				if (Game::instance().getKey('c') || Game::instance().getKey('C'))
				{
					bJumping = true;
					// start jumping
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
			}
			else { // falling
				if (sprite->animation() == STAND_LEFT || sprite->animation() == MOVE_LEFT || sprite->animation() == LOOK_LEFT_DOWN || sprite->animation() == LOOK_LEFT_UP)
					sprite->changeAnimation(JUMPING_LEFT);
				else if (sprite->animation() == STAND_RIGHT || sprite->animation() == MOVE_RIGHT || sprite->animation() == LOOK_RIGHT_DOWN || sprite->animation() == LOOK_RIGHT_UP)
					sprite->changeAnimation(JUMPING_RIGHT);
			}
		}
		if (Game::instance().getKey('x') || Game::instance().getKey('X'))
		{
			bDashing = true;
			bJumping = false;
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




