
#include "sprite_sheet.h"

namespace sugoma::graphics 
{
	SpriteSheet::SpriteSheet(const TextureCreateInfo& info, void* buffer, TextureFormat bufferFormat, TextureFormatComponent componentFormat):
		Texture2D(info, buffer, bufferFormat, componentFormat)
	{

	}
	void SpriteSheet::CreateSprite(glm::uvec2 offset, glm::uvec2 size)
	{
		Sprite sprite{};
		glm::vec2 tsize{ m_info.width, m_info.height };
		sprite.offset = (glm::vec2)offset / tsize;
		sprite.size = (glm::vec2)size / tsize;
		m_sprites.push_back(sprite);
	}
	void SpriteSheet::SpriteGrid(glm::uvec2 gridSize, uint32_t count)
	{
		uint32_t sc = glm::min(gridSize.x * gridSize.y, count);
		float ux = 1.0f / gridSize.x;
		float uy = 1.0f / gridSize.y;
		m_sprites.reserve(sc);
		for (uint32_t i = 0; i < sc; ++i)
		{
			uint32_t x = i % gridSize.x;
			uint32_t y = i / gridSize.x;
			Sprite sprite{};
			sprite.offset = { x * ux, (gridSize.y - y - 1) * uy };
			sprite.size = { ux, uy };
			m_sprites.push_back(sprite);
		}
	}
	const Sprite& SpriteSheet::GetSprite(uint32_t index) const { return m_sprites[index]; }
	const std::vector<Sprite>& SpriteSheet::GetSprites() const { return m_sprites; }
}