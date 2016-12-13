#include "SDL.h"
#include "SDL_ttf.h"
#include <iostream>
#include <vector>
#include "GameCore.h"
#include "Sprite.h"

using namespace DungeonGame;

int main(int argc, char *argv[])
{
	SDL_Init(SDL_INIT_VIDEO);
	TTF_Init();
	SDL_Window *pWindow = SDL_CreateWindow("Deep Ocean Cleanup!", 100, 100, (int)WINDOW_WIDTH, (int)WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
	SDL_Texture* g_pBackgroundTexture = nullptr;
	SDL_Renderer *pRenderer = SDL_CreateRenderer(pWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	WorldState worldState = {};
	PlayerState playerState = {};

	InitializeGame(pRenderer, worldState, playerState);
		
	int frameStartTicks, frameEndTicks = 0;
	int deltaTick = 0;

	while (!playerState.m_bHasFinishedGame)
	{
		if (deltaTick < 1)
		{
			frameStartTicks = SDL_GetTicks();
			SDL_Delay(1);
			frameEndTicks = SDL_GetTicks();
			deltaTick = frameEndTicks - frameStartTicks;
		}

		frameStartTicks = SDL_GetTicks();

		GetInput(worldState, playerState);
		
		const float deltaSeconds = (float)deltaTick / 1000.0f;
		UpdateGame(deltaSeconds, worldState, playerState);

		RenderGame(pRenderer, worldState, playerState);
		
		frameEndTicks = SDL_GetTicks();
		deltaTick = frameEndTicks - frameStartTicks;
	}

	CleanupGame();

	SDL_DestroyRenderer(pRenderer);
	SDL_DestroyWindow(pWindow);
	TTF_Quit();
	SDL_Quit();

	return 0;
}