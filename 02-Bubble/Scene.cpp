#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"


#define SCREEN_X 0//64
#define SCREEN_Y 0//16


Scene::Scene()
{
	map = NULL;
	player = NULL;
}

Scene::~Scene()
{
	if(map != NULL)
		delete map;
	if(player != NULL)
		delete player;
}


void Scene::init()
{
	initShaders();

	spritesheet_credits.loadFromFile("images/credits.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite_credits = Sprite::createSprite(glm::ivec2(512, 1024), glm::vec2(1, 1), &spritesheet_credits, &texProgram);


	spritesheet_pantalla.loadFromFile("images/menu.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite_pantalla = Sprite::createSprite(glm::ivec2(512, 512), glm::vec2(1, 1), &spritesheet_pantalla, &texProgram);
	sprite_pantalla->setPosition(glm::vec2(0, 0));

	map = TileMap::createTileMap("levels/level01.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	player = new Player();
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	player->initScore();
	init_player();
	vulnerability = true;
	init_entities();

	projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
	currentTime = 0.0f;

	jumpAngle = 180;
	shake = 0;
	creditant = 0;
	showScore = 0;

	// Select which font you want to use
	if (!text.init("fonts/OpenSans-Regular.ttf"))
		//if(!text.init("fonts/OpenSans-Bold.ttf"))
		//if(!text.init("fonts/DroidSerif.ttf"))
		cout << "Could not load font!!!" << endl;
}

void Scene::init_player()
{
	//player = new Player();
	//player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	initPos = map->getInitialPos();
	player->setPosition(glm::vec2(initPos.x * map->getTileSize(), initPos.y * map->getTileSize()));
	player->setTileMap(map);
	dead = false;
	//vulnerability = true;
}

void Scene::init_entities()
{
	entities = map->getEntities();
	std::vector<int> positionx = map->getPosx();
	std::vector<int> positiony = map->getPosy();
	Entity* e;
	for (unsigned int i = 0;i < entities.size();i++) {
		e = new Entity();
		e->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, entities[i]);
		e->setPosition(glm::vec2(positionx[i] * map->getTileSize(), positiony[i] * map->getTileSize()));
		e->setTileMap(map);
		entity.push_back(e);
	}
}

void Scene::update_map(int state) {
	if (state > 0 && state < 12) {
		if (state < 10) {
			map = TileMap::createTileMap("levels/level0" + to_string(state) + ".txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
			//player->setTileMap(map);
		}
		else {
			map = TileMap::createTileMap("levels/level" + to_string(state) + ".txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
			//player->setTileMap(map);
		}
		//posPlayer = glm::ivec2(0,0)
		init_player();
		entity.clear();
		init_entities();
	}
}

void Scene::update(int deltaTime, int state, bool *transition)
{
	if (*transition == false && (!dead || !vulnerability)) {
		if (state > 0 && state < 12) {
			currentTime += deltaTime;
			player->update(deltaTime);
			updateEntities(deltaTime);
		}
	}
	else if (dead || *transition) {
		if (jumpAngle == 180)
		{
			jumpAngle = 0;
			startY = 512;
		}
		else
		{
			if (shake == 0) {
				jumpAngle += 4;
				posPlayer = glm::vec2(initPos.x * map->getTileSize(), int(startY - 4 * 40 * sin(3.14159f * jumpAngle / 180.f)));
				player->setPosition(posPlayer);
			}
			if (jumpAngle > 90) {
				if ((map->collisionMoveDown(posPlayer, glm::ivec2(28, 28), &posPlayer.y) && (!map->collisionMoveDown(posPlayer, glm::ivec2(28, 0), &posPlayer.y))) || shake > 0) {
					switch (shake) {
					case 4:
						if (state < 10) {
							map = TileMap::createTileMap("levels/level0" + to_string(state) + ".txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
						}
						else {
							map = TileMap::createTileMap("levels/level" + to_string(state) + ".txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
						}
						init_player();
						player->resetDash();
						*transition = false;
						jumpAngle = 180;
						shake = 0;
						break;
					default:
						if (state < 10) {
							map = TileMap::createTileMap("levels/level0" + to_string(state) + ".txt", glm::vec2(((shake % 2) * 2 - 1) * 5, (((shake + 1) % 2) * 2 - 1) * 5), texProgram);
						}
						else {
							map = TileMap::createTileMap("levels/level" + to_string(state) + ".txt", glm::vec2(((shake % 2) * 2 - 1) * 5, (((shake + 1) % 2) * 2 - 1) * 5), texProgram);
						}
						posPlayer = glm::vec2(initPos.x * map->getTileSize() + ((shake % 2) * 2 - 1) * 5, int(startY - 4 * 40 * sin(3.14159f * jumpAngle / 180.f)) + (((shake + 1) % 2) * 2 - 1) * 5);
						++shake;
						break;
					}
					player->setPosition(posPlayer);
				}
			}
		}
	}
}

void Scene::updateEntities(int deltaTime)
{
	glm::ivec2 pos = player->getPosition();
	dead = map->collisionMoveDown(pos, glm::ivec2(28, 28), &pos.y);
	for (unsigned int i = 0;i < entity.size();i++) {
		entity[i]->update(deltaTime);
		switch (entities[i]) {
		case 13:
		case 14:
		case 0:
			if (entity[i]->collisionEntity(pos, glm::vec2(28, 28)) && !dead) {
				dead = true;
			}
			break;
		case 1:
			if (entity[i]->collisionEntity(pos, glm::vec2(28, 28))) {
				player->increaseScore();
				showScore = 100;
				entity.erase(entity.begin() + i);
				entities.erase(entities.begin() + i);
				if (entities[i] == 3) {
					entity[i]->update(deltaTime);
					entity.erase(entity.begin() + i);
					entities.erase(entities.begin() + i);
					entity.erase(entity.begin() + i - 1);
					entities.erase(entities.begin() + i - 1);
					i += 1;
				}
			}
			break;
		case 4:
			if (entity[i]->collisionEntity(pos, glm::vec2(28, 28))) {
				entity.erase(entity.begin() + i);
				entities.erase(entities.begin() + i);
				unsigned int e = 0;
				for (;e < entities.size() && entities[e] != 5;e++) {}
				entity[e]->transformChest();
			}
			break;
		case 5:
			if (entity[i]->collisionEntity(pos, glm::vec2(28, 28))) {
				player->increaseScore();
				showScore = 100;
				entity.erase(entity.begin() + i);
				entities.erase(entities.begin() + i);
			}
			break;
		case 6:
			if (entity[i]->collisionEntity(pos, glm::vec2(28, 28)) && entity[i]->getHideBalloon() == 0) {
				entity[i]->setHideBalloon();
				player->resetJump();
			}
			break;
		case 8:

			break;
		case 9:
			break;
		case 10:
			/*if (entity[i]->collisionBox(pos, glm::vec2(28, 28))) {
				entity[i]->destroyBox();
			}*/
			break;
		case 11:
			if (entity[i]->collisionEntity(pos, glm::vec2(28, 28))) {
				player->trampoline();
			}
			break;
		case 12:
			if (entity[i]->collisionEntity(pos, glm::vec2(28, 28))) {
				Game::instance().canviDestat();
			}
			break;
		default:
			break;
		}
	}
}

void Scene::render(int state)
{
	glm::mat4 modelview;

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	std::string message;
	switch (state) {
	case 0:			//Title
		spritesheet_pantalla.loadFromFile("images/menu.png", TEXTURE_PIXEL_FORMAT_RGBA);
		sprite_pantalla->render();
		break;
	case 13:		//Instructions
		spritesheet_pantalla.loadFromFile("images/instruccions.png", TEXTURE_PIXEL_FORMAT_RGBA);
		sprite_pantalla->render();
		break;
	case 12:		//Credits
		if (creditant > -512) {
			--creditant;
			sprite_credits->setPosition(glm::vec2(0, creditant));
		}
		sprite_credits->render();
		message = "Final Score: ";
		message += std::to_string(player->getScore());
		text.render(message, glm::vec2(10, SCREEN_HEIGHT - 20), 32, glm::vec4(1, 0, 0, 1));
		break;
	default:
		map->render();
		player->render();
		for (unsigned int i = 0;i < entity.size();i++) entity[i]->render();
		if (showScore > 0) {
			message = "Score: ";
			message += std::to_string(player->getScore());
			text.render(message, glm::vec2(10, SCREEN_HEIGHT - 20), 32, glm::vec4(1, 0, 0, 1));
			showScore--;
		}
		break;
	}
}

void Scene::re_init_credits() {
	creditant = 0;
}

void Scene::setVulnerability(bool v)
{
	vulnerability = v;
}

bool Scene::getVulnerability()
{
	return vulnerability;
}

void Scene::setInfinity_dash(bool d)
{
	player->setInfinity_dash(d);
}

bool Scene::getInfinity_dash()
{
	return player->getInfinity_dash();
}

void Scene::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if(!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if(!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	texProgram.init();
	texProgram.addShader(vShader);
	texProgram.addShader(fShader);
	texProgram.link();
	if(!texProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << texProgram.log() << endl << endl;
	}
	texProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}

void Scene::resetPlayer() {
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
}




