#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#define WIDTH 800
#define HEIGHT 800
#define TITLE "Geany Chess Engine 2"

void InitSDL(){
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0){
		printf("Could not init sdl2\n");
		exit(EXIT_FAILURE);
	}
	if (IMG_Init(IMG_INIT_PNG) < 0){
		printf("Could not init sdl2 image\n");
		exit (EXIT_FAILURE);
	}
}

void QuitSDL(){
	SDL_Quit();
	IMG_Quit();
}

int main(int argc, char* argv[]){
	InitSDL(); // Init SDL

	// Make window and renderer
	SDL_Window* Window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0);
	SDL_Renderer* Renderer = SDL_CreateRenderer(Window, -1, 0);


	int running = 1;
	SDL_Event e;
	while (running){
		while (SDL_PollEvent(&e)){
			if (e.type == SDL_QUIT){
				running = 0;
			}
		}
	}



	// Destroy window and renderer
	SDL_DestroyWindow(Window);
	SDL_DestroyRenderer(Renderer);

	QuitSDL(); // Quit SDL

	return EXIT_SUCCESS;
}