#include "GameCore.h"
#include "Sprite.h"
#include "Enemy.h"
#include "Hero.h"
#include "Foe.h"
#include "RoomRenderer.h"
#include "Item.h"
#include "Bullet.h"
#include "LockedDoor.h"
#include "HUD.h"
#include <stdio.h>
#include <cstring>
#include <string>
#include "SDL.h"
#include "SDL_ttf.h"

namespace DungeonGame
{
	TTF_Font* g_pFont = nullptr;

	//Main Game Loop Sprite List
	std::vector<Sprite*> g_spriteList;
	//Bullet Count for attacking (not needed)
	const unsigned int BULLET_COUNT = 3;
	//Camera Position for proper player following
	Vector2d g_cameraPosition;
	//Doors needed to win
	const unsigned int WIN_DOOR_NUM = 20;

	//Function to create Foes
	FoeData CreateFoe(bool alive, int hp, Vector2d pos) {
		FoeData foe = {};
		foe.bAlive = alive;
		foe.hp = hp;
		foe.Position = pos;

		return foe;
	}

	//Function to create Items
	ItemData CreateItem(ItemType type, bool alive, int hp, Vector2d pos) {
		ItemData item = {};
		item.bAlive = alive;
		item.Type = type;
		item.Position = pos;
		item.hp = hp;

		return item;
	}

	//Function to create Locked Doors
	LockedDoorData CreateDoor(bool locked, Vector2d pos) {
		LockedDoorData lockedDoor = {};
		lockedDoor.bLocked = locked;
		lockedDoor.position = pos;
		return lockedDoor;
	}

	//Decrement the hp of Items hit by bullets.
	void ItemData::OnHitByBullet(BulletData* pBullet) {
		hp--;
		if (hp <= 0) {
			bAlive = false;
		}
	}

	//If the player has collided with a locked door
	void LockedDoorData::OnCollidedWithPlayer(PlayerState& playerState) {
	
		bool bHasKey = false;
		//If the player has a key, remove the key
		for (unsigned int i = 0; i < playerState.m_Inventory.size(); i++) {
			if (playerState.m_Inventory[i] == Item_Shield) {
				
				playerState.m_Inventory.erase(playerState.m_Inventory.begin() + i);
				bHasKey = true;
				break;
			}
		}

		//if key exists, unlock door
		if (bHasKey) {
			bLocked = false;
			
			playerState.m_doorsUnlocked++;
			
			//Check win conditions
			if (playerState.m_doorsUnlocked == playerState.m_doorsNeededToWin) {
				playerState.m_bHasFinishedGame = true;
			}
		}
	}

	//Initialize the bullets for the player (object pooling?)
	void PlayerState::Initialize()
	{
		for (unsigned int i = 0; i < BULLET_COUNT; i++) {
			BulletData bullet = {};
			bullet.bAlive = false;
			m_Bullets.push_back(bullet);
		}

		//Set doors to win condition num
		m_doorsNeededToWin = WIN_DOOR_NUM;

		Reset();
	}

	//Reset playerstate values to defaults
	void PlayerState::Reset()
	{
		m_CurrentPosition = Vector2d(3.0f * TILE_DIAMETER, 2.0f * TILE_DIAMETER);
		m_bAlive = true;
		m_Hp = 2;

		m_bWantsToShoot = false;
		m_bHasFinishedGame = false;
		m_ShotCooldownSeconds = 0.0f;

		for (unsigned int i = 0; i <m_Bullets.size(); i++) {
			BulletData& bulletData = m_Bullets[i];
			bulletData.bAlive = false;
		}
	}
	
	//If an item collides with the player
	bool PlayerState::ItemCollidesWithPlayer(ItemData* pItem) {
		
		Vector2d itemToPlayer = pItem->Position - m_CurrentPosition;
		float distance = itemToPlayer.GetLength();

		return distance < TILE_DIAMETER * 0.5f;
	}

	//If a foe collides with the player
	bool PlayerState::FoeCollidesWithPlayer(FoeData* pFoe) {
		Vector2d itemToPlayer = pFoe->Position - m_CurrentPosition;
		float distance = itemToPlayer.GetLength();

		return distance < TILE_DIAMETER * 0.5f;
	}

