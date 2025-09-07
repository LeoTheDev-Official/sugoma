#pragma once
#include <glm/glm.hpp>
#include "camera.h"
#include "buffers/framebuffer.h"
namespace sugoma 
{
	using namespace graphics;
	struct RendererSpecification
	{
		glm::uvec2 resolution;
		uint8_t samples = 1;
	};
	class Renderer 
	{
	public:
		Renderer() = delete;
		Renderer(const RendererSpecification& spec);
		virtual ~Renderer() = default;
	public:
		const Ref<Texture> OutputTexture() const;
		virtual void Begin(const Camera& camera) = 0;
		virtual void End() = 0;
	protected:
		RendererSpecification m_rendererSpec;
		Ref<Texture> m_outputTexture;
	};
}