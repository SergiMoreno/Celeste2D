#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"

#include <stdio.h>
//#include <SDL.h>
//#include <SDL_mixer.h>
#include <iostream>

#define WIDTH 1280
#define HEIGHT 720



//Remove console (only works in Visual Studio)
#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")


#define TIME_PER_FRAME 1000.f / 60.f // Approx. 60 fps
//#define WAV_PATH "music/Ntempo.mp3" //WORK TOTALY FINE


static int prevTime;


// If a key is pressed this callback is called

static void keyboardDownCallback(unsigned char key, int x, int y)
{
	Game::instance().keyPressed(key);
}

// If a key is released this callback is called

static void keyboardUpCallback(unsigned char key, int x, int y)
{
	Game::instance().keyReleased(key);
}

// If a special key is pressed this callback is called

static void specialDownCallback(int key, int x, int y)
{
	Game::instance().specialKeyPressed(key);
}

// If a special key is released this callback is called

static void specialUpCallback(int key, int x, int y)
{
	Game::instance().specialKeyReleased(key);
}

// Same for changes in mouse cursor position

static void motionCallback(int x, int y)
{
	Game::instance().mouseMove(x, y);
}

// Same for mouse button presses or releases

static void mouseCallback(int button, int state, int x, int y)
{
	if(state == GLUT_DOWN)
		Game::instance().mousePress(button);
	else if(state == GLUT_UP)
		Game::instance().mouseRelease(button);
}

static void drawCallback()
{
	Game::instance().render();
	glutSwapBuffers();
}

static void idleCallback()
{
	int currentTime = glutGet(GLUT_ELAPSED_TIME);
	int deltaTime = currentTime - prevTime;
	
	if(deltaTime > TIME_PER_FRAME)
	{
		// Every time we enter here is equivalent to a game loop execution
		if(!Game::instance().update(deltaTime))
			exit(0);
		prevTime = currentTime;
		glutPostRedisplay();
	}
}

//void init_music() {
	/*Mix_Music* wave = NULL;
	if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) != 0) {
		//std::cout << "Impossible d'ouvrir mixer " << SDL_GetError() << std::endl;
	}
	wave = Mix_LoadMUS(WAV_PATH);
	if (wave == NULL) {
		//std::cout << "Impossible de charg� un fichier .ogg " << SDL_GetError() << std::endl;
	}
	if (Mix_PlayMusic(wave, 1) != 0) {
		//std::cout << "Impossible de jouer le fichier " << SDL_GetError() << std::endl;
	}
	//SDL_Delay(5000);

	while (Mix_PlayingMusic());

	Mix_FreeMusic(wave);
	//SDL_DestroyWindow(window);
	//SDL_Quit();*/
	//if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {}
		

//}

int main(int argc, char **argv)
{
	// GLUT initialization
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);

	glutCreateWindow(argv[0]);
	glutDisplayFunc(drawCallback);
	glutIdleFunc(idleCallback);
	glutKeyboardFunc(keyboardDownCallback);
	glutKeyboardUpFunc(keyboardUpCallback);
	glutSpecialFunc(specialDownCallback);
	glutSpecialUpFunc(specialUpCallback);
	glutMouseFunc(mouseCallback);
	glutMotionFunc(motionCallback);

	// GLEW will take care of OpenGL extension functions
	glewExperimental = GL_TRUE;
	glewInit();
	
	//init_music();

	// Game instance initialization
	Game::instance().init();
	prevTime = glutGet(GLUT_ELAPSED_TIME);
	// GLUT gains control of the application
	glutMainLoop();

	return 0;
}