	//Initialization of WorldState
	void WorldState::Initialize()
	{
		//The map and how wide it is (in tiles)
		m_TilesPerRow = 30;
		m_Tiles =
		{
			0,0,2,2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,2,1,1,1,1,2,0,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,0,
			0,2,1,1,1,1,2,0,0,2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,1,2,0,
			0,2,1,1,1,1,2,0,0,2,1,1,1,1,1,1,1,1,1,1,1,1,2,2,2,1,1,1,2,0,
			0,2,1,1,1,1,2,0,0,2,1,1,1,1,1,1,1,1,1,1,1,1,2,0,0,2,1,1,2,0,
			0,0,2,1,1,2,0,0,0,2,1,1,1,1,2,2,2,2,2,2,2,2,2,0,0,2,2,1,2,0,
			0,0,2,1,1,2,0,0,0,2,1,1,1,1,1,1,1,1,1,1,1,1,2,0,0,2,1,1,2,0,
			0,0,2,1,1,2,0,0,0,2,1,1,1,1,1,1,1,1,1,1,1,1,2,0,0,2,2,1,2,0,
			0,0,2,1,1,2,0,0,0,2,1,1,1,1,1,1,1,1,1,1,1,1,2,0,0,2,1,1,2,0,
			0,0,2,1,1,2,0,0,0,2,2,2,2,2,2,2,2,2,2,1,1,1,2,0,0,2,1,1,2,0,
			0,2,1,1,1,1,2,0,2,1,1,1,1,1,1,1,1,1,1,1,1,1,2,0,0,0,2,1,2,0,
			0,2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,0,0,0,2,1,2,0,
			0,2,1,1,1,1,2,0,2,1,1,1,1,1,1,1,1,1,1,1,1,1,2,0,0,0,2,1,2,0,
			0,0,2,2,2,2,0,0,0,2,2,2,2,2,2,2,2,2,2,2,2,2,0,0,0,0,2,1,2,0,
		};

		//LockedDoor Instantiation block
		{
			m_LockedDoors.push_back(CreateDoor(true, Vector2d(5.0f * TILE_DIAMETER, 3.0f * TILE_DIAMETER)));
			m_LockedDoors.push_back(CreateDoor(true, Vector2d(2.5f * TILE_DIAMETER, 4.5f * TILE_DIAMETER)));
			m_LockedDoors.push_back(CreateDoor(true, Vector2d(3.5f * TILE_DIAMETER, 8.0f * TILE_DIAMETER)));
			m_LockedDoors.push_back(CreateDoor(true, Vector2d(4.5f * TILE_DIAMETER, 8.0f * TILE_DIAMETER)));
			m_LockedDoors.push_back(CreateDoor(true, Vector2d(7.0f * TILE_DIAMETER, 11.5f * TILE_DIAMETER)));
			m_LockedDoors.push_back(CreateDoor(true, Vector2d(12.0f * TILE_DIAMETER, 10.5f * TILE_DIAMETER)));
			m_LockedDoors.push_back(CreateDoor(true, Vector2d(16.0f * TILE_DIAMETER, 12.5f * TILE_DIAMETER)));
			m_LockedDoors.push_back(CreateDoor(true, Vector2d(20.0f * TILE_DIAMETER, 10.5f * TILE_DIAMETER)));
			m_LockedDoors.push_back(CreateDoor(true, Vector2d(12.0f * TILE_DIAMETER, 6.5f * TILE_DIAMETER)));
			m_LockedDoors.push_back(CreateDoor(true, Vector2d(16.0f * TILE_DIAMETER, 6.5f * TILE_DIAMETER)));
			m_LockedDoors.push_back(CreateDoor(true, Vector2d(20.0f * TILE_DIAMETER, 6.5f * TILE_DIAMETER)));
			m_LockedDoors.push_back(CreateDoor(true, Vector2d(12.0f * TILE_DIAMETER, 4.5f * TILE_DIAMETER)));
			m_LockedDoors.push_back(CreateDoor(true, Vector2d(16.0f * TILE_DIAMETER, 4.5f * TILE_DIAMETER)));
			m_LockedDoors.push_back(CreateDoor(true, Vector2d(20.0f * TILE_DIAMETER, 4.5f * TILE_DIAMETER)));
			m_LockedDoors.push_back(CreateDoor(true, Vector2d(12.0f * TILE_DIAMETER, 2.5f * TILE_DIAMETER)));
			m_LockedDoors.push_back(CreateDoor(true, Vector2d(16.0f * TILE_DIAMETER, 2.5f * TILE_DIAMETER)));
			m_LockedDoors.push_back(CreateDoor(true, Vector2d(20.0f * TILE_DIAMETER, 2.5f * TILE_DIAMETER)));
			m_LockedDoors.push_back(CreateDoor(true, Vector2d(27.5f * TILE_DIAMETER, 2.5f * TILE_DIAMETER)));
			m_LockedDoors.push_back(CreateDoor(true, Vector2d(26.5f * TILE_DIAMETER, 8.5f * TILE_DIAMETER)));
			m_LockedDoors.push_back(CreateDoor(true, Vector2d(27.5f * TILE_DIAMETER, 13.5f * TILE_DIAMETER)));
		}

		//Run reset code to spawn all pertinent objects
		Reset();
	}

