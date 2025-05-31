#pragma once
#include <glm/glm.hpp>
#include <vector>
#include "texture2D.h"
namespace sugoma::graphics 
{
	struct Sprite 
	{
		glm::vec2 offset;
		glm::vec2 size;
	};

	class SpriteSheet : public Texture2D 
	{
	public:
		SpriteSheet() = delete;
		SpriteSheet(const TextureCreateInfo& info, void* buffer, TextureFormat bufferFormat, TextureFormatComponent componentFormat);
		//SpriteSheet(Ref<Texture2D> texture, glm::uvec2 gridSize, uint32_t count = -1);
		//SpriteSheet(Ref<Texture2D> texture, const std::vector<Sprite>& sprites);

		void CreateSprite(glm::uvec2 offset, glm::uvec2 size);
		void SpriteGrid(glm::uvec2 gridSize, uint32_t count = -1);


		const Sprite& GetSprite(uint32_t index) const;
		const std::vector<Sprite>& GetSprites() const;
	private:
		std::vector<Sprite> m_sprites;
	};
}