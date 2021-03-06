#include <switch.h>
#include <sys/stat.h>
#include <cstdio>
#include <cstring>
#include "domino.hpp"
#include "GameStateManager.hpp"
#include <random>
#include <algorithm>
#include <iterator>
#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/errno.h>
#include <unistd.h>
#include <cstdlib>
#include <functional>
#include <array>

#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include "main.h"

#define SCREEN_W 1280
#define SCREEN_H 720

using namespace std;

Domino stack[48] = {
   Domino(Tile(Terrain::wheat, 0),Tile(Terrain::wheat, 0),1)
  ,Domino(Tile(Terrain::wheat, 0),Tile(Terrain::wheat, 0),2)
  ,Domino(Tile(Terrain::forest, 0),Tile(Terrain::forest, 0),3)
  ,Domino(Tile(Terrain::forest, 0),Tile(Terrain::forest, 0),4)
  ,Domino(Tile(Terrain::forest, 0),Tile(Terrain::forest, 0),5)
  ,Domino(Tile(Terrain::forest, 0),Tile(Terrain::forest, 0),6)
  ,Domino(Tile(Terrain::water, 0),Tile(Terrain::water, 0),7)
  ,Domino(Tile(Terrain::water, 0),Tile(Terrain::water, 0),8)
  ,Domino(Tile(Terrain::water, 0),Tile(Terrain::water, 0),9)
  ,Domino(Tile(Terrain::grass, 0),Tile(Terrain::grass, 0),10)
  ,Domino(Tile(Terrain::grass, 0),Tile(Terrain::grass, 0),11)
  ,Domino(Tile(Terrain::swamp, 0),Tile(Terrain::swamp, 0),12)
  ,Domino(Tile(Terrain::wheat, 0),Tile(Terrain::forest, 0),13)
  ,Domino(Tile(Terrain::wheat, 0),Tile(Terrain::water, 0),14)
  ,Domino(Tile(Terrain::wheat, 0),Tile(Terrain::grass, 0),15)
  ,Domino(Tile(Terrain::wheat, 0),Tile(Terrain::swamp, 0),16)
  ,Domino(Tile(Terrain::forest, 0),Tile(Terrain::water, 0),17)
  ,Domino(Tile(Terrain::forest, 0),Tile(Terrain::grass, 0),18)
  ,Domino(Tile(Terrain::wheat, 1),Tile(Terrain::forest, 0),19)
  ,Domino(Tile(Terrain::wheat, 1),Tile(Terrain::water, 0),20)
  ,Domino(Tile(Terrain::wheat, 1),Tile(Terrain::grass, 0),21)
  ,Domino(Tile(Terrain::wheat, 1),Tile(Terrain::swamp, 0),22)
  ,Domino(Tile(Terrain::wheat, 1),Tile(Terrain::mine, 0),23)
  ,Domino(Tile(Terrain::forest, 1),Tile(Terrain::wheat, 0),24)
  ,Domino(Tile(Terrain::forest, 1),Tile(Terrain::wheat, 0),25)
  ,Domino(Tile(Terrain::forest, 1),Tile(Terrain::wheat, 0),26)
  ,Domino(Tile(Terrain::forest, 1),Tile(Terrain::wheat, 0),27)
  ,Domino(Tile(Terrain::forest, 1),Tile(Terrain::water, 0),28)
  ,Domino(Tile(Terrain::forest, 1),Tile(Terrain::grass, 0),29)
  ,Domino(Tile(Terrain::water, 1),Tile(Terrain::wheat, 0),30)
  ,Domino(Tile(Terrain::water, 1),Tile(Terrain::wheat, 0),31)
  ,Domino(Tile(Terrain::water, 1),Tile(Terrain::forest, 0),32)
  ,Domino(Tile(Terrain::water, 1),Tile(Terrain::forest, 0),33)
  ,Domino(Tile(Terrain::water, 1),Tile(Terrain::forest, 0),34)
  ,Domino(Tile(Terrain::water, 1),Tile(Terrain::forest, 0),35)
  ,Domino(Tile(Terrain::wheat, 0),Tile(Terrain::grass, 1),36)
  ,Domino(Tile(Terrain::water, 0),Tile(Terrain::grass, 1),37)
  ,Domino(Tile(Terrain::wheat, 0),Tile(Terrain::swamp, 1),38)
  ,Domino(Tile(Terrain::grass, 0),Tile(Terrain::swamp, 1),39)
  ,Domino(Tile(Terrain::mine, 1),Tile(Terrain::wheat, 0),40)
  ,Domino(Tile(Terrain::wheat, 0),Tile(Terrain::grass, 2),41)
  ,Domino(Tile(Terrain::water, 0),Tile(Terrain::grass, 2),42)
  ,Domino(Tile(Terrain::wheat, 0),Tile(Terrain::swamp, 2),43)
  ,Domino(Tile(Terrain::grass, 0),Tile(Terrain::swamp, 2),44)
  ,Domino(Tile(Terrain::mine, 2),Tile(Terrain::wheat, 0),45)
  ,Domino(Tile(Terrain::swamp, 0),Tile(Terrain::mine, 2),46)
  ,Domino(Tile(Terrain::swamp, 0),Tile(Terrain::mine, 2),47)
  ,Domino(Tile(Terrain::wheat, 0),Tile(Terrain::mine, 3),48)
};

