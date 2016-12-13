#pragma once
#include "Sprite.h"

namespace DungeonGame {
	class Hero : public Sprite {
	public:
		void InitMe(SDL_Renderer* pRenderer);
		virtual void Update(float deltaSeconds, WorldState& worldState, PlayerState& playerState);

	private:
		void TryToShoot(PlayerState& playerState);
		int m_Hp;
		int m_Mp;

		Vector2d m_LastNonZeroDirection;
	};
}