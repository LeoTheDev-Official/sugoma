
#include "renderer.h"

namespace sugoma::graphics 
{
	Renderer::Renderer(const RendererSpecification& spec) : m_render_resolution(spec.resolution) {}
	const Framebuffer* Renderer::GetFramebuffer() const { return m_framebuffer; }
	Framebuffer* Renderer::GetFramebuffer() { return m_framebuffer; }
}