void loadTilePictures(SDL_Renderer* renderer, SDL_Texture* textures[], SDL_Surface* surfaces[]){
  surfaces[0] = IMG_Load("romfs:/resources/images/grassTile0.png");
  surfaces[1] = IMG_Load("romfs:/resources/images/grassTile1.png");
  surfaces[2] = IMG_Load("romfs:/resources/images/grassTile2.png");
  surfaces[3] = IMG_Load("romfs:/resources/images/forestTile0.png");
  surfaces[4] = IMG_Load("romfs:/resources/images/forestTile1.png");
  surfaces[5] = IMG_Load("romfs:/resources/images/wheatTile0.png");
  surfaces[6] = IMG_Load("romfs:/resources/images/wheatTile1.png");
  surfaces[7] = IMG_Load("romfs:/resources/images/waterTile0.png");
  surfaces[8] = IMG_Load("romfs:/resources/images/waterTile1.png");
  surfaces[9] = IMG_Load("romfs:/resources/images/mineTile0.png");
  surfaces[10] = IMG_Load("romfs:/resources/images/mineTile1.png");
  surfaces[11] = IMG_Load("romfs:/resources/images/mineTile2.png");
  surfaces[12] = IMG_Load("romfs:/resources/images/mineTile3.png");
  surfaces[13] = IMG_Load("romfs:/resources/images/swampTile0.png");
  surfaces[14] = IMG_Load("romfs:/resources/images/swampTile1.png");
  surfaces[15] = IMG_Load("romfs:/resources/images/swampTile2.png");
  surfaces[16] = IMG_Load("romfs:/resources/images/cursor.png");
  surfaces[17] = IMG_Load("romfs:/resources/images/startingTile1.png");
  surfaces[18] = IMG_Load("romfs:/resources/images/startingTile2.png");
  surfaces[19] = IMG_Load("romfs:/resources/images/overlay_green.png");
  surfaces[20] = IMG_Load("romfs:/resources/images/overlay_red.png");
  surfaces[21] = IMG_Load("romfs:/resources/images/empty_tile.png");
  textures[0] = SDL_CreateTextureFromSurface(renderer, surfaces[0]);
  textures[1] = SDL_CreateTextureFromSurface(renderer, surfaces[1]);
  textures[2] = SDL_CreateTextureFromSurface(renderer, surfaces[2]);
  textures[3] = SDL_CreateTextureFromSurface(renderer, surfaces[3]);
  textures[4] = SDL_CreateTextureFromSurface(renderer, surfaces[4]);
  textures[5] = SDL_CreateTextureFromSurface(renderer, surfaces[5]);
  textures[6] = SDL_CreateTextureFromSurface(renderer, surfaces[6]);
  textures[7] = SDL_CreateTextureFromSurface(renderer, surfaces[7]);
  textures[8] = SDL_CreateTextureFromSurface(renderer, surfaces[8]);
  textures[9] = SDL_CreateTextureFromSurface(renderer, surfaces[9]);
  textures[10] = SDL_CreateTextureFromSurface(renderer, surfaces[10]);
  textures[11] = SDL_CreateTextureFromSurface(renderer, surfaces[11]);
  textures[12] = SDL_CreateTextureFromSurface(renderer, surfaces[12]);
  textures[13] = SDL_CreateTextureFromSurface(renderer, surfaces[13]);
  textures[14] = SDL_CreateTextureFromSurface(renderer, surfaces[14]);
  textures[15] = SDL_CreateTextureFromSurface(renderer, surfaces[15]);
  textures[16] = SDL_CreateTextureFromSurface(renderer, surfaces[16]);
  textures[17] = SDL_CreateTextureFromSurface(renderer, surfaces[17]);
  textures[18] = SDL_CreateTextureFromSurface(renderer, surfaces[18]);
  textures[19] = SDL_CreateTextureFromSurface(renderer, surfaces[19]);
  textures[20] = SDL_CreateTextureFromSurface(renderer, surfaces[20]);
  textures[21] = SDL_CreateTextureFromSurface(renderer, surfaces[21]);

  for (int i = 0; i < 21; i++) {
	  SDL_FreeSurface(surfaces[i]);
  }
}

