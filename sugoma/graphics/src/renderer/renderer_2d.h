#pragma once
#include "renderer.h"
#include <vector>
#include "camera.h"
namespace sugoma::graphics 
{
	class Mesh;
	class RenderPass;
	class SpriteSheet;
	class ShaderStorageBuffer;
	class UniformBuffer;
	class GraphicsPipeline;
	class Texture2D;
	struct SpriteDrawData
	{
		glm::mat4 transform = glm::mat4(1);
		uint32_t spriteIndex;
		glm::vec4 tint = glm::vec4(1);
	};

	class Renderer2D : public Renderer 
	{
	public:
		Renderer2D() = delete;
		Renderer2D(const RendererSpecification& spec);
		~Renderer2D();

		void Resize(uint32_t width, uint32_t height, uint32_t depth);

	public:
		void BeginScene(const Camera& camera);
		void EndScene();

		void DrawQuad(glm::mat4 transform, Ref<Texture2D> texture, glm::vec4 tint);
		void DrawSprite(glm::mat4 transform, SpriteSheet* spriteSheet, uint32_t spriteIndex, glm::vec4 tint);
		void DrawSprites(const std::vector<SpriteDrawData>& spriteData, SpriteSheet* spriteSheet);



	private:
		Camera m_camera;
		uint32_t m_texture_slots;

		RenderPass* m_main_pass;
		GraphicsPipeline* m_sprite_pipeline;
		UniformBuffer* m_camera_buffer;
		ShaderStorageBuffer* m_sprites_buffer;

		const Mesh* m_quad_mesh;
	};
}