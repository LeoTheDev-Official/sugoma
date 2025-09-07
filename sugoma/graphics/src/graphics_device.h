#pragma once
#include <glm/glm.hpp>
#include <string_view>
#include <cstdint>
namespace sugoma::graphics 
{
	struct GraphicsDeviceSpecification {
		char vendor[128] = { 0 };
		char renderer[128] = { 0 };
		char version[64] = { 0 };
		char shadingLanguageVersion[64] = { 0 };

		glm::ivec3 maxTextureSize;
		int32_t maxArrayTextureLayers;
		int32_t maxCubeMapTextureSize;
		int32_t maxRectangleTextureSize;

		int32_t maxRenderbufferSize;

		int32_t maxColorAttachments;
		int32_t maxDrawBuffers;

		int32_t maxSamples;

		int32_t maxUniformBlockSize;
		int32_t maxUniformBufferBindings;

		uint64_t maxShaderStorageBlockSize;
		int32_t maxShaderStorageBufferBindings;

		int32_t maxAtomicCounterBuffers;
		int32_t maxAtomicCounterBufferSize;

		int32_t maxVertexAttribs;
		int32_t maxVertexAttribBindings;
		int32_t maxVertexAttribStride;

		int32_t maxTextureImageUnits;
		int32_t maxCombinedTextureImageUnits;
		int32_t maxVertexTextureImageUnits;
		int32_t maxGeometryTextureImageUnits;
		int32_t maxFragmentTextureImageUnits;
		int32_t maxComputeTextureImageUnits;

		int32_t maxTextureCoords;

		glm::ivec3 maxComputeWorkGroupSize;
		glm::ivec3 maxComputeWorkGroupCount;
		int32_t maxComputeWorkGroupInvocations;

		glm::ivec2 maxViewportDims;

		int32_t maxClipDistances;

		int32_t maxPatchVertices;

		int32_t maxVertexUniformComponents;
		int32_t maxFragmentUniformComponents;
		int32_t maxGeometryUniformComponents;

		int32_t maxVaryingComponents;

		bool supportsComputeShader;
		bool supportsTessellation;
		bool supportsGeometryShader;
		bool supportsTextureFloat;
		bool supportsTextureFloatLinear;
		bool supportsAnisotropicFiltering;
		float maxAnisotropy;

		uint64_t maxUniformBufferSize64;
		uint64_t maxShaderStorageBufferSize64;
	};
	class GraphicsDevice 
	{
	public:
		GraphicsDevice() = default;
		void Fetch();
		const GraphicsDeviceSpecification& Specification() const;
		const std::string_view Vendor() const;
		const std::string_view Renderer() const;
		const std::string_view Version() const;
		const std::string_view GLSLVersion() const;
	private:
		GraphicsDeviceSpecification m_spec;
	};
}