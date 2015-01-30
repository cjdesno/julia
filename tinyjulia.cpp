#include <SDL2/SDL.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <cstdio>
#include <cmath>
using namespace std;

//Starts up SDL and creates new window
void init();

void anim();

void clickEvent();

//Generates colormap data, pixel by pixel
int generateSet(double x, double y, int zoom);

vector<int> HSLtoRGB(double h,double s,double l);

//Draws surface from pixel map
void draw();

//Frees media and shuts down SDL
void close();

//The window we'll be rendering to
SDL_Window* window = NULL;

//The surface contained by the window
SDL_Surface* surface = NULL;

//The image we will load and show on the screen
SDL_Surface* screenSurface = NULL;

const int SCREEN_WIDTH = 200;
const int SCREEN_HEIGHT = 200;
int channels = 3;
char* map = new char[SCREEN_WIDTH * SCREEN_HEIGHT * channels];
double cr,ci,rinc=0.001,iinc=0.001;
double myX=0.0, myY=0.0;
long long myZoom=1;

int arr[24][3];


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

int generateSet(double x, double y, int zoom) {
	int max=255;

	for (int j=0;j<SCREEN_HEIGHT;j++){
		for (int i=0; i<SCREEN_WIDTH;i++){
			double real=1.5*(i-0.5*SCREEN_WIDTH)/(0.5*SCREEN_WIDTH*zoom)+x;
			double imag=1.5*(j-0.5*SCREEN_HEIGHT)/(0.5*SCREEN_HEIGHT*zoom)+y;
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
					break;
				}
			}
			if (iter == 255) {
				map[(j * SCREEN_WIDTH + i) * 3] = iter;
				map[(j * SCREEN_WIDTH + i) * 3 + 1] = iter;
				map[(j * SCREEN_WIDTH + i) * 3 + 2] = iter;
			}
			else {
				int hue=iter%24;
	            map[(j * SCREEN_WIDTH + i) * 3]=arr[hue][0];
	            map[(j * SCREEN_WIDTH + i) * 3 + 1]=arr[hue][1];
	            map[(j * SCREEN_WIDTH + i) * 3 + 2]=arr[hue][2];
			}
		}
	}
	return 0;
}

void anim() {
	for (int i = 0; i < 200; i++) {
		cr += rinc;
		ci += iinc;
		if (cr >= 1.0 || cr<=-1.0) {
			rinc *= -1.0;
		}
		if (ci >= 1.0||ci<=-1.0) {
			iinc *= -1.0;
		}
		generateSet(myX,myY,myZoom);
		draw();
		SDL_BlitSurface(surface, NULL, screenSurface, NULL);
		SDL_UpdateWindowSurface(window);
	}
}

void clickEvent() {
	bool quit = false;
	SDL_Event e;
	while (!quit) {
		while(SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT ) {
				quit = true;
			}
			else if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
				int pX=e.button.x;
				int pY=e.button.y;
				double oldX=myX;
				double oldY=myY;
				myX=1.5*(pX-0.5*SCREEN_WIDTH)/(0.5*SCREEN_WIDTH*myZoom)+oldX;
				myY=1.5*(pY-0.5*SCREEN_HEIGHT)/(0.5*SCREEN_HEIGHT*myZoom)+oldY;
				if (myZoom <= 2147483648)
					myZoom*=2;
				generateSet(myX, myY, myZoom);
				//increase the counter by 1 and record the x and y position 

				draw();

				//Apply the image
				SDL_BlitSurface(surface, NULL, screenSurface, NULL);

				//Update the surface
				SDL_UpdateWindowSurface(window);
			}
			else if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_RIGHT) {
				int pX=e.button.x;
				int pY=e.button.y;
				double oldX=myX;
				double oldY=myY;
				myX=1.5*(pX-0.5*SCREEN_WIDTH)/(0.5*SCREEN_WIDTH*myZoom)+oldX;
				myY=1.5*(pY-0.5*SCREEN_HEIGHT)/(0.5*SCREEN_HEIGHT*myZoom)+oldY;
				if (myZoom>1) myZoom/=2;
				generateSet(myX, myY, myZoom);
				draw();

				//Apply the image
				SDL_BlitSurface(surface, NULL, screenSurface, NULL);

				//Update the surface
				SDL_UpdateWindowSurface(window);
				/*decrease counter by 1 and record the x and y position */
			}
			else if (e.type==SDL_KEYDOWN && e.key.keysym.sym == SDLK_b) {
				anim();
			}
		}
	}
}

void draw() {
	surface = SDL_CreateRGBSurfaceFrom((void*) map,
		SCREEN_WIDTH, SCREEN_HEIGHT, channels * 8, SCREEN_WIDTH * channels, 0x0000FF, 0x00FF00, 0xFF0000, 0);
}

vector<int> HSLtoRGB(double h,double s,double l){
    double c=(1-abs(2*l-1))*s;
    double hp=h/60.0;
    double x=c*(1-abs(fmod(hp,2.0)-1));
    double m=l-c/2.0;
    vector<int> rgb;
    int r,g,b;
    int fhp=hp;
    switch(fhp){
        case 0:
            r=255*(c+m);
            g=255*(x+m);
            b=255*(m);
            break;
        case 1:
            r=255*(x+m);
            g=255*(c+m);
            b=255*(m);
            break;
        case 2:
            r=255*m;
            g=255*(c+m);
            b=255*(x+m);
            break;
        case 3:
            r=255*(m);
            g=255*(x+m);
            b=255*(c+m);
            break;
        case 4:
            r=255*(x+m);
            g=255*(m);
            b=255*(c+m);
            break;
        case 5:
            r=255*(c+m);
            g=255*(m);
            b=255*(x+m);
            break;
    }
    rgb.push_back(r);
    rgb.push_back(g);
    rgb.push_back(b);
    return rgb;
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

    for (int i=0;i<24;i++){
        vector<int>rgb=HSLtoRGB(i*15,0.7,0.65);
        arr[i][0]=rgb.at(0);
        arr[i][1]=rgb.at(1);
        arr[i][2]=rgb.at(2);
    }

	if (generateSet(myX,myY,myZoom) != 0){
		printf("Error!");
	}

	init();

	draw();

	//Apply the image
	SDL_BlitSurface(surface, NULL, screenSurface, NULL);

	//Update the surface
	SDL_UpdateWindowSurface(window);

	clickEvent();

	//Free resources and close SDL
	close();

	return 0;
}