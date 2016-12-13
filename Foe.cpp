#include "Foe.h"

using namespace DungeonGame;

void Foe::InitMe(SDL_Renderer* pRenderer, FoeData* pFoeData) {

	m_pFoeData = pFoeData;

	Sprite::Initialize(Sprite::LoadTexture(pRenderer, "mine.bmp",0,255,0));
	m_Size = Vector2d(TILE_DIAMETER, TILE_DIAMETER);
}

void Foe::Update(float deltaSeconds, WorldState& worldState, PlayerState& playerState) {

	m_Position = m_pFoeData->Position - Vector2d(m_Size * 0.5f);
	m_bVisible = m_pFoeData->bAlive;
	//m_Position = m_pItemData->Position;

	if (m_pFoeData->bAlive && playerState.FoeCollidesWithPlayer(m_pFoeData)) {
		//m_bVisible = false;
		m_pFoeData->bAlive = false;

		playerState.m_Hp --;
	}

	//Items seek toward player code
	Vector2d itemToPlayer = m_pFoeData->Position - playerState.m_CurrentPosition;
	float distance = itemToPlayer.GetLength();
	if (distance < 260.0f) {
	const float MOVEMENT_SPEED = 80.0f * deltaSeconds;
	itemToPlayer.Normalize();
	m_pFoeData->Position = m_pFoeData->Position - itemToPlayer * MOVEMENT_SPEED;

	}
}