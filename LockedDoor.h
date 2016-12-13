#pragma once
#include "Sprite.h"

namespace DungeonGame {
	class LockedDoor : public Sprite {

	public:
		void InitMe(SDL_Renderer* pRenderer, LockedDoorData* pDoorData);
		virtual void Update(float deltaSeconds, WorldState& worldState, PlayerState& playerState);

	private:
		LockedDoorData* m_pLockedDoorData;


	};

}