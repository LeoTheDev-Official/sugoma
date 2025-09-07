#pragma once
#include <functional>
#include "renderer/renderer.h"
#include "mesh/mesh.h"
#include "buffers/shader_storage_buffer.h"
#include "buffers/uniform_buffer.h"
#include "pipeline/graphics_pipeline.h"
namespace sugoma 
{
	struct DragsMaterial 
	{
		glm::vec4 albedo = glm::vec4(1);
		Ref<Texture2D> albedoMap;
		Ref<Texture2D> normalMap;
		float metallic = 0.1f;
		Ref<Texture2D> metallicMap;
		float roughness = 0.1f;
		Ref<Texture2D> roughnessMap;
		float ao = 0.1f;
		Ref<Texture2D> aoMap;
	};
	struct DragsEnviorment 
	{
		glm::vec3 sunDirection;
		glm::vec3 sunColor;
	};
	enum class DragsRenderStep 
	{
		Clear,
		BeforeOpaque,
		Opaque,
		AfterOpaque,
		BeforeTransparent,
		Transparent,
		AfterTransparent,
		PostProcess,
		Count
	};
	class DragsRenderPass;
	class DragsRenderer : public Renderer 
	{
	public:
		DragsRenderer(const RendererSpecification& spec);
		~DragsRenderer();

		void InvalidatePipelines();
		void Invalidate();

		void Begin(const Camera& camera) override;
		void End() override;

		void DrawMesh(const Ref<Mesh>& mesh, const glm::mat4& transform, const DragsMaterial& material, DragsRenderStep step, int32_t submesh = -1);
		

		void SetEnviorment(const DragsEnviorment& enviorment);
	public:
		const Ref<Framebuffer> OutputFramebuffer();
	private:
		Camera m_camera;
		Ref<Framebuffer> m_framebuffer;
		std::vector<DragsRenderPass*> m_renderPasses;

		Ref<ShaderStorageBuffer> m_materialBuffer;
		Ref<ShaderStorageBuffer> m_instanceBuffer;
		Ref<ShaderStorageBuffer> m_enviormentBuffer;
		Ref<UniformBuffer> m_cameraBuffer;

		Ref<GraphicsPipeline> m_defaultLitPipeline;

		uint32_t m_instanceCursor = 0;
		uint32_t m_materialCursor = 0;
	};
}