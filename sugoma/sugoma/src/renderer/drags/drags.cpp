#include <functional>
#include <unordered_set>
#include <format>
#include "drags.h"
#include "pipeline/render_pass.h"
#include "draw_commands.h"
#include "utility/sush_utility.h"

#include "math/algorithm.h"

#include "window.h"
#include "pipeline/compute_pipeline.h";

#include "debug/logger.h"

#define DRAGS_MAX_INSTANCE_COUNT 2048
#define DRAGS_MAX_MATERIAL_COUNT 1024

namespace sugoma 
{
	Ref<Texture2D> s_whiteTexture;
	Ref<Texture2D> s_normalTexture;

	struct drags_camera_data 
	{
		glm::mat4 view = glm::mat4(1);
		glm::mat4 projection = glm::mat4(1);
		glm::mat4 invView = glm::mat4(1);
		glm::mat4 invProjection = glm::mat4(1);
	};
	struct drags_material 
	{
		glm::vec4 albedo = glm::vec4(1);
		int32_t albedoMap;
		int32_t normalMap;
		float metallic = 0.1f;
		int32_t metallicMap;
		float roughness = 0.1f;
		int32_t roughnessMap;
		float ao = 0.1f;
		int32_t aoMap;
	};
	struct drags_instance_data 
	{
		glm::mat4 transform = glm::mat4(1);
		int32_t materialIndex = 0;
		int32_t __padding[3];
	};
	struct drags_enviorment 
	{
		glm::vec3 sunDirection;
		float __padding0;
		glm::vec3 sunColor;
	};
	using DragsRenderCmd = std::function<void()>;
	class DragsRenderPass 
	{
	public:
		DragsRenderPass() = default;
		virtual void Execute(const Camera& camera, Framebuffer& fb) = 0;
		virtual void Clear() = 0;
		virtual void Invalidate(const Framebuffer& fb) = 0;

