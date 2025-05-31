#include "renderer_2d.h"

#include "buffers/framebuffer.h"
#include "buffers/shader_storage_buffer.h"
#include "buffers/uniform_buffer.h"
#include "pipeline/render_pass.h"
#include "pipeline/graphics_pipeline.h"

#include "buffers/vertex_array_object.h"

#include "mesh/mesh.h"

#include "utility/file_utility.h"
#include "debug/logger.h"


#include "textures/texture2D.h"
#include "textures/sprite_sheet.h"

#include <GL/glew.h>

#define MAX_SPRITES_PER_CALL 1024

namespace sugoma::graphics 
{
#pragma region utility_functions
	Framebuffer* create_framebuffer(glm::uvec2 resolution, TextureFilter magFilter, TextureFilter minFilter)
	{
		FramebufferSpecification fbSpec{};
		fbSpec.dimension = TextureDimension::Texture2D;
		fbSpec.width = resolution.x;
		fbSpec.height = resolution.y;
		fbSpec.depth = 1;
		fbSpec.layers = 1;

		FramebufferAttachmentInfo attachment{};
		attachment.format = TextureFormat::RGBA16F;
		attachment.attachmentType = FramebufferAttachmentType::Color;
		attachment.depth = 1;
		attachment.layer = 0;
		attachment.magFilter = magFilter;
		attachment.minFilter = minFilter;
		attachment.name = "color";
		fbSpec.attachments.push_back(attachment);

		attachment.format = TextureFormat::RGB8;
		attachment.name = "normal";
		fbSpec.attachments.push_back(attachment);

		attachment.format = TextureFormat::RGB16F;
		attachment.name = "position";
		fbSpec.attachments.push_back(attachment);
		
		return new Framebuffer(fbSpec);
	}
	GraphicsPipeline* create_sprite_pipeline() 
	{
		PipelineCreateInfo info{};
		
		PipelineStageCreateInfo vertex{};
		vertex.source = FileUtility::ReadStringFile("data/r2D_sprite_default_vertex.shader");
		vertex.stage = VertexStageBit;

		PipelineStageCreateInfo fragment{};
		fragment.source = FileUtility::ReadStringFile("data/r2D_sprite_default_fragment.shader");
		fragment.stage = FragmentStageBit;

		info.stages = { vertex, fragment };

		return new GraphicsPipeline(info);
	}
	Ref<Texture2D> get_blank_texture() 
	{
		static Ref<Texture2D> tex = 0;
		if (!tex) 
		{
			TextureCreateInfo info{};
			info.width = 1;
			info.height = 1;
			info.format = TextureFormat::RGBA8;
			glm::vec4 color = { 1, 1, 1, 1 };
			tex = Texture2D::Create(info, &color, TextureFormat::RGBA, TextureFormatComponent::FLOAT);
		}
		return tex;
	}
	RenderPass* create_main_render_pass(const FramebufferSpecification& spec) 
	{
		RenderPassCreateInfo info{};
		info.pFbSpec = &spec;
		info.pDebugName = "sprite_pass";
		
		RenderPassState& state = info.state;
		
		RenderPassBufferClear colorClear{ 0, glm::vec4(0.05f) };
		RenderPassBufferClear normalClear{1, glm::vec4(0)};
		RenderPassBufferClear positionClear{ 2, glm::vec4(0) };
		state.clearState = { colorClear, normalClear, positionClear };

		state.enableBlending = true;
		RenderPassBlending bl{};
		bl.fbIndex = 0;
		state.blendingState = { bl };

		return new RenderPass(info);
	}
#pragma endregion

#pragma region utility_structs
	struct SpriteInstance 
	{
		glm::mat4 transform;
		glm::vec4 tint;
		glm::vec2 offset;
		glm::vec2 size;
	};
	struct CameraData 
	{
		glm::mat4 view;
		glm::mat4 projection;
	};
#pragma endregion



	Renderer2D::Renderer2D(const RendererSpecification& spec) : Renderer(spec)
	{
		int32_t value;
		glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &value);
		m_texture_slots = value;

		m_framebuffer = create_framebuffer(spec.resolution, spec.magFilter, spec.minFilter);
		m_sprite_pipeline = create_sprite_pipeline();

