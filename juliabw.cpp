#include <SDL2/SDL.h>
#include <stdio.h>
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <cmath>
using namespace std;

//Starts up SDL and creates new window
void init();

//Generates colormap data, pixel by pixel
int generateSet();

//Draws surface from pixel map
void draw();

//Lets the user close the window
void clickEvent();

//Frees media and shuts down SDL
void close();

//The window we'll be rendering to
SDL_Window* window = NULL;

//The surface contained by the window
SDL_Surface* surface = NULL;

//The image we will load and show on the screen
SDL_Surface* screenSurface = NULL;

const int SCREEN_WIDTH = 600;
const int SCREEN_HEIGHT = 600;
int channels = 3;
char* map = new char[SCREEN_WIDTH * SCREEN_HEIGHT * channels];
double cr,ci;


void init() {
	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
	}
	else {
		//Create window
		window = SDL_CreateWindow( "Julia", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

		//Get window surface
		screenSurface = SDL_GetWindowSurface(window);
	}
}

int generateSet() {
	int max=255;
	int colorMax=0;
	for (int j=0;j<SCREEN_HEIGHT;j++){
		for (int i=0; i<SCREEN_WIDTH;i++){
			double real=1.7*(i-0.5*SCREEN_WIDTH)/(0.5*SCREEN_WIDTH);
			double imag=1.7*(j-0.5*SCREEN_HEIGHT)/(0.5*SCREEN_HEIGHT);
			double magsq;
			double oldReal;
			int iter=max;
			for (int k=1;k<=max;k++){
				oldReal=real;
				real=(oldReal*oldReal)-(imag*imag)+cr;
				imag=2*oldReal*imag+ci;
				magsq=real*real+imag*imag;
				if (magsq>=4){
					iter=k;
					if (iter>colorMax && iter != max){
						colorMax=iter;
					}
					break;
				}
			}

			map[(j * SCREEN_WIDTH + i) * 3] = iter;
			map[(j * SCREEN_WIDTH + i) * 3 + 1] = iter;
			map[(j * SCREEN_WIDTH + i) * 3 + 2] = iter;
		}
	}

	for (int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT * channels; i++) {
		if (map[i] != max) {
			map[i] = (255 * map[i]) / colorMax;
		}
	}

	return 0;
}

void draw() {
	surface = SDL_CreateRGBSurfaceFrom((void*) map,
		SCREEN_WIDTH, SCREEN_HEIGHT, channels * 8, SCREEN_WIDTH * channels, 0x0000FF, 0x00FF00, 0xFF0000, 0);
}

void clickEvent() {
	bool quit = false;
	SDL_Event e;
	while (!quit) {
		while(SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT ) {
				quit = true;
			}
		}
	}
}

void close() {
	//Deallocate surfaces
	SDL_FreeSurface(surface);
	SDL_FreeSurface(screenSurface);
	surface = NULL;
	screenSurface = NULL;

	//Destroy window
	SDL_DestroyWindow(window);
	window = NULL;

	//Quit SDL
	SDL_Quit();
}

int main(int argc, char* args[]) {
	//Start up SDL and create window

	cin >> cr >> ci;

	if (generateSet() != 0){
		printf("Error!");
	}

	init();

	draw();

	//Apply the image
	SDL_BlitSurface(surface, NULL, screenSurface, NULL);

	//Update the surface
	SDL_UpdateWindowSurface(window);

	//Keep window open
	clickEvent();

	//Free resources and close SDL
	close();

	return 0;
}