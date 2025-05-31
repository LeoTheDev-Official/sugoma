#include "framebuffer.h"
#include "debug/logger.h"

#include "textures/texture1D.h"
#include "textures/texture2D.h"
#include "textures/texture3D.h"
#include "textures/texture_cube.h"

#include <GL/glew.h>

namespace sugoma::graphics 
{
	Framebuffer::Framebuffer(const FramebufferSpecification& createInfo) : m_spec(createInfo)
	{
		glGenFramebuffers(1, &m_handle);
		glBindFramebuffer(GL_FRAMEBUFFER, m_handle);

		GLEnum colorAtt = GL_COLOR_ATTACHMENT0;
		m_attachments.reserve(createInfo.attachments.size());

		for (auto& att : createInfo.attachments)
		{
			TextureCreateInfo info{};
			info.border = 0;
			info.format = att.format;
			info.depth = createInfo.depth;
			info.width = createInfo.width;
			info.height = createInfo.height;
			info.horizontalWrap = att.horizontalWrap;
			info.verticalWrap = att.verticalWrap;
			info.layers = createInfo.layers;
			info.magFilter = att.magFilter;
			info.minFilter = att.minFilter;
			GLEnum binding = colorAtt;
			Texture* texture = nullptr;
			switch (att.attachmentType)
			{
			case FramebufferAttachmentType::Color:
				++colorAtt;
				break;
			case FramebufferAttachmentType::Depth:
				binding = GL_DEPTH_ATTACHMENT;
				break;
			case FramebufferAttachmentType::Stencil:
				binding = GL_STENCIL_ATTACHMENT;
				break;
			case FramebufferAttachmentType::DepthStencil:
				binding = GL_DEPTH_STENCIL_ATTACHMENT;
				break;
			}
			switch (createInfo.dimension)
			{
			case TextureDimension::Texture1D:
			{
				texture = new Texture1D(info, nullptr, TextureFormat::RGBA, TextureFormatComponent::BYTE);
				glBindTexture(GL_TEXTURE_1D, texture->Handle());
				glFramebufferTexture1D(GL_FRAMEBUFFER, binding, GL_TEXTURE_1D, texture->Handle(), att.level);
				break;
			}
			case TextureDimension::Texture2D:
			{
				texture = new Texture2D(info, nullptr, TextureFormat::RGBA, TextureFormatComponent::BYTE);
				glBindTexture(GL_TEXTURE_2D, texture->Handle());
				glFramebufferTexture2D(GL_FRAMEBUFFER, binding, GL_TEXTURE_2D, texture->Handle(), att.level);
				break;
			}
			case TextureDimension::Texture3D:
			{
				texture = new Texture3D(info, nullptr, TextureFormat::RGBA, TextureFormatComponent::BYTE);
				glBindTexture(GL_TEXTURE_3D, texture->Handle());
				glFramebufferTexture3D(GL_FRAMEBUFFER, binding, GL_TEXTURE_3D, texture->Handle(), att.level, att.layer);
				break;
			}
			default:
				sugoma_assert(false, "Framebuffer attachment dimension not implemented.");
				return;
			}
			FramebufferAttachment atti{};
			atti.info = att;
			atti.texture = texture;
			m_attachments.push_back(atti);
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	Framebuffer::~Framebuffer()
	{
		if (m_handle)
			glDeleteFramebuffers(1, &m_handle);
	}
	void Framebuffer::Resize(uint32_t width, uint32_t height, uint32_t depth)
	{
		for (auto& attachment : m_attachments)
			attachment.texture->Reinitialize(width, height, depth);
	}
	const FramebufferSpecification& Framebuffer::Specification() const { return m_spec; }
	GLHandle Framebuffer::Handle() const { return m_handle; }
	FramebufferAttachment* Framebuffer::GetAttachment(uint32_t index)
	{
		if (index >= m_attachments.size()) return nullptr;
		return &m_attachments[index];
	}
	const FramebufferAttachment* Framebuffer::GetAttachment(uint32_t index) const
	{
		if (index >= m_attachments.size()) return nullptr;
		return &m_attachments[index];
	}
	std::vector<FramebufferAttachment>& Framebuffer::Attachments() { return m_attachments; }
	const std::vector<FramebufferAttachment>& Framebuffer::Attachments() const { return m_attachments; }
}