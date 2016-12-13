#pragma once
#include "Sprite.h"

namespace DungeonGame 
{

	class Enemy : public Sprite 
	{
	
	public:
		void Initialize(SDL_Renderer* pRenderer);
		virtual void Render(SDL_Renderer *pRenderer, const WorldState& worldState, const PlayerState& playerState, const Vector2d& baseTransformation = Vector2d());

	private:
		SDL_Texture* m_pExtraSprite;
	};


}