	//Reset various variables in the WorldState
	void WorldState::Reset()
	{
		//Reset the amount of player lives
		m_PlayersLives = 3;

		//Clear the vectors (lists) or else they won't reset
		m_Items.clear();
		m_Foes.clear();

		//Item Instantiation block
		{
			m_Items.push_back(CreateItem(Item_Shield, true, 3, Vector2d(3.0f * TILE_DIAMETER, 3.0f * TILE_DIAMETER)));
			m_Items.push_back(CreateItem(Item_Shield, true, 3, Vector2d(5.0f * TILE_DIAMETER, 2.0f * TILE_DIAMETER)));
			m_Items.push_back(CreateItem(Item_Shield, true, 3, Vector2d(4.0f * TILE_DIAMETER, 5.0f * TILE_DIAMETER)));
			m_Items.push_back(CreateItem(Item_Shield, true, 3, Vector2d(4.0f * TILE_DIAMETER, 6.5f * TILE_DIAMETER)));
			m_Items.push_back(CreateItem(Item_Shield, true, 3, Vector2d(4.0f * TILE_DIAMETER, 12.5f * TILE_DIAMETER)));
			m_Items.push_back(CreateItem(Item_Shield, true, 3, Vector2d(9.5f * TILE_DIAMETER, 12.5f * TILE_DIAMETER)));
			m_Items.push_back(CreateItem(Item_Shield, true, 3, Vector2d(13.0f * TILE_DIAMETER, 10.5f * TILE_DIAMETER)));
			m_Items.push_back(CreateItem(Item_Shield, true, 3, Vector2d(21.0f * TILE_DIAMETER, 12.5f * TILE_DIAMETER)));
			m_Items.push_back(CreateItem(Item_Shield, true, 3, Vector2d(21.5f * TILE_DIAMETER, 6.5f * TILE_DIAMETER)));
			m_Items.push_back(CreateItem(Item_Shield, true, 3, Vector2d(14.5f * TILE_DIAMETER, 8.0f * TILE_DIAMETER)));
			m_Items.push_back(CreateItem(Item_Shield, true, 3, Vector2d(12.5f * TILE_DIAMETER, 7.5f * TILE_DIAMETER)));
			m_Items.push_back(CreateItem(Item_Shield, true, 3, Vector2d(13.5f * TILE_DIAMETER, 4.5f * TILE_DIAMETER)));
			m_Items.push_back(CreateItem(Item_Shield, true, 3, Vector2d(15.0f * TILE_DIAMETER, 4.5f * TILE_DIAMETER)));
			m_Items.push_back(CreateItem(Item_Shield, true, 3, Vector2d(18.5f * TILE_DIAMETER, 2.5f * TILE_DIAMETER)));
			m_Items.push_back(CreateItem(Item_Shield, true, 3, Vector2d(16.0f * TILE_DIAMETER, 3.5f * TILE_DIAMETER)));
			m_Items.push_back(CreateItem(Item_Shield, true, 3, Vector2d(19.0f * TILE_DIAMETER, 3.5f * TILE_DIAMETER)));
			m_Items.push_back(CreateItem(Item_Shield, true, 3, Vector2d(21.5f * TILE_DIAMETER, 3.5f * TILE_DIAMETER)));
			m_Items.push_back(CreateItem(Item_Shield, true, 3, Vector2d(27.0f * TILE_DIAMETER, 4.5f * TILE_DIAMETER)));
			m_Items.push_back(CreateItem(Item_Shield, true, 3, Vector2d(26.5f * TILE_DIAMETER, 9.5f * TILE_DIAMETER)));
			m_Items.push_back(CreateItem(Item_Shield, true, 3, Vector2d(27.5f * TILE_DIAMETER, 11.0f * TILE_DIAMETER)));
		}

		//Foe Instantiation block
		{
			//m_Foes.push_back(CreateFoe(true, 3, Vector2d(6.0f * TILE_DIAMETER, 3.0f * TILE_DIAMETER)));
			m_Foes.push_back(CreateFoe(true, 3, Vector2d(3.0f * TILE_DIAMETER, 12.0f * TILE_DIAMETER)));
			m_Foes.push_back(CreateFoe(true, 3, Vector2d(4.0f * TILE_DIAMETER, 12.0f * TILE_DIAMETER)));
			m_Foes.push_back(CreateFoe(true, 3, Vector2d(5.0f * TILE_DIAMETER, 12.0f * TILE_DIAMETER)));
			m_Foes.push_back(CreateFoe(true, 3, Vector2d(10.0f * TILE_DIAMETER, 12.0f * TILE_DIAMETER)));
			m_Foes.push_back(CreateFoe(true, 3, Vector2d(3.0f * TILE_DIAMETER, 10.0f * TILE_DIAMETER)));
			m_Foes.push_back(CreateFoe(true, 3, Vector2d(15.0f * TILE_DIAMETER, 9.0f * TILE_DIAMETER)));
			m_Foes.push_back(CreateFoe(true, 3, Vector2d(28.0f * TILE_DIAMETER, 3.0f * TILE_DIAMETER)));
			m_Foes.push_back(CreateFoe(true, 3, Vector2d(28.0f * TILE_DIAMETER, 12.0f * TILE_DIAMETER)));
			m_Foes.push_back(CreateFoe(true, 3, Vector2d(12.0f * TILE_DIAMETER, 4.0f * TILE_DIAMETER)));
			m_Foes.push_back(CreateFoe(true, 3, Vector2d(16.0f * TILE_DIAMETER, 4.0f * TILE_DIAMETER)));
			m_Foes.push_back(CreateFoe(true, 3, Vector2d(20.0f * TILE_DIAMETER, 4.0f * TILE_DIAMETER)));
			m_Foes.push_back(CreateFoe(true, 3, Vector2d(12.0f * TILE_DIAMETER, 6.0f * TILE_DIAMETER)));
			m_Foes.push_back(CreateFoe(true, 3, Vector2d(16.0f * TILE_DIAMETER, 6.0f * TILE_DIAMETER)));
			m_Foes.push_back(CreateFoe(true, 3, Vector2d(20.0f * TILE_DIAMETER, 6.0f * TILE_DIAMETER)));
		}

		//Reset all doors to their "Locked" state
		for (unsigned int i = 0; i < m_LockedDoors.size(); i++) 
		{
			m_LockedDoors[i].bLocked = true;
		}
	}

