#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "include/board.h"
#include "include/bitboard.h"
#include "include/piece.h"

#define WIDTH 800
#define HEIGHT 800
#define TITLE "Geany Chess Engine 2"

const int PIECE_WIDTH = WIDTH / 8;
const int PIECE_HEIGHT = HEIGHT / 8;

int WhiteSquareColor[4] = {240, 224, 161, 255};
int BlackSquareColor[4] = {115, 101, 70, 255};

SDL_Texture* PieceTextures[2][6];

const char* PieceTexturePaths[2][6] = {
									{"src/img/whitepawn.png", "src/img/whiteknight.png", "src/img/whitebishop.png", "src/img/whiterook.png", "src/img/whitequeen.png", "src/img/whiteking.png"},
									{"src/img/blackpawn.png", "src/img/blackknight.png", "src/img/blackbishop.png", "src/img/blackrook.png", "src/img/blackqueen.png", "src/img/blackking.png"}
									};

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

void DrawBoard(SDL_Renderer* renderer){
	SDL_Rect rect;
	rect.w = WIDTH / 8;
	rect.h = HEIGHT / 8;

	for (int i=0; i<8; i++){
		rect.y = rect.h * i;
		for (int j=0; j<8; j++){
			rect.x = rect.w * j;

			if ((i+j) % 2){ // Black
				SDL_SetRenderDrawColor(renderer, BlackSquareColor[0], BlackSquareColor[1], BlackSquareColor[2], 255);
			}
			else{
				SDL_SetRenderDrawColor(renderer, WhiteSquareColor[0], WhiteSquareColor[1], WhiteSquareColor[2], 255);
			}

			SDL_RenderFillRect(renderer, &rect);
		}
	}
}

void DrawPieces(SDL_Renderer* renderer, Board* b){
	SDL_Rect rect;
	rect.w = PIECE_WIDTH;
	rect.h = PIECE_HEIGHT;

	char p;

	for (int i=0; i<8; i++){
		for (int j=0; j<8; j++){
			p = b->pieces[i*8+j];
			if (p != '_'){
				rect.x = PIECE_WIDTH*j;
				rect.y = PIECE_HEIGHT*i;
				SDL_RenderCopy(renderer, PieceTextures[PieceColor(p)][PieceType(p)], NULL, &rect);
			}
		}
	}
}

int ScreenPosToSquare(int x, int y){
	int rank = ((float)y / (float)HEIGHT) * 8;
	int file = ((float)x / (float)WIDTH) * 8;

	return rank*8 + file;
}

void DestroyTextures(){
	for (int i=0; i<6; i++){
		SDL_DestroyTexture(PieceTextures[white][i]);
		SDL_DestroyTexture(PieceTextures[black][i]);
	}
}

void PrecomputeMoves(){
	PrecomputePawnTables(PrecomputedPawnAttacks, PrecomputedPawnMoves);
	PrecomputeKnightTables(PrecomputedKnightMoves);
	PrecomputeKingTables(PrecomputedKingMoves);
}

int main(int argc, char* argv[]){
	InitSDL(); // Init SDL

	// Make window and renderer
	SDL_Window* Window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0);
	SDL_Renderer* Renderer = SDL_CreateRenderer(Window, -1, 0);

	// Load Textures
	for (int i=0; i<6; i++){
		PieceTextures[white][i] = IMG_LoadTexture(Renderer, PieceTexturePaths[white][i]);
		PieceTextures[black][i] = IMG_LoadTexture(Renderer, PieceTexturePaths[black][i]);
	}

	
	// Init board
	Board board;
	ParseFEN(&board, StartFEN);
	GenerateBitboards(&board);

	// Loop Variables
	int mouse_start_x, mouse_start_y;
	int mouse_end_x, mouse_end_y;
	int mouse_held = 0;

	int start = -1;
	int end = -1;

	int running = 1;
	SDL_Event e;
	while (running){
		while (SDL_PollEvent(&e)){
			if (e.type == SDL_QUIT){
				running = 0;
			}
		}

		if (SDL_GetMouseState(NULL, NULL)&SDL_BUTTON(1)){
			if (mouse_held == 0) SDL_GetMouseState(&mouse_start_x, &mouse_start_y);
			mouse_held = 1;
		}
		else {
			if (mouse_held){
				mouse_held = 0;

				SDL_GetMouseState(&mouse_end_x, &mouse_end_y);
				start = ScreenPosToSquare(mouse_start_x, mouse_start_y);
				end = ScreenPosToSquare(mouse_end_x, mouse_end_y);
			}
		}


		if (start != -1 && end != -1 && PieceColor(board.pieces[start]) == board.color && start != end){
			if (board.pieces[end] != -1 && PieceColor(board.pieces[start]) == PieceColor(board.pieces[end])){}
			else {
				board.pieces[end] = board.pieces[start];
				board.pieces[start] = -1;
	
				// Swap color
				board.color ^= 1;
				
				start = -1;
				end = -1;
			}
		}
		DrawBoard(Renderer);
		DrawPieces(Renderer, &board);
		SDL_RenderPresent(Renderer);
	}


	// Destroy Textures
	DestroyTextures();

	// Destroy window and renderer
	SDL_DestroyWindow(Window);
	SDL_DestroyRenderer(Renderer);

	QuitSDL(); // Quit SDL

	return EXIT_SUCCESS;
}