#include "Enemy.h"

using namespace DungeonGame;

void Enemy::Initialize(SDL_Renderer* pRenderer) {

	Sprite::Initialize(Sprite::LoadTexture(pRenderer, "submarine.bmp"));
	m_Size = Vector2d(72.0f, 72.0f);

	m_pExtraSprite = Sprite::LoadTexture(pRenderer, "background.bmp");
}

void Enemy::Render(SDL_Renderer *pRenderer, const WorldState& worldState, const PlayerState& playerState, const Vector2d& baseTransformation)
{
	Sprite::Render(pRenderer, worldState, playerState, baseTransformation);
	/*
	if (m_pTexture && m_bVisible)
	{
		SDL_Rect destRect = { 
			(int)(m_Position.X + baseTransformation.X) + 12.0f, 
			(int)(m_Position.Y + baseTransformation.Y) + 12.0f, 
			(int)m_Size.X * 0.25f, 
			(int)m_Size.Y * 0.25f};
		SDL_RenderCopy(pRenderer, m_pExtraSprite, nullptr, &destRect);
	}
	*/
	//New Stuff
}