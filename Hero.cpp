#include "Hero.h"

using namespace DungeonGame;

void Hero::InitMe(SDL_Renderer* pRenderer) {
	Sprite::Initialize(Sprite::LoadTexture(pRenderer, "submarine.bmp",0,255,0));
	m_Size = Vector2d(96.0f, 96.0f);
	m_LastNonZeroDirection = Vector2d(0.0f, 1.0f);
}


void Hero::Update(float deltaSeconds, WorldState& worldState, PlayerState& playerState) {
	
	//Moving Logic
	{
		const float HERO_PIXELS_PER_SECOND = 300.0f * deltaSeconds;

		Vector2d newPosition = playerState.m_CurrentPosition + playerState.m_DesiredDirection * HERO_PIXELS_PER_SECOND;
		if (worldState.GetTileTypeAtPosition(newPosition) == 1) {
			
			bool bHasLockedDoor = worldState.PositionCollidesWithLockedDoor(newPosition, playerState);
			
			if (!bHasLockedDoor) {
			
				playerState.m_CurrentPosition = newPosition;
			}
		}

		m_Position = playerState.m_CurrentPosition - Vector2d(m_Size.X * 0.5f, m_Size.Y * 0.85f);
	}
	
	m_bVisible = playerState.m_bAlive;

	//Shooting Logic
	{
		playerState.m_ShotCooldownSeconds -= deltaSeconds;
		if (playerState.m_ShotCooldownSeconds < 0.0f) {
			playerState.m_ShotCooldownSeconds = 0.0f;
		}

		if (playerState.m_DesiredDirection != Vector2d(0.0f, 0.0f)) {
			m_LastNonZeroDirection = playerState.m_DesiredDirection;
		}

		if (playerState.m_bWantsToShoot) {
			TryToShoot(playerState);
		}	
	}

	if (playerState.m_Hp <= 0) {
		playerState.m_bAlive = false;
	}

	if (!playerState.m_bAlive) {
		worldState.m_PlayersLives--;
		playerState.Reset();

		//Clear relevant data for "Restart"
		if (worldState.m_PlayersLives <= 0) {
			playerState.m_Inventory.clear();
			playerState.m_doorsUnlocked = 0;
			worldState.Reset();
		}
	}

}

void Hero::TryToShoot(PlayerState& playerState) {
	if (playerState.m_ShotCooldownSeconds == 0.0f) {
		for (unsigned int i = 0; i < playerState.m_Bullets.size(); i++) {
			BulletData& currBullet = playerState.m_Bullets[i];
			if (!currBullet.bAlive) {
				playerState.m_ShotCooldownSeconds = 0.2f;

				currBullet.bAlive = true;
				currBullet.secondsAlive = 0.0f;
				
				//Put here for bullets
				currBullet.direction = m_LastNonZeroDirection;

				//put here for melee
				currBullet.position = playerState.m_CurrentPosition + currBullet.direction * (TILE_DIAMETER * .5f);

				break;
			}
		}
	}
}