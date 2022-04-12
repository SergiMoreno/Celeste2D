#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Player.h"
#include "Game.h"


#define JUMP_ANGLE_STEP 4/slowmo
#define JUMP_HEIGHT 96
#define FALL_STEP 4/slowmo
#define DASH_MOVEMENT 0
#define DIV 2* ((slowmo-1) * 3 + 1)
#define MOVIMENT_LATERAL 2/slowmo


enum PlayerAnims
{
	STAND_LEFT, STAND_RIGHT, MOVE_LEFT, MOVE_RIGHT, LOOK_RIGHT_UP, LOOK_RIGHT_DOWN, LOOK_LEFT_UP, LOOK_LEFT_DOWN,
	JUMPING_RIGHT, JUMPING_LEFT, DUSHING_RIGHT, DUSHING_LEFT, CLIMBING_LOOK_RIGHT, CLIMBING_LOOK_LEFT
};

enum PlayerDirection
{
	UP, RIGHT_UP, RIGHT, RIGHT_DOWN, DOWN, LEFT_DOWN, LEFT, LEFT_UP
};


void Player::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	bJumping = false;
	cReleased = true;
	sPressed = false;
	xReleased = false;
	direction = RIGHT;
	bDashing = false;
	bClimbing = false;
	bMoving = false;
	slowmo = 1;
	infinity_dash = false;
	//hasCollision = false;
	fromWall = false;
	dashing_count = 0;
	walking = 0;
	score = 0;
	transicion = 0;
	spritesheet.loadFromFile("images/player.png", TEXTURE_PIXEL_FORMAT_RGBA);
	float dimx = 8.f / 24.f;
	float dimy = 8.f / 48.f;
	dimyPlayer = 32;
	dimxPlayer = 32;
	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(dimx, dimy), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(14);
	
		sprite->setAnimationSpeed(STAND_RIGHT, 8);
		sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.f, 0.f));
		
		sprite->setAnimationSpeed(STAND_LEFT, 8);
		sprite->addKeyframe(STAND_LEFT, glm::vec2(0.f, dimy));
		
		sprite->setAnimationSpeed(MOVE_RIGHT, 8);
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.f, 0.f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(dimx, 0.f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(dimx, 0.f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(2 * dimx, 0.f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(2 * dimx, 0.f));
		
		sprite->setAnimationSpeed(MOVE_LEFT, 6);
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
		sprite->addKeyframe(DUSHING_RIGHT, glm::vec2(0.f, 2 * dimy));//5 * dimy));

		sprite->setAnimationSpeed(DUSHING_LEFT, 8);
		sprite->addKeyframe(DUSHING_LEFT, glm::vec2(dimx, 2 * dimy));//5 * dimy));

		sprite->setAnimationSpeed(CLIMBING_LOOK_RIGHT, 8);
		sprite->addKeyframe(CLIMBING_LOOK_RIGHT, glm::vec2(0.f, 5 * dimy));

		sprite->setAnimationSpeed(CLIMBING_LOOK_LEFT, 8);
		sprite->addKeyframe(CLIMBING_LOOK_LEFT, glm::vec2(dimx, 5 * dimy));
		
	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

void Player::update(int deltaTime)
{
	bool hasCollision = false;
	int init = posPlayer.y;
	int initA = jumpAngle;
	int initx = posPlayer.x;
	sprite->update(deltaTime);

	if (Game::instance().getKey('s') || Game::instance().getKey('S')) sPressed = true;

	if (!Game::instance().getKey('s') && !Game::instance().getKey('S') && sPressed){
			switch (slowmo) {
			case 1:
				slowmo = 2;
				if (dashing_count > 1) dashing_count = dashing_count * 2;
				break;
			default:
				slowmo = 1;
				if(dashing_count>1) dashing_count = dashing_count / 2;
				break;
			}
			sPressed = false;
	}

	if (!Game::instance().getKey('c') && !Game::instance().getKey('C')) cReleased = true;
	if (!Game::instance().getKey('x') && !Game::instance().getKey('X')) xReleased = true;

	if (bDashing) {
		if (dashing_count == 0) {
			dash_direction = direction;
			dashing_count = 24*slowmo;
		}
		else if (dashing_count != 1) {
			--dashing_count;
			switch (direction) {
			case 0:		//UP
				posPlayer.y -= DASH_MOVEMENT + dashing_count / (DIV);
				if (map->collisionMoveUp(posPlayer, glm::ivec2(32, 32))) {
					posPlayer.y += DASH_MOVEMENT + dashing_count / (DIV);
					dashing_count = 1;
				}
				break;
			case 1:		//RIGHT_UP
				posPlayer.y -= DASH_MOVEMENT + dashing_count / (DIV);
				if (map->collisionMoveUp(posPlayer, glm::ivec2(dimxPlayer, dimyPlayer))) {
					posPlayer.y += DASH_MOVEMENT + dashing_count / (DIV);
				}
				posPlayer.x += DASH_MOVEMENT + dashing_count / (DIV);
				if (map->collisionMoveRight(posPlayer, glm::ivec2(dimxPlayer, dimyPlayer))) {
					posPlayer.x -= DASH_MOVEMENT + dashing_count / (DIV);
				}
				break;
			case 2:		//RIGHT
				posPlayer.x += DASH_MOVEMENT + dashing_count / (DIV);
				if (map->collisionMoveRight(posPlayer, glm::ivec2(dimxPlayer, dimyPlayer))) {
					posPlayer.x -= DASH_MOVEMENT + dashing_count / (DIV);
					dashing_count = 1;
				}
				break;
			case 3:		//RIGHT_DOWN
				posPlayer.x += DASH_MOVEMENT + dashing_count / (DIV);
				if (map->collisionMoveRight(posPlayer, glm::ivec2(dimxPlayer, dimyPlayer))) {
					posPlayer.x -= DASH_MOVEMENT + dashing_count / (DIV);
				}
				if (DASH_MOVEMENT + dashing_count / (DIV) >= FALL_STEP) posPlayer.y += DASH_MOVEMENT + dashing_count / (DIV);
				else  posPlayer.y += FALL_STEP;
				if (map->collisionMoveDown(posPlayer, glm::ivec2(dimxPlayer, dimyPlayer), &posPlayer.y)) {
					//posPlayer.y -= DASH_MOVEMENT + dashing_count / DIV;
				}
				break;
			case 4:		//DOWN
				if (DASH_MOVEMENT + dashing_count / (DIV) >= FALL_STEP) posPlayer.y += DASH_MOVEMENT + dashing_count / (DIV);
				else  posPlayer.y += FALL_STEP;
				if (map->collisionMoveDown(posPlayer, glm::ivec2(dimxPlayer, dimyPlayer), &posPlayer.y)) {
					//posPlayer.y -= DASH_MOVEMENT + dashing_count / DIV;
					dashing_count = 1;
				}
				break;
			case 5:		//LEFT_DOWN
				if (DASH_MOVEMENT + dashing_count / (DIV) >= FALL_STEP) posPlayer.y += DASH_MOVEMENT + dashing_count / (DIV);
				else  posPlayer.y += FALL_STEP;
				if (map->collisionMoveDown(posPlayer, glm::ivec2(dimxPlayer, dimyPlayer), &posPlayer.y)) {
					//posPlayer.y -= DASH_MOVEMENT + dashing_count / DIV;
				}

				posPlayer.x -= DASH_MOVEMENT + dashing_count / (DIV);
				if (map->collisionMoveLeft(posPlayer, glm::ivec2(dimxPlayer, dimyPlayer))) {
					posPlayer.x += DASH_MOVEMENT + dashing_count / (DIV);
				}
				break;
			case 6:		//LEFT
				posPlayer.x -= DASH_MOVEMENT + dashing_count / (DIV);
				if (map->collisionMoveLeft(posPlayer, glm::ivec2(dimxPlayer, dimyPlayer))) {
					posPlayer.x += DASH_MOVEMENT + dashing_count / (DIV);
					dashing_count = 1;
				}
				break;
			case 7:		//LEFT_UP
				posPlayer.x -= DASH_MOVEMENT + dashing_count / (DIV);
				if (map->collisionMoveLeft(posPlayer, glm::ivec2(dimxPlayer, dimyPlayer))) {
					posPlayer.x += DASH_MOVEMENT + dashing_count / (DIV);
				}
				posPlayer.y -= DASH_MOVEMENT + dashing_count / (DIV);
				if (map->collisionMoveUp(posPlayer, glm::ivec2(dimxPlayer, dimyPlayer))) {
					posPlayer.y += DASH_MOVEMENT + dashing_count / (DIV);
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
			posPlayer.x -= MOVIMENT_LATERAL;
			if (map->collisionMoveLeft(posPlayer, glm::ivec2(dimxPlayer, dimyPlayer)))
			{
				posPlayer.x += MOVIMENT_LATERAL;
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

			posPlayer.x += MOVIMENT_LATERAL;

			if (map->collisionMoveRight(posPlayer, glm::ivec2(dimxPlayer, dimyPlayer)))
			{
				posPlayer.x -= MOVIMENT_LATERAL;
				hasCollision = true;
			}
		}
		else if (Game::instance().getSpecialKey(GLUT_KEY_UP)) {
			direction = UP;
		}
		else if (Game::instance().getSpecialKey(GLUT_KEY_DOWN)) {
			direction = DOWN;
		}
		else if (!bJumping && !bClimbing)
		{
			if (looksLeft())//&& (sprite->animation() != JUMPING_LEFT || sprite->animation() != CLIMBING_LOOK_LEFT)
				sprite->changeAnimation(STAND_LEFT);
			else if (looksRight())
				sprite->changeAnimation(STAND_RIGHT);
		}

		//if (hasCollision) bJumping = false;
		//else bClimbing = false;

		if (bJumping)
		{
			if (!hasCollision) {
				jumpAngle += JUMP_ANGLE_STEP;
				if (map->collisionMoveUp(posPlayer, glm::ivec2(dimxPlayer, dimyPlayer))) {
					startY = posPlayer.y + 97;
					jumpAngle = 92; // 4*23
				}
				if (jumpAngle == 180)
				{
					bJumping = false;
					jumpAngle = 0;
					// posPlayer.y = startY;
				}
				else
				{
					posPlayer.y = int(startY - 96 * sin(3.14159f * jumpAngle / 180.f));
					if (jumpAngle > 90) // falling
						bJumping = !map->collisionMoveDown(posPlayer, glm::ivec2(dimxPlayer, dimyPlayer), &posPlayer.y);
				}

				if ((Game::instance().getKey('x') || Game::instance().getKey('X')) && (xReleased || infinity_dash))
				{
					bDashing = true;
					bJumping = false;
					xReleased = false;
				}
			}
			else {
				bClimbing = true;
				bJumping = false;
			}
		}
		else {
			posPlayer.y += FALL_STEP;

			if (map->collisionMoveDown(posPlayer, glm::ivec2(dimxPlayer, dimyPlayer), &posPlayer.y))
			{
				if (bDashing) {
					bDashing = false;
					dashing_count = 0;
				}
				if ((Game::instance().getKey('C') || Game::instance().getKey('c')) && (cReleased) && !bClimbing && !hasCollision)
				{
					bJumping = true;
					jumpAngle = 0;
					startY = posPlayer.y;
					cReleased = false;
					if (looksLeft()) sprite->changeAnimation(JUMPING_LEFT);
					else sprite->changeAnimation(JUMPING_RIGHT);
				}
				else if ((Game::instance().getKey('C') || Game::instance().getKey('c')) && (cReleased) && hasCollision)
				{
					bClimbing = true;
					cReleased = false;
				}
			}
			else {
				if (looksLeft()) sprite->changeAnimation(JUMPING_LEFT);
				else sprite->changeAnimation(JUMPING_RIGHT);
			}

			if ((Game::instance().getKey('x') || Game::instance().getKey('X')) && (xReleased || infinity_dash))
			{
				bDashing = true;
				bJumping = false;
				xReleased = false;
			}
		}

		if (bClimbing) {
			jumpAngle = initA + JUMP_ANGLE_STEP;
			if (!hasCollision) bClimbing = false;
			else {
				if (looksLeft()) sprite->changeAnimation(CLIMBING_LOOK_RIGHT);
				else sprite->changeAnimation(CLIMBING_LOOK_LEFT);
				posPlayer.y = init - 1;
				if (map->collisionMoveUp(posPlayer, glm::ivec2(dimxPlayer, dimyPlayer))) {
					posPlayer.y = init + 2;
					jumpAngle = 120; // 4*23
					hasCollision = false;
				}
				else {
					if ((Game::instance().getKey('C') || Game::instance().getKey('c')) && (cReleased))
					{
						fromWall = true;
						hasCollision = false;
						wallAngle = 90;
						otherY = init;
						cReleased = false;
						transicion = 100;
						//posPlayer.y = init + 1;
						//if (!map->collisionMoveDown(posPlayer, glm::ivec2(dimxPlayer, dimyPlayer), &posPlayer.y)) {}
						if (looksLeft()) {
							initx -= 2;
							sprite->changeAnimation(JUMPING_LEFT);
							Game::instance().specialKeyReleased(GLUT_KEY_RIGHT);
						}
						else {
							initx += 2;
							sprite->changeAnimation(JUMPING_RIGHT);
						}
					}
					else {
						//if (looksLeft()) sprite->changeAnimation(CLIMBING_LOOK_RIGHT);
						//else sprite->changeAnimation(CLIMBING_LOOK_LEFT);
					}
				}
				if (jumpAngle > 120)
				{
					bClimbing = false;
					jumpAngle = 0;
				}
			}
		}
		else if (hasCollision) { // && jumpAngle == 0
			posPlayer.y = init + 1;
			if (!map->collisionMoveDown(posPlayer, glm::ivec2(dimxPlayer, dimyPlayer), &posPlayer.y)) {
				if (looksLeft()) sprite->changeAnimation(CLIMBING_LOOK_RIGHT);
				else sprite->changeAnimation(CLIMBING_LOOK_LEFT);

				if ((Game::instance().getKey('C') || Game::instance().getKey('c')) && (cReleased))
				{
					fromWall = true;
					hasCollision = false;
					wallAngle = 90;
					otherY = init;
					cReleased = false;
					transicion = 100;
					//posPlayer.y = init + 1;
					//if (!map->collisionMoveDown(posPlayer, glm::ivec2(dimxPlayer, dimyPlayer), &posPlayer.y)) {}
					if (looksLeft()) {
						initx -= 2;
						sprite->changeAnimation(JUMPING_LEFT);
						direction = 1;
						Game::instance().specialKeyReleased(GLUT_KEY_RIGHT);
					}
					else {
						initx += 2;
						direction = 2;
						sprite->changeAnimation(JUMPING_RIGHT);
						Game::instance().specialKeyReleased(GLUT_KEY_LEFT);
					}
				}

			}
		}

		if (fromWall && !hasCollision) {
			wallAngle += JUMP_ANGLE_STEP;
			if (direction == 1) {
				posPlayer.x = initx - 1;
				if (map->collisionMoveLeft(posPlayer, glm::ivec2(dimxPlayer, dimyPlayer)))
				{
					posPlayer.x += 1;
					fromWall = false;
				}

				sprite->changeAnimation(JUMPING_LEFT);
			}
			else if (direction == 2) {
				posPlayer.x = initx + 1;
				if (map->collisionMoveRight(posPlayer, glm::ivec2(dimxPlayer, dimyPlayer)))
				{
					posPlayer.x -= 1;
					fromWall = false;
				}
				//posPlayer.y = init - 1;
				sprite->changeAnimation(JUMPING_RIGHT);
			}
			//if (transicion != 0) {
			posPlayer.y = init - 1;
			transicion -= 2;
			//}
			//else posPlayer.y = int(otherY - 96 * sin(3.14159f * wallAngle / 180.f));
			if (map->collisionMoveDown(posPlayer, glm::ivec2(dimxPlayer, dimyPlayer), &posPlayer.y))
				fromWall = false;
		}
	}

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
	if (posPlayer.y < -dimyPlayer) Game::instance().canviDestat();
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

glm::ivec2 Player::getPosition()
{
	return posPlayer;
}

void Player::resetJump() {
	if ((Game::instance().getKey('C') || Game::instance().getKey('c')) && cReleased)
	{
		bJumping = true;
		jumpAngle = 0;
		startY = posPlayer.y;
		cReleased = false;
	}
}

void Player::increaseScore()
{
	score += 1000;
}

void Player::setInfinity_dash(bool d)
{
	infinity_dash = d;
}

bool Player::getInfinity_dash()
{
	return infinity_dash;
}

bool Player::looksLeft()
{
	return (sprite->animation() == STAND_LEFT || sprite->animation() == MOVE_LEFT || sprite->animation() == JUMPING_LEFT || sprite->animation() == LOOK_LEFT_DOWN || sprite->animation() == LOOK_LEFT_UP || sprite->animation() == CLIMBING_LOOK_LEFT);
}

bool Player::looksRight()
{
	return (sprite->animation() == STAND_RIGHT || sprite->animation() == MOVE_RIGHT || sprite->animation() == JUMPING_RIGHT || sprite->animation() == LOOK_RIGHT_DOWN || sprite->animation() == LOOK_RIGHT_UP || sprite->animation() == CLIMBING_LOOK_RIGHT);
}