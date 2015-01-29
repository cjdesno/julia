#include <SDL2/SDL.h>
#include <stdio.h>
#include <iostream>
//#include <map>
#include <vector>
#include <cstdlib>
#include <cstdio>
#include <cmath>
using namespace std;

//Starts up SDL and creates new window
void init();

//Generates colormap data, pixel by pixel
int generateSet();

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

//SDL_Renderer *renderer;

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
	//map<vector<double>,long> colors;
	for (int j=0;j<SCREEN_HEIGHT;j++){
		for (int i=0; i<SCREEN_WIDTH;i++){
			double real=1.7*(i-0.5*SCREEN_WIDTH)/(0.5*SCREEN_WIDTH);
			double imag=1.7*(j-0.5*SCREEN_HEIGHT)/(0.5*SCREEN_HEIGHT);
			//vector<double> coord;
			//coord.push_back(i);
			//coord.push_back(j);
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
					if (iter>colorMax && iter < max){
						colorMax=iter;
					}
					break;
				}
			}

			map[(j * SCREEN_WIDTH + i) * 3] = iter;
			map[(j * SCREEN_WIDTH + i) * 3 + 1] = iter;
			map[(j * SCREEN_WIDTH + i) * 3 + 2] = iter;
			if (iter == max) {
				int b=map[(j * SCREEN_WIDTH + i) * 3 + 2];
				cout << b << endl;
			}



			//colors[coord]=iter;
			//if (iter==max){
			//	cout << '*';
			//}else{
			//	cout<< ' ';
			//}
		}
		//cout << '\n';
	}

	/*for (int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT * channels; i++) {
		if (map[i] != max) {
			map[i] = (255 * map[i]) / colorMax;
		}
	}*/
	//cout << colorMax << endl;

	for (int i=0;i<SCREEN_WIDTH*SCREEN_HEIGHT;i++){
        if (map[3*i]>=colorMax){
            map[3*i]=0;
            map[3*i+1]=0;
            map[3*i+2]=0;
            cout << "sup";

        }
        else{
            double hue=359*log(map[3*i])/log(colorMax);
            if (hue>359){
            	cout << "fuckfuckfuck";
            }
            vector<int> rgb=HSLtoRGB(hue,1.0,0.7);
            map[3*i]=rgb.at(0) - 1;
            map[3*i+1]=rgb.at(1) - 1;
            map[3*i+2]=rgb.at(2) - 1;
        }
    }

	return 0;
}

void draw() {
	//Dummy data
/*	for (int i = 0; i < 200; i++) {
		for (int j = 0; j < 200; j++) {
			map[(i * 200 + j) * 3] = 255;
			map[(i * 200 + j) * 3 + 1] = i - j;
			map[(i * 200 + j) * 3 + 2] = i;
		}
	}*/

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
            r=256*(c+m);
            g=256*(x+m);
            b=256*(m);
            break;
        case 1:
            r=256*(x+m);
            g=256*(c+m);
            b=256*(m);
            break;
        case 2:
            r=256*m;
            g=256*(c+m);
            b=256*(x+m);
            break;
        case 3:
            r=256*(m);
            g=256*(x+m);
            b=256*(c+m);
            break;
        case 4:
            r=256*(x+m);
            g=256*(m);
            b=256*(c+m);
            break;
        case 5:
            r=256*(c+m);
            g=256*(m);
            b=256*(x+m);
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

	if (generateSet() != 0){
		printf("Error!");
	}

	init();

	draw();

	//Apply the image
	SDL_BlitSurface(surface, NULL, screenSurface, NULL);

	//Update the surface
	SDL_UpdateWindowSurface(window);

		for (int i = 0; i < 20; i++) {
		cr += .01;
		ci += .01;
		if (cr >= 1.0) {
			cr = -1.0;
		}
		if (ci >= 1.0) {
			ci = -1.0;
		}
		generateSet();
		draw();
		SDL_BlitSurface(surface, NULL, screenSurface, NULL);
		SDL_UpdateWindowSurface(window);
	}


	//Wait two seconds
	SDL_Delay(2000);

	//Free resources and close SDL
	close();

	return 0;
}