	//Get the type of tile at the requested position
	unsigned int WorldState::GetTileTypeAtPosition(const Vector2d& inPosition) {

		int col = (int)floor(inPosition.X / TILE_DIAMETER);
		int row = (int)floor(inPosition.Y / TILE_DIAMETER);

		int index = row * m_TilesPerRow + col;
		if (index >= 0 && index < (int)m_Tiles.size()) {
			return m_Tiles[index];
		}
		return 1;
	}

	//Return status of whether bullet has hit items or not
	bool WorldState::BulletCollidesWithItem(BulletData* pBullet) {
		bool bHasCollided = false;

		for (unsigned int i = 0; i < m_Items.size(); i++) {

			ItemData& currItem = m_Items[i];

			if (currItem.bAlive) {
				Vector2d bulletToItem = pBullet->position - currItem.Position;
				float distance = bulletToItem.GetLength();

				if (distance < TILE_DIAMETER * .5f) {
					currItem.OnHitByBullet(pBullet);
					bHasCollided = true;
					break;
				}
			}
		}
		return bHasCollided;
	}

	//Check whether the players proposed position collides with a door
	bool WorldState::PositionCollidesWithLockedDoor(const Vector2d& inPosition, PlayerState& playerState) {
		bool bHasCollided = false;
	
		for (unsigned int i = 0; i < m_LockedDoors.size(); i++) {
			LockedDoorData& currDoor = m_LockedDoors[i];
			if (currDoor.bLocked) {
				Vector2d positionToDoor = inPosition - currDoor.position;
				
				float distance = positionToDoor.GetLength();
				if (distance < TILE_DIAMETER * .6f) {
					currDoor.OnCollidedWithPlayer(playerState);
					bHasCollided = true;
					break;
				}
			}
		}
		return bHasCollided;
	}

