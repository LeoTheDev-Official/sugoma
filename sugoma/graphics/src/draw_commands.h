#pragma once
#include "buffers/vertex_array_object.h"
#include "buffers/vertex_buffer.h"
#include "buffers/index_buffer.h"
#include "sugoma_gl.h"
#include "textures/texture.h"

namespace sugoma::graphics
{
	class DrawCommands
	{
	public:
		static void BindVertexArray(GLHandle vao);
		static void UseTexture(GLHandle texture, uint32_t slot, TextureDimension dim);
		static void UseImage(GLHandle handle, uint32_t unit, uint32_t level, bool layered, uint32_t layer, TextureAccess access, TextureFormat format);
		static void UseShaderProgram(GLHandle program);
		static void DispatchCompute(uint32_t x, uint32_t y, uint32_t z);
		static void MemoryBarrier(MemoryBarrierBits barriers);
		static void SubmitBuffers(); //TODO : Research how this thing works
		//static void Blit(GLHandle source, BlitRegion sourceRegion, GLHandle dest, BlitRegion destRegion, TextureFilter filter);
		static void CopyImageSubData(TextureDimension dimension, GLHandle source, uint32_t s_ox, uint32_t s_oy, uint32_t s_oz, GLHandle destination, uint32_t d_ox, uint32_t d_oy, uint32_t d_oz, uint32_t width, uint32_t height, uint32_t depth);
		static void DrawElementsInstancedBaseVertexBaseInstance(
			PrimitiveType primitive,
			uint32_t index_count,
			IndexBufferElement elements,
			const void* indices,
			uint32_t instance_count,
			uint32_t base_vertex,
			uint32_t base_instance);
		static void DrawElementsInstancedBaseInstance(
			PrimitiveType primitive,
			uint32_t index_count,
			IndexBufferElement elements,
			const void* indices,
			uint32_t instance_count,
			uint32_t base_instance);
	};
};