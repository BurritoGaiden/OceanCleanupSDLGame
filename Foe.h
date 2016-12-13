#pragma once
#include "Sprite.h"

namespace DungeonGame {
	class Foe : public Sprite {

	public:
		void InitMe(SDL_Renderer* pRenderer, FoeData* pFoeData);
		virtual void Update(float deltaSeconds, WorldState& worldState, PlayerState& playerState);
		
	private:
		FoeData* m_pFoeData;
	};

}