void updateCursorLocation(SDL_Texture* textures[25], SDL_Renderer* renderer, SDL_Texture* bg_texture, Domino dominoSelection[4], SDL_Rect cursorDestination, GameStateManager &gameStateManager, int player)
{
	SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer, bg_texture, nullptr, nullptr);
	dominoSelection[0].printDominoForSelection(renderer, 1, textures);
	dominoSelection[1].printDominoForSelection(renderer, 2, textures);
	dominoSelection[2].printDominoForSelection(renderer, 3, textures);
	dominoSelection[3].printDominoForSelection(renderer, 4, textures);
	SDL_RenderCopy(renderer, textures[16], nullptr, &cursorDestination);
	gameStateManager.get_board_for_player(player).displayBoard(renderer, textures);
	SDL_RenderPresent(renderer);
}

void render_selected_tile(Domino domino, SDL_Renderer* renderer, Orientation orientation, SDL_Texture* textures[25])
{
	if (orientation == Orientation::horizontal) {
		domino.getTile1().printTile(renderer, 50, 50, textures);
		domino.getTile2().printTile(renderer, 150, 50, textures);
	}
	else if (orientation == Orientation::vertical) {
		domino.getTile1().printTile(renderer, 75, 50, textures);
		domino.getTile2().printTile(renderer, 75, 150, textures);
	}
	else if (orientation == Orientation::horizontal_reverse) {
		domino.getTile1().printTile(renderer, 150, 50, textures);
		domino.getTile2().printTile(renderer, 50, 50, textures);
	}
	else if (orientation == Orientation::vertical_flipped) {
		domino.getTile1().printTile(renderer, 75, 150, textures);
		domino.getTile2().printTile(renderer, 75, 50, textures);
	}
}

int pickTile(SDL_Renderer * renderer, SDL_Texture * bg_texture, int  tileNumbers[48], int tileCounter, SDL_Texture * textures[25], GameStateManager &gameStateManager, int player, int firstPlayersTile)
{
	SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer, bg_texture, NULL, NULL);
	std::array<int, 4> sortedDominoNumbers = {
		tileNumbers[tileCounter]
		,tileNumbers[tileCounter + 1]
		,tileNumbers[tileCounter + 2]
		,tileNumbers[tileCounter + 3]
	};
	std::sort(sortedDominoNumbers.begin(), sortedDominoNumbers.end());
	Domino dominoSelection[4] = {
		stack[sortedDominoNumbers[0]]
		,stack[sortedDominoNumbers[1]]
		,stack[sortedDominoNumbers[2]]
		,stack[sortedDominoNumbers[3]]
	};
	dominoSelection[0].printDominoForSelection(renderer, 1, textures);
	dominoSelection[1].printDominoForSelection(renderer, 2, textures);
	dominoSelection[2].printDominoForSelection(renderer, 3, textures);
	dominoSelection[3].printDominoForSelection(renderer, 4, textures);
	//display cursor at tile 1
	SDL_Rect cursorDestination;
	if (firstPlayersTile == 1) {
		cursorDestination.y = 250;
	}
	else {
		cursorDestination.y = 150;
	}
	cursorDestination.x = 900;
	cursorDestination.w = 80;
	cursorDestination.h = 60;
	SDL_RenderCopy(renderer, textures[16], nullptr, &cursorDestination);
	gameStateManager.get_board_for_player(player).displayBoard(renderer, textures);
	SDL_RenderPresent(renderer);
	

	int dominoNumberSelected = 1;

	if (firstPlayersTile == 1) {
		dominoNumberSelected = 2;
	}
	else {
		dominoNumberSelected = 1;
	}

	u32 kdown = 0x00000000;
	//wait until user makes decision on domino to pick
	while (true)
	{
		hidScanInput();
		kdown = hidKeysDown(CONTROLLER_P1_AUTO);
		if (kdown & KEY_UP && (dominoNumberSelected > 1) ) {
			if (firstPlayersTile != dominoNumberSelected - 1 && dominoNumberSelected > 1) {
				dominoNumberSelected -= 1;
				cursorDestination.y -= 100;
			}
			else if (dominoNumberSelected - 1 == firstPlayersTile && dominoNumberSelected > 2) {
				dominoNumberSelected -= 2;
				cursorDestination.y -= 200;
			}
			updateCursorLocation(textures, renderer, bg_texture, dominoSelection, cursorDestination, gameStateManager, player);
		}
		if (kdown & KEY_DOWN && (dominoNumberSelected < 4)) {
			if (firstPlayersTile != dominoNumberSelected + 1 && dominoNumberSelected < 4) {
				dominoNumberSelected += 1;
				cursorDestination.y += 100;
			}
			else if (dominoNumberSelected + 1 == firstPlayersTile && dominoNumberSelected < 3) {
				dominoNumberSelected += 2;
				cursorDestination.y += 200;
			}
			updateCursorLocation(textures, renderer, bg_texture, dominoSelection, cursorDestination, gameStateManager, player);
		}
		if (kdown & KEY_A) {
			gameStateManager.set_domino_for_player(dominoSelection[dominoNumberSelected - 1], player);
			break;
		}
	}
	return dominoNumberSelected;
}

