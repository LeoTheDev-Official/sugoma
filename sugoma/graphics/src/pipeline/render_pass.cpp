#include "render_pass.h"
#include "buffers/framebuffer.h"

#include <GL/glew.h>

namespace sugoma::graphics 
{
	RenderPass::RenderPass(const RenderPassCreateInfo& createInfo) : m_state(createInfo.state), m_name(createInfo.debugName) 
	{
		const FramebufferSpecification* spec = createInfo.fbSpec;
		m_drawBuffers.resize(spec->attachments.size());
		uint32_t ci = 0;
		for (size_t i = 0; i < m_drawBuffers.size(); ++i) 
		{
			auto& att = spec->attachments[i];
			if (att.attachmentType == FramebufferAttachmentType::Color) m_drawBuffers[i] = ci++;
			else m_drawBuffers[i] = -1;
		}
	}
	void RenderPass::BeginRenderPass(Framebuffer* fb)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, fb->Handle());

		glViewport(0, 0, fb->Specification().width, fb->Specification().height);

		SetupDrawBuffers(fb);
		ClearBuffers(fb);
		SetCullState();
		SetBlendState();
		SetDepthState();
	}
	void RenderPass::EndRenderPass()
	{

	}
	RenderPassState& RenderPass::State() { return m_state; }
	void RenderPass::SetupDrawBuffers(Framebuffer* target)
	{
		const auto attachments = target->Attachments();
		std::vector<GLEnum> buffers(attachments.size());
		uint32_t ci = 0;
		for (size_t i = 0; i < m_drawBuffers.size(); ++i) {
			int32_t b = m_drawBuffers[i];
			if (b != -1) buffers[b] = GL_COLOR_ATTACHMENT0 + b;
		}
		glDrawBuffers(buffers.size(), buffers.data());
	}

	void RenderPass::ClearBuffers(Framebuffer* target)
	{
		for (auto& clear : m_state.clearState) 
		{
			auto* att = target->GetAttachment(clear.fbIndex);
			switch (att->info.attachmentType)
			{
			case FramebufferAttachmentType::Color:
				glClearBufferfv(GL_COLOR, m_drawBuffers[clear.fbIndex], &clear.value.x);
				break;
			case FramebufferAttachmentType::Depth:
				glClearBufferfv(GL_DEPTH, 0, &clear.value.x);
				break;
			case FramebufferAttachmentType::Stencil: {
				glm::ivec4 v = clear.value;
				glClearBufferiv(GL_STENCIL, 0, &v.x);
				break;
			}
			case FramebufferAttachmentType::DepthStencil:
				glClearBufferfi(GL_DEPTH_STENCIL, 0, clear.value.x, (int)clear.value.y);
				break;
			}
		}
	}

	void RenderPass::SetCullState()
	{
		if (m_state.enableCulling)
		{
			glEnable(GL_CULL_FACE);
			glCullFace(m_state.cullState.faceToCull);
		}
		else
			glDisable(GL_CULL_FACE);
	}

	void RenderPass::SetBlendState()
	{
		if (m_state.enableBlending)
		{
			glEnable(GL_BLEND);
			for (auto& blend : m_state.blendingState) 
			{
				int32_t b = m_drawBuffers[blend.fbIndex];
				glEnablei(GL_BLEND, blend.fbIndex);
				glBlendFunci(b, blend.sourceFactor, blend.destFactor);
				glBlendEquationi(b, blend.equation);
			}
		}
		else
			glDisable(GL_BLEND);
	}

	void RenderPass::SetDepthState()
	{
		if (m_state.enableDepth)
		{
			glEnable(GL_DEPTH_TEST);
			glDepthFunc(m_state.depthState.depthFunction);
			glDepthMask(m_state.depthState.depthWrite);
		}
		else
			glDisable(GL_DEPTH_TEST);
	}

}