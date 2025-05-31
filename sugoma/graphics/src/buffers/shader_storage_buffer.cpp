#include "shader_storage_buffer.h"

#include <GL/glew.h>

namespace sugoma::graphics 
{
	ShaderStorageBuffer::ShaderStorageBuffer(size_t size, BufferUsage usage, uint32_t bind_point) : Buffer(GL_SHADER_STORAGE_BUFFER, size, nullptr, usage), m_bind_point(bind_point)
	{
		Use(bind_point);
	}
	ShaderStorageBuffer::~ShaderStorageBuffer()
	{
		GLHandle b;
		glGetIntegeri_v(GL_SHADER_STORAGE_BUFFER, m_bind_point, (GLint*)&b);
		if (b == m_handle)
			glBindBufferBase(GL_SHADER_STORAGE_BUFFER, m_bind_point, 0);
	}
	uint32_t ShaderStorageBuffer::BindPoint() const { return m_bind_point; }
	void ShaderStorageBuffer::Use(uint32_t bind)
	{
		glBindBuffer(GL_UNIFORM_BUFFER, m_handle);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, bind, m_handle);
		m_bind_point = bind;
	}
}