		void SubmitCommand(const DragsRenderCmd& cmd) { m_commands.push_back(cmd); }	
	protected:
		RenderPass m_renderPass;
		std::vector<DragsRenderCmd> m_commands;
	};
	class ClearPass : public DragsRenderPass
	{
	public:
		ClearPass(const Framebuffer& fb)
		{
			RenderPassCreateInfo info
			{
				.state = RenderPassState
				{
					.clearState =
					{
						RenderPassBufferClear{ 0, glm::vec4(1, 0, 1, 0) },
						RenderPassBufferClear{ 1, glm::vec4(0) },
						RenderPassBufferClear{ 2, glm::vec4(0) },
						RenderPassBufferClear{ 3, glm::vec4(0) },
						RenderPassBufferClear{ 4, glm::vec4(1) }
					}
				},
				.fbSpec = &fb.Specification(),
				.debugName = "ClearPass"
			};
			m_renderPass = RenderPass(info);
				
			info.state.clearState.clear();
			info.state.enableDepth = true;
			info.state.depthState.depthWrite = true;
			info.state.depthState.depthFunction = DepthFunc::Always;
			info.debugName = "SkyboxClearPass";
			m_skyboxPass = RenderPass(info);

			Invalidate(fb);
		}
		void Execute(const Camera& camera, Framebuffer& fb) override
		{
			if (camera.clearMode == ClearMode::SolidColor) {
				m_renderPass.State().clearState[0].value = camera.solidColor;
				m_renderPass.BeginRenderPass(&fb);

				m_renderPass.EndRenderPass();
			}
			else 
			{
				m_skyboxPass.BeginRenderPass(&fb);

				DrawCommands::BindVertexArray(Primitives::Cube()->VAO()->Handle());
				DrawCommands::UseShaderProgram(m_defaultSkyboxPipeline->Handle());
				DrawCommands::UseTexture(camera.skybox ? camera.skybox->Handle() : s_whiteTexture->Handle(), 0, TextureDimension::TextureCube);
				DrawCommands::DrawElementsInstancedBaseInstance(PrimitiveType::TRIANGLES, Primitives::Cube()->IndexCount(), IndexBufferElement::UNSIGNED_INT, nullptr, 1, 0);
				m_skyboxPass.EndRenderPass();
			}
		}
		void Clear() override {  }
		void Invalidate(const Framebuffer& fb) override 
		{
			SUSHResult result = SUSHUtility::Process("drags/drags_skybox.sush");
			PipelineCreateInfo pinfo{};
			for (auto& stage : result.stages)
			{
				PipelineStageCreateInfo pi{};
				pi.source = stage.second;
				pi.stage = (PipelineStageFlagBits)stage.first;
				pinfo.stages.push_back(pi);
			}
			if (m_defaultSkyboxPipeline) Resources::Destroy(m_defaultSkyboxPipeline);
			m_defaultSkyboxPipeline = GraphicsPipeline::Create(pinfo);
		}
		~ClearPass() { Resources::Destroy(m_defaultSkyboxPipeline); }
	private:
		RenderPass m_skyboxPass;
		Ref<GraphicsPipeline> m_defaultSkyboxPipeline;
	};
	class OpaquePass : public DragsRenderPass
	{
	public:
		OpaquePass(const Framebuffer& fb, const std::string_view name)
		{
			RenderPassCreateInfo info
			{
				.state = RenderPassState
				{
					.enableDepth = true,
					.depthState = RenderPassDepthState
					{
						.depthFunction = DepthFunc::Less,
						.depthWrite = true
					}
				},
				.fbSpec = &fb.Specification(),
				.debugName = name.data()
			};
			m_renderPass = RenderPass(info);
		}
		void Execute(const Camera& camera, Framebuffer& fb) override
		{
			m_renderPass.BeginRenderPass(&fb);
			for(auto& cmd : m_commands) cmd();
			m_renderPass.EndRenderPass();
		}
		void Clear() override { m_commands.clear(); }
		void Invalidate(const Framebuffer& fb) override {  }
	};
	class CompositePass : public DragsRenderPass 
	{
	public:
		CompositePass(const Framebuffer& fb) 
		{
			Invalidate(fb);
		}
		void Execute(const Camera& camera, Framebuffer& fb) override 
		{
			DrawCommands::UseShaderProgram(m_compositePipeline->Handle());
			DrawCommands::UseImage(m_outputTexture->Handle(), 5, 0, false, 0, TextureAccess::WriteOnly, m_outputTexture->Info().format);
			
			auto& a = fb.Attachments();
			for (uint32_t i = 0; i < a.size(); ++i) DrawCommands::UseTexture(a[i].texture->Handle(), i, TextureDimension::Texture2D);

			DrawCommands::DispatchCompute(m_invX, m_invY, 1);
			DrawCommands::MemoryBarrier(MemoryBarrierBits::ShaderImageAccess);

			auto& source = m_outputTexture;
			auto& dest = fb.GetAttachment(0)->texture;

			DrawCommands::CopyImageSubData(TextureDimension::Texture2D,
				source->Handle(),
				0, 0, 0,
				dest->Handle(),
				0, 0, 0,
				source->Width(),
				source->Height(),
				source->Depth());
		}
		~CompositePass() 
		{
			Resources::Destroy(m_outputTexture);
			Resources::Destroy(m_compositePipeline);
		}
		void Clear() override { }
		void Invalidate(const Framebuffer& fb) override 
		{
			const FramebufferSpecification& fbSpec = fb.Specification();
			glm::uvec2 fbSize = { fbSpec.width, fbSpec.height };

			auto& device_spec = GraphicsContext()->Device().Specification();

			uint32_t max_invocations = device_spec.maxComputeWorkGroupInvocations;

			uint32_t group_size_x = 64;
			uint32_t group_size_y = 64;
			while (group_size_x * group_size_y > max_invocations) //do this for now to maintain group symmetry
			{
				group_size_x /= 2;
				group_size_y /= 2;
			}


			uint32_t invocations_x = std::ceil(fbSize.x / (float)group_size_x);
			uint32_t invocations_y = std::ceil(fbSize.y / (float)group_size_y);

			SUSHResult res = SUSHUtility::Process("drags/drags_composite_compute.sush");
			PipelineCreateInfo info{};
			for (auto& [stage, source] : res.stages)
			{
				auto& s = info.stages.emplace_back();
				s.source = source;
				s.stage = (PipelineStageFlagBits)stage;
			}
			std::string x_def = std::format("GROUP_SIZE_X {}", group_size_x);
			std::string y_def = std::format("GROUP_SIZE_Y {}", group_size_y);
			info.defines =
			{
				x_def,
				y_def,
				"OUTPUT_FORMAT rgba16f"
			};
			if (m_compositePipeline) Resources::Destroy(m_compositePipeline);
			m_compositePipeline = ComputePipeline::Create(info);
			m_invX = invocations_x;
			m_invY = invocations_y;

			auto& textureSpec = fb.GetAttachment(0)->texture->Info();
			if (m_outputTexture) Resources::Destroy(m_outputTexture);
			m_outputTexture = Texture2D::Create(textureSpec, nullptr, TextureFormat::RGBA, TextureFormatComponent::UNSIGNED_BYTE);
		}
	private:
		Ref<ComputePipeline> m_compositePipeline;
		Ref<Texture2D> m_outputTexture;
		uint32_t m_invX, m_invY;
	};

