#include "GL/glew.h"
#include "draw_commands.h"

namespace sugoma::graphics
{
	void DrawCommands::BindVertexArray(GLHandle vao) { glBindVertexArray(vao); }
	void DrawCommands::UseTexture(GLHandle texture, uint32_t slot, TextureDimension dim)
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		switch (dim)
		{
		case TextureDimension::Texture1D:
			glBindTexture(GL_TEXTURE_1D, texture);
			break;
		case TextureDimension::Texture2D:
			glBindTexture(GL_TEXTURE_2D, texture);
			break;
		case TextureDimension::Texture3D:
			glBindTexture(GL_TEXTURE_3D, texture);
			break;
		case TextureDimension::TextureCube:
			glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
			break;
		}
	}
    void DrawCommands::UseImage(GLHandle handle, uint32_t unit, uint32_t level, bool layered, uint32_t layer, TextureAccess access, TextureFormat format)
    {
		GLEnum values[] = { GL_READ_ONLY, GL_WRITE_ONLY, GL_READ_WRITE };
		glBindImageTexture(unit, handle, level, layered, layer, values[(uint32_t)access], format);
    }
	void DrawCommands::UseShaderProgram(GLHandle program) { glUseProgram(program); }
	void DrawCommands::DispatchCompute(uint32_t x, uint32_t y, uint32_t z)
	{
		glDispatchCompute(x, y, z);
	}
	void DrawCommands::MemoryBarrier(MemoryBarrierBits barriers)
	{
		glMemoryBarrier((uint32_t)barriers);
	}

	void DrawCommands::SubmitBuffers()
	{
		glFinish();
	}

	void DrawCommands::CopyImageSubData(TextureDimension dimension, GLHandle source, uint32_t s_ox, uint32_t s_oy, uint32_t s_oz, GLHandle destination, uint32_t d_ox, uint32_t d_oy, uint32_t d_oz, uint32_t width, uint32_t height, uint32_t depth) 
	{
		GLEnum values[] = { GL_TEXTURE_1D, GL_TEXTURE_2D, GL_TEXTURE_3D, GL_TEXTURE_CUBE_MAP };
		glCopyImageSubData(source, values[(uint32_t)dimension], 0, s_ox, s_oy, s_oz, destination, values[(uint32_t)dimension], 0, d_ox, d_oy, d_oz, width, height, depth);
	}

	void DrawCommands::DrawElementsInstancedBaseVertexBaseInstance(
		PrimitiveType primitive,
		uint32_t index_count,
		IndexBufferElement elements,
		const void* indices,
		uint32_t instance_count,
		uint32_t base_vertex,
		uint32_t base_instance)
	{
		glDrawElementsInstancedBaseVertexBaseInstance(
			static_cast<GLenum>(primitive),
			index_count,
			static_cast<GLenum>(elements),
			indices,
			instance_count,
			base_vertex,
			base_instance);
	}
	void DrawCommands::DrawElementsInstancedBaseInstance(PrimitiveType primitive, uint32_t index_count, IndexBufferElement elements, const void* indices, uint32_t instance_count, uint32_t base_instance)
	{
		glDrawElementsInstancedBaseInstance(
			static_cast<GLenum>(primitive),
			index_count,
			static_cast<GLenum>(elements),
			indices,
			instance_count,
			base_instance);
	}
}