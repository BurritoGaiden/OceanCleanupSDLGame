#include "Sprite.h"

using namespace DungeonGame;

std::vector<Sprite::LoadedTexture> Sprite::s_LoadedTextures;

Sprite::Sprite() :
	m_pTexture(nullptr),
	m_Position(0.0f, 0.0f),
	m_Size(72.0f, 72.0f),
	m_bVisible(true)
{
}

void Sprite::Initialize(SDL_Texture* pTexture)
{
	m_pTexture = pTexture;
}

void Sprite::Update(float deltaSeconds, WorldState& worldState, PlayerState& playerState)
{
}

void Sprite::Render(SDL_Renderer *pRenderer, const WorldState& worldState, const PlayerState& playerState, const Vector2d& baseTransformation)
{
	if (m_pTexture && m_bVisible)
	{
		SDL_Rect destRect = { (int)(m_Position.X + baseTransformation.X), (int)(m_Position.Y + baseTransformation.Y), (int)m_Size.X, (int)m_Size.Y };
		SDL_RenderCopy(pRenderer, m_pTexture, nullptr, &destRect);
	}
}

void Sprite::Cleanup()
{
}

SDL_Texture* Sprite::LoadTexture(SDL_Renderer *pRenderer, const std::string& inFilename, Uint8 inColorKeyR, Uint8 inColorKeyG, Uint8 inColorKeyB)
{
	for (auto it = s_LoadedTextures.begin(); it != s_LoadedTextures.end(); ++it)
	{
		if (it->Filename == inFilename)
		{
			return it->pTexture;
		}
	}

	SDL_Surface *pBmp = SDL_LoadBMP(inFilename.c_str());
	SDL_SetColorKey(pBmp, SDL_TRUE, SDL_MapRGB(pBmp->format, inColorKeyR, inColorKeyG, inColorKeyB));
	SDL_Texture *pTexture = SDL_CreateTextureFromSurface(pRenderer, pBmp);
	SDL_FreeSurface(pBmp);

	LoadedTexture loadedTexture = {};
	loadedTexture.Filename = inFilename;
	loadedTexture.pTexture = pTexture;
	s_LoadedTextures.push_back(loadedTexture);

	return pTexture;
}

void Sprite::ClearTextures()
{
	for (unsigned int i = 0; i < s_LoadedTextures.size(); ++i)
	{
		SDL_DestroyTexture(s_LoadedTextures[i].pTexture);
	}
	s_LoadedTextures.clear();
}