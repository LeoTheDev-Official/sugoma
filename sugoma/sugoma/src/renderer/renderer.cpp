#include "renderer.h"

namespace sugoma 
{
	Renderer::Renderer(const RendererSpecification& spec) : m_rendererSpec(spec) {}
	const Ref<Texture> Renderer::OutputTexture() const { return m_outputTexture; }
}