		size_t size = sizeof(SpriteInstance) * MAX_SPRITES_PER_CALL;

		m_camera_buffer = new UniformBuffer(sizeof(CameraData), BufferUsage::DYNAMIC, 0);
		m_sprites_buffer = new ShaderStorageBuffer(size, BufferUsage::DYNAMIC, 0);
		m_main_pass = create_main_render_pass(m_framebuffer->Specification());

		m_quad_mesh = Primitives::Quad().get();
	}
	Renderer2D::~Renderer2D()
	{
		delete m_framebuffer;
		delete m_main_pass;
		delete m_camera_buffer;
		delete m_sprites_buffer;
		delete m_sprite_pipeline;
	}
	void Renderer2D::Resize(uint32_t width, uint32_t height, uint32_t depth)
	{
		m_framebuffer->Resize(width, height, depth);
	}
	void Renderer2D::BeginScene(const Camera& camera)
	{
		m_main_pass->BeginRenderPass(m_framebuffer);

		m_camera = camera;
		CameraData cd{};
		cd.projection = m_camera.Projection();
		cd.view = m_camera.View();

		m_camera_buffer->SubData(&cd, 0, sizeof(cd));

		m_camera_buffer->Use(0);
		m_sprites_buffer->Use(0);

		glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer->Handle());
		glViewport(0, 0, m_render_resolution.x, m_render_resolution.y);
	}
	void Renderer2D::EndScene()
	{
		m_main_pass->EndRenderPass();
	}
	void Renderer2D::DrawQuad(glm::mat4 transform, Ref<Texture2D> texture, glm::vec4 tint)
	{
		glUseProgram(m_sprite_pipeline->Handle());
		SpriteInstance inst{};
		inst.offset = { 0, 0 };
		inst.size = { 1, 1 };
		inst.tint = tint;
		inst.transform = transform;

		if (!texture) texture = get_blank_texture();

		m_sprites_buffer->SubData(&inst, 0, sizeof(inst));

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture->Handle());

		glBindVertexArray(m_quad_mesh->VAO()->Handle());
		glDrawElements(GL_TRIANGLES, m_quad_mesh->IndexCount(), GL_UNSIGNED_INT, nullptr);
	}
	void Renderer2D::DrawSprite(glm::mat4 transform, SpriteSheet* spriteSheet, uint32_t spriteIndex, glm::vec4 tint)
	{
		glUseProgram(m_sprite_pipeline->Handle());
		Sprite sp = spriteSheet->GetSprite(spriteIndex);
		SpriteInstance inst{};
		inst.offset = sp.offset;
		inst.size = sp.size;
		inst.tint = tint;
		inst.transform = transform;

		if (!spriteSheet) { DrawQuad(transform, get_blank_texture(), tint); return; }

		m_sprites_buffer->SubData(&inst, 0, sizeof(inst));

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, spriteSheet->Handle());

		glBindVertexArray(m_quad_mesh->VAO()->Handle());
		glDrawElements(GL_TRIANGLES, m_quad_mesh->IndexCount(), GL_UNSIGNED_INT, nullptr);
	}
	void Renderer2D::DrawSprites(const std::vector<SpriteDrawData>& spriteData, SpriteSheet* spriteSheet)
	{
		glUseProgram(m_sprite_pipeline->Handle());
		std::vector<SpriteInstance> instances(spriteData.size());
		for (size_t i = 0; i < spriteData.size(); ++i) 
		{
			auto& sd = spriteData[i];
			Sprite sp = spriteSheet->GetSprite(sd.spriteIndex);
			SpriteInstance& inst = instances[i];
			inst.offset = sp.offset;
			inst.size = sp.size;
			inst.tint = sd.tint;
			inst.transform = sd.transform;
		}
		m_sprites_buffer->SubData(instances.data(), 0, instances.size() * sizeof(SpriteInstance));
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, spriteSheet->Handle());
		Ref<Mesh> mesh = Primitives::Quad();
		glBindVertexArray(mesh->VAO()->Handle());
		glDrawElementsInstanced(GL_TRIANGLES, mesh->IndexCount(), GL_UNSIGNED_INT, nullptr, spriteData.size());
	}
}