	DragsRenderer::DragsRenderer(const RendererSpecification& spec) : Renderer(spec)
	{
		FramebufferSpecification fbSpec{};
		fbSpec.width = spec.resolution.x;
		fbSpec.height = spec.resolution.y;
		fbSpec.layers = 1;
		fbSpec.depth = 1;
		fbSpec.dimension = TextureDimension::Texture2D;

		FramebufferAttachmentInfo att{};

		att.magFilter = TextureFilter::LINEAR;
		att.minFilter = TextureFilter::LINEAR;
		//color
		att.name = "color";
		att.attachmentType = FramebufferAttachmentType::Color;
		att.format = TextureFormat::RGBA16F; //HDR color
		fbSpec.attachments.push_back(att);
		
		att.magFilter = TextureFilter::NEAREST;
		att.minFilter = TextureFilter::NEAREST;

		//normal
		att.name = "normal";
		att.format = TextureFormat::RGB16F;
		fbSpec.attachments.push_back(att);

		//position
		att.name = "position";
		att.format = TextureFormat::RGB16F;
		fbSpec.attachments.push_back(att);

		//G-buffer
		att.name = "gbuffer";
		att.format = TextureFormat::RGBA8;
		//R - metallic -> 8bit
		//G - roughness -> 8bit
		//B - ao -> 8bit
		//A - deferred stencil
		fbSpec.attachments.push_back(att);

		//depth
		att.name = "depth";
		att.attachmentType = FramebufferAttachmentType::Depth;
		att.format = TextureFormat::DEPTH_COMPONENT;
		fbSpec.attachments.push_back(att);

		m_framebuffer = Framebuffer::Create(fbSpec);
		Framebuffer& fb = *m_framebuffer.get();

		size_t material_buffer_size = DRAGS_MAX_MATERIAL_COUNT * sizeof(drags_material);
		size_t instance_buffer_size = DRAGS_MAX_INSTANCE_COUNT * sizeof(drags_instance_data);
		size_t camera_buffer_size = sizeof(drags_camera_data);
		size_t enviorment_buffer_size = sizeof(drags_enviorment);

		m_instanceBuffer = ShaderStorageBuffer::Create(instance_buffer_size, BufferUsage::DYNAMIC, 0);
		m_materialBuffer = ShaderStorageBuffer::Create(material_buffer_size, BufferUsage::DYNAMIC, 1);
		m_enviormentBuffer = ShaderStorageBuffer::Create(enviorment_buffer_size, BufferUsage::DYNAMIC, 2);
		m_cameraBuffer = UniformBuffer::Create(camera_buffer_size, BufferUsage::DYNAMIC, 0);

		if (!s_whiteTexture) {
			TextureCreateInfo whiteTexInfo{};
			whiteTexInfo.width = 1;
			whiteTexInfo.height = 1;
			whiteTexInfo.format = TextureFormat::RGBA8;
			float whiteData[] = { 1.0f, 1.0f, 1.0f, 1.0f };
			s_whiteTexture = Texture2D::Create(whiteTexInfo, whiteData, TextureFormat::RGBA, TextureFormatComponent::FLOAT);

			TextureCreateInfo normalTexInfo{};
			normalTexInfo.width = 1;
			normalTexInfo.height = 1;
			normalTexInfo.format = TextureFormat::RGB8;
			float normalData[] = { 0.5f, 0.5f, 1.0f };
			s_normalTexture = Texture2D::Create(normalTexInfo, normalData, TextureFormat::RGB, TextureFormatComponent::FLOAT);
		}

		InvalidatePipelines();

		m_renderPasses.push_back(new ClearPass(fb));
		m_renderPasses.push_back(new OpaquePass(fb, "BeforeOpaque"));
		m_renderPasses.push_back(new OpaquePass(fb, "Opaque"));
		m_renderPasses.push_back(new OpaquePass(fb, "AfterOpaque"));
		m_renderPasses.push_back(new CompositePass(fb));
	}

	DragsRenderer::~DragsRenderer()
	{
		if (m_defaultLitPipeline) Resources::Destroy(m_defaultLitPipeline);
	}

	void DragsRenderer::Invalidate() 
	{
		InvalidatePipelines();
		Framebuffer& fb = *m_framebuffer.get();
		for (auto& pass : m_renderPasses) { pass->Clear();  pass->Invalidate(fb); }
	}

