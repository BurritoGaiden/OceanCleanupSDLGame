#include "Item.h"

using namespace DungeonGame;

void Item::InitMe(SDL_Renderer* pRenderer, ItemData* pItemData) {
		
	m_pItemData = pItemData;

	std::string textureFileName = "shield.bmp";
	switch (m_pItemData->Type) {
		case Item_Shield:
			textureFileName = "shield.bmp";
			break;
		case Item_Core:
			textureFileName = "core.bmp";
			break;
	}
		
	Sprite::Initialize(Sprite::LoadTexture(pRenderer, textureFileName.c_str(),0,255,0));
	m_Size = Vector2d(TILE_DIAMETER, TILE_DIAMETER);
	
}

void Item::Update(float deltaSeconds, WorldState& worldState, PlayerState& playerState) {
	//worldState.m_Items[];
	m_Position = m_pItemData->Position - Vector2d(m_Size * 0.5f);
	m_bVisible = m_pItemData->bAlive;
	//m_Position = m_pItemData->Position;
	
	if (m_pItemData->bAlive && playerState.ItemCollidesWithPlayer(m_pItemData)) {
		//m_bVisible = false;
		m_pItemData->bAlive = false;


		playerState.m_Inventory.push_back(m_pItemData->Type);
	}
}

