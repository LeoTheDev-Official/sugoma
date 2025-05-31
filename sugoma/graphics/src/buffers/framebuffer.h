#pragma once
#include "sugoma_gl.h"
#include "textures/texture.h"
#include <vector>
#include <string>

namespace sugoma::graphics 
{
	enum class FramebufferAttachmentType 
	{
		Color,
		Depth,
		Stencil,
		DepthStencil
	};
	struct FramebufferAttachmentInfo 
	{
		TextureFormat format;
		TextureFilter magFilter = LINEAR, minFilter = LINEAR;
		TextureWrapMode verticalWrap = CLAMP_TO_EDGE, horizontalWrap = CLAMP_TO_EDGE;
		FramebufferAttachmentType attachmentType = FramebufferAttachmentType::Color;
		uint32_t depth = 1;
		uint32_t level = 0, layer = 0;
		std::string name;
	};
	struct FramebufferAttachment 
	{
		Texture* texture;
		FramebufferAttachmentInfo info;
	};
	struct FramebufferSpecification 
	{
		uint32_t width, height, depth = 1;
		TextureDimension dimension = TextureDimension::Texture2D;
		uint32_t layers = 1;
		std::vector<FramebufferAttachmentInfo> attachments;
	};
	class Framebuffer 
	{
	public:
		Framebuffer() = delete;
		Framebuffer(const FramebufferSpecification& createInfo);
		~Framebuffer();
		void Resize(uint32_t width, uint32_t height, uint32_t depth);
		const FramebufferSpecification& Specification() const;
		GLHandle Handle() const;
		FramebufferAttachment* GetAttachment(uint32_t index);
		const FramebufferAttachment* GetAttachment(uint32_t index) const;
		std::vector<FramebufferAttachment>& Attachments();
		const std::vector<FramebufferAttachment>& Attachments() const;
	private:
		GLHandle m_handle;
		FramebufferSpecification m_spec;
		std::vector<FramebufferAttachment> m_attachments;
	};
}