void placeTile(SDL_Renderer * renderer, SDL_Texture * bg_texture, GameStateManager &gameStateManager, SDL_Texture * textures[25], int player, int tileCounter,int  tileNumbers[48])
{
	u32 kdown = 0x00000000;
	while (true) {
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, bg_texture, nullptr, nullptr);

		//display next tiles on the right side
		if (tileCounter <= 44) {
			std::array<int, 4> sortedDominoNumbers = {
				tileNumbers[tileCounter]
				,tileNumbers[tileCounter + 1]
				,tileNumbers[tileCounter + 2]
				,tileNumbers[tileCounter + 3]
			};
			std::sort(sortedDominoNumbers.begin(), sortedDominoNumbers.end());
			Domino dominoSelection[4] = {
				stack[sortedDominoNumbers[0]]
				,stack[sortedDominoNumbers[1]]
				,stack[sortedDominoNumbers[2]]
				,stack[sortedDominoNumbers[3]]
			};
			dominoSelection[0].printDominoForSelection(renderer, 1, textures);
			dominoSelection[1].printDominoForSelection(renderer, 2, textures);
			dominoSelection[2].printDominoForSelection(renderer, 3, textures);
			dominoSelection[3].printDominoForSelection(renderer, 4, textures);
		}


		render_selected_tile(gameStateManager.get_domino_for_player(player), renderer, gameStateManager.get_orientation(), textures);
		gameStateManager.get_board_for_player(player).displayBoard(renderer, textures);
		gameStateManager.display_overlay(renderer, textures, player);

		SDL_RenderPresent(renderer);
		hidScanInput();
		kdown = hidKeysDown(CONTROLLER_P1_AUTO);
		if (kdown & KEY_LEFT && ((gameStateManager.getCoordinates1().getXCoordinate() >= 1) && (gameStateManager.getCoordinates2().getXCoordinate() >= 1))) {
			gameStateManager.move_left();
		}
		if (kdown & KEY_RIGHT && ((gameStateManager.getCoordinates1().getXCoordinate() <= 3) && (gameStateManager.getCoordinates2().getXCoordinate() <= 3))) {
			gameStateManager.move_right();
		}
		if (kdown & KEY_UP && ((gameStateManager.getCoordinates1().getYCoordinate() >= 1) && (gameStateManager.getCoordinates2().getYCoordinate() >= 1))) {
			gameStateManager.move_up();
		}
		if (kdown & KEY_DOWN && ((gameStateManager.getCoordinates1().getYCoordinate() <= 3) && (gameStateManager.getCoordinates2().getYCoordinate() <= 3))) {
			gameStateManager.move_down();
		}
		if (kdown & KEY_A && gameStateManager.get_board_for_player(player).can_place_domino(gameStateManager.get_domino_for_player(player), gameStateManager.getCoordinates1(), gameStateManager.getCoordinates2()))
		{
			gameStateManager.place_domino(gameStateManager.get_domino_for_player(player), gameStateManager.getCoordinates1(), gameStateManager.getCoordinates2());
			gameStateManager.clear_selections();
			break;
		}
		if (kdown & KEY_X)
		{
			gameStateManager.update_orientation();
		}
		if (kdown & KEY_B) {
			gameStateManager.clear_selections();
			break;
		}

	}
}

