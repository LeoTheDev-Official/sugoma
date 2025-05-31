#pragma once
#include <glm/glm.hpp>
#include "math/transform.h"
#include "textures/texture.h"
namespace sugoma::graphics 
{
	class Framebuffer;
	struct RendererSpecification
	{
		glm::uvec2 resolution;
		TextureFilter magFilter, minFilter;
	};

	class Renderer 
	{
	public:
		Renderer() = delete;
		Renderer(const RendererSpecification& spec);
		virtual ~Renderer() = default;
	public:
		const Framebuffer* GetFramebuffer() const;
		Framebuffer* GetFramebuffer();
	protected:
		Framebuffer* m_framebuffer;
		glm::uvec2 m_render_resolution;
	};
}