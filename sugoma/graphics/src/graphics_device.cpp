#include "graphics_device.h"
#include <GL/glew.h>
#include <string>
namespace sugoma::graphics 
{
	void GraphicsDevice::Fetch()
	{
		const uint8_t* str = nullptr;

		str = glGetString(GL_VENDOR);
		if (str) strncpy_s(m_spec.vendor, reinterpret_cast<const char*>(str), sizeof(m_spec.vendor));

		str = glGetString(GL_RENDERER);
		if (str) strncpy_s(m_spec.renderer, reinterpret_cast<const char*>(str), sizeof(m_spec.renderer));

		str = glGetString(GL_VERSION);
		if (str) strncpy_s(m_spec.version, reinterpret_cast<const char*>(str), sizeof(m_spec.version));

		str = glGetString(GL_SHADING_LANGUAGE_VERSION);
		if (str) strncpy_s(m_spec.shadingLanguageVersion, reinterpret_cast<const char*>(str), sizeof(m_spec.shadingLanguageVersion));
		m_spec.shadingLanguageVersion[sizeof(m_spec.shadingLanguageVersion) - 1] = '\0';

		// Texture sizes
		GLint val;
		glGetIntegerv(GL_MAX_TEXTURE_SIZE, &val);
		m_spec.maxTextureSize.x = val;
		m_spec.maxTextureSize.y = val; // usually square max for 2D
		glGetIntegerv(GL_MAX_3D_TEXTURE_SIZE, &val);
		m_spec.maxTextureSize.z = val;

		glGetIntegerv(GL_MAX_ARRAY_TEXTURE_LAYERS, &m_spec.maxArrayTextureLayers);
		glGetIntegerv(GL_MAX_CUBE_MAP_TEXTURE_SIZE, &m_spec.maxCubeMapTextureSize);
		glGetIntegerv(GL_MAX_RECTANGLE_TEXTURE_SIZE, &m_spec.maxRectangleTextureSize);
		glGetIntegerv(GL_MAX_RENDERBUFFER_SIZE, &m_spec.maxRenderbufferSize);

		// Framebuffer
		glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &m_spec.maxColorAttachments);
		glGetIntegerv(GL_MAX_DRAW_BUFFERS, &m_spec.maxDrawBuffers);

		// MSAA
		glGetIntegerv(GL_MAX_SAMPLES, &m_spec.maxSamples);

		// Uniform buffers
		glGetIntegerv(GL_MAX_UNIFORM_BLOCK_SIZE, &val);
		m_spec.maxUniformBlockSize = val;
		glGetIntegerv(GL_MAX_UNIFORM_BUFFER_BINDINGS, &val);
		m_spec.maxUniformBufferBindings = val;

		// Shader storage buffers (requires OpenGL 4.3+)
		GLint64 val64 = 0;
		glGetInteger64v(GL_MAX_SHADER_STORAGE_BLOCK_SIZE, &val64);
		m_spec.maxShaderStorageBlockSize = val64;
		glGetIntegerv(GL_MAX_SHADER_STORAGE_BUFFER_BINDINGS, &val);
		m_spec.maxShaderStorageBufferBindings = val;