	void DragsRenderer::InvalidatePipelines()
	{
		SUSHResult result = SUSHUtility::Process("drags/drags_opaque_lit.sush");
		PipelineCreateInfo info{};
		for (auto& stage : result.stages)
		{
			PipelineStageCreateInfo pi{};
			pi.source = stage.second;
			pi.stage = (PipelineStageFlagBits)stage.first;
			info.stages.push_back(pi);
		}
		if (m_defaultLitPipeline) Resources::Destroy(m_defaultLitPipeline);
		m_defaultLitPipeline = GraphicsPipeline::Create(info);
	}
	void DragsRenderer::Begin(const Camera& camera)
	{
		Framebuffer& fb = *m_framebuffer.get();
		for (auto& pass : m_renderPasses) pass->Clear();
		m_camera = camera;
		m_instanceCursor = 0;
		m_materialCursor = 0;
	}
	void DragsRenderer::End()
	{
		Framebuffer& fb = *m_framebuffer.get();
		drags_camera_data cameraData{};
		cameraData.view = m_camera.View();
		cameraData.projection = m_camera.Projection();
		cameraData.invView = glm::inverse(cameraData.view);
		cameraData.invProjection = glm::inverse(cameraData.projection);
		m_cameraBuffer->SubData(&cameraData, 0, sizeof(drags_camera_data));
		for(auto& pass : m_renderPasses) pass->Execute(m_camera, fb);
	}

	void DragsRenderer::DrawMesh(const Ref<Mesh>& mesh, const glm::mat4& transform, const DragsMaterial& material, DragsRenderStep step, int32_t submesh)
	{
		if (!mesh) return;
		if (submesh >= (int32_t)mesh->SubmeshCount()) return;
		drags_instance_data instanceData{};
		instanceData.transform = transform;
		instanceData.materialIndex = m_materialCursor;
		m_instanceBuffer->SubData(&instanceData, sizeof(drags_instance_data) * m_instanceCursor, sizeof(drags_instance_data));
		drags_material matData
		{
			.albedo = material.albedo,
			.albedoMap = 0,
			.normalMap = 1,
			.metallic = material.metallic,
			.metallicMap = 2,
			.roughness = material.roughness,
			.roughnessMap = 3,
			.ao = material.ao,
			.aoMap = 4
		};

		m_materialBuffer->SubData(&matData, sizeof(drags_material) * m_materialCursor, sizeof(drags_material));
		uint32_t baseInstance = m_instanceCursor++;
		++m_materialCursor;
		DragsRenderCmd cmd = [=]()
			{
				DrawCommands::UseShaderProgram(m_defaultLitPipeline->Handle());
				DrawCommands::BindVertexArray(mesh->VAO()->Handle());

				DrawCommands::UseTexture(material.albedoMap ? material.albedoMap->Handle() : s_whiteTexture->Handle(), 0, TextureDimension::Texture2D);
				DrawCommands::UseTexture(material.normalMap ? material.normalMap->Handle() : s_normalTexture->Handle(), 1, TextureDimension::Texture2D);
				DrawCommands::UseTexture(material.metallicMap ? material.metallicMap->Handle() : s_whiteTexture->Handle(), 2, TextureDimension::Texture2D);
				DrawCommands::UseTexture(material.roughnessMap ? material.roughnessMap->Handle() : s_whiteTexture->Handle(), 3, TextureDimension::Texture2D);
				DrawCommands::UseTexture(material.aoMap ? material.aoMap->Handle() : s_whiteTexture->Handle(), 4, TextureDimension::Texture2D);

				if (submesh == -1)
				{
					DrawCommands::DrawElementsInstancedBaseInstance(
						PrimitiveType::TRIANGLES,
						mesh->IndexCount(),
						IndexBufferElement::UNSIGNED_INT,
						nullptr,
						1,
						baseInstance
					);
				}
				else
				{
					auto& m = mesh->GetSubmesh(submesh);
					DrawCommands::DrawElementsInstancedBaseVertexBaseInstance(
						PrimitiveType::TRIANGLES,
						m.size,
						IndexBufferElement::UNSIGNED_INT,
						nullptr,
						1,
						m.offset,
						baseInstance
					);
				}
			};
		m_renderPasses[static_cast<size_t>(step)]->SubmitCommand(cmd);
	}
	void DragsRenderer::SetEnviorment(const DragsEnviorment& enviorment)
	{
		drags_enviorment env
		{
			.sunDirection = enviorment.sunDirection,
			.sunColor = enviorment.sunColor
		};
		m_enviormentBuffer->SubData(&env, 0, sizeof(env));
	}
	const Ref<Framebuffer> DragsRenderer::OutputFramebuffer() { return m_framebuffer; }
}