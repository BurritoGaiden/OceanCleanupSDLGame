#pragma once
#include "Sprite.h"

namespace DungeonGame {
	class HUD : public Sprite {

	public:
		void InitMe(SDL_Renderer* pRenderer);
		virtual void Render(SDL_Renderer *pRenderer, const WorldState& worldState, const PlayerState& playerState, const Vector2d& baseTransformation = Vector2d());

	private:
		SDL_Texture* m_pShieldIcon;
		SDL_Texture* m_pCoreIcon;
		SDL_Texture* m_pHealthIcon;
		SDL_Texture* m_pHazardIcon;
	};
}
