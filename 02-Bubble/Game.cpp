#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"

enum lvl {
	Title, lvl1, lvl2, lvl3, lvl4, lvl5, lvl6, lvl7, lvl8, lvl9, lvl10, lvl11, Credits, Instruccions
};


void Game::init()
{
	transition = false;
	bPlay = true;
	state = 0;
	glClearColor(0.f, 0.f, 0.f, 1.0f);
	scene.init();
}

bool Game::update(int deltaTime)
{
	scene.update(deltaTime, state, &transition);
	
	return bPlay;
}

void Game::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	scene.render(state);
}

void Game::keyPressed(int key)
{
	switch (state) {
	case Title:
		switch (key)
		{
		case 27: bPlay = false;
			break;
		case 'c':
		case 'x':
		case 'C':
		case 'X':
			transition = true;
			scene.resetPlayer();
			state = lvl1;
			scene.update_map(state);
			break;
		case 'i':
		case 'I':
			state = Instruccions;
		default:
			break;
		}
		break;
	case Instruccions:
		switch (key)
		{
		case 27: bPlay = false;
			break;
		case 13: //enter
			state = Title;
			break;
		default:
			break;
		}
		break;
	case Credits:
		switch (key)
		{
		case 27: bPlay = false;
			break;
		case 13:
			state = Title;
			scene.re_init_credits();
			break;
		default:
			break;
		}
		break;
	default:
		switch (key)
		{
		case 27: bPlay = false;
			break;
		case 'G':
		case 'g':
			if (scene.getVulnerability()) scene.setVulnerability(false);
			else scene.setVulnerability(true);
			break;
		case 'D':
		case 'd':
			if (scene.getInfinity_dash()) scene.setInfinity_dash(true);
			else scene.setInfinity_dash(false);
			break;
		default:
			break;
		}
		break;
	}
	keys[key] = true;
}

void Game::keyReleased(int key)
{
	if (!transition) {
		switch (key)
		{
		case '1':
			if (state != lvl1) {
				state = lvl1;
				transition = true;
			}
			break;
		case '2':
			if (state != lvl2) {
				state = lvl2;
				transition = true;
			}
			break;
		case '3':
			if (state != lvl3) {
				state = lvl3;
				transition = true;
			}
			break;
		case '4':
			if (state != lvl4) {
				state = lvl4;
				transition = true;
			}
			break;
		case '5':
			if (state != lvl5) {
				state = lvl5;
				transition = true;
			}
			break;
		case '6':
			if (state != lvl6) {
				state = lvl6;
				transition = true;
			}
			break;
		case '7':
			if (state != lvl7) {
				state = lvl7;
				transition = true;
			}
			break;
		case '8':
			if (state != lvl8) {
				state = lvl8;
				transition = true;
			}
			break;
		case '9':
			if (state != lvl9) {
				state = lvl9;
				transition = true;
			}
			break;
		case '0':
			if (state != lvl10) {
				state = lvl10;
				transition = true;
			}
			break;
		default:
			break;
		}
		if (transition) {
			scene.resetPlayer();
			scene.update_map(state);
		}
	}
	keys[key] = false;
}

void Game::specialKeyPressed(int key)
{
	specialKeys[key] = true;
}

void Game::specialKeyReleased(int key)
{
	specialKeys[key] = false;
}

void Game::mouseMove(int x, int y)
{
}

void Game::mousePress(int button)
{
}

void Game::mouseRelease(int button)
{
}

bool Game::getKey(int key) const
{
	return keys[key];
}

bool Game::getSpecialKey(int key) const
{
	return specialKeys[key];
}

void Game::canviDestat() {
	++state;
	if (state < 12) {
		transition = true;
		scene.resetPlayer();
		scene.update_map(state);
	}
}