	//Start the game
	void InitializeGame(SDL_Renderer* pRenderer, WorldState& worldState, PlayerState& playerState)
	{
		worldState.Initialize();
		playerState.Initialize();

		g_pFont = TTF_OpenFont("courier.ttf", 48);

		g_cameraPosition = Vector2d();

		//Render everything
		{
			//Don't need the background "under the sea"
			/*
			Sprite* pBackgroundSprite = new Sprite;
			pBackgroundSprite->Initialize(Sprite::LoadTexture(pRenderer, "background.bmp", 0, 255, 0));
			pBackgroundSprite->m_Size = Vector2d(WINDOW_WIDTH, WINDOW_HEIGHT);
			g_spriteList.push_back(pBackgroundSprite);
			*/

			RoomRenderer* pRoom = new RoomRenderer;
			pRoom->InitMe(pRenderer);
			g_spriteList.push_back(pRoom);

			for (unsigned int i = 0; i < worldState.m_Items.size(); i++) {
				Item* pItem = new Item;
				pItem->InitMe(pRenderer, &worldState.m_Items[i]);
				g_spriteList.push_back(pItem);
			}

			for (unsigned int i = 0; i < worldState.m_Foes.size(); i++) {
				Foe* pFoe = new Foe;
				pFoe->InitMe(pRenderer, &worldState.m_Foes[i]);
				g_spriteList.push_back(pFoe);
			}

			for (unsigned int i = 0; i < worldState.m_LockedDoors.size(); i++) {
				LockedDoor* pLockedDoor = new LockedDoor;
				pLockedDoor->InitMe(pRenderer, &worldState.m_LockedDoors[i]);
				g_spriteList.push_back(pLockedDoor);
			}

			Hero* pHero = new Hero;
			pHero->InitMe(pRenderer);
			pHero->m_Position.X = 128.0f;
			g_spriteList.push_back(pHero);

			for (unsigned int i = 0; i < playerState.m_Bullets.size(); i++) {

				Bullet* pBullet = new Bullet;
				pBullet->InitMe(pRenderer, &playerState.m_Bullets[i]);
				g_spriteList.push_back(pBullet);
			}

			HUD* pHUD = new HUD;
			pHUD->InitMe(pRenderer);
			g_spriteList.push_back(pHUD);
		}
	}

	//Get Input
	void GetInput(const WorldState& worldState, PlayerState& playerState)
	{
		SDL_Event e = {};
		while (SDL_PollEvent(&e) != 0)
		{
			if (e.type == SDL_QUIT)
			{
				playerState.m_bHasFinishedGame = true;
			}
			else if (e.type == SDL_KEYDOWN)
			{
				auto keyCode = e.key.keysym.sym;
				switch (keyCode)
				{
					case SDLK_UP:
						playerState.m_DesiredDirection.Y = -1.0f;
						break;
					case SDLK_DOWN:
						playerState.m_DesiredDirection.Y = 1.0f;
						break;
					case SDLK_LEFT:
						playerState.m_DesiredDirection.X = -1.0f;
						break;
					case SDLK_RIGHT:
						playerState.m_DesiredDirection.X = 1.0f;
						break;
					case SDLK_ESCAPE:
						playerState.m_bHasFinishedGame = true;
						break;
				}
			}
			
			else if (e.type == SDL_KEYUP) {
				auto keyCode = e.key.keysym.sym;
				switch (keyCode)
				{
					case SDLK_UP:
					case SDLK_DOWN:
						playerState.m_DesiredDirection.Y = 0.0f;
						break;
					case SDLK_LEFT:
					case SDLK_RIGHT:
						playerState.m_DesiredDirection.X = 0.0f;
						break;
					case SDLK_ESCAPE:
						playerState.m_bHasFinishedGame = true;
						break;
				}	
			}
		}
	}

