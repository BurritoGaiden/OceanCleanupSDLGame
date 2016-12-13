#include "HUD.h"

using namespace DungeonGame;

void HUD::InitMe(SDL_Renderer* pRenderer) {

	m_pShieldIcon = Sprite::LoadTexture(pRenderer, "shield.bmp");
	m_pCoreIcon = Sprite::LoadTexture(pRenderer, "core.bmp");
	m_pHealthIcon = Sprite::LoadTexture(pRenderer, "health.bmp");
	m_pHazardIcon = Sprite::LoadTexture(pRenderer, "hazard.bmp");
	m_Size = Vector2d(72.0f, 72.0f);
}

void HUD::Render(SDL_Renderer *pRenderer, const WorldState& worldState, const PlayerState& playerState, const Vector2d& baseTransformation) {
	
	for (unsigned int i = 0; i < playerState.m_Inventory.size(); i++) {
		ItemType currItem = playerState.m_Inventory[i];
		
		SDL_Texture* pCurrTexture = m_pShieldIcon;
		
		if (currItem == Item_Shield) {
			pCurrTexture = m_pShieldIcon;
		}
		else if (currItem == Item_Core) {
			pCurrTexture = m_pCoreIcon;
		}
		SDL_Rect destRect = {
			32 + 48 * i,
			112,
			(int)m_Size.X,
			(int)m_Size.Y };
		SDL_RenderCopy(pRenderer, pCurrTexture, nullptr, &destRect);
	}

	for (unsigned int j = 0; j < worldState.m_PlayersLives; j++) {
		SDL_Texture* pCurrTexture2 = m_pHealthIcon;

		SDL_Rect destRect2 = {
			32 + 64 * j,
			224,
			(int)m_Size.X,
			(int)m_Size.Y };
		SDL_RenderCopy(pRenderer, pCurrTexture2, nullptr, &destRect2);
	}

	//Shield
	SDL_Texture* pCurrTexture3 = m_pShieldIcon;

	SDL_Rect destRect3 = {
		420,
		16,
		(int)m_Size.X,
		(int)m_Size.Y };
	SDL_RenderCopy(pRenderer, pCurrTexture3, nullptr, &destRect3);

	//Toxic Waste
	SDL_Texture* pCurrTexture4 = m_pHazardIcon;

	SDL_Rect destRect4 = {
		1175,
		16,
		(int)m_Size.X,
		(int)m_Size.Y };
	SDL_RenderCopy(pRenderer, pCurrTexture4, nullptr, &destRect4);
}