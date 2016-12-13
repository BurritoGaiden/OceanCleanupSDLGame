#include "Bullet.h"

using namespace DungeonGame;

void Bullet::InitMe(SDL_Renderer* pRenderer, BulletData* pBulletData) {

	m_pBulletData = pBulletData;

	std::string textureFileName = "bullet.bmp";
	Sprite::Initialize(Sprite::LoadTexture(pRenderer, textureFileName.c_str()));
	
	m_Size = Vector2d(48.0f, 48.0f);

}

void Bullet::Update(float deltaSeconds, WorldState& worldState, PlayerState& playerState) {
	//worldState.m_Bullets[];
	m_Position = m_pBulletData->position - Vector2d(m_Size * 0.5f);
	m_bVisible = m_pBulletData->bAlive;
	


	if (m_pBulletData->bAlive) {
		m_pBulletData->secondsAlive += deltaSeconds;

		//Melee Logic
		if (m_pBulletData->secondsAlive >= .2f) {
			m_pBulletData->bAlive = false;
		}

		if (worldState.BulletCollidesWithItem(m_pBulletData)) {
			m_pBulletData->bAlive = false;
		}

		//Bullet Logic
		/*if (m_pBulletData->secondsAlive >= 1.5f) {
			m_pBulletData->bAlive = false;
		}
	
		const float BULLET_SPEED = 300.0f * deltaSeconds;
		//Vector2d bulletDirection = Vector2d(0.0f, 1.0f);
		m_pBulletData->position = m_pBulletData->position + m_pBulletData->direction * BULLET_SPEED;
	
		if (worldState.BulletCollidesWithItem(m_pBulletData))
		{
	
			m_pBulletData->bAlive = false;
		}
		*/
	}
}