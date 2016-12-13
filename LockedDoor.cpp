#include "LockedDoor.h"

using namespace DungeonGame;

void LockedDoor::InitMe(SDL_Renderer* pRenderer, LockedDoorData* pLockedDoorData) {

	m_pLockedDoorData = pLockedDoorData;

	std::string textureFileName = "hazard.bmp";
	
	Sprite::Initialize(Sprite::LoadTexture(pRenderer, textureFileName.c_str(), 0,255,0));
	m_Size = Vector2d(TILE_DIAMETER, TILE_DIAMETER);

}

void LockedDoor::Update(float deltaSeconds, WorldState& worldState, PlayerState& playerState) {
	//worldState.m_LockedDoors[];
	m_Position = m_pLockedDoorData->position - Vector2d(m_Size * 0.5f);
	m_bVisible = m_pLockedDoorData->bLocked;
	//m_Position = m_pLockedDoorData->Position;


}