int main(int argc, char** argv)
{
	SDL_Init(SDL_INIT_EVERYTHING);
	IMG_Init(IMG_INIT_PNG);
	TTF_Init();
	romfsInit();
	consoleInit(nullptr); //Init the console
	bool decision_made = false;

  SDL_Texture* textures[25];
  SDL_Surface* surfaces[25];

  TTF_Font* font;
  TTF_Font* font48;

  font = TTF_OpenFont("romfs:/resources/fonts/MiniSet2.ttf", 24);

  u32 kdown = 0x00000000;

  // Create an SDL window & renderer
  SDL_Window* window = SDL_CreateWindow("Main-Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 0, 0, SDL_WINDOW_FULLSCREEN_DESKTOP);
  SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);

  SDL_Surface* bg_surface = IMG_Load("romfs:/resources/images/background.png");			// Read image as surface
  SDL_Texture* bg_texture = SDL_CreateTextureFromSurface(renderer, bg_surface);	// Create texture from surface
  SDL_RenderClear(renderer);
  SDL_RenderCopy(renderer, bg_texture, nullptr, nullptr);
  SDL_RenderPresent(renderer);

  //determine where to start in the domino stack
  int tileCounter = 0;
  int tileNumbers[48];

  for(int i = 0; i<48;i++){
    tileNumbers[i] = i;
  }

  //shuffle up the tile numbers
  srand(time(nullptr));
  random_shuffle(begin(tileNumbers), end(tileNumbers));

  //create both boards
  GameStateManager gameStateManager = GameStateManager();
  loadTilePictures(renderer, textures, surfaces);

  int defaultTileChosen = -1;

  //pick first tiles
  int firstTilePicked = pickTile(renderer, bg_texture, tileNumbers, tileCounter, textures, gameStateManager, gameStateManager.order.first_player, defaultTileChosen);
  gameStateManager.update_current_player();
  int secondTilePicked = pickTile(renderer, bg_texture, tileNumbers, tileCounter, textures, gameStateManager, gameStateManager.order.second_player, firstTilePicked);
  gameStateManager.update_order(firstTilePicked, secondTilePicked);
  tileCounter += 4;

  while(appletMainLoop())
  {
    hidScanInput();
    kdown = hidKeysDown(CONTROLLER_P1_AUTO);

    if(kdown & KEY_PLUS)
      break;



    while(tileCounter <= 48){

      //load board, and move new domino around it
	  placeTile(renderer, bg_texture, gameStateManager, textures, gameStateManager.order.first_player, tileCounter, tileNumbers);
	  if (tileCounter <= 44) {
		  firstTilePicked = pickTile(renderer, bg_texture, tileNumbers, tileCounter, textures, gameStateManager, gameStateManager.order.first_player, defaultTileChosen);
	  }
	  gameStateManager.update_current_player();
	  
	  placeTile(renderer, bg_texture, gameStateManager, textures, gameStateManager.order.second_player, tileCounter, tileNumbers);
	  if (tileCounter <= 44) {
		  secondTilePicked = pickTile(renderer, bg_texture, tileNumbers, tileCounter, textures, gameStateManager, gameStateManager.order.second_player, firstTilePicked);
	  }
	  gameStateManager.update_order(firstTilePicked, secondTilePicked);

      //start the next round
      tileCounter+=4;
    }
	
	SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer, bg_texture, nullptr, nullptr);
	int player1Score = gameStateManager.getBoard1().calculate_score();
	int player2Score = gameStateManager.getBoard2().calculate_score();
	std::string player_1_score_text = "Player 1 Score: " + std::to_string(player1Score);
	std::string player_2_score_text = "Player 2 Score: " + std::to_string(player2Score);

	
	SDL_Color textColor = { 255, 255, 255, 0 };

	SDL_Surface* textSurface = TTF_RenderText_Solid(font, player_1_score_text.c_str(), textColor);
	SDL_Texture* text = SDL_CreateTextureFromSurface(renderer, textSurface);
	int text_width = textSurface->w;
	int text_height = textSurface->h;
	SDL_FreeSurface(textSurface);
	SDL_Rect renderQuad = { 20, 720 - 30, text_width, text_height };
	SDL_RenderCopy(renderer, text, NULL, &renderQuad);

	SDL_Surface* textSurface2 = TTF_RenderText_Solid(font, player_2_score_text.c_str(), textColor);
	SDL_Texture* text2 = SDL_CreateTextureFromSurface(renderer, textSurface2);
	int text_width2 = textSurface2->w;
	int text_height2 = textSurface2->h;
	SDL_FreeSurface(textSurface2);
	SDL_Rect renderQuad2 = { 1000, 720 - 30, text_width2, text_height2 };
	SDL_RenderCopy(renderer, text2, NULL, &renderQuad2);

	

	SDL_DestroyTexture(text);
	SDL_DestroyTexture(text2);
	SDL_RenderPresent(renderer);
	
  }
  SDL_Quit();				// SDL cleanup
	return EXIT_SUCCESS;
}