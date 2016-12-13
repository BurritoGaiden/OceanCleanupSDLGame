#pragma once
#include "SDL.h"
#include "GameCore.h"
#include <string>
#include <vector>

namespace DungeonGame
{
	class Sprite
	{
	public:
		Sprite();

		virtual void Initialize(SDL_Texture* pTexture);
		virtual void Update(float deltaSeconds, WorldState& worldState, PlayerState& playerState);
		virtual void Render(SDL_Renderer *pRenderer, const WorldState& worldState, const PlayerState& playerState, const Vector2d& baseTransformation = Vector2d());
		virtual void Cleanup();

	public:
		Vector2d m_Position;
		Vector2d m_Size;
		bool m_bVisible;

	protected:
		SDL_Texture* m_pTexture;
		std::vector<Sprite*> m_children;

	public:
		static SDL_Texture* LoadTexture(SDL_Renderer *pRenderer, const std::string& inFilename, Uint8 inColorKeyR = 0, Uint8 inColorKeyG = 255, Uint8 inColorKeyB = 0);
		static void ClearTextures();

	private:
		struct LoadedTexture
		{
			std::string Filename;
			SDL_Texture* pTexture;
		};

		static std::vector<LoadedTexture> s_LoadedTextures;
	};
}