		// Atomic counters
		glGetIntegerv(GL_MAX_ATOMIC_COUNTER_BUFFER_BINDINGS, &m_spec.maxAtomicCounterBuffers);
		glGetIntegerv(GL_MAX_ATOMIC_COUNTER_BUFFER_SIZE, &m_spec.maxAtomicCounterBufferSize);
		// Vertex attribs
		glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &m_spec.maxVertexAttribs);
		glGetIntegerv(GL_MAX_VERTEX_ATTRIB_BINDINGS, &m_spec.maxVertexAttribBindings);
		glGetIntegerv(GL_MAX_VERTEX_ATTRIB_STRIDE, &m_spec.maxVertexAttribStride);

		// Texture units
		glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &m_spec.maxTextureImageUnits);
		glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &m_spec.maxCombinedTextureImageUnits);
		glGetIntegerv(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS, &m_spec.maxVertexTextureImageUnits);
		glGetIntegerv(GL_MAX_GEOMETRY_TEXTURE_IMAGE_UNITS, &m_spec.maxGeometryTextureImageUnits);
		glGetIntegerv(GL_MAX_TEXTURE_UNITS, &m_spec.maxFragmentTextureImageUnits);
		glGetIntegerv(GL_MAX_COMPUTE_TEXTURE_IMAGE_UNITS, &m_spec.maxComputeTextureImageUnits);

		// Texture coords
		glGetIntegerv(GL_MAX_TEXTURE_COORDS, &m_spec.maxTextureCoords);

		// Compute work groups
		GLint cwgs[3];
		glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 0, &cwgs[0]);
		glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 1, &cwgs[1]);
		glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 2, &cwgs[2]);
		m_spec.maxComputeWorkGroupSize = glm::ivec3(cwgs[0], cwgs[1], cwgs[2]);

		GLint cwgc[3];
		glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 0, &cwgc[0]);
		glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 1, &cwgc[1]);
		glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 2, &cwgc[2]);
		m_spec.maxComputeWorkGroupCount = glm::ivec3(cwgc[0], cwgc[1], cwgc[2]);

		glGetIntegerv(GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS, &m_spec.maxComputeWorkGroupInvocations);

		// Viewport dims
		GLint dims[2];
		glGetIntegerv(GL_MAX_VIEWPORT_DIMS, dims);
		m_spec.maxViewportDims = glm::ivec2(dims[0], dims[1]);

		// Clip distances
		glGetIntegerv(GL_MAX_CLIP_DISTANCES, &m_spec.maxClipDistances);

		// Tessellation
		glGetIntegerv(GL_MAX_PATCH_VERTICES, &m_spec.maxPatchVertices);

		// Uniform components
		glGetIntegerv(GL_MAX_VERTEX_UNIFORM_COMPONENTS, &m_spec.maxVertexUniformComponents);
		glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_COMPONENTS, &m_spec.maxFragmentUniformComponents);
		glGetIntegerv(GL_MAX_GEOMETRY_UNIFORM_COMPONENTS, &m_spec.maxGeometryUniformComponents);

		// Varying components
		glGetIntegerv(GL_MAX_VARYING_COMPONENTS, &m_spec.maxVaryingComponents);

		// Extensions and features
		m_spec.supportsComputeShader = glewIsSupported("GL_ARB_compute_shader") || glewIsSupported("GL_VERSION_4_3");
		m_spec.supportsTessellation = glewIsSupported("GL_ARB_tessellation_shader") || glewIsSupported("GL_VERSION_4_0");
		m_spec.supportsGeometryShader = glewIsSupported("GL_ARB_geometry_shader4") || glewIsSupported("GL_VERSION_3_2");
		m_spec.supportsTextureFloat = glewIsSupported("GL_ARB_texture_float");
		m_spec.supportsTextureFloatLinear = glewIsSupported("GL_ARB_texture_float") && glewIsSupported("GL_ARB_texture_filter_linear");
		m_spec.supportsAnisotropicFiltering = glewIsSupported("GL_EXT_texture_filter_anisotropic");
		m_spec.maxAnisotropy = 1.0f;
		if (m_spec.supportsAnisotropicFiltering) {
			GLfloat maxAniso = 1.0f;
			glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAniso);
			m_spec.maxAnisotropy = maxAniso;
		}

		// 64-bit buffer sizes (if supported)
		GLint64 val64_ubo = 0, val64_ssbo = 0;
		glGetInteger64v(GL_MAX_UNIFORM_BLOCK_SIZE, &val64_ubo);
		glGetInteger64v(GL_MAX_SHADER_STORAGE_BLOCK_SIZE, &val64_ssbo);
		m_spec.maxUniformBufferSize64 = val64_ubo;
		m_spec.maxShaderStorageBufferSize64 = val64_ssbo;
	}
	const GraphicsDeviceSpecification& GraphicsDevice::Specification() const { return m_spec; }
	const std::string_view GraphicsDevice::Vendor() const { return m_spec.vendor; }
	const std::string_view GraphicsDevice::Renderer() const { return m_spec.renderer; }
	const std::string_view GraphicsDevice::Version() const { return m_spec.version; }
	const std::string_view GraphicsDevice::GLSLVersion() const { return m_spec.shadingLanguageVersion; }
}