	//Update the game sprites and camera
	void UpdateGame(float deltaSeconds, WorldState& worldState, PlayerState& playerState)
	{
		for (unsigned int i = 0; i < g_spriteList.size(); i++) {
			g_spriteList[i]->Update(deltaSeconds, worldState, playerState);
		}

		g_cameraPosition = Vector2d(WINDOW_WIDTH * 0.50f, WINDOW_HEIGHT * 0.50f) - playerState.m_CurrentPosition;
	}

	//Render everything on the spritelist
	void RenderGame(SDL_Renderer* pRenderer, const WorldState& worldState, const PlayerState& playerState)
	{
		std::string missionToSDLText = "Use Shields->    to clear toxic waste->";
		std::string lifeToSDLText = "Lives:";
		std::string inventoryToSDLText = "Inventory:";

		SDL_RenderClear(pRenderer);
		
		for (unsigned int i = 0; i < g_spriteList.size(); i++) {
			g_spriteList[i]->Render(pRenderer, worldState, playerState, g_cameraPosition);
		}

		//Text Rendering
		{
			SDL_Color textColor = { 128,255,128, 0 };

			//Mission text
			SDL_Surface* mTextSurface = TTF_RenderText_Blended_Wrapped(g_pFont, missionToSDLText.c_str(), textColor, 1200);
			SDL_Texture* mTextTexture = SDL_CreateTextureFromSurface(pRenderer, mTextSurface);
			SDL_Rect mTextDestRect = { 32,32, mTextSurface->w, mTextSurface->h };

			//Inventory text
			SDL_Surface* iTextSurface = TTF_RenderText_Blended_Wrapped(g_pFont, inventoryToSDLText.c_str(), textColor, 1200);
			SDL_Texture* iTextTexture = SDL_CreateTextureFromSurface(pRenderer, iTextSurface);
			SDL_Rect iTextDestRect = { 32,80, iTextSurface->w, iTextSurface->h };

			//Life text
			SDL_Surface* lTextSurface = TTF_RenderText_Blended_Wrapped(g_pFont, lifeToSDLText.c_str(), textColor, 1200);
			SDL_Texture* lTextTexture = SDL_CreateTextureFromSurface(pRenderer, lTextSurface);
			SDL_Rect lTextDestRect = { 32,188, lTextSurface->w, lTextSurface->h };

			//RenderCopy's
			SDL_RenderCopy(pRenderer, mTextTexture, nullptr, &mTextDestRect);
			SDL_RenderCopy(pRenderer, lTextTexture, nullptr, &lTextDestRect);
			SDL_RenderCopy(pRenderer, iTextTexture, nullptr, &iTextDestRect);
			
			//Get rid of textures for memory
			SDL_DestroyTexture(mTextTexture);
			SDL_DestroyTexture(lTextTexture);
			SDL_DestroyTexture(iTextTexture);
			
			//Free the textures
			SDL_FreeSurface(mTextSurface);
			SDL_FreeSurface(lTextSurface);
			SDL_FreeSurface(iTextSurface);
		}

		SDL_RenderPresent(pRenderer);
	}

	//Cleanup spritelist and textures
	void CleanupGame()
	{
		for (unsigned int i = 0; i < g_spriteList.size(); i++) {
			g_spriteList[i]->Cleanup();
			delete g_spriteList[i];
		}

		//SDL Cleanup
		//Kill Everything
		TTF_CloseFont(g_pFont);


		Sprite::ClearTextures();
	}
}